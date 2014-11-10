CS Engine Specs
===============

Features
--------
* flixable
* lua console
* .ini support

Main Functions
--------------
#####Initializing and starting
```c++
void CS_Start(...);
```
Initializes the game and call the game's main loop.

```c++
void CS_Init(...);
```
Initializes the game engine. doesn't call the game's main game loop.

#####Quit
```
void CS_Quit();
```
Called to quit.

#####Update
```
void CS_run();
```
runs the game's main loop.
```
void CS_update();
```
updates 1 frame. Used if you set up your own game loop.

Components Functions
--------------------
#####Adding components
```c++
ID CS_AddComponent<ComponantClass T>(ID id, args...);
```
or
```c++
ID CS_AddComponent<ComponantClass T>(T c);
```
adds a component to the system.

#####Getting components
```
shared_ptr<T> CS_GetC<ComponentClass T>(ID id);
```
Gets the entity's component and returns a std::shared_ptr.

System Functions
----------------
#####Drawing
```
void CS_Draw(args...);
```
System's draw call.

#####Lua
```
void CS_LuaInit(args...);
```
Sets Lua up.

```
void CS_runScript(args...);
```
Loads a Lua script and excutes it.

```
void CS_LuaFunc(args...);
```
Passes a lua funtion or command to run.

```
void CS_OpenConsole();
```
Opens console.

```
void CS_CloseConsole();
```
Closes console.

Usage Example
=============
main.cpp
```c++
int main(){
	CS_Start();
	return 0;
}
```

game.lua
```lua
function playerUpdate(id)
	
end

function player(x, y)
	id = CS_CreateEntity();
	CS_CreateMoveC(id, x, y);
	CS_CreateSpriteC(id, "../data/test.png");
	CS_getSprite(id).blsh blah blah
end

```

Concepts
========

Pushing draw functions
---
Each entity can push draw functions to it's layer.
```c++
std::map<int layer, std::map<ID, void (*)(ID)>> drawQueue;
```
or(chose the top one)
```c++
std::map<ID, void (*)(ID)> bgDrawQ;
std::map<ID, void (*)(ID)> mainDrawQ;
std::map<ID, void (*)(ID)> fgDrawQ;
```

Camera
---

#####Camera
```
Camera:
	window bounds;
	game bounds;
```

* A camera has a window bounds and a game bounds.
* A main camera is the camera that spriteComponents default to if no cameras are assinged to them.
* camera size in game world changes according to zoom level and camera veiw size.