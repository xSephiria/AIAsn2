#pragma once

#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include "SceneBase.h"
#include "Vector3.h"
#include "GameObject.h"

using namespace std;

enum States
{
	Explore,
	Scavenging,
	Drink,
	Eat,
	Dead,
	Jump,
};

class CSurvival : public SceneBase
{
public:
	CSurvival();
	~CSurvival();

	string itos(const long value);
	void Init();
	void Update(double dt);
	void Simulation();
	void Render();
	void Exit();
	
	void PhysicSIM(double dt);
	void RenderGO(GameObject *go);
	GameObject* FetchGO();
	void CollisionResponse(GameObject* go1, GameObject* go2);
	bool CheckCollision(GameObject* go, GameObject* other, double dt);
	bool DistXY(Vector3 first, Vector3 second, float dist);

private:

	Vector3 PlayerPosition;
	Vector3 TargetPosition;
	Vector3 ScavengingPosition;

	std::vector<GameObject *> m_goList;

	GameObject* player;
	GameObject* ScavengingPoint;
	GameObject* rocks[6];
	//GameObject* rocks2;
	//GameObject* rocks3;
	//GameObject* rocks4;
	//GameObject* rocks5;
	//GameObject* rocks6;

	//irrklang::ISoundEngine* theSoundEngine;
	//irrKlang

	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;

	int Rand(int lowerLimit, int upperLimit);

	//Action Bars
	float CHungerBar;
	float CThirstBar;
	int HungerBarCounter;
	int ThirstBarCounter;
	int ActionTimer;
	int PickUpDelay;
	int RelocateTimer;
	int JumpTimer;

	//Items
	int Food;
	int Water;

	bool AutoRun;
	bool FoodHunt;
	bool WaterHunt;
	bool Eating;
	bool Drinking;
	bool Jumping;

	bool ReLocate;
	bool ReSpawn;

	States state;
};

