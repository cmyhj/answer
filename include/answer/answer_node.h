//
// Created by cmyhj on 24-2-16.
//

#ifndef ANSWER_ANSWER_NODE_H
#define ANSWER_ANSWER_NODE_H

#include <iostream>
#include <thread>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/point32.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>



class Answer : public rclcpp::Node {
private:
    rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr clickPointPublisher;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr imageSubscription;
	void imageCallback(const sensor_msgs::msg::Image::SharedPtr rosImage)
	{
        cv_bridge::CvImagePtr cvImage;
        cvImage = cv_bridge::toCvCopy(rosImage, sensor_msgs::image_encodings::BGR8);
        cv::imshow("view", cvImage->image);
        cv::waitKey(1);
	}

public:
    Answer() : Node("homework_node") {
        imageSubscription= create_subscription<sensor_msgs::msg::Image>(
                "/raw_image",
                10,
                std::bind(&Answer::imageCallback, this, std::placeholders::_1)
                );
       clickPointPublisher = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);

    }
};

#endif //ANSWER_ANSWER_NODE_H
