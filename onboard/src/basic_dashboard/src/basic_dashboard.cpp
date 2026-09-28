#include "basic_dashboard.hpp"

BasicDashboard::BasicDashboard() :
    Node("basic_dashboard") {

}

#ifndef ENABLE_TESTING

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BasicDashboard>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
