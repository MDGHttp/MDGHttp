#pragma once

///º¯ÊýÇø
int CSocket_fnInitSocket();
int CSocket_fnAcceptSocket(int  CSocket_srv_soc);
//int CSocket_FnSend_response(SOCKET soc, char *buf, int buf_len);
void CSocket_fnClose(int  CSocket);
int CSocket_fnRecvSocket(int  CSocket_acpt_soc);
int get_line(int sock, char *buf, int size);
char *CHttp_getType(const char *suf);
int Indexof(const char *pSrc, const char *pDst);
