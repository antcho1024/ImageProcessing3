#include "Filter.h"

void ImageOutput(UChar* Data, Int wid, Int hei, Char String[]) //��� �̹��� ��������
{
	char Name_Hist[50] = "out_";
	char Name_extension[10] = ".raw";  //��� �̹��� �����̸� �⺻ ����

	FILE *fp;

	strcat_s(Name_Hist, 20, String); //�ش��ϴ� �̸� ���̱�
	strcat_s(Name_Hist, 20, Name_extension);


	fopen_s(&fp, Name_Hist, "wb"); //���� ���� ����
	fwrite(Data, sizeof(UChar), wid * hei, fp); //���� ��� 
	fclose(fp);
}