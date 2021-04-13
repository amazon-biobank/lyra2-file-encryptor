CXX      	:= -c++
GCC			:= -gcc
CXXFLAGS 	:= -c
BUILD_DIR   := build
OBJ_DIR  	:= $(BUILD_DIR)/objects
TARGET   	:= lyra2encryptor
LIB_TARGET	:= src/Lyra2FileEncryptor/Lyra2FileEncryptor.o
LIB_NAME	:= libLyra2FileEncryptor.a
BIN_INSTALL_PATH := /usr/local/bin
LIB_INSTALL_PATH := /usr/local/lib
INC_INSTALL_PATH := /usr/local/include
INC_DIR		:= include
INC_TARGET	:= Lyra2FileEncryptor.h
INCLUDES 	:= -I./include/ -I/usr/include/openssl/ -I./include/sse/ -I/usr/include/json/
LIBRARIES 	:= -L.lib/ -L/usr/lib/jsoncpp
CPPLIBSRC      	:=           		 		\
	$(wildcard src/HashWrapper/*.cpp) 		\
	$(wildcard src/FileAux/*.cpp) 			\
	$(wildcard src/Lyra2FileEncryptor/*.cpp)\
	$(wildcard src/AES/*.cpp) 				\

CPPSRC      	:=           		 		\
	$(wildcard src/*.cpp) 		 			\
	$(wildcard src/HashWrapper/*.cpp) 		\
	$(wildcard src/FileAux/*.cpp) 			\
	$(wildcard src/Lyra2FileEncryptor/*.cpp)\
	$(wildcard src/AES/*.cpp) 				\

CSRC      	:=           		 			\
	$(wildcard src/Lyra2/*.c) 				\

COBJ := $(CSRC:%.c=$(OBJ_DIR)/%.o)
CPPOBJ := $(CPPSRC:%.cpp=$(OBJ_DIR)/%.o) 
OBJ := $(COBJ) $(CPPOBJ) 
LINKER_LIBS := -lcrypto -lssl -ljsoncpp

LIBCPPOBJ := $(CPPLIBSRC:%.cpp=$(OBJ_DIR)/%.o) 
LIBOBJ := $(COBJ) $(LIBCPPOBJ) 

all: build $(BUILD_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_LIBS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(GCC) $(CXXFLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_LIBS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CXX) -fopenmp -o $@ $^ $(LIBRARIES) $(LINKER_LIBS)

$(BUILD_DIR)/%.o: $(LIBOBJ)
	@mkdir -p $(@D)

PHONY: all build clean debug release install-bin

install-bin: build $(BUILD_DIR)/$(TARGET)
	-@mv $(BUILD_DIR)/$(TARGET) $(BIN_INSTALL_PATH)/$(TARGET)

install-lib: build $(LIBOBJ)
	-@ar rcs $(BUILD_DIR)/$(LIB_NAME) $(LIBOBJ)
	-@mv $(BUILD_DIR)/$(LIB_NAME) $(LIB_INSTALL_PATH)/$(LIB_NAME)
	-@cp $(INC_DIR)/$(INC_TARGET) $(INC_INSTALL_PATH)/$(INC_TARGET)

uninstall-bin:
	-@rm $(BIN_INSTALL_PATH)/$(TARGET)

uninstall-lib:
	-@rm $(LIB_INSTALL_PATH)/$(LIB_NAME)
	-@rm $(INC_INSTALL_PATH)/$(INC_TARGET)

build-lib:
	@mkdir -p $(OBJ_DIR)

build:
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*