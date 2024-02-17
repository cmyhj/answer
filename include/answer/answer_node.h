//
// Created by cmyhj on 24-2-16.
//

#ifndef ANSWER_ANSWER_NODE_H
#define ANSWER_ANSWER_NODE_H

#include <iostream>
#include <thread>
#include <math.h>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point32.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std::chrono_literals;

class Answer : public rclcpp::Node {
private:
    rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr clickPointPublisher;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr imageSubscription;
    rclcpp::TimerBase::SharedPtr timer_;
    geometry_msgs::msg::Point32 clickPoint;
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr rosImage) {
        cv_bridge::CvImagePtr cvImage;
        cvImage = cv_bridge::toCvCopy(rosImage, sensor_msgs::image_encodings::BGR8);
        cv::imshow("view", cvImage->image);
        clickPoint.x=(rosImage->width)/2;
        clickPoint.y=rosImage->height;
        clickPoint.z=0;
        cv::waitKey(1);
    }
    void timer_callback() {
        clickPointPublisher->publish(clickPoint);
     }

public:
    Answer() : Node("answer_node") {
        imageSubscription = create_subscription<sensor_msgs::msg::Image>(
                "/raw_image",
                10,
                std::bind(&Answer::imageCallback, this, std::placeholders::_1)
        );
        clickPointPublisher = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);
        timer_ = create_wall_timer(
                500ms, std::bind(&Answer::timer_callback, this));
    }
};

#endif //ANSWER_ANSWER_NODE_H
