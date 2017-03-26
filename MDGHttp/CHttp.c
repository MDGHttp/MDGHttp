#include "stdafx.h"

struct http_st_HttpReq http_fnGetHeaders(int CSocket_acpt_soc)
{
	struct http_st_HttpReq req;
	char buf[1024];// = '\0';
	int recv_len = 1;
	recv_len = get_line(CSocket_acpt_soc, buf,sizeof(buf));
	strcpy(req.header, buf);
	
	//获取文件
	int ch = 0,j=0;
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

int http_fnSendHeaders(int client,char *ContentType)
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

int http_fnSendContent(int client, char *buf, int buf_len)
{
	//int read_len, file_len, hdr_len, send_len;
	//char *type;
	//char read_buf[HTTP_BUF_SIZE];
	//char http_header[HTTP_BUF_SIZE];
	//char file_name[HTTP_FILENAME_LEN] = "D:\test.html", suffix[16] = "html";
	//FILE *res_file;

	///* 得到文件名和后缀 */
	////http_parse_request_cmd(buf, buf_len, file_name, suffix);

	//res_file = fopen("D:/test.html", "rb+"); /* 用二进制格式打开文件 */
	//if (res_file == NULL)
	//{
	//	printf("[Web] 文件不存在\n", file_name);
	//	return 0;
	//}

	//fseek(res_file, 0, SEEK_END);
	//file_len = ftell(res_file);
	//fseek(res_file, 0, SEEK_SET);

	//do 
	//{
	//	read_len = fread(read_buf, sizeof(char), HTTP_BUF_SIZE, res_file);

	//	if (read_len > 0)
	//	{
	//		send_len = send(client, read_buf, read_len, 0);
	//		file_len -= read_len;
	//	}
	//} while ((read_len > 0) && (file_len > 0));
	int send_len = send(client, buf, strlen(buf), 0);
	//fclose(res_file);
	return 0;
}