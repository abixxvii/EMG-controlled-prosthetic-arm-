#include <Servo.h>

// EMG Sensor Pin declaration
const int EMG_PIN_1 = A0;  // EMG sensor (Any analog pin)

// Servo Pins declaration
const int PINKY_PIN = 9;   // Hitec HS485HB
const int RING_PIN = 10;   // Hitec HS485HB
const int MIDDLE_PIN = 11; // Hitec HS485HB
const int INDEX_PIN = 6;   // Parallax Continuous Servo [PWM]
const int THUMB_PIN = 5;   // Hitec HS485HB (added for thumb)

// Servo Objects
Servo pinkyServo, ringServo, middleServo, indexServo, thumbServo;

// EMG Variables
const int NUM_SAMPLES = 10;  // Number of samples for smoothing (moving average)
int emgSamples[NUM_SAMPLES]; // Array to hold EMG samples
int emgIndex = 0;            // Index for storing the sample
int smoothedEmg1 = 0;        // Smoothed EMG value

// Calibration time (5 seconds)
const unsigned long calibrationTime = 5000;
unsigned long calibrationStartTime = 0;
bool isCalibrating = true;
int baselineEmg = 0;  // Store baseline EMG value

// Low-pass filter constant (adjust this for desired smoothness)
const float alpha = 0.1;  // A smaller value (e.g., 0.1) will smooth more
float smoothedEmgFiltered = 0;  // Smoothed EMG after applying low-pass filter

// Servo movement variables
int currentThumbPos = 0;  // Store thumb servo position
int targetThumbPos = 0;   // Target position based on EMG

// Setup function
void setup() {
  Serial.begin(9600);

  // Attach servos
  pinkyServo.attach(PINKY_PIN);
  ringServo.attach(RING_PIN);
  middleServo.attach(MIDDLE_PIN);
  indexServo.attach(INDEX_PIN);
  thumbServo.attach(THUMB_PIN);

  // Initialize servos to resting (relaxed) positions
  resetServos();

  // Start calibration
  calibrationStartTime = millis();
}

// Reset servos to neutral (resting) position
void resetServos() {
  pinkyServo.write(30);    // Pinky fully open (0 degrees)
  ringServo.write(20);     // Ring fully open (20 degrees)
  middleServo.write(160);  // Middle fully open (160 degrees)
  indexServo.write(0);     // Index fully open (0 degrees)
  thumbServo.write(90);    // Thumb at home position (90 degrees)
}

// Function to apply low-pass filtering to the EMG signal
float applyLowPassFilter(int emgSample) {
  smoothedEmgFiltered = alpha * emgSample + (1 - alpha) * smoothedEmgFiltered;
  return smoothedEmgFiltered;
}

// Function to smooth EMG data
int smoothEmgData() {
  int rawEmg = analogRead(EMG_PIN_1);  // Read new EMG sample
  // Apply low-pass filter and return the smoothed value
  int smoothed = (int)applyLowPassFilter(rawEmg);
  return smoothed;
}

// Main loop
void loop() {
  // Smooth the EMG sensor data using low-pass filter
  smoothedEmg1 = smoothEmgData();

  // Calibration phase (for the initial five seconds)
  if (isCalibrating) {
    if (millis() - calibrationStartTime < calibrationTime) {
      // Provide feedback messages to guide the user
      Serial.println("Relax and gently flex your muscles.");
    } else {
      isCalibrating = false;
      baselineEmg = smoothedEmg1;  // Set baseline EMG after calibration
      Serial.println("Calibration complete.");
    }
    return;
  }

  // Print the smoothed EMG reading for debugging
  Serial.print("Smoothed EMG: ");
  Serial.println(smoothedEmg1);

  // Dynamically adjust thresholds based on baseline EMG reading
  int dynamicThresholdLow = baselineEmg + 10;   // Adjust low threshold
  int dynamicThresholdHigh = baselineEmg + 50;  // Adjust high threshold

  // Print the dynamic thresholds for debugging
  Serial.print("Dynamic Threshold Low: ");
  Serial.println(dynamicThresholdLow);
  Serial.print("Dynamic Threshold High: ");
  Serial.println(dynamicThresholdHigh);

  // Apply dynamic thresholding to control the servos based on rectified EMG
  if (smoothedEmg1 > dynamicThresholdHigh) {
    closeAllFingers();  // Close all fingers when above high threshold
    Serial.println("Flexing detected: Closing fingers.");
  } else if (smoothedEmg1 > dynamicThresholdLow) {
    closeRingMiddlePinky();  // Close some fingers when above low threshold
    Serial.println("Mild flex detected: Closing ring, middle, and pinky.");
  } else {
    resetServos();  // Reset to neutral position when below low threshold
    Serial.println("Relaxing detected: Resetting to neutral position.");
  }

  delay(50);  // Small delay for smoother control
}

// Function to close all fingers fully
void closeAllFingers() {
  pinkyServo.write(100);  // Fully close pinky
  ringServo.write(160);   // Fully close ring
  middleServo.write(50);  // Fully close middle
  indexServo.write(180);  // Fully close index
}

// Function to close ring, middle, and index fingers only, open pinky
void closeRingMiddlePinky() {
  pinkyServo.write(30);   // Keep pinky open
  ringServo.write(160);   // Fully close ring
  middleServo.write(50);  // Fully close middle
  indexServo.write(180);  // Fully close index (continuous servo)
}
