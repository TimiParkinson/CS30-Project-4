#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <unordered_set>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
 private:
	struct GameStats {
		int m_level;
		int m_lives;
		int m_health;
		int m_squirts;
		int m_gold;
		int m_barrelsLeft;
		int m_sonar;
		int score;
	};
	GameStats m_stats;
	StudentWorld* IceMan;
	//integer value is arbitrary, just need a key to access the value
	std::unordered_set<Actor*> m_actors;
 public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {

	}

	virtual int init() {

		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move() {
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		for (const auto& it : m_actors) {
			it.move();
		}
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	virtual void cleanUp() {
	}

};

#endif // STUDENTWORLD_H_