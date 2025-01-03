CC=gcc
CFLAGS=-Wall -Wconversion
LIBS=-ljpeg

AR=ar
ARFLAGS = rcs

SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
BUILD_OBJS_DIR = $(BUILD_DIR)/objs
BUILD_LIBS_DIR = $(BUILD_DIR)/libs
BUILD_BINS_DIR = $(BUILD_DIR)/bins

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
HEADER_FILES = $(wildcard $(SRC_DIR)/*.h)
BIN_FILES = $(wildcard $(BIN_DIR)/*.c)
BUILD_OBJS_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_OBJS_DIR)/%.o, $(SRC_FILES))
BUILD_BINS_FILES = $(patsubst $(BIN_DIR)/%.c, $(BUILD_BINS_DIR)/%, $(BIN_FILES))

.PHONY: all
all: objs libs bins

##################################################
# Build object files.
##################################################

.PHONY: objs
objs: $(BUILD_OBJS_FILES)

$(BUILD_OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

##################################################
# Build library files.
##################################################

.PHONY: libs
libs: $(BUILD_LIBS_DIR)/libcubot.a

$(BUILD_LIBS_DIR)/libcubot.a: $(BUILD_OBJS_FILES)
	mkdir -p $(BUILD_LIBS_DIR)
	$(AR) $(ARFLAGS) $(BUILD_LIBS_DIR)/libcubot.a $(BUILD_OBJS_FILES)

##################################################
# Build binary files.
##################################################

.PHONY: bins
bins: $(BUILD_BINS_FILES)

$(BUILD_BINS_DIR)/%: $(BIN_DIR)/%.c libs
	mkdir -p $(BUILD_BINS_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -L$(BUILD_LIBS_DIR) -lcubot $(LIBS) $< -o $@

##################################################
# Helpful commands.
##################################################

.PHONY: format
format:
	clang-format -i $(SRC_FILES) $(HEADER_FILES) $(BIN_FILES)

.PHONY: clean
clean:
	rm -fr $(BUILD_DIR)
