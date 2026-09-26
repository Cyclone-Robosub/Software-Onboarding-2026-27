# PS5 Gamepad to control the submarine

# Type name: `Gamepad`
| Name                 | Type      | Value range  | Notes                 |
| -------------------- | --------- | ------------ | --------------------- |
| x                    | `float32` | [-1.0, 1.0]  | Left joystick x axis  |
| y                    | `float32` | [-1.0, 1.0]  | Left joystick y axis  |
| rise                 | `float32` | [0.0, 1.0]   | Activates rise (R2)   |
| sink                 | `float32` | [0.0, 1.0]   | Activates sink (L2)   |
| yaw                  | `float32` | [-1.0, 1.0]  | Right joystick x axis |
| pitch                | `float32` | [-1.0, 1.0]  | Right joystick y axis |
| cross_button         | `bool`    | {true, false}| Whether the cross face button is pressed |
| square_button        | `bool`    | {true, false}|  |
| triangle             | `bool`    | {true, false}|  |
| circle_button        | `bool`    | {true, false}|  |
| dpad_down            | `bool`    | {true, false}|  |
| dpad_left            | `bool`    | {true, false}|  |
| dpad_up              | `bool`    | {true, false}|  |
| dpad_right           | `bool`    | {true, false}|  |
| bumper_left          | `bool`    | {true, false}| L1 |
| bumper_right         | `bool`    | {true, false}| R1 |
| start                | `bool`    | {true, false}|  |
| select               | `bool`    | {true, false}|  |
| joystick_press_left  | `bool`    | {true, false}| L3 |
| joystick_press_right | `bool`    | {true, false}| R3 |

## How to run the web controller
1. `source install/setup.bash`
2. Run the rosbridge server first. This node bridge ros to the browser. 
```bash
$ ros2 launch rosbridge_server rosbridge_websocket_launch.xml
```
3. Open webpage (i.e. `$ firefox remote_control_webpage/index.html`)