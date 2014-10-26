#ifndef ENTITIES_H
#define ENTITIES_H

void playerAnimationUpdate(eId id);
eId bullet(Vec2 const &pos, eId const &oid);
eId TEST(Vec2 const &pos);
eId ENEMY(Vec2 const &pos);
eId mBox(float x, float y);
eId GAMEOVER();
eId createCamera(float x, float y);
eId edgeChecker(eId parent);
eId wallChecker(eId parent);

//*CONSTANTS*//
extern float P_firerate;

#endif