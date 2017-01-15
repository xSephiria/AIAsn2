#ifndef AI_H
#define AI_H

#include "Vector3.h"

enum STATES
{
	STATE_IDLE,
	STATE_ATTACK,
	STATE_BUFF,
	STATE_DEAD,
	NUM_STATES
};

class AI
{
public:
	AI();
	AI(int HP, STATES State, bool active, Vector3 position);
	virtual ~AI();
	bool active;

	virtual void Update(double dt);
	void setState(STATES newState);
	STATES getState();
protected:
	STATES currentState;
	int HP;
	Vector3 position;
};

#endif AI_H