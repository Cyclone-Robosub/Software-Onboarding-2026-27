#include "gamepad_interpreter.hpp"

GamepadInterpreter::GamepadInterpreter() :
    Node("gamepad_interpreter") {

}

#ifndef ENABLE_TESTING

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GamepadInterpreter>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
