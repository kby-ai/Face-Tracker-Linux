#include "face_tracker.h"
#include "track/PoseEstimation.h"
//#include "utils/base_func.h"
#include "face_feature/face_feature.h"
#include "face_modeling/face_landmark.h"

static int track_cnt = 0;
FaceTracker::FaceTracker(){
    tracker = new MultiObjectTracker(1/15.0);
    features_saved.clear();
}

FaceTracker::~FaceTracker() {
    if(tracker)
        free(tracker);

    features_saved.clear();
}

int FaceTracker::LoadThirdPartyModels()
{
    init_landmark_vanila();
    feature_extractor = new FaceFeatureExtractor();
    feature_extractor->LoadModel();
    return 0;
}

int FaceTracker::ReleaseThirdPartyModels()
{
    if(feature_extractor)
    {
        free(feature_extractor);
    }

    deinit_landmark_vanila();
    return 0;
}

int FaceTracker::Get_Track(cv::Mat& src, vector<FaceInfo> dets, vector<Track>& tracks)
{
    vector<Detection*> detections;

    for (int i = 0; i < dets.size(); i++)
    {
        Detection* det = new Detection({dets[i].x1, dets[i].y1, dets[i].x2, dets[i].y2}, dets[i].score);
        detections.push_back(det);
    }

    tuple<vector<int>, map<int, int> > ret = tracker->step(detections);
    vector<int> difference_list = get<0>(ret);
    map<int, int> index_dict = get<1>(ret);

    if (index_dict.size() > 0)
    {
        vector<float*> landmarks;
        vector<float*> poses;
        cv::Mat src_gray;
        cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

        for (auto idx : difference_list)
        {
            float* landmark = new float[136];
            get_face_68Landmark(src_gray.data, src_gray.cols, src_gray.rows, dets[idx].x1, dets[idx].y1,
                                dets[idx].x2 - dets[idx].x1, dets[idx].y2 - dets[idx].y1, landmark);

            landmarks.push_back(landmark);
        }

        float threshold = 70;
        int id = 0;
        bool id_found = false;

        for (int idx = 0; idx < landmarks.size(); idx++)
        {
            int tracker_idx = index_dict.at(difference_list[idx]);

            float feature[256];
            feature_extractor->get_face_feature(src, landmarks[idx], feature);
            int features_size = features_saved.size();
            if (features_size && 0)
            {
                for (int idx1 = 0; idx1 < features_size; idx1++)
                {
                    float score = get_similarity(feature, features_saved[idx1]);
                    printf("@@@   SCORE      %f\n", score);
                    if (score >= threshold)
                    {
                        id_found = true;
                        tracker->trackers[tracker_idx]->id = idx1;
                        break;
                    }
                }

                if (id_found == false)
                {
                    tracker->trackers[tracker_idx]->id = features_saved.size();
                    printf("####  newly added id = %d\n", features_saved.size());
                    float* added_feature = new float[256];
                    for (int i = 0; i < 256; i++)
                        added_feature[i] = feature[i];
                    features_saved.push_back(added_feature);
                }
            }
            else
            {
                tracker->trackers[tracker_idx]->id = track_cnt;
                track_cnt++;
            }
        }

        tracker->remove_invalid();
        for (auto ptr : landmarks)
            free(ptr);

        for (auto ptr1 : poses)
            free(ptr1);
    }

    tracks = tracker->active_tracks(3);
    return 0;
}
