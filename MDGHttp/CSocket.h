#pragma once

///º¯ÊýÇø
int CSocket_fnInitSocket();
int CSocket_fnAcceptSocket();
int CSocket_FnSend_response(SOCKET soc, char *buf, int buf_len);
void CSocket_fnClose();
int CSocket_fnRecvSocket();
int get_line(int sock, char *buf, int size);
