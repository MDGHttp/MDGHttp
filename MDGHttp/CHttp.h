#pragma once

int http_fnSendHeaders(int client);
int http_fnSendContent(int client, char *buf, int buf_len);
struct http_st_HttpReq http_fnGetHeaders(int CSocket_acpt_soc);

