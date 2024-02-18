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
        cv::minEnclosingCircle(*it, clickCenter, clickRadius);
        if (processingImage.distanceToLine(clickCenter) < 90 && processingImage.distanceToLine(clickCenter) > 60) {
            clickPoint.x = clickCenter.x;
            clickPoint.y = clickCenter.y;
            clickPointPublisher->publish(clickPoint);
        }
    }


}

Answer::Answer() : Node("answer_node") {
    imageSubscription = create_subscription<sensor_msgs::msg::Image>(
            "/raw_image",
            10,
            std::bind(&Answer::imageCallback, this, std::placeholders::_1)
    );
    clickPointPublisher = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);
    clickPoint.x = 0;
    clickPoint.y = 0;
    clickPoint.z = 0;

}