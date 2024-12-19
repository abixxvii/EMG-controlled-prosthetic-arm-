import xgboost as xgb
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from sklearn.preprocessing import LabelEncoder
import joblib

# Load data
df = pd.read_csv('Data/preprocessed_data.csv')

# Features and target
X = df.drop(columns=['Movement'])
y = df['Movement']

# Encode target labels into numeric values
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

# Split the data into training and test sets
X_train, X_test, y_train, y_test = train_test_split(X, y_encoded, test_size=0.2, random_state=42)

# Convert the datasets into DMatrix format (required by XGBoost)
dtrain = xgb.DMatrix(X_train, label=y_train)
dtest = xgb.DMatrix(X_test, label=y_test)

# Define the parameters for the XGBoost model
params = {
    'objective': 'multi:softmax',  # Multi-class classification
    'num_class': len(label_encoder.classes_),  # Number of classes
    'eval_metric': 'mlogloss',  # Multi-class log-loss metric
    'max_depth': 3,  # Limit the depth of trees to avoid overfitting
    'min_child_weight': 1,  # Minimum child weight to further prevent overfitting
    'subsample': 0.8,  # Use 80% of the data for each tree (random sampling)
    'colsample_bytree': 0.8,  # Use 80% of the features for each tree
    'learning_rate': 0.1  # Lower learning rate for more gradual convergence
}

# Watchlist to use for early stopping
watchlist = [(dtest, 'eval'), (dtrain, 'train')]

# Train the model using the DMatrix format
model = xgb.train(
    params, 
    dtrain, 
    num_boost_round=1000,  # Limit the number of boosting rounds
    evals=watchlist,  # Use the evaluation set for early stopping
    early_stopping_rounds=10,  # Stop training after 10 rounds without improvement
    verbose_eval=True  # Display the evaluation results
)

# Predict on the test set
y_pred = model.predict(dtest)

# Convert predictions to integers (since `predict()` returns probabilities)
y_pred = y_pred.astype(int)

# Evaluate the model
print(f'Accuracy: {accuracy_score(y_test, y_pred)}')
print(f'Classification Report:\n{classification_report(y_test, y_pred)}')

# Save the model using XGBoost's save_model method
model.save_model('models/xgb_model.json')  # Save in XGBoost's format

# Save the label encoder using joblib
joblib.dump(label_encoder, 'models/label_encoder.pkl')

print("Model and LabelEncoder saved successfully.")
