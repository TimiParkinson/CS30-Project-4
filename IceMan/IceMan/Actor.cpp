#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Iceman::getWorld() const  {
	return m_studentWorldPointer;
}

void Iceman::onTick() {
	int ch;
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			moveTo(getX() + 1, getY());
			break;
		//case KEY_PRESS_SPACE:
		//	// add a squirt in front of the player
		//	break;
		}
	}
}