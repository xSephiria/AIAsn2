#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vertex.h"
#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_IRON_BALL,
		GO_CUBE,
		GO_ASTEROID,
		GO_BULLET,
		GO_SHIP,
		GO_BOSS,
		GO_BOSS_BULLET,
		GO_MISSILE,
		GO_PLAYER,
		GO_SCAVAGEPOINT,
		GO_ROCKS,
		GO_WALL,
		GO_PILLAR,
		GO_BLOCK,

		// AI asn2
		GO_MAGICIAN,

		GO_TOTAL, //must be last
	};

	enum STATES
	{
		STATE_IDLE = 0,
		STATE_DEAD,
		STATE_BUFF,
		STATE_ATTACK,
		STATE_CHASE,
		STATE_ESCAPE,
		STATE_RECOVER,
		STATE_RECHARGE,
		STATE_MOVE,
		NUM_STATES
	};

	float HP;
	GAMEOBJECT_TYPE type;
	STATES currentState = STATE_IDLE;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	Vector3 normal2;
	Vector3 scale2;
	bool active;
	float mass;
	static Color color;
	GameObject(Color color,GAMEOBJECT_TYPE typeValue = GO_BALL);
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif