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
	for (int i = 0; i <= 60 * 60; i++) {
		int x = i / 61;
		int y = i % 61;

		if ((x >= 30 && x <= 33) && (y >= 4 && y <= 59))
			m_oilField[x][y] = nullptr;
		else
			m_oilField[x][y] = new Ice(x, y);
	}
	/*for  (int x = 0; x < 60; x++) {
		for (int y = 0; y < 60; y++) {
			if ((x >= 30 && x <= 33) && (y >= 4 && y < 59))
				m_oilField[x][y] = nullptr;
			else
				m_oilField[x][y] = new Ice(x, y);
		}
	}*/

	/*for (auto& xAxis : m_oilField) {
		for (auto& yValue : xAxis) {
			if ((x >= 30 and x <= 33) and (y >= 4 and y <= 59))
				yValue == nullptr;
			else
				yValue = new Ice(x, y);
			y++;
		}
		x++;
	}*/
	//allocate and insert iceman
	m_iceMan = new Iceman(this);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	setGameStatText(m_stats.toString());

	for (const auto& i : m_actors) {
	    if (i != nullptr)
    		i->doSomething();
	}
	m_iceMan->doSomething();

	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
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

void StudentWorld::removeIce() noexcept {
	int endX = std::min(m_iceMan->getX() + 3, 59);
	int endY = std::min(m_iceMan->getY() + 3, 59);

	for (int i = m_iceMan->getX(); i <= endX; i++) {
		for (int j = m_iceMan->getY(); j <= endY; j++) {
			cout << "iceMan x: " << m_iceMan->getX() << " iceMan y: " << m_iceMan->getY() << endl;
			cout << "i: " << i << " j: " << j << endl << endl;
			if (m_oilField[i][j] != nullptr) {
				m_oilField[i][j]->setVisible(false);
				delete m_oilField[i][j];
				m_oilField[i][j] = nullptr;
			}
		}
	}
}