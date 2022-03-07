#include "Filter.h"

void ImageOutput(UChar* Data, Int wid, Int hei, Char String[]) //결과 이미지 내보내기
{
	char Name_Hist[50] = "out_";
	char Name_extension[10] = ".raw";  //결과 이미지 파일이름 기본 설정

	FILE *fp;

	strcat_s(Name_Hist, 20, String); //해당하는 이름 붙이기
	strcat_s(Name_Hist, 20, Name_extension);


	fopen_s(&fp, Name_Hist, "wb"); //원본 영상 열기
	fwrite(Data, sizeof(UChar), wid * hei, fp); //필터 결과 
	fclose(fp);
}