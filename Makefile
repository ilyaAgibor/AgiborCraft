CXX = g++
CXXFLAGS = -std=c++11 -mconsole -g -m64 $(INC_DIRS)
LIBS = $(LIB_DIRS) $(LIBRARIES)

SRC_DIR = ./src
BUILD_DIR = ./build
EXECUTABLE = ./build/AgiborCraft.exe

INC_DIRS = -I./glew/include
LIB_DIRS = -L./glew/lib/Release/x64
LIBRARIES = -lglew32 -lws2_32 -lopengl32 -lgdi32

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

all: copy_dll $(EXECUTABLE)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include stb_image.cpp in the compilation process
$(BUILD_DIR)/stb_image.o: $(SRC_DIR)/stb/stb_image.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ_FILES) $(BUILD_DIR)/stb_image.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

copy_dll:
	@echo Copying GLEW DLL...
	@cp -f ./glew/bin/Release/x64/glew32.dll $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXECUTABLE)

.PHONY: all clean copy_dll