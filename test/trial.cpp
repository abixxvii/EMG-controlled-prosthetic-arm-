#include <Servo.h>

Servo fingerServos[5]; // Array for finger servos
Servo wristServo; // Servo for wrist

int emgPin = A0; // Analog pin connected to EMG sensor
int threshold = 512; // Threshold value for detecting muscle contraction

void setup() {
  Serial.begin(9600);
  
  // Attach servos to digital pins
  for (int i = 0; i < 5; i++) {
    fingerServos[i].attach(i + 2); // Pins 2-6 for fingers
  }
  wristServo.attach(7); // Pin 7 for wrist
}

void loop() {
  int emgValue = analogRead(emgPin);
  
  Serial.println(emgValue); // For debugging
  
  if (emgValue > threshold) {
    // Move servos based on EMG input
    for (int i = 0; i < 5; i++) {
      fingerServos[i].write(180); // Close fingers
    }
    wristServo.write(90); // Adjust wrist position
  } else {
    for (int i = 0; i < 5; i++) {
      fingerServos[i].write(0); // Open fingers
    }
    wristServo.write(0); // Reset wrist position
  }
  
  delay(100);
}