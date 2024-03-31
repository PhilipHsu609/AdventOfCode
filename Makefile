CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17

BUILD_DIR = build
CPP_SOURCES = $(wildcard 2023/Day*/solve.cpp)
CPP_OBJECTS = $(patsubst 2023/Day%/solve.cpp,$(BUILD_DIR)/cpp/Day%,$(CPP_SOURCES))

all: init $(CPP_OBJECTS)

init:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/cpp
	mkdir -p $(BUILD_DIR)/rust

$(BUILD_DIR)/cpp/Day%: 2023/Day%/solve.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(CPP_OBJECTS)
