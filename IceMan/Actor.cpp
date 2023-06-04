#include "Actor.h"
#include "StudentWorld.h"

StudentWorld* Iceman::getWorld() const noexcept  {
    return m_studentWorldPointer;
}

#pragma region Iceman
void Iceman::doSomething() {
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
            if (getDirection() == right && getX() < 64) {
                moveTo(getX() + 1, getY());
                break;
            } else {
                setDirection(right);
                break;
            }
        case KEY_PRESS_UP:
            if (getDirection() == up && getY() < 64) {
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
        //case KEY_PRESS_SPACE:
        //    // add a squirt in front of the player
        //    break;
        }
        getWorld()->removeIce();
    }
}
#pragma endregion Iceman

#pragma region Protestor

#pragma endregion Protestor

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
