#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject{
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
		: GraphObject(imageID, startX, startY, dir, size, depth) {}
	~Actor();

	bool setVisible();
	
private:
	bool isVisible;

};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_