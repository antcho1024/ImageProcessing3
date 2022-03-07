#include "Filter.h"

UChar EmbossingFunc(UChar* Data, int maskSize) // ������ ��� �Լ�
{
	int Conv = 0; // ��� ��
	int Mask_Coeff[] = 
	{  -1, 0, 0,
		0, 0, 0,
		0, 0, 1 }; // ����ũ

	for (int i = 0; i < maskSize * maskSize; i++) // �ȼ��� ����ũ ������ ���ؼ� ����
		Conv += (Mask_Coeff[i] * (int)Data[i]);
	Conv = CLIP(Conv); // Ŭ����

	return (UChar)CLIP(Conv + 128); // �������� �����Ƿ� +128�� ���ְ� Ŭ���� �ٽ� ���༭ �ش� �ȼ��� ���� ��� �� return
}

UChar SharpeningFunc(UChar* Data, int maskSize) // ������ ��� �Լ�
{
	int Conv = 0; //��� ��
	int Mask_Coeff[] =
	{ -1, -1, -1,
	-1, 9, -1,
	-1, -1, -1 }; //����ũ

	/* */
	for (int i = 0; i < maskSize * maskSize; i++) // �ȼ��� ����ũ ������ ���ؼ� ����
		Conv += (Mask_Coeff[i] * (int)Data[i]);

	return (UChar)CLIP(Conv); //��� ���� Ŭ�����ؼ� ������� ��ȯ
}

UChar DoGFunc(UChar* Data, int maskSize) //DoG ��� �Լ�
{
	int Conv = 0;//��� ��
	int Mask_Coeff[] =
	{ 0, 0, -1, -1, -1, 0, 0,
	0,-2,-3,-3,-3,-2,0,
	-1,-3,5,5,5,-3,-1,
	-1,-3,5,16,5,-3,-1,
	-1,-3,5,5,5,-3,-1,
	0,-2,-3,-3,-3,-2,0,
	0,0,-1,-1,-1,0,0 };// 7*7 ����ũ

	for (int i = 0; i < maskSize * maskSize; i++)// �ȼ��� ����ũ ������ ���ؼ� ����
		Conv += (Mask_Coeff[i] * (int)Data[i]);
	

	Conv = CLIP(Conv);//��� ���� Ŭ�����ؼ� ������� ��ȯ
	return (UChar)Conv;
}

UChar SobelFunc(UChar* Data, int maskSize) //Sobel ��� �Լ�
{
	int ConvVer = 0; 
	int ConvHor = 0;
	int Mask_Coeff_Ver[] =
	{ 1, 0, -1,
	2, 0, -2,
	1, 0, -1 }; // x�� ���� ����ũ

	int Mask_Coeff_Hor[] = 
	{ -1, -2, -1,
	  0, 0, 0,
	1, 2, 1 }; //y�� ���� ����ũ

	/* */

	for (int i = 0; i < maskSize * maskSize; i++)
		ConvVer += (Mask_Coeff_Ver[i] * (int)Data[i]);

	for (int i = 0; i < maskSize * maskSize; i++)
		ConvHor += (Mask_Coeff_Hor[i] * (int)Data[i]);//������, ���� �� ���� ��� ��
	

	ConvVer = CLIP(ConvVer);
	ConvHor = CLIP(ConvHor);// ���� Ŭ���� ��

	return (UChar)CLIP(ConvVer + ConvHor); // �� �� ���ؼ� ��ȯ
}


UChar HomogeneityFunc(UChar* Data, int maskSize) //Homogeneity ��� �Լ�
{
	/* */
	int max = -1; // ���� ��� �� ( ���� max �� ��� ��� �̹Ƿ� -1�� �ʱ�ȭ)
	int tmp = 0; // ���� �A ���� ���� �� �� ����

	int center = (int)Data[4]; // ���� �� (�׻� ���� ��)

	for (int i = 0; i < maskSize * maskSize; i++)
	{
		if (i == 4) continue; // �ڱ� �ڽŰ��� ������ �ǳ� ��
		tmp = (int)Data[i] - center; // �� �ȼ��� ���Ϳ� �E�� ����
		if (tmp < 0) tmp *= -1; // ���� 
		if (max < tmp) max = tmp; // max �� ���� ũ�� max ����
	}
	max = CLIP(max); // Ŭ����

	return (UChar)CLIP(max + 60); //�� ���� ��� �ϱ� ���� +60 ���� �� Ŭ���� �ؼ� ���� ��� ��ȯ
}

UChar LoGFunc(UChar* Data, int maskSize) //LoG ��� �Լ�
{
	int Conv = 0;
	int Mask_Coeff[] =
	{0,0,-1,0,0,
	0,-1,-2,-1,0,
	-1,-2,16,-2,-1,
	0,-1,-2,-1,0,
	0,0,-1,0,0}; //5*5 ����ũ

	for (int i = 0; i < maskSize * maskSize; i++)
		Conv += (Mask_Coeff[i] * (int)Data[i]);// �ȼ��� ����ũ ������ ���ؼ� ����

	Conv = CLIP(Conv);
	return (UChar)Conv;// Ŭ���� �� ��ȯ
}

void OutputBlock(Img_Buf* img, UChar *paddingBlock, int maskSize, int i, int j, int PStride) // input ���󿡼� ��� ����
{
	for (int k = 0; k < maskSize; k++) // ����ũ ������ ��ŭ ��� paddingBlock �� ����
		for (int l = 0; l < maskSize; l++)
			paddingBlock[k * maskSize + l] = img->padding[(i + k) * PStride + (j + l)];
}

void ImageFiltering(UChar* Data, Img_Buf* img) //���ο��� ȣ���ϴ� �Լ� : ���͸� ȣ�� �Լ�
{
	Int wid = WIDTH; Int hei = HEIGHT;
	Int min = minVal; Int max = maxVal;

	Int picSize = wid * hei; //���� ������

	Int maskSize = 3;         //MxM size
	Int Add_size = (maskSize / 2) * 2;
	UChar *paddingBlock;

	Int Stride  = wid;
	Int PStride = wid + Add_size;

	Char String[8][10] = { "Embo", "Sharp", "Sobel", "Homogene", "Median", "DoG", "Blur","LoG"};

	img->Embossing     = (UChar*)calloc(picSize, sizeof(UChar));
	img->Sharpening    = (UChar*)calloc(picSize, sizeof(UChar));
	img->homogeneity   = (UChar*)calloc(picSize, sizeof(UChar)); //�޸� ���� �ʱ�ȭ

	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // ���󿡼� ����ũ ũ�⸸ŭ ������� ���� ����
	ImagePadding(img, Data, wid, hei, maskSize); // ���������� �е� (Copy Padding)
	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++) 
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // �е��� ���󿡼� ����ũ ������(3*3) ��ŭ ��� ��������
			img->Embossing  [i * wid + j] = EmbossingFunc  (paddingBlock, maskSize); // ������ ���(paddingBlock)�� ���� ���� ����
			img->Sharpening [i * wid + j] = SharpeningFunc (paddingBlock, maskSize);
			img->homogeneity[i * wid + j] = HomogeneityFunc(paddingBlock, maskSize);
		}
	free(img->padding);
	free(paddingBlock);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sobel ���� ����� �κ�, �Է¿����� Embossing�� ����� ����
	
	img->Sobel =  (UChar*)calloc(picSize, sizeof(UChar)); //�޸� �Ҵ�

	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // ���󿡼� ����ũ ũ�⸸ŭ ������� ���� ����
	ImagePadding(img, img->Embossing, wid, hei, maskSize); // Embossing ó�� �� ������ �е� (Copy Padding)

	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++)
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride); // �е��� ���󿡼� ����ũ ������(3*3)��ŭ ��� ��������
			img->Sobel[i * wid + j] = SobelFunc(paddingBlock, maskSize); // ������ ���(paddingBlock)�� ���� SobelFunc �����Լ� ȣ��
			
		}
	free(img->padding);
	free(paddingBlock);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DoG ���� ����� �κ�, �Է¿����� Sharpening�� ����� ����, Mask ũ��� 7x7

	img->DoG = (UChar*)calloc(picSize, sizeof(UChar)); // �޸� �Ҵ�

	maskSize = 7; //DoG�����̹Ƿ� ����ũ ������ 3���� 7�� �ٲ���
	Add_size = (maskSize / 2) * 2; // ���� �����ִ� ������ ���� ����
	PStride = wid + Add_size; //�� ����
	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar));// ���󿡼� ����ũ ũ�⸸ŭ ������� ���� ����
	ImagePadding(img, img->Sharpening, wid, hei, maskSize);// Sharpening ó�� �� ������ �е� (Copy Padding)

	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++)
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride);  // �е��� ���󿡼� ����ũ ������(7*7)��ŭ ��� ��������
			img->DoG[i * wid + j] = DoGFunc(paddingBlock, maskSize);// ������ ���(paddingBlock)�� ���� DoGFunc �����Լ�

		}
	free(img->padding);
	free(paddingBlock);





	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// LoG //
	img->LoG = (UChar*)calloc(picSize, sizeof(UChar));

	maskSize = 5; // ����ũ ������ 5�� ����
	Add_size = (maskSize / 2) * 2;
	PStride = wid + Add_size; //����ũ ������� ���õ� ������ �ٲ��ֱ�

	paddingBlock = (UChar*)calloc(maskSize * maskSize, sizeof(UChar)); // ���󿡼� ����ũ ũ�⸸ŭ ������� ���� ����
	ImagePadding(img, Data, wid, hei, maskSize); // ���� ���� �е�

	for (int i = 0; i < hei; i++)
		for (int j = 0; j < wid; j++)
		{
			OutputBlock(img, paddingBlock, maskSize, i, j, PStride);// �е��� ���󿡼� ����ũ ������(5*5)��ŭ ��� ��������
			img->LoG[i * wid + j] = LoGFunc(paddingBlock, maskSize); // ������ ���(paddingBlock)�� ���� LoGFunc �����Լ�

		}
	free(img->padding);
	free(paddingBlock);


	ImageOutput(img->Embossing,   wid, hei, String[0]);//Ȯ�ο�
	ImageOutput(img->Sharpening, wid, hei, String[1]);//Ȯ�ο�
	ImageOutput(img->Sobel,       wid, hei, String[2]);
	ImageOutput(img->homogeneity, wid, hei, String[3]);
	ImageOutput(img->DoG,         wid, hei, String[5]);
	ImageOutput(img->LoG,		  wid, hei, String[7]);
	
	free(img->Embossing);
	free(img->Sharpening);
	free(img->Sobel);
	free(img->homogeneity);
	free(img->DoG);
	free(img->LoG);//���� ����
}