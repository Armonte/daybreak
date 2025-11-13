# Makefile
# By: Tougafgc
# Date: 13 November 2025
#
# Creates the executables and extensions
# needed for Daybreak.
#

### Defines
CXX       = g++ #i686-w64-mingw32-g++
EXE_FLAGS = -static-libgcc -static-libstdc++
DLL_FLAGS = -shared -fPIC -static-libgcc -static-libstdc++
CPPFLAGS  = -Wall --std=c++17 -m32 -DWINDOWS
EXE_OUT   = out/daybreak.exe
DLL_OUT   = out/daybreak/melty_scheme.dll

SRC_DIR = src
DLL_DIR = dll

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJS  = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRC_FILES))

DLL_FILES = $(wildcard $(DLL_DIR)/*.cpp)
DLL_OBJS  = $(patsubst $(DLL_DIR)/%.cpp, $(DLL_DIR)/%.o, $(DLL_FILES))

### Major processes
all: out $(EXE_OUT) $(DLL_OUT)

out:
	mkdir -p out
	mkdir -p out/daybreak
	cp /mingw32/bin/libwinpthread-1.dll out/libwinpthread-1.dll
	cp crescent/newlisp.dll out/daybreak/newlisp.dll
	cp crescent/newlisp.exe out/daybreak/newlisp.exe

test: copy_mbaa run_daybreak

run_daybreak:
	cd mbaa && ./daybreak.exe

copy_mbaa:
	cp -r out/daybreak mbaa/daybreak
	cp -r out/daybreak.exe mbaa/daybreak.exe

clean:
	rm -rf mbaa/daybreak mbaa/daybreak.exe
	rm -rf out $(SRC_DIR)/*.o $(DLL_DIR)/*.o

### Compilation and linking
$(EXE_OUT): $(SRC_OBJS)
	$(CXX) $(EXE_FLAGS) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

$(DLL_OUT): $(DLL_OBJS)
	$(CXX) $(DLL_FLAGS) $^ -o $@

$(DLL_DIR)/%.o: $(DLL_DIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

