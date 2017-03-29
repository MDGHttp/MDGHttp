#pragma once


int MDGHttp();
int MDGHttp_Accept(int CSocket_srv_soc);
int MDGHttp_Resp(int CSocket_acpt_soc);
struct http_st_HttpReq http_fnGetHeaders(int CSocket_acpt_soc);
int http_fnSendHeaders(int client, char *ContentType);
void CSocket_fnClose(int CSocket);
char *CHttp_getType(const char *suf);
int get_line(int sock, char *buf, int size);
int HttpSendPhp(char *filename, FILE *res_file, char *type, int CSocket_acpt_soc);
int HttpSendPython(char *filename, FILE *res_file, char *type, int CSocket_acpt_soc);
