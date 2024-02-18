//
// Created by cmyhj on 24-2-17.
//

#include "image_processing.h"

//构造函数
imageProcessing::imageProcessing(cv::Mat &&originImage) {
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
    if (lines.size() > 0) {
        line = lines[0];
    }
}

//寻找click块轮廓
std::vector<std::vector<cv::Point>> imageProcessing::centerCounts() {
    cv::Mat result;
    std::vector<std::vector<cv::Point>> counts;
    //寻找click块（蓝色）
    cv::inRange(src, cv::Scalar(190, 150, 10), cv::Scalar(255, 255, 180), result);
    //图像处理
    cv::Mat medianBlurImageMat;
    cv::medianBlur(result, medianBlurImageMat, 3);
    //寻找轮廓
    cv::findContours(medianBlurImageMat, counts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    return counts;
}

//计算click块到判定线的距离
float imageProcessing::distanceToLine(cv::Point2f clickCenter) const {
    float A, B, C;
    A = line[endPointY] - line[startPointY];
    B = line[startPointX] - line[endPointX];
    C = line[endPointX] * line[startPointY] - line[startPointX] * line[endPointY];
    // 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)*/
    return abs(A * clickCenter.x + B * clickCenter.y + C) / sqrt(A * A + B * B);
}