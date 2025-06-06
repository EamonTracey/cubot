CC=clang++
CFLAGS=-Wall -Wconversion -std=c++17
LIBS=-lonnxruntime -lfmt -lspdlog

SRC_DIR = src
SRC_LIB_DIR = $(SRC_DIR)/lib
SRC_BIN_DIR = $(SRC_DIR)/bin
SRC_PYTHON_DIR = $(SRC_DIR)/python

BUILD_DIR = build
BUILD_LIB_DIR = $(BUILD_DIR)/lib
BUILD_OBJ_DIR = $(BUILD_DIR)/obj
BUILD_BIN_DIR = $(BUILD_DIR)/bin

LIB_NAME = cubot
SRC_LIB_CC = $(wildcard $(SRC_LIB_DIR)/*.cc)
SRC_LIB_H = $(wildcard $(SRC_LIB_DIR)/*.h)
OBJ_FILES = $(patsubst $(SRC_LIB_DIR)/%.cc, $(BUILD_OBJ_DIR)/%.o, $(SRC_LIB_CC))

SRC_BIN_CC = $(wildcard $(SRC_BIN_DIR)/*.cc)
BINARIES = $(patsubst $(SRC_BIN_DIR)/%.cc, $(BUILD_BIN_DIR)/%, $(SRC_BIN_CC))

SRC_PYTHON_PY = $(wildcard $(SRC_PYTHON_DIR)/*.py)

MODELS_DIR = models
MODELS_ONNX = $(wildcard $(MODELS_DIR)/*.onnx)
MODELS_HEADER = $(SRC_LIB_DIR)/models.h

.PHONY: all
all: $(BUILD_LIB_DIR)/lib$(LIB_NAME).a $(BINARIES)

# Generate the models header from all .onnx files.
$(MODELS_HEADER): $(MODELS_ONNX)
	@rm -f $(MODELS_HEADER)
	@for model_onnx in $(MODELS_ONNX); do \
		xxd -i $$model_onnx >> $(MODELS_HEADER); \
	done

# Compile the library into object files.
$(BUILD_OBJ_DIR)/%.o: $(SRC_LIB_DIR)/%.cc $(SRC_LIB_H) $(MODELS_HEADER)
	@mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Archive the library object files.
$(BUILD_LIB_DIR)/lib$(LIB_NAME).a: $(OBJ_FILES)
	@mkdir -p $(BUILD_LIB_DIR)
	ar rcs $@ $^

# Build the binaries into executables.
$(BUILD_BIN_DIR)/%: $(SRC_BIN_DIR)/%.cc $(BUILD_LIB_DIR)/lib$(LIB_NAME).a
	@mkdir -p $(BUILD_BIN_DIR)
	$(CC) $(CFLAGS) $(LIBS) $< -o $@ -I$(SRC_LIB_DIR) -L$(BUILD_LIB_DIR) -l$(LIB_NAME)

# Format.
.PHONY: format
format:
	yapf -i $(SRC_PYTHON_PY)
	clang-format -i $(SRC_LIB_CC) $(SRC_LIB_H) $(SRC_BIN_CC)

# Clean.
.PHONY: clean
clean:
	rm -fr $(BUILD_DIR)
	rm -f $(MODELS_HEADER)
