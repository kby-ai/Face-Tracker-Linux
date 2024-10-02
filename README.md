<p align="center">
  <a href="https://play.google.com/store/apps/dev?id=7086930298279250852" target="_blank">
    <img alt="" src="https://github-production-user-asset-6210df.s3.amazonaws.com/125717930/246971879-8ce757c3-90dc-438d-807f-3f3d29ddc064.png" width=500/>
  </a>  
</p>

#### 📚 Product & Resources - [Here](https://github.com/kby-ai/Product)
#### 🛟 Help Center - [Here](https://docs.kby-ai.com)
#### 💼 KYC Verification Demo - [Here](https://github.com/kby-ai/KYC-Verification-Demo-Android)
#### 🙋‍♀️ Docker Hub - [Here](https://hub.docker.com/u/kbyai)

# Face Tracking C++ Demo on Linux
## Overview
We implemented real-time face-tracking technology in this demo project.
This project can be built on Ubuntu 22.04 and model inference was implemented for web camera and real-time video streaming

You can visit our YouTube video for our face tracking SDK's performance [here](https://www.youtube.com/watch?v=FVo7PPVBgfQ) to see how well our demo app works.</br></br>
[![Face Tracking Demo](https://img.youtube.com/vi/FVo7PPVBgfQ/0.jpg)](https://www.youtube.com/watch?v=FVo7PPVBgfQ)</br>

## Install dependencies
- Build OpenCV on your platform, follow the steps in the link below.
https://gist.github.com/kleysonr/c0752306bb6c021a1ff3c448996636ee
- Install vulkan sdk with the command "./install.sh"

## How to build
Go to the project folder and enter the following command.
./build.sh

## How to run
Go to the build folder and run with the below command 
  ```bash
  ./<executive file name> --frame_num <frame number>
  ```
for example:
  ```bash
    ./face_track --frame_num 150
  ```	
If you need to save a video file with tracking, you can run the command below
  ```bash
    ./<executive file name> --video_out true
  ```
for example:
  ```bash
    ./face_track --video_out true
  ```	
TrackData contains one cropped face image per person, this can be used later for recognition purpose
  ```bash
    TrackData track_data = GetTrackData(frame, tracks);
  ```
## Improvement

This project can be improved by using KBY-AI's face recognition SDK [here](https://github.com/kby-ai/Face-Recognition-SDK).

- To request SDK, please contact us:</br>
🧙`Email:` contact@kby-ai.com</br>
🧙`Telegram:` [@kbyai](https://t.me/kbyai)</br>
🧙`WhatsApp:` [+19092802609](https://wa.me/+19092802609)</br>
🧙`Skype:` [live:.cid.66e2522354b1049b](https://join.skype.com/invite/OffY2r1NUFev)</br>
🧙`Facebook:` https://www.facebook.com/KBYAI</br>

[![Visitors](https://api.visitorbadge.io/api/combined?path=https%3A%2F%2Fgithub.com%2Fkby-ai%2FFace-Tracker-Linux&countColor=%23263759)](https://visitorbadge.io/status?path=https%3A%2F%2Fgithub.com%2Fkby-ai%2FFace-Tracker-Linux)
