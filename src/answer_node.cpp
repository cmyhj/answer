#include <cstdio>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <answer/answer_node.h>

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    std::cout << "Hello, I'm answer node!" << std::endl;
    // 创建一个Answer节点
    rclcpp::spin(std::make_shared<Answer>());
    rclcpp::shutdown();
}
