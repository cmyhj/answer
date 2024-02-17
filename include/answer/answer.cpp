//
// Created by cmyhj on 24-2-17.
//
#include "answer.h"

using namespace std::chrono_literals;

void Answer::imageCallback(const sensor_msgs::msg::Image::SharedPtr rosImage) {
    cv_bridge::CvImagePtr cvImage;
    cvImage = cv_bridge::toCvCopy(rosImage, sensor_msgs::image_encodings::BGR8);
    imageProcessing processingImage(cvImage->image);
    cv::Mat processingImageMat = processingImage.onlyClickImage();
    cv::MatIterator_<cv::Vec3b> it = processingImageMat.begin<cv::Vec3b>();
    cv::MatIterator_<cv::Vec3b> end = processingImageMat.end<cv::Vec3b>();
    for (; it != end; ++it) {
        if ((*it)[0] == 255 && (*it)[1] == 255 && (*it)[2] == 255) {
            clickPoint.x = (float)it.pos().x-50;
            clickPoint.y = (float)it.pos().y;
            clickPoint.z = 0;
        }
    }
    cv::imshow("onlyBlueImage", processingImage.onlyClickImage());
    cv::waitKey(1);
}

void Answer::timer_callback() {
    //RCLCPP_INFO(this->get_logger(), "timer_callback:y:%f",clickPoint.y);
    if (clickPoint.y < 200 && clickPoint.y > 60){
        clickPointPublisher->publish(clickPoint);
    }
}

Answer::Answer() : Node("answer_node") {
    imageSubscription = create_subscription<sensor_msgs::msg::Image>(
            "/raw_image",
            10,
            std::bind(&Answer::imageCallback, this, std::placeholders::_1)
    );
    clickPointPublisher = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);
    timer_ = create_wall_timer(
            16ms, std::bind(&Answer::timer_callback, this));
}