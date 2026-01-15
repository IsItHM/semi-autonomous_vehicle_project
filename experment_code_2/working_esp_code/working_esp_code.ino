#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <NewPing.h>

// WiFi Configuration
#define WIFI_SSID "Redmi Note 9 Pro"
#define WIFI_PASSWORD "22222222"
#define WIFI_SSID "UIU-CAIR"
#define WIFI_PASSWORD "12345678"
#define WEBSOCKET_PORT 81

// Motor Pin Definitions
#define MOTOR_LEFT_FWD 25
#define MOTOR_LEFT_BWD 26
#define MOTOR_LEFT_ENABLE 33
#define MOTOR_RIGHT_FWD 27 
#define MOTOR_RIGHT_BWD 18
#define MOTOR_RIGHT_ENABLE 19

// Ultrasonic Sensor Pins
#define TRIG_PIN 5
#define ECHO_PIN 14

// Autonomous Navigation Parameters
#define MAX_DISTANCE 200
#define SAFE_DISTANCE 30
#define SCAN_INTERVAL 250  // ms between distance scans
#define TURN_DURATION 800  // ms to turn when obstacle detected

// Operational Modes
enum OperationMode { MANUAL, AUTONOMOUS };
OperationMode currentMode = MANUAL;

WebSocketsServer webSocket(WEBSOCKET_PORT);
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// Enhanced motor control with PWM speed control
class MotorController {
private:
    int fwdPin, bwdPin, enablePin;
    int baseSpeed = 200;
    int currentSpeed = 0;

public:
    MotorController(int fwd, int bwd, int enable) {
        fwdPin = fwd;
        bwdPin = bwd;
        enablePin = enable;
        pinMode(fwdPin, OUTPUT);
        pinMode(bwdPin, OUTPUT);
        pinMode(enablePin, OUTPUT);
    }

    void forward(int speedFactor = 1) {
        digitalWrite(fwdPin, HIGH);
        digitalWrite(bwdPin, LOW);
        currentSpeed = baseSpeed * speedFactor;
        analogWrite(enablePin, currentSpeed);
    }

    void backward(int speedFactor = 1) {
        digitalWrite(fwdPin, LOW);
        digitalWrite(bwdPin, HIGH);
        currentSpeed = baseSpeed * speedFactor;
        analogWrite(enablePin, currentSpeed);
    }

    void stop() {
        digitalWrite(fwdPin, LOW);
        digitalWrite(bwdPin, LOW);
        currentSpeed = 0;
    }
};

MotorController leftMotor(MOTOR_LEFT_FWD, MOTOR_LEFT_BWD, MOTOR_LEFT_ENABLE);
MotorController rightMotor(MOTOR_RIGHT_FWD, MOTOR_RIGHT_BWD, MOTOR_RIGHT_ENABLE);

void moveForward() {
    leftMotor.forward();
    rightMotor.forward();
}

void moveBackward() {
    leftMotor.backward();
    rightMotor.backward();
}

void turnLeft() {
    leftMotor.backward();
    rightMotor.forward();
}

void turnRight() {
    leftMotor.forward();
    rightMotor.backward();
}

void stopMotors() {
    leftMotor.stop();
    rightMotor.stop();
}

void broadcastDistance(int distance) {
    StaticJsonDocument<100> doc;
    doc["type"] = "distance";
    doc["value"] = distance;

    String jsonString;
    serializeJson(doc, jsonString);
    webSocket.broadcastTXT(jsonString);
}

void autonomousNavigation() {
    static unsigned long lastTurnTime = 0;
    int distance = sonar.ping_cm();

    if (distance > 0) {
        broadcastDistance(distance);
    }

    // Enhanced obstacle avoidance
    if (distance > 0 && distance <= SAFE_DISTANCE) {
        stopMotors();
        delay(300);
        
        if (millis() - lastTurnTime > TURN_DURATION * 2) {
            if (random(2) == 0) {
                turnLeft();
            } else {
                turnRight();
            }
            delay(TURN_DURATION);
            lastTurnTime = millis();
        }
    } else if (distance == 0 || distance > SAFE_DISTANCE) {
        moveForward();
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
            Serial.print("JSON parsing error: ");
            Serial.println(error.c_str());
            return;
        }

        const char* messageType = doc["type"];
        
        if (strcmp(messageType, "mode") == 0) {
            const char* mode = doc["mode"];
            currentMode = (strcmp(mode, "autonomous") == 0) ? AUTONOMOUS : MANUAL;
            stopMotors();
            Serial.printf("Switched to %s Mode\n", mode);
        } 
        else if (strcmp(messageType, "joystick") == 0 && currentMode == MANUAL) {
            int x = doc["x"];
            int y = doc["y"];

            const int deadZone = 20; // Threshold to ignore small joystick movements
            Serial.printf("Joystick x: %d, y: %d\n", x, y);

            // Dead zone check
            if (abs(x) < deadZone && abs(y) < deadZone) {
                stopMotors();
                return;
            }

            // Forward/Backward movement
            if (abs(y) > abs(x)) {
                if (y > 0) {
                    Serial.println("Moving Forward");
                    moveForward();
                } else {
                    Serial.println("Moving Backward");
                    moveBackward();
                }
            }
            // Turning Left/Right
            else {
                if (x > 0) {
                    Serial.println("Turning Right");
                    turnRight();
                } else {
                    Serial.println("Turning Left");
                    turnLeft();
                }
            }
        }
    }
}


void setupWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.setAutoReconnect(true);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi Connected");
    Serial.println(WiFi.localIP());
}

void setupWebSocket() {
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));
    
    setupWiFi();
    setupWebSocket();
    
    Serial.println("RC Car Control System Initialized");
}

void loop() {
    webSocket.loop();

    static unsigned long lastScanTime = 0;
    if (currentMode == AUTONOMOUS && (millis() - lastScanTime >= SCAN_INTERVAL)) {
        autonomousNavigation();
        lastScanTime = millis();
    }
}