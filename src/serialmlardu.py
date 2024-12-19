import serial
import time
import joblib
import numpy as np
import pandas as pd
import xgboost as xgb

# Load trained XGBoost model and label encoder
model = joblib.load('models/xgb_model.pkl')
label_encoder = joblib.load('models/label_encoder.pkl')

# Open serial connection to Arduino (replace 'COM5' with your port)
arduino = serial.Serial('COM5', 9600)
time.sleep(2)  # Wait for Arduino to initialize

def control_servos(emg_value):
    try:
        # Create a DataFrame matching training data structure with 'Time Window' as 0
        input_data = pd.DataFrame({'Time Window': [0], 'EMG Value': [emg_value]})
        
        # Debug: Print input data for verification
        print(f"Input Data:\n{input_data[['Time Window', 'EMG Value']]}")
        
        # Get prediction from model directly using the DataFrame
        prediction = model.predict(input_data[['Time Window', 'EMG Value']])[0]
        predicted_label = label_encoder.inverse_transform([int(prediction)])[0]
        
        # Debug: Log predicted label to check if it's correct
        print(f"Predicted label: {predicted_label}")
        
        # Define servo angles based on predicted movement label
        if predicted_label == 'Pinky Flex':
            pinky_angle, ring_angle, middle_angle, index_angle = (90, 20, 160, 90)
        elif predicted_label == 'Ring Flex':
            pinky_angle, ring_angle, middle_angle, index_angle = (45, 160, 120, 90)
        elif predicted_label == 'Middle Flex':
            pinky_angle, ring_angle, middle_angle, index_angle = (60, 80, 50, 90)
        elif predicted_label == 'Index Flex':
            pinky_angle, ring_angle, middle_angle, index_angle = (70, 30, 90, 180)
        else:
            print(f"Unknown movement detected. EMG Value: {emg_value}, Predicted label: {predicted_label}")
            return

        # Send servo angles as a comma-separated string back to Arduino
        servo_command = f"{pinky_angle},{ring_angle},{middle_angle},{index_angle}\n"
        
        # Debug: Print command being sent to Arduino
        print(f"Sending command: {servo_command.strip()}")
        
        arduino.write(servo_command.encode())
    except Exception as e:
        print(f"Error during prediction or command sending: {e}")

# Main loop: Read EMG data from Arduino and send servo commands based on prediction
while True:
    try:
        # Read EMG data sent by Arduino over serial communication
        if arduino.in_waiting > 0:
            emg_data_raw = arduino.readline().decode('utf-8').strip()
            
            if emg_data_raw.isdigit():  # Ensure valid numeric data is received
                emg_data = int(emg_data_raw)
                print(f"EMG Data: {emg_data}")
                
                # Control servos based on ML model prediction using EMG data
                control_servos(emg_data)

        time.sleep(0.1)  # Small delay for stability

    except KeyboardInterrupt:
        print("Exiting...")
        arduino.close()  # Close serial connection before exiting program
        break

    except Exception as e:
        print(f"Error: {e}")
