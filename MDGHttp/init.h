#pragma once


int init();

//

/*����ͷ������*/
struct http_st_HttpReq
{
	char header[10240];
	char url[1024];
	char get[4096];
	char post[10240];
	char type[2];
	int http_state;
};

