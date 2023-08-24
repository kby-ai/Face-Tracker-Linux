/* face tracking PC application SDK */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include "detection/face_detector.h"
#include "face_tracker.h"

#include <string>
#include <sstream>

using std::vector;
using namespace cv;
using namespace std;

typedef struct {
	std::vector<int> id_vector;
	std::vector<cv::Mat> img_vector;
} TrackData;

void DrawDetection(cv::Mat& img, std::vector<FaceInfo> boxes){
	if (boxes.size() == 0) {
		//cv::imshow("image", img);
	}
	else {
		for (int i = 0; i < boxes.size(); i++) {
			cv::Point topleft;
			cv::Point bottomright;
			topleft.x = boxes[i].x1;
			topleft.y = boxes[i].y1;
			bottomright.x = boxes[i].x2;
			bottomright.y = boxes[i].y2;
			std::cout << topleft.x << std::endl;
			cv::rectangle(img, topleft, bottomright, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
			cv::imshow("image", img);
		}
	}
}

TrackData GetTrackData(cv::Mat& img, std::vector<Track> tracks, TrackData& track_buf) {
	TrackData data;
	if (tracks.size() != 0) {
		if (track_buf.id_vector.size() == 0) {
			for (int i = 0; i < tracks.size(); i++) {			
				cv::Point topleft;
				cv::Point bottomright;
				topleft.x = tracks[i].x1;
				topleft.y = tracks[i].y1;
				bottomright.x = tracks[i].x2;
				bottomright.y = tracks[i].y2;

				cv::Mat croppedImg;
				int width = (int)(bottomright.x - topleft.x);
				int height = (int)(bottomright.y - topleft.y);
				if (width != 0 || height != 0) {
					if ((bottomright.x < img.cols) && (bottomright.y < img.rows) && (topleft.x > 0 && topleft.y > 0)) {
						img(cv::Rect(topleft.x, topleft.y, width, height)).copyTo(croppedImg);
						data.id_vector.push_back(tracks[i].id);
						data.img_vector.push_back(croppedImg);
					}
				}
			}
			for (int i = 0; i < data.id_vector.size(); i++) {
				track_buf.id_vector.push_back(data.id_vector[i]);
				track_buf.img_vector.push_back(data.img_vector[i]);
			}
		}
		else {
			for (int i = 0; i < tracks.size(); i++) {
				if (tracks[i].id != track_buf.id_vector[i]) {
					cv::Point topleft;
					cv::Point bottomright;
					topleft.x = tracks[i].x1;
					topleft.y = tracks[i].y1;
					bottomright.x = tracks[i].x2;
					bottomright.y = tracks[i].y2;

					cv::Mat croppedImg;
					int width = (int)(bottomright.x - topleft.x);
					int height = (int)(bottomright.y - topleft.y);
					if (width != 0 || height != 0) {
						if ((bottomright.x < img.cols) && (bottomright.y < img.rows) && (topleft.x > 0 && topleft.y > 0)) {
							img(cv::Rect(topleft.x, topleft.y, width, height)).copyTo(croppedImg);
							data.id_vector.push_back(tracks[i].id);
							data.img_vector.push_back(croppedImg);						
						}
					}
				}
			}

			track_buf.id_vector.clear();
			track_buf.img_vector.clear();
			for (int i = 0; i < data.id_vector.size(); i++) {
				track_buf.id_vector.push_back(data.id_vector[i]);
				track_buf.img_vector.push_back(data.img_vector[i]);
			}
		}		
	}
	return data;
}

void WriteToFile(TrackData& data) {
	if (data.id_vector.size() != 0) {
		for (int i = 0; i < data.id_vector.size(); i++) {
			cv::String filename = "./" + std::to_string(data.id_vector[i]) + ".jpg";
			cv::imwrite(filename, data.img_vector[i]);
		}
	}
}

void DrawTrack(cv::Mat& img, std::vector<Track> tracks) {
	if (tracks.size() == 0) {
		cv::imshow("image", img);
	}
	else {
		for (int i = 0; i < tracks.size(); i++) {
			cv::Point topleft;
			cv::Point bottomright;
			topleft.x = tracks[i].x1;
			topleft.y = tracks[i].y1;
			bottomright.x = tracks[i].x2;
			bottomright.y = tracks[i].y2;
			std::cout << topleft.x << std::endl;
			cv::rectangle(img, topleft, bottomright, cv::Scalar(255, 255, 0), 2, cv::LINE_8);
			cv::putText(img, std::to_string(tracks[i].id), topleft,
				cv::FONT_HERSHEY_PLAIN, 2.0,
				cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
			cv::imshow("image", img);
		}
	}
}

void SaveTrack(cv::Mat& img, std::vector<Track> tracks, cv::VideoWriter& oVideoWriter) {
	if (tracks.size() == 0) {
		oVideoWriter.write(img);
	}
	else {
		for (int i = 0; i < tracks.size(); i++) {
			cv::Point topleft;
			cv::Point bottomright;
			topleft.x = tracks[i].x1;
			topleft.y = tracks[i].y1;
			bottomright.x = tracks[i].x2;
			bottomright.y = tracks[i].y2;
			std::cout << topleft.x << std::endl;
			cv::rectangle(img, topleft, bottomright, cv::Scalar(255, 255, 0), 2, cv::LINE_8);
			cv::putText(img, std::to_string(tracks[i].id), topleft,
				cv::FONT_HERSHEY_PLAIN, 2.0,
				cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
			oVideoWriter.write(img);
		}
	}
}

int VideoFileInferenceRealTimeView(int total_frame, std::vector<TrackData>& track_data) {

	cv::VideoCapture cap("./video4.mp4");

	if (!cap.isOpened()) {
		std::cout << "No video stream detected" << std::endl;
		system("pause");
		return -1;
	}

	//get the frames rate of the video
	double fps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "Frames per seconds : " << fps << std::endl;

	UltraFace detector("./weights/detection/RFB-320.bin", "./weights/detection/RFB-320.param", 320, 240, 1, 0.7);

	FaceTracker tracker;
	int __ = tracker.LoadThirdPartyModels();
	int frame_cnt = 0;
	TrackData track_buf;
	while (true) {
		cv::Mat frame;
		std::vector<FaceInfo> boxes;
		bool bSuccess = cap.read(frame); // read a new frame from video 
		if (bSuccess == false){
			std::cout << "Found the end of the video" << std::endl;
			break;
		}
		frame_cnt++;
		ncnn::Mat inmat = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);
		detector.detect(inmat, boxes);
		//DrawDetection(frame, boxes);

		std::vector<Track> tracks;

		tracker.Get_Track(frame, boxes, tracks);

		//TrackData contains one cropped face image per person, this can be used later for recognition purpose
		if (boxes.size() == tracks.size()) {
			track_data.push_back(GetTrackData(frame, tracks, track_buf));
		}
			
		//DrawTrack(frame, tracks);
		if (frame_cnt == total_frame)
			break;
		char c = (char)cv::waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition
		if (c == 27) { //If 'Esc' is entered break the loop//
			break;
		}
	}
	cap.release();//Releasing the buffer memory//
	return 0;

}

int VideoFileInferenceSaveVideo() {

	cv::VideoCapture cap("./video4.mp4");

	if (!cap.isOpened()) {
		std::cout << "No video stream detected" << std::endl;
		system("pause");
		return -1;
	}

	//get the frames rate of the video
	double fps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "Frames per seconds : " << fps << std::endl;

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video
    int frames_per_second = 25;
    Size frame_size(frame_width, frame_height);

    //Create and initialize the VideoWriter object 
    cv::VideoWriter oVideoWriter("./TrackingVideo.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 
                                                               frames_per_second, frame_size, true); 
    
    //If the VideoWriter object is not initialized successfully, exit the program
    if (oVideoWriter.isOpened() == false) 
    {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

	UltraFace detector("./weights/detection/RFB-320.bin", "./weights/detection/RFB-320.param", 320, 240, 1, 0.7);

	FaceTracker tracker;
	int __ = tracker.LoadThirdPartyModels();
	int frame_cnt = 0;
	while (true) {
		cv::Mat frame;
		std::vector<FaceInfo> boxes;
		bool bSuccess = cap.read(frame); // read a new frame from video 
		if (bSuccess == false){
			std::cout << "Found the end of the video" << std::endl;
			break;
		}
		frame_cnt++;
		ncnn::Mat inmat = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);
		detector.detect(inmat, boxes);

		std::vector<Track> tracks;

		tracker.Get_Track(frame, boxes, tracks);
		SaveTrack(frame, tracks, oVideoWriter);
		char c = (char)cv::waitKey(25);//Allowing 25 milliseconds frame processing time and initiating break condition
		if (c == 27) { //If 'Esc' is entered break the loop//
			break;
		}
	}
	cap.release();//Releasing the buffer memory//
    oVideoWriter.release();//Flush and close the video file
	return 0;

}

int main(int argc, char** argv){	
	int total_frame = 100;
	int video_out_index = 0;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Have " << argc << " arguments:" << std::endl;
	for (int i = 0; i < argc; ++i)
		std::cout << i << "  " << argv[i] << std::endl;
	std::cout << "**************************************************\n" << std::endl;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "--video_out")){
			if (!strcmp(argv[i + 1], "true"))
				video_out_index = 1;
			else
				video_out_index = 0;
				if (!strcmp(argv[i], "--frame_num"))
					total_frame = std::atoi(argv[i + 1]);
		}
	}

	std::vector<TrackData> track_data;
	if (video_out_index == 1)
		int _ = VideoFileInferenceSaveVideo();
	else	
		int _ = VideoFileInferenceRealTimeView(total_frame, track_data);
		for(int i = 0; i < track_data.size(); i++)
			WriteToFile(track_data[i]);
	return 0;
}
