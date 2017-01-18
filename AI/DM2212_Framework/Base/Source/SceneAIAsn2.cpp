#include "SceneAIAsn2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include <stdlib.h>
#include "MyMath.h"


SceneAIAsn2::SceneAIAsn2()
{
}

SceneAIAsn2::~SceneAIAsn2()
{
}

void SceneAIAsn2::Init()
{
	SceneBase::Init();

	Math::InitRNG();

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	state = GAMEPLAY;
	//tempGS = STAGE1;

	/*magician = FetchGO();
	magician->type = GameObject::GO_MAGICIAN;
	magician->active = false;
	magician->pos.Set(0, 10, 0);
	magician->vel.Set(10, 0, 0);
	magician->scale.Set(5, 5, 5);
	magician->Dmg = 10;
	magician->job = GameObject::JOB_MAGICIAN;
	magicianRechargeTimer = 0.f;*/

	/*warrior = FetchGO();
	warrior->type = GameObject::GO_WARRIOR;
	warrior->pos.Set(0, 10, 0);
	warrior->vel.Set(10, 0, 0);
	warrior->scale.Set(5, 5, 1);
	warrior->Dmg = 10;
	warrior->HP = 150;
	warrior->job = GameObject::JOB_WARRIOR;*/

	archer = FetchGO();
	archer->type = GameObject::GO_ARCHER;
	archer->active = true;
	archer->pos.Set(0, 10, 0);
	archer->vel.Set(10, 0, 0);
	archer->scale.Set(5, 5, 5);
	archer->Dmg = 10;
	archer->job = GameObject::JOB_ARCHER;

	/*healer = FetchGO();
	healer->type = GameObject::GO_HEALER;
	healer->pos.Set(0, 10, 0);
	healer->vel.Set(5, 0, 0);
	healer->scale.Set(5, 5, 5);
	healer->Dmg = 0;
	healer->job = GameObject::JOB_HEALER;
	healerAOETimer = 14.f;
	healerCooldown = 0.f;*/

	for (int i = 0; i < 10; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_MOB));
	}
	
	// Main menu words
	mainMenuOptions[0] = "Start Game";
	mainMenuOptions[1] = "Quit";
	mainMenuCursor = 0;
	cursorDebounce = 0.f;

	warriorAFrame = 0.f;
	archerAFrame = 0.f;
	mobAFrame = 0.f;
	WarriorGuard = -1;


	mobSpawnTimer = 5.f;
	FPS = 0;
	mobcount = 0;
	quitGame = false;

}

void SceneAIAsn2::Update(double dt)
{
	if (Application::IsKeyPressed(VK_ESCAPE))
		quitGame = true;
	SceneBase::Update(dt);
	FPS = 1 / dt;
	//std::cout << warrior->HP << " " << mob->HP  << " " <<  mob->currentState << " " << warrior->Def << std::endl;
	//std::cout << healer->currentState << healer->vel << std::endl;
	if (state == GAMEPLAY)
	{
		mobSpawnTimer -= dt;
		if (mobSpawnTimer <= 0.f && mobcount < 1)
		{
			GameObject* mob = new GameObject(GameObject::GO_MOB);
			mob->type = GameObject::GO_MOB;
			mob->pos.Set(100, 10, 0);
			mob->vel.Set(-10, 0, 0);
			mob->scale.Set(3, 3, 3);
			mob->Dmg = 5;
			mob->job = GameObject::JOB_MOB;
			mob->active = true;
			mobSpawnTimer = 5.f;
			mobcount++;
			m_goList.push_back(mob);
		}

		//if (warrior->active)
		//{
		//	if (warrior->HP > 0)
		//	{
		//		if (warrior->HP < 100)
		//		{
		//			warrior->isHealTarget = true;
		//		}
		//		if (warrior->currentState == GameObject::STATE_MOVE)
		//		{
		//			for (auto mob : m_goList)
		//			{
		//				if (mob->job == GameObject::JOB_MOB)
		//				{
		//					if (DistXY(warrior->pos, mob->pos) < 50.f && mob->active)
		//					{
		//						warrior->vel.SetZero();
		//						warrior->currentState = GameObject::STATE_ATTACK;
		//						//mob->currentState = GameObject::STATE_ATTACK;
		//					}
		//					if (warrior->currentState != GameObject::STATE_ATTACK && mob->active == false)
		//						warrior->vel.Set(10, 0, 0);
		//				}

		//			}

		//		}
		//		else if (warrior->currentState == GameObject::STATE_ATTACK)
		//		{
		//			for (auto mob : m_goList)
		//			{
		//				if (mob->job == GameObject::JOB_MOB)
		//				{
		//					if (DistXY(warrior->pos, mob->pos) <= 50.f && mob->active)
		//					{
		//						warriorAFrame -= 1.f;

		//						if (warriorAFrame <= 0.f)
		//						{
		//							WarriorGuard = Math::RandIntMinMax(0, 3);
		//							warrior->Def = 0;
		//							if (WarriorGuard == 0)
		//							{
		//								warrior->Def = 3;

		//							}
		//							else if (WarriorGuard > 0)
		//							{
		//								mob->HP -= warrior->Dmg;
		//								if (mob->HP <= 0)
		//									warrior->currentState = GameObject::STATE_MOVE;
		//							}
		//							warriorAFrame = 50.f;
		//						}

		//					}
		//				}
		//			}

		//		}
		//	}
		//	else
		//	{
		//		warrior->currentState = GameObject::STATE_DEAD;
		//		warrior->active = false;
		//		for (auto go : m_goList)
		//		{
		//			if (go->active == false)
		//			{
		//				go = NULL;
		//			}

		//		}

		//	}

		//}

		if (archer->active)
		{
			if (archer->HP > 0)
			{
				if (archer->HP < 70)
				{
					archer->isHealTarget = true;
				}
				if (archer->currentState == GameObject::STATE_MOVE)
				{
					for (auto mob : m_goList)
					{
						if (mob->job == GameObject::JOB_MOB)
						{
							if (DistXY(archer->pos, mob->pos) < 1000.f && mob->active)
							{
								archer->vel.SetZero();
								archer->currentState = GameObject::STATE_ATTACK;
							}
							if (archer->currentState != GameObject::STATE_ATTACK && mob->active == false)
								archer->vel.Set(10, 0, 0);
						}
					}
				}
				else if (archer->currentState == GameObject::STATE_ATTACK)
				{
					for (auto mob : m_goList)
					{
						
						if (mob->job == GameObject::JOB_MOB)
						{
							archerAFrame -= 1.f;
							if (archerAFrame <= 0.f)
							{
								GameObject* arrow = FetchGO();
								arrow->type = GameObject::GO_ARROW;
								arrow->pos.Set(archer->pos.x, archer->pos.y, 0);
								arrow->vel.Set(30, 0, 0);
								arrow->scale.Set(3, 3, 3);
								arrow->Dmg = 15;
								std::cout << "shoot" << std::endl;
								std::cout << arrow->pos << "" << mob->pos << std::endl;
								std::cout << (DistXY(arrow->pos, mob->pos)) << std::endl;
								if (DistXY(arrow->pos, mob->pos) < 50.f && mob->active)
								{
									mob->HP -= 50;
									arrow->active = false;
									if (mob->HP <= 0)
										archer->currentState = GameObject::STATE_MOVE;
								}
								archerAFrame = 100.f;
							}
						}
					}
				}
				else
				{
					archer->currentState == GameObject::STATE_DEAD;
					archer->active = false;
					for (auto go : m_goList)
					{
						if (go->active == false)
							go = NULL;
					}
				}
			}
		}

		for (auto PerMob : m_goList)
		{
			if (PerMob->job == GameObject::JOB_MOB)
			{
				if (PerMob->active)
				{
					if (PerMob->HP > 0)
					{
						if (PerMob->currentState == GameObject::STATE_MOVE) // 4
						{

							for (auto go : m_goList)
							{
								if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_MOB/* || go->active == false*/)
									continue;
								if (DistXY(go->pos, PerMob->pos) <= 50.f && go->active == true)
								{
									PerMob->vel.SetZero();
									PerMob->currentState = GameObject::STATE_ATTACK;
								}

								if (DistXY(go->pos, PerMob->pos) > 50.f /*&& PerMob->currentState != GameObject::STATE_ATTACK*/ && go->active == false)
								{
									PerMob->vel.Set(-10, 0, 0);
									PerMob->currentState = GameObject::STATE_MOVE;
								}
							}

						}
						else if (PerMob->currentState == GameObject::STATE_ATTACK) // 3 
						{
							//GameObject* temp;
							for (auto go : m_goList)
							{
								if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_MOB/* || go->active == false*/)
									continue;
								//temp = go;
								if (go->active == false)
								{
									PerMob->currentState = GameObject::STATE_MOVE;
									PerMob->vel.Set(-10, 0, 0);
								}
								if (DistXY(PerMob->pos, go->pos) <= 50.f && go->active == true)
								{
									mobAFrame -= 1.f;
									if (mobAFrame <= 0.f)
									{
										int tempDmg;
										tempDmg = (PerMob->Dmg - go->Def);
										if (tempDmg <= 0)
											tempDmg = 0;

										go->HP -= tempDmg;
										

										mobAFrame = 50.f;
									}
								}

							}
						}
					}
					else
					{
						PerMob->currentState = GameObject::STATE_DEAD;
						PerMob->active = false;
						mobcount--;
					}

				}
			}
		}



		//if (magician->active) // Magician codes here
		//{
		//	if (magician->currentState == GameObject::STATE_MOVE)
		//	{
		//		if (DistXY(magician->pos, mob->pos) < 200.f && mob->active)
		//		{
		//			if (magicianRechargeTimer <= 0.f)
		//				magician->currentState = GameObject::STATE_ATTACK;
		//			magician->vel.SetZero();
		//			mob->vel.SetZero();
		//			magician->HP = 10;
		//		}
		//		else
		//		{
		//			magician->vel.Set(10, 0, 0);
		//			mob->vel.Set(-10, 0, 0);
		//		}
		//			

		//	}
		//	else if (magician->currentState == GameObject::STATE_ATTACK)
		//	{
		//		if (magicianRechargeTimer <= 0.f)
		//		{
		//			//GameObject* fireball = FetchGO();
		//			//fireball->type = GameObject::GO_BULLET;
		//			//fireball->pos = magician->pos;
		//			//fireball->scale = bulletSize;
		//			//fireball->vel.Set(5, 0, 0);
		//			//fireball->active = true;
		//			//magicianRechargeTimer = 2.f;
		//			//magician->currentState = GameObject::STATE_RECHARGE;
		//		}
		//	}
		//	else if (magician->currentState == GameObject::STATE_RECHARGE)
		//	{
		//		magicianRechargeTimer -= dt;
		//		if (magicianRechargeTimer <= 0.f)
		//		{
		//			magician->currentState = GameObject::STATE_MOVE;
		//		}
		//	}
		//}

		//// Healer Codes here
		//if (healer->active)
		//{
		//	if (healer->HP > 0)
		//	{
		//		healerCooldown -= dt;
		//		healerAOETimer -= dt;
		//		if (healerAOETimer <= 0.f && healer->currentState != GameObject::STATE_HEAL)
		//		{
		//			for (auto go : m_goList)
		//			{
		//				if (go->job == GameObject::JOB_NONE || go->active == false)
		//					continue;
		//				if (DistXY(healer->pos, go->pos) < 400.f)
		//				{
		//					/*if (go->job == GameObject::JOB_MOB)
		//					{
		//						healer->currentState = GameObject::STATE_MOVE;
		//						healer->vel.SetZero();
		//					}*/
		//					if (go->job == GameObject::JOB_WARRIOR)
		//					{
		//						if (go->HP <= 150 && go->HP >= 90)
		//							go->HP = 150;
		//						else
		//							go->HP += 60;
		//					}
		//					else
		//					{
		//						if (go->HP <= 100 && go->HP >= 50)
		//							go->HP = 100;
		//						else
		//							go->HP += 50;
		//					}
		//				}
		//			}
		//			healerAOETimer = 15.f;
		//		}
		//		if (healer->currentState == GameObject::STATE_MOVE)
		//		{
		//			for (auto go : m_goList)
		//			{
		//				if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_HEALER || go->active == false)
		//					continue;
		//				if (go->job == GameObject::JOB_MOB)
		//				{
		//					if (DistXY(healer->pos, go->pos) < 200.f && go->active)
		//					{
		//						healer->vel.SetZero();
		//					}
		//					else if (go->active == false)
		//					{
		//						healer->vel.Set(5, 0, 0);
		//					}
		//				}
		//				else
		//				{
		//					if (DistXY(healer->pos, go->pos) < 200.f && go->active)
		//					{
		//						healer->vel.SetZero();
		//						if (go->isHealTarget == true)
		//						{
		//							healer->currentState = GameObject::STATE_HEAL;
		//							//go->isHealTarget = true;
		//						}
		//					}
		//					else
		//					{
		//						healer->vel.Set(5, 0, 0);
		//					}
		//				}

		//			}

		//		}
		//		else if (healer->currentState == GameObject::STATE_HEAL)
		//		{
		//			for (auto go : m_goList)
		//			{
		//				if (go->isHealTarget)
		//				{
		//					if (healerCooldown <= 0.f && (DistXY(healer->pos, go->pos) < 200.f))
		//					{
		//						healer->vel.SetZero();
		//						go->HP += 10;
		//						healerCooldown = 2.f;
		//						go->isHealTarget = false;
		//						healer->currentState = GameObject::STATE_MOVE;
		//						break;
		//					}
		//				}
		//			}
		//		}
		//	}
		//	else
		//	{
		//		healer->currentState == GameObject::STATE_DEAD;
		//		healer->active = false;
		//	}
		//}
	}

	// Collision checks here
	for (auto go : m_goList)
	{
		if (go->active)
		{
			go->pos += go->vel* (float)dt;
			if (go->type == GameObject::GO_MOB)
			{
				if (go->pos.x > m_worldWidth)
					go->currentState = GameObject::STATE_DEAD;
				if (go->pos.x < 0)
					go->currentState = GameObject::STATE_DEAD;
			}
		}
	}
}

GameObject* SceneAIAsn2::FetchGO()
{
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = *it;
		if (go->active == false)
		{
			go->active = true;
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_MOB));
	}
	GameObject* go = *(m_goList.end() - 1);
	go->active = true;
	return go;
}

void SceneAIAsn2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -2);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(meshList[GEO_BACKGROUND], false);
	modelStack.PopMatrix();

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}


	for (auto go : m_goList)
	{
		if (/*go->type == GameObject::GO_WARRIOR &&*/ go->active)
		{
			std::stringstream ss;
			ss.precision(3);
			ss << go->HP;
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y + 5, 1);
			modelStack.Scale(5, 5, 5);
			RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1));
			modelStack.PopMatrix();	
		}

		if (healerAOETimer < 0
			|| healerAOETimer > 14)
			RenderTextOnScreen(meshList[GEO_TEXT], "Healer Used AOE Heal", Color(1, 1, 1), 5, 10, 40);

		else if (go->isHealTarget /*&& healerCooldown > 0*/)
		{
			
			if (go->job ==GameObject::JOB_WARRIOR)
				RenderTextOnScreen(meshList[GEO_TEXT], "Healer Healed Warrior", Color(1, 1, 1), 5, 10, 40);
		}

	}

	if (WarriorGuard == 0 && warrior->active)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Warrior is guarding", Color(1, 1, 1), 5, 10, 50);
	}

	//if (healer->currentState == GameObject::STATE_HEAL)
	//{
	//	RenderTextOnScreen(meshList[GEO_TEXT], "Healer is healing party members", Color(1, 1, 1), 3, 10, 40);
	//}

}

void SceneAIAsn2::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_HEALER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_HEALER], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WARRIOR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_WARRIOR], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARCHER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_WARRIOR], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		break;
	default:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAIAsn2::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	/*if (m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}*/
}

float SceneAIAsn2::DistXY(Vector3 first, Vector3 second)
{
	return abs(((first - second).LengthSquared()));
}

int SceneAIAsn2::RNG(int y)
{
	srand(time(NULL));
	return rand()%y;

}