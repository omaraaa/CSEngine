
#include <lua.hpp>



#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include "../include/window.h"
#include "../include/cleanup.h"
#include "../include/CS.h"
#include "../include/system.h"
#include "../include/entities.h"
#include "../include/SimpleIni.h"
#include "../include/LuaBridge/LuaBridge.h"


using namespace std;
using namespace luabridge;

lua_State *L;


void foo () { cout << "TEST" << endl; }
Vec2 createVec2(float x, float y){
	Vec2 v = {x, y};
	return v;
}


// void foo(){
// 	cout << "TEST" << endl;
// }
std::string command = "";
bool consoleOpen = false;
eId c;

void clear(){
	CS::clear();
	c = createCamera(0,0);
}

void setPos(eId id, Vec2 p){
	moveCS[id]->pos = p;
}

Vec2 getMousePos(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vec2 v{x, y};
	return v;
}

double time_in_seconds(){
	auto t = chrono::high_resolution_clock::now();
	return chrono::duration_cast<
			chrono::duration<double>>(t.time_since_epoch()).count();
}
double timeMs(){
	auto t = chrono::high_resolution_clock::now();
	return chrono::duration_cast<
			chrono::duration<double, std::milli>>(t.time_since_epoch()).count();
}

void setLua(lua_State *L){
	getGlobalNamespace (L)
  .beginNamespace ("game")
	.beginClass<Vec2> ("Vec2")
		.addData("x", &Vec2::x)
		.addData("y", &Vec2::y)
	.endClass()
	.beginClass<MoveComponent> ("MoveComponent")
		.addData("pos", &MoveComponent::pos)
		.addData("vel", &MoveComponent::vel)
		.addData("acc", &MoveComponent::acc)
		.addData("maxVel", &MoveComponent::maxV)
		.addData("terVel", &MoveComponent::terV)
	.endClass()
   .endNamespace ()
    .addFunction ("mBox", mBox)
    .addFunction ("player", &TEST)
    .addFunction ("clear", clear)
    .addFunction("setPos", &setPos)
    .addFunction("getMousePos", &getMousePos)
    .addFunction ("createVec2", &createVec2);
}

int main(int argc, char **argv){
	L = lua_open();
	luaL_openlibs(L);
	setLua(L);

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile("../config.ini");
	const char* title = ini.GetValue("window","title",NULL);
	bool fullscreen = strToBool(ini.GetValue("window","fullscreen",NULL));
	P_firerate = ::atof(ini.GetValue("player","firerate",NULL));
	try {
		Window::Init(title, fullscreen);
	}
	catch (const std::runtime_error &e){
		std::cout << e.what() << std::endl;
		Window::Quit();
		return -1;
	}
	SDL_Event e;
	bool quit=false;
	int controll = 1;
	c = createCamera(0,0);
	SDL_Rect textrect = {0,0,400,100};
	SDL_StartTextInput();
	std::string text = "";
	if(luaL_loadfile(L, "../scripts/test.lua")
    	|| lua_pcall(L,0,0,0)){
		cout << "FAILED TO LOAD LUA SCRIPT" << endl;
	}

	Timer::currentTime = time_in_seconds();
	while(!quit){
		text = "";
		double newTime = time_in_seconds();
		double frameTime = newTime - Timer::currentTime;
		Timer::currentTime = newTime;
		Timer::accumulator += frameTime;
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT)
				quit = true;
			if(e.type == SDL_MOUSEWHEEL){
				CS::cameras[c]->zoom += 0.1*e.wheel.y;
			}
			if(e.type == SDL_MOUSEBUTTONDOWN){
				if(e.button.button == SDL_BUTTON_LEFT){
					Vec2 p = CS::cameras[c]->getWorldPos({e.button.x, e.button.y});
					collisionChecker(p.x, p.y, createBox);
				}
				if(e.button.button == SDL_BUTTON_RIGHT){
					if(controll < 0)
							ENEMY(CS::cameras[c]->getWorldPos({e.button.x, e.button.y}));
					else
						CS::cameras[c]->follow(
							TEST(CS::cameras[c]->getWorldPos({e.button.x, e.button.y})));
				}
				
			}
			if(e.type == SDL_MOUSEBUTTONUP){
			}
			if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;break;
					case SDLK_F1:
						consoleOpen = !consoleOpen;break;
					case SDLK_BACKSPACE:
						if(consoleOpen){
							command.erase(command.end()-1);
							cout << '\b';
						}
						break;
					case SDLK_RETURN:
						if(consoleOpen){
							luaL_dostring(L, command.c_str());
							cout << endl;
							command = "";
							consoleOpen = false;
						}
						break;
				}
			}
			if (e.type == SDL_KEYUP){
				switch(e.key.keysym.sym){
					case SDLK_e:
						controll *= -1;
						break;
				}
			}
			if(e.type == SDL_TEXTINPUT && consoleOpen){
				text = e.text.text;
				command += text;
			}
			if(!consoleOpen)
				CS::eventUpdate(e);
		}
		if(text != ""){
			std::cout << text;
			

		}
		Window::Clear();
		Timer::t = 0;
		while(Timer::accumulator >= Timer::dt)
		{
			if(!consoleOpen)
				CS::update();
			Timer::t += Timer::dt;
			Timer::accumulator -= Timer::dt;
			CS::cleanup();
		}
		Timer::alpha = Timer::accumulator / Timer::dt;
		CS::draw();
		Window::Present();
	}

	Window::Quit();
	lua_close(L);
	return 0;
}