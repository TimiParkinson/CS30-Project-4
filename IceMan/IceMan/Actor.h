#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : GraphObject(imageID, startX, startY, dir, size, depth) {}
	~Actor() {}

	virtual void doSomething() = 0;
};

#pragma region Entities
class Entity : public Actor {
public:
	Entity(int imageID, int startX, int startY) : Actor(imageID, startX, startY) {}
	~Entity() {}
};

class Iceman : public Entity {
private:
	StudentWorld* m_studentWorldPointer;
public:
	Iceman(StudentWorld* sp = nullptr): Entity(IID_PLAYER, 30, 60), m_studentWorldPointer(sp) { setVisible(true); }
	~Iceman() {}

	StudentWorld* getWorld() const;
	void doSomething();
};
#pragma endregion Entities

#pragma region GameObjects
class Object : public Actor {
public:
	Object(int imageID, int startX = 0, int startY = 0, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Actor(imageID, startX, startY, dir, size, depth) {}
	~Object(){}
};

class Terrain : public Object {
public:
	Terrain(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Object(imageID, startX, startY, dir, size, depth) {}
	Terrain(int imageID) : Object(imageID) {}
	~Terrain() {}
};

class Ice : public Terrain {
public:
	Ice(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 3) : Terrain(IID_ICE, startX, startY, dir, size, depth) {
		setVisible(true);
		//std::cout << "created ICE" << std::endl;
	}
	~Ice() {}
	void doSomething() {}
};
#pragma endregion GameObjects

#endif // ACTOR_H_