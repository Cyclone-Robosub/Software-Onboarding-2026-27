/* gtest, i.e. GoogleTest, is what we use for our C++ testing infrastructure */
#include <gtest/gtest.h>
#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/log.hpp"

#include <chrono>

#include "subcriber.hpp"
#include "publisher.hpp"

/* Tests must inherit from the gtest Test class "testing::Test" to be registered and run */
class TestSubscriber : public testing::Test {
protected:
    /* A node that we will spin up for testing */
    std::shared_ptr<MinimalSubscriber> node;
    /* Used to simulate publishing messages from upstream sources */
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    /* Used to capture output from the node that would be sent downstream */
    rclcpp::Subscription<rcl_interfaces::msg::Log>::SharedPtr output_subscription;
    /* Cumulatively stores all output we capture so that we can evaluate it for correctness */
    std::string output;

    /* If we set up our tests as fixtures, it will use the SetUp() function at the start of 
     * every test and TearDown() at the end of every test. We can initialize and clean up variables
     * and state before/after every test using these functions.
    */
    void SetUp() override {
        /* Initialise ROS2 if not yet initialised */
        if (!rclcpp::ok()) {
            rclcpp::init(0, nullptr);
        }
        output = std::string();
    }

    void TearDown() override {
        node.reset();
    }

    /* This sets up the MinimalSubscriber node and the publisher and subscriber used to create and
     * capture input and output respectively. We don't do this in the SetUp() function so that we can
     * test these steps seperately in unit testing. 
    */
    void create_node() {
        node = std::make_shared<MinimalSubscriber>();
        /* This would be published from MinimalPublisher, but we simulate it ourselves here so that 
         * we can control the node input to create desired testing scenarios. 
        */
        publisher = node->create_publisher<std_msgs::msg::String>("topic", 10);
        /* rosout is created automatically and will capture everything logged using the ROS2 logger. */
        output_subscription = node->create_subscription<rcl_interfaces::msg::Log>(
            "rosout", 10, std::bind(&TestSubscriber::append_output_callback, this, std::placeholders::_1)
        );
    }

    /* Helper function to publish a message with the format that MinimalPublisher would provide */
    void publish_standard_message_with_count(int count) {
        publish_custom_message("Hello, world! " + std::to_string(count));
    }

    /* Helper function to publish a message containing whatever we want */
    void publish_custom_message(std::string custom_message) {
        auto message = std_msgs::msg::String();
        message.data = custom_message;
        publisher->publish(message);
    }

    /* This callback is called whenever MinimalSubscriber prints to output. We capture the 
     * output and store it in the output variable. 
    */
    void append_output_callback(std::shared_ptr<rcl_interfaces::msg::Log> msg) {
        output += msg->msg;
    }
};

class TestPublisher : public testing::Test {
protected:
    std::shared_ptr<MinimalPublisher> node;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;
    std::string last_message_sent;

    void SetUp() override {
        if (!rclcpp::ok()) {
            rclcpp::init(0, nullptr);
        }
        last_message_sent = std::string();
    }

    void TearDown() override {
        node.reset();
    }

    void create_node() {
        node = std::make_shared<MinimalPublisher>();
        subscription = node->create_subscription<std_msgs::msg::String>(
            "topic", 10, std::bind(&TestPublisher::subscription_callback, this, std::placeholders::_1)
        );
    }

    void subscription_callback(std_msgs::msg::String msg) {
        last_message_sent = msg.data;
    }

};

/* This is a basic sanity check I recommend including with all your tests. It just checks
 * that we can create a node and that it has the correct name.
*/
TEST_F(TestSubscriber, NodeConstruction) {
    ASSERT_NO_THROW({
        create_node();
    });

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->get_name(), std::string("minimal_subscriber"));
}

/* ~~~~~~~~~~~~~~
 * From here, you'll need to write tests that match your specific project. Create
 * tests to verify the behaviour of your nodes rather than the implementation: we only
 * care that the node provides the correct output on a given input, and shouldn't
 * restrict ourselves to a specific implementation strategy. Try to think of edge cases
 * and error scenarios, and test those as well.
 * ~~~~~~~~~~~~~~
*/

/* Tests that the node outputs the correct message with the message format that 
 * is expected to be published from MinimalPublisher
*/
TEST_F(TestSubscriber, CorrectOutputOnStandardMessage) {
    create_node();

    /* Using executors and spinning directly in a seperate thread allows us to publish messages
     * in our test and have them be concurrently received by the MinimalSubscriber node 
    */
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    std::thread spin_thread([&executor]() {
        executor.spin();
    });

    
    publish_standard_message_with_count(0);
    publish_standard_message_with_count(42);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_EQ("I heard: 'Hello, world! 0'I heard: 'Hello, world! 42'", output);

    executor.cancel();
    spin_thread.join();
}

/* We should be able to accept strings of any format and print that we heard them,
 * even though MinimalPublisher shouldn't actually publish messages with this format.
*/
TEST_F(TestSubscriber, CorrectOutputOnCustomMessage) {
    create_node();

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    std::thread spin_thread([&executor]() {
        executor.spin();
    });

    publish_custom_message("Forty-two");

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_EQ("I heard: 'Forty-two'", output);

    executor.cancel();
    spin_thread.join();
}

TEST_F(TestPublisher, NodeConstruction) {
    ASSERT_NO_THROW({
        create_node();
    });

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->get_name(), std::string("minimal_publisher"));
}

TEST_F(TestPublisher, PublishesCorrectMessage) {
    create_node();

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    std::thread spin_thread([&executor]() {
        executor.spin();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1300));

    EXPECT_EQ("Hello, world! 1", last_message_sent);

    executor.cancel();
    spin_thread.join();
}

TEST_F(TestPublisher, IncrementsCountEveryMessage) {
    create_node();

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);

    std::thread spin_thread([&executor]() {
        executor.spin();
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    EXPECT_EQ("Hello, world! 0", last_message_sent);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ("Hello, world! 1", last_message_sent);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ("Hello, world! 2", last_message_sent);

    executor.cancel();
    spin_thread.join();
}

#ifdef ENABLE_TESTING

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // ENABLE_TESTING