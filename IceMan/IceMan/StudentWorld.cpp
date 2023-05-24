#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	//initialize data structures (std::unordered_set<>)
	for (auto i : m_actors) {
		i = nullptr;
	}
	//construct oil field
	int x;
	int y;
	for (auto& xAxis : m_oilField) {
		for (auto& yValue : xAxis) {
			if ((x >= 30 and x <= 33) and (y >= 4 and y <= 59))
				yValue == nullptr;
			else
				yValue = new Ice(x, y);
			y++;
		}
		x++;
	}
	//allocate and insert iceman
	m_iceMan = new Iceman(this);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	setGameStatText(m_stats.toString());
	
	for (const auto& i : m_actors) {
	    if (i != nullptr)
    		i->onTick();
	}

	m_iceMan->onTick();

	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp() noexcept {
    delete m_iceMan;
	m_iceMan = nullptr;
    for (auto& i : m_oilField) {
		for (auto& j : i) {
			delete j;
			j = nullptr;
		}
	}
	for (auto i : m_actors) {
		delete i;
		i = nullptr;
	}
}