#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {

	for (auto i : m_actors) {
		i = nullptr;
	}
	//initialize data structures (std::unordered_set<>)

	const int numBoulders = 50;
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

	int x = 0;
	int y = 0;

	while (x < 64) {
		while (y < 60) {
			if (m_oilField[x][y] != nullptr) {
				m_oilField[x][y] = nullptr;
				y += 3;
			} else if ((x >= 30 && x <= 33) && (y >= 4 && y <= 59))
				m_oilField[x][y] = nullptr;
			else
				m_oilField[x][y] = new Ice(x, y);
			y++;
		}
		x++;
		y = 0;
	}
	delete testIce;

	//allocate and insert iceman
	m_iceMan = new Iceman(this);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	setGameStatText(m_stats.toString());

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
	int endX = std::min(m_iceMan->getX() + 3, 64);
	int endY = std::min(m_iceMan->getY() + 3, 64);

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

