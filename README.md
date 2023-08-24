<p align="center">
  <a href="https://play.google.com/store/apps/dev?id=7086930298279250852" target="_blank">
    <img alt="" src="https://github-production-user-asset-6210df.s3.amazonaws.com/125717930/246971879-8ce757c3-90dc-438d-807f-3f3d29ddc064.png" width=500/>
  </a>  
</p>

# Face Tracking C++ Demo on Linux
## Overview
We implemented real-time face-tracking technology in this demo project.
This project can be built on Ubuntu 22.04 and model inference was implemented for web camera and real-time video streaming



https://github.com/kby-ai/KBY-AI-Technology-Face-Tracker-Windows/assets/125717930/35940547-eda8-464d-83cc-75dd6246cc9d

## Install dependencies
- Build OpenCV on your platform, follow the steps in the link below.
https://gist.github.com/kleysonr/c0752306bb6c021a1ff3c448996636ee
- Install vulkan sdk with the command "./install.sh"

## How to build
Go to the project folder and enter the following command.
./build.sh

## How to run
Go to the build folder and run with the below command 
  ```shell
  ./<executive file name> --frame_num <frame number>
  ```
for example:
  ```shell
    ./face_track --frame_num 150
  ```	
If you need to save video file with tracking, you can run the command below
  ```shell
    ./<executive file name> --video_out true
  ```
for example:
  ```shell
    ./face_track --video_out true
  ```	
TrackData contains one cropped face image per person, this can be used later for recognition purpose
	```shell
    TrackData track_data = GetTrackData(frame, tracks);
  ```
## Improvement

This project can be improved by using kby-ai's face recognition SDK. The SDK requires a license.

- To request a license, please contact us:
```
Email: contact@kby-ai.com

Telegram: @kbyai

WhatsApp: +19092802609

Skype: live:.cid.66e2522354b1049b
```
