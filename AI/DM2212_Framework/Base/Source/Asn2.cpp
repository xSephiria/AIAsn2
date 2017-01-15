#include "Asn2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

Asn2::Asn2()
{
}

Asn2::~Asn2()
{
}

void Asn2::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	debounce = 0.f;
	Mtx44 rot;
	rot.SetToRotation(90, 0, 0, 1);
	ballSpawn = false;
	check1 = false;
	check2 = false;

	retries = 3;
	score = 0;
	initialPos.Set(50, 10, 0);
	gs = GS_MAINMENU;
	homeScreenCursor = false;
	homeScreenDebounce = 0.f;
	quitGame = false;

	ironBall = false;
	IBTimer = 0.f;

	splitBall = false;
	SBTimer = 0.f;
	// Player
	player = FetchGO();
	player->type = GameObject::GO_PLAYER;
	player->pos = initialPos;
	player->normal = Vector3(0, 1, 0);
	player->active = false;
	player->scale.Set(3, 10, 1);

	playerLeft = FetchGO();
	playerLeft->type = GameObject::GO_PILLAR;
	playerLeft->pos = initialPos - Vector3(4.5f, 0, 0);
	playerLeft->scale.Set(2, 1.5f, 1);
	playerLeft->active = false;

	playerRight = FetchGO();
	playerRight->type = GameObject::GO_PILLAR;
	playerRight->pos = initialPos + Vector3(4.5f, 0, 0);
	playerRight->scale.Set(2, 1.5f, 1);
	playerRight->active = false;

	ballSpawn = false;

	ghostBall = FetchGO();
	ghostBall->type = GameObject::GO_PILLAR;
	ghostBall->pos = initialPos + Vector3(0, 5, 0);
	ghostBall->active = false;
	ghostBall->scale.Set(1, 1, 1);

	ball = FetchGO();
	ball->type = GameObject::GO_BALL;
	ball->pos = ghostBall->pos;
	ball->scale.Set(1, 1, 1);

	split = FetchGO();
	split->type = GameObject::GO_BALL;
	split->pos.Set(50, 10, 0);
	split->scale.Set(1, 1, 1);
	

	// Walls and Pillars
	
	GameObject* pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->pos = Vector3(5 , 100, 1);
	pillar->active = false;
	pillar->scale.Set(6, 6, 1);

	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->pos = Vector3(50, 100, 1);
	pillar->active = false;
	pillar->scale.Set(6, 6, 1);

	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->pos = Vector3(95, 100, 1);
	pillar->active = false;
	pillar->scale.Set(6, 6, 1);

	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(0, 0, 1);
	wall->active = false;
	wall->normal = Vector3(1, 0, 0);
	wall->scale.Set(5, 200, 1);

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(0, 100, 1);
	wall->active = false;
	wall->normal = rot* Vector3(1, 0, 0);
	wall->scale.Set(5, 200, 1);

	wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	wall->pos.Set(100, 100, 1);
	wall->active = false;
	wall->normal = Vector3(1, 0, 0);
	wall->scale.Set(3, 200, 1);

	// Level 1
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			GameObject* block = FetchGO();
			block->type = GameObject::GO_BLOCK;
			block->pos = Vector3(15 + (10 * x), 40 + (5 * y), 1);
			block->normal = Vector3(1, 0, 0);
			block->normal2 = Vector3(0, 1, 0);
			block->active = false;
			block->scale.Set(5, 2, 1);
			block->scale2.Set(2, 5, 1);
		}
	}

	// Boss test for AI
	boss = FetchGO();
	boss->type = GameObject::GO_BALL;
	boss->pos = initialPos + Vector3(0, 5, 0);
	boss->scale.Set(1, 1, 1);
	boss->active = true;
	boss->currentState = GameObject::STATE_IDLE;
	boss->HP = 100;

	bossCD = 0.f;
	attCount = 0;
	playerInRange = false;
	rangeBoss = 10.f;

	// Mob test for AI;
	mob = FetchGO();
	mob->type = GameObject::GO_CUBE;
	mob->pos = initialPos + Vector3(0, 10, 0);
	mob->scale.Set(1, 1, 1);
	mob->active = true;
	mob->currentState = GameObject::STATE_IDLE;
	mob->HP = 10;

	rangeMob = 5.f;
	playerInRangeMob = false;

}

GameObject* Asn2::FetchGO()
{
	for (auto go : m_goList)
	{
		if (go->active == false)
		{
			go->active = true;
			m_objectCount += 1;
			return go;
		}
	}
	for (unsigned int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	GameObject* go = *(m_goList.end() - 1);
	go->active = true;
	m_objectCount += 1;
	return go;
}

void Asn2::Update(double dt)
{	
	if (Application::IsKeyPressed(VK_ESCAPE))
		quitGame = true;
	if (gs == GS_MAINMENU)
	{
		homeScreenDebounce += (float)dt;
		if (homeScreenDebounce > 0.5f)
		{
			if (Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
			{
				homeScreenDebounce = 0.f;
				if (homeScreenCursor)
					homeScreenCursor = false;
				else
					homeScreenCursor = true;
			}
			if (Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
			{
				homeScreenDebounce = 0.f;
				if (homeScreenCursor)
					homeScreenCursor = false;
				else
					homeScreenCursor = true;
			}
		}
		if (Application::IsKeyPressed(VK_RETURN))
		{
			if (homeScreenCursor == false)
			{
				gs = GS_GAME;
				ball->active = false;
				split->active = false;
			}
			else if (homeScreenCursor == true)
			{
				quitGame = true;
			}
		}
		
	}
	else if (gs == GS_GAME)
	{
		SceneBase::Update(dt);
		if (CalcDist(boss->pos, player->pos) <= rangeBoss)
		{
			playerInRange = true;
		}
		else
		{
			playerInRange = false;
		}
		if (CalcDist(mob->pos, player->pos) <= rangeMob)
		{
			playerInRangeMob = true;
		}
		else
		{
			playerInRangeMob = false;
		}
		
		if (Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);
		}
		if (Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}

		if (Application::IsKeyPressed('A') || Application::IsKeyPressed(VK_LEFT))
		{
			player->pos.x -= MOVE_SPEED * dt;
			playerRight->pos.x -= MOVE_SPEED * dt;
			playerLeft->pos.x -= MOVE_SPEED * dt;
			ghostBall->pos.x -= MOVE_SPEED * dt;
		}
		if (Application::IsKeyPressed('D') || Application::IsKeyPressed(VK_RIGHT))
		{
			player->pos.x += MOVE_SPEED * dt;
			playerRight->pos.x += MOVE_SPEED * dt;
			playerLeft->pos.x += MOVE_SPEED * dt;
			ghostBall->pos.x += MOVE_SPEED * dt;
		}
		PhysicSIM(dt);
		// AI stuff
		bossCD += dt;
		if (boss->active && bossCD >= 2.f)
		{
			bossCD = 0.f;
			if (boss->HP <= 0)
			{
				boss->currentState = GameObject::STATE_DEAD;
				boss->active = false;
			}
			else if (playerInRange)
			{
				if (attCount < 5)
				{
					boss->currentState = GameObject::STATE_ATTACK;
					attCount++;
				}
				else
				{
					boss->currentState = GameObject::STATE_BUFF;
					attCount = 0;
				}
			}
			else
			{
				boss->currentState = GameObject::STATE_IDLE;
			}
		}
		if (mob->active)
		{
			if (mob->HP <= 0)
			{
				mob->currentState = GameObject::STATE_DEAD;
				mob->active = false;
			}
			else if (mob->currentState == GameObject::STATE_RECOVER && mob->HP < 7)
			{
				mob->HP += 0.5f;
			}
			else if (mob->HP > 7.f) // current hp more than 70%
			{
				if (playerInRangeMob)
				{
					mob->currentState = GameObject::STATE_ATTACK;
				}
				else
				{
					mob->currentState = GameObject::STATE_CHASE;
				}
			}
			else if (mob->HP < 3.f && playerInRangeMob)
			{
				mob->currentState = GameObject::STATE_ESCAPE;
			}
			else if (mob->currentState == GameObject::STATE_ESCAPE && mob->HP < 7 && !playerInRangeMob)
			{
				mob->currentState = GameObject::STATE_RECOVER;
			}
			else
				mob->currentState = GameObject::STATE_IDLE;
			
		}
	}
}

void Asn2::PhysicSIM(double dt)
{
	//Physics Simulation Section
	m_ghost->active = true;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->type == GameObject::GO_BALL || go->type == GameObject::GO_IRON_BALL)
			{

				go->pos.x += go->vel.x * dt;
				go->pos.y += go->vel.y * dt;
				float radius = go->scale.x;
				//Exercise 7: handle out of bound game objects
				if (go->pos.x > m_worldWidth - radius && go->vel.x > 0 ||
					go->pos.x < radius && go->vel.x < 0)
					go->vel.x = -go->vel.x;

				if (go->pos.y > m_worldHeight - radius && go->vel.y > 0 ||
					go->pos.y < radius && go->vel.y < 0)
					go->vel.y = -go->vel.y;

				if (go->pos.x > m_worldWidth + go->scale.x ||
					go->pos.x < 0 - go->scale.x ||
					go->pos.y > m_worldHeight + go->scale.y ||
					go->pos.y < 0 - go->scale.x)
				{
					go->active = false;
					m_objectCount -= 1;
				}

				if (go->pos.y < 4)
				{
					go->active = false;

					if (!split->active && !ball->active)
					{
						retries -= 1;
						player->pos = initialPos;
						playerLeft->pos = initialPos - Vector3(4.5f, 0, 0);
						playerRight->pos = initialPos + Vector3(4.5f, 0, 0);
						ballSpawn = false;
						ghostBall->active = true;
						ghostBall->pos = initialPos + Vector3(0, 5, 0);
						ball->pos = ghostBall->pos;
						split->active = false;
					}
					
				}
			}
			for (std::vector<GameObject *>::iterator its = it + 1; its != m_goList.end(); ++its)
			{
				GameObject *other = (GameObject*)*its;
				if (!other->active)
					continue;
				/*if (other->type != GameObject::GO_BALL)
				continue;*/

				GameObject *goA = go;
				GameObject* goB = other;
				if (go->type != GameObject::GO_BALL && go->type != GameObject::GO_IRON_BALL)
				{
					if (other->type != GameObject::GO_BALL && other->type != GameObject::GO_IRON_BALL)
					{
						continue;
					}
					goA = other;
					goB = go;
				}

				if (CheckCollision(goA, goB, dt))
				{
					//m_timerStarted = false;
					CollisionResponse(goA, goB);
					break;
				}
			}
		}
	}
}

void Asn2::CollisionResponse(GameObject* go1, GameObject* go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		m1 = go1->mass;
		m2 = go2->mass;
		u1 = go1->vel;
		u2 = go2->vel;

		Vector3 u1N, u2N, N;
		N = (go1->pos - go2->pos).Normalized();
		u1N = u1.Dot(N) * N;
		u2N = u2.Dot(N) * N;

		go1->vel = u1 + (2 * m2) / (m1 + m2) * (u2N - u1N);
		go2->vel = u2 + (2 * m1) / (m1 + m2) * (u1N - u2N);

		v1 = go1->vel;
		v2 = go2->vel;
		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 u = go1->vel;
		Vector3 N = go2->normal;
		Vector3 uN = u.Dot(N) * N;
		go1->vel = u - 2 * uN;
		break;
	}
	case GameObject::GO_BLOCK:
	{
		if (check1)
		{
			Vector3 u = go1->vel;
			Vector3 N = go2->normal;
			Vector3 uN = u.Dot(N) * N;
			if (go1->type == GameObject::GO_BALL)
			{
				go1->vel = u - 2 * uN;
			}
			go2->active = false;
			m_objectCount--;
			score++;
			break;
		}
		else if (check2)
		{
			Vector3 u = go1->vel;
			Vector3 N = go2->normal2;
			Vector3 uN = u.Dot(N) * N;
			if (go1->type == GameObject::GO_BALL)
			{
				go1->vel = u - 2 * uN;
			}
			go2->active = false;
			m_objectCount--;
			score++;
			break;
		}
	}
	case GameObject::GO_PLAYER:
	{
		Vector3 u = go1->vel;
		Vector3 N = go2->normal;
		Vector3 uN = u.Dot(N) * N;
		go1->vel = u - 2 * uN;
		go1->vel *= 1.05f;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 u = go1->vel;
		Vector3 N = (go1->pos - go2->pos).Normalized();
		go1->vel = u - (2 * (u.Dot(N))) * N;
		break;
	}
	}
}

bool Asn2::CheckCollision(GameObject* go, GameObject* other, double dt)
{
	switch (other->type)
	{
	case GameObject::GO_BALL:
	{
		float distanceSquared = ((go->pos + go->vel * dt) - (other->pos - other->vel * dt)).LengthSquared();
		float combinedRadiusSquared = (go->scale.x + other->scale.x) * (go->scale.x + other->scale.x);
		Vector3 relativeVel = go->vel - other->vel;
		Vector3 relativeDisplacement = other->pos - go->pos;
		return (distanceSquared < combinedRadiusSquared && relativeVel.Dot(relativeDisplacement) > 0);
		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos + go->vel *dt;
		Vector3 N = other->normal;
		float r = go->scale.x;
		float h = other->scale.x;
		float l = other->scale.y;
		//Vector3 NP = N.Cross(Vector3(0, 0, 1));
		Vector3 NP(-N.y, N.x); // (N.y, -N.x);

		Vector3 rDis = w0 - b1;
		Vector3 rVel = go->vel;

		if ((w0 - b1).Dot(N) < 0)
			N = -N;

		return abs(rDis.Dot(N)) < r + h / 2 && abs(rDis.Dot(NP)) < r + l / 2 && rVel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_BLOCK:
	{
		check1 = true;
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos + go->vel *dt;
		Vector3 N = other->normal;
		float r = go->scale.x;
		float h = other->scale.x;
		float l = other->scale.y;
		//Vector3 NP = N.Cross(Vector3(0, 0, 1));
		Vector3 NP(-N.y, N.x); // (N.y, -N.x);

		Vector3 rDis = w0 - b1;
		Vector3 rVel = go->vel;

		if (rDis.Dot(N) < 0)
			N = -N;
		if (!(abs(rDis.Dot(N)) < r + h / 2 && abs(rDis.Dot(NP)) < r + l / 2 && rVel.Dot(N) > 0))
		{
			check1 = false;
			check2 = true;
			Vector3 w0 = other->pos;
			Vector3 b1 = go->pos + go->vel *dt;
			Vector3 N = other->normal2;
			float r = go->scale.x;
			float h = other->scale2.x;
			float l = other->scale2.y;
			//Vector3 NP = N.Cross(Vector3(0, 0, 1));
			Vector3 NP(-N.y, N.x); // (N.y, -N.x);

			Vector3 rDis = w0 - b1;
			Vector3 rVel = go->vel;

			if (rDis.Dot(N) < 0)
				N = -N;

			return (abs(rDis.Dot(N)) < r + h / 2 && abs(rDis.Dot(NP)) < r + l / 2 && rVel.Dot(N) > 0);
		}
		return (abs(rDis.Dot(N)) < r + h / 2 && abs(rDis.Dot(NP)) < r + l / 2 && rVel.Dot(N) > 0);
		break;
	}
	case GameObject::GO_PLAYER:
	{
		Vector3 w0 = other->pos;
		Vector3 b1 = go->pos + go->vel *dt;
		Vector3 N = other->normal;
		float r = go->scale.x;
		float h = other->scale.x;
		float l = other->scale.y;
		//Vector3 NP = N.Cross(Vector3(0, 0, 1));
		Vector3 NP(-N.y, N.x); // (N.y, -N.x);

		Vector3 rDis = w0 - b1;
		Vector3 rVel = go->vel;

		if ((w0 - b1).Dot(N) < 0)
			N = -N;

		return abs(rDis.Dot(N)) < r + h / 2 && abs(rDis.Dot(NP)) < r + l / 2 && rVel.Dot(N) > 0;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go->pos;
		Vector3 p2 = other->pos;
		float r1 = go->scale.x;
		float r2 = other->scale.x;
		float combinedRadius = r1 + r2;
		Vector3 u = go->vel;

		return (p2 - p1).LengthSquared() < combinedRadius * combinedRadius && (p2 - p1).Dot(u) > 0;
		break;

	}
	}

}

float Asn2::CheckCollision2(GameObject* go1, GameObject* go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		// relative velocity of	go1 to go2
		Vector3 rel, dir;
		rel = go2->vel - go1->vel;
		dir = go1->pos - go2->pos;
		float r = go1->scale.x + go2->scale.x;
		float dirCheck = rel.Dot(dir);
		if (dirCheck > 0)
			return -1;
		float a = rel.LengthSquared();
		float b = 2 * rel.Dot(dir);
		float c = dir.LengthSquared() - r * r;

		float d = (b * b) - (4 * (a * c));
		if (d < 0)
			return -1;

		float root = -b - sqrt(d) / 2 * a;
		if (root < 0)
			root = -b + sqrt(d) / 2 * a;
		return root;
		break;
	}

	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->normal;
		float r = go1->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;

		Vector3 U = go1->vel;
		Vector3 relativeDisplacement = w0 - b1;

		if (relativeDisplacement.Dot(N) < 0)
			N = -N;
		if (U.Dot(N) < 0)
			return -1;

		w0 += -(r + h / 2) * N;
		float distance = relativeDisplacement.Dot(N);
		float speed = U.Dot(N);

		float th = distance / speed;

		Vector3 NP(-N.y, N.x);

		Vector3 w1 = w0 + (l * 0.5f) * NP;
		Vector3 w2 = w0 - (l * 0.5f) * NP;

		Vector3 bh = b1 + U * th;
		if ((w1 - bh).Dot(w2 - bh) > 0)
			return -1;

		return th;
		break;
	}

	}

}

void Asn2::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		//Exercise 11: think of a way to give balls different colors
		break;
	case GameObject::GO_IRON_BALL:
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_IRON_BALL], false);
		modelStack.PopMatrix();
		//Exercise 11: think of a way to give balls different colors
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();

		break;
	case GameObject::GO_BLOCK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BLOCK], false);
		modelStack.PopMatrix();

	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLAYER], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PILLAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PILLAR], false);
		modelStack.PopMatrix();
		break;
	}
}

void Asn2::Render()
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

	if (gs == GS_MAINMENU)
	{
		std::ostringstream ss;

		ss << "EZ GAME ONLY";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 7, 30, 30);
		ss.str("");

		ss << "Start Game";
		if (homeScreenCursor == 0)
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 30, 24);
		else
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 30, 24);
		ss.str("");

		ss << "Quit Game";
		if (homeScreenCursor == 1)
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0), 3, 30, 21);
		else
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 30, 21);
		ss.str("");
	}
	else if (gs == GS_GAME)
	{
		RenderMesh(meshList[GEO_AXES], false);

		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}

		std::ostringstream ss;

		//ss << "Lives:  " << retries;
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 64, 15);
		//ss.str("");


		//ss << "Score: " << score;
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 64, 12);
		//ss.str("");

		//ss.precision(3);
		//ss << "Objects: " << m_objectCount;
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 64, 9);
		////Exercise 8c: Render initial and final momentum

		//ss.str("");
		//ss.precision(3);
		//ss << "Speed: " << m_speed;
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 64, 6);

		//ss.str("");
		//ss.precision(3);
		//ss << "FPS: " << fps;
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 64, 3);

		ss << "Boss State: ";
		if (boss->currentState == GameObject::STATE_ATTACK)
			ss << "Attack";
		else if (boss->currentState == GameObject::STATE_BUFF)
			ss << "Buff";
		else if (boss->currentState == GameObject::STATE_IDLE)
			ss << "Idle";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 30, 3);

		ss.str("");
		ss.precision(3);
		ss << "Boss CD: " << bossCD;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 30, 6);

		ss.str("");
		ss << "Attack Count: " << attCount;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 30, 9);
	}
	else if (gs == GS_GAMEOVER)
	{
		std::ostringstream ss;

		ss << "Game Over! Press Enter to restart";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 15, 30);
		ss.str("");
	}
	else if (gs == GS_WIN)
	{
		std::ostringstream ss;

		ss << "You Win! Press Enter to Play Again";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 10, 30);
		ss.str("");
	}
}

void Asn2::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}


float Asn2::CalcDist(Vector3 first, Vector3 second)
{
	return abs(((first - second).LengthSquared()));
}