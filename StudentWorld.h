#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <array>
#include <unordered_set>
#include <string>
#include <vector>

class Actor;
class Iceman;
class Ice;
class Boulder;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
private:
	#pragma region GameStats
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
	#pragma endregion GameStats
	#pragma region OilField
	class OilField {
	 private:
	 	std::array<std::array<Ice*, 64>, 64> self;
	 public:
		 OilField();
        ~OilField();

        void cleanUp() noexcept;
		std::array<std::array<Ice*, 64>, 64> getField();
		bool getIce(int x, int y) const noexcept;
        void removeIce(int x, int y) noexcept;
		void init();
	};
	#pragma endregion OilField
	#pragma region Stage
	class Stage {
	 private:
	 	std::unordered_set<Actor*> self;

	 public:
		 Stage();
        ~Stage();

        void cleanUp() noexcept;
        void addActor(Actor* actor) noexcept;
        void removeActor(Actor* actor) noexcept;
        void init(OilField myField);
        void move();
	};
	#pragma endregion Stage
	
	GameStats m_stats;
	Stage m_stage;
	Iceman* m_iceman;

public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir), m_iceman(nullptr), m_stats(this) {}
	virtual ~StudentWorld() { cleanUp(); }
	virtual int init() override;
	virtual int move() override;
	virtual void cleanUp() noexcept override;
	void removeIce() noexcept;
	bool getIce(int x, int y) const noexcept;

	OilField m_oilField;
};

#endif // STUDENTWORLD_H_