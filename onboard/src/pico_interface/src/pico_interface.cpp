#include "pico_interface.hpp"

PicoInterface::PicoInterface() :
    Node("pico_interface") {

}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PicoInterface>());
    rclcpp::shutdown();
    return 0;
}
