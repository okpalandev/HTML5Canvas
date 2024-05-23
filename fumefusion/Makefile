CC=gcc

CORE_DIR := core
BIN_DIR := bin
OBJ_DIR := obj

# Flags
CFLAGS := -Wall -Wextra -Wpedantic -MMD  -MP -g
LDFLAGS := -lm

ifeq ($(OS),Windows_NT)
	RM = @rm -r
	MKDIR = @mkdir -p
	EXEC := $(BIN_DIR)/fumefusion
else
	RM = @rm -rf
	MKDIR = @mkdir -p
	EXEC := $(BIN_DIR)/fumefusion.exe
endif

# Source files
CORE_FILES := $(wildcard $(CORE_DIR)/*.c)
# Header Files
HEADER_FILES := $(wildcard $(CORE_DIR)/*.h)
# Object files
OBJ_FILES := $(CORE_FILES:$(CORE_DIR)/%.c=$(OBJ_DIR)/%.o)

# Executable
all: $(EXEC)

# Build rule 
$(EXEC): $(OBJ_FILES) | $(HEADER_FILES) 
	$(MKDIR) $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) main.c -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(CORE_DIR)/%.c | $(HEADER_FILES) 
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	$(RM) $(OBJ_FILES) $(EXEC) $(OBJ_FILES:.o=.d)

# Include dependency files
-include $(OBJ_FILES:$(OBJ_DIR)/%.o=$(OBJ_DIR)/%.d)

.PHONY: all clean
