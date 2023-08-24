#pragma once

#include "data_def.h"
#include <vector>
#include <string>
class Kalman
{
public:
    Kalman(const Box& initRect);
    ~Kalman()
    {
    }
    void predict();
    void update(Box& box);

private:
    void setRectFrom(const cv::Mat& state);
    uint N; //! dimension of transition matrix: NxN
    uint M; //! length of measurement vector

    cv::KalmanFilter* filter;

public:
    Box cur_box;
};