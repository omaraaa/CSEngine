CXX = g++
SDL_LIB = -LD:\SDL\x86_64-w64-mingw32\lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SDL_INCLUDE = -ID:\SDL\x86_64-w64-mingw32\include
#LUA_LIB = -LC:\lua\lib -llua
LUA_LIB = -LD:\lua\lib -llua
LUA_INCLUDE = -ID:\lua\include
LUAJIT_SRC = -LD:\LuaJIT-2.0.3\src D:\LuaJIT-2.0.3\src\lua51.dll
LUAJIT_SRC32 = -LD:\LuaJIT-2.0.3\src D:\LuaJIT-2.0.3\src\lua51.dll
LUAJIT_INCLUDE = -ID:\LuaJIT-2.0.3\src
LUAJIT_INCLUDE32 = -ID:\LuaJIT-2.0.3\src
# SGS_INCLUDE = -IC:\sgscript\src -IC:\sgscript\ext
# SGS_LIB = -Lbin -lsgscript
SDL_LIB32 = -LD:\SDL\i686-w64-mingw32\lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SDL_INCLUDE32 = -ID:\SDL\i686-w64-mingw32\include
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) $(LUAJIT_INCLUDE)
CXXFLAGS32 = -Wall -m32 -c -std=c++11 $(SDL_INCLUDE32) $(LUAJIT_INCLUDE32)
LDFLAGS = -lmingw32 -mwindows -mconsole $(SDL_LIB) $(LUAJIT_SRC)
LDFLAGS32 = -lmingw32 -mwindows -mconsole -m32 $(SDL_LIB32) $(LUAJIT_SRC32)
ODIR = obj/
ODIR32 = obj32/
SDIR = src/
IDIR = include/
EXE = bin/game.exe
EXE32 = bin32/game.exe
binDIR = bin/
binDIR32 = bin32/
changeDir = cd 
OBJS := $(patsubst $(SDIR)%.cpp,$(ODIR)%.o,$(wildcard $(SDIR)*.cpp))
OBJS32 := $(patsubst $(SDIR)%.cpp,$(ODIR32)%.o,$(wildcard $(SDIR)*.cpp))
INCLUDES := $(wildcard $(IDIR)*.h)

all: $(EXE)

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