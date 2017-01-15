
#include "GameObject.h"

Color GameObject::color;
GameObject::GameObject(Color color, GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	currentState(STATE_MOVE),
	job(JOB_NONE),
	HP(100),
	Dmg(0)
{
	this->color.Set(color.r, color.g, color.b);
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	currentState(STATE_MOVE),
	job(JOB_NONE),
	HP(100),
	Dmg(0)
{
}


GameObject::~GameObject()
{
}