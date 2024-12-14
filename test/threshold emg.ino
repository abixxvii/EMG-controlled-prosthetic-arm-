#include <Servo.h>


// Define servo pins
const int SERVO_PINS[] = {3, 6, 9, 10, 11}; // Pinky, Ring, Middle, Index, Thumb
Servo pinkyServo, ringServo, middleServo, indexServo, thumbServo;


// Define EMG sensor pin
const int EMG_PIN = A0;  // Myoware EMG sensor connected to analog pin A0


// Servo range mappings
const int PINKY_MIN = 0;
const int PINKY_MAX = 90;
const int RING_MIN = 20;
const int RING_MAX = 160;
const int MIDDLE_MIN = 180;  // Upper range for middle servo
const int MIDDLE_MAX = 0;    // Lower range for middle servo
const int INDEX_MIN = 90;    // Neutral position for Parallax continuous servo (no rotation)
const int INDEX_MAX = 110;   // Flexed position (will make the Parallax servo rotate in one direction)
const int THUMB_MIN = 0;
const int THUMB_MAX = 90;


// Calibration for EMG input
const int EMG_MIN = 0;    // Minimum EMG value (no muscle activity)
const int EMG_MAX = 1023; // Maximum EMG value (full muscle contraction)


void setup() {
  // Attach servos to their respective pins
  pinkyServo.attach(SERVO_PINS[0]);   // Pin 3 (Pinky)
  ringServo.attach(SERVO_PINS[1]);    // Pin 6 (Ring)
  middleServo.attach(SERVO_PINS[4]);  // Pin 9 (Middle)
  indexServo.attach(SERVO_PINS[3]);   // Pin 10 (Index - Parallax Continuous)
  thumbServo.attach(SERVO_PINS[2]);   // Pin 11 (Thumb)


  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Myoware EMG Control Initialized");
}


void loop() {
  // Read the raw EMG value
  int emgValue = analogRead(EMG_PIN);  
  Serial.print("EMG Value: ");
  Serial.println(emgValue);


  // Map EMG value to a servo angle range
  int pinkyAngle = map(emgValue, EMG_MIN, EMG_MAX, PINKY_MIN, PINKY_MAX);
  int ringAngle = map(emgValue, EMG_MIN, EMG_MAX, RING_MIN, RING_MAX);
  int middleAngle = map(emgValue, EMG_MIN, EMG_MAX, MIDDLE_MIN, MIDDLE_MAX);  // Mapping to 180-0 range
  int indexSpeed = map(emgValue, EMG_MIN, EMG_MAX, INDEX_MIN, INDEX_MAX);  // Map to Parallax servo control (90 for neutral, 110 for flexed)
  int thumbAngle = map(emgValue, EMG_MIN, EMG_MAX, THUMB_MIN, THUMB_MAX);


  // Apply the mapped angles to the positional servos
  pinkyServo.write(pinkyAngle);
  ringServo.write(ringAngle);
  middleServo.write(middleAngle);  
  thumbServo.write(thumbAngle);


  // For the Parallax (Continuous Rotation) servo, map the value to a rotation speed
  // Neutral position for Parallax servo at 90, less than 90 for forward, greater than 90 for backward
  indexServo.write(indexSpeed);  // This will rotate the Parallax servo accordingly
 
  // Add a small delay to allow for servo movement
  delay(100);
}


