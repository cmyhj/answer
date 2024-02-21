//
// Created by cmyhj on 24-2-17.
//
#include "answer.h"

using namespace std::chrono_literals;

void Answer::imageCallback(sensor_msgs::msg::Image::SharedPtr rosImage) {
    if (delayCalculateFlag) {//屏蔽第一帧的干扰图像（严重怀疑是BUG，IN模式第一帧图像是主菜单）
        //转换成opencv图像
        cv_bridge::CvImagePtr cvImage;
        cvImage = cv_bridge::toCvCopy(rosImage, sensor_msgs::image_encodings::BGR8);
        imageProcessing processingImage(std::move(cvImage->image), lastLine);
        lastLine = processingImage.line;

        //找出click位置
        std::vector<std::vector<cv::Point>> clickSet = processingImage.centerCounts();


        clickPoint.x = 0;
        clickPoint.y = 0;
        float clickRadius;
        cv::Point2f clickCenter;

        float nowDistance = 0;
        for (auto it = clickSet.begin(); it != clickSet.end(); it++) {//遍历所有click块
            cv::minEnclosingCircle(*it, clickCenter, clickRadius);//找出中心点
            nowDistance = processingImage.distanceToLine(clickCenter);//找出点到线的距离

            //延时计算，找出第一次点击后click块在那里消失，从而判断延时
            if (delayCalculateFlag >= 2 && delayCalculateFlag <= 40 && nowDistance < shortestDistance) {
                shortestDistance = nowDistance;//找出与判定线的最近距离
            }


            //根据点与线的距离判断是否点击
            if (nowDistance < (delayDistance + 25) && nowDistance > (delayDistance - 25)) {
                clickPoint.x = clickCenter.x;
                clickPoint.y = clickCenter.y;//赋值
                clickPointPublisher->publish(clickPoint);//发送

                //若为第一次click则开始进行延时计算(delayCalculateFlag=2）
                if (delayCalculateFlag == 1) {
                    delayCalculateFlag = 2;
                }
            }
        }


        //校准程序（对第一次click后的40张图片进行分析）
        if (delayCalculateFlag >= 2 && delayCalculateFlag <= 40) {
            if (delayCalculateFlag >= 40) {
                if (shortestDistance > 20) {//如果在判定线上方，则按比例减少延时距离
                    delayDistance -= (shortestDistance - 20);
                }
                if (processingImage.missClickDistance() == 0 && shortestDistance < 0) {//如果在判定线下方，则按比例增加延时距离
                    delayDistance -= (shortestDistance - 20);
                }
                //如果未点击到，则第40张图片click的位置增加延时距离
                delayDistance += (float) processingImage.missClickDistance() * 0.6;
                for (int i = 0; i < 100; ++i) {
                    RCLCPP_INFO_STREAM(get_logger(), "delayDistance: " << delayDistance);
                }
            }
            delayCalculateFlag++;
        }
    }
    if (delayCalculateFlag == 0) {//屏蔽第一帧的干扰图像（严重怀疑是BUG，IN模式第一帧图像是主菜单）
        delayCalculateFlag = 1;
    }
}

//构造函数
Answer::Answer() : Node("answer_node") {
    //click位置初始化
    clickPoint.x = 0;
    clickPoint.y = 0;
    clickPoint.z = 0;
    delayDistance = 40;
    delayCalculateFlag = 0;
    shortestDistance = 6000;
    lastLine = cv::Vec4i(0, 0, 0, 0);
    //图像订阅
    imageSubscription = create_subscription<sensor_msgs::msg::Image>(
            "/raw_image",
            10,
            std::bind(&Answer::imageCallback, this, std::placeholders::_1)
    );
    //click位置发布
    clickPointPublisher = create_publisher<geometry_msgs::msg::Point32>("/click_position", 10);
}