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
		int y;
		int x;

		for (y = 4; y <= 59; y++)
		{
			for (x = 1; x <= 29; x++)
			{
				game_objects.push_back(new Ice(x, y));
			}
		} 

		for (y = 4; y <= 59; y++)
		{
			for (x = 34; x <= 60; x++)
			{
				game_objects.push_back(new Ice(x, y));
			}
		}

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
