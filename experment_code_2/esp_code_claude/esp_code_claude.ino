#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Wi-Fi Credentials
const char* ssid = "Jayid wifi";
const char* password = "uiu+ds23";

// Motor Driver Pins
const int motor1Forward = 25;
const int motor1Backward = 26;
const int motor2Forward = 27;
const int motor2Backward = 14;
const int motor1Enable = 32; // ENA for Motor 1
const int motor2Enable = 33; // ENB for Motor 2

// Sonar Sensor Pins
const int triggerPin = 5;
const int echoPin = 18;

// Web Server and WebSocket
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Control Mode
String currentMode = "manual";

// Function to measure distance using sonar sensor
long getDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH);
    return duration / 58.2;
}

// Motor control functions
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

// WebSocket Event Handler
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_TEXT: {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, (char*)payload);
            if (error) {
                Serial.println("WebSocket JSON error");
                return;
            }

            const char* mode = doc["mode"];
            if (strcmp(mode, "manual") == 0 || strcmp(mode, "autonomous") == 0) {
                currentMode = mode;
                Serial.printf("Mode changed to: %s\n", currentMode.c_str());
            }
            break;
        }
    }
}

// Autonomous mode logic

void autonomousMode() {
    long distance = getDistance();
    
    // Debug print
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    StaticJsonDocument<200> doc;
    doc["type"] = "distance";
    doc["value"] = distance;

    String jsonString;
    serializeJson(doc, jsonString);
    
    // Debug print
    Serial.print("Sending JSON: ");
    Serial.println(jsonString);
    
    webSocket.broadcastTXT(jsonString);
}

void setup() {
    Serial.begin(115200);

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

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    server.on("/", []() {
        server.send(200, "text/plain", "RC Car Ready");
    });

    server.on("/move", []() {
        if (currentMode == "manual") {
            String cmd = server.arg("cmd");
            if (cmd == "forward") moveForward();
            else if (cmd == "backward") moveBackward();
            else if (cmd == "left") turnLeft();
            else if (cmd == "right") turnRight();
            else if (cmd == "stop") stopMotors();
        }
        server.send(200, "text/plain", "OK");
    });

    server.begin();
}

void loop() {
    webSocket.loop();
    server.handleClient();

    if (currentMode == "autonomous") {
        autonomousMode();
    }
}
