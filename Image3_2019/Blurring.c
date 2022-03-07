#include "Filter.h"

UChar BlurFunc(UChar* Data, int maskSize) // ���� ���� �Լ� 
{
	double Conv = 0; //ȭ�� ��� ��
	double Mask_Coeff[] =
	{ 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
		1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
		1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };//����ũ

	for (int i = 0; i < maskSize * maskSize; i++)
		Conv += (Mask_Coeff[i] * (double)Data[i]); // ���� ���ؼ� ���ϱ�(��ճ��ֱ�)

	return (UChar)CLIP(Conv);//Ŭ�����ؼ� ��ȯ
}

UChar MedianFunc(UChar* Data, int maskSize) // �޵�� ���� �Լ�
{
	
	int i, j;
	for (i = 0; i < maskSize * maskSize; i++)
	{
		for (j = i; j < maskSize * maskSize; j++) 
		{

			if ((int)Data[i] > (int)Data[j]) // ���� ���� ���� ������ �� ũ�� 
			{
				SWAP(Data[i], Data[j]);//�� �� �ٲٱ�
			}
		}
	}// ��������


	return (int)Data[4];  // ��� �� ��ȯ
}

void ImageFilteringBlur(UChar* Data, Img_Buf* img) //���ο��� ȣ�� : ��ũ��ġ �� ���� ���͸� �ϴ� �Լ�
{
	Int wid = WIDTH; Int hei = HEIGHT;
	Int min = minVal; Int max = maxVal;

	Int picSize = wid * hei; //���� ������

	Int maskSize = 3;         //MxM size
	Int Add_size = (maskSize / 2) * 2;;
	UChar *paddingBlock;

	Int Stride = wid;
	Int PStride = wid + Add_size;

	Char String[8][10] = { "Embo", "Sharp", "Sobel", "Homogene", "Median", "DoG", "Blur","LoG" };

	img->Blur = (UChar*)calloc(picSize, sizeof(UChar)); //�����Ҵ�
	for (int cnt = 0; cnt < 6; cnt++) // �� 6�� ����
	{
		paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // ���󿡼� ����ũ �����ŭ ������ ����

		if (cnt == 0) // ù ���࿡�� ���� ���� �е�
			ImagePadding(img, Data, wid, hei, maskSize);
		else // �� ���Ŀ��� �� �� ��� ���� �е�
			ImagePadding(img, img->Blur, wid, hei, maskSize);

		for (int i = 0; i < hei; i++)
			for (int j = 0; j < wid; j++)
			{
				OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // input ���� 3*3 ��� ��������
				img->Blur[i * wid + j] = BlurFunc(paddingBlock, maskSize); //�� ��Ͽ� ���� ���� ����ؼ� ����
			}
		free(img->padding);
		free(paddingBlock);
	}
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Median ���� ����� �κ�(10ȸ ����), ó�� �Է¿����� ���� ��ũ��ġ ����, 2ȸ���ʹ� Median�� ����� ����


	img->Median = (UChar*)calloc(picSize, sizeof(UChar)); //���� �Ҵ�
	for (int cnt = 0; cnt < 10; cnt++) // 10�� ���͸�
	{
		paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar));  // ���󿡼� ����ũ �����ŭ ������ ����

		if (cnt == 0) // ù ���� : ���� �е�
			ImagePadding(img, Data, wid, hei, maskSize);
		else // �� �� ���� : �� �� ��� ���� �е�
			ImagePadding(img, img->Median, wid, hei, maskSize);

		for (int i = 0; i < hei; i++)
			for (int j = 0; j < wid; j++)
			{
				OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // input ���� 3*3 ��� ��������
				img->Median[i * wid + j] = MedianFunc(paddingBlock, maskSize); // ������ �ſ��� �߾� �� ã�Ƽ� ����
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