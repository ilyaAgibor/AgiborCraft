CXX = g++
CXXFLAGS = -std=c++11 -mconsole -g -m64 $(INC_DIRS)
LIBS = $(LIB_DIRS) $(LIBRARIES)

SRC_DIR = ./src
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/objects
EXECUTABLE = ./build/AgiborCraft.exe

INC_DIRS = -I./glew/include
LIB_DIRS = -L./glew/lib/Release/x64
LIBRARIES = -lglew32 -lws2_32 -lopengl32 -lgdi32

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(OBJ_DIR) $(EXECUTABLE) copy_dll

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include stb_image.cpp in the compilation process
$(OBJ_DIR)/stb_image.o: $(SRC_DIR)/stb/stb_image.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ_FILES) $(OBJ_DIR)/stb_image.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

copy_dll:
	@echo Copying GLEW DLL...
	@cp -f ./glew/bin/Release/x64/glew32.dll $(BUILD_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)

.PHONY: all clean copy_dll