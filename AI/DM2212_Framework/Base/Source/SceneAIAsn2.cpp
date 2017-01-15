#include "SceneAIAsn2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>


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

	magician = FetchGO();
	magician->type = GameObject::GO_MAGICIAN;
	magician->active = false;
	magician->pos.Set(0, 10, 0);
	magician->vel.Set(10, 0, 0);
	magician->scale.Set(5, 5, 5);
	magician->Dmg = 10;
	magician->job = GameObject::JOB_MAGICIAN;
	magicianRechargeTimer = 0.f;

	warrior = FetchGO();
	warrior->type = GameObject::GO_WARRIOR;
	warrior->pos.Set(0, 10, 0);
	warrior->vel.Set(10, 0, 0);
	warrior->scale.Set(5, 5, 5);
	warrior->Dmg = 20;
	warrior->job = GameObject::JOB_WARRIOR;

	//archer = FetchGO();

	healer = FetchGO();
	healer->type = GameObject::GO_HEALER;
	healer->active = false;
	healer->pos.Set(0, 10, 0);
	healer->vel.Set(5, 0, 0);
	healer->scale.Set(5, 5, 5);
	healer->Dmg = 0;
	healer->job = GameObject::JOB_HEALER;
	healerAOETimer = 0.f;
	healerCooldown = 0.f;

	mob = FetchGO();
	mob->type = GameObject::GO_MOB;
	mob->pos.Set(100, 10, 0);
	mob->vel.Set(-10, 0, 0);
	mob->scale.Set(3, 3, 3);
	mob->Dmg = 5;
	mob->job = GameObject::JOB_MOB;
	
	// Main menu words
	mainMenuOptions[0] = "Start Game";
	mainMenuOptions[1] = "Quit";
	mainMenuCursor = 0;
	cursorDebounce = 0.f;

	warriorAFrame = 0.f;
	mobAFrame = 0.f;

	quitGame = false;

}

void SceneAIAsn2::Update(double dt)
{
	if (Application::IsKeyPressed(VK_ESCAPE))
		quitGame = true;
	SceneBase::Update(dt);

	std::cout << warrior->HP << " " << mob->HP  << " " << warrior->currentState << " " << mob->currentState << " " << mob->vel << std::endl;
	if (state == GAMEPLAY)
	{
		if (warrior->active)
		{
			if (warrior->HP > 0)
			{
				if (warrior->currentState == GameObject::STATE_MOVE)
				{
					if (DistXY(warrior->pos, mob->pos) < 50.f && mob->active)
					{
						warrior->vel.SetZero();
						warrior->currentState = GameObject::STATE_ATTACK;
						//mob->currentState = GameObject::STATE_ATTACK;
					}
					if (warrior->currentState != GameObject::STATE_ATTACK && mob->active == false)
						warrior->vel.Set(10, 0, 0);
				}
				else if (warrior->currentState == GameObject::STATE_ATTACK)
				{
					if (DistXY(warrior->pos, mob->pos) < 50.f && mob->active)
					{
						warriorAFrame -= 1.f;
						if (warriorAFrame <= 0.f)
						{
							mob->HP -= warrior->Dmg;
							if (mob->HP <= 0)
								warrior->currentState = GameObject::STATE_MOVE;
							warriorAFrame = 100.f;
						}
							
					}
				}
			}
			else
			{
				warrior->currentState = GameObject::STATE_DEAD;
				warrior->active = false;
			}
			
		}

		if (mob->active)
		{
			if (mob->HP > 0)
			{
				if (mob->currentState == GameObject::STATE_MOVE) // 4
				{
					GameObject* temp;
					for (auto go : m_goList)
					{
						temp = go;
						if (temp->job == GameObject::JOB_NONE || temp->job == GameObject::JOB_MOB || temp->active == false)
							continue;
						if (DistXY(temp->pos, mob->pos) < 50.f && temp->active)
						{
							mob->vel.SetZero();
							mob->currentState = GameObject::STATE_ATTACK;
						}
					}
					if (mob->currentState != GameObject::STATE_ATTACK && temp->active == false)
					{
						mob->vel.Set(-10, 0, 0);
					}
				}
				else if (mob->currentState == GameObject::STATE_ATTACK) // 3 
				{
					GameObject* temp;
					for (auto go : m_goList)
					{
						if (go->job == GameObject::JOB_NONE || go->job == GameObject::JOB_MOB || go->active == false)
							continue;
						temp = go;
						if (DistXY(mob->pos, temp->pos) < 50.f && temp->active)
						{
							mobAFrame -= 1.f;
							if (mobAFrame <= 0.f)
							{
								temp->HP -= mob->Dmg;
								if (temp->HP <= 0)
									mob->currentState = GameObject::STATE_MOVE;
								mobAFrame = 100.f;
							}
						}
					}
				}
			}
			else
			{
				mob->currentState = GameObject::STATE_DEAD;
				mob->active = false;
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
		//	GameObject* healingTarget;
		//	healerAOETimer -= dt;
		//	/*if (healerAOETimer <= 0.f && healer->currentState != GameObject::STATE_HEAL)
		//	{
		//		for (auto go : m_goList)
		//		{
		//			if (go->job == GameObject::JOB_NONE || go->active == false)
		//				continue;
		//			if (DistXY(healer->pos, go->pos) < 400.f)
		//			{
		//				if (go->HP <= 100 && go->HP >= 70)
		//					go->HP = 100;
		//				else
		//					go->HP += 30;
		//			}
		//		}
		//	}*/
		//	if (healer->currentState == GameObject::STATE_MOVE)
		//	{
		//		for (auto go : m_goList)
		//		{
		//			if (go->job == GameObject::JOB_NONE || go->active == false)
		//				continue;
		//			if (DistXY(healer->pos, go->pos) < 200.f && go->active)
		//			{
		//				healer->vel.SetZero();
		//				if (go->HP < 30)
		//				{
		//					//healer->currentState = GameObject::STATE_HEAL;
		//					if (healerCooldown <= 0.f)
		//					{
		//						go->HP += 5;
		//						healerCooldown = 5.f;
		//						//healingTarget = NULL;
		//					}
		//					healerCooldown -= dt;
		//				}
		//			}
		//			else
		//			{
		//				healer->currentState = GameObject::STATE_MOVE;
		//				healer->vel.Set(5, 0, 0);
		//			}
		//			
		//			
		//		}
		//		//healer->vel.Set(5,0,0);
		//	}
		//	else if (healer->currentState == GameObject::STATE_HEAL)
		//	{
		//		if (DistXY(healer->pos, healingTarget->pos) < 200 && healerCooldown <= 0.f)
		//		{
		//			healer->vel.SetZero();
		//			healingTarget->HP += 5;
		//			healerCooldown = 5.f;
		//			healingTarget = NULL;
		//			healer->currentState = GameObject::STATE_MOVE;
		//		}
		//		healerCooldown -= dt;
		//	}
		//}
	}

	// Collision checks here
	for (auto go : m_goList)
	{
		if (go->active)
		{
			go->pos += go->vel* (float)dt;
			//Exercise 16: unspawn bullets when they leave screen
			//if (go->type == GameObject::GO_BULLET)
			//{
			//	if (go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
			//	{
			//		go->active = false;
			//	}
			//	//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
			//	for (auto other : m_goList)
			//	{
			//		if (other->type == GameObject::GO_ASTEROID)
			//		{
			//			if (other->active == true)
			//			{
			//				float distanceSquared = ((other->pos - go->pos).LengthSquared());
			//				float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
			//				if (distanceSquared < combinedRadiusSquared)
			//				{
			//					other->HP -= 20;
			//					if (other->HP <= 0)
			//						other->active = false;
			//					go->active = false;
			//					break;
			//				}
			//			}
			//		}
			//	}
			//}
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

	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
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
}

void SceneAIAsn2::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_MAGICIAN:
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