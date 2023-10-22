CC        := gcc
_C_FLAGS := -Wall -Wextra -O2
C_OPT_FLAGS := -O2
C_DEBUG_FLAGS := -Og -g -D_DEBUG -fkeep-inline-functions
GLIB_FLAGS := `pkg-config --cflags glib-2.0`

C_FLAGS := $(_C_FLAGS) $(C_OPT_FLAGS) $(GLIB_FLAGS)

BIN		:= out
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=`pkg-config --libs glib-2.0` -lm 
EXECUTABLE	:= main

all: $(BIN)/$(EXECUTABLE)

build: clean all
build: clear
build: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

debug: C_FLAGS = $(_C_FLAGS) $(C_DEBUG_FLAGS) $(GLIB_FLAGS)
debug: $(BIN)/$(EXECUTABLE)

.PHONY: run-debug
run-debug: debug
	./$(BIN)/$(EXECUTABLE)

.PHONY: locate
locate:
	@echo $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(shell find ./$(SRC) -name '*.c')
	mkdir -p $(BIN)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*