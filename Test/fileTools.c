#include <stdio.h>
#include <stdlib.h>

int fnGetCmdOut(char* cmd, char* result) {
	char buffer[128];                         //���建����                        
	FILE* pipe = _popen(cmd, "r");            //�򿪹ܵ�����ִ������ 
	if (!pipe)
		return 0;                      //����0��ʾ����ʧ�� 

	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe)) {             //���ܵ������result�� 
			strcat(result, buffer);
		}
	}
	_pclose(pipe);                            //�رչܵ� 
	return 1;                                 //����1��ʾ���гɹ� 
}

int main()
{
	FILE *fp,*fp1;
	char ch1 = '\0';
	if ((fp = fopen("D:\\qq.py", "r")) == NULL)      //�ж��ļ��Ƿ���ڼ��ɶ�  
	{
		printf("Open Falied!");
		return -1;
	}
	if ((fp1 = fopen("D:\\qq1.py", "w")) == NULL)      //�ж��ļ��Ƿ���ڼ��ɶ�  
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
	fclose(fp1);  //�ر��ļ�  
	printf("===========================================\n");
	printf("===========================================\n");
	char result[1024] = "";                   //�����Ž�����ַ������� 
	if (1 == fnGetCmdOut("python D:\\qq1.py", result))
	printf("out==%s", result);
	printf("\n");
	system("pause");
	return 0;
}


int main1()
{
	FILE *fp;
	char strLine[256];                             //��ȡ������  
	if ((fp = fopen("D:\\qq.py", "r")) == NULL)      //�ж��ļ��Ƿ���ڼ��ɶ�  
	{
		printf("Open Falied!");
		return -1;
	}
	while (!feof(fp))                                   //ѭ����ȡÿһ�У�ֱ���ļ�β  
	{
		fgets(strLine, 256, fp);                     //��fp��ָ����ļ�һ�����ݶ���strLine������ 

		printf("%s", strLine);                          //���������������  													//DO SOMETHING ELSE  
	}
	fclose(fp);                                         //�ر��ļ�  
	printf("\n");
	system("pause");
	return 0;
}


