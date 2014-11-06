#include <iostream>
#include <cmath>
#include <typeinfo>
#include <memory>
#include <SDL2/SDL.h>
#include "../include/window.h"
#include "../include/system.h"
#include "../include/CS.h"
#include "../include/components.h"
#include "../include/entities.h"

PropertiesComponent::PropertiesComponent(eId id) : Component(id){

}

FuncQComponent::FuncQComponent(eId id) : Component(id){
};

void FuncQComponent::add(void (*f)(eId)){
	functions.push_back(f);
}

void FuncQComponent::addEventFunc(void (*f)(eId, SDL_Event&)){
	eventFunctions.push_back(f);
}

void FuncQComponent::update(){
	for(auto it = functions.begin(); it != functions.end(); ++it){
		(*it)(owner);
	}
}

void FuncQComponent::eventUpdate(SDL_Event &e){
	for(auto it = eventFunctions.begin(); it != eventFunctions.end(); ++it){
		(*it)(owner, e);
	}
}

CollisionComponent::CollisionComponent(std::map<eId, std::shared_ptr<SpriteComponent>> &spriteMap,
		std::map<eId, std::shared_ptr<MoveComponent>> &moveMap, eId id, bool s = true) : Component(id){
	spriteC = spriteMap[id];
	moveC = moveMap[id];
	rect.x = moveC->pos.x;
	rect.y = moveC->pos.y;
	rect.w = spriteC->imgRect.w;
	rect.h = spriteC->imgRect.h;
	debugDraw = false;
	collided = false;
	overlaped = false;
	solid = s;
	moveable = true;
	touching = NONE;
	touchable = ALL;
	collidedWith = 0;
}

CollisionComponent::CollisionComponent(int w, int h,
		std::map<eId, std::shared_ptr<MoveComponent>> &moveMap, eId id, bool s = true) : Component(id){
	spriteC = nullptr;
	rect.w = w;
	rect.h = h;
	moveC = moveMap[id];
	if(moveC->pos.x < CS::worldbounds.x){
		CS::worldbounds.x = moveC->pos.x;
	} else if(moveC->pos.x + rect.w > CS::worldbounds.x + CS::worldbounds.w){
		CS::worldbounds.w = (moveC->pos.x + rect.w) - CS::worldbounds.x;
	}
	if(moveC->pos.y < CS::worldbounds.y){
		CS::worldbounds.y = moveC->pos.y;
	} else if(moveC->pos.y + rect.h > CS::worldbounds.y + CS::worldbounds.h){
		CS::worldbounds.h = (moveC->pos.y + rect.h) - CS::worldbounds.y;
	}
	debugDraw = false;
	collided = false;
	overlaped = false;
	solid = s;
	moveable = true;
	touching = NONE;
	touchable = ALL;
	collidedWith = 0;
}

void CollisionComponent::update(){
	if(moveC->pos.x < CS::worldbounds.x){
		CS::worldbounds.x = moveC->pos.x;
	} else if(moveC->pos.x + rect.w > CS::worldbounds.x + CS::worldbounds.w){
		CS::worldbounds.w = (moveC->pos.x + rect.w) - CS::worldbounds.x;
	}
	if(moveC->pos.y < CS::worldbounds.y){
		CS::worldbounds.y = moveC->pos.y;
	} else if(moveC->pos.y + rect.h > CS::worldbounds.y + CS::worldbounds.h){
		CS::worldbounds.h = (moveC->pos.y + rect.h) - CS::worldbounds.y;
	}
	rect.x = moveC->pos.x;
	rect.y = moveC->pos.y;
	if(spriteC != nullptr){
		rect.w = spriteC->imgRect.w;
		rect.h = spriteC->imgRect.h;
	}
	touching = NONE;
	collided = false;
	overlaped = false;
	overlapingWith.clear();
	collidingWith.clear();
	//CS::qt.insert(owner);
}

void CollisionComponent::postUpdate(){
	float maxArea;
	for(auto it = overlapingWith.begin(); it != overlapingWith.end(); it++){
	}
}

void CollisionComponent::updatePosition(){
	rect.x = moveC->pos.x;
	rect.y = moveC->pos.y;
}

void CollisionComponent::CollideWith(eId e){
	collidingWith.push_back(e);
}

void CollisionComponent::getGridIndex(Grid &g){
	gridIndex = g.getIndex(owner);
}

ControllerComponent::ControllerComponent(std::map<eId, std::shared_ptr<MoveComponent>> &moveMap, eId id) : Component(id){
	moveC = moveMap[id];
}

void ControllerComponent::eventUpdate(SDL_Event &e){
	float SPEED = 1;
	bool shot;
	if (e.type == SDL_KEYDOWN){
		switch(e.key.keysym.sym){
			case SDLK_d:
				CS::getSpriteC(owner)->facing = RIGHT;
				moveC->acc.x = SPEED;break;
			case SDLK_a:
				CS::getSpriteC(owner)->facing = LEFT;
				moveC->acc.x = -SPEED;break;
			case SDLK_w:
				if(CS::getCollisionC(owner)->touching & FLOOR)
					moveC->vel.y = -20;break;	
			case SDLK_s:
				moveC->vel.y = SPEED;break;
		}
	}
	if (e.type == SDL_KEYUP){
		switch(e.key.keysym.sym){
			case SDLK_d:
				if(moveC->acc.x>0)
					moveC->acc.x -= SPEED;break;
			case SDLK_a:
				if(moveC->acc.x<0)
					moveC->acc.x += SPEED;break;
		}
	}
}

SpriteComponent::SpriteComponent(const std::string &file, 
		std::map<eId, std::shared_ptr<MoveComponent>> &moveMap, eId id) : Component(id){
	if(CS::textures[file] == nullptr){

		img = Window::LoadImage(file);
		CS::textures[file] = img;
	}
	else{
		img = CS::textures[file];

	}
	SDL_QueryTexture(img, nullptr, nullptr, &imgRect.w, &imgRect.h);
	
	texSize.x = imgRect.w;
	texSize.y = imgRect.h;
	moveC = moveMap[id];
	imgRect.x = moveC->pos.x;
	imgRect.y = moveC->pos.y;
	color = {255,255,255};
	scale = {1,1};
	offset = {0,0};
	clipRect.w = imgRect.w *= scale.x;
	clipRect.h = imgRect.h *= scale.y;
	clipRect.x = 0;
	clipRect.y = 0;
	//imgRect.w *= 0.5;
	//clipRect.w *= 0.5;
	playingAnimation = false;
	facing = RIGHT;
	layer = 0;
	flip = SDL_FLIP_NONE;
}


void SpriteComponent::draw(){
	SDL_SetTextureColorMod(img, color.r, color.g, color.b);
	Window::Draw(img, imgRect, &clipRect, 0,0,0, flip);
	//SDL_SetTextureColorMod(img, 255, 255, 255);
}

void SpriteComponent::CameraDraw(Vec2 pos, Vec2 size, float zoom, Vec2 gamePos){
	//interpolate();

	SDL_Rect b1, b2, cBounds, area;
	b1 = imgRect;
	b2 = clipRect;
	cBounds.x = pos.x;
	cBounds.y = pos.y;
	cBounds.w = size.x;
	cBounds.h = size.y;
	imgRect.x = (imgRect.x - int(gamePos.x));
	imgRect.y = (imgRect.y - int(gamePos.y));
	if(SDL_IntersectRect(&imgRect, &cBounds, &area)){
		// if(imgRect.x + imgRect.w > pos.x + size.x){
		// 	if((flip & SDL_FLIP_HORIZONTAL) == SDL_FLIP_NONE){
		// 		imgRect.w = area.w;
		// 		clipRect.w = area.w/scale.x;
		// 	}
		// 	else if((flip & SDL_FLIP_HORIZONTAL) == SDL_FLIP_HORIZONTAL){
		// 		clipRect.x += (imgRect.x - (pos.x+size.x))/scale.x;
		// 		imgRect.w = area.w;
		// 		clipRect.w = area.w/scale.x;
		// 	}
		// }
		// else if(imgRect.x < pos.x ){
		// 	if((flip & SDL_FLIP_HORIZONTAL) == SDL_FLIP_NONE){
		// 		clipRect.x += (pos.x - imgRect.x)/(scale.x);
		// 		imgRect.x = area.x;
		// 		imgRect.w = area.w;
		// 		clipRect.w = area.w/scale.x;
		// 	}
		// 	else if((flip & SDL_FLIP_HORIZONTAL) == SDL_FLIP_HORIZONTAL){
		// 		imgRect.x = area.x;
		// 		imgRect.w = area.w;
		// 		clipRect.w = area.w/scale.x;
		// 	}
		// }
		// if(imgRect.y + imgRect.h > pos.y + size.y){
		// 	if((flip & SDL_FLIP_VERTICAL) == SDL_FLIP_NONE){
		// 		imgRect.h = area.h;
		// 		clipRect.h = area.h/scale.y;
		// 	}
		// 	else if((flip & SDL_FLIP_VERTICAL) == SDL_FLIP_VERTICAL){
		// 		clipRect.y += (imgRect.y - (pos.y+size.y))/(scale.y);
		// 		imgRect.h = area.h;
		// 		clipRect.h = area.h/scale.y;
		// 	}
		// }
		// else if(imgRect.y < pos.y ){
		// 	if((flip & SDL_FLIP_VERTICAL) == SDL_FLIP_NONE ){
		// 		clipRect.y += (pos.y - imgRect.y)/scale.y;
		// 		imgRect.y = area.y;
		// 		imgRect.h = area.h;
		// 		clipRect.h = area.h/scale.y;
		// 	}
		// 	else if((flip & SDL_FLIP_VERTICAL) == SDL_FLIP_VERTICAL){
		// 		imgRect.y = area.y;
		// 		imgRect.h = area.h;
		// 		clipRect.h = area.h/scale.y;
		// 	}
		// }
		imgRect.x = round(imgRect.x*zoom);
		imgRect.y = round(imgRect.y*zoom);
		imgRect.h = round(imgRect.h*zoom);
		imgRect.w = round(imgRect.w*zoom);
	
		//Window::Draw(img, imgRect, &clipRect, 0,0,0,flip);
		draw();
	}
	imgRect = b1;
	clipRect = b2;
}

void drawCall(eId id){
	auto c = CS::spriteCS[id];
	c->draw();
}

void SpriteComponent::update(){
	deltaPos.x = imgRect.x;
	deltaPos.y = imgRect.y;
	imgRect.x = moveC->pos.x + offset.x;
	imgRect.y = moveC->pos.y + offset.y;
	if(playingAnimation){
		if(frameTimer < 1.f/currentAnimation.speed && !currentAnimation.played){
			currentAnimation.currentFrame = currentAnimation.frames.at(currentAnimation.currentIt);
			int fY = floor(currentAnimation.currentFrame/(texSize.x/(clipRect.w/scale.x)));
			clipRect.x = currentAnimation.currentFrame*clipRect.w;
			clipRect.y = fY*clipRect.h;
			currentAnimation.currentIt++;
			currentAnimation.played = true;
		}
		else if(frameTimer > 1.f/currentAnimation.speed && currentAnimation.played){
			currentAnimation.played = false;
			frameTimer = 0;
			if(currentAnimation.currentIt == currentAnimation.frames.size())
			{
				currentAnimation.currentIt = 0;
				if(!currentAnimation.loop) {
					playingAnimation = false;
				}
			}
		}
		frameTimer += Timer::dt;
	}
	if(!playingAnimation && currentAnimation.callback != nullptr){
		currentAnimation.callback(owner);
		currentAnimation.callback = nullptr;

	}
	if(facing == LEFT)
	{
		flip = SDL_FLIP_NONE;
	} else if(facing == RIGHT) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	CS::drawCalls[layer][owner] = drawCall;
}

void SpriteComponent::setColor(Uint8 r, Uint8 g, Uint8 b){
	color = {r, g, b};
}

void SpriteComponent::setFrame(int w, int h){
	if(w > imgRect.w || h > imgRect.h){
		return;
	}
	clipRect.w = w;
	clipRect.h = h;
	imgRect.w = clipRect.w*scale.x;
	imgRect.h = clipRect.h*scale.y;
}

void SpriteComponent::setScale(float x, float y){
	int w = clipRect.w, h = clipRect.h;
	scale = {x,y};

	// imgRect.w *= scale.x;
	// imgRect.h *= scale.y;
	setFrame(w, h);
}

void SpriteComponent::playAnimation(std::vector<int> frames, float speed, bool loop, bool force, void (*cb)(eId)){
	if(frames != currentAnimation.frames || force){
		currentAnimation.frames.swap(frames);
		currentAnimation.speed = speed;
		currentAnimation.loop = loop;
		currentAnimation.currentIt = 0;
		currentAnimation.played = false;
		currentAnimation.callback = cb;
		frameTimer = 0;
		playingAnimation = true;
		
	}
}

void SpriteComponent::interpolate(){
	imgRect.x = moveC->pos.x*Timer::alpha + moveC->deltaPos.x*(1.0-Timer::alpha);
	imgRect.y = moveC->pos.y*Timer::alpha + moveC->deltaPos.y*(1.0-Timer::alpha);
//	moveC->vel.x = moveC->vel.x*Timer::alpha + moveC->deltaVel.x*(1.0-Timer::alpha);
//	moveC->vel.y = moveC->vel.y*Timer::alpha + moveC->deltaVel.y*(1.0-Timer::alpha);
}

MoveComponent::MoveComponent(float xx, float yy, eId id) : Component(id) {
	pos = {xx, yy};
	vel = {0.f,0.f};
	acc = {0.f,0.f};
	drag = {0.03,0.03};
	maxV = {0.1,0.1};
	terV = {0.1,0.1};
}

void MoveComponent::update(){
	//vel = {vel.x+acc.x,vel.y+acc.y};
	deltaVel = vel;
	deltaPos = pos;
	if(acc.x != 0 ){
		vel.x += acc.x*Timer::dt;

	}
	else if(drag.x != 0){
		if(vel.x - drag.x*Timer::dt > 0) {
			vel.x = vel.x - drag.x*Timer::dt;
		} else if (vel.x + drag.x*Timer::dt < 0) {
			vel.x += drag.x*Timer::dt;
		} else {
			vel.x = 0;
		}
	}
	if(acc.y != 0){
		vel.y += acc.y*Timer::dt;
	}
	else if(drag.y != 0){
		if(vel.y - drag.y*Timer::dt > 0) {
			vel.y = vel.y - drag.y*Timer::dt;
		} else if (vel.y + drag.y*Timer::dt < 0) {
			vel.y += drag.y*Timer::dt;
		} else {
			vel.y = 0;
		}
	}
	if (vel.x > maxV.x || vel.x < -maxV.x){
		vel.x = (vel.x>0)?maxV.x:-maxV.x;
	}
	if (vel.y > maxV.y || vel.y < -maxV.y){
		vel.y = (vel.y>0)?maxV.y:-maxV.y;
	}
	
	pos.x += vel.x*Timer::dt;
	pos.y += vel.y*Timer::dt;
	//std::cout << vel.y << std::endl;
}

void MoveComponent::interpolate(){
	pos.x = pos.x*Timer::alpha + deltaPos.x*(1.0f-Timer::alpha);
	pos.y = pos.y*Timer::alpha + deltaPos.y*(1.0f-Timer::alpha);
	vel.x = vel.x*Timer::alpha + deltaVel.x*(1.0f-Timer::alpha);
	vel.y = vel.y*Timer::alpha + deltaVel.y*(1.0f-Timer::alpha);
}

void MoveComponent::setPosition(float x, float y){
	//deltaPos = pos;
	pos = {x, y};
}

void MoveComponent::resetVel(){
	vel = {0, 0};
}

Component::Component(eId id) : owner(id){
}


Camera::Camera(float x, float y, float w, float h, float z, eId id) : MoveComponent(0, 0, id) {
	size = {w, h};
	winPos = {x,y};
	winSize = {w,h};
	zoom = z;
	followE = NULL;
}

void Camera::follow(eId id){
	followE = id;
}

Vec2 Camera::getWorldPos(Vec2 p){
	return {((pos.x + p.x/zoom) - winPos.x), ((pos.y + p.y/zoom) - winPos.y)}; 
}

SDL_Rect Camera::getScreenRect(SDL_Rect r){
	SDL_Rect result;
	result.x = floor((r.x - pos.x)*zoom);
	result.y = floor((r.y - pos.y)*zoom);
	result.w = floor(r.w*zoom);
	result.h = floor(r.h*zoom);
	return result;
}

void Camera::update(){
	//pos.x = CS::worldbounds.x;
	//pos.y = CS::worldbounds.y;
	//winSize.x = CS::worldbounds.w;
	//winSize.y = CS::worldbounds.h;
	//zoom = (winSize.y*winSize.x)/(CS::worldbounds.h*CS::worldbounds.w);
	size.x = winSize.x/zoom;
	size.y = winSize.y/zoom;
	if(followE != NULL){
		pos.x = moveCS[followE]->pos.x + CS::collisionCS[followE]->rect.w/2.f - winSize.x/(2.f*zoom);
		pos.y = moveCS[followE]->pos.y + CS::collisionCS[followE]->rect.h/2.f - winSize.y/(2.f*zoom);
	}
	
}

void Camera::interpolate(){
	pos.x = pos.x*Timer::alpha + deltaPos.x*(1.0-Timer::alpha);
	pos.y = pos.y*Timer::alpha + deltaPos.y*(1.0-Timer::alpha);
}