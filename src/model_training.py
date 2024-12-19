import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
import joblib
import os

# Load the dataset (adjust the path if needed)
df = pd.read_csv('Data/preprocessed_data.csv')

# Check for missing values
if df.isnull().sum().any():
    print("Missing values found. Filling missing values.")
    df = df.fillna(0)  # Fill missing values with 0 (you can choose other strategies)

# Print the columns to check if 'target' is the correct column name
print("Columns in the dataset:", df.columns)

# If the target column isn't named 'target', inspect the dataset
# If 'Movement' is the target or another column, set it here. 
# We assume for this dataset that 'Movement' might be a classification target

# For now, let's assume 'Movement' is the target and we are predicting that
# This should be updated if 'Movement' is incorrect, and you need to set the actual target column.
if 'Movement' not in df.columns:
    print("Error: The 'Movement' column is missing.")
else:
    print("The target column 'Movement' is present. Proceeding with this as the target.")

# Define features (X) and target variable (y)
# Assuming 'Movement' is the target and dropping 'Time Window' which seems like an ID or time column
X = df.drop(columns=['Movement', 'Time Window'])  # Drop irrelevant columns
y = df['Movement']  # Target variable (Movement)

# If you want to categorize the 'Movement' into classes (like a classification problem)
# Ensure that 'Movement' is categorical or numerical for classification

# Convert categorical target variable to numerical if needed (for classification purposes)
if y.dtype == 'object':
    y = y.astype('category').cat.codes  # Convert to numerical codes for classification

# Split the data into training and test sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train a Random Forest model
model = RandomForestClassifier(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# Make predictions and evaluate the model
y_pred = model.predict(X_test)
print(f'Accuracy: {accuracy_score(y_test, y_pred)}')
print(f'Classification Report:\n{classification_report(y_test, y_pred)}')

# Ensure the 'models' directory exists
os.makedirs('models', exist_ok=True)

# Save the trained model
joblib.dump(model, 'models/rf_model.pkl')

print("Model saved successfully.")
