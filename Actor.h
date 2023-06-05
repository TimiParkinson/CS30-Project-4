#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
 public:
    Actor(int imageID, int soundID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
        : GraphObject(imageID, startX, startY, dir, size, depth), m_alive(true), m_soundID(soundID) {}
    virtual ~Actor() {}

    virtual void doSomething() = 0;
    bool isAlive() const;
    void unAlive();
    const int getDeathSound() const;
private:
    bool m_alive;
    const int m_soundID = 0;
};

#pragma region Objects
class Object : public Actor {
 public:
    Object(int imageID, int soundID, int startX = 0, int startY = 0, Direction dir = right, double size = 1.0, unsigned int depth = 0) : 
        Actor(imageID, soundID, startX, startY, dir, size, depth) {}
    virtual ~Object(){}
    virtual void doSomething() = 0;
};

#pragma region Terrain
class Terrain : public Object {
 public:
    Terrain(int imageID, int soundID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0) :
        Object(imageID, soundID, startX, startY, dir, size, depth) { setVisible(true); }
    virtual ~Terrain() {}

    virtual void doSomething() = 0;
};

class Ice : public Terrain {
 public:
    Ice(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3) : 
        Terrain(IID_ICE, SOUND_NONE, startX, startY, dir, size, depth) {}
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
        State() : m_boulderPointer(nullptr) {}
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
    Boulder(int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 1) : 
        Terrain(IID_BOULDER, SOUND_FALLING_ROCK, startX, startY, dir, size, depth), m_state(new Idle(this)) {}
    virtual ~Boulder();

    void setState(std::string s);
    virtual void doSomething() override;
};
#pragma endregion Terrain

#pragma region Interactable
class Interactable : public Object {
 public:
     Interactable(int imageID, int soundID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sp) :
         Object(imageID, soundID, startX, startY, dir, size, depth), m_studentWorldPointer(sp) {}
    virtual ~Interactable() {}
    virtual void doSomething() = 0;
    bool detectPlayer(int distance); //detects if player is within a certain distance
    StudentWorld* m_studentWorldPointer;
};

class OilBarrel : public Interactable {
public:
    OilBarrel(int startX, int startY, StudentWorld* sp) : Interactable(IID_BARREL, SOUND_FOUND_OIL, startX, startY, right, 1.0, 2, sp) {
        setVisible(false);
    }
    virtual ~OilBarrel() {}
    virtual void doSomething() override;
};

class GoldNugget : public Interactable {
public:
    GoldNugget(int startX, int startY, StudentWorld* sp) : Interactable(IID_GOLD, SOUND_GOT_GOODIE, startX, startY, right, 1.0, 2, sp) {
        setVisible(false);
    }
    virtual ~GoldNugget() {}
    virtual void doSomething() override;
};

class SonarKit : public Interactable {
public:
    SonarKit(int startX, int startY, StudentWorld* sp) : Interactable(IID_SONAR, SOUND_GOT_GOODIE, startX, startY, right, 1.0, 2, sp) {
        setVisible(true);
    }
    virtual ~SonarKit() {}
    virtual void doSomething() {}
};

class WaterPool : public Interactable {
public:
    WaterPool(int startX, int startY, StudentWorld* sp) : Interactable(IID_WATER_POOL, SOUND_GOT_GOODIE, startX, startY, right, 1.0, 2, sp) {
        setVisible(true);
    }
    virtual ~WaterPool() {}
    virtual void doSomething() {}
};
#pragma endregion Interactable
#pragma endregion Objects
#pragma region Entities
class Entity : public Actor {
 public:
	Entity(int imageID, int soundID, int startX, int startY, Direction dir, StudentWorld* sp, int health) :
      Actor(imageID, soundID, startX, startY, dir, 1.0, 0), health(health), m_studentWorldPointer(sp) { setVisible(true); }
	virtual ~Entity() {}
	StudentWorld* getWorld() const noexcept;
    void setHealth(int h);
    int getHealth() const;
	virtual void doSomething() = 0;
    unsigned int health;
private:
	StudentWorld* m_studentWorldPointer;
};

class Squirt : public Actor {
public:
    Squirt(int startX, int startY) : Actor(IID_WATER_SPURT, SOUND_NONE, startX, startY, none, 1.0, 2) { setVisible(false); }
    virtual ~Squirt() {}
    void doSomething() {}
    void doSomething(Direction dir, StudentWorld* wrld);
    bool isAlive();
private:
    unsigned int remaining = 4;
    unsigned int cooldown = 10;
};

class Iceman : public Entity {
 public:
	Iceman(StudentWorld* sp = nullptr) : Entity(IID_PLAYER, SOUND_NONE, 30, 60, right, sp, 10) {}
	virtual ~Iceman() {}
  void doSomething() {}
	void doSomething(Squirt* sq);
 private:
};

class Protestor : public Entity {
 public:
	Protestor(StudentWorld* sp = nullptr, Iceman* im = nullptr) : Entity(IID_PROTESTER, SOUND_NONE, 60, 60, left, sp, 5), iceman(im) {}
	Protestor(StudentWorld* sp, Iceman* im, const int imageID, unsigned int health) : Entity(imageID, SOUND_NONE, 60, 60, left, sp, health), iceman(im) {}
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

double euclidianDistance(int x1, int x2, int y1, int y2);

#endif // ACTOR_H_
