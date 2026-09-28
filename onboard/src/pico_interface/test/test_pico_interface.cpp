#include <gtest/gtest.h>
#include "rclcpp/rclcpp.hpp"

#include "pico_interface.hpp"

class TestSubscriber : public ::testing::Test {
protected:
    std::shared_ptr<PicoInterface> node;

    void SetUp() override {
        if (!rclcpp::ok()) {
            rclcpp::init(0, nullptr);
        }
    }

    void TearDown() override {
        node.reset();
    }

    void create_node() {
        node = std::make_shared<PicoInterface>();
    }
};

TEST_F(TestSubscriber, NodeConstruction) {
    ASSERT_NO_THROW({
        create_node();
    });

    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->get_name(), std::string("pico_interface"));
}

#ifdef ENABLE_TESTING

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // ENABLE_TESTING
