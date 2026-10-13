#include <chrono>
#include <list>

#include "thruster_manager.hpp"

using namespace std::chrono_literals;

ThrusterManager::ThrusterManager() :
    Node("thruster_manager") {

    subscription = this->create_subscription<custom_interfaces::msg::ThrusterPercents>(
    "thruster_percents", 10, std::bind(&ThrusterManager::thruster_manager_callback, this, std::placeholders::_1));

    pwm_publisher = this->create_publisher<custom_interfaces::msg::PWM>("topic", 10);
    /* Creates a timer that triggers every half a second, calling the timer_callback() function
     * every half a second
    */
    timer = this->create_wall_timer(
    500ms, std::bind(&ThrusterManager::timer_callback, this));
}

custom_interfaces::msg::PWM ThrusterManager::convert_percents_to_pwm(const custom_interfaces::msg::ThrusterPercents & msg) const {
    auto pwm = custom_interfaces::msg::PWM();
    pwm.thruster_pwm_0 = msg.thruster_percent_0 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_1 = msg.thruster_percent_1 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_2 = msg.thruster_percent_2 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_3 = msg.thruster_percent_3 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_4 = msg.thruster_percent_4 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_5 = msg.thruster_percent_5 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_6 = msg.thruster_percent_6 * (400 - pwmLimit) + 1500;
    pwm.thruster_pwm_7 = msg.thruster_percent_7 * (400 - pwmLimit) + 1500;
    return pwm;
}

void ThrusterManager::set_pwm_limit(int limit) {
    if (limit < 0) {
        limit = 0;
    } else if (limit > 400) {
        limit = 400;
    }
    ThrusterManager::pwmLimit = limit;
}

void ThrusterManager::thruster_manager_callback(const custom_interfaces::msg::ThrusterPercents & msg) const {
    
    auto pwm = ThrusterManager::convert_percents_to_pwm(msg);

    pwm_publisher->publish(pwm);
    // RCLCPP_INFO(this->get_logger(), "I heard: '%s'", std::to_string(msg.front()).c_str());
}

void ThrusterManager::timer_callback() {
    // RCLCPP_INFO(this->get_logger(), "Publishing PWM Messag: '%s'", std::to_string(thruster_pwm.front()).c_str());
}



#ifndef ENABLE_TESTING

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ThrusterManager>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
