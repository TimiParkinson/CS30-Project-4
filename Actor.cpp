#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Iceman::getWorld() 
{
	return m_studentWptr;
}

void Iceman::onTick()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{ 
		case KEY_PRESS_LEFT:
			moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_UP:
			moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_DOWN:
			moveTo(getX(), getY() - 1);
			break;
		//case KEY_PRESS_SPACE:
		//	// add a squirt in front of the player
		//	break;
		}
		getWorld()->removeIce();
	}
}

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
