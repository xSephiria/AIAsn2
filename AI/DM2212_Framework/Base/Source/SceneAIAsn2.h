#ifndef SCENEAIASN2_H
#define SCENEAIASN2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

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


	UNITS units;
	STATE state;

	//Parameters
	std::vector<GameObject *> m_goList;
	float m_worldHeight;
	float m_worldWidth;

	GameObject* magician;
	float magicianRechargeTimer;

	GameObject* mob;



	// Main Menu stuff
	bool mainMenuCursor;
	std::string mainMenuOptions[2];
	float cursorDebounce;


};

#endif