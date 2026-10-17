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
    if (!configure_serial_port()) {
        if (serial_fd >= 0) {
            close(serial_fd);
            serial_fd = -1;
        }
        throw std::runtime_error("Failed to configure serial port");
    }

    thruster_heartbeat_subscription = this->create_subscription<std_msgs::msg::Empty>(
        "thruster_manager_heartbeat", 10,
        [this](const std_msgs::msg::Empty&) { thruster_heartbeat_callback(); });

    thruster_pwm_subscription = this->create_subscription<custom_interfaces::msg::PWM>(
        "pwm", 10,
        [this](const custom_interfaces::msg::PWM& pwm) { thruster_pwm_callback(pwm); });

    heartbeat_timer = this->create_wall_timer(500ms, [this]() { heartbeat_timer_callback(); });
}

PicoInterface::~PicoInterface() {
    if (serial_fd >= 0) {
        close(serial_fd);
    }
}

void PicoInterface::thruster_heartbeat_callback() {
    first_heartbeat_received = true;
    most_recent_heartbeat = std::chrono::steady_clock::now();
}

void PicoInterface::thruster_pwm_callback(const custom_interfaces::msg::PWM& pwm) {
    if (!is_upstream_healthy()) {
        RCLCPP_WARN(this->get_logger(), "Upstream heartbeat is unhealthy, ignoring PWM command");
        return;
    }

    const std::array<uint16_t, 8> pwm_values = {
        pwm.thruster_pwm_0, pwm.thruster_pwm_1, pwm.thruster_pwm_2, pwm.thruster_pwm_3,
        pwm.thruster_pwm_4, pwm.thruster_pwm_5, pwm.thruster_pwm_6, pwm.thruster_pwm_7
    };

    std::string serial_message{};
    for (size_t i = 0; i < thruster_pins.size(); ++i) {
        serial_message = "Set " + std::to_string(thruster_pins[i]) + " PWM " + std::to_string(pwm_values[i]) + "\n";
        write_to_serial_port(serial_message);
    }
}

void PicoInterface::heartbeat_timer_callback() {
    std::string serial_message{};
    if (is_upstream_healthy()) {
        serial_message = "ping\n";
        write_to_serial_port(serial_message);
    } else {
        RCLCPP_WARN(this->get_logger(), "Upstream hearbeat is unhealthy, stopping thrusters");
        for (size_t i = 0; i < thruster_pins.size(); ++i) {
            serial_message = "Set " + std::to_string(thruster_pins[i]) + " PWM 1500\n";
            write_to_serial_port(serial_message);
        }
    }
}

bool PicoInterface::is_upstream_healthy() {
    if (!first_heartbeat_received) {
        return false;
    }

    return (std::chrono::steady_clock::now() - most_recent_heartbeat) <= 1s;
}

bool PicoInterface::configure_serial_port() {
    int serial_port = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);

    if (serial_port < 0) {
        RCLCPP_ERROR(
            this->get_logger(), 
            "Failed to open serial port: %s",
            strerror(errno)
        );
        return false;
    }

    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0) {
        RCLCPP_ERROR(
            this->get_logger(),
            "Failed to get serial port attributes: %s",
            strerror(errno)
        );
        close(serial_port);
        return false;
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
        RCLCPP_ERROR(
            this->get_logger(),
            "Failed to set serial port attributes: %s",
            strerror(errno)
        );
        close(serial_port);
        return false;
    }

    serial_fd = serial_port;
    return true;
}

void PicoInterface::write_to_serial_port(const std::string& message) {
    if (serial_fd < 0) {
        RCLCPP_ERROR(this->get_logger(), "Serial port is not open");
        return;
    }

    int length = message.size();
    ssize_t bytes_written = write(serial_fd, message.c_str(), length);

    if (bytes_written != length) {
        RCLCPP_ERROR(
            this->get_logger(), 
            "Failed to write to serial port (wrote %zd/%d bytes) messsage=%s",
            bytes_written, 
            length,
            message.c_str()
        );
    }
}

#ifndef ENABLE_TESTING

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PicoInterface>("/dev/ttyACM0"));
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
