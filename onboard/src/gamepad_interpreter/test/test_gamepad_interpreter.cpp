#include <gtest/gtest.h>
#include "rclcpp/rclcpp.hpp"

#include "custom_interfaces/msg/gamepad.hpp"
#include "gamepad_interpreter.hpp"

class TestSubscriber : public ::testing::Test {
protected:
    std::shared_ptr<GamepadInterpreter> node;
    rclcpp::Publisher<custom_interfaces::msg::Gamepad>::SharedPtr gamepad_publisher;

    void SetUp() override {
        if (!rclcpp::ok()) {
            rclcpp::init(0, nullptr);
        }
    }

    void TearDown() override {
        node.reset();
    }

    void create_node() {
        node = std::make_shared<GamepadInterpreter>();
        gamepad_publisher = node->create_publisher<custom_interfaces::msg::Gamepad>("ps5_controller", 10);
    }
};

TEST_F(TestSubscriber, NodeConstruction) {
    ASSERT_NO_THROW({
        create_node();
    });

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->get_name(), std::string("gamepad_interpreter"));
}

#ifdef ENABLE_TESTING

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // ENABLE_TESTING
