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
		int serverSocket = CSocket_fnInitSocket();
		
		while (1)
		{
			///改线程 TODO
			int rec = CSocket_fnAcceptSocket(serverSocket);
			CSocket_fnRecvSocket(rec);
			closesocket(rec);
		}
	}

	//system("pause");
	return 0;
	
}