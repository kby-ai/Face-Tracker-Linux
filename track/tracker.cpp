#include "tracker.h"
#include "Hungarian.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <set>
#include <map>

//#define DEBUG printf
#define DEBUG { }
/**
 * Kalman constructor: Parameters of the filter are set in here.
 * These parameters have a direct effect on the behaviour pf the filter.
 */
using namespace std;

float exponential_moving_average_fn(float gamma_, float old_, float new_)
{
    return (gamma_ * old_ + (1 - gamma_) * new_);
}

SingleObjectTracker::SingleObjectTracker(Box box0, float score0_, float max_staleness_, float smooth_score_gamma_, float smooth_feature_gamma_)
{
    id = 0;
    steps_alive = 1;
    steps_positive = 1;
    staleness = 0.0;
    smooth_score_gamma = smooth_score_gamma_;
    smooth_feature_gamma = smooth_feature_gamma_;
    max_staleness = max_staleness_;
    score = score0_;
    invalid = false;
    _tracker = new Kalman(box0);
    printf("[%s]  creating new tracker %d\n", __func__, id);
}

SingleObjectTracker::~SingleObjectTracker()
{
    if (_tracker)
        delete _tracker;
}

void SingleObjectTracker::_predict()
{
    _tracker->predict();
}

void SingleObjectTracker::predict()
{
    _predict();
    steps_alive++;
}

void SingleObjectTracker::_update_box(Detection detection_)
{
    Box box = detection_.box;
    _tracker->update(box);
}

void SingleObjectTracker::update(Detection detection_)
{
    _update_box(detection_);
    steps_positive++;
    score = exponential_moving_average_fn(smooth_score_gamma, score, detection_.score);
    unstale(3);
}

Box SingleObjectTracker::box()
{
    Box box = _tracker->cur_box;
    return box;
}

bool SingleObjectTracker::is_invalid()
{
    return false;
}

float SingleObjectTracker::stale(float rate)
{
    staleness += rate;
    return staleness;
}

float SingleObjectTracker::unstale(float rate)
{
    staleness = max((float)0., staleness);
    return staleness;
}

bool SingleObjectTracker::is_stale()
{
    return staleness >= max_staleness;
}

MultiObjectTracker::MultiObjectTracker(float dt, int order_pos, int dim_pos, int order_size, int dim_size, float q_var_pos, float r_var_pos)
{
}

MultiObjectTracker::~MultiObjectTracker()
{
}

vector<Track> MultiObjectTracker::active_tracks(float max_staleness_to_positive_ratio, float max_staleness, int min_steps_alive)
{
    vector<Track> tracks;
    tracks.clear();
    for (auto tracker : trackers)
    {
        bool cond1 = tracker->staleness / tracker->steps_positive < max_staleness_to_positive_ratio;
        bool cond2 = tracker->staleness < max_staleness;
        bool cond3 = tracker->steps_alive >= min_steps_alive;

        if (cond1 && cond2 && cond3)
        {
            Track t;
            t.id = tracker->id;
            t.x1 = tracker->box().xmin;
            t.x2 = tracker->box().xmax;
            t.y1 = tracker->box().ymin;
            t.y2 = tracker->box().ymax;
            t.score = tracker->score;
            tracks.push_back(t);
        }
    }

    DEBUG("active/all tracks: %d/%d\n", trackers.size(), tracks.size());
    return tracks;
}

void MultiObjectTracker::cleanup_trackers()
{
    int count_before = trackers.size();
    for (int i = 0; i < count_before; i++)
    {
        if (trackers[i]->is_stale() || trackers[i]->is_invalid())
        {
            delete trackers[i];
            trackers.erase(trackers.begin() + i);
        }
    }

    int count_after = trackers.size();
    DEBUG("cleanup %d/%d trackers", count_before - count_after, count_before);
}

void MultiObjectTracker::remove_invalid()
{
    int count_before = trackers.size();
    for (int i = 0; i < count_before; i++)
    {
        if (trackers[i]->invalid)
        {
            delete trackers[i];
            trackers.erase(trackers.begin() + i);
        }
    }

    int count_after = trackers.size();
    DEBUG("remove %d/%d trackers", count_before - count_after, count_before);
}

tuple<vector<int>, map<int, int> > MultiObjectTracker::step(const vector<Detection*> detections)
{
    /*the method matches the new detections with existing trackers,
    creates new trackers if necessaryand performs the cleanup.
    Returns the active tracks after active filtering applied*/
    vector<Track> tracks;
    for (int i = 0; i < trackers.size(); i++)
        trackers[i]->predict();

    //match trackers with detections
    DEBUG("step with %d detections\n", detections.size());
    vector<pair_track_det_idx> matches = IOUAndFeatureMatchingFunction(trackers, detections);
    DEBUG("matched %d pairs\n", matches.size());
    //assigned trackers : correct
    for (auto match : matches)
    {
        int track_idx = match.track_idx;
        int det_idx = match.det_idx;
        trackers[track_idx]->update(*detections[det_idx]);
    }
    //not assigned detections : create new trackers POF
    set<int> assigned_det_idxs;
    set<int> detections_idxs;
    for (int i = 0; i < matches.size(); i++)
        assigned_det_idxs.insert(matches[i].det_idx);

    for (int i = 0; i < detections.size(); i++)
        detections_idxs.insert(i);

    std::vector<int> difference_list(256); // 0  0  0  0  0  0  0  0  0  0
    std::vector<int>::iterator it, it1;

    it = std::set_difference(detections_idxs.begin(), detections_idxs.end(), assigned_det_idxs.begin(), assigned_det_idxs.end(), difference_list.begin());
    difference_list.resize(it - difference_list.begin());
    map<int, int> index_dict;
    map<int, int> index_dict1;

    for (auto det_idx : difference_list)
    {
        Detection* det = detections[det_idx];
        SingleObjectTracker* tracker = new SingleObjectTracker(det->box, det->score);
        index_dict.insert({det_idx, trackers.size()});
        trackers.push_back(tracker);
    }

    //unassigned trackers
    set<int> assigned_track_idxs;
    set<int> trackers_idxs;

    for (int i = 0; i < matches.size(); i++)
        assigned_track_idxs.insert(matches[i].track_idx);

    for (int i = 0; i < trackers.size(); i++)
        trackers_idxs.insert(i);

    //set<int>::iterator itr;
    //for (itr = trackers_idxs.begin(); itr != trackers_idxs.end(); ++itr)
    //{
    //    int ans = *itr;
    //    cout << ans << ", ";
    //}
    //cout << endl;
    std::vector<int> difference_list1(256); // 0  0  0  0  0  0  0  0  0  0
    it1 = std::set_difference(trackers_idxs.begin(), trackers_idxs.end(), assigned_track_idxs.begin(), assigned_track_idxs.end(), difference_list1.begin());
    difference_list1.resize(it1 - difference_list1.begin());
    for (auto track_idx : difference_list1)
        trackers[track_idx]->stale();

    //cleanup dead trackers
    //cleanup_trackers();

    int count_before = trackers.size();
    if (index_dict.size() > 0)
    {
        int idx = 0;
        map<int, int> tmp_dict;
        for (int i = 0; i < count_before; i++)
        {
            if (!(trackers[i]->is_stale() || trackers[i]->is_invalid()))
            {
                tmp_dict.insert({i, idx});
                idx++;
            }
        }

        for (auto index : index_dict)
        {
            int key = index.first;
            int val = index.second;
            for (auto tmp_idx : tmp_dict)
            {
                if (val == tmp_idx.first)
                    index_dict1.insert({key, tmp_dict.at(val)});
            }
        }
    }

    for (int i = 0; i < trackers.size(); i++)
        if (trackers[i]->is_stale() || trackers[i]->is_invalid())
        {
            trackers.erase(trackers.begin() + i);
        }

    int count_after = trackers.size();
    DEBUG("  before: %d\n", count_before);
    DEBUG("  after: %d\n", count_after);
    active_tracks();
    tuple<vector<int>, map<int, int> > ret;
    get<0>(ret) = difference_list;
    get<1>(ret) = index_dict1;
    return ret;
}

tuple<cv::Mat, cv::Mat> cost_matrix_iou_feature(vector<SingleObjectTracker*> trackers,
                                                vector<Detection*> detections)
{
    tuple<cv::Mat, cv::Mat> ret;
    int tracker_sz = trackers.size();
    int det_sz = detections.size();
    cv::Mat b1;
    cv::Mat b2;
    for (int i = 0; i < tracker_sz; i++)
    {
        float content[1][4] = {{trackers[i]->box().xmin, trackers[i]->box().ymin, trackers[i]->box().xmax, trackers[i]->box().ymax}};
        cv::Mat box(1, 4, CV_32F, content);
        b1.push_back(box);
    }

    for (int i = 0; i < det_sz; i++)
    {
        float content[1][4] = {detections[i]->box.xmin, detections[i]->box.ymin, detections[i]->box.xmax, detections[i]->box.ymax};
        cv::Mat box(1, 4, CV_32F, content);
        b2.push_back(box);
    }

    int inferred_dim = int(b1.cols / 2);
    cv::Mat iou_mat = calculate_iou(b1, b2, inferred_dim);
    cv::Mat cost_mat = iou_mat * (-1);
    get<0>(ret) = cost_mat;
    get<1>(ret) = iou_mat;
    return ret;
}

vector<pair_track_det_idx> IOUAndFeatureMatchingFunction(vector<SingleObjectTracker*> trackers, vector<Detection*> detections, float min_iou, float multi_match_min_iou)
{
    vector<pair_track_det_idx> matchs;
    matchs.clear();
    if (trackers.size() == 0 || detections.size() == 0)
        return matchs;

    tuple<cv::Mat, cv::Mat> ret;
    ret = cost_matrix_iou_feature(trackers, detections);
    cv::Mat cost_mat = get<0>(ret);
    cv::Mat iou_mat = get<1>(ret);
    vector<vector<double> > iou_vec(cost_mat.rows);
    for (int i = 0; i < cost_mat.rows; i++)
        cost_mat.row(i).reshape(1, 1).copyTo(iou_vec[i]);

    HungarianAlgorithm HungAlgo;
    vector<int> assignment;

    double cost = HungAlgo.Solve(iou_vec, assignment);
    //cout << cost_mat << endl;
    for (int x = 0; x < iou_vec.size(); x++)
    {
        if (assignment[x] < 0)
            continue;

        if (iou_mat.at<float>(x, assignment[x]) >= min_iou)
        {
            pair_track_det_idx pair = {x, assignment[x]};
            matchs.push_back(pair);
        }

        if (multi_match_min_iou < 1.0)
        {
            for (int i = 0; i < iou_mat.cols; i++)
            {
                if (i != assignment[x] && (iou_mat.at<float>(x, i) > multi_match_min_iou))
                {
                    pair_track_det_idx pair1 = {x, i};
                    matchs.push_back(pair1);
                }
            }
        }
    }

    return matchs;
}