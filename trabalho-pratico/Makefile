CC        := gcc
_C_FLAGS := -Wall -Wextra
C_OPT_FLAGS := -O2
C_DEBUG_FLAGS := -Og -g -D_DEBUG -fkeep-inline-functions
GLIB_FLAGS := `pkg-config --cflags glib-2.0`

C_FLAGS := $(_C_FLAGS) $(C_OPT_FLAGS) $(GLIB_FLAGS)

BIN		:= out
OBJ     := obj
SRC		:= src
INCLUDE	:= include
LIB		:= lib
EXECUTABLE	:= programa-principal

LIBRARIES	:=`pkg-config --libs glib-2.0` -lm 
EXEC_PATH := $(BIN)/$(EXECUTABLE)

SRC_FILES := $(wildcard $(SRC)/**/*.c $(wildcard $(SRC)/*.c))
OBJ_FILES := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRC_FILES))
SRC_DIRS  := $(wildcard $(SRC)/**/)
OBJ_DIRS  := $(patsubst $(SRC)/%,$(OBJ)/%,$(SRC_DIRS))

all: $(EXEC_PATH)


debug: C_FLAGS = $(_C_FLAGS) $(C_DEBUG_FLAGS) $(GLIB_FLAGS)
debug: $(EXEC_PATH)

$(EXEC_PATH): $(OBJ_DIRS) $(OBJ_FILES)
	mkdir -p $(BIN)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $(filter-out $(OBJ_DIRS), $^) -o $(EXEC_PATH) $(LIBRARIES)

$(OBJ_DIRS): $(SRC_DIRS)
	mkdir -p $@

$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -c $< -o $@ $(LIBRARIES)

run: $(EXEC_PATH)
	./$(EXEC_PATH)

.PHONY: run-debug
run-debug: debug
	./$(EXEC_PATH)

.PHONY: locate
locate:
	@echo $(EXEC_PATH)

clean:
	-rm $(BIN)/*
	-rm -rf $(OBJ)/*