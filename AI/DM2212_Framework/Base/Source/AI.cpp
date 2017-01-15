#include "AI.h"

AI::AI()
{
	active = false;
	HP = 0;
	position = Vector3(0, 0, 0);
}

AI::AI(int HP, STATES State, bool active, Vector3 position)
{
	this->HP = HP;
	currentState = State;
	this->active = active;
	this->position = position;
}

AI::~AI()
{

}

void AI::setState(STATES newState)
{
	currentState = newState;
}

STATES AI::getState()
{
	return currentState;
}

void AI::Update(double dt)
{

}