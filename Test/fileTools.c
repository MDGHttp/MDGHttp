#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Indexof(const char *pSrc, const char *pDst)
{
	int i, j;
	for (i = 0; pSrc[i] != '\0'; i++)
	{
		if (pSrc[i] != pDst[0])
			continue;
		j = 0;
		while (pDst[j] != '\0' && pSrc[i + j] != '\0')
		{
			j++;
			if (pDst[j] != pSrc[i + j])
				break;
		}
		if (pDst[j] == '\0')
			return i;
	}
	return -1;
}

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

int main2()
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
	int i = -1,j=0;
	char pubuf[1024];
	while (!feof(fp))
	{
		ch1 = fgetc(fp);
		printf("%c",ch1);
		if (ch1 == '<')
		{
			e = ch1;
			ch1 = fgetc(fp);
			printf("%c", ch1);
			if (ch1 == '?')
			{
				i = 1;
			}
		}
		if (ch1 == '?')
		{
			e = ch1;
			ch1 = fgetc(fp);
			printf("%c", ch1);
			if (ch1 == '>')
			{
				i = -1;
			}
		}
		if ( i == 1 )
		{
			fputc(ch1,fp1);
			pubuf[j++] = ch1;
		}
		else
		{
			if (i == -1)
			{
				i--;
				fputs("print(\"", fp1);
				fputc(e, fp1);
				e = '\0';
			}
			if (ch1 != '\n')
			{
				fputc(ch1, fp1);
			}
		}
	}
	if (i == -2)
	{
		fputs("\");", fp1);
	}
	//fputs("I Love You", fp1);
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


int main()
{
	FILE *fp,*fpw;
	char strLine[256]; 
	char tmpLine[512];//��ȡ������  
	if ((fp = fopen("D:\\qq.php", "r")) == NULL)      //�ж��ļ��Ƿ���ڼ��ɶ�  
	{
		printf("Open Falied!");
		return -1;
	}
	if ((fpw = fopen("tmp123.php", "w")) == NULL)      //�ж��ļ��Ƿ���ڼ��ɶ�  
	{
		printf("Open Falied!");
		return -1;
	}
	int oin = 0;
	fputs("<?php $dd[\"get\"] = \"123\"; ?>", fpw);
	while (!feof(fp))                                   //ѭ����ȡÿһ�У�ֱ���ļ�β  
	{
		int flen = fgets(strLine, 256, fp);  
		printf("%s", strLine);
		if (oin < 0)
		{
			oin++;
		}
		if (Indexof(strLine, "?>") >= 0)
		{
			oin = -1;
			if (fputs(strLine, fpw) < 0)
			{
				return -1;
			}
		}
		if (oin == 1)
		{
			if (fputs(strLine, fpw) < 0 )
			{
				return -1;
			}
		}         
		if (Indexof(strLine,"<?php") >= 0)
		{
			oin = 1;
			if (fputs(strLine, fpw) < 0)
			{
				return -1;
			}
		}
		if (oin == 0)
		{
			/*int len = strlen(strLine);
			strLine[--len] = '\0';
			strcpy(tmpLine,"echo \"");
			strcat(tmpLine, strLine);
			strcat(tmpLine, "\";\n");*/
			fputs(strLine, fpw);
		}
	}
	fclose(fp);   
	fclose(fpw); 
	printf("===========================================\n");
	char result[1024] = "";                   //�����Ž�����ַ������� 
	if (1 == fnGetCmdOut("php tmp123.php", result))
		printf("%s", result);
	printf("\n");
	system("pause");
	return 0;
}


