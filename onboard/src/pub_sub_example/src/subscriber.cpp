/* header file with MinimalPublisher class declaration */
#include "subcriber.hpp"

/* Class constructor: creates a ROS2 Node with name "minimal_subscriber" */
MinimalSubscriber::MinimalSubscriber() :
    Node("minimal_subscriber") {

    /* Creates a subcription to the topic with name "topic" and message type String.
     * When a message is published on that topic (from the minimal_publisher node) the
     * topic_callback() function is called. The std::placeholders::_1 specifies the number of arguments
     * that the callback function takes.
    */
    subscription = this->create_subscription<std_msgs::msg::String>(
    "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, std::placeholders::_1));
}

/* This function is called whenever a message is published on the "topic" topic. It simply
 * prints what it received to the command line (stdout).
*/
void MinimalSubscriber::topic_callback(const std_msgs::msg::String & msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
}

/* When running automated testing, the ENABLE_TESTING flag is set. This guard
 * ensures that we don't define the main() function multiple times.
*/
#ifndef ENABLE_TESTING

/* This is a simple main method that just initialises ROS2 and sets the subscriber running.*/
int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}

#endif // ENABLE_TESTING
