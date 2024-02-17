//
// Created by cmyhj on 24-2-17.
//

#include "image_processing.h"

imageProcessing::imageProcessing(cv::Mat originImage){
    src=originImage;
}

cv::Mat imageProcessing::onlyClickImage(){
    cv::Mat result;
    cv::Mat result1;
    cv::inRange(src,cv::Scalar(190,150,10),cv::Scalar(255,255,180),result);
    //cv::morphologyEx(result,result1,cv::MORPH_OPEN,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3)));
    return result;
}