#include <math.h>
#include <memory>
#include "../utils/base.h"
#include "face_landmark.h"
#include "vanila_model.h"

using namespace std;
static FaceModelExtractor* g_face_model_extractor;
int g_inited_vanila_engine = SDK_NOT_INITIALIZED;

int init_landmark_vanila() {
    g_face_model_extractor = new FaceModelExtractor(60, 60, 4);

    g_inited_vanila_engine = SDK_SUCCESS;
    return SDK_SUCCESS;
}

void resize_bilinear1(unsigned char* pbGraySrc, int iW, int iH, unsigned char* pbDst, int iDstW, int iDstH)
{	//bilinear method
	float rRateX = (float)iW / iDstW;
	float rRateY = (float)iH / iDstH;
	int iOffset = 0;
	int iIndex;
	float rDiffX, rDiffY;

	for (int i = 0; i < iDstH; i++)
	{
		for (int j = 0; j < iDstW; j++)
		{
			int iPosX = (int)(rRateX * j);
			int iPosY = (int)(rRateY * i);
			rDiffX = (rRateX * j) - iPosX;
			rDiffY = (rRateY * i) - iPosY;
			iIndex = iPosY * iW + iPosX;

			pbDst[iOffset++] = (unsigned char)(pbGraySrc[iIndex] * (1 - rDiffX) * (1 - rDiffY) + pbGraySrc[iIndex + 1] * rDiffX * (1 - rDiffY) +
				pbGraySrc[iIndex + iW] * (1 - rDiffX) * rDiffY + pbGraySrc[iIndex + iW + 1] * rDiffX * rDiffY);
		}
	}
}

void meanStdDev(unsigned char* pbSrc, int iSrcW, int iSrcH, float& rMean, float& rStdDev)
{
	float rSum = 0.f;
	int iSize = iSrcW * iSrcH;
	for (int i = 0; i < iSize; i++)
		rSum += (1.f * pbSrc[i]);
	rMean = rSum / iSize;

	float rDevSum = 0.f;
	for (int i = 0; i < iSize; i++)
		rDevSum += ((pbSrc[i] - rMean) * (pbSrc[i] - rMean));

	rStdDev = sqrt(rDevSum / (iSize - 1));
}


void get_face_68Landmark(unsigned char* pbGrayImg, int iW, int iH, int iFaceX, int iFaceY, int iFaceW, int iFaceH, float* landmark_ptr)
{
	float rExRate = 0.05;
	int iExFaceX = (int)(iFaceX - rExRate * iFaceW);
	int iExFaceY = (int)(iFaceY - rExRate * iFaceH);
	int iExFaceW = (int)((1 + 2 * rExRate) * iFaceW);
	int	iExFaceH = (int)((1 + 2 * rExRate) * iFaceH);

	iExFaceX = iExFaceX > 0 ? (iExFaceX < iW ? iExFaceX : iW - 1) : iFaceX;
	iExFaceY = iExFaceY > 0 ? (iExFaceY < iH ? iExFaceY : iH - 1) : iFaceY;
	iExFaceW = (iExFaceX + iExFaceW) < iW ? iExFaceW : (iW - iExFaceX - 1);
	iExFaceH = (iExFaceY + iExFaceH) < iH ? iExFaceH : (iH - iExFaceY - 1);

	unsigned char* pbFace = (unsigned char*)malloc(iExFaceW * iExFaceH);
	int iOffset = iExFaceY * iW + iExFaceX;
	unsigned char *pbCurImg, *pbTemp;
	pbCurImg = pbGrayImg + iOffset;
	pbTemp = pbFace;

	for (int i = 0; i < iExFaceH; i++)
	{
		memcpy(pbTemp, pbCurImg, iExFaceW);
		pbCurImg += iW;
		pbTemp += iExFaceW;
	}

	SMSize xDstSize;
	xDstSize.iW = 60;
	xDstSize.iH = 60;
	unsigned char* pbResizeImg = (unsigned char*)malloc(xDstSize.iW * xDstSize.iH);
	resize_bilinear1(pbFace, iExFaceW, iExFaceH, pbResizeImg, xDstSize.iW, xDstSize.iH);
	free(pbFace);

	float rMean, rStdDev;
	meanStdDev(pbResizeImg, xDstSize.iW, xDstSize.iH, rMean, rStdDev);

	float* rData = (float*)malloc(sizeof(float) * xDstSize.iW * xDstSize.iH);
	int iSize = xDstSize.iW * xDstSize.iH;
	for (int i = 0; i < iSize; i++)
		rData[i] = (1.0f * pbResizeImg[i] - rMean) / (1e-9 + rStdDev);


	free(pbResizeImg);

	float* feat2 = (float*)malloc(136 * sizeof(float));

    g_face_model_extractor->extract(rData, feat2);

	for (int i = 0; i < 68; i++) {
        landmark_ptr[2 * i] = feat2[2 * i] * iExFaceW + iExFaceX;			  // xPos
		landmark_ptr[2 * i + 1] = feat2[2 * i + 1] * iExFaceH + iExFaceY;   // yPos
	}

	free(rData);
	free(feat2);
}

int deinit_landmark_vanila() {
	if (!g_inited_vanila_engine)
		return true;

	delete g_face_model_extractor;
	g_inited_vanila_engine = false;
	return true;
}