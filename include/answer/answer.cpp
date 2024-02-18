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
    cv::Mat medianBlurImageMat;
    cv::medianBlur(processingImageMat,medianBlurImageMat,3);
    std::vector<std::vector<cv::Point>> counts;
    cv::findContours(medianBlurImageMat,counts,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    float clickRadius;
    cv::Point2f clickCenter;
    for (auto it= counts.begin();it!=counts.end();it++) {
        cv::minEnclosingCircle(*it, clickCenter, clickRadius);
        cv::circle(medianBlurImageMat, clickCenter, 30, cv::Scalar(255, 255, 255), 5);

    if (clickCenter.y >270&&clickCenter.y<300) {
        clickPoint.x = clickCenter.x;
        clickPoint.y = clickCenter.y;
        clickPointPublisher->publish(clickPoint);
        RCLCPP_INFO_STREAM(this->get_logger(), "click point x: " << clickPoint.x << " y: " << clickPoint.y );
        /*cv::imshow("onlyBlueImage", medianBlurImageMat);
        cv::waitKey(1);*/
    }
    }
    /*cv::MatIterator_<cv::Vec3b> it = processingImageMat.begin<cv::Vec3b>();
    cv::MatIterator_<cv::Vec3b> end = processingImageMat.end<cv::Vec3b>();
    for (; it != end; ++it) {
        if ((*it)[0] == 255 && (*it)[1] == 255 && (*it)[2] == 255) {
            clickPoint.x = (float) it.pos().x;
            clickPoint.y = (float) it.pos().y;
            if (clickPoint.y < 350 && clickPoint.y > 280) {
                clickPointPublisher->publish(clickPoint);
                cv::circle(processingImageMat,cv::Point((int)clickPoint.x,(int)clickPoint.y),
                           10,cv::Scalar(255,255,255),20);
                cv::imshow("onlyBlueImage", processingImageMat);
                cv::waitKey(1);
                break;
            }
            cv::circle(processingImageMat,cv::Point((int)clickPoint.x,(int)clickPoint.y),
                       10,cv::Scalar(255,255,255),20);
            cv::imshow("onlyBlueImage", processingImageMat);
            cv::waitKey(1);
        }

    }*/

}

void Answer::timer_callback() {

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
    clickPoint.x = 0;
    clickPoint.y = 0;
    clickPoint.z = 0;
}