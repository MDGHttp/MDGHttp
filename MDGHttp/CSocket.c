#include "stdafx.h"

///变量区
struct sockaddr_in CSocket_serv_addr;   
struct sockaddr_in CSocket_from_addr;   
char CSocket_recv_buf[HTTP_BUF_SIZE];
unsigned short CSocket_port = HTTP_DEF_PORT;
int from_len = sizeof(CSocket_from_addr);
int result = 0, recv_len;
struct http_st_HttpReq CSocket_HttpReq;

/* 定义文件类型对应的 Content-Type */
struct doc_type
{
	char *suffix; /* 文件后缀 */
	char *type;   /* Content-Type */
};

//**mime 类型**/
struct doc_type file_type[] =
{
	{ "html",    "text/html" },
	{ "gif",     "image/gif" },
	{ "jpeg",    "image/jpeg" },
	{ NULL,      NULL }
};

int CSocket_fnInitSocket()
{
	WSADATA CSocket_wsa_data;
	WSAStartup(MAKEWORD(2, 0), &CSocket_wsa_data); /* 初始化 WinSock 资源 */

	SOCKET CSocket_srv_soc = socket(AF_INET, SOCK_STREAM, 0); /* 创建 socket */
	if (CSocket_srv_soc == INVALID_SOCKET)
	{
		printf("[Web] WSAStartup error = %d\n", WSAGetLastError());
		return -1;
	}

	/* 服务器地址 */
	CSocket_serv_addr.sin_family = AF_INET;
	CSocket_serv_addr.sin_port = htons(CSocket_port);
	CSocket_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(CSocket_srv_soc, (struct sockaddr *) &CSocket_serv_addr, sizeof(CSocket_serv_addr));
	if (result == SOCKET_ERROR) /* 绑定失败 */
	{
		closesocket(CSocket_srv_soc);
		printf("[Web] bind error = %d\n", WSAGetLastError());
		return -1;
	}

	result = listen(CSocket_srv_soc, SOMAXCONN);
	printf("[Web] running ... ...\n");
	return CSocket_srv_soc;
}

int CSocket_fnAcceptSocket(int  CSocket_srv_soc)
{
	//等待介入
	SOCKET CSocket_acpt_soc = accept(CSocket_srv_soc, (struct sockaddr *) &CSocket_from_addr, &from_len);
	if (CSocket_acpt_soc == INVALID_SOCKET) 
	{
		printf("[Web] 链接失败 ： %d\n", WSAGetLastError());
		return -1;
	}

	printf("[Web] 链接成功:[%s], 端口:[%d]\n",
		inet_ntoa(CSocket_from_addr.sin_addr), ntohs(CSocket_from_addr.sin_port));
	return CSocket_acpt_soc;
}

int CSocket_fnRecvSocket(int CSocket_acpt_soc)
{
	CSocket_HttpReq = http_fnGetHeaders(CSocket_acpt_soc);

	int urlLen = strlen(CSocket_HttpReq.url);
	char read_buf[20480];
	char filename[1024];
	FILE *res_file;
	strcpy(filename, HTTP_HOME);
	if (urlLen == 1)
	{
		strcat(filename, "/index.html\0");
	}
	else
	{
		strcat(filename, CSocket_HttpReq.url);
		//strcat(filename, '\0');
	}
	res_file = fopen(filename, "rb+"); /* 用二进制格式打开文件 */
	if (res_file == NULL)
	{
		printf("[Web] The file [%s] is not existed\n", filename);
		/*strcpy(filename, HTTP_HOME);
		strcat(filename, "/index.html\0");*/
		return 0;
	}
	fseek(res_file, 0, SEEK_END);
	int file_len = ftell(res_file);
	fseek(res_file, 0, SEEK_SET);
	//char read_buf[2048];
	int read_len = fread(read_buf, sizeof(char), sizeof(read_buf), res_file);
	read_buf[--read_len] = '\0';
	/* 向客户端发送响应数据 */

	// 构造 HTTP 首
	int i = 0;
	i += http_fnSendHeaders(CSocket_acpt_soc);
	char rec[1024];
	int recv_len = recv(CSocket_acpt_soc, rec, 1024, 0);
	//
	int send_len = 0;
	do /* 发送文件, HTTP 的消息体 */
	{
		read_len = fread(read_buf, sizeof(char), HTTP_BUF_SIZE, res_file);

		if (read_len > 0)
		{
			send_len = send(CSocket_acpt_soc, read_buf, read_len, 0);
			file_len -= read_len;
		}
	} while ((read_len > 0) && (file_len > 0));

	fclose(res_file);

	/*int len1 = strlen(read_buf);
	result = CSocket_FnSend_response(CSocket_acpt_soc, read_buf, len-1);*/
	closesocket(CSocket_acpt_soc);
	return 0;
}

void CSocket_fnClose(int CSocket)
{
	closesocket(CSocket);
	WSACleanup();
	printf("[Web] stopped.\n");
}

///发送响应数据
int CSocket_FnSend_response(SOCKET soc, char *buf, int buf_len)
{
	// 构造 HTTP 首
	int i = 0;
	i+= http_fnSendHeaders(soc);
	char rec[1024];
	int recv_len = recv(soc, rec, 1024, 0);
	//
	i+= http_fnSendContent(soc,buf, buf_len);
	if (1 != 0)
	{
		//error 
		//TODO
	}
	return 1;
}


int get_line(int sock, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;

	/*把终止条件统一为 \n 换行符，标准化 buf 数组*/
	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0);
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';
	return(i);
}
