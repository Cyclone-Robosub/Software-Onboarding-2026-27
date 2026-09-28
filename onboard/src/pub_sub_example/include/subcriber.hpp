/* This is a compiler guard, preventing the file from being included multiple times. */
#ifndef MINIMAL_SUBSCRIBER_HPP
#define MINIMAL_SUBSCRIBER_HPP

/* This is the ROS2 standard library for C++: it provides Node. */
#include "rclcpp/rclcpp.hpp"
/* This is a ROS2 message type for strings: when we want to send a string
 * from one node to another, we specify the message type as std_msgs::msg::String
*/
#include "std_msgs/msg/string.hpp"

/* Declares the class, specifying that it inherits from ROS2 Node */
class MinimalSubscriber : public rclcpp::Node {
public:
    /* No-arg constructor, which we use in main(). In real life, we often have a number
     * of arguments here.
    */
    MinimalSubscriber();

private:
    /* Methods and member variables are declared as private or protected unless we have a
     * very good reason to make them public
    */
    void topic_callback(const std_msgs::msg::String & msg) const;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;
};

#endif // MINIMAL_SUBSCRIBER_HPP
