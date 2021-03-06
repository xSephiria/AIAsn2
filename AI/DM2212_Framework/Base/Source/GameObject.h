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
		// AI asn2
		GO_MOB,
		GO_MAGICIAN,
		GO_HEALER,
		GO_WARRIOR,
		GO_ARCHER,
		GO_ARROW,
		GO_FIREBALL,
		GO_HEROTOWER,
		GO_ENEMYTOWER,
		GO_TOTAL, //must be last
	};

	enum STATES
	{
		STATE_IDLE = 0,
		STATE_DEAD,
		STATE_BUFF,
		STATE_ATTACK,
		STATE_MOVE,
		STATE_STOP,
		STATE_HEAL,
		STATE_RECHARGE,
		NUM_STATES
	};

	enum JOB_CLASS
	{
		JOB_MAGICIAN,
		JOB_HEALER,
		JOB_WARRIOR,
		JOB_ARCHER,
		JOB_MOB,
		JOB_HTOWER,
		JOB_ETOWER,
		JOB_NONE,
		NUM_JOB
	};

	int HP;
	GAMEOBJECT_TYPE type;
	STATES currentState;
	JOB_CLASS job;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	Vector3 normal2;
	Vector3 scale2;
	bool active;
	float mass;
	int Dmg;
	int Def;
	bool isHealTarget;
	static Color color;
	GameObject(Color color,GAMEOBJECT_TYPE typeValue = GO_BALL);
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif