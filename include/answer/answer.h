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

#include "image_processing.h"

class Answer : public rclcpp::Node {
private:
    rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr clickPointPublisher;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr imageSubscription;
    rclcpp::TimerBase::SharedPtr timer_;
    geometry_msgs::msg::Point32 clickPoint;

    void imageCallback(const sensor_msgs::msg::Image::SharedPtr rosImage);
    void timer_callback();

public:
    Answer();
};

#endif //ANSWER_ANSWER_H
