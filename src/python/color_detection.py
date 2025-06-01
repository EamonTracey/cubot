# %% [markdown]
# # Color Detection

# %% [markdown]
# ## Setup

# %%
import json
import os

import cv2
from IPython.display import clear_output
import matplotlib.pyplot as plt
import numpy as np
import onnxruntime
import pandas as pd
from skimage.color import rgb2lab
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score, confusion_matrix, ConfusionMatrixDisplay
from sklearn.model_selection import train_test_split
from sklearn.svm import LinearSVC
from skl2onnx import to_onnx

# %%
DATA_DIR = os.path.join(os.pardir, os.path.join(os.pardir, "data"))
MODELS_DIR = os.path.join(os.pardir, os.path.join(os.pardir, "models"))
COLOR_MAP = {
    "w": [1.0, 1.0, 1.0],
    "y": [1.0, 1.0, 0.0],
    "g": [0.0, 1.0, 0.0],
    "b": [0.0, 0.0, 1.0],
    "r": [1.0, 0.0, 0.0],
    "o": [1.0, 0.5, 0.0]
}


# %%
def split_grid(image, n):
    """Split an image into a grid of n**2 equally sized images."""
    height, width, _ = image.shape
    height, width = height // n, width // n
    for row in range(n):
        for column in range(n):
            row0, row1 = height * row, height * (row + 1)
            column0, column1 = width * column, width * (column + 1)
            yield image[row0:row1, column0:column1]


def extract_feature_vector(image):
    """Convert an RGB image into a feature vector.

    The image is converted to the CIELAB color space. Then, the 9-dimensional
    feature vector includes the mean, median, and standard deviation of the L*,
    a*, and b* components.
    """
    image_lab = rgb2lab(image)
    pixels = image_lab.reshape(-1, 3)
    return np.concatenate([
        np.mean(pixels, axis=0),
        np.median(pixels, axis=0),
        np.std(pixels, axis=0),
    ])


# %% [markdown]
# ## Label

# %%
image_paths = os.listdir(DATA_DIR)
image_paths = [ip for ip in image_paths if ip.endswith(".jpg")]
image_paths = [os.path.join(DATA_DIR, ip) for ip in image_paths]

labels = {}
for image_path in image_paths:
    image_basename = os.path.basename(image_path)
    labels[image_basename] = list()

    image = cv2.imread(os.path.abspath(image_path), cv2.IMREAD_COLOR_RGB)
    for i, item in enumerate(split_grid(image, 3)):
        plt.imshow(item)
        plt.axis("off")

        print(
            f"Progress: {(len(labels) - 1)*9 + i + 1} / {len(image_paths)*9}")
        print(f"Current: {image_basename} {i + 1}")
        plt.show()

        color = input("Color (w|y|g|b|r|o)?: ")
        labels[image_basename].append(color)
        clear_output()

# %%
labels_path = os.path.join(DATA_DIR, "labels.json")
with open(labels_path, "w") as file:
    json_string = json.dumps(labels, indent=4)
    file.write(json_string)
    file.write("\n")

# %% [markdown]
# ## Train

# %%
SEED = 0
TEST_SIZE = 0.2

# %%
labels_path = os.path.join(DATA_DIR, "labels.json")
labels: dict
with open(labels_path) as file:
    labels = json.load(file)

# %%
COLUMNS = [
    "L*_mean", "a*_mean", "b*_mean", "L*_median", "a*_median", "b*_median",
    "L*_std", "a*_std", "b*_std", "color"
]

df = pd.DataFrame(columns=COLUMNS)
for image_basename, colors in labels.items():
    image_path = os.path.join(DATA_DIR, image_basename)
    image = cv2.imread(image_path, cv2.IMREAD_COLOR_RGB)
    for i, item in enumerate(split_grid(image, 3)):
        feature_vector = extract_feature_vector(item)
        df.loc[len(df)] = [*feature_vector, colors[i]]

df

# %%
X = df.drop("color", axis=1).to_numpy()
y = df["color"].to_numpy()
X_train, X_test, y_train, y_test = train_test_split(X,
                                                    y,
                                                    test_size=TEST_SIZE,
                                                    random_state=SEED)

# %%
models = {
    "Linear SVC": LinearSVC(max_iter=10000),
    "Logistic Regression": LogisticRegression(max_iter=10000)
}

fig, axes = plt.subplots(1, len(models), figsize=(10, 4))
for ax, (name, model) in zip(axes, models.items()):
    model.fit(X_train, y_train)
    y_pred = model.predict(X_test)
    acc = accuracy_score(y_test, y_pred)
    print(f"{name} Accuracy: {acc * 100:.2f}%")

    cm = confusion_matrix(y_test, y_pred, labels=model.classes_)
    disp = ConfusionMatrixDisplay(confusion_matrix=cm,
                                  display_labels=model.classes_)
    disp.plot(ax=ax, cmap="Reds", colorbar=False)
    ax.set_title(f"{name} Confusion Matrix")

plt.tight_layout()
plt.show()

# %%
for name, model in models.items():
    onnx = to_onnx(model, X[:1].astype(np.float32), target_opset=21)
    path = os.path.join(MODELS_DIR, f"{name.replace(" ", "_").lower()}.onnx")
    with open(path, "wb") as f:
        f.write(onnx.SerializeToString())

# %% [markdown]
# ## Demo

# %%
models = {}
onnx_paths = os.listdir(MODELS_DIR)
onnx_paths = [os.path.join(MODELS_DIR, op) for op in onnx_paths]
for onnx_path in onnx_paths:
    name = os.path.basename(onnx_path).removesuffix(".onnx").replace(
        "_", " ").title()
    session = onnxruntime.InferenceSession(onnx_path)
    input_name = session.get_inputs()[0].name
    output_name = session.get_outputs()[0].name
    models[name] = (session, input_name, output_name)

# %%
camera = cv2.VideoCapture(0)
window_name = "Color Detection Demo"
cv2.namedWindow(window_name)
image: np.ndarray
while True:
    _, frame = camera.read()
    frame = cv2.flip(frame, 1)

    height, width = frame.shape[:2]
    size = min(width // 2, height // 2)
    x0 = (width - size) // 2
    y0 = (height - size) // 2
    x1 = x0 + size
    y1 = y0 + size

    image = frame[y0:y1, x0:x1].copy()
    cv2.rectangle(frame, (x0, y0), (x1, y1), (0, 0, 255), 2)

    cv2.imshow(window_name, frame)
    k = cv2.waitKey(1) & 0xFF
    if k == 32:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        break
camera.release()
cv2.destroyAllWindows()
cv2.waitKey(1)

plt.imshow(image)
plt.axis("off")
plt.show()

# %%
X = np.array([extract_feature_vector(item) for item in split_grid(image, 3)])

fig, axes = plt.subplots(1, len(models), figsize=(4 * len(models), 4))
axes = axes if len(models) > 1 else [axes]
for ax, (name, (session, input_name,
                output_name)) in zip(axes, models.items()):
    y = session.run([output_name], {input_name: X.astype(np.float32)})
    face = y[0].reshape(3, 3)

    reconstruction = np.zeros((3, 3, 3))
    for i in range(3):
        for j in range(3):
            reconstruction[i, j] = COLOR_MAP[face[i, j]]

    ax.imshow(reconstruction)
    ax.set_xticks([]), ax.set_yticks([])
    ax.set_title(f"{name} Color Detection")
    for i in range(1, 3):
        ax.axhline(i - 0.5, color="black", linewidth=3)
    for j in range(1, 3):
        ax.axvline(j - 0.5, color="black", linewidth=3)

plt.tight_layout()
plt.show()
