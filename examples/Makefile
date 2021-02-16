
tests=json_test
CXXFLAGS=-std=c++17 -g -I../include

all: files_example create_example

%_example: %_example.cpp  ../include/json/json.h
	g++ $< -o $@ $(CXXFLAGS)


