//
// Created by cmyhj on 24-2-17.
//

#include "image_processing.h"

imageProcessing::imageProcessing(cv::Mat && originImage){
    src=originImage;
    cv::Mat gray_image;
    std::vector<cv::Vec4i> lii; // 存储检测到的直线
    cvtColor(src, gray_image, cv::COLOR_BGR2GRAY); // 转换为灰度图像
    cv::Mat edges_image;
    cv::Canny(gray_image, edges_image, 100, 200); // 应用Canny边缘检测算法
    HoughLinesP(edges_image, lii, 1, CV_PI / 180,80, 200, 10); // 应用霍夫直线检测算法
    if (lii.size() > 0) {
        lines = lii[0];
    }
}

std::vector<std::vector<cv::Point>> imageProcessing::centerCounts(){
    cv::Mat result;
    std::vector<std::vector<cv::Point>> counts;
    cv::inRange(src,cv::Scalar(190,150,10),cv::Scalar(255,255,180),result);
    cv::Mat medianBlurImageMat;
    cv::medianBlur(result,medianBlurImageMat,3);
    cv::findContours(medianBlurImageMat,counts,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    //cv::morphologyEx(result,result1,cv::MORPH_OPEN,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3)));
    return counts;
}

float imageProcessing::distanceToLine(cv::Point2f clickCenter) const{
        float A, B, C;
        A = lines[3] - lines[1];
        B = lines[0] - lines[2];
        C = lines[2] * lines[1] - lines[0] * lines[3];
        // 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)*/
        return abs(A * clickCenter.x + B * clickCenter.y + C) / sqrt(A * A + B * B);
}