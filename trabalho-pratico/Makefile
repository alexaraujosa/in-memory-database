#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# SHELL := $(shell pwd)/scripts/timer.sh

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= out

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

# define object directory
OBJ_DIR := obj

# ifeq ($(OS),Windows_NT)
# MAIN	:= main.exe
# SOURCEDIRS	:= $(SRC)
# INCLUDEDIRS	:= $(INCLUDE)
# LIBDIRS		:= $(LIB)
# FIXPATH = $(subst /,\,$1)
# RM			:= del /q /f
# MD	:= mkdir
# else
# MAIN	:= main
# SOURCEDIRS	:= $(shell find $(SRC) -type d)
# INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
# LIBDIRS		:= $(shell find $(LIB) -type d)
# FIXPATH = $1
# RM = rm -f
# MD	:= mkdir -p
# endif

MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p

define process_time =
	@echo PROCESS TIME
	$(eval $$timeStart = $(1))
	$(eval dt=$$(echo "$$timeEnd - $$timeStart" | bc))
	$(eval dd=$$(echo "$$dt/86400" | bc))
	$(eval dt2=$$(echo "$$dt-86400*$$dd" | bc))
	$(eval dh=$$(echo "$$dt2/3600" | bc))
	$(eval dt3=$$(echo "$$dt2-3600*$$dh" | bc))
	$(eval dm=$$(echo "$$dt3/60" | bc))
	$(eval ds=$$(echo "$$dt3-60*$$dm" | bc))
	$(eval LC_NUMERIC=C printf "Total runtime: $$%d:%02d:%02d:%02.4f\n" $$dd $$dh $$dm $$ds)
endef

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C object files
# Place the object files in the obj directory
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
# OBJECTS		:= $(SOURCES:.c=.o)

# define the dependency output files
# Place the dependency files in the obj directory
DEPS := $(patsubst $(SRC)/%.c, $(OBJ_DIR)/%.d, $(SOURCES))
# DEPS		:= $(OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

# OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))
OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(OBJ_DIR) $(MAIN)
	@echo Build complete!

$(OUTPUT):
	d=$$(date +%s)
	$(MD) $(OUTPUT)
    # && echo "Build took $$(($$(date +%s)-d)) seconds"
	@$(call process_time, d)

$(OBJ_DIR):
	$(MD) $(OBJ_DIR)

$(MAIN): $(OBJECTS) 
	@echo $(OUTPUTMAIN)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
# .c.o:
$(OBJ_DIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!

run: all
	@echo ========== START EXECUTION CONTEXT ==========
	./$(OUTPUTMAIN)
	@echo =========== END EXECUTION CONTEXT ===========
