#ifndef FACE_FEATURE_EXTRACTOR_H
#define FACE_FEATURE_EXTRACTOR_H
#include <net.h>
#include <cpu.h>
#if defined(USE_NCNN_SIMPLEOCV)
#include "simpleocv.h"
#else
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif
#include "benchmark.h"
#include "platform.h"

class FaceFeatureExtractor {
public:
    FaceFeatureExtractor();

    ~FaceFeatureExtractor();

    int LoadModel();

    int get_face_feature(cv::Mat& src, float* landmark, float* feature);

private:
    ncnn::Net net_;
    const std::string net_input_name_ = "data";
    const std::string net_output_name_ = "fc1";
    ncnn::Option option_;
    int thread_num_;
};

#endif //FACE_FEATURE_EXTRACTOR_H
