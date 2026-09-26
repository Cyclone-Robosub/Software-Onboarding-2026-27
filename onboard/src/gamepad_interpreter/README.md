# `gamepad_interpreter`

## What is it?
This package converts gamepad actions (pressing a button, moving a joystick) into thruster power for each of Manny's thrusters. Each of the eight thrusters will get a value between -1 and 1, representing either full reverse or full fowards power, respectively. This will get sent to `thruster_manager` to be converted into PWM values.

## How do I use it? **{TODO}**
Provide the necessary instructions to interact with the project from a user (not developer) perspective. For example, if your package has a user interface, specify valid inputs and the corresponding behaviour/output. Specify any common error messages or user mistakes, and resolutions.

Provide launch instructions for each node in the package (i.e. `ros2 run package_name node_name` or a launch file instruction).

## What topics/services/actions does the package use for input?
- Topics:
    - `ps5_controller`: the current gamepad state
    - `joystick_heartbeat`: heartbeat from the joystick website

## What topics/services/actions does the package use for output? **{TODO add the outputs you choose}**
- Topics:
    - `topic_name`: description
    - `gamepad_interpreter_heartbeat`: heartbeat for this package

## What custom message types or libraries does the package use?
This uses the `custom_interfaces/msg/Gamepad` and **{TODO anything else}** Everything else is a standard message type from ROS.
