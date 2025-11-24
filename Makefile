# Makefile
# By: tougafgc
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
DLL_OUT   = out/daybreak/daybreak.dll

SRC_DIR = src

SRC_FILES = $(filter-out dll.cpp, $(wildcard $(SRC_DIR)/*.cpp))
SRC_OBJS  = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SRC_FILES))

DLL_FILES = $(SRC_DIR)/dll.cpp
DLL_OBJS  = $(SRC_DIR)/dll.o

.PHONY: out

### Major processes
all: $(EXE_OUT) $(DLL_OUT) out

out:
	cp /mingw32/bin/libwinpthread-1.dll out/libwinpthread-1.dll
	cp crescent/newlisp.dll  out/daybreak/newlisp.dll
	cp crescent/newlisp.exe  out/daybreak/newlisp.exe
	cp crescent/server.lsp out/daybreak/server.lsp
	cp crescent/client.lsp out/daybreak/client.lsp
	./crescent/newlisp.exe -x crescent/client.lsp out/daybreak/client.exe
	cp -r out/daybreak mbaa/daybreak
	cp -r out/daybreak.exe mbaa/daybreak.exe

test:
	cd mbaa && ./daybreak.exe #./mbaa/daybreak/client.exe

clean:
	rm -rf mbaa/daybreak mbaa/daybreak.exe
	rm -rf out $(SRC_DIR)/*.o

### Compilation and linking
$(EXE_OUT): $(SRC_OBJS)
	mkdir -p out
	mkdir -p out/daybreak
	$(CXX) $(EXE_FLAGS) $^ -o $@

$(DLL_OUT): $(DLL_OBJS)
	$(CXX) $(DLL_FLAGS) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@
