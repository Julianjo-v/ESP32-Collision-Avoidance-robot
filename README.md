# ESP32 Collision Avoidance Robot

Lab project for **247-415-VA Robotics II: Mobile Robotic Systems** (Computer Engineering Technology), submitted 18 May 2026.

## Abstract

This project is the design and implementation of an ESP32-based autonomous collision avoidance robot. The robot uses three VL53L0X Time-of-Flight (ToF) sensors to detect obstacles in front and at both 45-degree angles, while a DRV8833 motor driver controls two DC motors for movement and steering. The system continuously reads sensor distances and makes real-time navigation decisions such as moving forward, reversing, and turning to avoid collisions. PWM motor control regulates movement speed and improves maneuverability. During testing, the robot successfully detected nearby obstacles and adjusted its direction autonomously based on available open space. The project demonstrates embedded programming, sensor integration, motor control, autonomous navigation, and real-time obstacle avoidance on the ESP32.

## Introduction

The ESP32 Collision Avoidance Robot is an autonomous robot designed to move around and avoid obstacles on its own. It uses an ESP32 microcontroller, DC motors with encoders, and three Time-of-Flight (ToF) distance sensors to detect objects and navigate safely. The robot can drive forward, detect walls or obstacles, and react by changing direction automatically.

## Main Features

- Dual N20 micro gear motors with magnetic quadrature encoders for closed-loop speed and position control
- Three VL53L0X Time-of-Flight sensors providing 180° forward obstacle detection (front, left-diagonal, right-diagonal)
- DRV8833 dual H-bridge motor driver with PWM speed control
- ESP32 WROOM microcontroller (dual-core, 240 MHz) running all control algorithms
- Proportional (P) speed controller for smooth, consistent motor performance
- LM7805 5V regulator providing stable logic power from a 7-12V battery pack

## System Architecture

| Subsystem | Key Component | Function |
| --- | --- | --- |
| Computation | ESP32 WROOM | Runs all control algorithms, reads sensors, outputs motor commands |
| Propulsion | 2x N20 Motor + Encoder | Drives wheels; encoders provide odometry feedback |
| Motor Driver | DRV8833 | H-bridge PWM speed and direction control for both motors |
| Sensing | 3x VL53L0X ToF | Laser distance measurement for obstacle detection |
| Power | 2x 18650 Cells + LM7805 | 7-12V battery feeds the motors directly (VM); LM7805 regulates 5V into the ESP32 VIN |

## Repository Contents

- `esp32_collision_avoidance/esp32_collision_avoidance.ino` - Arduino sketch for the robot

## Step-by-Step User Guide

### Equipment needed

- Assembled ESP32 Collision Avoidance Robot
- Charged 18650 battery pack (7-12V, minimum 2A continuous)
- USB-C cable
- Computer with Arduino IDE 2.x installed
- Multimeter (for power-on verification)
- Flat, clear floor area (minimum 3 m x 3 m for navigation tests)

### Powering on the robot

1. Confirm the power switch is in the OFF position.
2. Insert the charged 18650 battery pack into the holder and secure the lid.
3. Flip the power switch to ON. The ESP32 blue LED will illuminate.
4. Motors should spin at this point; if they do not, power off immediately and check motor driver wiring.

### Connecting to the Arduino IDE

1. Connect the USB-C cable from your computer to the ESP32 DevKit USB port.
2. Open the Arduino IDE. Under **Tools → Board**, select **ESP32 Dev Module**.
3. Under **Tools → Port**, select the COM port that appeared when you plugged in the cable (e.g., COM3 on Windows or /dev/ttyUSB0 on Linux/macOS).
4. Set Upload Speed to 115200 and Flash Frequency to 80 MHz.
5. To verify the connection, open **Tools → Serial Monitor** and press the EN (reset) button on the ESP32. You should see boot messages.

### Loading a program

1. Open the Arduino sketch (`.ino` file) from this repo.
2. Click **Verify** (checkmark icon) to compile. Fix any errors before continuing.
3. Click **Upload** (arrow icon).
4. While uploading, press the BOOT button on the ESP32 if needed. The ESP32 will reboot automatically after upload.
5. Open Serial Monitor (Ctrl+Shift+M) at 115200 baud to observe program output.
6. Press the EN button on the ESP32 to restart the program if needed.

### Running

1. Press the power button on the top of the robot.
2. Once on, the robot starts running and avoids any obstacle in front of it.

### Power off

Press the off switch to turn the robot off.

## Bill of Materials (BOM)

| Part | Description | Qty | Unit Price ($) | Price ($) | Where to find |
| --- | --- | --- | --- | --- | --- |
| ESP32 WROOM | Elegoo ESP32 WROOM dev. board | 1 | 9 | 26.99 | [Amazon](https://a.co/d/5GH8Q3q) |
| N20 motors | N20 gear motor with encoder, 100 RPM, DC 6V | 2 | 13.55 | 27.09 | [Amazon](https://a.co/d/idWsUWr) |
| Circuit box | 53-551-0 Project Box 125 x 60 x 40 mm enclosure | 1 | 8.48 | 8.48 | [Abra](https://abra-electronics.com/enclosures-boxes/plastic-box-generic/53-551-0-project-box-125-x-60-x-40-mm-plastic-abs-enclosure-grey.html) |
| Ball caster | 935-POLOLU Ball Caster with 1/2" Metal Ball | 1 | 5.69 | 5.69 | [Abra](https://abra-electronics.com/robotics/robot-wheel-adapters-hubs/953-pololu-ball-caster-with-1-2-metal-ball.html) |
| Rubber wheels | D-lock wheels | 2 | 1.64 | 16.41 | [Amazon](https://a.co/d/aKz8cLc) |
| ToF sensors | VL53L0X ranging sensor | 3 | 6.33 | 19.19 | [Amazon](https://a.co/d/2z55a3e) |
| Dual motor driver | Motor drivers | 1 | 1.39 | 1.39 | [AliExpress](https://www.aliexpress.com/item/1005008557663879.html) |
| 18650 batteries | Li-ion rechargeable battery, 3500 mAh | 2 | 19.75 | 39.50 | [Abra](https://abra-electronics.com/batteries-holders/batteries-polymer-lithium-ion/bat-18650-s35e-b-samsung-35e-18650-3500mah-8a-button-top-battery.html) |
| Battery holder | 18650 battery holder/case with switch. Batteries are in series | 1 | 5.25 | 10.50 | [Amazon](https://a.co/d/9fUBe1K) |
| Battery charger | XTAR VC2SL Battery Charger Powerbank | 1 | 25.99 | 25.99 | [Amazon](https://a.co/d/0277hrMu) |
| Screw terminal | 2442P Screw Terminal Block: 2-Pin, 5mm Pitch | 1 | 3.00 | 3.00 | [Abra](https://abra-electronics.com/interconnects/terminals-blocks-barrier/terminal-blocks/screw-terminal-blocks-stackable/2442p-screw-terminal-block-2-pin-5-mm-pitch-top-entry-4-pack.html) |
| 5V regulator | 7805T Positive Voltage Regulator 1A 5V | 1 | 0.65 | 0.65 | [Abra](https://abra-electronics.com/ics-semiconductors/transistors/voltage-regulators-en/7805t-transistor-positive-voltage-regulator-1a-5v-7805t.html) |
| Heatsink | HS-220AB-2W Heatsink Wide Black Aluminum | 1 | 0.99 | 0.99 | [Abra](https://abra-electronics.com/thermal-management/heat-sinks/hs-220ab-2w-heat-sink-wide-black-aluminum-to-220-with-double-mounting-pins.html) |

## Full System Schematic

[View the circuit diagram on Cirkit Designer](https://app.cirkitdesigner.com/project/785f8a58-f1ff-4eb8-a993-7227f19162d3)

### Power connections

- Battery (+) → DRV8833 (VM)
- Battery (+) → LM7805 (Input)
- Battery (+) → Electrolytic Capacitor (+)
- Battery (-) → Common GND
- LM7805 (Output) → ESP32 (VIN)
- LM7805 (Output) → Tantalum Capacitor (+)
- ESP32 (3V3) → DRV8833 (STBY), all three ToF sensor (VIN) pins, and both encoder (VCC) pins

### Common ground

Common GND connects to:
- ESP32 (GND)
- DRV8833 (GND)
- LM7805 (GND)
- All ToF sensor (GND) pins
- Both encoder (GND) pins
- Both capacitor (-) pins

### Motor driver ↔ ESP32

- ESP32 (D27) → DRV8833 (AIN1)
- ESP32 (D26) → DRV8833 (AIN2)
- ESP32 (D32) → DRV8833 (BIN1)
- ESP32 (D33) → DRV8833 (BIN2)
- ESP32 (3V3) → DRV8833 (STBY)

> **Note:** in the sketch, `AIN1 = 26` and `AIN2 = 27` are GPIO numbers, so the constant names are swapped relative to the physical DRV8833 pins above. The behavior is the same.

### Motor connections

**Left motor (Encoder N20 #1)**
- DRV8833 (BO1) → Motor (M1)
- DRV8833 (BO2) → Motor (M2)

**Right motor (Encoder N20 #2)**
- DRV8833 (AO1) → Motor (M1)
- DRV8833 (AO2) → Motor (M2)

### Encoder connections

**Left encoder**
- C1 → ESP32 (D18)
- C2 → ESP32 (D19)
- VCC → 3.3V

**Right encoder**
- C1 → ESP32 (D35)
- C2 → ESP32 (D34)
- VCC → 3.3V

### ToF sensor connections (I2C)

**Shared I2C lines**
- ESP32 (D21) → All ToF Sensors (SDA)
- ESP32 (D22) → All ToF Sensors (SCL)

**XSHUT pins**
- ESP32 (D15) → Left ToF Sensor (XSHUT)
- ESP32 (D4) → Center ToF Sensor (XSHUT)
- ESP32 (D2) → Right ToF Sensor (XSHUT)

**Power**
- ESP32 (3V3) → All ToF Sensors (VIN)

## Maintenance Plan

### Daily pre-operation checks

- Inspect all screw terminals for loose wires; tighten as needed
- Verify wheel rubber is intact and seated on the shaft; replace if cracked or slipping
- Check battery voltage before each session (minimum 7.0V for reliable operation)
- Verify the ball caster rolls freely in all directions
- Ensure no jumper wires are pulled out or intermittently connected

### Weekly maintenance

- Clean wheel rubber with a dry cloth to remove dust and maintain traction
- Inspect motor shafts for wobble; a loose D-shaft collar indicates the set-screw has loosened
- Check sensor windows on all three VL53L0X modules for dust; clean gently with a cotton swab
- Verify protoboard connections are firmly seated, since vibration can cause partial disconnection

### Monthly / semester-end maintenance

- Discharge and recharge 18650 cells to maintain capacity; replace cells below 70% capacity
- Inspect the heatsink thermal contact on the voltage regulator; re-apply thermal paste if loose
- Check chassis fasteners (screws, standoffs) for tightness; Loctite thread-locker is recommended for vibration-prone joints
- Update firmware and library dependencies in the Arduino IDE if newer versions are available

## Troubleshooting Guide

### Issue 1: Motors do not spin

| | |
| --- | --- |
| **Symptom** | One or both motors show zero movement despite code upload and serial messages appearing. |
| **Root cause** | VM rail not connected, motor output terminals loose, or motor driver STBY pin held low. |
| **Fix applied** | 1. Measure VM at the screw terminal with a multimeter; it should read 7-12V. 2. Check AO1/AO2 and BO1/BO2 motor output pins are firmly in screw terminals. 3. Verify the DRV8833 STBY pin is tied HIGH (3.3V) and not floating. |
| **Result** | After confirming VM at 8.4V and tightening the AO1 terminal, the motor resumed normal operation. |

### Issue 2: ToF sensor reports timeout / zero

| | |
| --- | --- |
| **Symptom** | Serial Monitor shows a sensor FAILED message, or distance reads as 0 mm or 8190 mm (error code). |
| **Root cause** | XSHUT pin wired to the wrong GPIO, SDA/SCL swapped, or address collision from an incomplete initialization sequence. |
| **Fix applied** | 1. Power off and verify XSHUT wiring: Left → D15, Center → D4, Right → D2. 2. Confirm SDA is on GPIO21 and SCL is on GPIO22 (not reversed). 3. Power cycle fully so all sensors reset to 0x29 before re-running initialization. |
| **Result** | Swapping the reversed SDA/SCL jumper wires resolved the issue; all three sensors initialized at their unique addresses (0x30, 0x31, 0x32). |

## Demo Video

[Watch the demo on YouTube](https://youtu.be/pEJDUwm_-jM)

## Reflection on Technical Challenges

The primary challenge on this project was time. Due to the constraints of the semester schedule alongside other coursework, most of the planned subsystems could not be implemented. The only component that was fully completed was the obstacle avoidance functionality.

The hardware was assembled and the basic wiring was verified, but encoder calibration, the odometry system, forward and inverse kinematics, the proportional speed controller, and waypoint following were left incomplete. These systems depend heavily on one another: without reliable encoder calibration there is no odometry, and without odometry there is no waypoint navigation.

The obstacle avoidance implementation showed that the ToF sensors and the ESP32 communicate correctly and that the basic reactive logic of detecting an obstacle and responding to it works.

Given more time, the priority would be to finish encoder calibration first, since every other software system depends on it, followed by the kinematics and the waypoint controller.

This project was completed individually. It reinforced the importance of scheduling and incremental testing, starting from hardware validation and building up layer by layer so that time pressure on later stages doesn't erase earlier progress.

## Code Explanation

The sketch controls an autonomous ESP32 obstacle avoidance robot that uses three VL53L0X Time-of-Flight distance sensors and a DRV8833 motor driver.

1. The program defines the motor control pins, PWM speed settings, and obstacle detection distances for the front and side sensors.
2. At startup, the ESP32 initializes the I2C bus and powers on each ToF sensor one at a time using the XSHUT pins, so each sensor can be assigned a unique I2C address.
3. The robot continuously reads distances from the left, center, and right sensors and compares them to predefined thresholds.
4. If the center sensor detects an obstacle directly ahead, the robot stops, reverses slightly, and turns toward the side with more open space.
5. If only the left or right angled sensor detects an obstacle, the robot makes a small corrective turn away from it.
6. If nothing is detected, both motors drive forward at full speed.

Motor movement uses PWM signals through the DRV8833, allowing the robot to move forward, backward, turn left, turn right, or stop. The loop repeats at about 20 times per second so the robot reacts in real time.
