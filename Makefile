# Compilatore e flags
CXX = g++
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs)
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude $(ROOTCFLAGS)

# Cartelle
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Sorgenti comuni
COMMON_SOURCES = $(wildcard $(SRC_DIR)/*.cc)
COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(COMMON_SOURCES))

# Eseguibili e relativi sorgenti
EXECUTABLES = $(BIN_DIR)/main $(BIN_DIR)/align

MAIN_SOURCE = main.cpp
ALIGN_SOURCE = align.cpp

MAIN_OBJECT = $(OBJ_DIR)/main.o
ALIGN_OBJECT = $(OBJ_DIR)/align.o

all: $(EXECUTABLES)

$(BIN_DIR)/main: $(MAIN_OBJECT) $(COMMON_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(ROOTLIBS)

$(BIN_DIR)/align: $(ALIGN_OBJECT) $(COMMON_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(ROOTLIBS)

# Regole di compilazione
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
