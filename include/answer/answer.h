//
// Created by cmyhj on 24-2-16.
//

#ifndef ANSWER_ANSWER_H
#define ANSWER_ANSWER_H

#include <iostream>
#include <thread>
#include <cmath>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point32.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include "image_processing/image_processing.h"

class Answer : public rclcpp::Node {
private:
    rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr clickPointPublisher;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr imageSubscription;

    void imageCallback(sensor_msgs::msg::Image::SharedPtr rosImage);

    geometry_msgs::msg::Point32 clickPoint;

    float delayDistance;//调整延时
    int delayCalculateFlag;//计算延时状态机
    float shortestDistance;//计算延时的最近距离
    cv::Vec4i lastLine;//存储上一帧的直线，若这一帧没有直线，则使用上一帧的直线



public:
    Answer();
};

#endif //ANSWER_ANSWER_H
