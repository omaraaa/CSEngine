#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <map>
#include <memory>
#include <SDL2/SDL.h>

struct Vec2 {
	double x;
	double y;
	Vec2 operator*(const double& n);
	Vec2 operator*=(double const& n);
	Vec2 operator-=(Vec2 const& v);
	Vec2 operator+=(Vec2 const& v);
	bool operator==(Vec2 const& v);
	double getX()const;
	double getY()const;
	void setX(double xx);
	void setY(double yy);
};

struct Rect
{
	double x, y, w, h;
	SDL_Rect getSDLRect();
};

class Timer {
public:
	static float start, end, elapsed, frame, slice, alpha;
	static double t, dt, currentTime, accumulator;
};

class QuadTree {
public:
	QuadTree(SDL_Rect bounds);

private:
	SDL_Rect bounds;
	int level;
	int maxLevel;
	QuadTree** nodes;
};

class Grid {
public:
	float cellSize;
	SDL_Rect bounds;
	std::map<int, std::vector<unsigned long>> activeIndexes;
	Grid(int x, int y, int w, int h, int c): cellSize(c){bounds = { x, y, w, h};};
	bool overlap(const unsigned long &id1,const unsigned long &id2, SDL_Rect* result);
	std::vector<int> getIndex(unsigned long id);
	void draw();
	void clear();
	SDL_Rect getRect(const int index);
	void updateBounds(SDL_Rect *r);
	std::vector<unsigned long> getEntities(std::vector<int> indexes);
};

//bool checkOverlap(unsigned long id1, unsigned long id2, SDL_Rect* result);
bool checkOverlap(unsigned long id1, unsigned long id2, Rect* result);
bool outOfBounds(unsigned long id, SDL_Rect& bounds);
void collide(unsigned long e1, unsigned long e2);

//TEXT operations
bool strToBool(std::string str);

#endif