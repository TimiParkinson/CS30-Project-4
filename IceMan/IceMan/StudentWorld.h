#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <array>
#include <unordered_set>
#include <string>
#include <vector>
#include <utility>
#include <ranges>
#include <random>
#include <functional>

class Actor;
class Iceman;
class Ice;
class Boulder;

constexpr int MAX_SIZE = 64;

class StudentWorld : public GameWorld {
 private:
	#pragma region BlackList
	template <typename T>
	class BlackList {
	 private:
		std::array<std::pair<int, int>, MAX_SIZE> m_positions;
	 public:
		BlackList() {
			init();
		}
		virtual ~BlackList() {}

		void init () {
			for (int x = 0; x < MAX_SIZE; x++) {
				for (int y = 0; y < MAX_SIZE; y++){
					if ((x >= 30 && x <= 33) && (y >= 4 && y <= MAX_SIZE - 1)) {
						m_positions[m_positions.size() - 1] = std::make_pair(x, y);
					}
				}
			}
		}
		void add(std::pair<int, int> p) {
			if (!isListed(p)) {
				m_positions[m_positions.size() - 1] = p;
			}
		}
		bool isListed (std::pair<int, int> p) {
			return std::ranges::find(m_positions, p) != m_positions.end();
		}
		bool isListed (int x, int y) {
			return std::ranges::find(m_positions, std::make_pair(x, y)) != m_positions.end();
		}
	};

	template <>
	class BlackList<Boulder> : public BlackList<BlackList<Boulder>> {
	 private:
		std::array<std::pair<int, int>, MAX_SIZE> m_positions;
		const int m_boulderPadding = 6;
	 public:
		BlackList() {
			init();
		}
		virtual ~BlackList() {}
		void add(std::pair<int, int> p) {
			for (int i = 0; i < m_boulderPadding; i++) {
				for (int j = 0; j < m_boulderPadding; j++) {
					p = std::make_pair(p.first + i, p.second + j);
					if (isListed(p.first, p.second)) {
						continue;
					} else {
						m_positions[m_positions.size() - 1] = p;
					}
				}
			}
		}
	};

	#pragma endregion BlackList

	#pragma region GameStats
	class GameStats {
	 private:
	 	StudentWorld* m_studentWorldPointer;

		int m_levelCount;
		int m_lifeCount;
		int m_scoreCount;
		int m_healthCount;
		int m_squirtCount;
		int m_goldCount;
		int m_barrelCount;
		int m_sonarCount;
		int m_boulderCount;
	 public:
		GameStats(StudentWorld* swp) : m_studentWorldPointer(swp), m_levelCount(swp->getLevel()), m_lifeCount(swp->getLives()), m_scoreCount(swp->getScore()) {}
        void init() noexcept;
        std::string toString() const noexcept;

		// Getters
		int getLevel() const noexcept;
		int getLives() const noexcept;
		int getScore() const noexcept;
		int getHealth() const noexcept;
		int getSquirts() const noexcept;
		int getGold() const noexcept;
		int getBarrels() const noexcept;
		int getSonar() const noexcept;
		int getBoulders() const noexcept;
	};
	#pragma endregion GameStats

	#pragma region OilField
	class OilField {
	 private:
	 	std::array<std::array<Ice*, MAX_SIZE>, MAX_SIZE> self;
		BlackList<Ice> m_iceBlackList;
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
		bool isIce(int x, int y) const noexcept;
		void init();
	};
	#pragma endregion OilField

	#pragma region Stage
	class Stage {
	 private:
		BlackList<Boulder> m_boulderBlackList;
	 	std::unordered_set<Actor*> self;
		StudentWorld* m_studentWorldPointer;
		void spawnActor(Actor* actor) noexcept;
		void spawnActor(Boulder* boulder) noexcept;
        void removeActor(Actor* actor) noexcept;
	 public:
		Stage(StudentWorld* swp) : m_studentWorldPointer(swp) {
			for (auto i : self) {
				i = nullptr;
			}
		}
        ~Stage();

        void cleanUp() noexcept;
        void init();
        void move();
	};
    #pragma endregion Stage

	GameStats m_stats;
    OilField m_oilField;
	Stage m_stage;
    Iceman* m_iceman;

 public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir), m_iceman(nullptr), m_stats(this), m_stage(this) {}
    virtual ~StudentWorld();

	virtual int init() override;

	virtual int move() override;

	virtual void cleanUp() noexcept override ;

	void removeIce() noexcept;
};

#endif // STUDENTWORLD_H_
