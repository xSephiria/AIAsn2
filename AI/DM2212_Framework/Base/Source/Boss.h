#ifndef BOSS_H
#define BOSS_H
#include "AI.h"

class Boss : public AI
{
public:
	virtual void Update(double dt);
};
  
#endif BOSS_H