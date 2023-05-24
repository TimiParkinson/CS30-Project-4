#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

//StudentWolrd Implementation

int StudentWorld::init() {
	//initialize stats
	m_stats.init();

	//initialize stage for actors
	m_stage.init();

	//construct oil field
	m_oilField.init();

	//allocate and insert iceman
	m_iceman = new Iceman(this);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	// Display Stats
	setGameStatText(m_stats.toString());

	// Give each actor a chance to do something
	m_stage.move();

	//Give player a chance to do something
	m_iceman->doSomething();

	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() noexcept {
    delete m_iceman;
	m_iceman = nullptr;
    m_oilField.cleanUp();
	m_stage.cleanUp();
}

void StudentWorld::removeIce() noexcept {
	int endX = std::min(m_iceman->getX() + 3, 59);
	int endY = std::min(m_iceman->getY() + 3, 59);

	for (int i = m_iceman->getX(); i <= endX; i++) {
		for (int j = m_iceman->getY(); j <= endY; j++) {
			cout << "iceMan x: " << m_iceman->getX() << " iceMan y: " << m_iceman->getY() << endl;
			cout << "i: " << i << " j: " << j << endl << endl;
			m_oilField.removeIce(i, j);
		}
	}
}

//OilField Implementation

void StudentWorld::OilField::init() {
	for (int x = 0; x < 60; x++) {
		for (int y = 4; y < 60; y++){
			if (x >= 30 && x <= 33 && y >= 4 && y <= 59) {
				continue;
			}
			self[x][y] = new Ice(x, y);
		}
	}
}

//Stage Implementation