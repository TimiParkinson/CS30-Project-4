#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Iceman::getWorld() const noexcept  {
	return m_studentWorldPointer;
}

void Iceman::doSomething() {
	int ch;
	if (getWorld()->getKey(ch)) {
		switch (ch) {
			case KEY_PRESS_LEFT:
                setDirection(Direction::left);
				moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_RIGHT:
                setDirection(Direction::right);
				moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_UP:
                setDirection(Direction::up);
				moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_DOWN:
                setDirection(Direction::down);
                moveTo(getX(), getY() - 1);
				break;
			//case KEY_PRESS_SPACE:
			//	// add a squirt in front of the player
			//	break;
		}
		m_studentWorldPointer->removeIce();
	}
}
