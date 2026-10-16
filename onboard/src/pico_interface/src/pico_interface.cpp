#include "pico_interface.hpp"
#include "std_msgs/msg/empty.hpp"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <chrono>
#include <array>

using namespace std::chrono_literals;

PicoInterface::PicoInterface(
    std::string path,
    std::array<int, 8> thruster_pins
)
    : Node("pico_interface"),
    path(std::move(path)),
    thruster_pins(thruster_pins)
{
    configure_serial_port();

    thruster_heartbeat_subscription = this->create_subscription<std_msgs::msg::Empty>(
        "thruster_heartbeat", 10,
        [this](const std_msgs::msg::Empty& heartbeat) { thruster_heartbeat_callback(heartbeat); });

    thruster_pwm_subscription = this->create_subscription<custom_interfaces::msg::PWM>(
        "thruster_pwm", 10,
        [this](const custom_interfaces::msg::PWM& pwm) { thruster_pwm_callback(pwm); });

    heartbeat_timer = this->create_wall_timer(500ms, [this]() { heartbeat_timer_callback(); });
}

void PicoInterface::thruster_heartbeat_callback(const std_msgs::msg::Empty& heartbeat) {
    most_recent_heartbeat = std::chrono::steady_clock::now();
    (void)heartbeat;
}

void PicoInterface::thruster_pwm_callback(const custom_interfaces::msg::PWM& pwm) {
    const std::array<uint16_t, 8> pwm_values = {
        pwm.thruster_pwm_0, pwm.thruster_pwm_1, pwm.thruster_pwm_2, pwm.thruster_pwm_3,
        pwm.thruster_pwm_4, pwm.thruster_pwm_5, pwm.thruster_pwm_6, pwm.thruster_pwm_7
    };

    for (size_t i = 0; i < pwm_values.size(); ++i) {
        send_pwm_to_pico(i, pwm_values[i]);
    }
}


void PicoInterface::heartbeat_timer_callback() {
    bool healthy = get_health_status();
    if (healthy) {
        send_heartbeat_to_pico();
    } else {
        RCLCPP_WARN(this->get_logger(), "PicoInterface is unhealthy, stopping thrusters");
        // stop thrusters
    }
}

void PicoInterface::configure_serial_port() {
    int serial_port = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);

    if (serial_port < 0) {
        RCLCPP_ERROR(this->get_logger(), "Failed to open serial port: %s", strerror(errno));
        serial_fd = -1;
        return;
    }

    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0) {
        RCLCPP_ERROR(this->get_logger(), "Failed to get serial port attributes: %s", strerror(errno));
        serial_fd = -1;
        return;
    }

    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;  // Clear current data size setting
    tty.c_cflag |= CS8;     // 8 data bits
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore modem control

    tty.c_lflag &= ~ICANON; // Disable canonical mode
    tty.c_lflag &= ~ECHO;   // Disable echo
    tty.c_lflag &= ~ECHOE;  // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT and SUSP characters

    // Disable software flow control
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); 
    // Disable special handling of bytes
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); 

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 1; // Wait for up to 0.1s (1 deciseconds) 
    tty.c_cc[VMIN] = 0;

    // Set baud rate to 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        RCLCPP_ERROR(this->get_logger(), "Failed to set serial port attributes: %s", strerror(errno));
        serial_fd = -1;
        return;
    }



}

void PicoInterface::send_heartbeat_to_pico() {
    std::string serial_message = "ping\n";
    size_t length = serial_message.size();

    ssize_t bytes_written = write(serial_fd, serial_message.c_str(), length);

    if (bytes_written != length) {
        RCLCPP_WARN(this->get_logger(), "Failed to ping Pico (wrote %zd/%d bytes)", bytes_written,
                    length);
    }
}

void PicoInterface::send_pwm_to_pico(int pin, int value) {
    std::string serial_message = "Set " + std::to_string(pin) + " PWM " + std::to_string(value) + "\n";
    size_t length = serial_message.size();



};


bool PicoInterface::get_health_status() {
    return (std::chrono::steady_clock::now() - most_recent_heartbeat) > 1s;
}

#ifndef ENABLE_TESTING

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PicoInterface>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
