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
#include "../include/luaSystem.h"
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
void foo() { cout << "TEST" << endl; }
Vec2 createVec2(float x, float y){
	Vec2 v = { x, y };
	return v;
}

void setPos(eId id, Vec2 p){
	moveCS[id]->pos = p;
}

Vec2 getMousePos(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vec2 v{ x, y };
	return CS::cameras[1]->getWorldPos(v);
}

MoveComponent* getEntityMove(eId id){
	return moveCS[id].get();
}

void setMoveC(MoveComponent* mc){
	shared_ptr<MoveComponent> sp(mc);
	moveCS[mc->owner] = sp;
}

void setSeed(int seed){
	srand(seed);
}

void cameraFollow(eId id){
	CS::cameras[1]->follow(id);
}


namespace LS {
	lua_State* L;
	void init_LS(){
		L = lua_open();
		luaL_openlibs(L);

		LS::init_LuaBridge(L);
	}

	void init_LuaBridge(lua_State *L){
		getGlobalNamespace(L)
			.beginClass<Vec2>("Vec2")
			.addProperty("x", &Vec2::getX, &Vec2::setX)
			.addProperty("y", &Vec2::getY, &Vec2::setY)
			.endClass()
			.beginClass<MoveComponent>("MoveComponent")
			.addData("pos", &MoveComponent::pos)
			.addData("vel", &MoveComponent::vel)
			.addData("acc", &MoveComponent::acc)
			.addData("maxVel", &MoveComponent::maxV)
			.addData("terVel", &MoveComponent::terV)
			.endClass()
			.beginNamespace("CS")
			.endNamespace()
			.addFunction("mBox", mBox)
			.addFunction("player", &TEST)
			//.addFunction("clear", clear)
			.addFunction("setPos", &setPos)
			.addFunction("getMousePos", &getMousePos)
			.addFunction("getMC", &getEntityMove)
			.addFunction("setMC", &setMoveC)
			.addFunction("follow", cameraFollow)
			.addFunction("setSeed", setSeed)
			.addFunction("createVec2", &createVec2);
	}

	void loadScript(const char* path){
		if (luaL_loadfile(L, path)
			|| lua_pcall(L, 0, 0, 0)){
			cout << "FAILED TO LOAD LUA SCRIPT" << endl;
		}
	}

	void runCommand(const char* command) {
		luaL_dostring(L, command);
	}
}