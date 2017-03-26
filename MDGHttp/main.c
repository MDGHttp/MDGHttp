#include "stdafx.h"

/**
	main
	2017-02-25
	m
**/
int main(int argc, char *argv[])
{
	while (argc--> 1) 
	{
		printf("%s\n", *++argv);
		if (strcmp(argv,"server") == 0)
		{
			///TODO 服务模式启动

		}
	}	
	
	int i = init();
	//printf("%d \n" ,i);
	if (i == 0)
	{
		CSocket_fnInitSocket();
		while (CSocket_fnAcceptSocket() == 0)
		{
			///改线程
			CSocket_fnRecvSocket();
		}
	}

	//system("pause");
	return 0;
	
}