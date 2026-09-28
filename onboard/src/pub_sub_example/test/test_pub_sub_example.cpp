#include <gtest/gtest.h>
#include <rclcpp/rclcpp.hpp>
#include "rcl_interfaces/msg/log.hpp"

#include <unistd.h>
#include <chrono>

#include "subcriber.hpp"
#include "publisher.hpp"

class TestSubscriber : public ::testing::Test {
protected:
    std::shared_ptr<MinimalSubscriber> node;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
    rclcpp::Subscription<rcl_interfaces::msg::Log>::SharedPtr output_subscription;
    std::string output;

    void SetUp() override {
        if (!rclcpp::ok()) {
            rclcpp::init(0, nullptr);
        }
        output = std::string();
    }

    void TearDown() override {
        node.reset();
    }

    void create_node() {
        node = std::make_shared<MinimalSubscriber>();
        publisher = node->create_publisher<std_msgs::msg::String>("topic", 10);
        output_subscription = node->create_subscription<rcl_interfaces::msg::Log>(
            "rosout", 10, std::bind(&TestSubscriber::append_output_callback, this, std::placeholders::_1)
        );
    }

    void publish_standard_message_with_count(int count) {
        auto message = std_msgs::msg::String();
        message.data = "Hello, world! " + std::to_string(count);
        publisher->publish(message);
    }

    void publish_custom_message(std::string custom_message) {
        auto message = std_msgs::msg::String();
        message.data = custom_message;
        publisher->publish(message);
    }

    void append_output_callback(std::shared_ptr<rcl_interfaces::msg::Log> msg) {
        output += msg->msg;
    }
};

class TestPublisher : public ::testing::Test {
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

TEST_F(TestSubscriber, NodeConstruction) {
    ASSERT_NO_THROW({
        create_node();
    });

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->get_name(), std::string("minimal_subscriber"));
}

TEST_F(TestSubscriber, CorrectOutputOnStandardMessage) {
    create_node();

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