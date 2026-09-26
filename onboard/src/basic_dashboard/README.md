# `basic_dashboard`

## What is it?
This is a non-interactive dashboard showing the current gamepad status (what buttons are pressed, joystick axes, etc.) and the current robot status. This includes the thruster power sent between each component, the final output sent to the thrusters, and the heartbeat status for each of the components.

## How do I use it? **{TODO}**
Provide the necessary instructions to interact with the project from a user (not developer) perspective. For example, if your package has a user interface, specify valid inputs and the corresponding behaviour/output. Specify any common error messages or user mistakes, and resolutions.

Provide launch instructions for each node in the package (i.e. `ros2 run package_name node_name` or a launch file instruction).

## What topics/services/actions does the package use for input? **{TODO add additional inputs}**
- Topics:
    - `ps5_controller`: the current gamepad state
    - `topic_name`: description
    - `joystick_heartbeat`: heartbeat for the joystick website

## What topics/services/actions does the package use for output?
None

## What custom message types or libraries does the package use?
This uses the `custom_interfaces/msg/Gamepad` and **{TODO anything else}** Everything else is a standard message type from ROS.
