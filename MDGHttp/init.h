#pragma once


int init();

//
char httpheader[1024];

/*请求头的属性*/
struct http_st_HttpReq
{
	char *header;
	char *url;
	char *get;
	char *post;
	char *type;
	int http_state;
};
