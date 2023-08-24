#include "metrics.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

cv::Mat calculate_iou(cv::Mat bboxes1, cv::Mat bboxes2, int dim)
{
    int h1 = bboxes1.rows;
    int w1 = bboxes1.cols;
    int h2 = bboxes2.rows;
    int w2 = bboxes2.cols;
    
    cv::Mat iou = cv::Mat::zeros(h1, h2, CV_32F);

    cv::Mat coords_b1(w1, h1, CV_32F);
    cv::Mat coords_b2(w2, h2, CV_32F);
    
    for (int j = 0; j < w1; j++)
        for (int i = 0; i < h1; i++)
            coords_b1.at<float>(j, i) = bboxes1.at<float>(i, j);
    
    for (int j = 0; j < w2; j++)
        for (int i = 0; i < h2; i++)
            coords_b2.at<float>(j, i) = bboxes2.at<float>(i, j);

    cv::Mat val_inter = cv::Mat::ones(h1, h2, CV_32F);
    cv::Mat val_b1 = cv::Mat::ones(1, h1, CV_32F);
    cv::Mat val_b2 = cv::Mat::ones(1, h2, CV_32F);
    
    for (int k = 0; k < dim; k++)
    {
        cv::Mat coords1(h1, h2, CV_32F);
        cv::Mat coords2(h1, h2, CV_32F);
        for (int i = 0; i < h1; i++)
        {
            for (int j = 0; j < h2; j++)
            {
                coords1.at<float>(i, j) = max(coords_b1.row(k).at<float>(i), coords_b2.row(k).at<float>(j));
                coords2.at<float>(i, j) = min(coords_b1.row(k + dim).at<float>(i), coords_b2.row(k + dim).at<float>(j));
                val_inter.at<float>(i, j) *= max(coords2.at<float>(i, j) - coords1.at<float>(i, j), (float)0.);
            }
        }

        for (int i = 0; i < h1; i++)
            val_b1.at<float>(0, i) *= (coords_b1.row(k + dim).at<float>(i) - coords_b1.row(k).at<float>(i));

        for (int j = 0; j < h2; j++)
            val_b2.at<float>(0, j) *= (coords_b2.row(k + dim).at<float>(j) - coords_b2.row(k).at<float>(j));
    }

    for (int i = 0; i < h1; i++)
        for (int j = 0; j < h2; j++)
            iou.at<float>(i, j) = val_inter.at<float>(i, j) / (max(val_b1.at<float>(i) + val_b2.at<float>(j) - val_inter.at<float>(i, j), (float)0.) + EPS);

    return iou;
}

float get_similarity(float* feat1, float* feat2)
{
    float similarity = 0;
    for (int i = 0; i < 256; i++)
        similarity += feat1[i] * feat2[i];

    similarity += 1;
    return similarity * 50;
}