#ifndef ASN2_H
#define ASN2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class Asn2 : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;
	const double MOVE_SPEED = 80.f;
public:
	Asn2();
	~Asn2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	bool CheckCollision(GameObject* go, GameObject* other, double dt);
	float CheckCollision2(GameObject* go1, GameObject* go2);
	void CollisionResponse(GameObject* go1, GameObject* go2);
	void PhysicSIM(double dt);
	void RenderGO(GameObject *go);
	float CalcDist(Vector3 first, Vector3 second);
	float debounce;
	float m_estimatedTime, timeTaken;
	GameObject* FetchGO();
	
	enum GameState
	{
		GS_MAINMENU,
		GS_GAME,
		GS_GAMEOVER,
		GS_WIN,
		NUM_GS,
	};

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
	Vector3 u1, u2, v1, v2, ke1, ke2;

	bool ballSpawn;
	bool check1;
	bool check2;
	bool ironBall;
	bool splitBall;
	float SBTimer;
	float IBTimer;

	GameObject* player;
	GameObject* playerLeft;
	GameObject* playerRight;
	GameObject* ghostBall;
	GameObject* ball;
	GameObject* split;
	int retries;
	int score;

	Vector3 initialPos;
	GameState gs;

	bool homeScreenCursor;
	float homeScreenDebounce;

	// ai stuff

	// fsm1 boss
	float bossCD;
	GameObject* boss;
	int attCount;
	bool playerInRange;
	float rangeBoss;

	//fsm2 mob
	bool playerInRangeMob;
	float rangeMob;
	GameObject* mob;

};

#endif