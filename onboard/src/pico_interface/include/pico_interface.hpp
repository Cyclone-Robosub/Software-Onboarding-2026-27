#ifndef PICO_INTERFACE_HPP
#define PICO_INTERFACE_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/empty.hpp"

class PicoInterface : public rclcpp::Node {
  public:
    PicoInterface();
    ~PicoInterface();

  private:
    void thruster_pwm_callback(const std_msgs::msg::Empty& pwm);
    void thruster_heartbeat_callback(const std_msgs::msg::Empty& heartbeat);
    void heartbeat_timer_callback();
    void configure_serial_port();
    void send_heartbeat_to_pico();
    void send_pwm_to_pico();

    rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr thruster_heartbeat_subscription;
    rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr thruster_pwm_subscription;
    rclcpp::TimerBase::SharedPtr heartbeat_timer;

    std::chrono::time_point<std::chrono::steady_clock> most_recent_heartbeat;

    int serial_fd = -1;
};

#endif // PICO_INTERFACE_HPP
