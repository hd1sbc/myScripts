CC = gcc
CFLAGS = -Wall -Werror -fPIC -g -shared

OBJ_DIR = $(shell findPath -f 11)/
LIB_DIR = $(shell findPath -f 2)/
INCLUDE_DIR = $(shell findPath -f 120)/
DIRS_TO_LOOK_IN =
INCLUDED_LIB_NAME = -lupdateFAndF
OBJ_FILES = $(OBJ_DIR)customOpen.o $(OBJ_DIR)customRmDir.o $(OBJ_DIR)dbValidationSpecial.o $(OBJ_DIR)origOwFuncs.o $(OBJ_DIR)sourceIsInCsv.o $(OBJ_DIR)customOpenHelpers.o $(OBJ_DIR)customUnlink.o $(OBJ_DIR)fileInfoOwSysCalls.o $(OBJ_DIR)customRename.o $(OBJ_DIR)customWrite.o $(OBJ_DIR)globalVarsOwSysCalls.o $(OBJ_DIR)setupAndSpecialFuncs.o $(OBJ_DIR)updateFAndFForNewPathAndOldPath.o 
TARGET = overWrittenSysCalls

ALL_FILES = $(OBJ_FILES)

all: $(TARGET)


$(TARGET): $(ALL_FILES)
	@if [ -z "$(INCLUDED_LIB_NAME)" ]; then \
                $(CC) $(CFLAGS) -o $(LIB_DIR)lib$(TARGET).so $(OBJ_FILES) -I$(INCLUDE_DIR); \
        else \
                $(CC) $(CFLAGS) -o $(LIB_DIR)lib$(TARGET).so $(ALL_FILES) -L$(LIB_DIR) $(INCLUDED_LIB_NAME) -I$(INCLUDE_DIR) -Wl,-rpath,$(LIB_DIR); \
        fi


.PHONY: all

# Notes:
# if you get stuck with the compilation, try adding -Wl,--trace or -Wl,--trace-symbol=nameOfFile to the end of the lines beginning with $(CC) to get a better idea of potential errors

