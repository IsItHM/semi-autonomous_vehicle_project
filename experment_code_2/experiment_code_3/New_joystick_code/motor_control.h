#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

class MotorController {
private:
    // Motor A Pins (Left Motor)
    const int IN1 = 16;    // Direction 1
    const int IN2 = 17;    // Direction 2
    const int ENA = 26;    // Enable/PWM Channel

    // Motor B Pins (Right Motor)
    const int IN3 = 18;    // Direction 1
    const int IN4 = 19;    // Direction 2
    const int ENB = 27;    // Enable/PWM Channel

    // PWM Properties
    const int FREQ = 5000;
    const int RESOLUTION = 8;
    const int CHANNEL_A = 0;
    const int CHANNEL_B = 1;

public:
    void init() {
        // Configure motor control pins
        pinMode(IN1, OUTPUT);
        pinMode(IN2, OUTPUT);
        pinMode(IN3, OUTPUT);
        pinMode(IN4, OUTPUT);

        // Setup PWM for enable pins
        ledcSetup(CHANNEL_A, FREQ, RESOLUTION);
        ledcSetup(CHANNEL_B, FREQ, RESOLUTION);
        
        // Attach PWM channels to enable pins
        ledcAttachPin(ENA, CHANNEL_A);
        ledcAttachPin(ENB, CHANNEL_B);
    }

    void forward(int speed = 200) {
        // Motor A forward
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        
        // Motor B forward
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        
        // Set speed using PWM
        ledcWrite(CHANNEL_A, speed);
        ledcWrite(CHANNEL_B, speed);
    }

    void backward(int speed = 200) {
        // Motor A backward
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        
        // Motor B backward
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        
        // Set speed using PWM
        ledcWrite(CHANNEL_A, speed);
        ledcWrite(CHANNEL_B, speed);
    }

    void turnLeft(int speed = 150) {
        // Motor A backward
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        
        // Motor B forward
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        
        // Set different speeds for turning
        ledcWrite(CHANNEL_A, speed);
        ledcWrite(CHANNEL_B, speed);
    }

    void turnRight(int speed = 150) {
        // Motor A forward
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        
        // Motor B backward
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        
        // Set different speeds for turning
        ledcWrite(CHANNEL_A, speed);
        ledcWrite(CHANNEL_B, speed);
    }

    void stop() {
        // Stop both motors
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        
        // Set PWM to 0
        ledcWrite(CHANNEL_A, 0);
        ledcWrite(CHANNEL_B, 0);
    }
};

#endif