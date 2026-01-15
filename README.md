# Semi-Autonomous Vehicle Project

ESP32-based semi-autonomous 4-wheel robotic car with joystick control, ultrasonic obstacle avoidance, and web-based mode switching.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Dependencies](#software-dependencies)
- [Project Structure](#project-structure)
- [Setup and Installation](#setup-and-installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

## Overview

This project implements a semi-autonomous robotic vehicle controlled by an ESP32 microcontroller. The vehicle can operate in two modes:
- **Manual Mode**: Controlled via joystick (USB/Bluetooth) or web interface
- **Autonomous Mode**: Uses ultrasonic sensors for obstacle avoidance and autonomous navigation

The system uses WebSocket communication for real-time control and data transmission, with an optional IP camera feed for remote monitoring.

## Features

- **Dual-Mode Operation**: Switch between manual and autonomous modes
- **Joystick Control**: Support for USB and Bluetooth joystick input
- **Obstacle Avoidance**: Ultrasonic sensor-based collision prevention
- **Web Interface**: Browser-based control panel with live camera feed
- **Real-time Telemetry**: WebSocket-based distance sensor data streaming
- **PWM Motor Control**: Smooth speed control for DC motors
- **WiFi Connectivity**: Remote operation over local network

## Hardware Requirements

### Essential Components
- **ESP32 Development Board** (e.g., ESP32-WROOM-32)
- **Motor Driver**: L298N or similar H-bridge motor driver
- **DC Motors**: 4x DC geared motors with wheels
- **Ultrasonic Sensor**: HC-SR04 or compatible (5V or 3.3V)
- **Power Supply**: 
  - 7.4V - 12V battery pack for motors
  - 5V power supply or USB for ESP32
- **Joystick** (Optional): USB or Bluetooth gamepad/joystick

### Optional Components
- **IP Camera**: For live video streaming (e.g., smartphone camera app)
- **Chassis**: 4-wheel robot car chassis kit
- **Breadboard and Jumper Wires**: For prototyping

### Pin Connections

**Motor Driver Pins:**
```
Motor 1 (Left):
- Forward: GPIO 25
- Backward: GPIO 26
- Enable (PWM): GPIO 32/33

Motor 2 (Right):
- Forward: GPIO 27/18
- Backward: GPIO 14/19
- Enable (PWM): GPIO 19/33
```

**Ultrasonic Sensor:**
```
- Trigger Pin: GPIO 4/5/12
- Echo Pin: GPIO 2/14/18
```

*Note: Pin assignments may vary between different code versions in the project.*

## Software Dependencies

### Python Dependencies

#### Required Libraries
```bash
# Joystick control
pygame>=2.0.0

# HTTP requests for manual control
requests>=2.25.0

# WebSocket communication
websockets>=10.0

# Bluetooth support (for Bluetooth joystick)
pybluez>=0.23  # May require additional system packages

# Async I/O
asyncio  # Built-in for Python 3.7+
```

#### Missing/Recommended Python Packages
Some Python scripts use the following imports without explicit package specifications:
- **React (design.py)**: This appears to be a React component mistakenly placed in a `.py` file. It should be in a `.jsx` or `.tsx` file.

### Arduino/ESP32 Dependencies

#### Required Arduino Libraries
Install these through Arduino IDE Library Manager or PlatformIO:

```
# Core ESP32 libraries
- WiFi (built-in)
- WebServer (built-in for some ESP32 cores)

# WebSocket communication
- WebSocketsServer by Markus Sattler
  URL: https://github.com/Links2004/arduinoWebSockets

# JSON parsing
- ArduinoJson by Benoit Blanchon (v6.x recommended)
  URL: https://arduinojson.org/

# Ultrasonic sensor library
- NewPing by Tim Eckel
  URL: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home

# Bluetooth (for Bluetooth variants)
- BluetoothSerial (built-in ESP32 library)

# USB Host (for direct USB joystick)
- USBHost (if using ESP32 with USB OTG support)
```

#### Missing/Recommended Arduino Libraries
Some sketches reference these libraries which may need installation:
- **joystick_handler.h**: Custom header file referenced in `ESP32_sketch.ino` but not included in the repository

### System Requirements

**For Python Environment:**
- Python 3.7 or higher
- pip package manager

**For ESP32 Development:**
- Arduino IDE (1.8.13+) or PlatformIO
- ESP32 Board Support Package
  - Arduino IDE: Add ESP32 boards via Boards Manager
  - URL: `https://dl.espressif.com/dl/package_esp32_index.json`

**For PyBluez (Linux):**
```bash
sudo apt-get install libbluetooth-dev
sudo apt-get install bluez bluetooth
```

**For PyBluez (Windows):**
- Download and install Microsoft Visual C++ 14.0 or greater
- May require manual installation of Bluetooth drivers

## Project Structure

> **Note:** Some directory and file names contain typos (e.g., `experment_code_2` instead of `experiment_code_2`, `joystick_contoler.py` instead of `joystick_controller.py`). These names are documented as they exist in the repository.

```
semi-autonomous_vehicle_project/
├── README.md                          # This file
├── design.py                          # React component (misplaced, should be .jsx)
├── joystick_python.py                # Simple joystick controller via HTTP
├── example.json                       # WebSocket message format example
├── webpage.html                       # Web control interface v1
├── webpage_experiment_2.html         # Web control interface v2
│
├── esp_code/
│   └── esp_code.ino                  # Basic ESP32 code with HTTP server
│
├── sketch_nov29a/
│   └── sketch_nov29a.ino             # Simple ultrasonic distance measurement
│
├── sonar_distance_code/
│   └── sonar_distance_code.ino       # WebSocket-based distance streaming
│
└── experment_code_2/
    ├── rc_car_controller.py          # Advanced Python controller with async
    ├── rc_car_controller_2.py        # Alternative controller implementation
    ├── tempCodeRunnerFile.py         # Temporary file (can be ignored)
    │
    ├── working_esp_code/
    │   └── working_esp_code.ino      # Stable ESP32 code with WebSocket
    │
    ├── esp_code_claude/
    │   └── esp_code_claude.ino       # ESP32 variant with enhanced features
    │
    ├── esp_code_claude_2/
    │   └── esp_code_claude_2.ino     # ESP32 variant v2
    │
    ├── esp_code_claude_3/
    │   └── esp_code_claude_3.ino     # ESP32 variant v3
    │
    └── experiment_code_3/
        ├── joystick_controller.py    # Enhanced joystick controller
        │
        ├── New_joystick_code/
        │   ├── joystick_contoler.py  # Bluetooth joystick controller
        │   ├── bluetooth_controller.h # Bluetooth helper class
        │   ├── motor_control.h        # Motor control helper class
        │   └── ESP32_sketch/
        │       └── ESP32_sketch.ino   # USB joystick ESP32 code
        │
        ├── only_autonomous_code/
        │   └── only_autonomous_code.ino # Autonomous-only operation
        │
        └── esp_code/
            ├── esp_code.ino           # Main ESP32 implementation
            └── esp_code2.ino          # Alternative ESP32 implementation
```

## Setup and Installation

### 1. Hardware Assembly

1. **Connect Motor Driver to ESP32:**
   - Follow pin connections listed in [Hardware Requirements](#hardware-requirements)
   - Connect motor driver to DC motors
   - Ensure proper power supply connections

2. **Connect Ultrasonic Sensor:**
   - Connect VCC to 5V (or 3.3V for 3.3V sensors)
   - Connect GND to GND
   - Connect Trigger and Echo pins as per your code version

3. **Power Setup:**
   - Connect motor power supply to motor driver VCC/GND
   - Connect ESP32 to USB or separate 5V power supply

### 2. Software Installation

#### Python Environment Setup

```bash
# Create virtual environment (recommended)
python -m venv venv

# Activate virtual environment
# Windows:
venv\Scripts\activate
# Linux/Mac:
source venv/bin/activate

# Install dependencies
pip install pygame requests websockets

# For Bluetooth support (optional)
pip install pybluez
```

#### ESP32 Arduino Setup

1. **Install Arduino IDE** or PlatformIO

2. **Add ESP32 Board Support:**
   - Arduino IDE: File → Preferences → Additional Board Manager URLs
   - Add: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install

3. **Install Required Libraries:**
   - Sketch → Include Library → Manage Libraries
   - Search and install:
     - WebSocketsServer
     - ArduinoJson
     - NewPing

4. **Select Board:**
   - Tools → Board → ESP32 Arduino → Select your ESP32 board

### 3. Configuration

#### ESP32 Code Configuration

Edit the following in your chosen `.ino` file:

```cpp
// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

#### Python Controller Configuration

Edit the configuration in `rc_car_controller.py` or equivalent:

```python
# ESP32 IP address (check serial monitor after ESP32 connects)
ESP32_IP = "http://192.168.x.x"  
WEBSOCKET_URL = "ws://192.168.x.x:81"

# Camera URL (if using IP camera)
CAMERA_URL = "http://192.168.x.x:8080/video"
```

#### Web Interface Configuration

Edit `webpage.html` or `webpage_experiment_2.html`:

```javascript
// WebSocket connection
const ws = new WebSocket("ws://192.168.x.x:81");

// Camera feed (if using IP camera)
<img id="cameraFeed" src="http://192.168.x.x:8080/video" />
```

## Usage

### Method 1: Using ESP32 with Joystick Controller (Recommended)

1. **Upload ESP32 Code:**
   ```bash
   # Use working_esp_code.ino or experiment_code_3/esp_code/esp_code.ino
   # Upload via Arduino IDE
   ```

2. **Get ESP32 IP Address:**
   - Open Serial Monitor (115200 baud)
   - Note the IP address displayed after WiFi connection

3. **Run Python Joystick Controller:**
   ```bash
   # Navigate to the appropriate directory
   cd experment_code_2/experiment_code_3
   
   # Run the controller
   python joystick_controller.py
   ```

4. **Control the Vehicle:**
   - Use joystick axes for forward/backward/left/right movement
   - Press Button 0 (typically 'A' button) to toggle between manual and autonomous modes

### Method 2: Using Simple HTTP Joystick Control

1. **Upload ESP32 Code:**
   ```bash
   # Use esp_code/esp_code.ino
   ```

2. **Run Simple Controller:**
   ```bash
   python joystick_python.py
   ```

### Method 3: Using Web Interface

1. **Upload ESP32 Code with WebSocket support:**
   ```bash
   # Use working_esp_code.ino or sonar_distance_code.ino
   ```

2. **Open Web Interface:**
   - Open `webpage.html` or `webpage_experiment_2.html` in a web browser
   - Update IP addresses in the HTML file as needed
   - Click button to toggle between manual and autonomous modes

### Method 4: Bluetooth Joystick Control

1. **Upload Bluetooth-enabled ESP32 Code:**
   ```bash
   # Use experiment_code_3/New_joystick_code/ESP32_sketch/ESP32_sketch.ino
   ```

2. **Pair ESP32 via Bluetooth:**
   - Pair your computer with ESP32 Bluetooth
   - Note the MAC address

3. **Run Bluetooth Controller:**
   ```bash
   cd experment_code_2/experiment_code_3/New_joystick_code
   
   # Edit joystick_contoler.py and update MAC address
   python joystick_contoler.py
   ```

### Method 5: Autonomous-Only Mode

1. **Upload Autonomous Code:**
   ```bash
   # Use experiment_code_3/only_autonomous_code/only_autonomous_code.ino
   ```

2. **Power On:**
   - Vehicle will automatically navigate and avoid obstacles

## Configuration

### Motor Speed Adjustment

Edit motor speed values in ESP32 code:

```cpp
// Adjust base speed (0-255)
int baseSpeed = 200;

// PWM control
analogWrite(motor1Enable, 255);  // Full speed
analogWrite(motor2Enable, 255);

// Motor compensation for drift
#define LEFT_MOTOR_SPEED_COMPENSATION 100
```

### Obstacle Detection Parameters

```cpp
// Distance thresholds in centimeters
#define SAFE_DISTANCE 30          // Stop/turn distance
#define MAX_DISTANCE 200          // Sensor maximum range
#define SCAN_INTERVAL 250         // Time between scans (ms)
#define TURN_DURATION 800         // Turn duration (ms)
```

### Joystick Sensitivity

```python
# Deadzone threshold (0.0 - 1.0)
DEADZONE = 0.2

# Joystick threshold
if y_axis < -0.5:  # Forward
if y_axis > 0.5:   # Backward
```

## Troubleshooting

### ESP32 Not Connecting to WiFi
- Verify SSID and password are correct
- Ensure ESP32 is within WiFi range
- Check if WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
- Try power cycling the ESP32

### Joystick Not Detected
```bash
# Test joystick detection
python -c "import pygame; pygame.init(); pygame.joystick.init(); print(f'Joysticks: {pygame.joystick.get_count()}')"
```

### WebSocket Connection Failed
- Verify ESP32 IP address is correct
- Check that WebSocket port 81 is not blocked
- Ensure ESP32 code includes WebSocket server initialization
- Check firewall settings

### Motors Not Moving
- Check motor driver connections and power supply
- Verify GPIO pins match code configuration
- Test motor driver by connecting directly to power
- Check if enable pins are receiving PWM signals

### Ultrasonic Sensor Returning 0
- Verify trigger and echo pin connections
- Check sensor power supply (5V or 3.3V as appropriate)
- Ensure no obstacles within 2cm of sensor
- Try different GPIO pins

### Python Import Errors
```bash
# Reinstall dependencies
pip install --upgrade pygame requests websockets

# For Bluetooth issues on Linux
sudo apt-get install python3-dev libbluetooth-dev
pip install pybluez
```

### Missing Arduino Libraries
- Open Arduino IDE Library Manager (Sketch → Include Library → Manage Libraries)
- Search for: WebSocketsServer, ArduinoJson, NewPing
- Install latest compatible versions

### Camera Feed Not Loading
- Verify camera IP address and port
- Ensure camera app is running and streaming
- Check network connectivity between devices
- Try accessing camera URL directly in browser

## Contributing

This is an experimental project with multiple code variants. When contributing:
1. Test your changes with actual hardware
2. Document any new dependencies
3. Update pin configurations if changed
4. Maintain backward compatibility where possible

## License

This project is open source. Please check with the repository owner for specific license terms.

## Acknowledgments

- ESP32 community for excellent libraries
- Contributors to WebSocketsServer, ArduinoJson, and NewPing libraries
