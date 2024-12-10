#include <Servo.h>

// Servo pins
const int SERVO_PINS[] = {3, 6, 9, 10, 11}; // Pinky, Ring, Middle, Index, Thumb
Servo pinkyServo, ringServo, middleServo, indexServo, thumbServo;

void setup() {
  // Attach servos to their respective pins
  pinkyServo.attach(SERVO_PINS[0]);   // Pin 3
  ringServo.attach(SERVO_PINS[1]);    // Pin 5
  middleServo.attach(SERVO_PINS[4]);  // Pin 6
  indexServo.attach(SERVO_PINS[3]);   // Pin 9
  thumbServo.attach(SERVO_PINS[2]);   // Pin 10

  Serial.begin(9600);
  Serial.println("Manual Servo Control Started");
  Serial.println("Commands: pinky 45, ring 60, thumb 30, etc. (Angle: 0°-90°)");
}

void loop() {
  // Check for user input in Serial Monitor
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read the input
    input.trim(); // Remove whitespace
    
    // Parse servo name and angle
    int spaceIndex = input.indexOf(' ');
    String servoName = input.substring(0, spaceIndex);
    int angle = input.substring(spaceIndex + 1).toInt();
    
    // Constrain angle to range 0-90 to prevent overloading servos
    angle = constrain(angle, 0, 180);

    // Control servos based on user input
    if (servoName == "pinky") {
      pinkyServo.write(angle);
      Serial.print("Pinky set to: "); Serial.print(angle); Serial.println("°");
    } else if (servoName == "ring") {
      ringServo.write(angle);
      Serial.print("Ring set to: "); Serial.print(angle); Serial.println("°");
    } else if (servoName == "middle") {
      middleServo.write(angle);
      Serial.print("Middle set to: "); Serial.print(angle); Serial.println("°");
    } else if (servoName == "index") {
      indexServo.write(angle);
      Serial.print("Index set to: "); Serial.print(angle); Serial.println("°");
    } else if (servoName == "thumb") {
      thumbServo.write(angle);
      Serial.print("Thumb set to: "); Serial.print(angle); Serial.println("°");
    } else {
      Serial.println("Invalid command! Use 'pinky 45', 'ring 60', etc.");
    }
  }
}

