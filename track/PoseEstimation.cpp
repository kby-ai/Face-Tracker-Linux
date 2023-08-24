#include "PoseEstimation.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
//#include "../face_pose/base_func.h"
using namespace std;

void get_face_pose(FaceBox_ box, float* landmark, float* pose)
{
    float fw = box.x2 - box.x1;
    float fh = box.y2 - box.y1;
    float sz = pow(fw * fh, 0.5);
    float cx = (box.x2 + box.x1) / 2;
    float cy = (box.y2 + box.y1) / 2;
    float x1 = cx - sz / 2;
    float y1 = cy - sz / 2;

//    getPose68(x1, y1, sz, landmark, 68, pose, pose + 1, pose + 2);
}