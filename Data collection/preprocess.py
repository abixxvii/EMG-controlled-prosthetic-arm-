import pandas as pd
import numpy as np
import glob

# Define the file paths for each CSV file
csv_files = {
    '4fingerflex.csv': '4fingerflex',
    'indexfingerflex.csv': 'Index Flex',
    'middlefingerflex.csv': 'Middle Flex',
    'ringfingerflex.csv': 'Ring Flex',
    'pinkyfingerflex.csv': 'Pinky Flex',
    'wristflexion.csv': 'Wrist Flexion',
    'wristextension.csv': 'Wrist Extension'
}

# Create an empty list to store the data
all_data = []

# Loop through each file and label the data
for file, label in csv_files.items():
    df = pd.read_csv(file)  # Read the data from the CSV file
    df['Movement'] = label   # Add a column for the movement type label
    all_data.append(df)

# Concatenate all the data into one DataFrame
data = pd.concat(all_data, ignore_index=True)

# Convert 'Time' column to numeric (in case it's not already)
data['Time'] = pd.to_numeric(data['Time'])

# Optional: Segment the data into smaller windows (e.g., 1 second)
window_size = 1  # in seconds
data['Time'] = data['Time'] - data['Time'].min()  # Normalize time to start from zero
data['Time Window'] = (data['Time'] // window_size).astype(int)  # Create a time window column

# Group the data by the 'Time Window' column and calculate the mean EMG value for each window
windowed_data = data.groupby(['Time Window', 'Movement'])['EMG Value'].mean().reset_index()

# Preview the processed data
print(windowed_data.head())

# Save the preprocessed data to a new CSV file
windowed_data.to_csv('preprocessed_data.csv', index=False)
