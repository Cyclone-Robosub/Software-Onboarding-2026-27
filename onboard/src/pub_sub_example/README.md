# `pub_sub_example`

## What is it?
This is a small ROS2 system designed as a working example and for sanity checks (like a "Hello, world!" program). It is mostly taken from the ROS2 documentation [here](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html). It provides two nodes, one of which repeatedly publishes a basic "Hello, world!" message with a count, and the other which simply prints that message to the terminal upon reception.

## How do I use it?
Make sure you're at the root of the repository (i.e. `Software-Onboarding-2026-27`), then run `colcon build` followed by `source install/setup.bash`.

First run `ros2 run pub_sub_example subscriber`, then run `ros2 run pub_sub_example publisher` in a different terminal window (both from the root of the repo). You should see both printing the same message: the subscriber received the message from the publisher.

## What topics/services/actions does the package use for input?
None: all communication is internal to the package.

## What topics/services/actions does the package use for output?
None

## What custom message types or libraries does the package use?
None

## Notes (Optional)
None