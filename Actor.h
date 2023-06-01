#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) 
		: GraphObject(imageID, startX, startY, dir, size, depth) {}
	virtual ~Actor() {}
	
	virtual void doSomething() = 0;

private:
};

#pragma region Entities
class Entity : public Actor {
public:
	Entity(int imageID, int startX, int startY): Actor(imageID, startX, startY) {}
	virtual ~Entity() {}
	virtual void doSomething() = 0;							   
};

class Iceman : public Entity {									
public:
	Iceman(StudentWorld* sp = nullptr): Entity(IID_PLAYER, 30, 60), m_studentWorldPointer(sp) { setVisible(true); }
	virtual ~Iceman() {}

	StudentWorld* getWorld() const noexcept;
	void doSomething();

private:
	StudentWorld* m_studentWorldPointer;
};
#pragma endregion Entities

#pragma region GameObjects
class Object : public Actor {
public:
	Object(int imageID, int startX = 0, int startY = 0, Direction dir = right, double size = 1.0, unsigned int depth = 0): 
		Actor(imageID, startX, startY, dir, size, depth) {}
	virtual ~Object(){}
	virtual void doSomething() = 0;
};

class Terrain : public Object {
public:
	Terrain(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : 
		Object(imageID, startX, startY, dir, size, depth) {}
	Terrain(int imageID) : Object(imageID) {}
	virtual ~Terrain() {}
	virtual void doSomething() = 0;					   
};

class Ice : public Terrain {
public:
	Ice(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3) :
		Terrain(IID_ICE, startX, startY, dir, size, depth) {
		setVisible(true);
	}
	virtual ~Ice() {}
	void doSomething() {}
};

class Boulder : public Terrain {
public:
	Boulder(int startX, int startY, Direction dir = down, double size = 1.0, unsigned int depth = 1) :
		Terrain(IID_BOULDER, startX, startY, dir, size, depth) {
		setVisible(true);
	}
	virtual ~Boulder() {}
	void doSomething() {}
};
#pragma endregion GameObjects





// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_