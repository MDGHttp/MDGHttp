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
			///TODO ����ģʽ����  TODO

		}
	}	
	
	int i = init();
	if (i == 0)
	{
		int serverSocket = MDGHttp();
		while (1)
		{
			///���߳�
			int rec = MDGHttp_Accept(serverSocket);
			HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, MDGHttp_Resp, rec, 0, NULL);
		}
	}
	return 0;
	
}