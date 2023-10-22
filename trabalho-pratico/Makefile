CC        := gcc
# C_FLAGS := -Wall -Wextra -pedantic -O2
_C_FLAGS := -Wall -Wextra -O2
C_OPT_FLAGS := -O2
C_DEBUG_FLAGS := -Og -g -D_DEBUG -fkeep-inline-functions
C_FLAGS := $(_C_FLAGS) $(C_OPT_FLAGS)

BIN		:= out
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=-lm
EXECUTABLE	:= main

all: $(BIN)/$(EXECUTABLE)

build: clean all
build: clear
build: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

debug: C_FLAGS = $(_C_FLAGS) $(C_DEBUG_FLAGS)
debug: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(shell find ./$(SRC) -name '*.c')
	mkdir -p $(BIN)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*