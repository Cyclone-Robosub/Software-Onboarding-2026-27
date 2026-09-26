# Software-Onboarding-2026-27
A repository to contain the code created during the onboarding process. This is distinct from the competition-ready repository.

## Projects

Each student will pick a sub-project to work on. Documentation on project requirements is included in the README for each sub-project, and each should build (but not actually do anything interesting yet).

The easiest is likely `thruster_manager`, so students with less experience with coding (particularly incoming first-year students) are encouraged to work there. This is a service to convert thruster percentages to PWM values and pass that down the control chain.

For students with a bit of coding experience, or who feel particularly motivated to learn, can pick either `gamepad_interpreter` or `basic_dashboard`. The former converts gamepad state into thruster power percentages, and the latter displays robot and gamepad state in a terminal user interface (TUI).

Students who are fairly confident coders but haven't used ROS2 before are encouraged to pick `pico_interface`. This handles communication with a Raspberry Pi microcontroller and passes along thruster PWM values to it. If students on this team are interested, they can also write their own code to run on the Pico rather than re-using the code from last year.

## Basic ROS2 Commands

ROS2 documentation can be found [here](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html).

### Build projects
- Make sure you're at the top of the project directory
    - Shell prompt should *end* in `Software-Onboarding-2026-27$`
- Run `$ colcon build` to build all projects
- Or run `$ colcon build --packages-select <package_name>` if you want to build just your package

### Run projects
- Make sure you're at the top of the project directory
- Run `$ ros2 run <package_name> <node_name>`
- Stop running with `ctrl + c`

### Test projects (if tests are written)
- Make sure you're at the top of the project directory
- Run `$ colcon test` to test all projects
- Or run `$ colcon test --packages-select <package_name>` if you want to test just your package

## READMEs

Teams should add to the README files provided in their sub-projects, following the template below. This includes more information than you will be using (including services and actions), so just include what's relevant to your project.

---

# `package_name`

## What is it?
Provide an overview of the package. Specify what it does and how it integrates with the rest of our system, and and provide a brief overview of how it works on a high level.

## How do I use it?
Provide the necessary instructions to interact with the project from a user (not developer) perspective. For example, if your package has a user interface, specify valid inputs and the corresponding behaviour/output. Specify any common error messages or user mistakes, and resolutions.

Provide launch instructions for each node in the package (i.e. `ros2 run package_name node_name` or a launch file instruction).

## What topics/services/actions does the package use for input?
This is for topics that the package subscribes to, services that it responds to when called, and actions that it responds to and performs as a server. List them and provide a brief description of what they mean and are used for.
- Topics:
    - `topic_name`: description
    - `topic_name`: description
- Services:
    - `service_name`: description
    - `service_name`: description
- Actons;
    - `action_name`: description
    - `action_name`: description

## What topics/services/actions does the package use for output?
This is for topics that the package publishes to, services that it calls, and actions that it requests as a client. List them and provide a brief description of what they meand and are used for.
- Topics:
    - `topic_name`: description
    - `topic_name`: description
- Services:
    - `service_name`: description
    - `service_name`: description
- Actions:
    - `action_name`: description
    - `action_name`: description

## What custom message types or libraries does the package use?
This is the place to list all custom messages, services, actions, or libraries (from `/core/src`) that are used in the project. An example of a custom message is `Gamepad.msg`; an example of a custom service is `ControlMode.srv`; an example of of a custom library is `fd_interface`. This provides an easy way to check for breaking changes when we update our API. A simple list sufficies here.
- Messages:
    - `message_type`
    - `message_type`
- Services:
    - `service_type`
    - `service_type`
- Actions:
    - `action_type`
    - `action_type`
- Libraries:
    - `library`
    - `library`

## Notes (Optional)
If there is any extra information that would be useful for a fellow team member to understand the project, include it here.

---
