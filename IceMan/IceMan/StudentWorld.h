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
        void init() noexcept;
        std::string toString() const noexcept;
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
        ~OilField();

        void cleanUp() noexcept;
        void removeIce(int x, int y) noexcept;
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
        ~Stage();

        void cleanUp() noexcept;
        void addActor(Actor* actor) noexcept;
        void removeActor(Actor* actor) noexcept;
        void init();
        void move();
	};
    
	GameStats m_stats;
    OilField m_oilField;
	Stage m_stage;
    Iceman* m_iceman;

 public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir), m_iceman(nullptr), m_stats(this) {}
    virtual ~StudentWorld();

	virtual int init() override;

	virtual int move() override;

	virtual void cleanUp() noexcept override ;

	void removeIce() noexcept;
};

#endif // STUDENTWORLD_H_
