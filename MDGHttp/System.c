#include "stdafx.h"

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