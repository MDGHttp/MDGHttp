#pragma once


int init();

//
char httpheader[1024];

/*����ͷ������*/
struct http_st_HttpReq
{
	char header[4096];
	char url[1024];
	char get[4096];
	char post[10240];
	char type[2];
	int http_state;
};
