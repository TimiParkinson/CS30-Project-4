#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Iceman::getWorld() 
{
	return m_studentWptr;
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

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
