#include "StudentWorld.h"
#include "Actor.h"
using namespace std;

Protestor* m_Protestor;

GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

#pragma region Utilities
std::mt19937 generatorX { std::random_device{}() };
std::uniform_int_distribution<> distributionX { 0, ICE_WIDTH - 1 };
auto get_randomX = std::bind(distributionX, generatorX);

std::mt19937 generatorY { std::random_device{}() };	
std::uniform_int_distribution<> distributionY { 0, ICE_HEIGHT - 1 };
auto get_randomY = std::bind(distributionY, generatorY);

std::pair<int, int> getRandomPosition() {
    int x = get_randomX();
    int y = get_randomY();
    return std::make_pair(x, y);
}
#pragma endregion Utilities

#pragma region StudentWorld
StudentWorld::~StudentWorld() {
	delete m_iceman;
	m_iceman = nullptr;
    m_oilField.cleanUp();
	m_stage.cleanUp();
}

int StudentWorld::init() {
	//initialize stats
	m_stats.init();

	//initialize actors
	m_stage.init();

	//construct oil field
	m_oilField.init();

	//allocate and insert iceman
	m_iceman = new Iceman(this);
	m_squirt = nullptr;

	m_Protestor = new Protestor(this, m_iceman);
	//HardcoreProtestor* m_HardcoreProtestor = new HardcoreProtestor();

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	// Display Stats
	setGameStatText(m_stats.toString());

	// Give each actor a chance to do something
	m_stage.move();

	//Give player a chance to do something
	m_iceman->doSomething(m_squirt);
	if (m_squirt != nullptr && !m_squirt->isAlive()) {
		delete m_squirt;
		m_squirt = nullptr;
	}

	m_Protestor->doSomething();
	if (m_stats.getHealth() <= 0) {
		m_stats.m_lifeCount--;
		m_stats.m_healthCount = 100;
		return GWSTATUS_PLAYER_DIED;
	} else if (m_stats.getBarrels() == 0) {
		return GWSTATUS_FINISHED_LEVEL;
	} else if (m_stats.getLives() == 0) {
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() noexcept {
    delete m_iceman;
	m_iceman = nullptr;
    m_oilField.cleanUp();
	m_stage.cleanUp();
}

void StudentWorld::removeIce() noexcept {
	auto iceRemoved = [this]() -> bool {
		int deletedSomething = false;
		int endX = max(0, min(m_iceman->getX() + 3, ICE_WIDTH - 1)); //no lower than 0, no higher than ICE_WIDTH - 1
		int endY = max(0, min(m_iceman->getY() + 3, ICE_HEIGHT - 1)); //no lower than 0, no higher than ICE_HEIGHT - 1

		for (int i = m_iceman->getX(); i <= endX; i++) {
			for (int j = m_iceman->getY(); j <= endY; j++) {
				i = max(0, min(i, ICE_WIDTH - 1)); //prevents rebounding
				j = max(0, min(j, ICE_HEIGHT - 1)); //prevents rebounding
				if (m_oilField.isIce(i, j)) {
					deletedSomething = true;
				}
				m_oilField.removeIce(i, j);
			}
		}
		return deletedSomething;
	};
	if (iceRemoved()) {
		playSound(SOUND_DIG);
	}
}

bool StudentWorld::isIce(int x, int y) const noexcept {
    return m_oilField.isIce(x, y);
}

int StudentWorld::playerX() const {
    return m_iceman->getX();
}

int StudentWorld::playerY() const {
    return m_iceman->getY();
}

void StudentWorld::takeDamage(int dmg) {
	playSound(SOUND_PROTESTER_YELL);
	m_stats.m_healthCount -= dmg * 10;
}

void StudentWorld::protestorGiveUp() {
	playSound(SOUND_PROTESTER_GIVE_UP);
}

void StudentWorld::createSquirt(int x, int y) noexcept {
    playSound(SOUND_PLAYER_SQUIRT);
    m_squirt = new Squirt(x, y);
	m_stats.m_squirtCount--;
}

Squirt* StudentWorld::getSquirt() {
	return m_squirt;
}
#pragma endregion StudentWorld

#pragma region GameStats
void StudentWorld::GameStats::init() noexcept {
    m_boulderCount = min(m_levelCount / 2 + 2, 9);
    m_goldCount = max(5 - m_levelCount / 2, 2);
    m_barrelCount = min(2 + m_levelCount, 21);

}
string StudentWorld::GameStats::toString() const noexcept {
	string lvlTxt, hlthTxt, wtrTxt, gldTxt, oilTxt, snrTxt, scrTxt;
	if (m_levelCount < 10) { lvlTxt = " " + std::to_string(m_levelCount); } else { lvlTxt = std::to_string(m_levelCount); }
	if (m_healthCount < 10) { hlthTxt = "  " + std::to_string(m_healthCount) + "%"; } else if (m_healthCount < 100) { hlthTxt = " " + std::to_string(m_healthCount) + "%"; } else { hlthTxt = std::to_string(m_healthCount) + "%"; }
	if (m_squirtCount < 10) { wtrTxt = " " + std::to_string(m_squirtCount); } else { wtrTxt = std::to_string(m_squirtCount); }
	if (mm_goldCount < 10) { gldTxt = " " + std::to_string(mm_goldCount); } else { gldTxt = std::to_string(mm_goldCount); }
	if (m_barrelCount < 10) { oilTxt = " " + std::to_string(m_barrelCount); } else { oilTxt = std::to_string(m_barrelCount); }
	if (mm_sonarCount < 10) { snrTxt = " " + std::to_string(mm_sonarCount); } else { snrTxt = std::to_string(mm_sonarCount); }
	if (m_scoreCount < 10) { scrTxt = "00000" + std::to_string(m_scoreCount); } else if (m_scoreCount < 100) { scrTxt = "0000" + std::to_string(m_scoreCount); } else if (m_scoreCount < 1000) { scrTxt = "000" + std::to_string(m_scoreCount); } else if (m_scoreCount < 10000) { scrTxt = "00" + std::to_string(m_scoreCount); } else if (m_scoreCount < 100000) { scrTxt = "0" + std::to_string(m_scoreCount); } else { scrTxt = std::to_string(m_scoreCount); }
	return "Lvl: " +lvlTxt+ "  Lives: " +std::to_string(m_lifeCount)+ "  Hlth: "+hlthTxt+"  Wtr: "+wtrTxt+"  Gld: "+gldTxt+"  Oil Left: "+oilTxt+"  Sonar: "+snrTxt+" Scr: "+scrTxt;
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
	if ((x >= 0 && x <= ICE_WIDTH - 1) && (y >= 0 && y <= ICE_HEIGHT - 1)) {
		if (self[x][y] != nullptr) {
			self[x][y]->setVisible(false);
			delete self[x][y];
			self[x][y] = nullptr;
		}
	}
}

bool StudentWorld::OilField::isIce(int x, int y) const noexcept {
	if ((x >= 0 && x <= ICE_WIDTH - 1) && (y >= 0 && y <= ICE_HEIGHT - 1)) {
		if (self[x][y] != nullptr) {
			return true;
		}
	}
	return false;
}

void StudentWorld::OilField::init() {
	for (int x = 0; x < ICE_WIDTH; x++) {
		for (int y = 0; y < ICE_HEIGHT; y++) {
			if ((x >= 30 && x <= 33) && (y >= 4 && y <= ICE_HEIGHT - 1)) {
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
		if (i != nullptr) {
			delete i;
			i = nullptr;
		}
	}
}

void StudentWorld::Stage::cleanUp() noexcept {
	for (auto i : self) {
		if (i != nullptr) {
			delete i;
			i = nullptr;
		}
	}
}
template <typename T>
T* StudentWorld::Stage::spawnActor() {
	T* newActor = new T();
	self.insert(newActor);
	return newActor;
}

template <>
Boulder* StudentWorld::Stage::spawnActor<Boulder>() {
	static Boulder* newBoulder = nullptr;
	pair<int, int> randomPosition = getRandomPosition();

	if (!m_boulderBlackList.isListed(randomPosition)) {
		m_boulderBlackList.add(randomPosition);
		newBoulder = new Boulder(randomPosition.first, randomPosition.second);
		self.insert(newBoulder);
		return newBoulder;
	}
	spawnActor<Boulder>();
	return newBoulder;
}

template <>
OilBarrel* StudentWorld::Stage::spawnActor<OilBarrel>() {
	static OilBarrel* newOilBarrel = nullptr;
	pair<int, int> randomPosition = getRandomPosition();

	newOilBarrel = new OilBarrel(randomPosition.first, randomPosition.second, m_studentWorldPointer);
	self.insert(newOilBarrel);
	return newOilBarrel;

	//FIX ME
	//not sure how to seperate barrels by 6 blocks
	
}

template <>
GoldNugget* StudentWorld::Stage::spawnActor<GoldNugget>() {
	static GoldNugget* newGoldNugget = nullptr;
	pair<int, int> randomPosition = getRandomPosition();

	newGoldNugget = new GoldNugget(randomPosition.first, randomPosition.second, m_studentWorldPointer);
	self.insert(newGoldNugget);
	return newGoldNugget;
}

void StudentWorld::Stage::removeActor(Actor* actor) noexcept {
	self.erase(actor);
}

void StudentWorld::Stage::init() {

	for (int i = 0; i < m_studentWorldPointer->m_stats.getBoulders(); i++) {
		spawnActor<Boulder>();
	}
	
	for (int spawnBarrel = 0; spawnBarrel < m_studentWorldPointer->m_stats.getBarrels(); spawnBarrel++) {
		spawnActor<OilBarrel>();
	}

	for (int spawnGold = 0; spawnGold < m_studentWorldPointer->m_stats.getGold(); spawnGold++) {
		spawnActor<GoldNugget>();
	}
}
/*
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
 */
void StudentWorld::Stage::move() {
	auto it = self.begin();
	while (it != self.end()) {
		if (*it != nullptr) {
			(*it)->doSomething();
			if (!((*it)->isAlive())) {
				m_studentWorldPointer->playSound((*it)->getDeathSound());
				delete (*it);
				it = self.erase(it);
				continue;
			}
		}
		it++;
	}
}

#pragma endregion Stage
