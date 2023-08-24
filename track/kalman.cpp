#include "kalman.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
Kalman::Kalman(const Box& initRect)
{
    N = 8; // dimension of trans. matrix
    M = 4; // length of measurement

    // setup kalman filter with a Model Matrix, a Measurement Matrix and no control vars
    filter = new cv::KalmanFilter(N, M, 0);

    // transitionMatrix is eye(n,n) by default
    filter->transitionMatrix.at<float>(0, 4) = 1.567f; // dt=0.04, stands for the time
    filter->transitionMatrix.at<float>(1, 5) = 1.567f; // betweeen two video frames in secs.
    filter->transitionMatrix.at<float>(2, 6) = 1.567f;
    filter->transitionMatrix.at<float>(3, 7) = 1.567f;

    // measurementMatrix is zeros(n,p) by default
    filter->measurementMatrix.at<float>(0, 0) = 1.0f;
    filter->measurementMatrix.at<float>(1, 1) = 1.0f;
    filter->measurementMatrix.at<float>(2, 2) = 1.0f;
    filter->measurementMatrix.at<float>(3, 3) = 1.0f;

    using cv::Scalar;

    // assign a small value to diagonal coeffs of processNoiseCov
    cv::setIdentity(filter->processNoiseCov, Scalar::all(1e-2)); // 1e-2

    // Measurement noise is important, it defines how much can we trust to the
    // measurement and has direct effect on the smoothness of tracking window
    // - increase this tracking gets smoother
    // - decrease this and tracking window becomes almost same with detection window
    cv::setIdentity(filter->measurementNoiseCov, Scalar::all(1e-1)); // 1e-1
    cv::setIdentity(filter->errorCovPost, Scalar::all(1));

    // we are tracking 4 points, thus having 4 states: corners of rectangle
    filter->statePost.at<float>(0, 0) = initRect.xmin;
    filter->statePost.at<float>(1, 0) = initRect.ymin;
    filter->statePost.at<float>(2, 0) = initRect.xmax;
    filter->statePost.at<float>(3, 0) = initRect.ymax;
}

void Kalman::update(Box& box)
{
    // Tracking 4 points
    cv::Mat measurement(M, 1, CV_32F);
    measurement.at<float>(0, 0) = box.xmin;
    measurement.at<float>(1, 0) = box.ymin;
    measurement.at<float>(2, 0) = box.xmax;
    measurement.at<float>(3, 0) = box.ymax;

    const cv::Mat& statePost = filter->correct(measurement);

    // update rectangle
    setRectFrom(statePost);
}

void Kalman::predict()
{
    const cv::Mat& statePrior = filter->predict();
    // update rectangle
    setRectFrom(statePrior);
}

void Kalman::setRectFrom(const cv::Mat& state)
{
    cur_box.xmin = state.at<float>(0, 0);
    cur_box.ymin = state.at<float>(1, 0);
    cur_box.xmax = state.at<float>(2, 0);
    cur_box.ymax = state.at<float>(3, 0);
}