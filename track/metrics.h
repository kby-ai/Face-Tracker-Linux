#pragma once

#include "data_def.h"

#include <vector>
#include <string>

#define EPS 1e-7

cv::Mat calculate_iou(cv::Mat bboxes1, cv::Mat bboxes2, int dim = 2);
float get_similarity(float* feat1, float* feat2);