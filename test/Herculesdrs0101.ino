#include <Herkulex.h>

int n = 0xfe; // Motor ID - verify your motor ID!

void setup() {
  delay(2000);  // A delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  
  // Start communication with the Herkulex motor
  Herkulex.beginSerial1(115200); // Open serial port 1
  
  // Reboot the motor to ensure it's ready
  Herkulex.reboot(n); 
  delay(500);
  
  // Initialize the motor
  Herkulex.initialize();
  delay(500); // Give time for initialization
  
  // Start motor movement test
  Serial.println("Motor Test Starting...");
}

void loop() {
  // Move motor to 0 degrees and check
  Serial.println("Moving to 0 degrees");
  Herkulex.moveOneAngle(n, 0, 500, LED_BLUE); // Move to 0 degrees with a speed of 500 ms
  delay(1500); // Wait for the motor to reach position
  Serial.print("Current Angle: ");
  Serial.println(Herkulex.getAngle(n));  // Get and print the current angle 
  
  delay(1000);  // Wait a second before next move
  
  // Move motor to 15 degrees and check
  Serial.println("Moving to 15 degrees");
  Herkulex.moveOneAngle(n, 15, 500, LED_BLUE); // Move to 15 degrees
  delay(1500); // Wait for the motor to reach position
  Serial.print("Current Angle: ");
  Serial.println(Herkulex.getAngle(n));  // Get and print the current angle
  
  delay(1000);  // Wait a second before next loop
}
