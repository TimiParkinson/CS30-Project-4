#include "StudentWorld.h"
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

std::mt19937 generator { std::random_device{}() };
std::uniform_int_distribution<> distribution { 0, MAX_SIZE };
auto get_random = std::bind(distribution, generator);
pair<int, int> getRandomPosition() {
	int x = get_random();
	int y = get_random();
	return std::make_pair(x, y);
}

#pragma region StudentWorld
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
	auto wasIceRemoved = [this]() -> bool {
		int deletedSomething = false;
		int endX = max(0, min(m_iceman->getX() + 3, 59)); //no lower than 0, no higher than 59
		int endY = max(0, min(m_iceman->getY() + 3, 59)); //no lower than 0, no higher than 59

		for (int i = m_iceman->getX(); i <= endX; i++) {
			for (int j = m_iceman->getY(); j <= endY; j++) {
				i = max(0, min(i, 59)); //prevents rebounding
				j = max(0, min(j, 59)); //prevents rebounding
				if (m_oilField.isIce(i, j)) {
					deletedSomething = true;
				}
				m_oilField.removeIce(i, j);
			}
		}
		return deletedSomething;
	};
	if (wasIceRemoved()) {
		playSound(SOUND_DIG);
	}
}
#pragma endregion StudentWorld

#pragma region GameStats
void StudentWorld::GameStats::init() noexcept {
    m_boulderCount = min(m_levelCount / 2 + 2, 9);
    m_goldCount = max(5 - m_levelCount / 2, 2);
    m_barrelCount = min(2 + m_levelCount, 21);
}
string StudentWorld::GameStats::toString() const noexcept {
    return "<game statistics>";
}

// Getters
int StudentWorld::GameStats::getLevel() const noexcept {
	return m_levelCount;
}
int StudentWorld::GameStats::getLives() const noexcept {
	return m_lifeCount;
}
int StudentWorld::GameStats::getScore() const noexcept {
	return m_scoreCount;
}
int StudentWorld::GameStats::getHealth() const noexcept {
	return m_healthCount;
}
int StudentWorld::GameStats::getSquirts() const noexcept {
	return m_squirtCount;
}
int StudentWorld::GameStats::getGold() const noexcept {
	return m_goldCount;
}
int StudentWorld::GameStats::getBarrels() const noexcept {
	return m_barrelCount;
}
int StudentWorld::GameStats::getSonar() const noexcept {
	return m_sonarCount;
}
int StudentWorld::GameStats::getBoulders() const noexcept {
	return m_boulderCount;
}
#pragma endregion GameStats

#pragma region OilField
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

bool StudentWorld::OilField::isIce(int x, int y) const noexcept {
	if (x >= 0 && x <= 59) {
		if (self[x][y] != nullptr) {
			return true;
		}
	}
	return false;
}

void StudentWorld::OilField::init() {
   for (int x = 0; x < 60; x++) {
		for (int y = 0; y < 60; y++){
			if (m_iceBlackList.isListed(x, y)) {
				continue;
			}
			self[x][y] = new Ice(x, y);
		}
	}
}
#pragma endregion OilField

#pragma region Stage
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
void StudentWorld::Stage::spawnActor(Actor* actor) noexcept {
    self.insert(actor);
}
void StudentWorld::Stage::spawnActor(Boulder* boulder) noexcept {
	pair<int, int> randomPosition = getRandomPosition();

	if (m_boulderBlackList.isListed(randomPosition)) {
		boulder->moveTo(randomPosition.first, randomPosition.second);
		m_boulderBlackList.add(randomPosition);
		self.insert(boulder);
	}

	spawnActor(boulder);
}
void StudentWorld::Stage::removeActor(Actor* actor) noexcept {
    self.erase(actor);
}
void StudentWorld::Stage::init() {

	for (int i = 0; i < m_studentWorldPointer->m_stats.getBoulders(); i++) {
		spawnActor(new Boulder);
	}
}
void StudentWorld::Stage::move() {
	for (auto i : self) {
		i->doSomething();
	}
}
#pragma endregion Stage