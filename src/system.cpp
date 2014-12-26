#include "../include/system.h"
#include "../include/window.h"
#include "../include/CS.h"
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <SDL2/SDL.h>

float Timer::start{0};
float Timer::end{0};
float Timer::elapsed{0};
float Timer::frame{1000.f/60.f};
float Timer::slice{0.f};
double Timer::t{0.0};
double Timer::dt{1.f/60.f};
double Timer::currentTime{0.0};
double Timer::accumulator{0.0};
float Timer::alpha{0.0};



Vec2 Vec2::operator*(double const& n){
	x *= n;
	y *= n;
	return *this;
}

Vec2 Vec2::operator*=(double const& n){
	x *= n;
	y *= n;
	return *this;
}

Vec2 Vec2::operator+=(Vec2 const& v){
	x += v.x;
	y += v.y;
	return *this;
}

Vec2 Vec2::operator-=(Vec2 const& v){
	x -= v.x;
	y -= v.y;
	return *this;
}

bool Vec2::operator==(Vec2 const& v){
	return (x == v.x) && (y == v.y);
}

double Vec2::getX() const {return x;}
double Vec2::getY() const {return y;}
void Vec2::setX(double xx){x = xx;}
void Vec2::setY(double yy){y = yy;}

SDL_Rect Rect::getSDLRect(){
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

double Rect::maxY(){
	return y+h;
}
double Rect::maxX(){
	return x+w;
}

bool inCamBounds(Vec2 pos, Camera* c){
	if(pos.x < c->pos.x || pos.x > c->pos.x + c->size.x || pos.y < c->pos.y || pos.y > c->pos.y + c->size.y)
		return false;
	return true;
}

int DELTA = 0;
bool outOfBounds(unsigned long id, SDL_Rect& bounds){
	std::shared_ptr<CollisionComponent> c = CS::collisionCS[id];
	if(c->rect.x < bounds.x || c->rect.x + c->rect.w > bounds.x + bounds.w || c->rect.y < bounds.y || c->rect.y + c->rect.h > bounds.y + bounds.h){
		if(c->rect.x < bounds.x){
			c->moveC->pos.x = bounds.x+DELTA;
			c->touching |= RIGHT;
		} else if( c->rect.x + c->rect.w > bounds.x + bounds.w) {
			c->moveC->pos.x = bounds.x + bounds.w - c->rect.w-DELTA;
			c->touching |= LEFT;
		}
		if(c->rect.y < bounds.y){
			c->moveC->pos.y = bounds.y+DELTA; 
			c->touching |= TOP;
		} else if( c->rect.y + c->rect.h > bounds.y + bounds.h) {
			c->moveC->pos.y = bounds.y + bounds.h - c->rect.h-DELTA;
			c->touching |= FLOOR;
		}
		return true;
	}
	return false;
}

// bool checkOverlap(unsigned long id1, unsigned long id2, SDL_Rect* result){
// 	SDL_Rect r1, r2;
// 	r1 = CS::collisionCS[id1]->rect;
// 	r2 = CS::collisionCS[id2]->rect;
// 	if(SDL_IntersectRect(&r1, &r2, result))
// 			return true;
// 	return false;

// }

bool checkOverlap(unsigned long id1, unsigned long id2, Rect* result){
	Rect r1, r2;
	r1 = CS::collisionCS[id1]->rect;
	r2 = CS::collisionCS[id2]->rect;
	float left = std::max<float>(r1.x, r2.x);
	float right = std::min<float>(r1.x+r1.w, r2.x+r2.w);
	float top = std::max<float>(r1.y, r2.y);
	float bot = std::min<float>(r1.y+r1.h, r2.y+r2.h);
	//if(r1.x < r2.x+r2.w && r1.x+r1.w > r2.x && r1.y < r2.y+r2.h && r1.y+r1.h > r2.y){
	if(left < right && top < bot){
		result->x = left;
		result->y = top;
		result->w = right - left;
		result->h = bot - top;
		return true;
	}
	return false;
}

bool checkOverlap(Rect r1, Rect r2, Rect* result){
	float left = std::max<float>(r1.x, r2.x);
	float right = std::min<float>(r1.x+r1.w, r2.x+r2.w);
	float top = std::max<float>(r1.y, r2.y);
	float bot = std::min<float>(r1.y+r1.h, r2.y+r2.h);
	//if(r1.x < r2.x+r2.w && r1.x+r1.w > r2.x && r1.y < r2.y+r2.h && r1.y+r1.h > r2.y){
	if(left < right && top < bot){
		result->x = left;
		result->y = top;
		result->w = right - left;
		result->h = bot - top;
		return true;
	}
	return false;
}


void Grid::updateBounds(Rect r){
	if(fmod(r.w,cellSize) != 0){
		r.w = ceil(r.w/cellSize)*cellSize;
	}
	if(fmod(r.h,cellSize) != 0){
		r.h = ceil(r.h/cellSize)*cellSize;
	}
	bounds = r;
}

void Grid::clear(){
	activeIndexes.clear();
}

void Grid::draw(){
	SDL_Rect r;
	int count=0;
	for(int i =0; i<bounds.w/cellSize; i++){
		for(int y = 0; y < bounds.h/cellSize; y++){
			count++;
		}
	}
	for(int i =0; i<count; i++){
		r = CS::cameras[1]->getScreenRect(getRect(i));
		if(activeIndexes[i].size()>0)
			Window::DrawRect(&r, 0, 0, 255);
		else
			Window::DrawRect(&r, 200, 0, 0);
	}
}

SDL_Rect Grid::getRect(const int index){
	SDL_Rect r;
	r.x = bounds.x + cellSize*index - bounds.w*floor(index/(bounds.w/cellSize));
	r.y = bounds.y + floor(index/(bounds.w/cellSize))*cellSize;
	r.w = cellSize;
	r.h = cellSize;
	return r;
}


std::vector<unsigned long> Grid::getEntities(std::vector<int> indexes){
	std::vector<unsigned long> result{};
	for (auto i = indexes.begin(); i != indexes.end(); ++i)
	{

		result.insert(result.end(), activeIndexes[*i].begin(), activeIndexes[*i].end());
	}
	return result;
}

std::vector<int> Grid::getIndex(unsigned long id){
	Rect r1 = CS::collisionCS[id]->rect;
	int xstart = int((floor((r1.x)/cellSize)));
	if(r1.x < bounds.x)
		xstart = 0;
	int xend = int((floor((r1.x+r1.w)/cellSize)));
	int ystart = int((floor(r1.y/cellSize)));
	if(r1.y < bounds.y)
		ystart = 0;
	int yend = int((floor((r1.y+r1.h)/cellSize)));
	// if(r1.x > bounds.x+bounds.w || r1.x +r1.w < bounds.x || r1.y > bounds.y+bounds.h || r1.y +r1.h < bounds.y){
	// 	return std::vector<int>(1, -1);
	// }
	std::vector<int> indexes;
	int index = xstart + ystart*floor(bounds.w/cellSize);
	//std::cout << index << std::endl;
	for(int i=0; i<(yend-ystart+1); i++){
		for(int g=0; g<(xend-xstart+1); g++){
			index = xstart+g+ystart*floor(bounds.w/cellSize) + floor(bounds.w/cellSize)*i;
			activeIndexes[index].push_back(id);
			indexes.push_back(index);
		}

	}

	return indexes;
}

void collide2(eId e1, eId e2){
	std::shared_ptr<CollisionComponent> c1 = CS::collisionCS[e1];
	std::shared_ptr<CollisionComponent> c2 = CS::collisionCS[e2];
	if(!c1->moveable && !c2->moveable)
	{
		return;
	}

	//create 2 rects. They are used to determine which direction the collision has taken place.
	Rect r1, r2;
	float dx1 = -c1->moveC->deltaPos.x + c1->moveC->pos.x;
	float dy1 = -c1->moveC->deltaPos.y + c1->moveC->pos.y;
	float dx2 = -c2->moveC->deltaPos.x + c2->moveC->pos.x;
	float dy2 = -c2->moveC->deltaPos.y + c2->moveC->pos.y;
	float absDX1 = (dx1>0)?dx1:(-dx1);
	float absDY1 = (dy1>0)?dy1:-dy1;
	float absDX2 = (dx2>0)?dx2:(-dx2);
	float absDY2 = (dy2>0)?dy2:-dy2;
	r1.x = c1->moveC->pos.x - (dx1>0?dx1:0);
	r2.x = c2->moveC->pos.x - (dx2>0?dx2:0);
	r1.y = c1->moveC->pos.y - (dy1>0?dy1:0);
	r2.y = c2->moveC->pos.y - (dy2>0?dy2:0);
	r1.w = c1->rect.w+absDX1;
	r2.w = c2->rect.w+absDX2;
	r1.h = c1->rect.h+absDY1;
	r2.h = c2->rect.h+absDY2;
	SDL_Rect sr1 = r1.getSDLRect();
	SDL_Rect sr2 = r2.getSDLRect();
	Window::DrawRect(&sr1, 0,255,0);
	Window::DrawRect(&sr2, 0,255,0);
	// SDL_Rect overlapRect;
	// SDL_IntersectRect(&r1, &r2, &overlapRect);
	float overlapX=0, overlapY=0;
	int BIAS = 4;
	SDL_Rect a;

	if(dx1 != dx2)
	{
		float maxOverlapX = absDX1 + absDX2 + BIAS;
		if((r1.x < r2.x + r2.w) && (r1.x + r1.w > r2.x) 
			&& (r1.y < r2.y + r2.h) && ( r1.y + r1.h > r2.y))
		if(r1.x < r2.x)
		{
			overlapX = r1.x + r1.w - r2.x;
			if(overlapX > maxOverlapX)
				overlapX = 0;
			else{
				c1->touching |= LEFT;
				c2->touching |= RIGHT;
			}
		} else  {
			overlapX = -(r2.x + r2.w - r1.x);
			if(-overlapX > maxOverlapX)
				overlapX = 0;
			else{
				c1->touching |= RIGHT;
				c2->touching |= LEFT;
			}
		}
		//std::cout << maxOverlapX << " " << overlapX << std::endl;
	}

	if(dy1 != dy2)
	{
		float maxOverlapY = absDY1 + absDY2 + BIAS;
		if((r1.x < r2.x + r2.w) && (r1.x + r1.w > r2.x) 
			&& (r1.y < r2.y + r2.h) && ( r1.y + r1.h > r2.y))
		if(dy1 > dy2)
		{
			overlapY = r1.y + r1.h - r2.y;
			if(overlapY > maxOverlapY)
				overlapY = 0;
			else{
				c1->touching |= FLOOR;
				c2->touching |= TOP;
			}
		} else  {
			overlapY = -(r2.y + r2.h - r1.y);
			if(-overlapY > maxOverlapY)
				overlapY = 0;
			else{
				c1->touching |= TOP;
				c2->touching |= FLOOR;
			}
		}
		//std::cout << absDY2 << "<-2 1->" << absDY1 << std::endl;
	}
	if(overlapX != 0){
		if(!c2->moveable){
			c1->moveC->pos.x = c1->moveC->pos.x - overlapX;
			//c1->moveC->vel.x = 0;
			c1->moveC->vel.x = (c1->moveC->pos.x - c1->moveC->deltaPos.x);
		}
		else if(!c1->moveable){
			c2->moveC->pos.x = c2->moveC->pos.x + overlapX;
			c2->moveC->vel.x = (c2->moveC->pos.x - c2->moveC->deltaPos.x);
			//c2->moveC->vel.x = 0;
		}
	}
	if(overlapY != 0){
		if(!c2->moveable){
			c1->moveC->setPosition(c1->moveC->pos.x, c1->moveC->pos.y-overlapY );
			c1->moveC->vel.y = (c1->moveC->pos.y - c1->moveC->deltaPos.y);
		}
		else if(!c1->moveable){
			c2->moveC->setPosition(c2->moveC->pos.x, c2->moveC->pos.y+overlapY);
			c2->moveC->vel.y = (c2->moveC->pos.y - c2->moveC->deltaPos.y);
		}
	}
	

	c1->updatePosition();
	c2->updatePosition();
}

void collide(eId e1, eId e2){
	std::shared_ptr<CollisionComponent> c1 = CS::collisionCS[e1];
	std::shared_ptr<CollisionComponent> c2 = CS::collisionCS[e2];
	if(!c1->moveable && !c2->moveable)
	{
		return;
	}

	//create 2 rects. They are used to determine which direction the collision has taken place.
	Rect r1, r2;
	float dx1 = -c1->moveC->deltaPos.x + c1->moveC->pos.x;
	float dy1 = -c1->moveC->deltaPos.y + c1->moveC->pos.y;
	float dx2 = -c2->moveC->deltaPos.x + c2->moveC->pos.x;
	float dy2 = -c2->moveC->deltaPos.y + c2->moveC->pos.y;
	float absDX1 = (dx1>0)?dx1:(-dx1);
	float absDY1 = (dy1>0)?dy1:-dy1;
	float absDX2 = (dx2>0)?dx2:(-dx2);
	float absDY2 = (dy2>0)?dy2:-dy2;
	r1.x = c1->moveC->pos.x - (dx1>0?dx1:0);
	r2.x = c2->moveC->pos.x - (dx2>0?dx2:0);
	r1.y = c1->moveC->pos.y - (dy1>0?dy1:0);
	r2.y = c2->moveC->pos.y - (dy2>0?dy2:0);
	r1.w = c1->rect.w+absDX1;
	r2.w = c2->rect.w+absDX2;
	r1.h = c1->rect.h+absDY1;
	r2.h = c2->rect.h+absDY2;
	// SDL_Rect sr1 = r1.getSDLRect();
	// SDL_Rect sr2 = r2.getSDLRect();
	// Window::DrawRect(&sr1, 0,255,0);
	// Window::DrawRect(&sr2, 0,255,0);
	float overlapX=0, overlapY=0;
	int BIAS = 4;
	Rect a;
	if(!checkOverlap(r1, r2, &a))
		return;
	// SDL_Rect sr3 = a.getSDLRect();
	// Window::DrawRect(&sr3, 0,255,0);
	double xDir = c1->moveC->vel.x + c2->moveC->vel.x;
	double yDir = c1->moveC->vel.y + c2->moveC->vel.y;
	if(a.w <= a.h){
		float change;
		bool c = true;
		if(xDir > 0 && c1->moveC->deltaPos.x + c1->rect.w <= r2.x+1){
			if(c1->touchable & RIGHT && c2->touchable & LEFT){
				c1->touching |= LEFT;
				c2->touching |= RIGHT;
				change = c1->rect.w;
			}else{
				change = a.x - c1->moveC->pos.x;
				c = false;
			}
		} else if(xDir < 0 && c1->moveC->deltaPos.x  >= r2.x + r2.w-1){
			if(c1->touchable & LEFT && c2->touchable & RIGHT){
				c1->touching |= RIGHT;
				c2->touching |= LEFT;
				a.w *= -1;
				change = a.w;
			}else{
				change = a.x - c1->moveC->pos.x;
				c = false;
			}
		} else {
			c = false;
		}
		if(!c2->moveable && c){
			c1->moveC->pos.x = a.x - change;
			//c1->moveC->vel.x = 0;
			c1->moveC->vel.x = 0;//(c1->moveC->pos.x - c1->moveC->deltaPos.x);
		}
		else if(!c1->moveable && c){
			c2->moveC->pos.x = c2->moveC->pos.x + a.w;
			c2->moveC->vel.x = 0;//(c2->moveC->pos.x - c2->moveC->deltaPos.x);
			//c2->moveC->vel.x = 0;
		}
	}
	else {
		float change;
		float c = true;
		if(yDir > 0 && c1->moveC->deltaPos.y + c1->rect.h <= r2.y+1){
			//std::cout << yDir << " " << c1->moveC->deltaPos.y + c1->rect.h << " " << r2.y << "\n";

			if(c1->touchable & TOP && c2->touchable & FLOOR){
				c1->touching |= FLOOR;
				c2->touching |= TOP;
				change = c1->rect.h;
			}else{
				change = a.y - c1->moveC->pos.y;
				c = false;
			}
		} else if( yDir < 0 && c1->moveC->deltaPos.y  >= r2.y + r2.h-1){

			if(c1->touchable & FLOOR && c2->touchable & TOP){
				c1->touching |= TOP;
				c2->touching |= FLOOR;
				a.h *= -1;
				change = a.h+a.y-c1->moveC->pos.y;
			}else{
				change = a.y - c1->moveC->pos.y;
				c = false;
			}
		} else {
			//std::cout << yDir << " " << c1->moveC->deltaPos.y + c1->rect.h << " " << r2.y << "\n";

			c = false;
		}
		if(!c2->moveable && c){
			c1->moveC->setPosition(c1->moveC->pos.x, a.y-change);
			c1->moveC->vel.y = 0;//(c1->moveC->pos.y - c1->moveC->deltaPos.y);
		}
		else if(!c1->moveable && c){
			c2->moveC->setPosition(c2->moveC->pos.x, c2->moveC->pos.y+a.h);
			c2->moveC->vel.y = 0;//(c2->moveC->pos.y - c2->moveC->deltaPos.y);
		}
	}	
	// if(dy1 != dy2)
	// {
	// 	float maxOverlapY = absDY1 + absDY2 + BIAS;
	// 	if((r1.x < r2.x + r2.w) && (r1.x + r1.w > r2.x) 
	// 		&& (r1.y < r2.y + r2.h) && ( r1.y + r1.h > r2.y))
	// 	if(dy1 > dy2)
	// 	{
	// 		overlapY = r1.y + r1.h - r2.y;
	// 		if(overlapY > maxOverlapY)
	// 			overlapY = 0;
	// 		else{
	// 			c1->touching |= FLOOR;
	// 			c2->touching |= TOP;
	// 		}
	// 	} else  {
	// 		overlapY = -(r2.y + r2.h - r1.y);
	// 		if(-overlapY > maxOverlapY)
	// 			overlapY = 0;
	// 		else{
	// 			c1->touching |= TOP;
	// 			c2->touching |= FLOOR;
	// 		}
	// 	}
	// 	//std::cout << absDY2 << "<-2 1->" << absDY1 << std::endl;
	// }
	
	//if(overlapY != 0){
		
	//}
	

	//c1->updatePosition();
	//c2->updatePosition();
}

void seperateX(eId e1, eId e2){

}

void seperateY(eId e1, eId e2){

}

bool strToBool(std::string str){
	if(str == "False" || str == "0" || str == "false")
		return false;
	else if(str == "True" || str == "1" || str == "true")
		return true;
	else{
		std::cout << "Value recieved not true nor false! returning false" << std::endl;
		return false;
	}
}

