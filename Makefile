CXX      	:= -c++
GCC			:= -gcc
CXXFLAGS 	:= -g -c
BUILD_DIR   := build
OBJ_DIR  	:= $(BUILD_DIR)/objects
TARGET   	:= OurCypher
INCLUDES 	:= -I./include/ -I/usr/include/openssl/ -I./include/sse/
LIBRARIES 	:= -L.lib/
CPPSRC      	:=           		 			\
	$(wildcard src/*.cpp) 		 			\
	$(wildcard src/HashWrapper/*.cpp) 		\
	$(wildcard src/AES/*.cpp) 				\

CSRC      	:=           		 			\
	$(wildcard src/Lyra2/*.c) 				\

COBJ := $(CSRC:%.c=$(OBJ_DIR)/%.o)
CPPOBJ := $(CPPSRC:%.cpp=$(OBJ_DIR)/%.o) 
OBJ := $(COBJ) $(CPPOBJ) 
LINKER_LIBS := -lcrypto -lssl

all: build $(BUILD_DIR)/$(TARGET)


$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_LIBS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(GCC) $(CXXFLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_LIBS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CXX) -g -fopenmp -o $@ $^ $(LIBRARIES) $(LINKER_LIBS)

PHONY: all build clean debug release

build:
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*