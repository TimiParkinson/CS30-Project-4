#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	~StudentWorld(){} //free up memory

	virtual int init()
	{
		Actor* sampleIce = new Ice(1,1);
		Actor* sampleIceman = new Iceman(this);
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		//decLives();
		//return GWSTATUS_PLAYER_DIED;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
	}


private:
	std::vector<Actor*> game_objects;
};

#endif // STUDENTWORLD_H_
