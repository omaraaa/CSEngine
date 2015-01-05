CXX = g++


SDL_LIB = -LC:\SDL\x86_64-w64-mingw32\lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SDL_INCLUDE = -IC:\SDL\x86_64-w64-mingw32\include


LUA_LIB = -LC:\lua\lib -llua
LUA_INCLUDE = -IC:\lua\include
LUAJIT_SRC = -LC:\LuaJIT-2.0.3\src C:\LuaJIT-2.0.3\src\lua51.dll
LUAJIT_SRC32 = -LC:\LuaJIT-2.0.3\32bit\src C:\LuaJIT-2.0.3\32bit\src\lua51.dll
LUAJIT_INCLUDE = -IC:\LuaJIT-2.0.3\src
LUAJIT_INCLUDE32 = -IC:\LuaJIT-2.0.3\32bit\src


CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) $(LUAJIT_INCLUDE)
LDFLAGS = -lmingw32 -mwindows -mconsole $(SDL_LIB) $(LUAJIT_SRC)


SDL_LIB32 = -LC:\SDL\i686-w64-mingw32\lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SDL_INCLUDE32 = -IC:\SDL\i686-w64-mingw32\include
CXXFLAGS32 = -Wall -m32 -c -std=c++11 $(SDL_INCLUDE32) $(LUAJIT_INCLUDE32)
LDFLAGS32 = -lmingw32 -mwindows -mconsole -m32 $(SDL_LIB32) $(LUAJIT_SRC32)


ODIR = obj/
ODIR32 = obj32/
SDIR = src/
IDIR = include/
binDIR = bin/
binDIR32 = bin32/

EXE = bin/game.exe
EXE32 = bin32/game.exe


OBJS := $(patsubst $(SDIR)%.cpp,$(ODIR)%.o,$(wildcard $(SDIR)*.cpp))
OBJS32 := $(patsubst $(SDIR)%.cpp,$(ODIR32)%.o,$(wildcard $(SDIR)*.cpp))
INCLUDES := $(wildcard $(IDIR)*.h)


changeDir = cd


all: $(EXE) build32

# O -> EXE 
$(EXE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

build32: $(EXE32)

$(EXE32): $(OBJS32)
	$(CXX) $(OBJS32) $(LDFLAGS32) -o $@

$(ODIR32)%.o: $(SDIR)%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS32) $< -o $@

# CPP -> O
$(ODIR)%.o: $(SDIR)%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) $< -o $@

# RUN 
run: $(binDIR)game.exe $(OBJS)
	$(changeDir) $(binDIR) & game.exe

run32: $(binDIR32)game.exe $(OBJS32)
	$(changeDir) $(binDIR32) & game.exe	

# CLEAN
clean:
	del obj\*.o && del bin\*.exe && del obj32\*.o && del bin32\*.exe