#ifndef SCENEAIASN2_H
#define SCENEAIASN2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "MessageBoard.h"
#include <sstream>

enum UNITS{
	WARRIOR,
	HEALER,
	ARCHER,
	MAGICIAN,
	MOB,
	UNITS_ALL,
};

enum STATE{
	MAINMENU,
	GAMEPLAY,
	LOSE,
	WIN,
	STATE_ALL,
};

class SceneAIAsn2 : public SceneBase
{
public:
	SceneAIAsn2();
	~SceneAIAsn2();

private:
	void Init();
	void Update(double dt);
	void RenderGO(GameObject *go);
	void Render();
	void Exit();



	GameObject* FetchGO();

	float DistXY(Vector3 first, Vector3 second);
	int RNG(int y);

	UNITS units;
	STATE state;
	MessageBoard msgBoard;

	//Parameters
	std::vector<GameObject *> m_goList;
	float m_worldHeight;
	float m_worldWidth;

	float warriorAFrame;
	float archerAFrame;

	float mobAFrame;

	GameObject* warrior;
	int WarriorGuard;
	bool isGuarding;

	GameObject* archer;
	GameObject* arrow;
	void CreateArrow();

	GameObject* HeroTower;

	GameObject* EnemyTower;

	GameObject* magician;
	GameObject* fireball;
	float magicianRechargeTimer;

	GameObject* mob;
	float mobSpawnTimer;
	void CreateMob();

	GameObject* healer;
	float healerCooldown;
	float healerAOETimer;


	// Main Menu stuff
	bool mainMenuCursor;
	std::string mainMenuOptions[2];
	float cursorDebounce;

	int mobcount;
	int arrowcount;
	float FPS;

	void ArcherAnimation();
	void WarriorAnimation();
	void HealerAnimation();
	void MagicianAnimation();
	float ArcherAnimCounter;
	float WarriorAnimCounter;
	float HealerAnimCounter;
	float MagicianAnimCounter;

	float WarriorGuardTime;

	bool ArcherShoot;
	bool HealerHeal;
	bool WarriorAttack;
	bool MagicianAttack;
	bool MagicianEnemy;

};

#endif