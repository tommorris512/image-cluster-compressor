# Compiler and flags to use.
CC = clang
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -L./lib -lcml -lm

# Define the build directory for binaries to be placed in.
BUILD_DIR = build

# Define the source and object files to use in compilation/linking.
SRC_FILES = src/image_compressor.c
OBJ_FILES = $(SRC_FILES:src/%.c=$(BUILD_DIR)/%.o)

# Define the image_compressor executable as the main target.
all: $(BUILD_DIR)/image_compressor

# Build the image_compressor executable.
$(BUILD_DIR)/image_compressor: $(OBJ_FILES) lib/libcml.so
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile the relevant object files.
$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build directory of compiled binaries.
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/image_compressor

.PHONY: all clean
