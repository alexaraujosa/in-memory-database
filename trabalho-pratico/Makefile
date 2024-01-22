CC        := gcc
_C_FLAGS := -Wall -Wextra #-DGM_WIDECHAR
C_OPT_FLAGS := -O2
C_DEBUG_FLAGS := -Og -g -ggdb3 -D_DEBUG -fkeep-inline-functions
GLIB_FLAGS := `pkg-config --cflags glib-2.0`

C_FLAGS := $(_C_FLAGS) $(C_OPT_FLAGS) $(GLIB_FLAGS)

BIN		:= out
OBJ     := obj
SRC		:= src
INCLUDE	:= include
LIB		:= lib
EXECUTABLE	:= programa-principal
TEST_EXECUTABLE	:= programa-testes

LIBRARIES	:=`pkg-config --libs glib-2.0` -lm -lncurses
# EXEC_PATH := $(BIN)/$(EXECUTABLE)
EXEC_PATH := $(EXECUTABLE)
TEST_EXEC_PATH := $(TEST_EXECUTABLE)

SRC_DIRS  := $(shell find $(SRC) -mindepth 1 -type d)
SRC_FILES := $(shell find $(SRC) -name "*.c")

SRC_OBJ_DIRS  := $(patsubst $(SRC)/%,$(OBJ)/$(EXEC_PATH)/%,$(SRC_DIRS))
TEST_OBJ_DIRS  := $(patsubst $(SRC)/%,$(OBJ)/$(TEST_EXEC_PATH)/%,$(SRC_DIRS))

SRC_DEP_FILES := $(patsubst $(SRC)/%.c,$(OBJ)/$(EXEC_PATH)/%.d,$(SRC_FILES))
TEST_DEP_FILES := $(patsubst $(SRC)/%.c,$(OBJ)/$(TEST_EXEC_PATH)/%.d,$(SRC_FILES))

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

-include $(SRC_DEP_FILES)
-include $(TEST_DEP_FILES)

$(OBJ)/$(EXEC_PATH)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(CC) $(C_FLAGS) -I$(INCLUDE) -MMD -c $< -o $@ $(LIBRARIES)

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

clean:
	-rm $(EXEC_PATH)
	-rm $(TEST_EXEC_PATH)
	-rm -rf $(OBJ)
	-rm -rf Resultados/*.csv
	-rm -rf Resultados/*.txt