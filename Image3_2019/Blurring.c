#include "Filter.h"

UChar BlurFunc(UChar* Data, int maskSize) // 블러링 연산 함수 
{
	double Conv = 0; //화소 결과 값
	double Mask_Coeff[] =
	{ 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
		1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
		1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };//마스크

	for (int i = 0; i < maskSize * maskSize; i++)
		Conv += (Mask_Coeff[i] * (double)Data[i]); // 각각 곱해서 더하기(평균내주기)

	return (UChar)CLIP(Conv);//클리핑해서 반환
}

UChar MedianFunc(UChar* Data, int maskSize) // 메디안 연산 함수
{
	
	int i, j;
	for (i = 0; i < maskSize * maskSize; i++)
	{
		for (j = i; j < maskSize * maskSize; j++) 
		{

			if ((int)Data[i] > (int)Data[j]) // 앞의 수가 뒤의 수보다 더 크면 
			{
				SWAP(Data[i], Data[j]);//두 값 바꾸기
			}
		}
	}// 오름차순


	return (int)Data[4];  // 가운데 값 반환
}

void ImageFilteringBlur(UChar* Data, Img_Buf* img) //메인에서 호출 : 스크래치 된 영상 필터링 하는 함수
{
	Int wid = WIDTH; Int hei = HEIGHT;
	Int min = minVal; Int max = maxVal;

	Int picSize = wid * hei; //영상 사이즈

	Int maskSize = 3;         //MxM size
	Int Add_size = (maskSize / 2) * 2;;
	UChar *paddingBlock;

	Int Stride = wid;
	Int PStride = wid + Add_size;

	Char String[8][10] = { "Embo", "Sharp", "Sobel", "Homogene", "Median", "DoG", "Blur","LoG" };

	img->Blur = (UChar*)calloc(picSize, sizeof(UChar)); //공간할당
	for (int cnt = 0; cnt < 6; cnt++) // 블러 6번 시행
	{
		paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // 영상에서 마스크 사이즈만큼 가져올 공간

		if (cnt == 0) // 첫 시행에는 원본 영상 패딩
			ImagePadding(img, Data, wid, hei, maskSize);
		else // 그 이후에는 그 전 결과 영상 패딩
			ImagePadding(img, img->Blur, wid, hei, maskSize);

		for (int i = 0; i < hei; i++)
			for (int j = 0; j < wid; j++)
			{
				OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // input 에서 3*3 블록 가져오기
				img->Blur[i * wid + j] = BlurFunc(paddingBlock, maskSize); //그 블록에 대해 블러링 계산해서 갱신
			}
		free(img->padding);
		free(paddingBlock);
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Median 영상 만드는 부분(10회 적용), 처음 입력영상은 원본 스크래치 레나, 2회부터는 Median이 적용된 영상


	img->Median = (UChar*)calloc(picSize, sizeof(UChar)); //공간 할당
	for (int cnt = 0; cnt < 10; cnt++) // 10번 필터링
	{
		paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar));  // 영상에서 마스크 사이즈만큼 가져올 공간

		if (cnt == 0) // 첫 시행 : 원본 패딩
			ImagePadding(img, Data, wid, hei, maskSize);
		else // 이 후 시행 : 그 전 결과 영상 패딩
			ImagePadding(img, img->Median, wid, hei, maskSize);

		for (int i = 0; i < hei; i++)
			for (int j = 0; j < wid; j++)
			{
				OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // input 에서 3*3 블록 가져오기
				img->Median[i * wid + j] = MedianFunc(paddingBlock, maskSize); // 가져온 거에서 중앙 값 찾아서 갱신
			}
		free(img->padding);
		free(paddingBlock);
	}




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	ImageOutput(img->Blur, wid, hei, String[6]);
	ImageOutput(img->Median, wid, hei, String[4]);

	free(img->Blur);
	free(img->Median);
}