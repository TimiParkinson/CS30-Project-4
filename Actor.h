#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject{
public:
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
		: GraphObject(imageID, startX, startY, dir, size, depth), isVisible(false) {}
	~Actor() {}

	void setVisible(bool visible) {
		isVisible = visible;
	}
	
	virtual void doSomething() = 0;

private:
	bool isVisible;

};

#pragma region Entities
class Entity : public Actor {
public:
	Entity(int imageID, int startX, int startY) : Actor(imageID, startX, startY) {}
	~Entity() {}
};

class Iceman : public Entity {
public:
	Iceman(StudentWorld* swP) : Entity(IID_PLAYER, 30, 60), studentWrldPtr(swP) { setVisible(true); }
	~Iceman() {}

	void doSomething() {
		moveTo(getX() + 1, getY() + 1);
	}

private:
	StudentWorld* studentWrldPtr;

};
#pragma endregion Entities

#pragma region GameObjects
class Object : public Actor {
public:
	Object(int imageID, int startX = 0, int startY = 0): Actor(imageID, startX, startY) {}
	~Object(){}
};

class Terrain : public Object {
public:
	Terrain(int imageID, int startX, int startY) : Object(imageID, startX, startY) {}
	Terrain(int imageID) : Object(imageID) {}
	~Terrain() {}
};

class Ice : public Terrain {
public:
	Ice(int startX, int startY) : Terrain(IID_ICE, startX, startY) {}
	~Ice() {}

};
#pragma endregion GameObjects





// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_