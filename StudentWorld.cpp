#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

// StudentWorld Implementation
int StudentWorld::init() {
	//initialize stage for actors
	m_stage.init(m_oilField);

	//construct oil field
	m_oilField.init();

	//allocate and insert iceman
	m_iceman = new Iceman(this);

	//initialize stats
	m_stats.init();

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
	int endX = std::min(m_iceman->getX() + 3, 64);
	int endY = std::min(m_iceman->getY() + 3, 64);

	for (int i = m_iceman->getX(); i <= endX; i++) {
		for (int j = m_iceman->getY(); j <= endY; j++) {
			i = max(0, min(i, 64)); //prevents rebounding
            j = max(0, min(j, 64)); //prevents rebounding
			cout << "iceMan x: " << m_iceman->getX() << " iceMan y: " << m_iceman->getY() << endl;
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

StudentWorld::OilField::OilField() {
	for (auto& i : self) {
		for (auto& j : i)
			j = nullptr;
	}
}
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
std::array<std::array<Ice*, 64>, 64> StudentWorld::OilField::getField() {
	return self;
}
void StudentWorld::OilField::removeIce(int x, int y) noexcept {
    if (x >= 0 && x <= 64) {
        if (self[x][y] != nullptr) {
            self[x][y]->setVisible(false);
            delete self[x][y];
            self[x][y] = nullptr;
        }
    }
}
void StudentWorld::OilField::init() {
   for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 60; y++){
			if (self[x][y] != nullptr) {
				self[x][y] = nullptr;
				y += 4;
				continue;
			} else if ((x >= 30 && x <= 33) && (y >= 4 && y <= 59)) {
				continue;
			}
			self[x][y] = new Ice(x, y);
		}
	}
}

//Stage Implementation
StudentWorld::Stage::Stage() {
	for (auto i : self) {
		i = nullptr;
	}
}
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
void StudentWorld::Stage::init(OilField myField) {
	auto m_oilField = myField.getField();
	const int numBoulders = 6;
	std::array<Boulder*, numBoulders> m_boulderField;
	Ice* testIce = new Ice(0, 0);
	int temp = numBoulders;
	while (temp > 0) {
		int rand_x = rand() % 60;
		int rand_y = rand() % 36 + 20;
		m_oilField[rand_x][rand_y] = testIce;
		m_oilField[rand_x+1][rand_y] = testIce;
		m_oilField[rand_x+2][rand_y] = testIce;
		m_oilField[rand_x+3][rand_y] = testIce;
		m_boulderField[numBoulders-temp] = new Boulder(rand_x, rand_y);
		temp--;
	}

	delete testIce;
}
void StudentWorld::Stage::move() {}