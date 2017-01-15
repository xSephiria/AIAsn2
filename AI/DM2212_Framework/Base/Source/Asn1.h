#ifndef ASN1_H
#define ASN1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class Asn1 : public SceneBase
{
	static const int MAX_SPEED = 30;
	static const int BULLET_SPEED = 40;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	Asn1();
	~Asn1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	enum GameState
	{
		MAIN_MENU,
		STAGE1,
		STAGE2,
		STAGEFINAL,
		STAGE_1_CLEARED,
		STAGE_2_CLEARED,
		DEAD,
		VICTORY,
		GAME_OVER,
		NUM_GS
	};

	GameState gs;
	GameState tempGS;
	
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;

	float enemySpawnTimer;
	float asteroidZ;

	// Player stuff
	GameObject *m_ship;
	Vector3 m_force;
	int m_objectCount;
	int m_lives;
	int m_score;
	int playerHP;
	float rotateShip = 0.f;
	Mtx44 shipRotation;

	//Bullet stuff
	static const int fireRate = 5;
	float firingDebounce;
	const Vector3 bulletSize = Vector3(0.5f, 0.5f, 0.5f);
	Mtx44 bulletRotation;

	// boss stuff
	GameObject* Boss;
	float bossFireDebounce;
	float bossFireRate = 5;
	int bossHP;
	float rotateBoss = 0.f;
	Mtx44 bossRotation;

	// Main Menu stuff
	bool mainMenuCursor;
	std::string mainMenuOptions[2];
	float cursorDebounce;

};

#endif