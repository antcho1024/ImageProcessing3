#include "Filter.h"

void ImagePadding(Img_Buf* img, UChar* Buf, int width, int height, int maskSize) // Copy Padding 함수
{
	int line, i, j;
	int Add_size = (maskSize / 2) * 2; // 상하, 좌우에 더해줄 값
	int Stride = width; // 원래 폭
	int PStride = width + Add_size; // 패딩 후 폭
	img->padding = (UChar*)calloc((width + Add_size) * (height + Add_size), sizeof(UChar)); //메모리 할당

	for (line = 0; line < (maskSize / 2); line++) //마스크 크기에 따라 패딩 크기( 반복문 횟수 ) 달라짐
	{
		//상하단 패딩
		for (i = 0; i < width; i++)
		{
			img->padding[line                               * PStride + (maskSize / 2 + i)] = Buf[i]; // 상단 패딩 : 첫 행 값 넣어주기
			img->padding[((height + (maskSize / 2)) + line) * PStride + (maskSize / 2 + i)] = Buf[(height - 1) * Stride + i]; // 하단 패딩 : 마지막 행 값 넣어주기
		}

		//좌우측 패딩
		for (i = 0; i < height; i++)
		{
			img->padding[(maskSize / 2 + i)                 * PStride +         line] = Buf[i * Stride]; //좌측 패딩 : 첫 열 값 넣어주기
			img->padding[(maskSize / 2 + i)                 * PStride + width + line] = Buf[i * Stride + (width - 1)]; // 우측 패딩 : 마지막 열 값 넣어주기
		}
	}

	for (i = 0; i < (maskSize / 2); i++) //모서리 패딩해주기
	{
		for (j = 0; j < (maskSize / 2); j++)
		{
			/*** 좌상단 패딩 ***/
			img->padding[i * PStride + j] = Buf[0];
			
			/*** 우상단 패딩 ***/
			img->padding[i * PStride + ((maskSize / 2) + width + j)] = Buf[width - 1];
			
			/*** 좌하단 패딩 ***/
			img->padding[(height + (maskSize / 2) + i) * PStride + j] = Buf[(height - 1) * Stride];
			
			/*** 우하단 패딩 ***/
			img->padding[(height + (maskSize / 2) + i) * PStride + ((maskSize / 2) + width + j)] = Buf[(height - 1) * Stride + (width - 1)];
		}
	}

	/*** 원본 버퍼 불러오기 ***/
	for (i = 0; i < height; i++) // 패딩 부분 외의 원본 부분 넣어주기
	{
		for (j = 0; j < width; j++)
		{
			img->padding[(i + (maskSize / 2)) * PStride + (j + (maskSize / 2))] = Buf[i * Stride + j];
		}
	}
}