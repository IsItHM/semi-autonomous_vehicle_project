#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <NewPing.h>

// WiFi Configuration
#define WIFI_SSID "Redmi Note 9 Pro"
#define WIFI_PASSWORD "22222222"
// #define WIFI_SSID "UIU-CAIR"
// #define WIFI_PASSWORD "12345678"
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
#define LEFT_MOTOR_SPEED_COMPENSATION 90

// Operational Modes
enum OperationMode { MANUAL, AUTONOMOUS };
OperationMode currentMode = MANUAL;

// Create objects
WebSocketsServer webSocket(WEBSOCKET_PORT);
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// Function prototypes
void setupPins();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void broadcastDistance(int distance);
void autonomousNavigation();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
void setupWiFi();
void setupWebSocket();

void setupPins() {
    pinMode(MOTOR_LEFT_FWD, OUTPUT);
    pinMode(MOTOR_LEFT_BWD, OUTPUT);
    pinMode(MOTOR_LEFT_ENABLE, OUTPUT);
    pinMode(MOTOR_RIGHT_FWD, OUTPUT);
    pinMode(MOTOR_RIGHT_BWD, OUTPUT);
    pinMode(MOTOR_RIGHT_ENABLE, OUTPUT);

    // Compensate for motor speed difference
    analogWrite(MOTOR_LEFT_ENABLE, 200 - LEFT_MOTOR_SPEED_COMPENSATION);
    analogWrite(MOTOR_RIGHT_ENABLE, 200);

    stopMotors();
}

void moveForward() {
    digitalWrite(MOTOR_LEFT_FWD, HIGH);
    digitalWrite(MOTOR_LEFT_BWD, LOW);
    digitalWrite(MOTOR_RIGHT_FWD, HIGH);
    digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void moveBackward() {
    digitalWrite(MOTOR_LEFT_FWD, LOW);
    digitalWrite(MOTOR_LEFT_BWD, HIGH);
    digitalWrite(MOTOR_RIGHT_FWD, LOW);
    digitalWrite(MOTOR_RIGHT_BWD, HIGH);
}

void turnLeft() {
    digitalWrite(MOTOR_LEFT_FWD, LOW);
    digitalWrite(MOTOR_LEFT_BWD, HIGH);
    digitalWrite(MOTOR_RIGHT_FWD, HIGH);
    digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void turnRight() {
    digitalWrite(MOTOR_LEFT_FWD, HIGH);
    digitalWrite(MOTOR_LEFT_BWD, LOW);
    digitalWrite(MOTOR_RIGHT_FWD, LOW);
    digitalWrite(MOTOR_RIGHT_BWD, HIGH);
}

void stopMotors() {
    digitalWrite(MOTOR_LEFT_FWD, LOW);
    digitalWrite(MOTOR_LEFT_BWD, LOW);
    digitalWrite(MOTOR_RIGHT_FWD, LOW);
    digitalWrite(MOTOR_RIGHT_BWD, LOW);
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
    int distance = sonar.ping_cm();

    // Broadcast the distance
    if (distance > 0) {
        broadcastDistance(distance);
    }

    // Obstacle avoidance logic
    if (distance > 0 && distance <= SAFE_DISTANCE) {
        stopMotors();
        delay(500);
        if (random(2) == 0) {
            turnLeft();
        } else {
            turnRight();
        }
        delay(random(1000, 1500));
    } else if (distance == 0 || distance > SAFE_DISTANCE) {
        moveForward();
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            return;
        }

        const char* messageType = doc["type"];
        if (strcmp(messageType, "mode") == 0) {
            const char* mode = doc["mode"];
            if (strcmp(mode, "autonomous") == 0) {
                currentMode = AUTONOMOUS;
                Serial.println("Switched to Autonomous Mode");
            } else if (strcmp(mode, "manual") == 0) {
                currentMode = MANUAL;
                stopMotors();
                Serial.println("Switched to Manual Mode");
            }
        } else if (strcmp(messageType, "joystick") == 0 && currentMode == MANUAL) {
            int x = doc["x"];
            int y = doc["y"];

            // Manual control logic based on joystick input
            if (abs(y) > 20) {  // Forward/Backward
                if (y > 0) {
                    moveForward();
                } else {
                    moveBackward();
                }
            } else if (abs(x) > 20) {  // Turning
                if (x > 0) {
                    turnRight();
                } else {
                    turnLeft();
                }
            } else {
                stopMotors();
            }
        }
    }
}

void setupWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");
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

    setupPins();
    setupWiFi();
    setupWebSocket();
    Serial.println("Setup complete");
}

void loop() {
    webSocket.loop();

    static unsigned long lastDistanceMeasurement = 0;
    if (millis() - lastDistanceMeasurement > 500) {
        if (currentMode == AUTONOMOUS) {
            autonomousNavigation();
        }
        lastDistanceMeasurement = millis();
    }
}