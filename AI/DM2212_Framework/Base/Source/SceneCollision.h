#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneCollision : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	bool CheckCollision(GameObject* go, GameObject* other, double dt);
	float CheckCollision2(GameObject* go1, GameObject* go2);
	void CollisionResponse(GameObject* go1, GameObject* go2);
	void PhysicSIM(double dt);
	void RenderGO(GameObject *go);
	
	float debounce;
	float m_estimatedTime, timeTaken;
	GameObject* FetchGO();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	int m_objectCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2,	ke1,ke2;

};

#endif