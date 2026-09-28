#include <chrono>

#include "publisher.hpp"

using namespace std::chrono_literals;

MinimalPublisher::MinimalPublisher() : 
    Node("minimal_publisher"), count(0) {

    publisher = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer = this->create_wall_timer(
    500ms, std::bind(&MinimalPublisher::timer_callback, this));
}

void MinimalPublisher::timer_callback() {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher->publish(message);
}

#ifndef ENABLE_TESTING

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
