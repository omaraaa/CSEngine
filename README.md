CSEngine
========
My own personal engine made for educational purposes. The development of this project is inactive, for I got new projects to work on. I learned a great deal of concepts and gained experience from making this engine, so it was worth the effort.

Components:
-----------
* MoveComponent
>Component that handles position, velocity, and acceleration

* SpriteComponent
>Component that handles sprites and drawing

* CollisionComponent
>Component that handles collision

* PropsComponent
>Handles properties

* FuncQComponent
>handles function queues

* Camera
>Camera with zoom, position, and size in addition to screen position and screen size

Lua Commands:
-------------
Press F1 to type a command

* createVec2(x, y) - creates a 2d Vector
* setPos(entity id, vec2) - sets the position of an entity
* createBoxes(x, y, no. boxes) - creates a row of boxes
* getMousePos() - return Vec2 of mouse pos
* mBox(x, y) - creates a box
* player(Vec2) - creates a player
* follow(ID) - camera follow entity provided
* getMC(ID) - gets movement component
* setMC(ID) - sets movement component

TODO:
-----
* none

notes
-----
[TwinklebearDev SDL 2.0 Tutorial] - awesome SDL 2 tutorial

Verion
------
0.3

[TwinklebearDev SDL 2.0 Tutorial]:http://www.willusher.io/pages/sdl2/
