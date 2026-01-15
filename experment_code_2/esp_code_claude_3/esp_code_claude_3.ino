#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "Jayid wifi";
const char* password = "uiu+ds23";

// Motor Driver Pins
const int motor1Forward = 25;
const int motor1Backward = 26;
const int motor2Forward = 27;
const int motor2Backward = 14;
const int motor1Enable = 32;
const int motor2Enable = 33;

// Sonar Sensor Pins
const int triggerPin = 5;
const int echoPin = 18;

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String currentMode = "manual";

// Distance Measurement Function
// Improve distance measurement reliability
long getDistance() {
    // Take multiple readings for accuracy
    long totalDistance = 0;
    int numReadings = 5;
    
    for (int i = 0; i < numReadings; i++) {
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        
        long duration = pulseIn(echoPin, HIGH, 20000);  // Added timeout
        totalDistance += duration / 58.2;
        delay(10);
    }
    
    long averageDistance = totalDistance / numReadings;
    return constrain(averageDistance, 0, 400);
}

// Motor Control Functions
void moveForward() {
    digitalWrite(motor1Forward, HIGH);
    digitalWrite(motor1Backward, LOW);
    digitalWrite(motor2Forward, HIGH);
    digitalWrite(motor2Backward, LOW);
}

void moveBackward() {
    digitalWrite(motor1Forward, LOW);
    digitalWrite(motor1Backward, HIGH);
    digitalWrite(motor2Forward, LOW);
    digitalWrite(motor2Backward, HIGH);
}

void turnLeft() {
    digitalWrite(motor1Forward, HIGH);
    digitalWrite(motor1Backward, LOW);
    digitalWrite(motor2Forward, LOW);
    digitalWrite(motor2Backward, HIGH);
}

void turnRight() {
    digitalWrite(motor1Forward, LOW);
    digitalWrite(motor1Backward, HIGH);
    digitalWrite(motor2Forward, HIGH);
    digitalWrite(motor2Backward, LOW);
}

void stopMotors() {
    digitalWrite(motor1Forward, LOW);
    digitalWrite(motor1Backward, LOW);
    digitalWrite(motor2Forward, LOW);
    digitalWrite(motor2Backward, LOW);
}

// Enhanced WebSocket Event Handler
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_TEXT: {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, (char*)payload);
            
            if (error) {
                Serial.println("WebSocket JSON parsing error");
                return;
            }

            if (doc.containsKey("type")) {
                const char* type = doc["type"];
                
                // Mode Change
                if (strcmp(type, "mode") == 0) {
                    const char* mode = doc["mode"];
                    currentMode = mode;
                    Serial.printf("Mode changed to: %s\n", currentMode.c_str());
                }
                
                // Joystick Control
                if (currentMode == "manual" && strcmp(type, "joystick") == 0) {
                    int x = doc["x"];
                    int y = doc["y"];
                    
                    // Directional control based on joystick input
                    if (y > 50) moveForward();
                    else if (y < -50) moveBackward();
                    
                    if (x > 50) turnRight();
                    else if (x < -50) turnLeft();
                    
                    if (abs(x) < 20 && abs(y) < 20) stopMotors();
                }
            }
            break;
        }
    }
}

// Advanced Autonomous Mode
// Enhanced Autonomous Navigation Algorithm
void autonomousMode() {
    long distance = getDistance();
    
    // Debug: Print detailed navigation information
    Serial.print("Autonomous Mode - Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // Broadcast distance for dashboard
    StaticJsonDocument<200> doc;
    doc["type"] = "distance";
    doc["value"] = distance;
    String jsonString;
    serializeJson(doc, jsonString);
    webSocket.broadcastTXT(jsonString);
    
    // More Intelligent Obstacle Avoidance
    if (distance < 30) {  // Critical proximity threshold
        stopMotors();
        delay(500);
        
        // Random directional change with angle variation
        int turnDirection = random(2);
        int turnDuration = random(800, 1500);  // Varied turn duration
        
        if (turnDirection == 0) {
            turnLeft();
            Serial.println("Turning Left to Avoid Obstacle");
        } else {
            turnRight();
            Serial.println("Turning Right to Avoid Obstacle");
        }
        
        delay(turnDuration);
        stopMotors();
    } 
    else if (distance < 50) {  // Cautious approach zone
        // Gradual speed reduction or careful movement
        moveForward();
        Serial.println("Cautious Forward Movement");
    } 
    else {
        // Open path - confident forward movement
        moveForward();
        Serial.println("Clear Path - Moving Forward");
    }
    
    // Add small delay to prevent overwhelming processor
    delay(100);
}

void setup() {
    Serial.begin(115200);
    Serial.println("RC Car Initialization");
    Serial.printf("Motor Pins: F1=%d, B1=%d, F2=%d, B2=%d\n", 
                  motor1Forward, motor1Backward, 
                  motor2Forward, motor2Backward);
    Serial.printf("Sonar Pins: Trigger=%d, Echo=%d\n", 
                  triggerPin, echoPin);

    // Pin Setup
    pinMode(motor1Forward, OUTPUT);
    pinMode(motor1Backward, OUTPUT);
    pinMode(motor2Forward, OUTPUT);
    pinMode(motor2Backward, OUTPUT);
    pinMode(motor1Enable, OUTPUT);
    pinMode(motor2Enable, OUTPUT);

    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(motor1Enable, HIGH);
    digitalWrite(motor2Enable, HIGH);

    stopMotors();
    randomSeed(analogRead(0));

    // WiFi Connection
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // WebSocket and Server Setup
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    server.begin();
}

void loop() {
    webSocket.loop();
    server.handleClient();

    if (currentMode == "autonomous") {
        autonomousMode();
    }
}