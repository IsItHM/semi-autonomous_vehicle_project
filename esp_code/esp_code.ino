#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials
const char* ssid = "UIU-CAIR";          // Replace with your Wi-Fi SSID
const char* password = "12345678";  // Replace with your Wi-Fi Password

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

// Web Server
WebServer server(80);

// Function to measure distance using sonar sensor
long getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration/58.2;
  return distance;
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

// Web server route handlers
void handleRoot() {
  server.send(200, "text/plain", "ESP32 RC Car Ready");
}

void handleMove() {
  String command = server.arg("cmd");

  if (command == "forward") {
    moveForward();
  } else if (command == "backward") {
    moveBackward();
  } else if (command == "left") {
    turnLeft();
  } else if (command == "right") {
    turnRight();
  } else if (command == "stop") {
    stopMotors();
  }

  server.send(200, "text/plain", "OK");
}

void handleDistance() {
  long distance = getDistance();
  server.send(200, "text/plain", String(distance));
}

// Setup function
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Motor Pins
  pinMode(motor1Forward, OUTPUT);
  pinMode(motor1Backward, OUTPUT);
  pinMode(motor2Forward, OUTPUT);
  pinMode(motor2Backward, OUTPUT);
  pinMode(motor1Enable, OUTPUT);
pinMode(motor2Enable, OUTPUT);

analogWrite(motor1Enable, 255); // Full speed
analogWrite(motor2Enable, 255); 

  // Initialize Sonar Sensor Pins
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Stop all motors at startup
  stopMotors();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/distance", handleDistance);

  server.begin();
  Serial.println("Web server started");
}

// Loop function
void loop() {
  server.handleClient();
}
