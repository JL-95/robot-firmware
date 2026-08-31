# Caldova Robot Firmware

Onboard control firmware for the Caldova sidewalk delivery rover. The process reads operator packets from a UART-style link, updates drive and routing state, and emits compact telemetry frames for the fleet console at caldova.io.

## Features

- Drive command dispatch for throttle, steering, and mode labels
- Route plan loading with waypoint payload transfer
- Pairing token storage for depot handoff stations
- Telemetry framing for battery, motion, and active route status
- Portable HAL so the same control loop runs on a workstation or the robot controller

## Setup

Requirements:

- CMake 3.16 or newer
- A C11 compiler such as MSVC, Clang, or GCC

```powershell
cmake -S . -B build
cmake --build build
```

## Usage

Run the binary and type one packet per line:

```text
DRV 35 -4 curbside
PAIR depot-rose-7
ROUTE lunch-42 3 010203040506
NOTE arrived at north gate
STAT
```

The workstation HAL prints actuator and telemetry activity to standard output. Hardware targets provide the same HAL functions over UART, motor control, battery ADC, and flash storage.
