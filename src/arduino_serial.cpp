#include <Servo.h>

// Servo objects
Servo pinkyServo;
Servo ringServo;
Servo middleServo;
Servo indexServo;

// Pin assignments for servos
const int pinkyPin = 9;
const int ringPin = 10;
const int middlePin = 11;
const int indexPin = 6;

// Pin for MyoWare sensor
const int muscleSensorPin = A0;

void setup() {
  // Attach servos to their respective pins
  pinkyServo.attach(pinkyPin);
  ringServo.attach(ringPin);
  middleServo.attach(middlePin);
  indexServo.attach(indexPin);

  // Initialize serial communication at the same baud rate as Python script
  Serial.begin(9600);
}

void loop() {
  // Read the MyoWare sensor value (EMG value)
  int muscleValue = analogRead(muscleSensorPin);

  // Send the muscle value to Python via serial communication
  Serial.println(muscleValue);

  // Debugging: Print the muscle value to the Serial Monitor
  Serial.print("Muscle Value: ");
  Serial.println(muscleValue);

  // Timeout setup for serial communication
  unsigned long timeout = 500;  // Timeout of 500ms
  unsigned long startMillis = millis();
  String input = "";
  
  // Wait for data from Python or timeout
  while (Serial.available() == 0) {
    if (millis() - startMillis > timeout) {
      Serial.println("Timeout waiting for data from Python");
      return; // Exit if no data received within the timeout period
    }
  }
  
  // Read the incoming data as a string
  input = Serial.readStringUntil('\n'); // Read until newline character

  // Debug: Print the received data to Serial Monitor
  Serial.println("Received: " + input);

  // Parse the angles from the input string
  int pinkyAngle = getAngle(input, 0);
  int ringAngle = getAngle(input, 1);
  int middleAngle = getAngle(input, 2);
  int indexAngle = getAngle(input, 3);

  // Debug: Print parsed angles to Serial Monitor
  Serial.print("Pinky: ");
  Serial.print(pinkyAngle);
  Serial.print(", Ring: ");
  Serial.print(ringAngle);
  Serial.print(", Middle: ");
  Serial.print(middleAngle);
  Serial.print(", Index: ");
  Serial.println(indexAngle);

  // Move each servo to its respective angle
  pinkyServo.write(constrain(pinkyAngle, 0, 90));   // Pinky: constrained to 0-90 degrees
  ringServo.write(constrain(ringAngle, 20, 160));   // Ring: constrained to 20-160 degrees
  middleServo.write(constrain(middleAngle, 50, 160)); // Middle: constrained to 50-160 degrees
  indexServo.write(constrain(indexAngle, 0, 180));   // Index: constrained to 0-180 degrees

  // Small delay for servo movement stability
  delay(100);
}

// Helper function to extract angles from a comma-separated string
int getAngle(String input, int index) {
  int startIndex = 0;

  for (int i = 0; i < index; i++) {
    startIndex = input.indexOf(',', startIndex) + 1; // Find next comma
    if (startIndex == -1) return -1;                // Return -1 if no more commas are found
  }

  int endIndex = input.indexOf(',', startIndex);     // Find the next comma or end of string
  if (endIndex == -1) endIndex = input.length();     // If no more commas, take the rest of the string

  String angleStr = input.substring(startIndex, endIndex); // Extract substring for angle

  return angleStr.toInt();                           // Convert substring to integer
}
