mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(mkfile_path))
INC=-I./lib -I./src
CC=g++
FOLDERS=./lib ./src
OBJ_FOLDER=bin/objects
SOURCES=$(shell for file in `find $(FOLDERS) -name '*.cpp'`;do echo $$file; done)
OBJECTS=$(shell for file1 in `for file in $(SOURCES);do echo $$file | sed 's/.cpp/.o/g'; done`;do echo "$(OBJ_FOLDER)/$$file1"; done; )
FLAGS=-Wall -std=c++11 -pthread

ifeq ($(OS),Windows_NT)
EXT=.exe
else
EXT=
endif

DIST=debug

all: $(DIST)

test: clean_test bin/tests/main$(EXT) 
	
debug: clean_debug bin/debug/main$(EXT)

clean: 	
	
clean_test:
	rm -rf bin/objects/*
	rm -rf bin/tests/*

clean_debug:
	rm -rf bin/objects/*
	rm -rf bin/debug/*
	

	
.PHONY: all test clean debug

bin/%$(EXT): %.cpp $(SOURCES)
	$(CC) $^ $(INC) $(FLAGS) -o $@
	@echo -e "--== " $@ " Executable Built Successfully ==--"
	@echo " "

.SECONDARY: $(OBJECTS)