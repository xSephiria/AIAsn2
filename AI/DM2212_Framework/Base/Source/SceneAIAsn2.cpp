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

	magician = FetchGO();
	magician->type = GameObject::GO_MAGICIAN;
	magician->pos.Set(0, 10, 0);
	magician->vel.Set(10, 0, 0);
	magician->scale.Set(5, 5, 5);
	magicianRechargeTimer = 0.f;

	state = MAINMENU;
	//tempGS = STAGE1;

	// Main menu words
	mainMenuOptions[0] = "Start Game";
	mainMenuOptions[1] = "Quit";
	mainMenuCursor = 0;
	cursorDebounce = 0.f;

	quitGame = false;

}

void SceneAIAsn2::Update(double dt)
{

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
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
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