#ifndef FACE_TRACKER_H
#define FACE_TRACKER_H
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
#include "track/data_def.h"
#include "track/tracker.h"
#include "face_feature/face_feature.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include "definition.h"
#include "detection/face_detector.h"

using namespace std;

class FaceTracker {
public:
    FaceTracker();
    ~FaceTracker();
    int Get_Track(cv::Mat& src, vector<FaceInfo> dets, vector<Track>& tracks);
    int LoadThirdPartyModels();
    int ReleaseThirdPartyModels();

private:
    MultiObjectTracker* tracker;
    vector<float*> features_saved;
    FaceFeatureExtractor* feature_extractor;
};



#endif //FACE_TRACKER_H