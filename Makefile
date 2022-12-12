CC:=gcc
OUTPUT_CONTROLER := controller
OUTPUT_SERVER_MOTOR := motorServer

SRC_DIR_controler := src/controller
OBJ_DIR_controler := build/obj/controller
SRC_FILES_controler := $(wildcard $(SRC_DIR_controler)/*.c)
OBJ_FILES_controler := $(patsubst $(SRC_DIR_controler)/%.c,$(OBJ_DIR_controler)/%.o,$(SRC_FILES_controler))

SRC_DIR_motorServer := src/motorDriver
OBJ_DIR_motorServer := build/obj/motorDriver
SRC_FILES_motorServer := $(wildcard $(SRC_DIR_motorServer)/*.c)
OBJ_FILES_motorServer := $(patsubst $(SRC_DIR_motorServer)/%.c,$(OBJ_DIR_motorServer)/%.o,$(SRC_FILES_motorServer))


CXXFLAGS := -O3 -Wall -Wextra




$(OBJ_DIR_controler)/%.o: $(SRC_DIR_controler)/%.c
	mkdir -p $(OBJ_DIR_controler)
	$(CC)   $(CXXFLAGS)   -c -o $@ $^

$(OUTPUT_CONTROLER): $(OBJ_FILES_controler)
	$(CC)  -o $@ $^   $(CXXFLAGS)




$(OBJ_DIR_motorServer)/%.o: $(SRC_DIR_motorServer)/%.c
	mkdir -p $(OBJ_DIR_motorServer)
	$(CC)   $(CXXFLAGS)   -c -o $@ $^

$(OUTPUT_SERVER_MOTOR): $(OBJ_FILES_motorServer)
	mkdir -p $(OBJ_DIR_motorServer)
	$(CC)  -o $@ $^   $(CXXFLAGS)


all: $(OUTPUT_SERVER_MOTOR) $(OUTPUT_CONTROLER)


clean :
	rm -rf build
	rm $(OUTPUT_CONTROLER)
	rm $(OUTPUT_SERVER_MOTOR)

