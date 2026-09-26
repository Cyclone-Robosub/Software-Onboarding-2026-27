#ifndef MINIMAL_PUBLISHER_HPP
#define MINIMAL_PUBLISHER_HPP

#include <string>

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/empty.hpp"


class MinimalPublisher : public rclcpp::Node {
public:
    MinimalPublisher();

private:
    void timer_callback();
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    rclcpp::TimerBase::SharedPtr timer;
    size_t count;
    std::chrono::time_point<std::chrono::steady_clock> most_recent_heartbeat;
};

#endif // MINIMAL_PUBLISHER_HPP
