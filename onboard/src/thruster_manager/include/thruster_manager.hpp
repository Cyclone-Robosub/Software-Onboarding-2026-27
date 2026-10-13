#ifndef THRUSTER_MANAGER_HPP
#define THRUSTER_MANAGER_HPP

#include "rclcpp/rclcpp.hpp"

#include "custom_interfaces/msg/pwm.hpp"
#include "custom_interfaces/msg/thruster_percents.hpp"

class ThrusterManager : public rclcpp::Node {
public:
    ThrusterManager();

private:
    void thruster_manager_callback(const custom_interfaces::msg::ThrusterPercents & msg) const;
    rclcpp::Subscription<custom_interfaces::msg::ThrusterPercents>::SharedPtr subscription;

    custom_interfaces::msg::PWM convert_percents_to_pwm(const custom_interfaces::msg::ThrusterPercents & msg) const;

    void set_pwm_limit(int limit);
    int pwmLimit = 200;

    void timer_callback();
    rclcpp::Publisher<custom_interfaces::msg::PWM>::SharedPtr pwm_publisher;
    rclcpp::TimerBase::SharedPtr timer;
    std::chrono::time_point<std::chrono::steady_clock> most_recent_heartbeat;
};

#endif // THRUSTER_MANAGER_HPP