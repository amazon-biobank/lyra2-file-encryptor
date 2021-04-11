CXX      	:= -c++
GCC			:= -gcc
CXXFLAGS 	:= -g
BUILD_DIR   := ./build
OBJ_DIR  	:= $(BUILD_DIR)/objects
TARGET   	:= OurCypher
INCLUDES 	:= -I./include/ -I/usr/include/openssl/ -I./include/sse/
LIBRARIES 	:= -L.lib/
SRC      	:=           		 			\
	$(wildcard src/*.cpp) 		 			\
	$(wildcard src/Lyra2/*.c) 			\
	$(wildcard src/HashWrapper/*.cpp) 		\
	$(wildcard src/AES/*.cpp) 				\

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
LINKER_LIBS := -lcrypto -lssl

all: build $(BUILD_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_LIBS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(GCC) $(CXXFLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_LIBS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $^ $(LIBRARIES) $(LINKER_LIBS)

PHONY: all build clean debug release

build:
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*