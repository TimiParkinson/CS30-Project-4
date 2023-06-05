#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Entity::getWorld() const noexcept {
	return m_studentWorldPointer;
}

void Entity::setHealth(int h) {
	health = h;
}

int Entity::getHealth() const {
	return health;
}

bool Actor::isAlive() const {
	return m_alive;
}

void Actor::unAlive() {
	m_alive = false;
}

const int Actor::getDeathSound() const {
	return m_soundID;
}

#pragma region Iceman
void Iceman::doSomething(Squirt* sq) {
	  if (sq != nullptr) {
		    sq->doSomething(getDirection(), getWorld());
	  }
  
	  constexpr int boundIceMan = 60;

    int ch;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
        case KEY_PRESS_LEFT:
            if(getDirection() == left && getX() > 0) {
                moveTo(getX() - 1, getY());
                break;
            } else {
                setDirection(left);
                break;
            }
        case KEY_PRESS_RIGHT:
            if (getDirection() == right && getX() < boundIceMan) {
                moveTo(getX() + 1, getY());
                break;
            } else {
                setDirection(right);
                break;
            }
        case KEY_PRESS_UP:
            if (getDirection() == up && getY() < boundIceMan) {
                moveTo(getX(), getY() + 1);
                break;
            } else {
                setDirection(up);
                break;
            }
        case KEY_PRESS_DOWN:
            if (getDirection() == down && getY() > 0) {
                moveTo(getX(), getY() - 1);
                break;
            } else {
                setDirection(down);
                break;
            }
        case KEY_PRESS_SPACE:
			if (getWorld()->m_stats.m_squirtCount != 0 && getWorld()->getSquirt() == nullptr) getWorld()->createSquirt(getX(), getY());
			break;
        }
        getWorld()->removeIce();
    }
}
#pragma endregion Iceman

#pragma region Protestor
void Protestor::makeMovement() {
	int x = getX();
	int y = getY();

	unsigned int u = 0;
	unsigned int r = 0;
	unsigned int l = 0;
	unsigned int d = 0;

	// Get Movement Possibilities
	if (y < 60 && !getWorld()->isIce(x, y + 4) && !getWorld()->isIce(x + 1, y + 4) && !getWorld()->isIce(x + 2, y + 4) && !getWorld()->isIce(x + 3, y + 4)) ++u;
	if (x < 60 && !getWorld()->isIce(x + 4, y) && !getWorld()->isIce(x + 4, y + 1) && !getWorld()->isIce(x + 4, y + 2) && !getWorld()->isIce(x + 4, y + 3)) ++r;
	if (x > 0 && !getWorld()->isIce(x - 1, y) && !getWorld()->isIce(x - 1, y + 1) && !getWorld()->isIce(x - 1, y + 2) && !getWorld()->isIce(x - 1, y + 3)) ++l;
	if (y > 0 && !getWorld()->isIce(x, y - 1) && !getWorld()->isIce(x + 1, y - 1) && !getWorld()->isIce(x + 2, y - 1) && !getWorld()->isIce(x + 3, y - 1)) ++d;

	// Get sightlines (FIX: Check for blocks in between)
	if (iceman->getX() == x) {
		if (u != 0 && iceman->getY() > y) { ++u; hasSeen = true; }
		else if (d != 0) { ++d; hasSeen = true; }
	}
	else if (iceman->getY() == y) {
		if (r != 0 && iceman->getX() > x) { ++r; hasSeen = true; }
		else if (l != 0) { ++l; hasSeen = true; }
	}
	else {
		if (hasSeen == true) {
			if (u != 0 && getDirection() == up) ++u;
			else if (r != 0 && getDirection() == right) ++r;
			else if (l != 0 && getDirection() == left) ++l;
			else if (d != 0 && getDirection() == down) ++d;
		}
	}

	// Discourage Backtracking
	if (u != 0 && getDirection() != down) ++u;
	if (r != 0 && getDirection() != left) ++r;
	if (l != 0 && getDirection() != right) ++l;
	if (d != 0 && getDirection() != up) ++d;

	// Encourage Turning
	bool encourageTurning = (std::rand() % 7) % 3 == 0;
	if (encourageTurning) {
		if (u != 0 && (getDirection() == left || getDirection() == right)) ++u;
		if (r != 0 && (getDirection() == down || getDirection() == up)) ++r;
		if (l != 0 && (getDirection() == up || getDirection() == down)) ++l;
		if (d != 0 && (getDirection() == right || getDirection() == left)) ++d;
	}

	Direction toGo;
	int highest = 0;
	if (u > highest) {
		highest = u;
		toGo = up;
	} if (r > highest) {
		highest = r;
		toGo = right;
	} if (l > highest) {
		highest = l;
		toGo = left;
	} if (d > highest) {
		highest = d;
		toGo = down;
	}

	if (highest > 0) {
		if (toGo == up) {
			if (getDirection() != up) {
				hasSeen = false;
				setDirection(up);
			}
			moveTo(x, y + 1);
		} else if (toGo == down) {
			if (getDirection() != down) {
				hasSeen = false;
				setDirection(down);
			}
			moveTo(x, y - 1);
		} else if (toGo == right) {
			if (getDirection() != right) {
				hasSeen = false;
				setDirection(right);
			}
			moveTo(x + 1, y);
		} else {
			if (getDirection() != left) {
				hasSeen = false;
				setDirection(left);
			}
			moveTo(x - 1, y);
		}
	}
	waitTime = 5;
}

void Protestor::doSomething() {
	if (isLeaving) {
		if (isVisible() && getX() == 60 && getY() == 60) {
			setVisible(false);
		} else {
			makeMovement();
		}
		return;
	} else if (waitTime != 0) {
		waitTime--;
	} else {
		if (euclidianDistance(getX(), getY(), getWorld()->playerX(), getWorld()->playerY()) <= 4) {
			if (shoutCooldown == 0) {
				getWorld()->takeDamage(2);
				shoutCooldown = 15;
				return;
			}
		} else {
			makeMovement();
		}

		if (shoutCooldown != 0) shoutCooldown--;
		waitTime = std::max(0, 3 - getWorld()->m_stats.m_levelCount / 4);
	}
	
	if (getWorld()->getSquirt() != nullptr && euclidianDistance(getX(), getY(), getWorld()->getSquirt()->getX(), getWorld()->getSquirt()->getY()) <= 4) {
		health--;
		if (health == 0) {
			isLeaving = true;
			getWorld()->protestorGiveUp();
			if (getDirection() == down) setDirection(up);
			if (getDirection() == left) setDirection(right);
			if (getDirection() == right) setDirection(left);
			if (getDirection() == up) setDirection(down);
		}
	}
}

void HardcoreProtestor::doSomething() {

}
#pragma endregion Protestor

bool Interactable::detectPlayer(int distance) {
	return (euclidianDistance(getX(), getY(), m_studentWorldPointer->playerX(), m_studentWorldPointer->playerY()) <= distance);
}

#pragma region Boulder
void Boulder::doSomething() {
    m_state->doSomething();
}

void Boulder::setState(std::string s) {
    if (s == "idle")
        m_state = new Idle(this);
    else if (s == "waiting")
        m_state = new Waiting(this);
    else if (s == "falling")
        m_state = new Falling(this);
    else
        throw std::invalid_argument("Invalid State. State::" + s + " does not exist.");
}

Boulder::~Boulder() {
    delete m_state;
    m_state = nullptr;
}

#pragma region State
#pragma region Idle
void Boulder::Idle::doSomething() {} //literally just do nothing
#pragma endregion Idle

#pragma region Waiting
void Boulder::Waiting::doSomething() {
    //wait 30 ticks
    //if 30 ticks have passed, set state to falling
    //may use threading | may use a counter
}
#pragma endregion Waiting

#pragma region Falling
void Boulder::Falling::doSomething() {
    m_boulderPointer->setDirection(Direction::down);
    m_boulderPointer->moveTo(m_boulderPointer->getX(), m_boulderPointer->getY() - 1);
}
#pragma endregion Falling
#pragma endregion State
#pragma endregion Boulder
#pragma region Oil Barrel
void OilBarrel::doSomething() {
	if (!isAlive()) return;
	
	if (!isVisible() && detectPlayer(4)) {
		setVisible(true);
		return;
	}
	else if (detectPlayer(3)) {
		unAlive();
		m_studentWorldPointer->m_stats.m_scoreCount += 1000;
		m_studentWorldPointer->m_stats.m_barrelCount--;
	}
}

void GoldNugget::doSomething() {
	if (!isAlive()) return;

	if (!isVisible() && detectPlayer(4)) {
		setVisible(true);
		return;
	}
	else if (detectPlayer(3)) {
		unAlive();
		m_studentWorldPointer->m_stats.m_scoreCount += 10;
		m_studentWorldPointer->m_stats.m_goldCount--;
		m_studentWorldPointer->m_stats.mm_goldCount++;
	}
}

double euclidianDistance(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

#pragma endregion Oil Barrel
void Squirt::doSomething(Direction dir, StudentWorld* wrld) {
	if (cooldown == 0) {
		if (getDirection() == none) {
			setDirection(dir);
		}
		if (isVisible() == false) {
			setVisible(true);
		}
		int x = getX();
		int y = getY();
		if (getDirection() == up) {
			if (y < 60-1 && !wrld->isIce(x, y + 4) && !wrld->isIce(x + 1, y + 4) && !wrld->isIce(x + 2, y + 4) && !wrld->isIce(x + 3, y + 4)) {
				moveTo(x, y + 2);
			} else {
				remaining = 0;
				return;
			}
		} else if (getDirection() == right) {
			if (x < 60-1 && !wrld->isIce(x + 4, y) && !wrld->isIce(x + 4, y + 1) && !wrld->isIce(x + 4, y + 2) && !wrld->isIce(x + 4, y + 3)) {
				moveTo(x + 2, y);
			} else {
				remaining = 0;
				return;
			}
		} else if (getDirection() == down) {
			if (y > 0+1 && !wrld->isIce(x, y - 1) && !wrld->isIce(x + 1, y - 1) && !wrld->isIce(x + 2, y - 1) && !wrld->isIce(x + 3, y - 1)) {
				moveTo(x, y - 2);
			} else {
				remaining = 0;
				return;
			}
		} else {
			if (x > 0+1 && !wrld->isIce(x - 1, y) && !wrld->isIce(x - 1, y + 1) && !wrld->isIce(x - 1, y + 2) && !wrld->isIce(x - 1, y + 3)) {
				moveTo(x - 2, y);
			} else {
				remaining = 0;
				return;
			}
		}
		--remaining;
		cooldown = 10;
	} else {
		--cooldown;
	}
}

bool Squirt::isAlive() {
	return (remaining != 0);
}
