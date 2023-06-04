#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Entity::getWorld() const noexcept
{
	return studentWorldPtr;
}

void Iceman::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{ 
		case KEY_PRESS_LEFT:
			if(getDirection() == left && getX() > 0)
			{
				moveTo(getX() - 1, getY());
				break;
			}
			else
			{
				setDirection(left);
				break;
			}
		case KEY_PRESS_RIGHT:
			if (getDirection() == right && getX() < 60)
			{
				moveTo(getX() + 1, getY());
				break;
			}
			else {
				setDirection(right);
				break;
			}
		case KEY_PRESS_UP:
			if (getDirection() == up && getY() < 60)
			{
				moveTo(getX(), getY() + 1);
				break;
			}
			else
			{
				setDirection(up);
				break;
			}
		case KEY_PRESS_DOWN:
			if (getDirection() == down && getY() > 0)
			{
				moveTo(getX(), getY() - 1);
				break;
			}
			else
			{
				setDirection(down);
				break;
			}
		//case KEY_PRESS_SPACE:
		//	// add a squirt in front of the player
		//	break;
		}
		getWorld()->removeIce();
	}
}

void Protestor::makeMovement() {
	int x = getX();
	int y = getY();

	unsigned int u = 0;
	unsigned int r = 0;
	unsigned int l = 0;
	unsigned int d = 0;

	// Get Movement Possibilities
	if (y < 60 && getWorld()->getIce(x, y + 4) && getWorld()->getIce(x + 1, y + 4) && getWorld()->getIce(x + 2, y + 4) && getWorld()->getIce(x + 3, y + 4)) ++u;
	if (x < 60 && getWorld()->getIce(x + 4, y) && getWorld()->getIce(x + 4, y + 1) && getWorld()->getIce(x + 4, y + 2) && getWorld()->getIce(x + 4, y + 3)) ++r;
	if (x > 0 && getWorld()->getIce(x - 1, y) && getWorld()->getIce(x - 1, y + 1) && getWorld()->getIce(x - 1, y + 2) && getWorld()->getIce(x - 1, y + 3)) ++l;
	if (y > 0 && getWorld()->getIce(x, y - 1) && getWorld()->getIce(x + 1, y - 1) && getWorld()->getIce(x + 2, y - 1) && getWorld()->getIce(x + 3, y - 1)) ++d;

	// Get sightlines (FIX: Check for blocks in between)
	if (iceman->getX() == x) {
		if (u != 0 && iceman->getY() > y) { ++u; hasSeen = true; }
		else if (d != 0) { ++d; hasSeen = true; }
	} else if (iceman->getY() == y) {
		if (r != 0 && iceman->getX() > x) { ++r; hasSeen = true; }
		else if (l != 0) { ++l; hasSeen = true; }
	} else {
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

	} else if (waitTime != 0) {
		waitTime--;
	} else {
		makeMovement();
	}
}

void HardcoreProtestor::doSomething() {

}
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
