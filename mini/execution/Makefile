# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -g

# Directories
SRC_DIR = .
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Source Files
SRCS = main.c \
       builtins/builtin_pwd.c \
       builtins/builtin_echo.c \
       builtins/builtin_env.c \
       builtins/builtin_export.c \
       builtins/builtin_unset.c \
       builtins/builtin_cd.c \
       external/external.c \
       helpers/helpers.c

# Object Files
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Executable Name
TARGET = $(BIN_DIR)/shell

# Default Target
all: $(TARGET)

# Create Directories
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Linking the Executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile Source Files into Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up Build Files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean