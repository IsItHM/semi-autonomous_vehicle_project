#include <Arduino.h>
#include <USBHost.h>
#include "motor_control.h"
#include "joystick_handler.h"

USBHost usb;
JoystickHandler joystick(usb);
MotorController motors;

void setup() {
  Serial.begin(115200);
  
  // Initialize USB Host
  usb.begin();
  
  // Initialize Motor Pins
  motors.init();
}

void loop() {
  // Process USB events
  usb.Task();
  
  // Check if joystick is connected
  if (joystick.isConnected()) {
    // Read joystick axes
    int xAxis = joystick.getXAxis();
    int yAxis = joystick.getYAxis();
    
    // Convert joystick input to motor speeds
    motors.drive(xAxis, yAxis);
  }
}