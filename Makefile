# Compilatore e flags
CXX = g++
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs)
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude $(ROOTCFLAGS)

# Cartelle
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# File sorgenti (includiamo main.cpp separatamente)
SOURCES = $(wildcard $(SRC_DIR)/*.cc) main.cpp
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(filter %.cpp, $(SOURCES))) \
          $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(filter %.cc, $(SOURCES)))

EXECUTABLE = $(BIN_DIR)/main

# Regole
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(ROOTLIBS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
