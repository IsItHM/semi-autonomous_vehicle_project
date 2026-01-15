#include <NewPing.h>

#define TRIG_PIN 12
#define ECHO_PIN 14

#define MOTOR_LEFT_FWD 25
#define MOTOR_LEFT_BWD 26
#define MOTOR_LEFT_ENABLE 33
#define MOTOR_RIGHT_FWD 27 
#define MOTOR_RIGHT_BWD 18
#define MOTOR_RIGHT_ENABLE 19

#define MAX_DISTANCE 200
#define SAFE_DISTANCE 30

// Adjust this value to slow down the faster motor
#define LEFT_MOTOR_SPEED_COMPENSATION 100

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  pinMode(MOTOR_LEFT_FWD, OUTPUT);
  pinMode(MOTOR_LEFT_BWD, OUTPUT);
  pinMode(MOTOR_LEFT_ENABLE, OUTPUT);
  pinMode(MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(MOTOR_RIGHT_BWD, OUTPUT);
  pinMode(MOTOR_RIGHT_ENABLE, OUTPUT);

  // Compensate for motor speed difference
  analogWrite(MOTOR_LEFT_ENABLE, 200 - LEFT_MOTOR_SPEED_COMPENSATION);
  analogWrite(MOTOR_RIGHT_ENABLE, 200);

  Serial.begin(115200);
}

void loop() {
  int distance = sonar.ping_cm();
  
  if (distance > 0 && distance <= SAFE_DISTANCE) {
    stopMotors();
    delay(500);
    
    if (random(2) == 0) {
      turnLeft();
    } else {
      turnRight();
    }
    
    delay(1000);
  } else {
    moveForward();
  }
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FWD, HIGH);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, HIGH);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_FWD, LOW);
  digitalWrite(MOTOR_LEFT_BWD, LOW);
  digitalWrite(MOTOR_RIGHT_FWD, LOW);
  digitalWrite(MOTOR_RIGHT_BWD, LOW);
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