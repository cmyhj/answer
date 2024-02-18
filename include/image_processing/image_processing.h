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
#include <rclcpp/rclcpp.hpp>

class imageProcessing
{
private:
    cv::Mat src;
    cv::Mat dst;
    cv::Vec4i lines;
    //std::vector<cv::Vec4i> lines; // 存储检测到的直线
public:
    explicit imageProcessing(cv::Mat && originImage);
    std::vector<std::vector<cv::Point>> centerCounts();
    [[nodiscard]] float distanceToLine(cv::Point2f clickCenter) const;
};


#endif //ANSWER_IMAGE_PROCESSING_H
