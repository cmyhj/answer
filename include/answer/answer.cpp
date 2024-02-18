//
// Created by cmyhj on 24-2-17.
//
#include "answer.h"

using namespace std::chrono_literals;

void Answer::imageCallback(sensor_msgs::msg::Image::SharedPtr rosImage) {
    //转换成opencv图像
    cv_bridge::CvImagePtr cvImage;
    cvImage = cv_bridge::toCvCopy(rosImage, sensor_msgs::image_encodings::BGR8);
    imageProcessing processingImage(std::move(cvImage->image));

    //找出click位置
    std::vector<std::vector<cv::Point>> counts = processingImage.centerCounts();

    //判断是否发送click位置
    float clickRadius;
    cv::Point2f clickCenter;
    for (auto it = counts.begin(); it != counts.end(); it++) {
        cv::minEnclosingCircle(*it, clickCenter, clickRadius);//找出中心点

        //根据点与线的距离判断是否点击
        if (processingImage.distanceToLine(clickCenter) < (95 + 25)
            && processingImage.distanceToLine(clickCenter) > (95 - 25)) {
            clickPoint.x = clickCenter.x;
            clickPoint.y = clickCenter.y;//赋值
            clickPointPublisher->publish(clickPoint);//发送
        }
    }
}

//构造函数
Answer::Answer() : Node("answer_node") {
    //click位置初始化
    clickPoint.x = 0;
    clickPoint.y = 0;
    clickPoint.z = 0;
    //图像订阅
    imageSubscription = create_subscription<sensor_msgs::msg::Image>(
            "/raw_image",
            10,
            std::bind(&Answer::imageCallback, this, std::placeholders::_1)
    );
    //click位置发布
    clickPointPublisher = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);
}