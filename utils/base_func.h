#ifndef _BASEFUNC_H_
#define _BASEFUNC_H_

#include "armCommon.h"
#include "base.h"

float* wExpandArray(float* pSrc, int nHeight, int nWidth, int nPadWidth, bool fSetZero = 0);
float* imgResize(float* pSrc, int nHeight, int nWidth, float rScale, float rMin = 0.0f, float rMax = 1.0f);
float get_distance(const float* imgData1, const float* imgData2, int size);
int align_vertical(unsigned char* pSrcBuf, int nSrcWidth, int nSrcHeight, unsigned char* pDstBuf, int nDstWidth, int nDstHeight, int nChannelCount, float* landmark_ptr,
                   float rDistanceEye_Mouth, float rFaceCenterX, float rFaceCenterY);
void shrink_RGB(unsigned char *src, int src_height, int src_width, unsigned char *dst, int dst_height, int dst_width);
int EvalNightImage(unsigned char* pbSrcImage, int nWidth, int nHeight, int* pnImageMean);
int resize_bilinear(unsigned char* src_img, int w, int h, unsigned char* dst_img, int w2, int h2);
void ShrinkImage(BYTE *pbInputImg, int nImgWid, int nImgHei, BYTE *pbOutImg, int nOutWid, int nOutHei);

int CreateDicFiles(const char* szLinkFileName, const char* szDicFolder, AlignParam& param);
void RemoveDicFiles(const char* szDicPath);
unsigned char* ReadDicPatch(int &size1, int &size2, const char *szDicPath);
int PrepareDataFile(const char *szDicPath);
void KdnnSetupInputofNet(float* pInput_data, int nSrcChannel, int nDstChannel, unsigned char* psCropedimg, int nWidth, int nHeight);
#endif
