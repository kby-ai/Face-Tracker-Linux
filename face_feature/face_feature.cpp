#include "face_feature.h"
//#include "../utils/base_func.h"
#include "../face_modeling/face_landmark.h"

void SetupInputofNet(float* pInput_data, unsigned char* psCropedimg, int nWidth, int nHeight)
{
    float* input_data = pInput_data;

    int nInput_width = nWidth;
    int nInput_height = nHeight;
    float iKDNN_SCALE = 1 / 256.;
    int nIdx = 0;
    int nChsize = nInput_height * nInput_width;
    float rVal;
    for (int i = 0; i < nHeight; i++)
    {
        for (int j = 0; j < nWidth; j++)
        {
            input_data[i * nInput_width + j] = (float)(psCropedimg[3 * nIdx]) * iKDNN_SCALE;                   //b
            input_data[nChsize + i * nInput_width + j] = (float)(psCropedimg[3 * nIdx + 1]) * iKDNN_SCALE;     //g
            input_data[2 * nChsize + i * nInput_width + j] = (float)(psCropedimg[3 * nIdx + 2]) * iKDNN_SCALE; //r
            nIdx++;
        }
    }
}

FaceFeatureExtractor::FaceFeatureExtractor() : thread_num_(2){
    option_.lightmode = true;
    option_.num_threads = thread_num_;
}

FaceFeatureExtractor::~FaceFeatureExtractor() {
    net_.clear();
}

int FaceFeatureExtractor::LoadModel() {
    net_.opt = option_;
    net_.opt.use_vulkan_compute = true;
    int ret = net_.load_param("./weights/face_recognize/mobilefacenet.param");
    if(ret != 0) {
        printf("FaceFeatureExtractor load param failed. %d", ret);
        return -1;
    }

    ret = net_.load_model("./weights/face_recognize/mobilefacenet.bin");
    if(ret != 0) {
        printf("FaceFeatureExtractor load model failed. %d", ret);
        return -2;
    }
    return 0;
}

int FaceFeatureExtractor::get_face_feature(cv::Mat& src, float* landmark, float* feature)
{
    return 0;
}
