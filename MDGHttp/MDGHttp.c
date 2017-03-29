#include "stdafx.h"

///变量区
struct sockaddr_in CSocket_from_addr;

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
	{ "jpeg",    "image/jpeg" },//image/png css text/css
	{ "png",    "image/png" },
	{ "css",    "text/css" },
	{ "js",    "application/x-javascript" }, //js application/x-javascript
	{ NULL,      NULL }
};

int MDGHttp()
{
	WSADATA CSocket_wsa_data;
	WSAStartup(MAKEWORD(2, 0), &CSocket_wsa_data); /* 初始化 WinSock 资源 */

	SOCKET HttpServer = socket(AF_INET, SOCK_STREAM, 0); /* 创建 socket */
	if (HttpServer == INVALID_SOCKET)
	{
		printf("[Web] WSAStartup error = %d\n", WSAGetLastError());
		return -1;
	}
	struct sockaddr_in CSocket_serv_addr;
	/* 服务器地址 */
	CSocket_serv_addr.sin_family = AF_INET;
	CSocket_serv_addr.sin_port = htons(HTTP_DEF_PORT);
	CSocket_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int result = bind(HttpServer, (struct sockaddr *) &CSocket_serv_addr, sizeof(CSocket_serv_addr));
	if (result == SOCKET_ERROR) /* 绑定失败 */
	{
		closesocket(HttpServer);
		printf("[Web] bind error = %d\n", WSAGetLastError());
		return -1;
	}

	result = listen(HttpServer, SOMAXCONN);
	printf("[Web] MDGHttp Server Running ... ...\n");
	return HttpServer;
}

int MDGHttp_Accept(int CSocket_srv_soc)
{
	//等待介入
	int len = sizeof(CSocket_from_addr);
	SOCKET CSocket_acpt_soc = accept(CSocket_srv_soc, (struct sockaddr *)&CSocket_from_addr, &len);
	if (CSocket_acpt_soc == INVALID_SOCKET)
	{
		printf("[Web] 链接失败 ： %d\n", WSAGetLastError());
		return -1;
	}

	printf("[Web] 链接成功:[%s], 端口:[%d]\n",
		inet_ntoa(CSocket_from_addr.sin_addr), ntohs(CSocket_from_addr.sin_port));
	return CSocket_acpt_soc;
}

/**
重_____
**/
int MDGHttp_Resp(int CSocket_acpt_soc)
{
	//获取请求体信息
	struct http_st_HttpReq CSocket_HttpReq;
	CSocket_HttpReq = http_fnGetHeaders(CSocket_acpt_soc);

	int urlLen = strlen(CSocket_HttpReq.url);
	char read_buf[HTTP_BUF_SIZE];
	char filename[HTTP_FILENAME_LEN];
	FILE *res_file;
	strcpy(filename, HTTP_HOME);
	if (urlLen <= 1)
	{
		strcat(filename, "/index.html\0");
	}
	else
	{
		strcat(filename, CSocket_HttpReq.url);
	}
	res_file = fopen(filename, "rb+");
	if (res_file == NULL)
	{
		printf("[Web] The file [%s] is not existed\n", filename);
		return 0;
	}
	printf("[Web] Open file [%s]\n", filename);
	fseek(res_file, 0, SEEK_END);
	int file_len = ftell(res_file);
	fseek(res_file, 0, SEEK_SET);
	//char read_buf[2048];
	int read_len = 0;
	/* 向客户端发送响应数据 */

	// 构造 HTTP 首
	//获取mime类型
	char *type;
	type = CHttp_getType(filename); /* 文件对应的 Content-Type */
	if (type == NULL)
	{
		type = "text/html";
		//return -1;
		if (Indexof(filename, "py") >= 0)
		{
			char result[1024] = "";                   //定义存放结果的字符串数组 
			if (1 == fnGetCmdOut("python D:\\Http\\index.py", result)) {
				printf(result);
				char rec[HTTP_BUF_SIZE];
				int i = 0;
				i += http_fnSendHeaders(CSocket_acpt_soc, type);
				int recv_len = recv(CSocket_acpt_soc, rec, HTTP_BUF_SIZE, 0);
				int pylen = send(CSocket_acpt_soc, result, 1024, 0);
				pylen = send(CSocket_acpt_soc, "\r\n", 4, 0);
				fclose(res_file);
				closesocket(CSocket_acpt_soc);
				printf("[Web] closesocket\n");
				return 0;
			}
			//int pylen = send(CSocket_acpt_soc, result, read_len, 0);
		}
		type = "text/html";
	}

	int i = 0;
	i += http_fnSendHeaders(CSocket_acpt_soc, type);
	char rec[HTTP_BUF_SIZE];
	int recv_len = recv(CSocket_acpt_soc, rec, HTTP_BUF_SIZE, 0);
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
	send_len = send(CSocket_acpt_soc, "\r\n", strlen("\r\n"), 0);

	fclose(res_file);
	closesocket(CSocket_acpt_soc);
	printf("[Web] closesocket\n");
	return 0;
}

struct http_st_HttpReq http_fnGetHeaders(int CSocket_acpt_soc)
{
	struct http_st_HttpReq req;
	char buf[1024];// = '\0';
	int recv_len = 1;
	recv_len = get_line(CSocket_acpt_soc, buf, sizeof(buf));
	strcpy(req.header, buf);
	//printf("1=%s", buf);
	//int uu = get_line(CSocket_acpt_soc, buf, sizeof(buf));
	//printf("2=%s", buf);
	//int jjj = 3;
	//while (uu >= 0)
	//{
	//	if (jjj == 13)
	//	{
	//		int sdfa = 0;
	//		sdfa++;
	//	}
	//	uu = get_line(CSocket_acpt_soc, buf, sizeof(buf));
	//	printf("%d=%s",jjj++, buf);
	//}
	//uu = get_line(CSocket_acpt_soc, buf, sizeof(buf));
	//printf("%d=%s", 45, buf);
	//获取文件
	int ch = 0, j = 0;
	for (size_t i = 0; i < recv_len; i++)
	{
		if (req.header[i] == ' ')
		{
			ch++;
			continue;
		}
		if (ch == 1)
		{
			req.url[j++] = req.header[i];
		}
		if (ch > 1)
		{
			break;
		}
	}
	req.url[j] = '\0';
	return req;
}

int http_fnSendHeaders(int client, char *ContentType)
{
	//  Connection:keep - alive
	//	Content - Length : 252
	//	Content - Type : application / json; charset = utf - 8
	//	Date:Sun, 26 Mar 2017 05 : 55 : 07 GMT
	//	Server : Apache
	char buf[512];
	/*正常的 HTTP header */
	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	/*服务器信息*/
	strcpy(buf, "Server: MDGHttp1.0\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "Connection:keep-alive\r\n");//Referrer Policy:unsafe-url
	send(client, buf, strlen(buf), 0);
	char str[256] = "Content-Type: ";//: {0};charset=utf-8\r\n";
	strcat(str, ContentType);
	strcat(str, ";charset=utf-8\r\n");
	sprintf(buf, str);
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	return 0;
}

void CSocket_fnClose(int CSocket)
{
	closesocket(CSocket);
	WSACleanup();
	printf("[Web] stopped.\n");
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

char *CHttp_getType(const char *suf)
{
	struct doc_type *type;

	for (type = file_type; type->suffix; type++)
	{
		int i = Indexof(suf, type->suffix);
		if (i >= 0)
			return type->type;
	}

	return NULL;
}

