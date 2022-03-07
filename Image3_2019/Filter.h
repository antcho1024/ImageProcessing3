#include "main.h"

#define CLIP(x) (x < minVal) ? minVal : x > maxVal ? maxVal : x  // 클리핑 과정
#define CLIPPIC_HOR(x) (x < 0) ? 0 : x >= WIDTH  ? WIDTH  - 1 : x
#define CLIPPIC_VER(x) (x < 0) ? 0 : x >= HEIGHT ? HEIGHT - 1 : x

#define MAX(x, y) x > y ? x : y // 큰 값 비교
#define SWAP(x, y) {int tmp; tmp = x; x = y; y = tmp;} // 두 값 바꾸기 

typedef struct _Image_Buffer
{
	UChar* padding; // 패딩 영상 저장 버퍼

	UChar* Embossing;
	UChar* Sharpening;
	UChar* DoG;
	UChar* Median;
	UChar* Sobel;
	UChar* Blur;
	UChar* homogeneity;
	UChar* LoG; // LoG 추가

}Img_Buf;

void ImageFiltering(UChar* Data, Img_Buf* img);
void ImageFilteringBlur(UChar* Data, Img_Buf* img);
void ImagePadding(Img_Buf* img, UChar* Buf, int width, int height, int maskSize);
void ImageOutput(UChar* Data, Int wid, Int hei, Char String[]);

void OutputBlock(Img_Buf* img, UChar *paddingBlock, int maskSize, int i, int j, int PStride); // 각 함수들 선언 