CC        := gcc
_C_FLAGS := -Wall -Wextra
C_OPT_FLAGS := -O2
C_DEBUG_FLAGS := -O0 -g -D_DEBUG -fkeep-inline-functions
GLIB_FLAGS := `pkg-config --cflags glib-2.0`

C_FLAGS := $(_C_FLAGS) $(C_OPT_FLAGS) $(GLIB_FLAGS)

BIN		:= out
OBJ     := obj
SRC		:= src
INCLUDE	:= include
LIB		:= lib
EXECUTABLE	:= programa-principal
TEST_EXECUTABLE	:= programa-testes

LIBRARIES	:=`pkg-config --libs glib-2.0` -lm 
EXEC_PATH := $(EXECUTABLE)
TEST_EXEC_PATH := $(TEST_EXECUTABLE)

SRC_DIRS  := $(wildcard $(SRC)/**/)
SRC_FILES := $(wildcard $(SRC)/**/*.c $(wildcard $(SRC)/*.c))

SRC_OBJ_DIRS  := $(patsubst $(SRC)/%,$(OBJ)/$(EXEC_PATH)/%,$(SRC_DIRS))
TEST_OBJ_DIRS  := $(patsubst $(SRC)/%,$(OBJ)/$(TEST_EXEC_PATH)/%,$(SRC_DIRS))

SRC_OBJ_FILES := $(patsubst $(SRC)/%.c,$(OBJ)/$(EXEC_PATH)/%.o,$(SRC_FILES))
TEST_OBJ_FILES := $(patsubst $(SRC)/%.c,$(OBJ)/$(TEST_EXEC_PATH)/%.o,$(SRC_FILES))

all: $(EXEC_PATH) $(TEST_EXEC_PATH)

debug: C_FLAGS = $(_C_FLAGS) $(C_DEBUG_FLAGS) $(GLIB_FLAGS)
debug: $(EXEC_PATH) $(TEST_EXEC_PATH)

# ============== PROGRAMA PRINCIPAL ==============
$(EXEC_PATH): $(SRC_OBJ_DIRS) $(SRC_OBJ_FILES)
	mkdir -p $(BIN)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $(filter-out $(SRC_OBJ_DIRS), $^) -o $(EXEC_PATH) $(LIBRARIES)

$(SRC_OBJ_DIRS): $(SRC_DIRS)
	mkdir -p $@

$(OBJ)/$(EXEC_PATH)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -c $< -o $@ $(LIBRARIES)

# ============== PROGRAMA TESTES ==============
$(TEST_EXEC_PATH): $(TEST_OBJ_DIRS) $(TEST_OBJ_FILES)
	mkdir -p $(BIN)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) -DMAKE_TEST $(filter-out $(TEST_OBJ_DIRS), $^) -o $(TEST_EXEC_PATH) $(LIBRARIES)

$(TEST_OBJ_DIRS): $(SRC_DIRS)
	mkdir -p $@

$(OBJ)/$(TEST_EXEC_PATH)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) -DMAKE_TEST -c $< -o $@ $(LIBRARIES)

run: $(EXEC_PATH)
	./$(EXEC_PATH)

.PHONY: run-debug
run-debug: debug
	./$(EXEC_PATH)

.PHONY: locate
locate:
	@echo ./$(EXEC_PATH)

.PHONY: locate_test
locate_test:
	@echo ./$(TEST_EXEC_PATH)

clean:
	-rm $(EXEC_PATH)
	-rm $(TEST_EXEC_PATH)
	-rm -rf $(OBJ)
	-rm -rf Resultados/*.csv
	-rm -rf Resultados/*.txt