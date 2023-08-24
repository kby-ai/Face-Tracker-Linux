#ifndef FACE_LANDMARK_H
#define FACE_LANDMARK_H
typedef struct _tagSMSize
{
    int iH;
    int iW;
}SMSize;

int init_landmark_vanila();
int deinit_landmark_vanila();
void get_face_68Landmark(unsigned char* pbGrayImg, int iW, int iH, int iFaceX, int iFaceY, int iFaceW, int iFaceH, float* landmark_ptr);
#endif // FACE_LANDMARK_H