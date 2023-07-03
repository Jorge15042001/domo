CC:=g++
OUTPUT_CONTROLER := controller
OUTPUT_SERVER_MOTOR := motorServer
OUTPUT_SERVER_CAMERA := cameraServer

SRC_DIR_controler := src/controller
OBJ_DIR_controler := build/obj/controller
SRC_FILES_controler := $(wildcard $(SRC_DIR_controler)/*.cpp)
OBJ_FILES_controler := $(patsubst $(SRC_DIR_controler)/%.cpp,$(OBJ_DIR_controler)/%.o,$(SRC_FILES_controler))

SRC_DIR_motorServer := src/motorDriver
OBJ_DIR_motorServer := build/obj/motorDriver
SRC_FILES_motorServer := $(wildcard $(SRC_DIR_motorServer)/*.cpp)
OBJ_FILES_motorServer := $(patsubst $(SRC_DIR_motorServer)/%.cpp,$(OBJ_DIR_motorServer)/%.o,$(SRC_FILES_motorServer))

SRC_DIR_cameraServer := src/cameraDriver
OBJ_DIR_cameraServer := build/obj/cameraDriver
SRC_FILES_cameraServer := $(wildcard $(SRC_DIR_cameraServer)/*.cpp)
OBJ_FILES_cameraServer := $(patsubst $(SRC_DIR_cameraServer)/%.cpp,$(OBJ_DIR_cameraServer)/%.o,$(SRC_FILES_cameraServer))

# CXXFLAGS := -O3 -Wall -Wextra  -fsanitize=address
CXXFLAGS := -Wall -Wextra  -fsanitize=address -std=c++20




$(OBJ_DIR_controler)/%.o: $(SRC_DIR_controler)/%.cpp
	mkdir -p $(OBJ_DIR_controler)
	$(CC)   $(CXXFLAGS)   -c -o $@ $^

$(OUTPUT_CONTROLER): $(OBJ_FILES_controler)
	$(CC)  -o $@ $^   $(CXXFLAGS) -lfmt




$(OBJ_DIR_motorServer)/%.o: $(SRC_DIR_motorServer)/%.cpp
	mkdir -p $(OBJ_DIR_motorServer)
	$(CC)   $(CXXFLAGS)   -c -o $@ $^

$(OUTPUT_SERVER_MOTOR): $(OBJ_FILES_motorServer)
	mkdir -p $(OBJ_DIR_motorServer)
	$(CC)  -o $@ $^   $(CXXFLAGS) -lCppLinuxSerial -lfmt


$(OBJ_DIR_cameraServer)/%.o: $(SRC_DIR_cameraServer)/%.cpp
	mkdir -p $(OBJ_DIR_cameraServer)
	$(CC)   $(CXXFLAGS)   -c -o $@ $^

$(OUTPUT_SERVER_CAMERA): $(OBJ_FILES_cameraServer)
	mkdir -p $(OBJ_DIR_cameraServer)
	$(CC)  -o $@ $^   $(CXXFLAGS) -lCppLinuxSerial

all: $(OUTPUT_SERVER_MOTOR) $(OUTPUT_SERVER_CAMERA) $(OUTPUT_CONTROLER)


clean :
	rm -rf build
	rm $(OUTPUT_CONTROLER)
	rm $(OUTPUT_SERVER_MOTOR)
	rm $(OUTPUT_SERVER_CAMERA)

