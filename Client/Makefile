SRC_DIR := .
OBJ_DIR := .
BIN_DIR := .

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


CC := gcc
CFLAGS := -g --std=c11 -pedantic -D _GNU_SOURCE -D _POSIX_C_SOURCE=1 -Wall -Wextra
EXE := CL Driver

.PHONY: all clean

all: ${EXE}

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $@

clean:
	${RM} ${OBJ} ${EXE}
