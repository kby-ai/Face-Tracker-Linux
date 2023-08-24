#pragma once

#include "opencv2/video/tracking.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

typedef struct Box
{
    float xmin;
    float ymin;
    float xmax;
    float ymax;
} Box;

typedef struct FaceBox_
{
    float confidence;
    float x1;
    float y1;
    float x2;
    float y2;
} FaceBox_;

typedef struct pair_track_det_idx
{
    int track_idx;
    int det_idx;
} pair_track_det_idx;

class Detection
{
public:
    Detection(Box box_, float score_ = 0)
        : box(box_), score(score_){};
    ~Detection()
    {
    }

public:
    Box box;
    float score;
};