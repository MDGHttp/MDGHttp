#include <stdio.h>
#include <stdlib.h>

int fnGetCmdOut(char* cmd, char* result) {
	char buffer[128];                         //定义缓冲区                        
	FILE* pipe = _popen(cmd, "r");            //打开管道，并执行命令 
	if (!pipe)
		return 0;                      //返回0表示运行失败 

	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe)) {             //将管道输出到result中 
			strcat(result, buffer);
		}
	}
	_pclose(pipe);                            //关闭管道 
	return 1;                                 //返回1表示运行成功 
}

int main()
{
	FILE *fp,*fp1;
	char ch1 = '\0';
	if ((fp = fopen("D:\\qq.py", "r")) == NULL)      //判断文件是否存在及可读  
	{
		printf("Open Falied!");
		return -1;
	}
	if ((fp1 = fopen("D:\\qq1.py", "w")) == NULL)      //判断文件是否存在及可读  
	{
		printf("Open Falied! 111");
		return -1;
	}
	char b ='\0', e='\0';
	int i = 0,j=0;
	char pubuf[1024];
	while (!feof(fp))
	{
		ch1 = fgetc(fp);
		printf("%c",ch1);
		if (ch1 == '<')
		{
			ch1 = fgetc(fp);
			printf("%c", ch1);
			if (ch1 == '?')
			{
				i = 1;
			}
		}
		if (ch1 == '?')
		{
			ch1 = fgetc(fp);
			printf("%c", ch1);
			if (ch1 == '>')
			{
				i = 0;
			}
		}
		if ( i == 1 )
		{
			fputc(ch1,fp1);
			pubuf[j++] = ch1;
		}
	}
	printf("===========================================");
	pubuf[j] = '\0';
	printf("all==%s", pubuf);

	fclose(fp);   
	fclose(fp1);  //关闭文件  
	printf("===========================================\n");
	printf("===========================================\n");
	char result[1024] = "";                   //定义存放结果的字符串数组 
	if (1 == fnGetCmdOut("python D:\\qq1.py", result))
	printf("out==%s", result);
	printf("\n");
	system("pause");
	return 0;
}


int main1()
{
	FILE *fp;
	char strLine[256];                             //读取缓冲区  
	if ((fp = fopen("D:\\qq.py", "r")) == NULL)      //判断文件是否存在及可读  
	{
		printf("Open Falied!");
		return -1;
	}
	while (!feof(fp))                                   //循环读取每一行，直到文件尾  
	{
		fgets(strLine, 256, fp);                     //将fp所指向的文件一行内容读到strLine缓冲区 

		printf("%s", strLine);                          //输出所读到的内容  													//DO SOMETHING ELSE  
	}
	fclose(fp);                                         //关闭文件  
	printf("\n");
	system("pause");
	return 0;
}


