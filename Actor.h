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
    Entity(int imageID, int startX, int startY) : Actor(imageID, startX, startY) {}
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
    Protestor(StudentWorld* swp) : Entity(IID_PROTESTER, 60, 60), m_studentWorldPointer(swp) { setDirection(Direction::left); setVisible(true); }
    virtual ~Protestor() {}

    virtual void doSomething() override;
};
#pragma endregion Entities

#pragma region Objects
class Object : public Actor {
public:
    Object(int imageID, int startX = 0, int startY = 0, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Actor(imageID, startX, startY, dir, size, depth) {}
    virtual ~Object() {}

    virtual void doSomething() = 0;
};

class Terrain : public Object {
public:
    Terrain(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) : Object(imageID, startX, startY, dir, size, depth) { setVisible(true); }
    Terrain(int imageID) : Object(imageID) { setVisible(true); }
    virtual ~Terrain() {}

    virtual void doSomething() = 0;
};

class Interactable : public Object {
public:
    Interactable(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) :
        Object(imageID, startX, startY, dir, size, depth) {}
    Interactable(int imageID) : Object(imageID) {}
    virtual ~Interactable() {}
    virtual void doSomething() = 0;
};

class Ice : public Terrain {
public:
    Ice(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3) : Terrain(IID_ICE, startX, startY, dir, size, depth) {}
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
    class Waiting : public State {
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
    Boulder(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 1) : Terrain(IID_BOULDER, startX, startY, dir, size, depth), m_state(new Idle(this)) {}
    virtual ~Boulder();

    void setState(std::string s);
    virtual void doSomething() override;
};

class OilBarrel : public Interactable {
public:
    OilBarrel(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 2) :
        Interactable(IID_BARREL, startX, startY, dir, size, depth) {
        setVisible(false);
    }
    virtual ~OilBarrel() {}
    virtual void doSomething() {}
};

class GoldNugget : public Interactable {
public:
    GoldNugget(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 2) :
        Interactable(IID_GOLD, startX, startY, dir, size, depth) {}

    virtual ~GoldNugget() {}
    virtual void doSomething() {}
};

class SonarKit : public Interactable {
public:
    SonarKit(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 2) :
        Interactable(IID_SONAR, startX, startY, dir, size, depth) {
        setVisible(true);
    }

    virtual ~SonarKit() {}
    virtual void doSomething() {}
};

class WaterPool : public Interactable {
public:
    WaterPool(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 2) :
        Interactable(IID_WATER_POOL, startX, startY, dir, size, depth) {
        setVisible(true);
    }
    virtual ~WaterPool() {}
    virtual void doSomething() {}
};
#pragma endregion Objects


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_