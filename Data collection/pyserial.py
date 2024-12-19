import serial  # To communicate with Arduino
import csv     # To save data to a CSV file
import time    # For time-related functions

# Set up serial communication (adjust 'COM3' to your port)
# On Windows it might be 'COM5', on Linux/macOS it might be '/dev/ttyUSB0' or '/dev/ttyACM0'
ser = serial.Serial('COM5', 9600)  # Open the serial port at 9600 baud rate
time.sleep(2)  # Wait for Arduino to initialize

# Open a CSV file to log data (this will overwrite if the file exists, use 'a' to append)
with open('wristextension.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Time", "EMG Value"])  # Write headers to CSV

    # Define the logging duration (in seconds)
    logging_duration = 60  # Example: 60 seconds (1 minute)
    start_time = time.time()  # Capture the start time

    # Start reading and logging the data
    try:
        print(f"Logging EMG data for {logging_duration} seconds... Press Ctrl+C to stop.")
        while True:
            # Check if the logging time limit has been reached
            elapsed_time = time.time() - start_time
            if elapsed_time >= logging_duration:
                print("Logging time limit reached. Stopping data collection.")
                break  # Exit the loop after reaching the time limit

            # Read a line of data from Arduino
            line = ser.readline().decode('utf-8').strip()  # Decode byte data and remove newline

            # Get the current time for logging
            current_time = time.time()

            # Convert the EMG value from string to integer
            emg_value = int(line)

            # Write time and EMG value to the CSV file
            writer.writerow([current_time, emg_value])

            # Optionally, print the data to the console
            print(f"Time: {current_time}, EMG Value: {emg_value}")

    except KeyboardInterrupt:
        print("\nLogging stopped by user.")
    finally:
        ser.close()  # Close the serial port when done
