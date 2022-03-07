#include "Filter.h"

UChar EmbossingFunc(UChar* Data, int maskSize) // 엠보싱 계산 함수
{
	int Conv = 0; // 결과 값
	int Mask_Coeff[] = 
	{  -1, 0, 0,
		0, 0, 0,
		0, 0, 1 }; // 마스크

	for (int i = 0; i < maskSize * maskSize; i++) // 픽셀과 마스크 값들을 곱해서 더함
		Conv += (Mask_Coeff[i] * (int)Data[i]);
	Conv = CLIP(Conv); // 클립핑

	return (UChar)CLIP(Conv + 128); // 음수값이 있으므로 +128을 해주고 클립핑 다시 해줘서 해당 픽셀의 최종 결과 값 return
}

UChar SharpeningFunc(UChar* Data, int maskSize) // 샤프닝 계산 함수
{
	int Conv = 0; //결과 값
	int Mask_Coeff[] =
	{ -1, -1, -1,
	-1, 9, -1,
	-1, -1, -1 }; //마스크

	/* */
	for (int i = 0; i < maskSize * maskSize; i++) // 픽셀과 마스크 값들을 곱해서 더함
		Conv += (Mask_Coeff[i] * (int)Data[i]);

	return (UChar)CLIP(Conv); //결과 값을 클리핑해서 최종결과 반환
}

UChar DoGFunc(UChar* Data, int maskSize) //DoG 계산 함수
{
	int Conv = 0;//결과 값
	int Mask_Coeff[] =
	{ 0, 0, -1, -1, -1, 0, 0,
	0,-2,-3,-3,-3,-2,0,
	-1,-3,5,5,5,-3,-1,
	-1,-3,5,16,5,-3,-1,
	-1,-3,5,5,5,-3,-1,
	0,-2,-3,-3,-3,-2,0,
	0,0,-1,-1,-1,0,0 };// 7*7 마스크

	for (int i = 0; i < maskSize * maskSize; i++)// 픽셀과 마스크 값들을 곱해서 더함
		Conv += (Mask_Coeff[i] * (int)Data[i]);
	

	Conv = CLIP(Conv);//결과 값을 클리핑해서 최종결과 반환
	return (UChar)Conv;
}

UChar SobelFunc(UChar* Data, int maskSize) //Sobel 계산 함수
{
	int ConvVer = 0; 
	int ConvHor = 0;
	int Mask_Coeff_Ver[] =
	{ 1, 0, -1,
	2, 0, -2,
	1, 0, -1 }; // x축 방향 마스크

	int Mask_Coeff_Hor[] = 
	{ -1, -2, -1,
	  0, 0, 0,
	1, 2, 1 }; //y축 방향 마스크

	/* */

	for (int i = 0; i < maskSize * maskSize; i++)
		ConvVer += (Mask_Coeff_Ver[i] * (int)Data[i]);

	for (int i = 0; i < maskSize * maskSize; i++)
		ConvHor += (Mask_Coeff_Hor[i] * (int)Data[i]);//수직축, 수평 축 따로 계산 함
	

	ConvVer = CLIP(ConvVer);
	ConvHor = CLIP(ConvHor);// 각각 클리핑 후

	return (UChar)CLIP(ConvVer + ConvHor); // 두 값 더해서 반환
}


UChar HomogeneityFunc(UChar* Data, int maskSize) //Homogeneity 계산 함수
{
	/* */
	int max = -1; // 최종 결과 값 ( 이후 max 값 모두 양수 이므로 -1로 초기화)
	int tmp = 0; // 각각 뺸 값들 비교할 때 쓸 변수

	int center = (int)Data[4]; // 센터 값 (항상 빼는 값)

	for (int i = 0; i < maskSize * maskSize; i++)
	{
		if (i == 4) continue; // 자기 자신과의 뺄셈은 건너 뜀
		tmp = (int)Data[i] - center; // 각 픽셀들 센터와 뺼셈 연산
		if (tmp < 0) tmp *= -1; // 절댓값 
		if (max < tmp) max = tmp; // max 값 보다 크면 max 갱신
	}
	max = CLIP(max); // 클리핑

	return (UChar)CLIP(max + 60); //더 눈에 띄게 하기 위해 +60 해준 후 클리핑 해서 최종 결과 반환
}

UChar LoGFunc(UChar* Data, int maskSize) //LoG 계산 함수
{
	int Conv = 0;
	int Mask_Coeff[] =
	{0,0,-1,0,0,
	0,-1,-2,-1,0,
	-1,-2,16,-2,-1,
	0,-1,-2,-1,0,
	0,0,-1,0,0}; //5*5 마스크

	for (int i = 0; i < maskSize * maskSize; i++)
		Conv += (Mask_Coeff[i] * (int)Data[i]);// 픽셀과 마스크 값들을 곱해서 더함

	Conv = CLIP(Conv);
	return (UChar)Conv;// 클리핑 후 반환
}

void OutputBlock(Img_Buf* img, UChar *paddingBlock, int maskSize, int i, int j, int PStride) // input 영상에서 블록 떼기
{
	for (int k = 0; k < maskSize; k++) // 마스크 사이즈 만큼 떼어서 paddingBlock 에 저장
		for (int l = 0; l < maskSize; l++)
			paddingBlock[k * maskSize + l] = img->padding[(i + k) * PStride + (j + l)];
}

void ImageFiltering(UChar* Data, Img_Buf* img) //메인에서 호출하는 함수 : 필터링 호출 함수
{
	Int wid = WIDTH; Int hei = HEIGHT;
	Int min = minVal; Int max = maxVal;

	Int picSize = wid * hei; //영상 사이즈

	Int maskSize = 3;         //MxM size
	Int Add_size = (maskSize / 2) * 2;
	UChar *paddingBlock;

	Int Stride  = wid;
	Int PStride = wid + Add_size;

	Char String[8][10] = { "Embo", "Sharp", "Sobel", "Homogene", "Median", "DoG", "Blur","LoG"};

	img->Embossing     = (UChar*)calloc(picSize, sizeof(UChar));
	img->Sharpening    = (UChar*)calloc(picSize, sizeof(UChar));
	img->homogeneity   = (UChar*)calloc(picSize, sizeof(UChar)); //메모리 공간 초기화

	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // 영상에서 마스크 크기만큼 떼어오기 위한 공간
	ImagePadding(img, Data, wid, hei, maskSize); // 원본영상을 패딩 (Copy Padding)
	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++) 
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // 패딩된 영상에서 마스크 사이즈(3*3) 만큼 블록 가져오기
			img->Embossing  [i * wid + j] = EmbossingFunc  (paddingBlock, maskSize); // 가져온 블록(paddingBlock)에 대해 각각 연산
			img->Sharpening [i * wid + j] = SharpeningFunc (paddingBlock, maskSize);
			img->homogeneity[i * wid + j] = HomogeneityFunc(paddingBlock, maskSize);
		}
	free(img->padding);
	free(paddingBlock);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sobel 영상 만드는 부분, 입력영상은 Embossing이 적용된 영상
	
	img->Sobel =  (UChar*)calloc(picSize, sizeof(UChar)); //메모리 할당

	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // 영상에서 마스크 크기만큼 떼어오기 위한 공간
	ImagePadding(img, img->Embossing, wid, hei, maskSize); // Embossing 처리 된 영상을 패딩 (Copy Padding)

	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++)
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // 패딩된 영상에서 마스크 사이즈(3*3)만큼 블록 가져오기
			img->Sobel[i * wid + j] = SobelFunc(paddingBlock, maskSize); // 가져온 블록(paddingBlock)에 대해 SobelFunc 연산함수 호출
			
		}
	free(img->padding);
	free(paddingBlock);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DoG 영상 만드는 부분, 입력영상은 Sharpening이 적용된 영상, Mask 크기는 7x7

	img->DoG = (UChar*)calloc(picSize, sizeof(UChar)); // 메모리 할당

	maskSize = 7; //DoG필터이므로 마스크 사이즈 3에서 7로 바꿔줌
	Add_size = (maskSize / 2) * 2; // 폭에 더해주는 변수도 새로 갱신
	PStride = wid + Add_size; //폭 갱신
	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar));// 영상에서 마스크 크기만큼 떼어오기 위한 공간
	ImagePadding(img, img->Sharpening, wid, hei, maskSize);// Sharpening 처리 된 영상을 패딩 (Copy Padding)

	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++)
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride);  // 패딩된 영상에서 마스크 사이즈(7*7)만큼 블록 가져오기
			img->DoG[i * wid + j] = DoGFunc(paddingBlock, maskSize);// 가져온 블록(paddingBlock)에 대해 DoGFunc 연산함수

		}
	free(img->padding);
	free(paddingBlock);





	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// LoG //
	img->LoG = (UChar*)calloc(picSize, sizeof(UChar));

	maskSize = 5; // 마스크 사이즈 5로 갱신
	Add_size = (maskSize / 2) * 2;
	PStride = wid + Add_size; //마스크 사이즈와 관련된 변수값 바꿔주기

	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // 영상에서 마스크 크기만큼 떼어오기 위한 공간
	ImagePadding(img, Data, wid, hei, maskSize); // 원본 영상 패딩

	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++)
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride);// 패딩된 영상에서 마스크 사이즈(5*5)만큼 블록 가져오기
			img->LoG[i * wid + j] = LoGFunc(paddingBlock, maskSize); // 가져온 블록(paddingBlock)에 대해 LoGFunc 연산함수

		}
	free(img->padding);
	free(paddingBlock);


	ImageOutput(img->Embossing,   wid, hei, String[0]);//확인용
	ImageOutput(img->Sharpening, wid, hei, String[1]);//확인용
	ImageOutput(img->Sobel,       wid, hei, String[2]);
	ImageOutput(img->homogeneity, wid, hei, String[3]);
	ImageOutput(img->DoG,         wid, hei, String[5]);
	ImageOutput(img->LoG,		  wid, hei, String[7]);
	
	free(img->Embossing);
	free(img->Sharpening);
	free(img->Sobel);
	free(img->homogeneity);
	free(img->DoG);
	free(img->LoG);//공간 해제
}