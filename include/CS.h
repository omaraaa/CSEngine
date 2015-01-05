#ifndef CS_H
#define CS_H
//Includes
#include <lua.hpp>



#include <stdexcept>
#include <string>
#include <sstream>
#include <chrono>

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include "../include/system.h"
#include "../include/components.h"
#include "../include/CSLua.h"
#include "../include/system.h"
#include "../include/entities.h"
#include "../include/SimpleIni.h"
#include "../include/LuaBridge/LuaBridge.h"
#include "../include/window.h"
#include "../include/cleanup.h"


class CS {
private:
	lua_State *L;
	std::shared_ptr<Camera> mainCamera;
public:
	CS();
	
};

#endif