

tests=json_test
CXXFLAGS=-std=c++11 -g -I../include

all: $(tests)

%_test: %_test.cpp
	g++ $< -o $@ $(CXXFLAGS)
	
	
json_test: json_test.cpp ../include/json.h
	g++ $< -o $@ $(CXXFLAGS)
	