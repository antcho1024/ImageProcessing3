#include "Filter.h"

void main()
{
	FILE *fp;
	UChar *ori; //원본 영상 화소값들을 저장하기 위한 버퍼
	Img_Buf image; 


	Int wid = WIDTH; Int hei = HEIGHT; // 영상 크기 기본 값 정의
	Int min = minVal; Int max = maxVal; // 화소 기본 값 정의

	Int picSize = wid * hei; //영상 사이즈


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Embossing, Sharpening, DoG, Sobel, Homogeneity
	fopen_s(&fp, "lena_512x512.raw", "rb"); //원본 영상 열기
	ori = (UChar*)malloc(sizeof(UChar) * picSize); //원본 영상 크기만큼 공간 선언
	memset(ori, 0, sizeof(UChar) * picSize); //0으로 초기화
	fread(ori, sizeof(UChar), picSize, fp); // 원본 영상 읽기(원본 영상의 픽셀 값을 배열 변수에 저장)

	ImageFiltering(ori, &image); // 원본영상( 스크래치 넣지 않은 영상 )을 인자로 가진 필터링 함수 호출

	free(ori);
	fclose(fp); //메모리 해제
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Median, Blur
	fopen_s(&fp, "Lena-scratch-noise.raw", "rb"); //원본 영상 열기
	ori = (UChar*)malloc(sizeof(UChar) * picSize); //원본 영상 크기만큼 공간 선언
	memset(ori, 0, sizeof(UChar) * picSize); //0으로 초기화
	fread(ori, sizeof(UChar), picSize, fp); // 원본 영상 읽기(원본 영상의 픽셀 값을 배열 변수에 저장)

	ImageFilteringBlur(ori, &image); //스크래치 영상을 인자로 갖는 필터링 블러 함수 호출

	free(ori);
	fclose(fp);// 메모리 해제
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}