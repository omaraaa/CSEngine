	
//#include <lua.hpp>

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
#include "../include/window.h"
#include "../include/cleanup.h"
#include "../include/CS.h"
#include "../include/system.h"
#include "../include/entities.h"
#include "../include/luaSystem.h"

//SimpleIni includes
#include "../include/SimpleIni.h"

using namespace std;
using namespace LS;

std::string command = "";
unsigned int cursor = 0;
bool consoleOpen = false;
eId c; //camera id

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





void clear(){
	CS::clear();
	c = createCamera(0, 0);
}

int main(int argc, char **argv){
	LS::init_LS();

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
	bool createboxes=false;
	int controll = 1;
	eId boxCreater = 0;
	c = createCamera(0,0);
	SDL_Rect textrect = {0,0,400,100};
	SDL_StartTextInput();
	std::string text = "";
	
	LS::loadScript("../scripts/test.lua");

	//LuaRef update = getGlobal(L, "update");
	// CS::funcQCS[5]->add(update);
	Timer::currentTime = time_in_seconds();
	while(!quit){
		text = "";
		double newTime = time_in_seconds();
		double frameTime = newTime - Timer::currentTime;
		Timer::currentTime = newTime;
		Timer::accumulator += frameTime;
		while (SDL_PollEvent(&e)){
			if(!consoleOpen)
				CS::eventUpdate(e);
			
			if (e.type == SDL_QUIT)
				quit = true;
			if(e.type == SDL_MOUSEWHEEL){
				CS::cameras[c]->zoom += 0.025*e.wheel.y;
			}
			if(e.type == SDL_MOUSEBUTTONDOWN){
				if(e.button.button == SDL_BUTTON_LEFT){
					createboxes = true;
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
				if(e.button.button == SDL_BUTTON_LEFT){
					createboxes = false;
				}
			}
			if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){
					case SDLK_ESCAPE:
						quit = true;break;
					case SDLK_F1:
						consoleOpen = !consoleOpen;break;
					case SDLK_BACKSPACE:
						if(consoleOpen && cursor > 0){
							command.erase(command.end()-1);
							cursor--;
							cout << "\b \b";
						}
						break;
					case SDLK_RETURN:
						if(consoleOpen){
							LS::runCommand(command.c_str());
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
				cursor++;
			}
			
		}
		if(text != ""){
			std::cout << text;
			

		}
		Window::Clear();
		Timer::t = 0;
		
		while(Timer::accumulator >= Timer::dt)
		{
			if(createboxes){
				Vec2 p = getMousePos();
				if(moveCS.find(boxCreater) == moveCS.end())
					boxCreater = collisionChecker(p.x, p.y, createBox);
			}
			if(!consoleOpen)
				CS::update();

			Timer::t += Timer::dt;
			Timer::accumulator -= Timer::dt;
			CS::cleanup();

		}
		Timer::alpha = Timer::accumulator / Timer::dt;
		CS::draw();
		std::ostringstream ss;
		ss << Timer::elapsed;
		std::string s(ss.str());
		SDL_Texture* tex = Window::RenderText(command, "../data/fonts/PressStart2P.ttf", {255,255,255}, 13);
		SDL_Rect r = {0,0};
		SDL_QueryTexture(tex, NULL, NULL, &r.w, &r.h);
		Window::Draw(tex, r);
		Window::Present();
		
	}

	Window::Quit();
	return 0;
}