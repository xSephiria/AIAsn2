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
	magician->active = true;
	magician->pos.Set(0, 12, -1);
	magician->vel.Set(10, 0, 0);
	magician->scale.Set(10, 10, 5);
	magician->Dmg = 10;
	magician->job = GameObject::JOB_MAGICIAN;
	magicianRechargeTimer = 0.f;*/

	fireball = new GameObject(GameObject::GO_FIREBALL);
	fireball->active = false;
	fireball->pos.Set(0, 0, 0);
	fireball->scale.Set(1,1,1);
	fireball->vel.Set(5, 0, 0);
	fireball->Dmg = 20;
	m_goList.push_back(fireball);

	HeroTower = FetchGO();
	HeroTower->type = GameObject::GO_HEROTOWER;
	HeroTower->active = true;
	HeroTower->pos.Set(12, 32, -1);
	HeroTower->scale.Set(35, 50, 10);
	HeroTower->vel.Set(0, 0, 0);
	HeroTower->HP = 1000;
	HeroTower->job = GameObject::JOB_HTOWER;

	EnemyTower = FetchGO();
	EnemyTower->type = GameObject::GO_ENEMYTOWER;
	EnemyTower->active = true;
	EnemyTower->pos.Set(165, 32, 0);
	EnemyTower->scale.Set(35, 50, 10);
	EnemyTower->vel.Set(0, 0, 0);
	EnemyTower->HP = 2000;
	EnemyTower->job = GameObject::JOB_ETOWER;

	warrior = FetchGO();
	warrior->type = GameObject::GO_WARRIOR;
	warrior->pos.Set(0, 12, 0);
	warrior->vel.Set(10, 0, 0);
	warrior->scale.Set(10, 10, 1);
	warrior->Dmg = 10;
	warrior->HP = 150;
	warrior->job = GameObject::JOB_WARRIOR;

	/*archer = FetchGO();
	archer->type = GameObject::GO_ARCHER;
	archer->active = true;
	archer->pos.Set(HeroTower->pos.x, 12, 0);
	archer->vel.Set(10, 0, 0);
	archer->scale.Set(10, 10, 5);
	archer->Dmg = 10;
	archer->job = GameObject::JOB_ARCHER;

	healer = FetchGO();
	healer->type = GameObject::GO_HEALER;
	healer->pos.Set(HeroTower->pos.x, 12, 0);
	healer->vel.Set(8, 0, 0);
	healer->scale.Set(10, 10, 5);
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
	isGuarding = false;

	ArcherAnimCounter = 0;
	HealerAnimCounter = 0;
	WarriorAnimCounter = 0;
	MagicianAnimCounter = 0;
	WarriorGuardTime = 10;

	ArcherShoot = false;
	HealerHeal = false;
	WarriorAttack = false;
	MagicianAttack = false;
	arrowcount = 0;

	mobSpawnTimer = 5.f;
	FPS = 0;
	mobcount = 0;
	quitGame = false;

}

void SceneAIAsn2::Update(double dt)
{
	msgBoard.Update(dt);
	if (Application::IsKeyPressed(VK_ESCAPE))
		quitGame = true;
	SceneBase::Update(dt);
	FPS = 1 / dt;

	//ArcherAnimation();
	//HealerAnimation();
	WarriorAnimation();
	std::cout << WarriorGuardTime << " " << isGuarding << std::endl;

	for (std::vector<GameObject *> ::iterator it = m_goList.begin(); it != m_goList.end(); )
	{
		if ((*it)->type == GameObject::GO_FIREBALL)
		{
			it++;
			continue;
		}
		if ((*it)->active == false)
		{
			delete *it;
			it = m_goList.erase(it);
		}
		else
			it++;
	}

	if (state == GAMEPLAY)
	{
		mobSpawnTimer -= dt;
		if (mobSpawnTimer <= 0.f && mobcount < 1)
		{
			mob = new GameObject(GameObject::GO_MOB);
			mob->type = GameObject::GO_MOB;
			mob->pos.Set(EnemyTower->pos.x, 10, 1);
			mob->vel.Set(-10, 0, 0);
			mob->scale.Set(3, 3, 3);
			mob->Dmg = 5;
			mob->job = GameObject::JOB_MOB;
			mob->currentState = GameObject::STATE_MOVE;
			mob->active = true;
			mobSpawnTimer = 5.f;
			mobcount++;
			m_goList.push_back(mob);
		}

		if (ArcherShoot)
		{
			arrow = new GameObject(GameObject::GO_ARROW);
			arrow->type = GameObject::GO_ARROW;
			arrow->pos.Set(0, 0, 0);
			arrow->scale.Set(1, 1, 1);
			arrow->vel.Set(15, 0, 0);
			arrow->Dmg = 5;
			m_goList.push_back(arrow);
		}



		if (warrior->active)
		{
			if (warrior->HP > 0)
			{
				if (warrior->HP < 100)
				{
					warrior->isHealTarget = true;
				}
				if (warrior->currentState == GameObject::STATE_MOVE)
				{
					for (auto mob : m_goList)
					{
						if (warrior->currentState == GameObject::STATE_MOVE)
						{
							for (auto mob : m_goList)
							{
								if (mob->job == GameObject::JOB_MOB || mob->job == GameObject::JOB_ETOWER)
								{
									if (mob->job == GameObject::JOB_ETOWER && DistXY(warrior->pos, mob->pos) < 150.f && mob->active)
									{
										warrior->vel.SetZero();
										warrior->currentState = GameObject::STATE_ATTACK;
										break;
									}
									if (mob->job == GameObject::JOB_MOB && DistXY(warrior->pos, mob->pos) < 50.f && mob->active)
									{
										warrior->vel.SetZero();
										warrior->currentState = GameObject::STATE_ATTACK;
										break;
									}
									
									else
									{
										warrior->vel.Set(10, 0, 0);
									}
								}
							}
						}
					}
				}
				else if (warrior->currentState == GameObject::STATE_ATTACK)
				{
					bool EnemyAlive = false;
					for (auto mob : m_goList)
					{
						if (mob->job == GameObject::JOB_MOB || mob->job == GameObject::JOB_ETOWER)
						{
							if (mob->job == GameObject::JOB_ETOWER && DistXY(warrior->pos, mob->pos) <= 200.f && mob->active)
							{
								isGuarding = false;
								EnemyAlive = true;
								if (WarriorAttack)
									mob->HP -= warrior->Dmg;
								WarriorAttack = false;
								break;
							}
							if (mob->job == GameObject::JOB_MOB && DistXY(warrior->pos, mob->pos) <= 50.f && mob->active)
							{
								EnemyAlive = true;
								WarriorGuard = Math::RandIntMinMax(0, 3);
								warrior->Def = 0;
								if (WarriorGuard == 0)
								{
									isGuarding = true;
									warrior->Def = 3;
									WarriorAttack = false;
									break;
								}
								else if (WarriorGuard > 0)
								{
									isGuarding = false;
									if (WarriorAttack)
									{
										mob->HP -= warrior->Dmg;
										WarriorAttack = false;
									}
									break;
								}								
							}
							
						}
					}
					if (!EnemyAlive)
					{
						warrior->currentState = GameObject::STATE_MOVE;
					}
				}
			}
			else
			{
				warrior->currentState = GameObject::STATE_DEAD;
				warrior->active = false;
				for (auto go : m_goList)
				{
					if (go->active == false)
					{
						go = NULL;
					}
				}
			}
		}

		//if (archer->active)
		//{
		//	for (auto arrowList : m_goList)
		//	{
		//		if (arrowList->type == GameObject::GO_ARROW)
		//		{
		//			for (std::vector<GameObject*>::iterator mob = m_goList.begin(); mob != m_goList.end(); mob++)
		//			{
		//				if (!(*mob)->active || (*mob)->job != GameObject::JOB_MOB || !(arrowList)->active)
		//					continue;
		//				if ( (*mob)->job == GameObject::JOB_MOB && DistXY(arrowList->pos, (*mob)->pos) < 100.f)
		//				{
		//					(*mob)->HP -= arrowList->Dmg;
		//					arrowList->active = false;
		//					break;
		//				}
		//			}
		//			if (DistXY(arrowList->pos, EnemyTower->pos) < 100.f && EnemyTower->active)
		//			{
		//				EnemyTower->HP -= arrowList->Dmg;
		//				arrowList->active = false;
		//				break;
		//			}
		//		}
		//	}
		//	if (archer->HP > 0)
		//	{
		//		if (archer->HP < 70)
		//		{
		//			if (archer->isHealTarget == false)
		//			{
		//				archer->isHealTarget = true;
		//				msgBoard.addMessage("Archer", "Healer", "Heal Me...");
		//			}
		//		}
		//		if (archer->currentState == GameObject::STATE_MOVE)
		//		{
		//			for (auto mob : m_goList)
		//			{
		//				if (mob->job == GameObject::JOB_MOB || mob->job == GameObject::JOB_ETOWER)
		//				{
		//					if (mob->job == GameObject::JOB_MOB && DistXY(archer->pos, mob->pos) < 500.f && mob->active)
		//					{
		//						archer->vel.SetZero();
		//						archer->currentState = GameObject::STATE_ATTACK;
		//						break;
		//					}
		//					if (mob->job == GameObject::JOB_ETOWER && DistXY(archer->pos, mob->pos) < 2000.f && mob->active)
		//					{
		//						archer->vel.SetZero();
		//						archer->currentState = GameObject::STATE_ATTACK;
		//						break;
		//					}
		//					else
		//					{
		//						archer->vel.Set(10, 0, 0);
		//					}
		//				}
		//			}
		//		}
		//		else if (archer->currentState == GameObject::STATE_ATTACK)
		//		{
		//			bool EnemyAlive = false;
		//			for (auto mob : m_goList)
		//			{
		//				if (mob->active == false)
		//					continue;
		//				if (mob->job == GameObject::JOB_MOB && DistXY(archer->pos, mob->pos) < 500.f)
		//				{
		//					EnemyAlive = true;
		//					if (ArcherShoot == true && mob->active)
		//					{
		//						arrow->pos = archer->pos;
		//						arrow->active = true;
		//						ArcherShoot = false;
		//						break;
		//					}
		//					
		//				}
		//				if (mob->job == GameObject::JOB_ETOWER && DistXY(archer->pos, mob->pos) < 2000.f)
		//				{
		//					EnemyAlive = true;
		//					if (ArcherShoot == true && mob->active)
		//					{
		//						arrow->pos = archer->pos;
		//						arrow->active = true;
		//						ArcherShoot = false;
		//						break;
		//					}
		//				}
		//				
		//			}
		//			if (!EnemyAlive)
		//			{
		//				archer->currentState = GameObject::STATE_MOVE;
		//			}
		//		}
		//		
		//	}
		//	else
		//	{
		//		archer->currentState == GameObject::STATE_DEAD;
		//		archer->active = false;
		//		for (auto go : m_goList)
		//		{
		//			if (go->active == false)
		//				go = NULL;
		//		}
		//	}
		//}

		for (auto PerMob : m_goList)
		{
			if (PerMob->job == GameObject::JOB_MOB)
			{
				if (PerMob->active)
				{
					if (PerMob->HP > 0)
					{
						if (PerMob->currentState == GameObject::STATE_MOVE)
						{
							for (auto go : m_goList)
							{
								if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_ETOWER || go->job == GameObject::JOB_MOB/* || go->active == false*/)
									continue;
								if (go->active == true && DistXY(go->pos, PerMob->pos) <= 50.f)
								{
									PerMob->vel.SetZero();
									PerMob->currentState = GameObject::STATE_ATTACK;
									break;
								}

								if (go->active == false && DistXY(go->pos, PerMob->pos) > 50.f)
								{
									PerMob->vel.Set(-10, 0, 0);
									PerMob->currentState = GameObject::STATE_MOVE;
									break;
								}
							}

						}
						else if (PerMob->currentState == GameObject::STATE_ATTACK) // 3 
						{
							GameObject* temp;
							for (auto go : m_goList)
							{
								if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_ETOWER || go->job == GameObject::JOB_MOB/* || go->active == false*/)
									continue;
								temp = go;
								if (go->active == false)
								{
									PerMob->currentState = GameObject::STATE_MOVE;
									PerMob->vel.Set(-10, 0, 0);
									break;
								}
								if (DistXY(PerMob->pos, go->pos) <= 50.f && go->active == true)
								{
									mobAFrame -= 1.f;
									if (mobAFrame <= 0.f)
									{
										if (go->type == GameObject::GO_WARRIOR)
										{
											int tempDmg;
											tempDmg = (PerMob->Dmg - go->Def);
											if (tempDmg <= 0)
												tempDmg = 0;

											go->HP -= tempDmg;
											mobAFrame = 50.f;
										}
										else
										{
											int tempDmg;
											tempDmg = (PerMob->Dmg - go->Def);
											if (tempDmg <= 0)
												tempDmg = 0;

											go->HP -= tempDmg;
											mobAFrame = 50.f;
										}
										
									}
									break;
								}

							}
						}
					}
					else
					{
						PerMob->currentState = GameObject::STATE_DEAD;
						PerMob->active = false;
						mobcount--;
						break;
					}

				}
			}
		}



		//if (magician->active) // Magician codes here
		//{
		//	if (magician->HP < 70)
		//	{
		//		if (magician->isHealTarget == false)
		//		{
		//			magician->isHealTarget = true;
		//			msgBoard.addMessage("Magician", "Healer", "Heal Me...");
		//		}
		//	}
		//	if (fireball->active)
		//	{
		//		for (std::vector<GameObject*>::iterator mob = m_goList.begin(); mob != m_goList.end(); mob++)
		//		{
		//			if (!(*mob)->active || (*mob)->job != GameObject::JOB_MOB)
		//				continue;
		//			if (DistXY(fireball->pos, (*mob)->pos) < 50.f)
		//			{
		//				(*mob)->HP -= fireball->Dmg;
		//				fireball->active = false;
		//				break;
		//			}
		//		}
		//		if (DistXY(fireball->pos, EnemyTower->pos) < 50.f && EnemyTower->active)
		//		{
		//			EnemyTower->HP -= fireball->Dmg;
		//			fireball->active = false;
		//			//break;
		//		}
		//	}
		//	if (magician->currentState == GameObject::STATE_MOVE)
		//	{
		//		magician->vel.Set(10, 0, 0);
		//		for (std::vector<GameObject*>::iterator mob = m_goList.begin(); mob != m_goList.end(); mob++)
		//		{
		//			if (!(*mob)->active || (*mob)->job != GameObject::JOB_MOB && (*mob)->job != GameObject::JOB_ETOWER)
		//				continue;
		//			if ((*mob)->job == GameObject::JOB_MOB && DistXY(magician->pos, (*mob)->pos) < 200.f && (*mob)->active)
		//			{
		//				if (magicianRechargeTimer <= 0.f)
		//				{
		//					magician->currentState = GameObject::STATE_ATTACK;
		//					magician->vel.SetZero();
		//					break;
		//				}
		//			}
		//			if ((*mob)->job == GameObject::JOB_ETOWER && DistXY(magician->pos, (*mob)->pos) < 1000.f && (*mob)->active)
		//			{
		//				if (magicianRechargeTimer <= 0.f)
		//				{
		//					magician->currentState = GameObject::STATE_ATTACK;
		//					magician->vel.SetZero();
		//					break;
		//				}
		//			}
		//			
		//		}
		//	}
		//	else if (magician->currentState == GameObject::STATE_ATTACK)
		//	{
		//		if (magicianRechargeTimer <= 0.f && !fireball->active)
		//		{
		//			msgBoard.addMessage("Magician","Everyone","Attacking the Enemy!");
		//			fireball->pos = magician->pos;
		//			fireball->active = true;
		//			magicianRechargeTimer = 2.f;
		//			magician->currentState = GameObject::STATE_RECHARGE;
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

		//Healer Codes here
		/*if (healer->active)
		{
			if (healer->HP > 0)
			{
				healerCooldown -= dt;
				healerAOETimer -= dt;
				if (healerAOETimer <= 0.f && healer->currentState != GameObject::STATE_HEAL)
				{
					for (auto go : m_goList)
					{
						if (go->job == GameObject::JOB_NONE || go->active == false)
							continue;
						if (DistXY(healer->pos, go->pos) < 400.f)
						{
							if (go->job == GameObject::JOB_WARRIOR)
							{
								if (go->HP <= 150 && go->HP >= 90)
									go->HP = 150;
								else
									go->HP += 60;
								break;
							}
							else
							{
								if (go->HP <= 100 && go->HP >= 50)
									go->HP = 100;
								else
									go->HP += 50;
								break;
							}
						}
					}
					msgBoard.addMessage("Healer", "Everyone", "AOE Heal Complete!");
					healerAOETimer = 15.f;
				}
				if (healer->currentState == GameObject::STATE_MOVE)
				{
					for (auto go : m_goList)
					{
						if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_HEALER || go->active == false)
							continue;
						if (go->job == GameObject::JOB_MOB)
						{
							if (go->active && DistXY(healer->pos, go->pos) < 200.f)
							{
								healer->vel.SetZero();
								break;
							}
							else if (go->active == false)
							{
								healer->vel.Set(5, 0, 0);
							}
						}
						else
						{
							if (go->active && DistXY(healer->pos, go->pos) < 200.f)
							{
								healer->vel.SetZero();
								if (go->isHealTarget == true)
								{
									healer->currentState = GameObject::STATE_HEAL;
									break;
								}
							}
							else
							{
								healer->vel.Set(5, 0, 0);
							}
						}

					}

				}
				else if (healer->currentState == GameObject::STATE_HEAL)
				{
					for (auto go : m_goList)
					{
						if (go->isHealTarget)
						{
							if (HealerHeal)
							{
								if ((DistXY(healer->pos, go->pos) < 400.f))
								{
									healer->vel.SetZero();
									go->HP += 10;
									healerCooldown = 2.f;
									go->isHealTarget = false;
									healer->currentState = GameObject::STATE_MOVE;
									msgBoard.addMessage("Healer", "Everyone", "Healing Complete!");
									HealerHeal = false;
									break;
								}
							}
							
						}
					}
				}
			}
			else
			{
				healer->currentState == GameObject::STATE_DEAD;
				healer->active = false;
			}
		}*/


	} // All gameplay checks must finish by this brace

	// Collision checks here
	for (auto go : m_goList)
	{
		if (go->active)
		{
			go->pos += go->vel* (float)dt;
			if (go->type == GameObject::GO_MOB)
			{
				if (go->pos.x < 0)
				{
					go->currentState = GameObject::STATE_DEAD;
					go->active = false;
					//delete go;
				}
			}
			if (go->pos.x > m_worldWidth)
			{
				go->currentState = GameObject::STATE_DEAD;
				go->active = false;
				//delete go;
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
	modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -6);
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
		if (go->active)
		{
			if (go->job == GameObject::JOB_NONE)
				continue;
			std::stringstream ss;
			ss.precision(3);
			ss << go->HP;
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y + (go->scale.y * 0.5 ), 1);
			modelStack.Scale(5, 5, 5);
			RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1));
			modelStack.PopMatrix();	

			if (go->type == GameObject::GO_WARRIOR)
			{
				if (go->currentState == GameObject::STATE_ATTACK)
				{
					if (isGuarding)
					{
						//int TempTimer = 100;
						//TempTimer--;
						//if (TempTimer >= 0)
						//{
							modelStack.PushMatrix();
							modelStack.Translate(go->pos.x, go->pos.y + (go->scale.y), 1);
							modelStack.Scale(5, 5, 5);
							RenderMesh(meshList[GEO_WARRIOR_GUARD], true);
							modelStack.PopMatrix();
						//}
					}
				}
			}
		}

		//if (healerAOETimer < 0
		//	|| healerAOETimer > 14)
		//	RenderTextOnScreen(meshList[GEO_TEXT], "Healer Used AOE Heal", Color(1, 1, 1), 5, 10, 40);

		//else if (go->isHealTarget /*&& healerCooldown > 0*/)
		//{
		//	
		//	if (go->job ==GameObject::JOB_WARRIOR)
		//		RenderTextOnScreen(meshList[GEO_TEXT], "Healer Healed Warrior", Color(1, 1, 1), 5, 10, 40);
		//}

	}

	// MessageBoard stuff
	RenderTextOnScreen(meshList[GEO_TEXT], "Sender", Color(1.f, 0.271f, 0.f), 3, 0, 55);
	RenderTextOnScreen(meshList[GEO_TEXT], "Receiver", Color(1.f, 0.271f, 0.f), 3, 15, 55);
	RenderTextOnScreen(meshList[GEO_TEXT], "Content", Color(1.f, 0.271f, 0.f), 3, 35, 55);

	int i = 0;
	for (std::list<Message>::iterator msg = msgBoard.Messages.begin(); msg != msgBoard.Messages.end(); msg++)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], (*msg).sender, Color(0.8f ,0.8f, 0.8f), 3, 0 ,50 - (i * msgBoard.yOffset));
		RenderTextOnScreen(meshList[GEO_TEXT], (*msg).receiver, Color(0.8f, 0.8f, 0.8f), 3, 15, 50 - (i * msgBoard.yOffset));
		RenderTextOnScreen(meshList[GEO_TEXT], (*msg).content, Color(0.8f, 0.8f, 0.8f), 3, 35, 50 - (i * msgBoard.yOffset));
		i += 1;
	}
}

void SceneAIAsn2::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_HEALER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (healer->currentState == GameObject::STATE_MOVE)
		{
			if (HealerAnimCounter >= 0.f && HealerAnimCounter < 1.f)
				RenderMesh(meshList[GEO_HEALER_WALK_FRAME0], false);
			else if (HealerAnimCounter >= 1.f && HealerAnimCounter < 2.f)
				RenderMesh(meshList[GEO_HEALER_WALK_FRAME1], false);
			else if (HealerAnimCounter >= 2.f && HealerAnimCounter < 3.f)
				RenderMesh(meshList[GEO_HEALER_WALK_FRAME2], false);
			else if (HealerAnimCounter >= 3.f)
				RenderMesh(meshList[GEO_HEALER_WALK_FRAME3], false);
		}
		else if (healer->currentState == GameObject::STATE_HEAL)
		{
			if (HealerAnimCounter >= 0.f && HealerAnimCounter < 1.f)
				RenderMesh(meshList[GEO_HEALER_SHOOT_FRAME0], false);
			else if (HealerAnimCounter >= 1.f && HealerAnimCounter < 2.f)
				RenderMesh(meshList[GEO_HEALER_SHOOT_FRAME1], false);
			else if (HealerAnimCounter >= 2.f && HealerAnimCounter < 3.f)
				RenderMesh(meshList[GEO_HEALER_SHOOT_FRAME2], false);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WARRIOR:
		modelStack.PushMatrix();
		//modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		if (warrior->currentState == GameObject::STATE_MOVE)
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		}
		else if (warrior->currentState == GameObject::STATE_ATTACK)
		{
			modelStack.Translate(go->pos.x, go->pos.y - (1), go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y + (4), go->scale.z);
		}
		if (warrior->currentState == GameObject::STATE_MOVE)
		{
			if (WarriorAnimCounter >= 0.f && WarriorAnimCounter < 1.f)
				RenderMesh(meshList[GEO_WARRIOR_WALK_FRAME0], false);
			else if (WarriorAnimCounter >= 1.f && WarriorAnimCounter < 2.f)
				RenderMesh(meshList[GEO_WARRIOR_WALK_FRAME1], false);
			else if (WarriorAnimCounter >= 2.f && WarriorAnimCounter < 3.f)
				RenderMesh(meshList[GEO_WARRIOR_WALK_FRAME2], false);
			else if (WarriorAnimCounter >= 3.f)
				RenderMesh(meshList[GEO_WARRIOR_WALK_FRAME3], false);
		}
		else if (warrior->currentState == GameObject::STATE_ATTACK)
		{
			if (WarriorAnimCounter >= 0.f && WarriorAnimCounter < 1.f)
				RenderMesh(meshList[GEO_WARRIOR_SHOOT_FRAME0], false);
			else if (WarriorAnimCounter >= 1.f && WarriorAnimCounter < 2.f)
				RenderMesh(meshList[GEO_WARRIOR_SHOOT_FRAME1], false);
			else if (WarriorAnimCounter >= 2.f && WarriorAnimCounter < 3.f)
				RenderMesh(meshList[GEO_WARRIOR_SHOOT_FRAME2], false);
			else if (WarriorAnimCounter >= 3.f)
				RenderMesh(meshList[GEO_WARRIOR_SHOOT_FRAME3], false);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARCHER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (archer->currentState == GameObject::STATE_MOVE)
		{
			if (ArcherAnimCounter >= 0.f && ArcherAnimCounter < 1.f)
				RenderMesh(meshList[GEO_ARCHER_WALK_FRAME0], false);
			else if (ArcherAnimCounter >= 1.f && ArcherAnimCounter < 2.f)
				RenderMesh(meshList[GEO_ARCHER_WALK_FRAME1], false);
			else if (ArcherAnimCounter >= 2.f && ArcherAnimCounter < 3.f)
				RenderMesh(meshList[GEO_ARCHER_WALK_FRAME2], false);
			else if (ArcherAnimCounter >= 3.f)
				RenderMesh(meshList[GEO_ARCHER_WALK_FRAME3], false);
		}
		else if (archer->currentState == GameObject::STATE_ATTACK)
		{
			if (ArcherAnimCounter >= 0.f && ArcherAnimCounter < 1.f)
				RenderMesh(meshList[GEO_ARCHER_SHOOT_FRAME0], false);
			else if (ArcherAnimCounter >= 1.f && ArcherAnimCounter < 2.f)
				RenderMesh(meshList[GEO_ARCHER_SHOOT_FRAME1], false);
			else if (ArcherAnimCounter >= 2.f && ArcherAnimCounter < 3.f)
				RenderMesh(meshList[GEO_ARCHER_SHOOT_FRAME2], false);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_MAGICIAN:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (magician->currentState == GameObject::STATE_MOVE)
		{
			if (MagicianAnimCounter >= 0.f && MagicianAnimCounter < 1.f)
				RenderMesh(meshList[GEO_MAGICIAN_WALK_FRAME0], false);
			else if (MagicianAnimCounter >= 1.f && MagicianAnimCounter < 2.f)
				RenderMesh(meshList[GEO_MAGICIAN_WALK_FRAME1], false);
			else if (MagicianAnimCounter >= 2.f && MagicianAnimCounter < 3.f)
				RenderMesh(meshList[GEO_MAGICIAN_WALK_FRAME2], false);
			else if (MagicianAnimCounter >= 3.f)
				RenderMesh(meshList[GEO_MAGICIAN_WALK_FRAME3], false);
		}
		else if (magician->currentState == GameObject::STATE_ATTACK)
		{
			if (MagicianAnimCounter >= 0.f && MagicianAnimCounter < 1.f)
				RenderMesh(meshList[GEO_MAGICIAN_SHOOT_FRAME0], false);
			else if (MagicianAnimCounter >= 1.f && MagicianAnimCounter < 2.f)
				RenderMesh(meshList[GEO_MAGICIAN_SHOOT_FRAME1], false);
			else if (MagicianAnimCounter >= 2.f && MagicianAnimCounter < 3.f)
				RenderMesh(meshList[GEO_MAGICIAN_SHOOT_FRAME2], false);
		}
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
	case GameObject::GO_FIREBALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ENEMYTOWER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_TOWER], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HEROTOWER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_TOWER], false);
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
	return abs(((first.x - second.x) * (first.x - second.x)));
}

int SceneAIAsn2::RNG(int y)
{
	srand(time(NULL));
	return rand()%y;

}

void SceneAIAsn2::ArcherAnimation()
{
	if (archer->currentState == GameObject::STATE_MOVE)
	{
		ArcherAnimCounter += 0.1f;
		if (ArcherAnimCounter > 4)
			ArcherAnimCounter = 0;
	}
	else if (archer->currentState == GameObject::STATE_ATTACK)
	{
		ArcherAnimCounter += 0.1f;
		if (ArcherAnimCounter > 3)
		{
			if (ArcherAnimCounter >= 2.f)
			{
				ArcherShoot = true;
				msgBoard.addMessage("Archer", "Everyone", "Shooting down the Enemy!");
				ArcherAnimCounter = 0;
			}
			//ArcherAnimCounter = 0;
		}
	}
}

void SceneAIAsn2::HealerAnimation()
{
	if (healer->vel != Vector3(0,0,0) && healer->currentState == GameObject::STATE_MOVE)
	{
		HealerAnimCounter += 0.1f;
		if (HealerAnimCounter > 4)
			HealerAnimCounter = 0;
	}
	else if (healer->currentState == GameObject::STATE_HEAL && healerCooldown <= 0.f)
	{
		HealerAnimCounter += 0.1f;
		if (HealerAnimCounter > 3)
		{
			if (HealerAnimCounter >= 2.f)
			{
				HealerHeal = true;
				msgBoard.addMessage("Healer", "Everyone", "Healing Complete!");
				HealerAnimCounter = 0;
			}
			
		}
	}
}

void SceneAIAsn2::WarriorAnimation()
{
	if (warrior->currentState == GameObject::STATE_MOVE)
	{
		WarriorAnimCounter += 0.1f;
		if (WarriorAnimCounter > 4)
			WarriorAnimCounter = 0;
	}
	else if (warrior->currentState == GameObject::STATE_ATTACK)
	{
		/*if (isGuarding)
		//{
		//	WarriorGuardTime--;
		//	WarriorAnimCounter = 3;
		//	if (WarriorGuardTime <= 0)
		//		isGuarding = false;
		//}
		else*/ if (!isGuarding)
		{
			//WarriorAnimCounter = 0;
			WarriorAnimCounter += 0.05f;
			if (WarriorAnimCounter > 3)
			{
				if (WarriorAnimCounter >= 2.f)
				{
					WarriorAttack = true;
					WarriorAnimCounter = 0;
					//msgBoard.addMessage("Archer", "Everyone", "Shooting down the Enemy!");
				}

			}
		}
	}
}

void SceneAIAsn2::CreateArrow()
{
	arrow = FetchGO();
	arrow->type = GameObject::GO_ARROW;
	arrow->pos.Set(archer->pos.x, archer->pos.y, 0);
	arrow->vel.Set(30, 0, 0);
	arrow->scale.Set(3, 3, 3);
	arrow->Dmg = 15;
}