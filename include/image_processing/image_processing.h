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

enum {
    startPointX, startPointY, endPointX, endPointY
};

class imageProcessing//图像处理类
{
private:
    cv::Mat src;//原图
public:
    imageProcessing(cv::Mat &&originImage, const cv::Vec4i &lastLine);//构造函数
    std::vector<std::vector<cv::Point>> centerCounts() const;//寻找click块轮廓
    [[nodiscard]] float distanceToLine(cv::Point2f clickCenter) const;//计算click块到判定线的距离
    float missClickDistance() const;////如果未点击到，则第40张图片click的位置增加延时

    cv::Vec4i line;//判定线
};


#endif //ANSWER_IMAGE_PROCESSING_H
