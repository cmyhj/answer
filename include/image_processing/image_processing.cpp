//
// Created by cmyhj on 24-2-17.
//

#include "image_processing.h"

//构造函数
imageProcessing::imageProcessing(cv::Mat &&originImage,const cv::Vec4i& lastLine) {
    //初始化
    src = originImage;

    //查找判定线
    cv::Mat gray_image;
    std::vector<cv::Vec4i> lines; // 存储检测到的线
    cvtColor(src, gray_image, cv::COLOR_BGR2GRAY); // 转换为灰度图像
    cv::Mat edges_image;
    cv::Canny(gray_image, edges_image, 100, 200); // 边缘检测
    HoughLinesP(edges_image, lines, 1, CV_PI / 180,
                80, 200, 10); // 直线检测算
    //存储判定线
    if (!lines.empty()) {
        line = lines[0];
    }
    else{
        line=lastLine;
    }
}

//寻找click块轮廓
std::vector<std::vector<cv::Point>> imageProcessing::centerCounts() const {
    cv::Mat result;
    std::vector<std::vector<cv::Point>> counts;
    std::vector<cv::Vec4i> lines;
    //寻找click块（蓝色）
    cv::inRange(src, cv::Scalar(190, 150, 10), cv::Scalar(255, 255, 180), result);
    //图像处理
    cv::Mat medianBlurImageMat;
    cv::medianBlur(result, medianBlurImageMat, 3);
    /*cv::Mat edges_image;
    cv::Canny(medianBlurImageMat, edges_image, 100, 200); */// 边缘检测
    HoughLinesP(medianBlurImageMat, lines, 1, CV_PI / 180,
                80, 80, 10); // 直线检测算
    cv::inRange(src, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0), result);
    for (auto l : lines)
    {
        cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 255, 255), 3);
    }
    cv::findContours(result, counts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    return counts;
}

float imageProcessing::missClickDistance() const{
    cv::Mat result;
    float distance=0;
    std::vector<std::vector<cv::Point>> contours;
    //寻找miss块（黑色）
    cv::inRange(src, cv::Scalar(0, 0, 0), cv::Scalar(50, 50, 50), result);
    cv::Mat medianBlurImageMat;
    cv::medianBlur(result, medianBlurImageMat, 3);
    cv::findContours(medianBlurImageMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    float clickRadius;
    cv::Point2f missCenter;
    for (auto it = contours.begin(); it != contours.end(); it++) {
        cv::minEnclosingCircle(*it, missCenter, clickRadius);//找出中心点
        //根据点与线的距离判断是否点击
        if (distanceToLine(missCenter) < distance) {
            distance=distanceToLine(missCenter);
        }
    }
    return  -distance;
}
//计算click块到判定线的距离
float imageProcessing::distanceToLine(cv::Point2f clickCenter) const {
    float A, B, C,result;
    A = line[endPointY] - line[startPointY];
    B = line[startPointX] - line[endPointX];
    C = line[endPointX] * line[startPointY] - line[startPointX] * line[endPointY];
    // 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)*/
    result=abs(A * clickCenter.x + B * clickCenter.y + C) / sqrt(A * A + B * B);
    if(clickCenter.y>-(C+A * clickCenter.x)/B){
        result=-result;
    }
    return result;
}