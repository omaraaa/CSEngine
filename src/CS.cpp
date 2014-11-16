#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include "../include/window.h"
#include "../include/system.h"
#include "../include/CS.h"
#include "../include/components.h"
#include "../include/entities.h"




//CS INIT//
eId CS::_E_INDEX{0};
eId CS::_C_INDEX{0};
SDL_Rect CS::worldbounds = {0,0,0,0};
std::map<eId, std::shared_ptr<MoveComponent>> moveCS{};
std::map<eId, std::shared_ptr<SpriteComponent>> CS::spriteCS{};
std::map<eId, std::shared_ptr<ControllerComponent>> CS::controllerCS{};
std::map<eId, std::shared_ptr<CollisionComponent>> CS::collisionCS{};
std::map<eId, std::shared_ptr<PropertiesComponent>> CS::propCS{};
std::map<eId, std::shared_ptr<FuncQComponent>> CS::funcQCS{};
std::map<const std::string, std::vector<eId>> CS::groups{};
std::map<const std::string, SDL_Texture*> CS::textures{};
std::map<const int, std::map<eId, void (*)(eId)>> CS::drawCalls;
std::map<eId, std::shared_ptr<Camera>> CS::cameras{};
std::vector<eId> CS::deletedEntities{};
std::vector<eId> CS::toDelete{};
//QuadTree CS::qt(0,0,800,600);
//CS INIT//

//CS CREATE//
eId CS::createEntityID(){
	if(deletedEntities.empty()){
		_E_INDEX++;
		return _E_INDEX; 
	}
	eId id = deletedEntities.back();
	deletedEntities.pop_back();
	return id;
}

eId CS::createCameraID(){
	_C_INDEX++;
	return _C_INDEX;
}

void CS::createMoveC(float xx, float yy, const eId &id){
	moveCS[id] = std::shared_ptr<MoveComponent>(new MoveComponent(xx, yy, id));
}

void CS::updateEntity(eId id){
	if(moveCS[id])
		moveCS[id]->update();
	if(CS::spriteCS[id])
		CS::spriteCS[id]->update();
	if(CS::collisionCS[id])
		CS::collisionCS[id]->update();
	if(CS::funcQCS[id])
		CS::funcQCS[id]->update();
}
//CS CREATE//


void CS::clear(){
	moveCS.clear();
	CS::spriteCS.clear();
	CS::drawCalls.clear();
	CS::controllerCS.clear();
	CS::collisionCS.clear();
	CS::funcQCS.clear();
	CS::propCS.clear();
	CS::cameras.clear();
	deletedEntities.clear();
	CS::groups.clear();
	CS::_E_INDEX = 0;
	CS::_C_INDEX = 0;
	worldbounds = {0,0,0,0};
}

Grid CS::grid(0,0,800,600,100);
int nc=0;
void CS::collisionUpdate(){
	//qt.clear();
	//qt.updateBounds(&worldbounds);
	//std::cout << "CRASHED?!1" << std::endl;
	grid.updateBounds(&worldbounds);
	grid.clear();
	for(auto checking = collisionCS.begin(); checking != collisionCS.end(); checking++){
		//qt.insert(checking->first);
		checking->second->getGridIndex(grid);
	}
	Rect area;
	int n=0;
	//std::cout << "CRASHED?!2" << std::endl;

	for(auto checking = collisionCS.begin(); checking != collisionCS.end(); checking++){
		if(!CS::collisionCS[checking->first]->moveable)
			continue;
		std::vector<eId> entities;
		std::map<float, eId> overlapedMap;
		//if(!qt.getObject(entities, checking->first))
		//	continue;
		//std::map<eId, float> areas;
		float maxArea=0;
		eId maxAreaID;
		bool collided = false;
		entities = grid.getEntities(checking->second->gridIndex);
		for (auto it = entities.begin(); it != entities.end(); ++it){
			if(*it == checking->first)
				continue;
			if(!CS::collisionCS[*it]->moveable && !CS::collisionCS[checking->first]->moveable)
				continue;
			if(checkOverlap(checking->first, *it, &area)){
				checking->second->overlapingWith.push_back(*it);
				collisionCS[*it]->overlapingWith.push_back(checking->first);
				checking->second->overlaped = true;
				CS::collisionCS[*it]->overlaped = true;
				if(CS::collisionCS[*it]->solid && CS::collisionCS[checking->first]->solid){
					checking->second->collided = true;
					CS::collisionCS[*it]->collided = true;
					collided = true;
					overlapedMap[area.w*area.h] = *it;
				}
			}
		}
		for(auto it = overlapedMap.rbegin(); it != overlapedMap.rend(); ++it)
			collide(checking->first, it->second);
		overlapedMap.clear();
	}
	//qt.draw();
	//grid.draw();
	//Window::DrawRect(&CS::worldbounds, 255, 0, 0);
	if(nc != n)
	std::cout << n << std::endl;
	nc = n;
}

void CS::eventUpdate(SDL_Event &e){
	for(auto it = funcQCS.begin(); it != funcQCS.end(); it++){
		it->second->eventUpdate(e);
	}
}

void CS::update(){
	
	for(auto it = moveCS.begin(); it != moveCS.end(); it++){
		it->second->update();
	}
	for(auto it = collisionCS.begin(); it != collisionCS.end(); it++){
		it->second->update();
	}
	CS::collisionUpdate();
	for(auto it = collisionCS.begin(); it != collisionCS.end(); it++){
		it->second->postUpdate();
	}
	for(auto it = spriteCS.begin(); it != spriteCS.end(); it++){
		it->second->update();
	}
	CS::cameraUpdate();
	for(auto it = funcQCS.begin(); it != funcQCS.end(); it++){
		it->second->update();
	}
}

void CS::cameraUpdate(){
	for(auto it = cameras.begin(); it != cameras.end(); it++){
			it->second->update();
	}
}

void CS::interpolate(){
	for(auto it = moveCS.begin(); it != moveCS.end(); it++){
		it->second->interpolate();
	}
}

void CS::draw(){
	
	for(auto it = drawCalls.begin(); it != drawCalls.end(); it++){
		std::map<eId, void(*)(eId)> dmap = it->second;
		if(dmap.size() == 0){
			it = drawCalls.erase(it);
		}
		for(auto it2 = dmap.begin(); it2 != dmap.end(); it2++){
			void (*drawcall)(eId) = it2->second;
			if(CS::spriteCS.find(it2->first) != CS::spriteCS.end())
				drawcall(it2->first);
		}
	}
	for(auto it = collisionCS.begin(); it != collisionCS.end(); it++){
		if(it->second->debugDraw)
			for(auto cIt = cameras.begin(); cIt != cameras.end(); cIt++){
				SDL_Rect r = cameras.begin()->second->getScreenRect(it->second->rect.getSDLRect());
				Window::DrawRect(&r, 100, 150, 100);
			}
	}
}

void CS::deleteEntity(eId id){
	toDelete.push_back(id);
}

void CS::cleanup(){
	for(auto it = toDelete.begin(); it != toDelete.end();){
		moveCS.erase(*it);
		//if(spriteCS.find(*it) != spriteCS.end())
		//drawCalls[spriteCS.at(*it)->layer].erase(*it);
		CS::spriteCS.erase(*it);
		CS::controllerCS.erase(*it);
		CS::collisionCS.erase(*it);
		CS::funcQCS.erase(*it);
		CS::propCS.erase(*it);
		CS::deletedEntities.push_back(*it);
		it = toDelete.erase(it);
	}
}

void CS::setGroup(const eId &id, const std::string &s){
	CS::groups[s].push_back(id);
}



std::shared_ptr<SpriteComponent> CS::getSpriteC(eId e){
	if(spriteCS[e] != nullptr){
		return spriteCS[e];
	} else {
		throw "spirte component not found";
		return nullptr;
	}
}



std::shared_ptr<CollisionComponent> CS::getCollisionC(eId e){
	if(collisionCS[e] != nullptr){
		return collisionCS[e];
	} else {
		throw "collision component not found";
		return nullptr;
	}
}