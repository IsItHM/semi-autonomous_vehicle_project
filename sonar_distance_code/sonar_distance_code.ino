#include <WiFi.h>
#include <WebSocketsServer.h>

// Wi-Fi Credentials
const char* ssid = "UIU-CAIR";
const char* password = "12345678";

// Ultrasonic Sensor Pins
#define TRIG_PIN 4
#define ECHO_PIN 2

WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected. IP address: " + WiFi.localIP().toString());

    // Start WebSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // Configure ultrasonic sensor pins
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    webSocket.loop();

    // Measure distance
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration / 58.2;

    // Send distance data to the WebSocket client
    String message = "{\"type\":\"distance\",\"value\":" + String(distance) + "}";
    webSocket.broadcastTXT(message);
    Serial.println("ESP32 is starting...");


    delay(10); // Adjust frequency as needed
}

// Handle WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = String((char *)payload).substring(0, length);
        Serial.println("Received: " + message);
    }
}
