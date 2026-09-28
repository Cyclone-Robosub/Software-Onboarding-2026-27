cmake_minimum_required(VERSION 3.8)
project(gamepad_interpreter)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

# Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

# add executables
add_executable(gamepad_interpreter src/gamepad_interpreter.cpp)
ament_target_dependencies(gamepad_interpreter rclcpp std_msgs)

install(TARGETS
  gamepad_interpreter
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  
  # Create a library from the source for testing (WITH ENABLE_TESTING defined)
  add_library(${PROJECT_NAME}_lib src/gamepad_interpreter.cpp)
  target_compile_definitions(${PROJECT_NAME}_lib PUBLIC ENABLE_TESTING)
  ament_target_dependencies(${PROJECT_NAME}_lib
    rclcpp
    std_msgs
    custom_interfaces
  )
  target_include_directories(${PROJECT_NAME}_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  
  # Add test executable and link with library
  ament_add_ros_isolated_gtest(${PROJECT_NAME}_test test/test_gamepad_interpreter.cpp)
  target_compile_definitions(${PROJECT_NAME}_test PUBLIC ENABLE_TESTING)
  target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_lib)
  target_include_directories(${PROJECT_NAME}_test PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  ament_target_dependencies(${PROJECT_NAME}_test
    rclcpp
    std_msgs
    custom_interfaces
  )
endif()

ament_package()
