#include <WiFi.h>

#define TRIG_PIN 5      // Ultrasonic Sensor Trig Pin
#define ECHO_PIN 18     // Ultrasonic Sensor Echo Pin
#define GAS_SENSOR_PIN 34 // MQ-2 Gas Sensor Analog Pin
#define GAS_RELAY_PIN 23  // Solenoid Gas Valve Relay
#define POWER_RELAY_PIN 22 // Main Power Relay

long duration;
float distanceCm;
int gasValue;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GAS_RELAY_PIN, OUTPUT);
  pinMode(POWER_RELAY_PIN, OUTPUT);

  // Default state: Power & Gas active
  digitalWrite(GAS_RELAY_PIN, LOW);   // Relay Active (Gas Open)
  digitalWrite(POWER_RELAY_PIN, LOW); // Relay Active (Power ON)
}

void loop() {
  // 1. Measure Water Level via Ultrasonic Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * 0.034 / 2;

  // 2. Measure Gas Leakage
  gasValue = analogRead(GAS_SENSOR_PIN);

  Serial.print("Water Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm | Gas Level: ");
  Serial.println(gasValue);

  // 3. Automated Safety Cutoff Logic
  if (distanceCm < 15.0) { // Flood water too high
    digitalWrite(POWER_RELAY_PIN, HIGH); // Cutoff main power grid
    Serial.println("ALERT: High Water Level! Main Power Cutoff Triggered.");
  }

  if (gasValue > 2000) { // Gas leak detected
    digitalWrite(GAS_RELAY_PIN, HIGH); // Cutoff gas solenoid valve
    Serial.println("ALERT: Gas Leakage Detected! Gas Supply Valve Closed.");
  }

  delay(2000);
}
