#ifndef MINIMAL_SUBSCRIBER_HPP
#define MINIMAL_SUBSCRIBER_HPP

#include "rclcpp/rclcpp.hpp"    
#include "std_msgs/msg/string.hpp"

class MinimalSubscriber : public rclcpp::Node {
    public:
        MinimalSubscriber();

    private:
        void topic_callback(const std_msgs::msg::String & msg) const;
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;
};

#endif // MINIMAL_SUBSCRIBER_HPP
