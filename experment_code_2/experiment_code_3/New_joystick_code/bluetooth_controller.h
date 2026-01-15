#ifndef BLUETOOTH_CONTROLLER_H
#define BLUETOOTH_CONTROLLER_H

#include <BluetoothSerial.h>

class BluetoothController {
private:
    BluetoothSerial bluetooth;
    String deviceName;

public:
    BluetoothController(String name = "ESP32-RC-Car") : deviceName(name) {}

    void init() {
        // Initialize Bluetooth with device name
        bluetooth.begin(deviceName);
        Serial.println("Bluetooth Device is Ready to Pair");
    }

    bool isConnected() {
        return bluetooth.connected();
    }

    int available() {
        return bluetooth.available();
    }

    char readCommand() {
        return bluetooth.read();
    }

    void sendResponse(String message) {
        bluetooth.println(message);
    }
};

#endif