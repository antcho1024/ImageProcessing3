#include "Filter.h"

void main()
{
	FILE *fp;
	UChar *ori; //���� ���� ȭ�Ұ����� �����ϱ� ���� ����
	Img_Buf image; 


	Int wid = WIDTH; Int hei = HEIGHT; // ���� ũ�� �⺻ �� ����
	Int min = minVal; Int max = maxVal; // ȭ�� �⺻ �� ����

	Int picSize = wid * hei; //���� ������


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Embossing, Sharpening, DoG, Sobel, Homogeneity
	fopen_s(&fp, "lena_512x512.raw", "rb"); //���� ���� ����
	ori = (UChar*)malloc(sizeof(UChar) * picSize); //���� ���� ũ�⸸ŭ ���� ����
	memset(ori, 0, sizeof(UChar) * picSize); //0���� �ʱ�ȭ
	fread(ori, sizeof(UChar), picSize, fp); // ���� ���� �б�(���� ������ �ȼ� ���� �迭 ������ ����)

	ImageFiltering(ori, &image); // ��������( ��ũ��ġ ���� ���� ���� )�� ���ڷ� ���� ���͸� �Լ� ȣ��

	free(ori);
	fclose(fp); //�޸� ����
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Median, Blur
	fopen_s(&fp, "Lena-scratch-noise.raw", "rb"); //���� ���� ����
	ori = (UChar*)malloc(sizeof(UChar) * picSize); //���� ���� ũ�⸸ŭ ���� ����
	memset(ori, 0, sizeof(UChar) * picSize); //0���� �ʱ�ȭ
	fread(ori, sizeof(UChar), picSize, fp); // ���� ���� �б�(���� ������ �ȼ� ���� �迭 ������ ����)

	ImageFilteringBlur(ori, &image); //��ũ��ġ ������ ���ڷ� ���� ���͸� �� �Լ� ȣ��

	free(ori);
	fclose(fp);// �޸� ����
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}