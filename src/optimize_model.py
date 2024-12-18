import xgboost as xgb
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from sklearn.preprocessing import LabelEncoder
import joblib

# Load data
df = pd.read_csv('Data/preprocessed_data.csv')
X = df.drop(columns=['Movement'])
y = df['Movement']

# Encode the string labels to numerical labels
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)  # Convert 'Movement' to numeric values

# Split the data
X_train, X_test, y_train, y_test = train_test_split(X, y_encoded, test_size=0.2, random_state=42)

# Train XGBoost model
model = xgb.XGBClassifier(eval_metric='mlogloss', random_state=42)
model.fit(X_train, y_train)

# Evaluate the model
y_pred = model.predict(X_test)
print(f'Accuracy: {accuracy_score(y_test, y_pred)}')
print(f'Classification Report:\n{classification_report(y_test, y_pred)}')

# Save the model
joblib.dump(model, 'models/xgb_model.pkl')

# Optionally save the label encoder to use for future predictions
joblib.dump(label_encoder, 'models/label_encoder.pkl')
