#include "pico_interface.hpp"
#include "std_msgs/msg/empty.hpp"

#include <chrono>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

using namespace std::chrono_literals;

PicoInterface::PicoInterface(std::string path) : Node("pico_interface"), path(path) {
    configure_serial_port();

    thruster_heartbeat_subscription = this->create_subscription<std_msgs::msg::Empty>(
        "thruster_heartbeat", 10,
        [this](const std_msgs::msg::Empty& heartbeat) { thruster_heartbeat_callback(heartbeat); });

    thruster_pwm_subscription = this->create_subscription<std_msgs::msg::Empty>(
        "thruster_pwm", 10,
        [this](const std_msgs::msg::Empty& pwm) { thruster_pwm_callback(pwm); });

    heartbeat_timer = this->create_wall_timer(500ms, [this]() { heartbeat_timer_callback(); });
}

void PicoInterface::thruster_heartbeat_callback(const std_msgs::msg::Empty& heartbeat) {
    most_recent_heartbeat = std::chrono::steady_clock::now();
    (void)heartbeat;
}

void PicoInterface::thruster_pwm_callback(const std_msgs::msg::Empty& pwm) {}

void PicoInterface::heartbeat_timer_callback() {
    bool healthy = get_health_status();
    if (healthy) {
        send_heartbeat_to_pico();
    } else {
        // log health status rclcpp
        // stop thrusters
    }
}

void PicoInterface::configure_serial_port() {
    struct termios options;
    speed_t baud = B115200;
    int status, fd;

    if ((fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1) {
        serial_fd = -1;
        return;
    }

    fcntl(fd, F_SETFL, O_RDWR);

    // Get and modify current options:
    tcgetattr(fd, &options);
    cfmakeraw(&options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 1; // 1/10 (1 decisecond)

    tcsetattr(fd, TCSANOW, &options);

    ioctl(fd, TIOCMGET, &status);
    status |= TIOCM_DTR;
    status |= TIOCM_RTS;
    ioctl(fd, TIOCMSET, &status);

    usleep(100000); // 100ms

    serial_fd = fd;
}

void PicoInterface::send_heartbeat_to_pico() {
    std::string serial_message = "ping\n";
    int length = serial_message.size();

    ssize_t bytes_written = write(serial_fd, serial_message.c_str(), length);

    if (bytes_written != length) {
        RCLCPP_WARN(this->get_logger(), "Failed to ping Pico (wrote %zd/%d bytes)", bytes_written,
                    length);
    }
}

void PicoInterface::send_pwm_to_pico() {}

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
