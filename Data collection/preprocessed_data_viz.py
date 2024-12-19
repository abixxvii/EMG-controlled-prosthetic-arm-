import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Load the preprocessed data
df = pd.read_csv("preprocessed_data.csv")

# Visualize the EMG value distribution by movement
plt.figure(figsize=(10, 6))
sns.boxplot(data=df, x="Movement", y="EMG Value")
plt.title("Distribution of EMG Values by Movement")
plt.xticks(rotation=45)
plt.show()

# Check for missing values
print(df.isnull().sum())
