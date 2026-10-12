#include "pico_interface.hpp"
#include "std_msgs/msg/empty.hpp"

PicoInterface::PicoInterface() : Node("pico_interface") {
    configure_serial_port();

    thruster_heartbeat_subscription = this->create_subscription<std_msgs::msg::Empty>(
        "thruster_heartbeat", 10,
        [this](const std_msgs::msg::Empty& heartbeat) { thruster_heartbeat_callback(heartbeat); });

    thruster_pwm_subscription = this->create_subscription<std_msgs::msg::Empty>(
        "thruster_pwm", 10,
        [this](const std_msgs::msg::Empty& pwm) { thruster_pwm_callback(pwm); });
}

void PicoInterface::thruster_heartbeat_callback(const std_msgs::msg::Empty& heartbeat) {}

void PicoInterface::thruster_pwm_callback(const std_msgs::msg::Empty& pwm) {}

void PicoInterface::heartbeat_timer_callback() {}

void PicoInterface::configure_serial_port() {}

void PicoInterface::send_heartbeat_to_pico() {}

void PicoInterface::send_pwm_to_pico() {}

#ifndef ENABLE_TESTING

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PicoInterface>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
