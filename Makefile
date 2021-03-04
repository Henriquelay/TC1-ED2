TARGET_EXEC ?= trab1

BUILD_DIR ?= .
SRC_DIRS ?= ./src ./headers

MKDIR_P ?= mkdir -p

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
CFLAGS ?= $(INC_FLAGS) -g -Wall -Werror -O4

VALGRIND ?= valgrind
VALZIN_FLAGS ?= ""
VAL_FLAGS ?= $(VALZIN_FLAGS) --leak-check-full
VALZAO_FLAGS ?= $(VAL_FLAGS) --show-leak-kinds=all

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET_EXEC) */*.o


valzin: $(TARGET_EXEC)
	$(VALGRIND) $(VALZIN_FLAGS) $(TARGET_EXEC) $(ARGS)
valzin: $(TARGET_EXEC)
	$(VALGRIND) $(VAL_FLAGS) $(TARGET_EXEC) $(ARGS)
valzin: $(TARGET_EXEC)
	$(VALGRIND) $(VALZAO_FLAGS) $(TARGET_EXEC) $(ARGS)
