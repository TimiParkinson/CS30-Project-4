#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <array>
#include <unordered_set>
#include <string>

class Actor;
class Iceman;
class Ice;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
 private:
	class GameStats {
	 private:
		int m_level;
		int m_lives;
		int m_score;
		int m_health;
		int m_squirts;
		int m_gold;
		int m_barrels;
		int m_sonar;

		int m_boulders;
	 public:
		GameStats(/*StudentWorld* stw */) /*: m_level(stw->getLevel()), m_lives(stw->getLives()), m_score(stw->getScore())*/ {}
		void init() noexcept {
			m_boulders = std::min(m_level / 2 + 2, 9);
			m_gold = std::max(5 - m_level / 2, 2);
			m_barrels = std::min(2 + m_level, 21);
		}
		std::string toString() {
			return "<game statistics>";
		}
	};
	GameStats m_stats;
	Iceman* m_iceMan;
	std::unordered_set<Actor*> m_actors;
	std::array<std::array<Ice*, 60>, 60> m_oilField;

 public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir), m_iceMan(nullptr) {
		for (auto i : m_actors) {
			i = nullptr;
		}
		for (auto& i : m_oilField) {
			for (auto& j : i)
			j = nullptr;
		}
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp() noexcept;

	void removeIce() noexcept;

	~StudentWorld() {
		cleanUp();
	}
};

#endif // STUDENTWORLD_H_