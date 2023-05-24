#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

//StudentWolrd Implementation

StudentWorld::~StudentWorld() {
    delete m_iceman;
    m_iceman = nullptr;
}

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
	int endX = max(0, min(m_iceman->getX() + 3, 59)); //no lower than 0, no higher than 59
	int endY = max(0, min(m_iceman->getY() + 3, 59)); //no lower than 0, no higher than 59

	for (int i = m_iceman->getX(); i <= endX; i++) {
		for (int j = m_iceman->getY(); j <= endY; j++) {
			cout << "iceMan x: " << m_iceman->getX() << " iceMan y: " << m_iceman->getY() << endl;
            i = max(0, min(i, 59)); //prevents rebounding
            j = max(0, min(j, 59)); //prevents rebounding
			cout << "i: " << i << " j: " << j << endl << endl;
			m_oilField.removeIce(i, j);
		}
	}
}

//GameStats Implementation
void StudentWorld::GameStats::init() noexcept {
    m_boulders = min(m_level / 2 + 2, 9);
    m_gold = max(5 - m_level / 2, 2);
    m_barrels = min(2 + m_level, 21);
}
string StudentWorld::GameStats::toString() const noexcept {
    return "<game statistics>";
}

//OilField Implementation

StudentWorld::OilField::~OilField() {
    for (auto& i : self) {
        for (auto& j : i)
            delete j;
    }
}

void StudentWorld::OilField::cleanUp() noexcept {
    for (auto& i : self) {
        for (auto& j : i) {
            delete j;
            j = nullptr;
        }
    }
}
void StudentWorld::OilField::removeIce(int x, int y) noexcept {
    if (x >= 0 && x <= 59) {
        if (self[x][y] != nullptr) {
            self[x][y]->setVisible(false);
            delete self[x][y];
            self[x][y] = nullptr;
        }
    }
}

void StudentWorld::OilField::init() {
   for (int x = 0; x < 60; x++) {
		for (int y = 0; y < 60; y++){
			if ((x >= 30 && x <= 33) && (y >= 4 && y <= 59)) {
				continue;
			}
			self[x][y] = new Ice(x, y);
		}
	}
}

//Stage Implementation
StudentWorld::Stage::~Stage() {
    for (auto i : self) {
        delete i;
        i = nullptr;
    }
}

void StudentWorld::Stage::cleanUp() noexcept {
    for (auto i : self) {
        delete i;
        i = nullptr;
    }
}
void StudentWorld::Stage::addActor(Actor* actor) noexcept {
    self.insert(actor);
}
void StudentWorld::Stage::removeActor(Actor* actor) noexcept {
    self.erase(actor);
}
void StudentWorld::Stage::init() {}
void StudentWorld::Stage::move() {}
