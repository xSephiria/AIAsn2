#include "Survival.h"
#include "GL\glew.h"
#include "Application.h"

CSurvival::CSurvival()
{
}


CSurvival::~CSurvival()
{
}

string CSurvival::itos(const long value)
{
	ostringstream buffer;
	buffer << value;
	return buffer.str();
}

int CSurvival::Rand(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

bool CSurvival::DistXY(Vector3 first, Vector3 second, float dist)
{
	Vector3 temp = first - second;
	temp.z = 0;

	if (temp.LengthSquared() < dist * dist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CSurvival::Init()
{
	SceneBase::Init();

	//Action Bars
	CHungerBar = 100;
	CThirstBar = 100;
	HungerBarCounter = 50;
	ThirstBarCounter = 70;
	PickUpDelay = 10;
	RelocateTimer = 10;
	ActionTimer = 50;

	//Items
	Food = 0;
	Water = 0;

	AutoRun = true;
	FoodHunt = false;
	WaterHunt = false;
	Eating = false;
	Drinking = false;

	quitGame = false;

	ReLocate = true;
	ReSpawn = false;
	 
	PlayerPosition = Vector3(75, 50, -1);
	TargetPosition = Vector3(0, 0, -2);
	ScavengingPosition = Vector3(5, 5, -2);

	player = FetchGO();
	player->type = GameObject::GO_PLAYER;
	player->pos = PlayerPosition;
	//player->vel = Vector3(2, 2, 0);
	player->normal = Vector3(0, 1, 0);
	player->active = true;
	player->scale.Set(3, 3, 1);

	ScavengingPoint = FetchGO();
	ScavengingPoint->type = GameObject::GO_SCAVAGEPOINT;
	ScavengingPoint->pos = ScavengingPosition;
	ScavengingPoint->normal = Vector3(0, 1, 0);
	ScavengingPoint->active = true;
	ScavengingPoint->scale.Set(10, 12, 1);

	for (int i = 0; i < 6; i++)
	{
		rocks[i] = FetchGO();
		rocks[i]->type = GameObject::GO_ROCKS;
		//rocks[i]->pos = Vector3(50, 5, -2);
		rocks[i]->normal = Vector3(0, 1, 0);
		rocks[i]->active = true;
		rocks[i]->scale.Set(13, 10, 1);
	}
	
	rocks[0]->pos = Vector3(50, 5, -2);
	rocks[1]->pos = Vector3(75, 50, -2);
	rocks[2]->pos = Vector3(20, 75, -2);
	rocks[3]->pos = Vector3(30, 20, -2);
	rocks[4]->pos = Vector3(98, 10, -2);
	rocks[5]->pos = Vector3(120, 60, -2);
		 

	state = Explore;
}

void CSurvival::Simulation()
{
	if (!quitGame)
	{
		if (AutoRun)
		{
			//If Player is not dead
			if (state != Dead)
			{
				//Both Hunger and Thirst is still available
				if (CHungerBar > 0 && CThirstBar > 0)
				{
					//Hunger goes down
					HungerBarCounter--;
					if (HungerBarCounter <= 0)
					{
						CHungerBar--;
						HungerBarCounter = 50;
					}

					//Thirst goes down
					ThirstBarCounter--;
					if (ThirstBarCounter <= 0)
					{
						CThirstBar--;
						ThirstBarCounter = 75;
					}

					if (ActionTimer <= 0 && (Drinking || Eating ))
					{
						Drinking = false;
						Eating = false;
						//Jumping = false;
						state = Explore;
						ActionTimer = 50;
					}

					if (JumpTimer <= 0 && Jumping)
					{
						Jumping = false;
						state = Explore;
						JumpTimer = 10;
					}


					RelocateTimer--;
					if (RelocateTimer <= 0 && DistXY(player->pos, TargetPosition, 10) && !Drinking && !Eating && !Jumping)
					{
						RelocateTimer = 10;
						ReLocate = true;
					}
						

					switch (state)
					{
					case Explore:
					{
									//music->engine->stopAllSounds();
									// Hunger less than 50, go to Eat
									if (CHungerBar < 50)
									{
										state = Eat;
									}
									// Thirst less than 50, go to Drink
									else if (CThirstBar < 50)
									{
										state = Drink;
									}
									
									// Not Hungry nor Thirsty && Not full
									else if ((CHungerBar > 50 && CHungerBar < 70)
										&& (CThirstBar > 50 && CThirstBar < 70)
										&& Rand(1, 5) == 3 )
									{
										FoodHunt = true;
										WaterHunt = true;
										TargetPosition = ScavengingPosition;
										state = Scavenging;
									}
									else if (DistXY(player->pos, rocks[0]->pos, rocks[0]->scale.x)
										|| DistXY(player->pos, rocks[1]->pos, rocks[0]->scale.x)
										|| DistXY(player->pos, rocks[2]->pos, rocks[0]->scale.x)
										|| DistXY(player->pos, rocks[3]->pos, rocks[0]->scale.x)
										|| DistXY(player->pos, rocks[4]->pos, rocks[0]->scale.x)
										|| DistXY(player->pos, rocks[5]->pos, rocks[0]->scale.x))
									{
										state = Jump;
									}
									
									break;
					}

					case Scavenging:
					{
									   PickUpDelay--;
									   // Hunt for Food
									   if (Food < 10 && FoodHunt )
									   {
										   TargetPosition = ScavengingPosition;

										   if ((DistXY(player->pos, ScavengingPosition, 2)) && (PickUpDelay <= 0))
										   {
											   Food++;
											   PickUpDelay = 10;
											   FoodHunt = false;
											   ReSpawn = true;
											   state = Eat;
											
										   }
									   }
									   // Hunt for Water
									   else if (Water < 10 && WaterHunt )
									   {
										   TargetPosition = ScavengingPosition;

										   if ((DistXY(player->pos, ScavengingPosition, 2)) && (PickUpDelay <= 0))
										   {
											   Water++;
											   PickUpDelay = 10;
											   WaterHunt = false;
											   ReSpawn = true;
											   state = Drink;
										   }
									   }
									   // Else go explore
									   /*else
									   {
										   if (!FoodHunt && !WaterHunt)
										   {
											   ReLocate = true;
											  
										   }
										   state = Explore;
									   }*/
									   else if (DistXY(player->pos, rocks[0]->pos, rocks[0]->scale.x)
										   || DistXY(player->pos, rocks[1]->pos, rocks[0]->scale.x)
										   || DistXY(player->pos, rocks[2]->pos, rocks[0]->scale.x)
										   || DistXY(player->pos, rocks[3]->pos, rocks[0]->scale.x)
										   || DistXY(player->pos, rocks[4]->pos, rocks[0]->scale.x)
										   || DistXY(player->pos, rocks[5]->pos, rocks[0]->scale.x))
									   {
										   state = Jump;
									   }
									   break;
					}
					case Drink:
					{
								  ActionTimer--;
								  //Player is thirsty and have water
								  if (CThirstBar < 50 && Water >= 1)
								  {
									  
									  CThirstBar += 20;
									  Water--;
									  Drinking = true;
									  //state = Explore;
								  }
								  //Player is thirsty but does not have water
								  else if (CThirstBar < 50 && Water < 1)
								  {
									  WaterHunt = true;
									  TargetPosition = ScavengingPosition;
									  state = Scavenging;
								  }
								  break;
					}

					case Eat:
					{
								ActionTimer--;
								//Player is hungry and have food
								if (CHungerBar < 50 && Food >= 1)
								{
									
									CHungerBar += 50;
									Food--;
									Eating = true;
					
								}
								//Player is hungry but does not have food
								else if (CHungerBar < 50 && Food < 1)
								{
									FoodHunt = true;
									TargetPosition = ScavengingPosition;
									state = Scavenging;
								}
								//Player is not hungry
								break;
					}
					case Jump:
					{
								/* if (!DistXY(player->pos, rocks[0]->pos, rocks[0]->scale.x)
									 || !DistXY(player->pos, rocks[1]->pos, rocks[0]->scale.x)
									 || !DistXY(player->pos, rocks[2]->pos, rocks[0]->scale.x)
									 || !DistXY(player->pos, rocks[3]->pos, rocks[0]->scale.x)
									 || !DistXY(player->pos, rocks[4]->pos, rocks[0]->scale.x)
									 || !DistXY(player->pos, rocks[5]->pos, rocks[0]->scale.x))
								 {
									 
								 }*/
								 JumpTimer--;
								 for (int i = 0; i < 6; i++)
								 {
									 if (!DistXY(player->pos, rocks[i]->pos, rocks[i]->scale.x))
									 {
										 Jumping = true;
									 }
								 }

								 //if (player->pos  )
								
					}

					}
					//End of Switch Case

				}

			}
		}

	}
}

GameObject* CSurvival::FetchGO()
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

void CSurvival::Update(double dt)
{
	SceneBase::Update(dt);
	
	player->pos.x += (TargetPosition.x - player->pos.x) * dt;
	player->pos.y += (TargetPosition.y - player->pos.y) * dt;

	cout << state << endl;

	if (ReLocate && (!FoodHunt || !WaterHunt))
	{
		TargetPosition.x = Rand(3, 127);
		TargetPosition.y = Rand(3, 97);
		ReLocate = false;
	}
	

	if (ReSpawn)
	{
		ScavengingPoint->pos.x = Rand(3, 127);
		ScavengingPoint->pos.y = Rand(3, 97);

		ScavengingPosition = ScavengingPoint->pos;
		ReSpawn = false;
	}

	Simulation();

}

void CSurvival::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLAYER], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SCAVAGEPOINT:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SCAVENGING], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ROCKS:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ROCKS], false);
		modelStack.PopMatrix();
		break;
	}
}

void CSurvival::PhysicSIM(double dt)
{
	//Physics Simulation Section
	//m_ghost->active = true;
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

void CSurvival::CollisionResponse(GameObject* go1, GameObject* go2)
{
	switch (go2->type)
	{
	case GameObject::GO_PLAYER:
	{
								  Vector3 u = go1->vel;
								  Vector3 N = go2->normal;
								  Vector3 uN = u.Dot(N) * N;
								  go1->vel = u - 2 * uN;
								  go1->vel *= 1.05f;
								  break;
	}
	
	}
}

bool CSurvival::CheckCollision(GameObject* go, GameObject* other, double dt)
{
	switch (other->type)
	{
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

	}

}

void CSurvival::Render()
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
	modelStack.Translate(m_worldWidth / 2, m_worldHeight / 2, -5);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(meshList[GEO_GRASSBG], false);
	modelStack.PopMatrix();


	std::ostringstream ss;

	//Text on Screen
	RenderTextOnScreen(meshList[GEO_TEXT], "Hunger  : " + itos(CHungerBar), Color(0, 1, 0), 3, 3, 55);

	RenderTextOnScreen(meshList[GEO_TEXT], "Thirsty : " + itos(CThirstBar), Color(0, 1, 0), 3, 3, 50);

	RenderTextOnScreen(meshList[GEO_TEXT], "Food Count  : " + itos(Food), Color(0, 1, 0), 3, 25, 55);

	RenderTextOnScreen(meshList[GEO_TEXT], "Water Count : " + itos(Water), Color(0, 1, 0), 3, 25, 50);

	switch (state)
	{
	case Explore:
	{
					modelStack.PushMatrix();
					modelStack.Translate(player->pos.x - player->scale.x , player->pos.y + player->scale.y, -1);
					modelStack.Scale(3, 3, 1);
					RenderText(meshList[GEO_TEXT], "Explore" , Color(0, 1, 0));
					modelStack.PopMatrix();
					break;
	}
		
	case Scavenging:
	{
					   modelStack.PushMatrix();
					   modelStack.Translate(player->pos.x - player->scale.x , player->pos.y + player->scale.y, -1);
					   modelStack.Scale(3, 3, 1);
					   RenderText(meshList[GEO_TEXT], "Scavenging", Color(0, 1, 0));
					   modelStack.PopMatrix();
					   break;
	}
	case Drink:
	{
				  modelStack.PushMatrix();
				  modelStack.Translate(player->pos.x - player->scale.x , player->pos.y + player->scale.y, -1);
				  modelStack.Scale(3, 3, 1);
				  RenderText(meshList[GEO_TEXT], "Drink", Color(0, 1, 0));
				  modelStack.PopMatrix();
				  break;
	}

	case Eat:
	{
				modelStack.PushMatrix();
				modelStack.Translate(player->pos.x - player->scale.x , player->pos.y + player->scale.y, -1);
				modelStack.Scale(3, 3, 1);
				RenderText(meshList[GEO_TEXT], "Eat", Color(0, 1, 0));
				modelStack.PopMatrix();
				break;
	}
	case Dead:
	{
				 modelStack.PushMatrix();
				 modelStack.Translate(player->pos.x - player->scale.x , player->pos.y + player->scale.y, -1);
				 modelStack.Scale(3, 3, 1);
				 RenderText(meshList[GEO_TEXT], "Dead", Color(0, 1, 0));
				 modelStack.PopMatrix();
				 break;
	}
	case Jump:
	{
				 modelStack.PushMatrix();
				 modelStack.Translate(player->pos.x - player->scale.x , player->pos.y + player->scale.y, -1);
				 modelStack.Scale(3, 3, 1);
				 RenderText(meshList[GEO_TEXT], "Jump", Color(0, 1, 0));
				 modelStack.PopMatrix();
				 break;
	}
	}
	
	//RenderTextOnScreen(meshList[GEO_TEXT], "LOL", Color(0, 1, 0), 3, player->pos.x, player->pos.y);
	

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}
}

void CSurvival::Exit()
{
	SceneBase::Exit();
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}

