#pragma once

#include "init.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include "CSocket.h"
#include "CHttp.h"

#pragma comment(lib, "ws2_32.lib")  /* WinSockʹ�õĿ⺯�� */
 


/* ���峣�� */
#define HTTP_DEF_PORT      8888     /* ���ӵ�ȱʡ�˿� */
#define HTTP_BUF_SIZE      2048     /* �������Ĵ�С */
#define HTTP_FILENAME_LEN   256     /* �ļ������� */
#define HTTP_HOME   "D:/HTTP"         /* Ŀ¼ */

