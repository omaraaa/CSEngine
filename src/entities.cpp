#include "../include/CS.h"
#include "../include/components.h"
#include "../include/entities.h"
#include "../include/window.h"
#include <cmath>

float P_firerate{};

void kill(eId id){
	CS::deleteEntity(id);
	return;
}

eId STAR(float x, float y){
	eId id = CS::createEntityID();
	CS::createMoveC(x-16, y-16, id);
	CS::spriteCS[id] = std::shared_ptr<SpriteComponent>(new SpriteComponent("../data/Caret.png", moveCS, id));
	CS::spriteCS[id]->setFrame(32,32);
	CS::spriteCS[id]->layer = -10;
	std::vector<int> v = {0,1,2,3};
	void (*animC)(eId) = kill;
	CS::spriteCS[id]->playAnimation(v, 10, false, false, animC);
	return id;
}

bool testCollision(eId owner, std::string type){
	const eId shooter = CS::propCS[owner]->entities["shooter"];
	if(CS::collisionCS[owner]->overlaped && CS::collisionCS[owner]->overlapingWith[0] != shooter){
				return true;
			
	}
	return false;
}

void bulletUpdate(eId id){
	const eId shooter = CS::propCS[id]->entities["shooter"];
	int flip = CS::propCS[id]->fProps["flip"];
	CS::propCS[id]->fProps["duration"] -= Timer::dt;

	if(CS::propCS[id]->fProps["duration"] <= 0 
			|| testCollision(id, "bullet")){
		STAR(moveCS[id]->pos.x, moveCS[id]->pos.y);
			CS::propCS[id]->boolProps["dead"] = true;
		CS::deleteEntity(id);

		return;
	}

}

eId bullet(Vec2 const &pos, eId const &oid){
	eId id = CS::createEntityID();
	CS::createMoveC(pos.x, pos.y + CS::spriteCS[oid]->imgRect.h/2 + 10, id);
	moveCS[id]->drag = {1,1};
	moveCS[id]->maxV = {1500, 1500};
	moveCS[id]->terV = {1500, 1500};
	moveCS[id]->vel.x = moveCS[oid]->vel.x;

	CS::spriteCS[id] = std::shared_ptr<SpriteComponent>(new SpriteComponent("../data/hello.png", moveCS, id));
	CS::spriteCS[id]->setScale(0.3, 0.1);
	CS::spriteCS[id]->setColor(255, 255, 255);
	float speed = 4000;
	if(CS::spriteCS[oid]->facing == RIGHT){
		moveCS[id]->acc.x = speed;
		moveCS[id]->pos.x += CS::spriteCS[oid]->imgRect.w;
		//STAR(moveCS[id]->pos.x, moveCS[id]->pos.y);
	}
	else{
		moveCS[id]->acc.x = -speed;
		moveCS[id]->pos.x = moveCS[id]->pos.x - CS::spriteCS[id]->imgRect.w;
		//STAR(moveCS[id]->pos.x+CS::spriteCS[id]->imgRect.w, moveCS[id]->pos.y);
	}
	CS::spriteCS[id]->update();
	CS::propCS[id] = std::shared_ptr<PropertiesComponent>(new PropertiesComponent(id));
	CS::propCS[id]->entities["shooter"] = oid;
	CS::propCS[id]->fProps["duration"] = 0.3;
	CS::propCS[id]->fProps["flip"] = 1;
	CS::propCS[id]->stringProps["type"] == "bullet";
	CS::funcQCS[id] = std::shared_ptr<FuncQComponent>(new FuncQComponent(id));
	void (*Func)(eId) = bulletUpdate;
	CS::funcQCS[id]->add(Func);
	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(new CollisionComponent(CS::spriteCS,moveCS,id,false));
	CS::collisionCS[id]->moveable = false;
	//CS::collisionCS[id]->debugDraw = true;
}

//float shootTimer = 0;
bool holdFace = false;
void playerUpdate(eId id){
	bool running = false;
	float shootTimer = CS::propCS[id]->fProps["shootTimer"];
	float starTimer = CS::propCS[id]->fProps["starTimer"];
	starTimer += Timer::dt;
	if(starTimer > 0.085 && abs(moveCS[id]->vel.x) > 0){
		STAR(moveCS[id]->pos.x + CS::spriteCS[id]->imgRect.w/2, moveCS[id]->pos.y + CS::spriteCS[id]->imgRect.h/2);
		starTimer = 0;
	}
	if(moveCS[id]->vel.y > 0 && !(CS::collisionCS[id]->touching & FLOOR)){
		std::vector<int> v = {2};
		CS::spriteCS[id]->playAnimation(v, 1, false, true);
	}
	else if(moveCS[id]->vel.y < 0 && !(CS::collisionCS[id]->touching & FLOOR)){
		std::vector<int> v = {1};
		CS::spriteCS[id]->playAnimation(v, 1, false, true);		
	}
	else {
		if(moveCS[id]->vel.x == 0){
			std::vector<int> v = {0};
			CS::spriteCS[id]->playAnimation(v, 1, false, true);
		}
		else {
			running = true;
			std::vector<int> v = {0,1,2};
			CS::spriteCS[id]->playAnimation(v, 16, true);
		}
	}
	if(CS::propCS[id]->boolProps["shooting"] && CS::propCS[id]->fProps["fireRate"] <= shootTimer)
	{
		bullet(moveCS[id]->pos, id);
		Vec2 pos2 = moveCS[id]->pos;
		pos2.y -= 10;
		bullet(pos2, id);	
		//CS::propCS[id]->boolProps["shooting"] = false;
		shootTimer = 0;
	}
	float v = moveCS[id]->vel.x;
	float mv = moveCS[id]->maxV.x;
	if(running)
		CS::spriteCS[id]->currentAnimation.speed  = abs(24*(v/mv));
	shootTimer += Timer::dt;
	CS::propCS[id]->boolProps["holdFace"] = false;
	CS::propCS[id]->fProps["shootTimer"] = shootTimer;
	CS::propCS[id]->fProps["starTimer"] = starTimer;
}
bool shooting = false;
bool shot = false;

void playerEventUpdate(eId id, SDL_Event &e){
	float SPEED = 360;
	if (e.type == SDL_KEYDOWN){
		switch(e.key.keysym.sym){
			case SDLK_d:
				if(!holdFace)
					CS::spriteCS[id]->facing = RIGHT;
				if(moveCS[id]->vel.x < 0)
					moveCS[id]->vel.x = 0;
				moveCS[id]->acc.x = SPEED;break;
			case SDLK_a:
				if(!holdFace)
					CS::spriteCS[id]->facing = LEFT;
				if(moveCS[id]->vel.x > 0)
					moveCS[id]->vel.x = 0;
				moveCS[id]->acc.x = -SPEED;break;
			case SDLK_SPACE:
				if(CS::collisionCS[id]->touching & FLOOR)
					moveCS[id]->vel.y = -400;break;	
			case SDLK_s:
				moveCS[id]->vel.y = SPEED;break;
			case SDLK_RETURN:
				// if(shot){
					CS::propCS[id]->boolProps["shooting"]=true;
				// 	shot=true;
				// }else{
				// 	CS::propCS[id]->boolProps["shooting"]=false;
				// }
				break;
			case SDLK_LSHIFT:
				holdFace = true;
				break;
		}
	}
	if (e.type == SDL_KEYUP){
		switch(e.key.keysym.sym){
			case SDLK_d:
				if(moveCS[id]->acc.x>0)
					moveCS[id]->acc.x -= SPEED;break;
			case SDLK_a:
				if(moveCS[id]->acc.x<0)
					moveCS[id]->acc.x += SPEED;break;
			case SDLK_RETURN:
				// if(shot)
				// 	shot=false;
				CS::propCS[id]->boolProps["shooting"]=false;
				break;
			case SDLK_LSHIFT:
				holdFace = false;
				break;
		}
	}
}

eId TEST(Vec2 const &pos) {
	float x = pos.x;
	float y = pos.y;
	eId id = CS::createEntityID();
	CS::createMoveC(x, y, id);
	CS::spriteCS[id] = std::shared_ptr<SpriteComponent>(new SpriteComponent("../data/csP.png", moveCS, id));
	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(new CollisionComponent(CS::spriteCS,moveCS,id,true));
	CS::collisionCS[id]->moveable = true;
	//CS::controllerCS[id] = new ControllerComponent(moveCS, id);
	CS::propCS[id] = std::shared_ptr<PropertiesComponent>(new PropertiesComponent(id));
	CS::propCS[id]->groups["bullets"] = std::vector<eId>{};
	CS::propCS[id]->boolProps["shooting"] = false;
	CS::propCS[id]->fProps["shootTimer"] = 0;
	CS::propCS[id]->fProps["starTimer"] = 0;
	CS::propCS[id]->fProps["fireRate"] = 1/P_firerate;
	//CS::propCS[id]->entities["edgeChecker"] = edgeChecker(id);
	//CS::propCS[id]->entities["wallChecker"] = wallChecker(id);
	CS::funcQCS[id] = std::shared_ptr<FuncQComponent>(new FuncQComponent(id));;
	void (*pFunc)(eId) = playerUpdate;
	void (*pUpdate)(eId, SDL_Event&) = playerEventUpdate;
	CS::funcQCS[id]->add(pFunc);
	CS::funcQCS[id]->addEventFunc(pUpdate);
	CS::spriteCS[id]->setScale(2,2);
	CS::spriteCS[id]->setFrame(16,16);
	// std::vector<int> f = {1,2,3,4};
	// CS::spriteCS[id]->playAnimation(f, 8, true);
	moveCS[id]->maxV = {350,650};
	moveCS[id]->drag = {320,0};
	moveCS[id]->acc.y = 800;
	//CS::collisionCS[id]->debugDraw = true;
	//moveCS[id]->vel.y = sin(rand()%361)*moveCS[id]->maxV.y;
	//moveCS[id]->vel.x = cos(rand()%361)*moveCS[id]->maxV.x;


	return id;
};


void enemyUpdate(eId id){
	// eId echecker = CS::propCS[id]->entities["edgeChecker"];
	// eId wchecker = CS::propCS[id]->entities["wallChecker"];
	// if(!CS::collisionCS[echecker]->overlaped || CS::collisionCS[wchecker]->overlaped){
	// 	moveCS[id]->acc.x *= -1;
	// 	moveCS[id]->vel.x = 0;
	// }
	if(moveCS[id]->vel.x > 0){
		CS::spriteCS[id]->facing = RIGHT;
	}else if(moveCS[id]->vel.x < 0){
		CS::spriteCS[id]->facing = LEFT;
	}
}

void edgeCheckerUpdate(eId id){
	eId parent = CS::propCS[id]->entities["parent"];
	//MoveComponent* pMoveC = moveCS[parent];
	//CollisionComponent* pCollC = CS::collisionCS[parent];
	if(!CS::collisionCS[id]->overlaped && CS::propCS[id]->boolProps["active"]){
		moveCS[parent]->acc.x *= -1;
		moveCS[parent]->vel.x = 0;
		CS::propCS[id]->boolProps["active"] = false;
	}else if(CS::collisionCS[id]->overlaped && !CS::propCS[id]->boolProps["active"]) {
		CS::propCS[id]->boolProps["active"] = true;
	}
	if(moveCS[parent]->acc.x > 0){
		moveCS[id]->pos.x = moveCS[parent]->pos.x + CS::collisionCS[parent]->rect.w + moveCS[id]->acc.x + moveCS[id]->vel.x + 1;
	} else if(moveCS[parent]->acc.x < 0){
		moveCS[id]->pos.x = moveCS[parent]->pos.x - CS::collisionCS[id]->rect.w + moveCS[id]->acc.x + moveCS[id]->vel.x - 1;
	}
	moveCS[id]->pos.y = moveCS[parent]->pos.y + CS::collisionCS[parent]->rect.h + moveCS[id]->acc.y + moveCS[id]->vel.y + 1;
}

eId edgeChecker(eId parent){
	Vec2 cPos = {moveCS[parent]->pos.x + CS::collisionCS[parent]->rect.w,
	             moveCS[parent]->pos.y + CS::collisionCS[parent]->rect.h};
	eId id = CS::createEntityID();
	CS::createMoveC(cPos.x, cPos.y, id);
	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(new CollisionComponent(4, 4, moveCS, id, false));
	CS::propCS[id] = std::shared_ptr<PropertiesComponent>(new PropertiesComponent(id));
	CS::propCS[id]->entities["parent"] = parent;
	CS::funcQCS[id] = std::shared_ptr<FuncQComponent>(new FuncQComponent(id));;
	void (*cFunc)(eId) = edgeCheckerUpdate; 
	CS::funcQCS[id]->add(cFunc);
	CS::propCS[id]->boolProps["active"] = false;
	return id;
}

void wallCheckerUpdate(eId id){
	eId parent = CS::propCS[id]->entities["parent"];
	//MoveComponent* pMoveC = moveCS[parent];
	//CollisionComponent* pCollC = CS::collisionCS[parent];
	if(CS::collisionCS[id]->overlaped){
		//moveCS[parent]->acc.x *= -1;
		//moveCS[parent]->vel.x = 0;
		//if(CS::collisionCS[parent]->touching & FLOOR){
					//moveCS[parent]->acc.x *= -1;
					moveCS[parent]->vel.y = -300;
					CS::propCS[id]->boolProps["active"] = false;
		//}
	}
	if(moveCS[parent]->vel.x > 0){
		moveCS[id]->pos.x = moveCS[parent]->pos.x + CS::collisionCS[parent]->rect.w 
		+ moveCS[id]->acc.x + moveCS[id]->vel.x + 5;
	} else if(moveCS[parent]->vel.x < 0){
		moveCS[id]->pos.x = moveCS[parent]->pos.x - CS::collisionCS[id]->rect.w 
		+ moveCS[id]->acc.x + moveCS[id]->vel.x - 5;
	}
	moveCS[id]->pos.y = moveCS[parent]->pos.y + moveCS[id]->acc.y + moveCS[id]->vel.y - 1;
}


eId wallChecker(eId parent){
	eId id = CS::createEntityID();
	Vec2 cPos =
	{moveCS[parent]->pos.x + CS::collisionCS[parent]->rect.w,
		moveCS[parent]->pos.y + CS::collisionCS[parent]->rect.h};
	CS::createMoveC(cPos.x, cPos.y, id);
	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(
		new CollisionComponent(1, CS::collisionCS[parent]->rect.h - 1, moveCS, id, false));
	CS::propCS[id] = std::shared_ptr<PropertiesComponent>(new PropertiesComponent(id));
	CS::propCS[id]->entities["parent"] = parent;
	CS::propCS[id]->boolProps["active"] = false;
	CS::funcQCS[id] = std::shared_ptr<FuncQComponent>(new FuncQComponent(id));;
	void (*cFunc)(eId) = wallCheckerUpdate; 
	CS::funcQCS[id]->add(cFunc);

	return id;
}

eId ENEMY(Vec2 const &pos) {
	float x = pos.x;
	float y = pos.y;
	eId id = CS::createEntityID();
	CS::createMoveC(x, y, id);
	CS::spriteCS[id] = std::shared_ptr<SpriteComponent>(new SpriteComponent("../data/player.png", moveCS, id));
	CS::spriteCS[id]->setScale(2,2);
	CS::spriteCS[id]->setFrame(30,27);
	std::vector<int> f = {1,2,3,4};
	CS::spriteCS[id]->playAnimation(f, 8, true);

	moveCS[id]->maxV = {450,650};
	moveCS[id]->drag = {420,0};
	//moveCS[id]->acc.y = 600;
	moveCS[id]->acc = {600, 600};

	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(new CollisionComponent(CS::spriteCS,moveCS,id,true));
	CS::collisionCS[id]->moveable = true;
	//CS::collisionCS[id]->debugDraw = true;

	CS::propCS[id] = std::shared_ptr<PropertiesComponent>(new PropertiesComponent(id));
	CS::propCS[id]->entities["edgeChecker"] = edgeChecker(id);
	CS::propCS[id]->entities["wallChecker"] = wallChecker(id);

	CS::funcQCS[id] = std::shared_ptr<FuncQComponent>(new FuncQComponent(id));;
	void (*eFunc)(eId) = enemyUpdate;
	CS::funcQCS[id]->add(eFunc);
	
	return id;
};

eId collisionChecker(float x, float y, void(*callback)(eId)){
	eId id = CS::createEntityID();
	CS::createMoveC(x, y, id);
	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(new CollisionComponent(1, 1, moveCS, id, true));
	CS::funcQCS[id] = std::shared_ptr<FuncQComponent>(new FuncQComponent(id));
	CS::funcQCS[id]->add(callback);
}

eId mBox(float x, float y){
	eId id = CS::createEntityID();
	CS::createMoveC(x, y, id);
	CS::spriteCS[id] = std::shared_ptr<SpriteComponent>(new SpriteComponent("../data/hello.png", moveCS, id));
	CS::spriteCS[id]->setColor(0,111,0);
	//moveCS[id]->vel.y = sin(rand()%361)*moveCS[id]->maxV.y;
	//moveCS[id]->vel.x = cos(rand()%361)*moveCS[id]->maxV.x;
	//moveCS[id]->maxV = {3,3};
	//CS::spriteCS[id]->setScale(0.1,0.1);
	CS::collisionCS[id] = std::shared_ptr<CollisionComponent>(new CollisionComponent(CS::spriteCS,moveCS,id,true));
	CS::collisionCS[id]->moveable = false;
	CS::collisionCS[id]->touchable =  FLOOR;
	//CS::collisionCS[id]->debugDraw = true;
	float grid = CS::spriteCS[id]->imgRect.w;
	moveCS[id]->pos = {floor(x/grid)*grid, floor(y/grid)*grid};
	CS::spriteCS[id]->update();
	//CS::setGroup(id, "tiles");
	//CS::updateEntity(id);
	return id;
}

eId GAMEOVER(){
	eId id = CS::createEntityID();
	CS::createMoveC(0, 0, id);
	CS::spriteCS[id] = std::shared_ptr<SpriteComponent>(new SpriteComponent("../data/gameover.bmp", moveCS, id));


	return id;
}

eId createCamera(float x, float y){
	eId id = CS::createCameraID();
	int w, h;
	SDL_GetWindowSize(Window::mWindow.get(), &w, &h);
	CS::cameras[id] = std::shared_ptr<Camera>(new Camera(x, y, w, h, 1, id));

	return id;
}

void createBox(eId id){
	auto c = CS::collisionCS[id];
	auto m = moveCS[id];
	if(!c->overlaped){
		mBox(m->pos.x, m->pos.y);
		CS::deleteEntity(id);
		return;
	} else {
		CS::deleteEntity(id);
		return;
	}
}
