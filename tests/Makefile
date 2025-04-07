

tests=json_test
CXXFLAGS=-std=c++17 -g -I../include

all: $(tests)

%_test: %_test.cpp
	g++ $< -o $@ $(CXXFLAGS)
	
	
json_test: json_test.cpp ../include/json/json.h mls-unit-test/test_main.cpp
	g++ mls-unit-test/test_main.cpp $< -o $@ $(CXXFLAGS)
	
