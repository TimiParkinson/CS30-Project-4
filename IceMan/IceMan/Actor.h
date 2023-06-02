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
};

#pragma region Entities
class Entity : public Actor {
 public:
    Entity(int imageID, int startX, int startY): Actor(imageID, startX, startY) {}
    virtual ~Entity() {}

    virtual void doSomething() = 0;
};

class Iceman : public Entity {
 private:
    StudentWorld* m_studentWorldPointer;
	int m_health;
 public:
    Iceman(StudentWorld* swp = nullptr) : Entity(IID_PLAYER, 30, 60), m_studentWorldPointer(swp) { setVisible(true); }
    virtual ~Iceman() {}

    StudentWorld* getWorld() const noexcept;
    virtual void doSomething() override;
};

class Protestor : public Entity {
 private:
	StudentWorld* m_studentWorldPointer;
 public:
	Protestor(StudentWorld* swp) : Entity(IID_PROTESTER, 60, 60), m_studentWorldPointer(swp) {setDirection(Direction::left); setVisible(true); }
	virtual ~Protestor() {}

	virtual void doSomething() override;
};
#pragma endregion Entities

#pragma region Objects
class Object : public Actor {
 public:
    Object(int imageID, int startX = 0, int startY = 0, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Actor(imageID, startX, startY, dir, size, depth) {}
    virtual ~Object(){}

    virtual void doSomething() = 0;
};

class Terrain : public Object {
 public:
    Terrain(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Object(imageID, startX, startY, dir, size, depth) {}
    Terrain(int imageID) : Object(imageID) {}
    virtual ~Terrain() {}

    virtual void doSomething() = 0;
};

class Ice : public Terrain {
 public:
    Ice(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3) : Terrain(IID_ICE, startX, startY, dir, size, depth) {
        setVisible(true);
    }
    virtual ~Ice() {}
    virtual void doSomething() override {}
};

class Boulder : public Terrain {
 private:
	#pragma region State
	class State {
	 private:
		Boulder* m_boulderPointer;
	 public:
	 	State() {}
		State(Boulder* boulder) : m_boulderPointer(boulder) {}
		virtual void doSomething() = 0;
        virtual ~State() {}
	};
	class Idle : public State {
	 private:
		Boulder* m_boulderPointer;
	 public:
		Idle(Boulder* boulder) : m_boulderPointer(boulder) {}
		virtual void doSomething() override;
        virtual ~Idle() {}
	};
	class Waiting: public State {
	 private:
		Boulder* m_boulderPointer;
	 public:
		Waiting(Boulder* boulder) : m_boulderPointer(boulder) {}
		virtual void doSomething() override;
        virtual ~Waiting() {}
	};
	class Falling : public State {
	 private:
		Boulder* m_boulderPointer;
	 public:
		Falling(Boulder* boulder) : m_boulderPointer(boulder) {}
		virtual void doSomething() override;
        virtual ~Falling() {}
	};
	#pragma endregion State
	State* m_state;
 public:
	Boulder(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 1) : Terrain(IID_BOULDER, startX, startY, dir, size, depth), m_state(new Idle(this)) {
		setVisible(true);
	}
	virtual ~Boulder();

	void setState(std::string s);
	virtual void doSomething() override;
};
#pragma endregion Objects

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
