#include <Servo.h>

// EMG Sensor Pin
const int EMG_PIN_1 = A0;  // EMG sensor (only using A0)

// Servo Pins
const int PINKY_PIN = 3;
const int RING_PIN = 5;
const int MIDDLE_PIN = 6;
const int INDEX_PIN = 9;  // Continuous Servo

// Servo Objects
Servo pinkyServo, ringServo, middleServo, indexServo;

// EMG Variables
const int NUM_SAMPLES = 10;  // Number of samples for smoothing
int emgSamples[NUM_SAMPLES];  // Array to hold EMG samples
int emgIndex = 0;             // Index for storing the sample
int smoothedEmg1 = 0;         // Smoothed EMG value

// Calibration time (2 seconds)
const unsigned long calibrationTime = 2000;
unsigned long calibrationStartTime = 0;
bool isCalibrating = true;

// Setup Function
void setup() {
  Serial.begin(9600);

  // Attach servos
  pinkyServo.attach(PINKY_PIN);
  ringServo.attach(RING_PIN);
  middleServo.attach(MIDDLE_PIN);
  indexServo.attach(INDEX_PIN);

  // Initialize servos to resting (neutral) positions
  resetServos();

  // Start calibration
  calibrationStartTime = millis();
}

// Reset Servos to Neutral (Resting) Position
void resetServos() {
  pinkyServo.write(0);      // Pinky fully open (0 degrees)
  ringServo.write(20);      // Ring fully open (20 degrees)
  middleServo.write(160);   // Middle fully open (160 degrees)
  indexServo.write(0);      // Index fully open (0 degrees)
}

// Function to smooth EMG data
int smoothEmgData() {
  emgSamples[emgIndex] = analogRead(EMG_PIN_1);  // Read new EMG sample
  emgIndex = (emgIndex + 1) % NUM_SAMPLES;       // Circular buffer index

  // Calculate the average of the samples to smooth out noise
  int smoothed = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    smoothed += emgSamples[i];
  }
  smoothed /= NUM_SAMPLES;                       // Calculate the average

  return smoothed;
}

// Main Loop
void loop() {
  // Smooth the EMG sensor data
  smoothedEmg1 = smoothEmgData();

  // Calibration phase (first two seconds)
  if (isCalibrating) {
    if (millis() - calibrationStartTime < calibrationTime) {
      Serial.println("Calibrating...");
    } else {
      isCalibrating = false;
      Serial.println("Calibration complete.");
    }
    return;
  }

  Serial.print("Smoothed EMG: ");
  Serial.println(smoothedEmg1);

  
if(smoothedEmg1 >260){
closeAllFingers();           // Close all fingers for values greater than 260
}else if(smoothedEmg1 >180){
closeRingMiddlePinky();      // Close ring, middle, and pinky, open index for values >180
}else if(smoothedEmg1 >120){
closeRingMiddle();           // Close ring and middle, open others for values >120
}else if(smoothedEmg1 >70){
closeRingMiddleIndex();      // Close ring, middle, and index for values >70
}else{
resetServos();               // Reset to open position for values <=70
}

delay(80); 
}

// Function to close all fingers fully
void closeAllFingers() {
pinkyServo.write(90);    // Fully close pinky
ringServo.write(160);    // Fully close ring
middleServo.write(50);   // Fully close middle
indexServo.write(180);   // Fully close index (continuous servo)
}

// Function to close ring, middle, and index fingers only, open pinky
void closeRingMiddleIndex() {
pinkyServo.write(0);     // Keep pinky open
ringServo.write(160);    // Fully close ring
middleServo.write(50);   // Fully close middle
indexServo.write(180);   // Fully close index (continuous servo)
}

// Function to close ring and middle fingers only, open others
void closeRingMiddle() {
pinkyServo.write(0);     // Open pinky
ringServo.write(160);    // Fully close ring
middleServo.write(50);   // Fully close middle
indexServo.write(0);     // Open index or neutral position
}

// Function to close ring, middle, and pinky fingers only, open index
void closeRingMiddlePinky() {
pinkyServo.write(90);    // Fully close pinky
ringServo.write(160);    // Fully close ring
middleServo.write(50);   // Fully close middle
indexServo.write(0);     // Open index or neutral position
}
