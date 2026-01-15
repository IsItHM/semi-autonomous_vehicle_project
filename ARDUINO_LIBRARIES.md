# Arduino/ESP32 Library Dependencies
# Installation instructions for Semi-Autonomous Vehicle Project

## Required Arduino Libraries

Install these libraries through Arduino IDE Library Manager:
(Sketch → Include Library → Manage Libraries)

### 1. WebSocketsServer
**Author:** Markus Sattler (Links2004)
**Version:** 2.3.x or later
**Repository:** https://github.com/Links2004/arduinoWebSockets
**Purpose:** Enables WebSocket communication for real-time control and telemetry

**Installation:**
- Arduino IDE: Search "WebSocketsServer" in Library Manager
- PlatformIO: `lib_deps = links2004/WebSockets@^2.3.6`

### 2. ArduinoJson
**Author:** Benoit Blanchon
**Version:** 6.x (version 6.19.0 or later recommended)
**Repository:** https://arduinojson.org/
**Purpose:** JSON parsing for WebSocket messages and data serialization

**Installation:**
- Arduino IDE: Search "ArduinoJson" in Library Manager (install v6.x)
- PlatformIO: `lib_deps = bblanchon/ArduinoJson@^6.19.4`

### 3. NewPing
**Author:** Tim Eckel
**Version:** 1.9.x or later
**Repository:** https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home
**Purpose:** Ultrasonic sensor distance measurement with improved performance

**Installation:**
- Arduino IDE: Search "NewPing" in Library Manager
- PlatformIO: `lib_deps = teckel12/NewPing@^1.9.1`

## Built-in ESP32 Libraries

These libraries are included with the ESP32 board support package:

### 4. WiFi
**Purpose:** WiFi connectivity for network communication
**Included:** ESP32 Arduino Core

### 5. WebServer
**Purpose:** HTTP server for REST API endpoints
**Included:** ESP32 Arduino Core

### 6. BluetoothSerial
**Purpose:** Bluetooth Serial communication (for Bluetooth variants)
**Included:** ESP32 Arduino Core
**Note:** Only available on ESP32 (not ESP8266)

### 7. USBHost
**Purpose:** USB Host support for direct USB joystick connection
**Included:** Some ESP32 variants with USB OTG support
**Note:** Not all ESP32 boards support USB Host mode

## ESP32 Board Support Package

### Installation for Arduino IDE:

1. Open Arduino IDE
2. Go to File → Preferences
3. Add to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to Tools → Board → Boards Manager
5. Search for "ESP32" by Espressif Systems
6. Install "esp32" package (version 2.0.x or later recommended)

### Installation for PlatformIO:

Add to your `platformio.ini`:
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    links2004/WebSockets@^2.3.6
    bblanchon/ArduinoJson@^6.19.4
    teckel12/NewPing@^1.9.1
```

## Missing/Custom Libraries

### joystick_handler.h
**Status:** Missing from repository
**Used in:** `experment_code_2/experiment_code_3/New_joystick_code/ESP32_sketch/ESP32_sketch.ino`
**Note:** This appears to be a custom library for USB joystick handling. 
If you need this functionality, you may need to:
- Create your own implementation
- Use a third-party USB Host library compatible with your ESP32 board
- Switch to Bluetooth or WiFi-based joystick control instead

## Alternative Installation Methods

### Using Arduino CLI:
```bash
arduino-cli lib install "WebSockets"
arduino-cli lib install "ArduinoJson"
arduino-cli lib install "NewPing"
```

### Manual Installation:
1. Download library ZIP from GitHub/repository
2. Arduino IDE: Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

## Version Compatibility Notes

- **ArduinoJson 6.x vs 5.x:** This project requires ArduinoJson 6.x (major API differences from v5)
- **ESP32 Core:** Tested with ESP32 Arduino Core 2.0.x
- **WebSocketsServer:** Version 2.3.x or later recommended for stability

## Troubleshooting Library Issues

### Library not found:
- Ensure you've installed the correct library name
- Restart Arduino IDE after installation
- Check Sketch → Include Library → Manage Libraries for installed libraries

### Compilation errors:
- Verify library versions match requirements
- Update ESP32 board support to latest version
- Check for library conflicts (remove duplicate libraries)

### WebSocket connection issues:
- Ensure WebSocketsServer library is version 2.3.x or later
- Check that ArduinoJson is version 6.x (not 5.x)

## Library Documentation Links

- WebSocketsServer: https://github.com/Links2004/arduinoWebSockets#readme
- ArduinoJson: https://arduinojson.org/v6/doc/
- NewPing: https://playground.arduino.cc/Code/NewPing/
- ESP32 Arduino Core: https://docs.espressif.com/projects/arduino-esp32/
