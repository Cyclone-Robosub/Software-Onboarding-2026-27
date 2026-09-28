#include "thruster_manager.hpp"

ThrusterManager::ThrusterManager() :
    Node("thruster_manager") {

}

#ifndef ENABLE_TESTING

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ThrusterManager>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
