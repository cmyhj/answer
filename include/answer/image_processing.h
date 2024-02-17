//
// Created by cmyhj on 24-2-17.
//

#ifndef ANSWER_IMAGE_PROCESSING_H
#define ANSWER_IMAGE_PROCESSING_H

#include <iostream>
#include <thread>
#include <cmath>

#include <geometry_msgs/msg/point32.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

class imageProcessing
{
private:
    cv::Mat src;
    cv::Mat dst;
public:
    imageProcessing(cv::Mat originImage);
    cv::Mat onlyClickImage();
};


#endif //ANSWER_IMAGE_PROCESSING_H
