#ifndef CS_H
#define CS_H
//Includes
#include <vector>
#include <map>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include "../include/system.h"
#include "../include/components.h"


struct Entity
{
	
};

extern std::map<eId, std::shared_ptr<MoveComponent>> moveCS;

class CS {
public:
	static std::map<eId, std::shared_ptr<SpriteComponent>> spriteCS;
	static std::map<eId, std::shared_ptr<ControllerComponent>> controllerCS;
	static std::map<eId, std::shared_ptr<CollisionComponent>> collisionCS;
	static std::map<eId, std::shared_ptr<PropertiesComponent>> propCS;
	static std::map<eId, std::shared_ptr<FuncQComponent>> funcQCS;
	static std::map<const std::string, std::vector<eId>> groups; 
	static std::map<const std::string, SDL_Texture*> textures;
	static std::map<const int, std::map<eId, void (*)(eId)>> drawCalls;
	static std::map<eId, std::shared_ptr<Camera>> cameras;
	static std::vector<eId> deletedEntities;
	static std::vector<eId> toDelete;
	static QuadTree qt;
	static Grid grid;
	static std::shared_ptr<Camera> mainCamera;

	static eId createEntityID();
	static eId createCameraID();
	static void createMoveC(float xx, float yy, const eId &id);
	static void deleteEntity(eId id);
	static void updateEntity(eId id);
	static void setGroup(const eId &id, const std::string &s);

	static void eventUpdate(SDL_Event &e);
	static void update();
	static void interpolate();
	static void draw();
	static void collisionUpdate();
	static void cameraUpdate();
	static void clear();
	static void cleanup();

	static std::shared_ptr<SpriteComponent> getSpriteC(eId e);
	static std::shared_ptr<CollisionComponent> getCollisionC(eId e);
	static bool inGroup(const eId &id, const std::string &s);

	static eId _E_INDEX; //entities index
	static eId _C_INDEX; //cameras index
	static Rect worldbounds;
};

struct Base
{
	unsigned long id;

};

template<class E>
class Manager : public Base {
public:
	using Container = std::vector<std::unique_ptr<E>>;

private:
	unsigned long index = 0;
	Container entities;
public:
	
	E createEntity();
	void deleteEntity(E e);
};

#endif