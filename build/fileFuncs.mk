CC = gcc
CFLAGS = -Wall -Werror -fPIC -g -shared

OBJ_DIR = $(shell findPath -f 11)/
LIB_DIR = $(shell findPath -f 2)/
INCLUDE_DIR = $(shell findPath -f 120)/
DIRS_TO_LOOK_IN =
INCLUDED_LIB_NAME = -lregexC
OBJ_FILES = $(OBJ_DIR)fileExists.o $(OBJ_DIR)fileRegexInC.o $(OBJ_DIR)writeToFile.o 
TARGET = fileFuncs

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

