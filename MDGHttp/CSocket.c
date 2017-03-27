#include "stdafx.h"

///������
struct sockaddr_in CSocket_serv_addr;   
struct sockaddr_in CSocket_from_addr;   
char CSocket_recv_buf[HTTP_BUF_SIZE];
unsigned short CSocket_port = HTTP_DEF_PORT;
int from_len = sizeof(CSocket_from_addr);
int result = 0, recv_len;
struct http_st_HttpReq CSocket_HttpReq;

/* �����ļ����Ͷ�Ӧ�� Content-Type */
struct doc_type
{
	char *suffix; /* �ļ���׺ */
	char *type;   /* Content-Type */
};

//**mime ����**/
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

int CSocket_fnInitSocket()
{
	WSADATA CSocket_wsa_data;
	WSAStartup(MAKEWORD(2, 0), &CSocket_wsa_data); /* ��ʼ�� WinSock ��Դ */

	SOCKET CSocket_srv_soc = socket(AF_INET, SOCK_STREAM, 0); /* ���� socket */
	if (CSocket_srv_soc == INVALID_SOCKET)
	{
		printf("[Web] WSAStartup error = %d\n", WSAGetLastError());
		return -1;
	}

	/* ��������ַ */
	CSocket_serv_addr.sin_family = AF_INET;
	CSocket_serv_addr.sin_port = htons(CSocket_port);
	CSocket_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(CSocket_srv_soc, (struct sockaddr *) &CSocket_serv_addr, sizeof(CSocket_serv_addr));
	if (result == SOCKET_ERROR) /* ��ʧ�� */
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
	//�ȴ�����
	SOCKET CSocket_acpt_soc = accept(CSocket_srv_soc, (struct sockaddr *) &CSocket_from_addr, &from_len);
	if (CSocket_acpt_soc == INVALID_SOCKET) 
	{
		printf("[Web] ����ʧ�� �� %d\n", WSAGetLastError());
		return -1;
	}

	printf("[Web] ���ӳɹ�:[%s], �˿�:[%d]\n",
		inet_ntoa(CSocket_from_addr.sin_addr), ntohs(CSocket_from_addr.sin_port));
	return CSocket_acpt_soc;
}

/**
��
**/
int CSocket_fnRecvSocket(int CSocket_acpt_soc)
{
	CSocket_HttpReq = http_fnGetHeaders(CSocket_acpt_soc);

	int urlLen = strlen(CSocket_HttpReq.url);
	char read_buf[HTTP_BUF_SIZE];
	char filename[1024];
	FILE *res_file;
	strcpy(filename, HTTP_HOME);
	if (urlLen <= 1)
	{
		strcat(filename, "/index.html\0");
	}
	else
	{
		strcat(filename, CSocket_HttpReq.url);
		//strcat(filename, '\0');
	}
	res_file = fopen(filename, "rb+"); /* �ö����Ƹ�ʽ���ļ� */
	if (res_file == NULL)
	{
		printf("[Web] The file [%s] is not existed\n", filename);
		/*strcpy(filename, HTTP_HOME);
		strcat(filename, "/index.html\0");*/
		return 0;
	}
	printf("[Web] Open file [%s]\n", filename);
	fseek(res_file, 0, SEEK_END);
	int file_len = ftell(res_file);
	fseek(res_file, 0, SEEK_SET);
	//char read_buf[2048];
	int read_len = 0;
	/* ��ͻ��˷�����Ӧ���� */

	// ���� HTTP ��
	//��ȡmime����
	char *type;
	type = CHttp_getType(filename); /* �ļ���Ӧ�� Content-Type */
	if (type == NULL)
	{
		type = "text/html";
		//return -1;
	}

	int i = 0;
	i += http_fnSendHeaders(CSocket_acpt_soc, type);
	char rec[1024];
	int recv_len = recv(CSocket_acpt_soc, rec, 1024, 0);
	//
	int send_len = 0;
	do /* �����ļ�, HTTP ����Ϣ�� */
	{
		read_len = fread(read_buf, sizeof(char), HTTP_BUF_SIZE, res_file);

		if (read_len > 0)
		{
			send_len = send(CSocket_acpt_soc, read_buf, read_len, 0);
			file_len -= read_len;
		}
	} while ((read_len > 0) && (file_len > 0));

	fclose(res_file);
	closesocket(CSocket_acpt_soc);
	printf("[Web] closesocket\n");
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

	/*����ֹ����ͳһΪ \n ���з�����׼�� buf ����*/
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
		int i = Indexof(suf,type->suffix );
		if ( i >= 0)
			return type->type;
	}

	return NULL;
}

int Indexof(const char *pSrc, const char *pDst)
{
	int i, j;
	for (i = 0; pSrc[i] != '\0'; i++)
	{
		if (pSrc[i] != pDst[0])
			continue;
		j = 0;
		while (pDst[j] != '\0' && pSrc[i + j] != '\0')
		{
			j++;
			if (pDst[j] != pSrc[i + j])
				break;
		}
		if (pDst[j] == '\0')
			return i;
	}
	return -1;
}
