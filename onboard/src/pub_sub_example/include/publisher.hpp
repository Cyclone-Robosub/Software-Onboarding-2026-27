/* This is a compiler guard, preventing the file from being included multiple times. */
#ifndef MINIMAL_PUBLISHER_HPP
#define MINIMAL_PUBLISHER_HPP

/* This is the ROS2 standard library for C++: it provides Node. */
#include "rclcpp/rclcpp.hpp"
/* This is a ROS2 message type for strings: when we want to send a string
 * from one node to another, we specify the message type as std_msgs::msg::String
*/
#include "std_msgs/msg/string.hpp"

/* Declares the class, specifying that it inherits from ROS2 Node */
class MinimalPublisher : public rclcpp::Node {
public:
    /* No-arg constructor, which we use in main(). In real life, we often have a number
     * of arguments here.
    */
    MinimalPublisher();

private:
    /* Methods and member variables are declared as private or protected unless we have a
     * very good reason to make them public
    */
    void timer_callback();
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    rclcpp::TimerBase::SharedPtr timer;
    size_t count;
    std::chrono::time_point<std::chrono::steady_clock> most_recent_heartbeat;
};

#endif // MINIMAL_PUBLISHER_HPP
