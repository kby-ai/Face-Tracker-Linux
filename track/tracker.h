#pragma once

#include "metrics.h"
#include "kalman.h"
#include "../definition.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class SingleObjectTracker
{
public:
    SingleObjectTracker(Box box0, float score0_, float max_staleness_ = 12.0, float smooth_score_gamma_ = 0.8,
                        float smooth_feature_gamma_ = 0.9);
    ~SingleObjectTracker();
    Box box();
    bool is_invalid();
    void _predict();
    void _update_box(Detection detection_);

    void predict();
    void update(Detection detection_);
    float stale(float rate = 1.0);
    float unstale(float rate = 2.0);
    bool is_stale();

public:
    int id;
    float score;
    float staleness;
    int steps_positive;
    int steps_alive;
    bool invalid;

private:
    float max_staleness;
    float smooth_score_gamma;
    float smooth_feature_gamma;

    Kalman* _tracker;
};

class MultiObjectTracker
{
public:
    MultiObjectTracker(float dt, int order_pos = 1, int dim_pos = 2,
                       int order_size = 0, int dim_size = 2,
                       float q_var_pos = 5000., float r_var_pos = 0.1);
    ~MultiObjectTracker();

    vector<Track> active_tracks(float max_staleness_to_positive_ratio = 3.0,
                                float max_staleness = 999,
                                int min_steps_alive = -1);
    void cleanup_trackers();
    void remove_invalid();
    tuple<vector<int>, map<int, int> > step(const vector<Detection*> detections);

public:
    vector<SingleObjectTracker*> trackers;
};

tuple<cv::Mat, cv::Mat> cost_matrix_iou_feature(vector<SingleObjectTracker*> trackers,
                                                vector<Detection*> detections);

vector<pair_track_det_idx> IOUAndFeatureMatchingFunction(vector<SingleObjectTracker*> trackers, vector<Detection*> detections, float min_iou = 0.1, float multi_match_min_iou = 1. + EPS);

void get_face_pose(FaceBox_ box, float* landmark, float* pose);