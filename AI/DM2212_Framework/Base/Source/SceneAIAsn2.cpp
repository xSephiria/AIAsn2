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

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Player
	m_lives = 3;
	m_score = 0;
	playerHP = 100;


	magician = FetchGO();
	magician->type = GameObject::GO_MAGICIAN;
	magician->pos.Set(0, 10, 0);
	magician->vel.Set(10,0,0);
	magician->scale.Set(5, 5, 5);
	magicianRechargeTimer = 0.f;

	gs = STAGEFINAL;
	tempGS = STAGE1;

	mob = FetchGO();
	mob->type = GameObject::GO_ASTEROID;
	mob->pos.Set(70, 10, 0);
	mob->scale.Set(5, 5, 1);
	mobHP = 10;
	mobDead = false;
	mob->active = true;
	// Main menu words
	mainMenuOptions[0] = "Start Game";
	mainMenuOptions[1] = "Quit";
	mainMenuCursor = 0;
	cursorDebounce = 0.f;

	quitGame = false;
}

GameObject* SceneAIAsn2::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	//for (auto go : m_goList)
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = *it;
		if (go->active == false)
		{
			go->active = true;
			asteroidZ += 0.1;
			return go;
		}
	}

	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	GameObject* go = *(m_goList.end() - 1);
	go->active = true;
	asteroidZ += 0.1;
	return go;
}

void SceneAIAsn2::Update(double dt)
{
	if (Application::IsKeyPressed(VK_ESCAPE))
		quitGame = true;
	if (gs == DEAD)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			playerHP = 100;
			m_lives -= 1;
			m_ship->pos.Set(m_worldWidth / 2, 5, 0);
			m_ship->vel.SetZero();
			rotateShip = 0;
			m_ship->active = true;
			gs = tempGS;
		}
		return;
	}
	if (gs == GAME_OVER)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			m_lives = 3;
			m_score = 0;
			playerHP = 100;
			m_ship->pos.Set(m_worldWidth / 2, 5, 0);
			m_ship->vel.SetZero();
			rotateShip = 0;
			m_ship->active = true;
			gs = STAGEFINAL;
		}
		return;
	}
	if (gs == VICTORY)
	{
		if (Application::IsKeyPressed(VK_RETURN))
			gs = STAGEFINAL;
		return;
	}
	SceneBase::Update(dt);

	// Rotate ship
	Vector3 acceleration(0, 0, 0);

	/*shipRotation.SetToRotation(rotateShip, 0, 0, 1);
	if (m_ship->mass > Math::EPSILON)
		acceleration = m_force * (1.f / m_ship->mass);

	m_ship->vel += shipRotation * acceleration * (float)dt;
	m_ship->pos += m_ship->vel * (float)dt;

	if (m_ship->vel.LengthSquared() > (MAX_SPEED)* (MAX_SPEED))
	{
		m_ship->vel.Normalize() *= MAX_SPEED;
	}*/

	if (gs == STAGEFINAL)
	{
		if (magician->active) // Magician codes here
		{
			if (DistXY(magician->pos, mob->pos) < 200.f && mob->active)
			{
				if (magicianRechargeTimer <= 0.f)
					magician->currentState = GameObject::STATE_ATTACK;
				magician->vel.SetZero();
			}
			if (magician->currentState == GameObject::STATE_ATTACK)
			{
				if (magicianRechargeTimer <= 0.f)
				{
					GameObject* fireball = FetchGO();
					fireball->type = GameObject::GO_BULLET;
					fireball->pos = magician->pos;
					fireball->scale = bulletSize;
					fireball->vel.Set(5, 0, 0);
					fireball->active = true;
					magicianRechargeTimer = 2.f;
					magician->currentState = GameObject::STATE_RECHARGE;
				}
			}
			if (magician->currentState == GameObject::STATE_RECHARGE)
			{
				magicianRechargeTimer -= dt;
				if (magicianRechargeTimer <= 0.f)
				{
					magician->currentState = GameObject::STATE_MOVE;
				}
			}
		}
		
	}

	if (gs != DEAD && playerHP <= 0)
	{
		tempGS = gs;
		m_ship->active = false;
		for (auto go : m_goList)
		{
			go->active = false;
		}
		gs = DEAD;
	}
	if (m_lives <= 0)
	{
		m_ship->active = false;
		for (auto go : m_goList)
		{
			go->active = false;
		}
		gs = GAME_OVER;
	}

	m_force.SetZero();
	//Exercise 6: set m_force values based on WASD
	//if (Application::IsKeyPressed(VK_UP))
	//{
	//	m_force.y = 30;
	//}
	//if (Application::IsKeyPressed(VK_LEFT))
	//{
	//	rotateShip += 1.f;
	//	//m_force.x = -100;
	//}
	//if (Application::IsKeyPressed(VK_DOWN))
	//{
	//	m_force.y = -30;
	//}
	//if (Application::IsKeyPressed(VK_RIGHT))
	//{
	//	rotateShip -= 1.f;
	//	//m_force.x = 100;
	//}
	////Exercise 8: use 2 keys to increase and decrease mass of ship
	//if (Application::IsKeyPressed(VK_OEM_MINUS))
	//{
	//	m_ship->mass = Math::Max(0.1f, m_ship->mass - 0.1f);
	//}
	//if (Application::IsKeyPressed(VK_OEM_PLUS))
	//{
	//	m_ship->mass = Math::Min(10.f, m_ship->mass + 0.1f);
	//}


	//Exercise 14: use a key to spawn a bullet
	firingDebounce += (float)dt;
	if (Application::IsKeyPressed('Z') && firingDebounce > 1.f / fireRate)
	{

		firingDebounce = 0;

		GameObject* bulletMid = FetchGO();
		bulletMid->type = GameObject::GO_BULLET;
		bulletMid->scale = bulletSize;
		bulletMid->pos = m_ship->pos;


		//Exercise 15: limit the spawn rate of bullets
		if (m_ship->vel.IsZero())
		{
			bulletMid->vel = shipRotation * Vector3(0, 30, 0);
		}
		else
		{
			Vector3 direction = Vector3(0, abs(atan2(m_ship->vel.y, m_ship->vel.x)), 0);
			bulletMid->vel = shipRotation * direction * BULLET_SPEED;
		}
	}
	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section

	//Exercise 9: wrap ship position if it leaves screen
	/*if (m_ship->pos.x > m_worldWidth)
		m_ship->pos.x -= m_worldWidth;
	if (m_ship->pos.x < 0)
		m_ship->pos.x += m_worldWidth;
	if (m_ship->pos.y > m_worldHeight)
		m_ship->pos.y -= m_worldHeight;
	if (m_ship->pos.y < 0)
		m_ship->pos.y += m_worldHeight;
*/
	// Collision checks here
	for (auto go : m_goList)
	{
		if (go->active)
		{
			go->pos += go->vel* (float)dt;
			//Exercise 16: unspawn bullets when they leave screen
			if (go->type == GameObject::GO_BULLET)
			{
				if (go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
				{
					go->active = false;
				}
				//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
				for (auto other : m_goList)
				{
					if (other->type == GameObject::GO_ASTEROID)
					{
						if (other->active == true)
						{
							float distanceSquared = ((other->pos - go->pos).LengthSquared());
							float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
							if (distanceSquared < combinedRadiusSquared)
							{
								other->active = false;
								go->active = false;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void SceneAIAsn2::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Exercise 17b:	re-orientate the ship with velocity
		modelStack.Rotate(rotateShip, 0, 0, 1);
		//Exercise 17a: render a ship texture or 3D ship model
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		//Exercise 4c: render a sphere with radius 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_MISSILE:
		//Exercise 4c: render a sphere with radius 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOSS:
		//Exercise 4c: render a sphere with radius 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(rotateBoss, 0, 0, 1);
		RenderMesh(meshList[GEO_BOSS], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOSS_BULLET:
		//Exercise 4c: render a sphere with radius 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOSS_BULLET], false);
		modelStack.PopMatrix();
		break;
	default:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(rotateShip, 0, 0, 1);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		break;
	}
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

	RenderMesh(meshList[GEO_AXES], false);

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text
	if (gs == MAIN_MENU)
	{
		if (!mainMenuCursor)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], mainMenuOptions[0], Color(1, 1, 0), 7, 25, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], mainMenuOptions[1], Color(1, 1, 1), 7, 25, 23);
		}
		else if (mainMenuCursor)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], mainMenuOptions[0], Color(1, 1, 1), 7, 25, 30);
			RenderTextOnScreen(meshList[GEO_TEXT], mainMenuOptions[1], Color(1, 1, 0), 7, 25, 23);
		}
	}
	else
	{
		std::ostringstream ss;
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 0, 3);

		ss.str("");
		ss.precision(3);
		ss << "HP: " << playerHP << "/100";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 0, 6);

		ss.str("");
		ss.precision(5);
		ss << "Lives: " << m_lives;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 0, 9);

		// Stage info


		ss.str("");
		if (gs == STAGE1)
			ss << "Score needed to clear this stage: " << m_score << "/30";
		else if (gs == STAGE2)
			ss << "Score needed to clear this stage: " << m_score << "/50";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 10, 54);

		// If dead
		ss.str("");
		if (gs == STAGE_1_CLEARED || gs == STAGE_2_CLEARED)
		{
			ss << "Press Enter to continue to next stage";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 10, 30);
		}
		if (gs == DEAD)
		{
			ss << "You Died! Press Enter to respawn";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 15, 30);
		}
		if (gs == GAME_OVER || gs == VICTORY)
		{
			if (gs == VICTORY)
				ss << "Victory! ";
			ss << "Press Enter to restart the game!";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 10, 30);
		}
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