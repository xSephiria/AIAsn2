#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);

	debounce = 0.f;

	Vector3 octaStructCentre(50, 50, 0);
	Vector3 octaStructRadius(25, 0, 0);
	Mtx44 octaRot;
	for (int i = 0; i < 8; i++)
	{
		GameObject* wall = FetchGO();
		wall->type = GameObject::GO_WALL;
		octaRot.SetToRotation(i * 45.f, 0, 0, 1);
		wall->pos = (octaRot * octaStructRadius) + octaStructCentre;
		wall->normal = (octaRot * Vector3(1, 0, 0));
		wall->active = true;
		wall->scale.Set(2, 20, 1);
	}

	GameObject* wall = FetchGO();
	octaRot.SetToRotation(90, 0, 0, 1);
	wall->type = GameObject::GO_WALL;
	wall->active = true;
	wall->pos = octaStructCentre;
	wall->normal = (octaRot * Vector3(1, 0, 0));
	wall->scale = Vector3(2, 20, 1);


	/*GameObject* Pillar = FetchGO();
	Pillar->type = GameObject::GO_PILLAR;
	Pillar->active = true;
	Pillar->pos = octaStructCentre + Vector3(18,0,0);
	Pillar->scale.Set(2, 2, 1);

	Pillar = FetchGO();
	Pillar->type = GameObject::GO_PILLAR;
	Pillar->active = true;
	Pillar->pos = octaStructCentre - Vector3(18, 0, 0);
	Pillar->scale.Set(2, 2, 1);*/
	
}

GameObject* SceneCollision::FetchGO()
{
	
	//Exercise 2a: implement FetchGO()
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
	//Exercise 2b: increase object count every time an object is set to active
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		m_ghost->active = true;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x / w * m_worldWidth;
		float worldY = (h - y) * m_worldHeight / h;
		//float worldY = 0.5f * m_worldHeight;

		m_ghost->pos.Set(worldX, worldY, 0);

	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		GameObject *go = FetchGO();
		go->type = GameObject::GO_BALL;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x / w * m_worldWidth;
		float worldY = (h - y) * m_worldHeight / h;
		//float worldY = 0.5f * m_worldHeight;
		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - worldX, m_ghost->pos.y - worldY, 0.f);
		//go->vel.Set(m_ghost->pos.x - worldX, 0.f, 0.f);
		go->scale.Set(1, 1, 1);
		go->mass = 1;
		m_ghost->vel = go->vel;
		m_ghost->active = false;

		/*for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
		GameObject *other = (GameObject*)*it;
		if (other->active)*/

	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		m_ghost->active = true;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x / w * m_worldWidth;
		float worldY = (h - y) * m_worldHeight / h; //0.5f * m_worldHeight;

		m_ghost->pos.Set(worldX, worldY, 0);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 10: spawn large GO_BALL
		GameObject *go = FetchGO();
		go->type = GameObject::GO_BALL;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x / w * m_worldWidth;
		float worldY = (h - y) * m_worldHeight / h; //0.5f * m_worldHeight;
		//float worldY = 0.5f * m_worldHeight;
		go->pos = m_ghost->pos;
		go->vel.Set(m_ghost->pos.x - worldX, m_ghost->pos.y - worldY, 0.f);
		//go->vel.Set(m_ghost->pos.x - worldX, 0.f, 0.f);

		Vector3 dist = Vector3(worldX, worldY, 0) - m_ghost->pos;
		float radius = Math::Clamp(dist.Length(), 2.0f, 10.0f);
		m_ghost->scale.Set(radius, radius, radius);
		go->scale = m_ghost->scale;

		//go->scale.Set(2, 2, 2);
		go->mass = 2;
		m_ghost->vel = go->vel;
		m_ghost->active = false;
	}

	static bool BButtonState = false;
	if (!BButtonState && Application::IsKeyPressed('B'))
	{
		BButtonState = true;
		std::cout << "'B' DOWN" << std::endl;


	}

	else if (BButtonState && !Application::IsKeyPressed('B'))
	{
		BButtonState = false;
		std::cout << "'B' UP" << std::endl;

		GameObject *ball = FetchGO();
		ball->type = GameObject::GO_BALL;


		ball->pos.Set(m_worldWidth * 0.9f, 10, 0);
		ball->vel.Set(0.f, 10.f, 0.f);
		ball->scale.Set(5, 5, 5);

		//go->scale.Set(2, 2, 2);
		ball->mass = 125;
	}

	PhysicSIM(dt);
}

void SceneCollision::PhysicSIM(double dt)
{
	//Physics Simulation Section
	m_ghost->active = true;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->type == GameObject::GO_BALL)
			{

				go->pos += go->vel * dt;
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


				//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap

			}

			for (std::vector<GameObject *>::iterator its = it + 1; its != m_goList.end(); ++its)
			{
				GameObject *other = (GameObject*)*its;
				if (!other->active)
					continue;
				/*if (other->type != GameObject::GO_BALL)
				continue;*/

				GameObject *goA = go, *goB = other;
				if (go->type != GameObject::GO_BALL)
				{
					if (other->type != GameObject::GO_BALL)
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

				//Exercise 10: handle collision using momentum swap instead


				//Exercise 12: improve inner loop to prevent double collision

				//Exercise 13: improve collision detection algorithm [solution to be given later] 
			}
		}
	}
}

void SceneCollision::CollisionResponse(GameObject* go1, GameObject* go2)
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
	case GameObject::GO_PILLAR:
	{
		Vector3 u = go1->vel;
		Vector3 N = (go1->pos - go2->pos).Normalized();
		go1->vel = u - 2 * u.Dot(N) * N;
		break;
	}
	}
}

bool SceneCollision::CheckCollision(GameObject* go, GameObject* other, double dt)
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
		Vector3 b1 = go->pos + go->vel * dt;
		Vector3 N = other->normal;
		float r = go->scale.x;
		float h = other->scale.x;
		float l = other->scale.y;

		Vector3 relativeVel = go->vel;
		Vector3 relativeDisplacement = w0 - b1;
		Vector3 NP(-N.y, N.x);
		if (relativeDisplacement.Dot(N) < 0)
			N = -N;

		return abs((w0 - b1).Dot(N) < r + h / 2 && abs(relativeDisplacement.Dot(NP)) < r + 1 / 2 && relativeVel.Dot(N) > 0);
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go->pos;
		Vector3 p2 = other->pos;

	}
	}
	
}

float SceneCollision::CheckCollision2(GameObject* go1, GameObject* go2)
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
void SceneCollision::RenderGO(GameObject *go)
{
	switch(go->type)
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
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x,go->pos.y,go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)),0,0,1);
		modelStack.Scale(go->scale.x,go->scale.y,go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
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

void SceneCollision::Render()
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
	
	RenderMesh(meshList[GEO_AXES], false);

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text

	//Exercise 5: Render m_objectCount
	std::ostringstream ss3;
	ss3.precision(3);
	ss3 << "No. of Objects: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 6);
	//Exercise 8c: Render initial and final momentum

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 6);
	
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
