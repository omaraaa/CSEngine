#ifndef LUA_SYSTEM_H
#define LUA_SYSTEM_H

//STD includes
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>

//SDL includes
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>

//My includes
#include "../include/CS.h"
#include "../include/system.h"
#include "../include/entities.h"


//Lua includes
#include <lua.hpp>
#include "../include/LuaBridge/LuaBridge.h"

//luabridge namespace
using namespace luabridge;
using namespace std;


//basic functions for lua scripting
void foo();
Vec2 createVec2(float x, float y);

void setPos(eId id, Vec2 p);

Vec2 getMousePos();

MoveComponent* getEntityMove(eId id);

void setMoveC(MoveComponent* mc);

void setSeed(int seed);

void cameraFollow(eId id);



//LuaSystem
namespace LS {

	void init_LS();
	void init_LuaBridge(lua_State *L);
	void loadScript(const char* path);
	void runCommand(const char* command);
}

#endif