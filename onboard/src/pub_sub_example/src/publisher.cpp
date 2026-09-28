/* used for the timer creation */
#include <chrono>
/* Used for converting the count to a string in the timer callback */
#include <string>

/* header file with MinimalPublisher class declaration */
#include "publisher.hpp"

/* used so that we can write 500ms directly instead of using the full namespace */
using namespace std::chrono_literals;

/* Class constructor: creates a ROS2 Node with name "minimal_publisher" and
 * sets the private count variable to 0
*/
MinimalPublisher::MinimalPublisher() : 
    Node("minimal_publisher"), count(0) {

    /* Creates a publisher that publishes a String message to a topic with name "topic" */
    publisher = this->create_publisher<std_msgs::msg::String>("topic", 10);
    /* Creates a timer that triggers every half a second, calling the timer_callback() function
     * every half a second
    */
    timer = this->create_wall_timer(
    500ms, std::bind(&MinimalPublisher::timer_callback, this));
}

/* This function is called every half a second, and publishes "Hello, world! <n>" where
 * n increases from 0 every time. It also logs to stdout the message that it's about
 * to publish
*/
void MinimalPublisher::timer_callback() {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher->publish(message);
}

/* When running automated testing, the ENABLE_TESTING flag is set. This guard
 * ensures that we don't define the main() function multiple times.
*/
#ifndef ENABLE_TESTING


/* This is a simple main method that just initialises ROS2 and sets the publisher running.*/
int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}

#endif // ENABLE_TESTING
