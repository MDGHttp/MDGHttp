#pragma once

#include "init.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include "CSocket.h"
#include "CHttp.h"

#pragma comment(lib, "ws2_32.lib")  /* WinSock使用的库函数 */
 


/* 定义常量 */
#define HTTP_DEF_PORT      8888     /* 连接的缺省端口 */
#define HTTP_BUF_SIZE      2048     /* 缓冲区的大小 */
#define HTTP_FILENAME_LEN   256     /* 文件名长度 */
#define HTTP_HOME   "D:/HTTP"         /* 目录 */

