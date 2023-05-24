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
		GameStats(StudentWorld* stw ) : m_level(stw->getLevel()), m_lives(stw->getLives()), m_score(stw->getScore()) {}
		void init() noexcept {
			m_boulders = std::min(m_level / 2 + 2, 9);
			m_gold = std::max(5 - m_level / 2, 2);
			m_barrels = std::min(2 + m_level, 21);
		}
		std::string toString() const noexcept {
			return "<game statistics>";
		}
	};
	class OilField {
	 private:
	 	std::array<std::array<Ice*, 60>, 60> self;
	 public:
		OilField() {
			for (auto& i : self) {
				for (auto& j : i)
					j = nullptr;
			}
		}
		~OilField() {
			for (auto& i : self) {
				for (auto& j : i)
					delete j;
			}
		}

		void cleanUp() noexcept {
			for (auto& i : self) {
				for (auto& j : i) {
					delete j;
					j = nullptr;
				}
			}
		}
		void removeIce(int x, int y) noexcept {
			if (self[x][y] != nullptr) {
				self[x][y]->setVisible(false);
				delete self[x][y];
				self[x][y] = nullptr;
			}
		}
		void init();
	};
	class Stage {
	 private:
	 	std::unordered_set<Actor*> self;

	 public:
		Stage() {
			for (auto i : self) {
				i = nullptr;
			}
		}
		~Stage() {
			for (auto i : self) {
				delete i;
				i = nullptr;
			}
		}

		void cleanUp() noexcept {
			for (auto i : self) {
				delete i;
				i = nullptr;
			}
		}
		void addActor(Actor* actor) noexcept {
			self.insert(actor);
		}
		void removeActor(Actor* actor) noexcept {
			self.erase(actor);
		}
		void init() {}
		void move() {}
	};

	GameStats m_stats;
	Iceman* m_iceman;
	Stage m_stage;
	OilField m_oilField;

 public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir), m_iceman(nullptr), m_stats(this) {}

	virtual int init() override;

	virtual int move() override;

	virtual void cleanUp() noexcept override ;

	void removeIce() noexcept;

	~StudentWorld() {
		delete m_iceman;
		m_iceman = nullptr;
	}
};

#endif // STUDENTWORLD_H_