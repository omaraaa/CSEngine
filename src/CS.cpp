#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include "../include/window.h"
#include "../include/system.h"
#include "../include/CS.h"
#include "../include/components.h"
#include "../include/entities.h"

using namespace luabridge;

int ComponentSystem::init(){
	initLua();
	initConfigIni();

	initWindow();
	SDL_StartTextInput();
}

void ComponentSystem::initLua(){
	L = lua_open();
	luaL_openlibs(L);
	setLua(L);
}

void ComponentSystem::initConfigIni(){
	CSimpleIniA ini;
	ini.SetUnicode();
	// ini.LoadFile("../config.ini");
	// const char* title = ini.GetValue("window","title",NULL);
	// bool fullscreen = strToBool(ini.GetValue("window","fullscreen",NULL));
}

void ComponentSystem::initWindow(){
	try {
		Window::Init(title, fullscreen);
	}
	catch (const std::runtime_error &e){
		std::cout << e.what() << std::endl;
		Window::Quit();
		return -1;
	}
}

void ComponentSystem::update(){

}