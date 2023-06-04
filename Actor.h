#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) 
		: GraphObject(imageID, startX, startY, dir, size, depth) {}
	virtual ~Actor() {}
	virtual void doSomething() = 0;
private:
};

#pragma region Entities
class Entity : public Actor {
public:
	Entity(int imageID, int startX, int startY, Direction dir, StudentWorld* sp, int health) : Actor(imageID, startX, startY, dir, 1.0, 0), health(health), studentWorldPtr(sp) { setVisible(true); }
	virtual ~Entity() {}
	StudentWorld* getWorld() const noexcept;
	virtual void doSomething() = 0;
private:
	unsigned int health;
	StudentWorld* studentWorldPtr;
};

class Iceman : public Entity {									
public:
	Iceman(StudentWorld* sp = nullptr) : Entity(IID_PLAYER, 30, 60, right, sp, 10) {}
	virtual ~Iceman() {}
	void doSomething();
private:
	
};

class Protestor : public Entity {
public:
	Protestor(StudentWorld* sp = nullptr, Iceman* im = nullptr) : Entity(IID_PROTESTER, 60, 60, left, sp, 5), iceman(im) {}
	Protestor(StudentWorld* sp, Iceman* im, const int imageID, unsigned int health) : Entity(imageID, 60, 60, left, sp, health), iceman(im) {}
	virtual ~Protestor() {}
	virtual void doSomething();
	virtual void makeMovement();
private:
	Iceman* iceman;
	bool hasSeen = false;

	unsigned int waitTime = 0;
	bool isLeaving = false;
	unsigned int shoutCooldown = 0;
};

class HardcoreProtestor : public Protestor {
public:
	HardcoreProtestor(StudentWorld* sp = nullptr, Iceman* im = nullptr) : Protestor(sp, im, IID_HARD_CORE_PROTESTER, 20) {}
	virtual ~HardcoreProtestor() {};
	void doSomething();
private:
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

#endif // ACTOR_H_