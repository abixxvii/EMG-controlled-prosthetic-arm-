import serial
import time
import joblib
import numpy as np
import pandas as pd
import xgboost as xgb

# Load the trained XGBoost model and label encoder
model = joblib.load('models/xgb_model.pkl')
label_encoder = joblib.load('models/label_encoder.pkl')

# Open serial connection to Arduino
arduino = serial.Serial('COM5', 9600)  # Replace 'COM5' with your port
time.sleep(2)  # Wait for Arduino to initialize

# Function to preprocess EMG data and control servos
def control_servos(emg_value):
    # Create a DataFrame matching the training data structure
    input_data = pd.DataFrame({'Time Window': [0], 'EMG Value': [emg_value]})  # Add dummy 'Time Window'
    
    # Convert input features to DMatrix format for XGBoost prediction
    dmatrix_input = xgb.DMatrix(input_data)
    
    # Get prediction from the model
    prediction = model.predict(dmatrix_input)[0]
    predicted_label = label_encoder.inverse_transform([int(prediction)])[0]
    
    # Define servo angles based on predicted movement
    if predicted_label == 'Pinky Flex':
        pinky_angle, ring_angle, middle_angle, index_angle = (90, 20, 160, 90)
    elif predicted_label == 'Ring Flex':
        pinky_angle, ring_angle, middle_angle, index_angle = (45, 160, 120, 90)
    elif predicted_label == 'Middle Flex':
        pinky_angle, ring_angle, middle_angle, index_angle = (60, 80, 50, 90)
    elif predicted_label == 'Index Flex':
        pinky_angle, ring_angle, middle_angle, index_angle = (70, 30, 90, 180)
    else:
        print("Unknown movement detected.")
        return

    # Send servo angles to Arduino as a comma-separated string
    servo_command = f"{pinky_angle},{ring_angle},{middle_angle},{index_angle}\n"
    
    # Debug: Print command being sent to Arduino
    print(f"Sending to Arduino: {servo_command}")
    
    arduino.write(servo_command.encode())

# Main loop to read EMG data and control servos
while True:
    try:
        # Simulate EMG data or replace with actual Arduino reading logic
        emg_data = np.random.randint(50)  # Replace with actual EMG input
        
        print(f"EMG Data: {emg_data}")
        
        # Control servos based on prediction
        control_servos(emg_data)
        
        time.sleep(0.1)  # Adjust delay as needed for smooth operation
    
    except KeyboardInterrupt:
        print("Exiting program...")
        arduino.close()  # Close the serial connection before exiting
        break

    except Exception as e:
        print(f"Error: {e}")
