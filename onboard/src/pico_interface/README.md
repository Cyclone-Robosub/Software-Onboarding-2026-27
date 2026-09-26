# `pico_interface`

## What is it?
This package communicates with the Raspberry Pi Pico, sending thruster PWMs to it over a serial connection. These thruster PWMs will directly control each individual thruster: it's the last step in the control chain.

## How do I use it? **{TODO}**
Provide the necessary instructions to interact with the project from a user (not developer) perspective. For example, if your package has a user interface, specify valid inputs and the corresponding behaviour/output. Specify any common error messages or user mistakes, and resolutions.

Provide launch instructions for each node in the package (i.e. `ros2 run package_name node_name` or a launch file instruction).

## What topics/services/actions does the package use for input? **{TODO add more inputs}**
- Topics:
    - `topic_name`: description
    - `thruster_manager_heartbeat`: heartbeat from the `thruster_manager` node

## What topics/services/actions does the package use for output? **{TODO add any extra outputs as needed}**
- Topics:
    - `topic_name`: description
    - `pico_interface_heartbeat`: the heartbeat for this package

## What custom message types or libraries does the package use?
This is the place to list all custom messages, services, actions, or libraries (from `/core/src`) that are used in the project.
