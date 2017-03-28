#include "stdafx.h"

/**
	main
	2017-03-25
	m
**/
int main(int argc, char *argv[])
{
	while (argc--> 1) 
	{
		printf("%s\n", *++argv);
		if (strcmp(argv,"server") == 0)
		{
			///TODO 服务模式启动  TODO

		}
	}	
	
	int i = init();
	if (i == 0)
	{
		int mHttpHandle = MDGHttp();
		while (1)
		{
			///改线程
			int mHttp_Rec = MDGHttp_Accept(mHttpHandle);
			HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, MDGHttp_Resp, mHttp_Rec, 0, NULL);
		}
	}
	return 0;	
}