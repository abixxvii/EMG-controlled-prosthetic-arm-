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
int emgThresholdResting = 0;  // Dynamic resting threshold (calibrated)
int emgThresholdMedium = 0;   // Medium flex threshold
int emgThresholdHigh = 0;     // High flex threshold

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
  emgIndex = (emgIndex + 1) % NUM_SAMPLES;  // Circular buffer index

  // Calculate the average of the samples to smooth out noise
  int smoothed = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    smoothed += emgSamples[i];
  }
  smoothed /= NUM_SAMPLES;  // Calculate the average

  return smoothed;
}

// Main Loop
void loop() {
  // Smooth the EMG sensor data
  smoothedEmg1 = smoothEmgData();

  // Calibration phase (first 2 seconds)
  if (isCalibrating) {
    if (millis() - calibrationStartTime < calibrationTime) {
      // Collect EMG values for calibration
      int calibrationValue = smoothedEmg1;
      if (calibrationValue < 40) calibrationValue = 40;  // Avoid values less than 40 (invalid)
      if (calibrationValue > 500) calibrationValue = 500;  // Limit to max EMG value of 500

      // Calculate resting threshold dynamically
      emgThresholdResting = calibrationValue;
      emgThresholdMedium = emgThresholdResting + (emgThresholdResting * 0.5);  // 50% above resting
      emgThresholdHigh = emgThresholdResting + (emgThresholdResting * 1.2);   // 120% above resting

      // Display dynamic thresholds for debugging
      Serial.print("Resting Threshold: ");
      Serial.println(emgThresholdResting);
      Serial.print("Medium Threshold: ");
      Serial.println(emgThresholdMedium);
      Serial.print("High Threshold: ");
      Serial.println(emgThresholdHigh);
    } else {
      // End calibration after 2 seconds
      isCalibrating = false;
      Serial.println("Calibration complete. Thresholds set.");
    }
  }

  // Debugging EMG values
  Serial.print("Smoothed EMG: ");
  Serial.println(smoothedEmg1);

  // Control based on the EMG sensor value
  if (smoothedEmg1 <= emgThresholdResting) {
    // When EMG is below the resting threshold (muscle relaxed)
    resetServos();  // Keep all fingers open
  } 
  else if (smoothedEmg1 > emgThresholdResting && smoothedEmg1 <= emgThresholdMedium) {
    // When EMG is between resting and medium threshold (some flex)
    int pinkyAngle = map(smoothedEmg1, emgThresholdResting, emgThresholdMedium, 0, 90);
    int ringAngle = map(smoothedEmg1, emgThresholdResting, emgThresholdMedium, 20, 160);
    int middleAngle = map(smoothedEmg1, emgThresholdResting, emgThresholdMedium, 160, 50);
    int indexAngle = map(smoothedEmg1, emgThresholdResting, emgThresholdMedium, 0, 180);

    // Apply the mapped angles to each servo
    pinkyServo.write(pinkyAngle);    // Move pinky based on EMG
    ringServo.write(ringAngle);      // Move ring based on EMG
    middleServo.write(middleAngle);  // Move middle based on EMG
    indexServo.write(indexAngle);    // Move index (continuous servo)
  } 
  else if (smoothedEmg1 > emgThresholdMedium && smoothedEmg1 <= emgThresholdHigh) {
    // When EMG is between medium and high threshold (more flex)
    closeAllFingers();  // Close all fingers fully
  } 
  else if (smoothedEmg1 > emgThresholdHigh) {
    // When EMG is above high threshold (fully flexed)
    closeAllFingers();  // Close all fingers fully
  }

  delay(50);  // Smooth operation
}

// Function to close all fingers fully
void closeAllFingers() {
  pinkyServo.write(90);    // Fully close pinky
  ringServo.write(160);    // Fully close ring
  middleServo.write(50);   // Fully close middle
  indexServo.write(180);   // Fully close index (continuous servo)
}
