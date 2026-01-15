// #include <WiFi.h>
// #include <WebSocketsServer.h>
// #include <ArduinoJson.h>

// // WiFi Configuration
// #define WIFI_SSID "Redmi Note 9 Pro"
// #define WIFI_PASSWORD "22222222"
// // #define WIFI_SSID "UIU-CAIR"
// // #define WIFI_PASSWORD "12345678"
// #define WEBSOCKET_PORT 82

// // Motor Driver Configuration
// struct MotorConfig {
//     int forward;
//     int backward;
//     int enable;
// };

// const MotorConfig LEFT_MOTOR = {25, 26, 32};
// const MotorConfig RIGHT_MOTOR = {27, 14, 33};

// // Sonar Sensor Pin Configuration
// const int TRIGGER_PIN = 5;
// const int ECHO_PIN = 18;

// // Operational Modes
// enum OperationMode { MANUAL, AUTONOMOUS };
// OperationMode currentMode = MANUAL;

// WebSocketsServer webSocket(WEBSOCKET_PORT);

// // Measure distance using the ultrasonic sensor
// long measureDistance() {
//     digitalWrite(TRIGGER_PIN, LOW);
//     delayMicroseconds(2);
//     digitalWrite(TRIGGER_PIN, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(TRIGGER_PIN, LOW);

//     long duration = pulseIn(ECHO_PIN, HIGH, 20000);
//     return (duration / 58.2);  // Convert duration to centimeters
// }

// // Control motor behavior
// void controlMotor(const MotorConfig& motor, bool forward, bool backward) {
//     digitalWrite(motor.forward, forward ? HIGH : LOW);
//     digitalWrite(motor.backward, backward ? HIGH : LOW);
// }

// // Movement functions
// void moveForward() {
//     controlMotor(LEFT_MOTOR, true, false);
//     controlMotor(RIGHT_MOTOR, true, false);
// }

// void moveBackward() {
//     controlMotor(LEFT_MOTOR, false, true);
//     controlMotor(RIGHT_MOTOR, false, true);
// }

// void turnLeft() {
//     controlMotor(LEFT_MOTOR, false, true);
//     controlMotor(RIGHT_MOTOR, true, false);
// }

// void turnRight() {
//     controlMotor(LEFT_MOTOR, true, false);
//     controlMotor(RIGHT_MOTOR, false, true);
// }

// void stopMotors() {
//     controlMotor(LEFT_MOTOR, false, false);
//     controlMotor(RIGHT_MOTOR, false, false);
// }

// // Broadcast distance measurement to the client
// void broadcastDistance(long distance) {
//     StaticJsonDocument<100> doc;
//     doc["type"] = "distance";
//     doc["value"] = distance;

//     String jsonString;
//     serializeJson(doc, jsonString);
//     webSocket.broadcastTXT(jsonString);
// }

// // Autonomous navigation logic
// void autonomousNavigation() {
//     long distance = measureDistance();
//     broadcastDistance(distance);

//     if (distance < 30) {  // Obstacle close
//         stopMotors();
//         delay(200);

//         // Random avoidance strategy
//         if (random(2)) {
//             turnLeft();
//         } else {
//             turnRight();
//         }
//         delay(random(500, 1500));
//     } else if (distance < 50) {
//         moveForward();
//         delay(50);  // Move cautiously
//     } else {
//         moveForward();
//     }
// }

// // WebSocket event handler
// void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
//     if (type == WStype_TEXT) {
//         StaticJsonDocument<200> doc;
//         DeserializationError error = deserializeJson(doc, payload);

//         if (error) {
//             Serial.print("JSON parsing failed: ");
//             Serial.println(error.c_str());
//             return;
//         }

//         const char* messageType = doc["type"];
//         if (strcmp(messageType, "mode") == 0) {
//             const char* mode = doc["mode"];
//             if (strcmp(mode, "autonomous") == 0) {
//                 currentMode = AUTONOMOUS;
//                 Serial.println("Switched to Autonomous Mode");
//             } else if (strcmp(mode, "manual") == 0) {
//                 currentMode = MANUAL;
//                 stopMotors();
//                 Serial.println("Switched to Manual Mode");
//             }
//         } else if (strcmp(messageType, "joystick") == 0 && currentMode == MANUAL) {
//             int x = doc["x"];
//             int y = doc["y"];

//             // Manual control logic based on joystick input
//             if (abs(y) > 20) {  // Forward/Backward
//                 if (y > 0) {
//                     moveForward();
//                 } else {
//                     moveBackward();
//                 }
//             } else if (abs(x) > 20) {  // Turning
//                 if (x > 0) {
//                     turnRight();
//                 } else {
//                     turnLeft();
//                 }
//             } else {
//                 stopMotors();
//             }
//         }
//     }
// }

// // Initialize pins and motor driver setup
// void setupPins() {
//     pinMode(LEFT_MOTOR.forward, OUTPUT);
//     pinMode(LEFT_MOTOR.backward, OUTPUT);
//     pinMode(LEFT_MOTOR.enable, OUTPUT);

//     pinMode(RIGHT_MOTOR.forward, OUTPUT);
//     pinMode(RIGHT_MOTOR.backward, OUTPUT);
//     pinMode(RIGHT_MOTOR.enable, OUTPUT);

//     pinMode(TRIGGER_PIN, OUTPUT);
//     pinMode(ECHO_PIN, INPUT);

//     digitalWrite(LEFT_MOTOR.enable, HIGH);
//     digitalWrite(RIGHT_MOTOR.enable, HIGH);

//     stopMotors();
// }

// // Setup WiFi connection
// void setupWiFi() {
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to WiFi...");
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("\nWiFi Connected");
//     Serial.println(WiFi.localIP());
// }

// // Setup WebSocket server
// void setupWebSocket() {
//     webSocket.begin();
//     webSocket.onEvent(webSocketEvent);
// }

// void setup() {
//     Serial.begin(115200);
//     randomSeed(analogRead(0));  // Initialize random seed

//     setupPins();
//     setupWiFi();
//     setupWebSocket();
//     Serial.println("Setup complete");
// }

// void loop() {
//     webSocket.loop();

//     // Periodic distance measurement
//     static unsigned long lastDistanceMeasurement = 0;
//     if (millis() - lastDistanceMeasurement > 500) {  // Measure every 500ms
//         long distance = measureDistance();
//         broadcastDistance(distance);
//         lastDistanceMeasurement = millis();
//     }

//     // Handle autonomous navigation
//     if (currentMode == AUTONOMOUS) {
//         autonomousNavigation();
//     }
// }

// #include <WiFi.h>
// #include <WebSocketsServer.h>
// #include <ArduinoJson.h>
// #include <NewPing.h>

// // WiFi Configuration
// #define WIFI_SSID "Redmi Note 9 Pro"
// #define WIFI_PASSWORD "22222222"
// #define WEBSOCKET_PORT 82

// // Motor Pin Definitions
// #define MOTOR_LEFT_FWD 25
// #define MOTOR_LEFT_BWD 26
// #define MOTOR_LEFT_ENABLE 33
// #define MOTOR_RIGHT_FWD 27 
// #define MOTOR_RIGHT_BWD 18
// #define MOTOR_RIGHT_ENABLE 19

// // Ultrasonic Sensor Pins
// #define TRIG_PIN 5
// #define ECHO_PIN 14

// // Autonomous Navigation Parameters
// #define MAX_DISTANCE 200
// #define SAFE_DISTANCE 30
// #define LEFT_MOTOR_SPEED_COMPENSATION 90

// // Operational Modes
// enum OperationMode { MANUAL, AUTONOMOUS };
// OperationMode currentMode = MANUAL;

// // Create objects
// WebSocketsServer webSocket(WEBSOCKET_PORT);
// NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// void setupPins() {
//   // Motor Pins
//   pinMode(MOTOR_LEFT_FWD, OUTPUT);
//   pinMode(MOTOR_LEFT_BWD, OUTPUT);
//   pinMode(MOTOR_LEFT_ENABLE, OUTPUT);
//   pinMode(MOTOR_RIGHT_FWD, OUTPUT);
//   pinMode(MOTOR_RIGHT_BWD, OUTPUT);
//   pinMode(MOTOR_RIGHT_ENABLE, OUTPUT);

//   // Compensate for motor speed difference
//   analogWrite(MOTOR_LEFT_ENABLE, 200 - LEFT_MOTOR_SPEED_COMPENSATION);
//   analogWrite(MOTOR_RIGHT_ENABLE, 200);

//   // Initially stop motors
//   stopMotors();
// }

// void moveForward() {
//   digitalWrite(MOTOR_LEFT_FWD, HIGH);
//   digitalWrite(MOTOR_LEFT_BWD, LOW);
//   digitalWrite(MOTOR_RIGHT_FWD, HIGH);
//   digitalWrite(MOTOR_RIGHT_BWD, LOW);
// }

// void moveBackward() {
//   digitalWrite(MOTOR_LEFT_FWD, LOW);
//   digitalWrite(MOTOR_LEFT_BWD, HIGH);
//   digitalWrite(MOTOR_RIGHT_FWD, LOW);
//   digitalWrite(MOTOR_RIGHT_BWD, HIGH);
// }

// void turnLeft() {
//   digitalWrite(MOTOR_LEFT_FWD, LOW);
//   digitalWrite(MOTOR_LEFT_BWD, HIGH);
//   digitalWrite(MOTOR_RIGHT_FWD, HIGH);
//   digitalWrite(MOTOR_RIGHT_BWD, LOW);
// }

// void turnRight() {
//   digitalWrite(MOTOR_LEFT_FWD, HIGH);
//   digitalWrite(MOTOR_LEFT_BWD, LOW);
//   digitalWrite(MOTOR_RIGHT_FWD, LOW);
//   digitalWrite(MOTOR_RIGHT_BWD, HIGH);
// }

// void stopMotors() {
//   digitalWrite(MOTOR_LEFT_FWD, LOW);
//   digitalWrite(MOTOR_LEFT_BWD, LOW);
//   digitalWrite(MOTOR_RIGHT_FWD, LOW);
//   digitalWrite(MOTOR_RIGHT_BWD, LOW);
// }

// // Broadcast distance measurement to the client
// void broadcastDistance(int distance) {
//     StaticJsonDocument<100> doc;
//     doc["type"] = "distance";
//     doc["value"] = distance;

//     String jsonString;
//     serializeJson(doc, jsonString);
//     webSocket.broadcastTXT(jsonString);
// }

// // Autonomous navigation logic
// void autonomousNavigation() {
//     int distance = sonar.ping_cm();
    
//     // Broadcast the distance
//     if (distance > 0) {
//         broadcastDistance(distance);
//     }

//     // Obstacle avoidance logic
//     if (distance > 0 && distance <= SAFE_DISTANCE) {
//         stopMotors();
//         delay(500);
        
//         // Random turn to avoid obstacle
//         if (random(2) == 0) {
//             turnLeft();
//         } else {
//             turnRight();
//         }
        
//         delay(random(1000, 1500));
//     } else if (distance == 0 || distance > SAFE_DISTANCE) {
//         moveForward();
//     }
// }

// // WebSocket event handler
// void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
//     if (type == WStype_TEXT) {
//         StaticJsonDocument<200> doc;
//         DeserializationError error = deserializeJson(doc, payload);

//         if (error) {
//             Serial.print("JSON parsing failed: ");
//             Serial.println(error.c_str());
//             return;
//         }

//         const char* messageType = doc["type"];
//         if (strcmp(messageType, "mode") == 0) {
//             const char* mode = doc["mode"];
//             if (strcmp(mode, "autonomous") == 0) {
//                 currentMode = AUTONOMOUS;
//                 Serial.println("Switched to Autonomous Mode");
//             } else if (strcmp(mode, "manual") == 0) {
//                 currentMode = MANUAL;
//                 stopMotors();
//                 Serial.println("Switched to Manual Mode");
//             }
//         } else if (strcmp(messageType, "joystick") == 0 && currentMode == MANUAL) {
//             int x = doc["x"];
//             int y = doc["y"];

//             // Manual control logic based on joystick input
//             if (abs(y) > 20) {  // Forward/Backward
//                 if (y > 0) {
//                     moveForward();
//                 } else {
//                     moveBackward();
//                 }
//             } else if (abs(x) > 20) {  // Turning
//                 if (x > 0) {
//                     turnRight();
//                 } else {
//                     turnLeft();
//                 }
//             } else {
//                 stopMotors();
//             }
//         }
//     }
// }

// // Setup WiFi connection
// void setupWiFi() {
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to WiFi...");
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("\nWiFi Connected");
//     Serial.println(WiFi.localIP());
// }

// // Setup WebSocket server
// void setupWebSocket() {
//     webSocket.begin();
//     webSocket.onEvent(webSocketEvent);
// }

// void setup() {
//     Serial.begin(115200);
//     randomSeed(analogRead(0));  // Initialize random seed

//     setupPins();
//     setupWiFi();
//     setupWebSocket();
//     Serial.println("Setup complete");
// }

// void loop() {
//     webSocket.loop();

//     // Periodic distance measurement and autonomous navigation
//     static unsigned long lastDistanceMeasurement = 0;
//     if (millis() - lastDistanceMeasurement > 500) {  // Measure every 500ms
//         if (currentMode == AUTONOMOUS) {
//             autonomousNavigation();
//         }
//         lastDistanceMeasurement = millis();
//     }
// }