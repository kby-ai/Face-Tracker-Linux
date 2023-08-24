cmake_minimum_required(VERSION 3.5)

project(face_track LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-Wall -pthread")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17 -lstdc++")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})

find_package(OpenCV REQUIRED)

add_library(NCNN SHARED IMPORTED)
set_target_properties(NCNN PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/ncnn-20210720-ubuntu-2004-shared/lib/libncnn.so)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/ncnn-20210720-ubuntu-2004-shared/include/ncnn
    ${CMAKE_SOURCE_DIR}
    )

add_executable(
    ${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/face_tracker.cpp
    ${CMAKE_SOURCE_DIR}/detection/face_detector.cpp
    ${CMAKE_SOURCE_DIR}/face_feature/face_feature.cpp
    ${CMAKE_SOURCE_DIR}/face_modeling/face_landmark.cpp
    ${CMAKE_SOURCE_DIR}/face_modeling/vanila_model.cpp
    ${CMAKE_SOURCE_DIR}/track/Hungarian.cpp
    ${CMAKE_SOURCE_DIR}/track/kalman.cpp
    ${CMAKE_SOURCE_DIR}/track/metrics.cpp
    ${CMAKE_SOURCE_DIR}/track/PoseEstimation.cpp
    ${CMAKE_SOURCE_DIR}/track/tracker.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    NCNN
    stdc++fs
    -ldl
    )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
