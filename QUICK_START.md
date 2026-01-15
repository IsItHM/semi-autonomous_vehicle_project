# Quick Start Guide

## Overview
This is a semi-autonomous robotic vehicle project based on ESP32 with dual-mode operation (manual and autonomous).

## What You Need

### Hardware
- ESP32 development board
- L298N motor driver or similar
- 4x DC motors with wheels
- HC-SR04 ultrasonic sensor
- Battery pack (7.4V-12V for motors, 5V for ESP32)
- Optional: USB/Bluetooth joystick or IP camera

### Software
```bash
# Python dependencies
pip install -r requirements.txt

# Arduino libraries (install via Library Manager)
- WebSocketsServer
- ArduinoJson (v6.x)
- NewPing
```

## Quick Setup

### 1. Hardware Setup
- Connect motors to L298N driver
- Connect driver to ESP32 (see README.md for pins)
- Connect ultrasonic sensor to ESP32
- Power everything up

### 2. ESP32 Setup
1. Install ESP32 board support in Arduino IDE
2. Install required libraries
3. Open `experment_code_2/working_esp_code/working_esp_code.ino`
4. Update WiFi credentials:
   ```cpp
   #define WIFI_SSID "YOUR_WIFI"
   #define WIFI_PASSWORD "YOUR_PASSWORD"
   ```
5. Upload to ESP32
6. Note the IP address from Serial Monitor

### 3. Python Controller Setup
1. Install dependencies: `pip install -r requirements.txt`
2. Edit `experment_code_2/experiment_code_3/joystick_controller.py`
3. Update ESP32 IP address:
   ```python
   ESP32_IP = "192.168.x.x"  # Your ESP32's IP
   ```
4. Connect USB joystick
5. Run: `python joystick_controller.py`

## Control Methods

### Manual Mode
- Joystick: Move axes for forward/backward/left/right
- Web: Open `webpage_experiment_2.html` in browser

### Autonomous Mode
- Press joystick button 0 (A button) to toggle
- Or click "Switch to Autonomous" in web interface
- Vehicle will automatically avoid obstacles

## Troubleshooting

**ESP32 not connecting to WiFi?**
- Check SSID/password
- Ensure 2.4GHz network (not 5GHz)

**Motors not moving?**
- Check power supply
- Verify pin connections
- Test motor driver directly

**Joystick not detected?**
```bash
python -c "import pygame; pygame.init(); pygame.joystick.init(); print(pygame.joystick.get_count())"
```

**Need more help?**
- See full README.md for detailed instructions
- Check ARDUINO_LIBRARIES.md for library installation
- Review pin configurations in code comments

## Files to Use

**Recommended ESP32 code:**
- `experment_code_2/working_esp_code/working_esp_code.ino` (stable version)

**Recommended Python controller:**
- `experment_code_2/experiment_code_3/joystick_controller.py` (full featured)

**Simple HTTP control:**
- `joystick_python.py` (basic version)

**Autonomous only:**
- `experment_code_2/experiment_code_3/only_autonomous_code/only_autonomous_code.ino`

## Next Steps

1. ✅ Get basic manual control working via HTTP
2. ✅ Test WebSocket communication
3. ✅ Try autonomous mode with obstacle avoidance
4. ✅ Add camera feed for remote monitoring
5. ✅ Experiment with different control methods

## Documentation

- **README.md** - Complete documentation (495 lines)
- **ARDUINO_LIBRARIES.md** - Detailed library setup guide
- **requirements.txt** - Python dependencies
- **QUICK_START.md** - This file

---

For complete documentation, see README.md
