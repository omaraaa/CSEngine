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
CS_Start(...);
```
Initializes the game and call the game's main loop.

```c++
CS_Init(...);
```
Initializes the game engine. doesn't call the game's main game loop.

#####Quit
```
CS_Quit();
```
Called to quit.

#####Update
```
CS_run();
```
runs the game's main loop.
```
CS_update();
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



