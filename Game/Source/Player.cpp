#include "Render.h"
#include "Textures.h"
#include "App.h"
#include "Map.h"
#include "List.h"
#include "Player.h"
#include "Input.h"
#include "Scene.h"
#include "Transitions.h"
#include "Audio.h"
#include "SceneLose.h"
#include "CastleScene.h"
#include "Animation.h"
#include "Window.h"
#include "SceneTitle.h"
#include "Text.h"

#ifdef OPTICKPROFILE
#include "optick.h"
#endif // OPTICKPROFILE


Player::Player() : Module() 
{
	name.create("player");
	speed = { 0.0f,0.0f };
	maxSpeed = 3.0f;
	jumpSpeed = -8.5f;
	ladderSpeed = 2.0f;
	terminalSpeed = 10.0f;
	width = 16;
	height = 16;
	fullLives = 3;
	lives = fullLives;
	onLadder = false;
	//groundA = 1.0f;
	//airA = 0.1f;
	//a = groundA;
	a = 1.0f;
	doLogic = false;
	acumulatedMs = 0.0f;
	updateMsCycle = 16.66666666f; //A 60 fps
	targetSpeed = {0,0};
	flip = false;
	idleAnimation.PushBack({ 0, 16, 16, 16 });
	runAnimation.PushBack({ 16, 16, 16, 16 });
	runAnimation.PushBack({ 32, 16, 16, 16 });
	runAnimation.speed = 6.0f;
	climbAnimation.PushBack({ 48, 16, 16, 16 });
	climbAnimation.PushBack({ 291, 16, 16, 16 });
	//If change the climbAnimation speed remember to change it to on the move ladder function
	climbAnimation.speed = 6.0f;
	jumpAnimation.PushBack({ 16, 16, 16, 16 });
	
	white = { 255,255,255 };

	
}

Player::~Player() 
{

}

bool Player::Awake(pugi::xml_node& playerNode)
{
	texturePath.create(playerNode.child_value("texture"));
	coinTextPath.create(playerNode.child_value("cointexture"));
	textureRect = { playerNode.child("textureRect").attribute("x").as_int(), playerNode.child("textureRect").attribute("y").as_int(), playerNode.child("textureRect").attribute("w").as_int(), playerNode.child("textureRect").attribute("h").as_int() };
	heartRect = { playerNode.child("heartRect").attribute("x").as_int(), playerNode.child("heartRect").attribute("y").as_int(), playerNode.child("heartRect").attribute("w").as_int(), playerNode.child("heartRect").attribute("h").as_int() };
	initialPos = iPoint(playerNode.child("position").attribute("x").as_int(), playerNode.child("position").attribute("y").as_int());
	coinPos = iPoint(playerNode.child("coinposition").attribute("x").as_int(), playerNode.child("coinposition").attribute("y").as_int());
	heartPos = iPoint(playerNode.child("heartposition").attribute("x").as_int(), playerNode.child("heartposition").attribute("y").as_int());
	heartCounterPos = { 30,2790 };
	checkpoint1x = int(playerNode.child("checkpointpos1x").attribute("x").as_int());
	checkpoint1y = int(playerNode.child("checkpointpos1y").attribute("y").as_int());
	checkpoint2x = int(playerNode.child("checkpointpos2x").attribute("x").as_int());
	checkpoint2y = int(playerNode.child("checkpointpos2y").attribute("y").as_int());
	checkpoint3x = int(playerNode.child("checkpointpos3x").attribute("x").as_int());
	checkpoint3y = int(playerNode.child("checkpointpos3y").attribute("y").as_int());
	return true;
}

void Player::Init()
{
	enabled = false;
	active = true;

}

bool Player::Start()
{
	text->Start();

	texture = app->tex->Load(texturePath.GetString());
	jumpSound = app->audio->LoadFx("Assets/audio/fx/jump.wav");
	checkpointSound = app->audio->LoadFx("Assets/audio/fx/checkpoint.wav");
	position = initialPos;
	coinTexture = app->tex->Load(coinTextPath.GetString());
	heartTexture = app->tex->Load(texturePath.GetString());

	heartCounterTex = text->AddText("Assets/Textures/8bit.ttf", 10, (const char*)lives, white);
	moveTut = text->AddText("Assets/Textures/8bit.ttf", 20, "PRESS W, A, S, D, TO MOVE", white);
	
	return true;
}


float Sign(float num) //Problems: Where should we put this function
{
	if (num > 0)
		return 1.0f;
	else if (num < 0)
		return -1.0f;
	else
		return 0.0f;
}

bool Player::Update(float dt) 
{
#ifdef OPTICKPROFILE
	OPTICK_EVENT("PlayerUpdate");
#endif // OPTICKPROFILE

	//Get the input and update the movement variables accordingly
	bool onPlatform = OnPlatform();
	bool onDeath = OnDeath();
	bool onChange = Onchange();
	bool onSave = OnSave();
	bool onEnd = OnEnd();

	currentAnimation = &idleAnimation;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if(!onLadder)
			if(SnapToLadder(onPlatform, false))
				onLadder = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if(!onLadder)
			if (SnapToLadder(onPlatform, true)) 
				onLadder = true;
	}
	if (onLadder) 
	{
		currentAnimation = &climbAnimation;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			if (!OnBlockedTile())
			{
				onLadder = false;
				speed.x = ladderSpeed;
				
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			if (!OnBlockedTile())
			{
				onLadder = false;
				speed.x = -ladderSpeed;
				
			}
		}
	}
	else 
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			targetSpeed.x = maxSpeed; 
			currentAnimation = &runAnimation;
			
		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{ 
			targetSpeed.x = -maxSpeed;
			currentAnimation = &runAnimation;
		}
		else
			targetSpeed.x = 0;
	}

	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) //Problem: KEY_DOWN or KEY_REPEAT
	{
		if (onPlatform)
		{
			speed.y = jumpSpeed;
			app->audio->PlayFx(jumpSound);
		}
	}
	if (onPlatform)
	{
		targetSpeed.y = 0;
		//a = groundA;
	}
	else
	{
		targetSpeed.y = terminalSpeed;
		//a = airA;
	}

	acumulatedMs += dt * 1000.0f;
	if (acumulatedMs >= updateMsCycle)
		doLogic = true;
	if (doLogic) 
	{
		if (onLadder) 
		{
			MoveLadder();
		}
		else 
		{
			fPoint direction = fPoint(Sign(targetSpeed.x - speed.x), Sign(targetSpeed.y - speed.y));
			speed.x += a * direction.x;
			speed.y += a * direction.y;
			if (Sign(targetSpeed.x - speed.x) != direction.x)
				speed.x = targetSpeed.x;
			if (Sign(targetSpeed.y - speed.y) != direction.y)
				speed.y = targetSpeed.y;

			Move();
		}

		acumulatedMs = 0.0f;
		doLogic = false;
		app->render->CameraMovement();//Problem: if we dont put the camera movement here the player gets drawn double
		CoinMovement();
		HeartMovement();
		HeartCounterMovement();
	}

	

	if (onDeath)
	{
		//Die(); Respawn
		/*app->transitions->FadeToBlack(app->scene,app->loseScene);*/
		app->LoadGame();
	}
	
	if (onChange)
	{
		app->transitions->FadeToBlack(app->scene, app->castleScene);
	}
	
	if (onEnd)
	{
		app->transitions->FadeToBlack(app->castleScene, app->loseScene);
	}

	if (onSave)
	{
		app->SaveGame();
		app->audio->PlayFx(checkpointSound);
	}
	

	/*if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		app->transitions->FadeToBlack(app->scene, app->castleScene);*/

	if (speed.y != 0 && !onLadder)
		currentAnimation = &jumpAnimation;

	//DEBUG Checkpoints map 1.
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		iPoint checkpoint1 = Map::MapToWorld(checkpoint1x, checkpoint1y);

		position = checkpoint1;
	}

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		iPoint checkpoint2 = Map::MapToWorld(checkpoint2x, checkpoint2y);

		position = checkpoint2;
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		iPoint checkpoint3 = Map::MapToWorld(checkpoint3x, checkpoint3y);

		position = checkpoint3;
	}

	
	Draw(dt);
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(coinTexture);
	app->tex->UnLoad(heartTexture);
	app->tex->UnLoad(heartCounterTex);
	app->tex->UnLoad(moveTut);
	app->audio->UnloadMusic();
	app->audio->UnloadFx();
	text->Clean();

	return true;
}

iPoint Player::GetPosition() const
{
	return position;
}

void Player::Draw(float dt)
{
	//Animations
	//Diferent tile drawings depending on the player action or state
	//Maybe some tiles will need to get fliped depending on the orientation
	
	if (flip)
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(dt), 1.0f, SDL_FLIP_HORIZONTAL);
	else
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(dt), 1.0f);

	app->render->DrawTexture(coinTexture, coinPos.x, coinPos.y);

	app->render->DrawTexture(heartTexture, heartPos.x, heartPos.y, &heartRect);

	app->render->DrawTexture(heartCounterTex, heartCounterPos.x, heartCounterPos.y);

	app->render->DrawTexture(moveTut, 40, 2900);

	
}


bool Player::OnPlatform()
{
	if ((position.x % Map::GetTileWidth()) != 0) //check if we have to check 2 columns or 1
	{
		iPoint left = Map::WorldToMap(position.x, position.y + height + 1);
		uint leftIndex = left.y * Map::GetMapHeight() + left.x;
		if (Map::GetTileProperty(leftIndex, "Blocked"))
			return true;

		iPoint right = Map::WorldToMap(position.x + width, position.y + height + 1);
		uint rightIndex = right.y * Map::GetMapHeight() + right.x;
		if (Map::GetTileProperty(rightIndex, "Blocked"))
			return true;

		return false;
	}
	else
	{
		iPoint left = Map::WorldToMap(position.x, position.y + height + 1);
		uint leftIndex = left.y * Map::GetMapHeight() + left.x;
		if (Map::GetTileProperty(leftIndex, "Blocked"))
			return true;

		return false;
	}
}

bool Player::OnDeath()
{
	if ((position.x % Map::GetTileWidth()) != 0) //check if we have to check 2 columns or 1
	{
		iPoint left = Map::WorldToMap(position.x, position.y + height + 1);
		uint leftIndex = left.y * Map::GetMapHeight() + left.x;
		if (Map::GetTileProperty(leftIndex, "Death"))
			return true;

		iPoint right = Map::WorldToMap(position.x + width, position.y + height + 1);
		uint rightIndex = right.y * Map::GetMapHeight() + right.x;
		if (Map::GetTileProperty(rightIndex, "Death"))
				return true;

		return false;
	}
	else
	{
		iPoint left = Map::WorldToMap(position.x, position.y + height + 1);
		uint leftIndex = left.y * Map::GetMapHeight() + left.x;
			if (Map::GetTileProperty(leftIndex, "Death"))
				return true;

		return false;
	}
}

bool Player::Onchange()
{
	iPoint left = Map::WorldToMap(position.x, position.y);
	uint leftIndex = left.y * Map::GetMapHeight() + left.x;
	if (Map::GetTileProperty(leftIndex, "Change"))
		return true;

	return false;
}

bool Player::OnEnd()
{
	iPoint left = Map::WorldToMap(position.x, position.y);
	uint leftIndex = left.y * Map::GetMapHeight() + left.x;
	if (Map::GetTileProperty(leftIndex, "End"))
		return true;

	return false;
}

bool Player::OnSave()
{
	iPoint left = Map::WorldToMap(position.x, position.y);
	uint leftIndex = left.y * Map::GetMapHeight() + left.x;
	if (Map::GetTileProperty(leftIndex, "Save"))
		return true;

	return false;
}

bool Player::OnLadder(iPoint position)
{
	position = Map::WorldToMap(position.x, position.y);
	uint index = position.y * Map::GetMapHeight() + position.x;
	if (Map::GetTileProperty(index, "Ladder"))
	{
		return true;
	}
	return false;
}


bool Player::OnBlockedTile()
{
	iPoint tilePosition = Map::WorldToMap(position.x, position.y);
	uint index = tilePosition.y * Map::GetMapHeight() + tilePosition.x;
	if (Map::GetTileProperty(index, "Blocked"))
	{
		return true;
	}

	return false;
}

bool Player::SnapToLadder(bool onPlatform, bool down)
{
	iPoint playerCenter(position.x + width / 2, position.y + height / 2);
	if (OnLadder(playerCenter))
	{
		playerCenter = Map::WorldToMap(playerCenter.x, playerCenter.y);
		uint index = playerCenter.y * Map::GetMapHeight() + playerCenter.x; //Problem : Maybe we don't need the index calculation to snap the player
		position.x = (index % Map::GetMapWidth()) * Map::GetTileWidth();
		return true;
	}

	if (onPlatform) 
	{
		if (down) //chack if there is a ladder the tile below
		{
			iPoint playerCenterDown(position.x + width / 2, position.y + height);
			if (OnLadder(playerCenterDown))
			{
				playerCenterDown = Map::WorldToMap(playerCenter.x, playerCenter.y);
				uint index = playerCenterDown.y * Map::GetMapHeight() + playerCenterDown.x; //Problem : Maybe we don't need the index calculation to snap the player
				position.x = (index % Map::GetMapWidth()) * Map::GetTileWidth();
				position.y += Map::GetTileHeight();
				return true;
			}
		}
		else ////chack if there is a ladder the tile avobe
		{
			iPoint playerCenterUp(position.x + width / 2, position.y - 1);
			if (OnLadder(playerCenterUp))
			{
				playerCenterUp = Map::WorldToMap(playerCenter.x, playerCenter.y);
				uint index = playerCenterUp.y * Map::GetMapHeight() + playerCenterUp.x; //Problem : Maybe we don't need the index calculation to snap the player
				position.x = (index % Map::GetMapWidth()) * Map::GetTileWidth();
				position.y -= Map::GetTileHeight();
				return true;
			}
		}
	}
	return false;
}

void Player::MoveLadder()
{
	bool moving = false;
	bool up = false;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !app->input->GetKey(SDL_SCANCODE_S))
	{
		moving = true;
		up = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !app->input->GetKey(SDL_SCANCODE_W))
	{
		moving = true;
		up = false;
	}

	if(moving)
	{
		if(up)
		{
			position.y -= ladderSpeed;
			if (!OnLadder(position))
			{
				position.y -= position.y % Map::GetTileHeight();
				speed.x = 0;
				speed.y = 0;
				onLadder = false;
			}
		}
		else 
		{
			position.y += ladderSpeed;
			if (!OnLadder(position))
			{
				position.y += (position.y + height) % Map::GetTileHeight();
				speed.x = 0;
				speed.y = 0;
				onLadder = false;
			}
		}
	}

	if (moving)
		climbAnimation.speed = 6.0f;
	else
		climbAnimation.speed = 0.0f;
}

bool Player::Load(pugi::xml_node& playerNode)
{
	position.x = playerNode.child("position").attribute("x").as_int();
	position.y = playerNode.child("position").attribute("y").as_int();
	return true;
}

bool Player::Save(pugi::xml_node& playerNode) const
{
	pugi::xml_node playerPos = playerNode.append_child("position");

	playerPos.append_attribute("x") = position.x;
	playerPos.append_attribute("y") = position.y;
	return true;
}

void Player::Move() 
{

	if (speed.x != 0) 
	{
		if (position.y % Map::GetTileHeight() != 0) 
		{ //check 2 rows of tiles to find obstacles
			iPoint top, bottom;
			float distance;
			bool movingright = speed.x > 0;
			if (movingright) 
			{
				top = Map::WorldToMap(position.x + width, position.y);
				bottom = Map::WorldToMap(position.x + width, position.y + height);
			}
			else 
			{
				top = Map::WorldToMap(position.x, position.y);
				bottom = Map::WorldToMap(position.x, position.y + height);
			}

			uint indextop = top.y * Map::GetMapWidth() + top.x;
			uint indexbottom = bottom.y * Map::GetMapWidth() + bottom.x;

			if (movingright) 
			{ //estem anant cap a la dreta

				uint lastcheckindex = (indextop - (indextop % Map::GetMapWidth())) + Map::GetMapWidth() -1;
					for (int i = indextop; i <= lastcheckindex; ++i) 
					{
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							top.x = i % Map::GetMapWidth();
							top.y = i / Map::GetMapWidth();
							top = Map::MapToWorld(top.x -1, top.y);
							break;
						}
					}

				lastcheckindex = (indexbottom - (indexbottom % Map::GetMapWidth())) + Map::GetMapWidth() -1;
					for (int i = indexbottom; i <= lastcheckindex; ++i) 
					{
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							bottom.x = i % Map::GetMapWidth();
							bottom.y = i / Map::GetMapWidth();
							bottom = Map::MapToWorld(bottom.x - 1, bottom.y);
							break;
						}
					}

				float topdistance = top.x - position.x;
				float bottomdistance = bottom.x - position.x;
				distance = MIN(topdistance, bottomdistance);
			}
			else 
			{//estem anant cap a l'esquerra

				uint lastcheckindex = indextop - (indextop % Map::GetMapWidth());
					for (int i=indextop; i >= lastcheckindex; --i) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							top.x = i % Map::GetMapWidth();
							top.y = i / Map::GetMapWidth();
							top = Map::MapToWorld(top.x + 1, top.y);
							break;
						}
					}
				lastcheckindex = indexbottom - (indexbottom % Map::GetMapWidth());
					for (int i = indexbottom; i >= lastcheckindex; --i) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							bottom.x = i % Map::GetMapWidth();
							bottom.y = i / Map::GetMapWidth();
							bottom = Map::MapToWorld(bottom.x + 1, bottom.y);
							break;
						}
					}
				float topdistance = position.x - top.x;
				float bottomdistance = position.x - bottom.x;
				distance = MIN(topdistance, bottomdistance);
			}

			if (movingright) 
			{
				position.x += MIN(speed.x, distance);
				flip = false;
			}
			else 
			{
				position.x += MAX(speed.x, -distance);
				flip = true;
			}
			if (distance == 0)
				speed.x = 0;
		}
		else 
		{ //just check 1 row of tiles to find obstacles
			iPoint top;
			float distance;
			bool movingright = speed.x > 0;
			if (movingright) 
			{
				top = Map::WorldToMap(position.x + width, position.y);
			}
			else 
			{
				top = Map::WorldToMap(position.x, position.y);
			}

			uint indextop = top.y * Map::GetMapWidth() + top.x;

			if (movingright) 
			{ //estem anant cap a la dreta

				uint lastcheckindex = (indextop - (indextop % Map::GetMapWidth())) + Map::GetMapWidth() -1;
					for (int i = indextop; i <= lastcheckindex; ++i) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							top.x = i % Map::GetMapWidth();
							top.y = i / Map::GetMapWidth();
							top = Map::MapToWorld(top.x -1, top.y);
							break;
						}
					}
				distance = top.x - position.x;
			}
			else 
			{//estem anant cap a l'esquerra

				uint lastcheckindex = indextop - (indextop % Map::GetMapWidth());
					for (int i = indextop; i >= lastcheckindex; --i) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							top.x = i % Map::GetMapWidth();
							top.y = i / Map::GetMapWidth();
							top = Map::MapToWorld(top.x + 1, top.y);
							break;
						}
					}
				
				distance = position.x - top.x;
			}

			if (movingright) 
			{
				position.x += MIN(speed.x, distance);
				flip = false;
			}
			else 
			{
				position.x += MAX(speed.x, -distance);
				flip = true;
			}
			if (distance == 0)
				speed.x = 0;
		}
	}

	//Repetir el process pel moviment en l'altre coordenada
	if (speed.y != 0) 
	{
		if (position.x % Map::GetTileWidth() != 0) 
		{ //check 2 columns of tiles to find obstacles
			iPoint left, right;
			float distance;
			bool movingup = speed.y < 0;
			if (movingup) 
			{
				left = Map::WorldToMap(position.x, position.y);
				right = Map::WorldToMap(position.x + width, position.y);
			}
			else 
			{
				left = Map::WorldToMap(position.x, position.y + height);
				right = Map::WorldToMap(position.x + width, position.y + height);
			}

			uint indexleft = left.y * Map::GetMapWidth() + left.x;
			uint indexright = right.y * Map::GetMapWidth() + right.x;

			if (movingup) 
			{ //estem anant cap amunt

				int lastcheckindex = indexleft % Map::GetMapWidth();
					for (int i = indexleft; i >= lastcheckindex; i = i - Map::GetMapWidth()) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							left.x = i % Map::GetMapWidth();
							left.y = i / Map::GetMapWidth();
							left = Map::MapToWorld(left.x, left.y + 1);
							break;
						}
					}
				lastcheckindex = indexright % Map::GetMapWidth();
					for (int i = indexright; i >= lastcheckindex; i -= Map::GetMapWidth()) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							right.x = i % Map::GetMapWidth();
							right.y = i / Map::GetMapWidth();
							right = Map::MapToWorld(right.x, right.y + 1);
							break;
						}
					}
				float leftdistance = position.y - left.y;
				float rightdistance = position.y - right.y;
				distance = MIN(leftdistance, rightdistance);
			}
			else 
			{//movingdown

				uint lastcheckindex = indexleft % Map::GetMapWidth() + (Map::GetMapHeight() -1) * Map::GetMapWidth();
					for (int i = indexleft; i <= lastcheckindex; i += Map::GetMapWidth()) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							left.x = i % Map::GetMapWidth();
							left.y = i / Map::GetMapWidth();
							left = Map::MapToWorld(left.x, left.y - 1);
							break;
						}
					}
				lastcheckindex = indexright % Map::GetMapWidth() + (Map::GetMapHeight() - 1) * Map::GetMapWidth();
					for (int i = indexright; i <= lastcheckindex; i += Map::GetMapWidth()) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) {
							//get the position of the tile
							right.x = i % Map::GetMapWidth();
							right.y = i / Map::GetMapWidth();
							right = Map::MapToWorld(right.x, right.y -1);
							break;
						}
					}
				float leftdistance = left.y - position.y;
				float rightdistance = right.y - position.y;
				distance = MIN(leftdistance, rightdistance);
			}

			if (movingup) 
			{
				position.y += MAX(speed.y, -distance);
			}
			else 
			{
				position.y += MIN(speed.y, distance);
			}
			if (distance == 0)
				speed.y = 0;
		}
		else { //just check 1 row of tiles to find obstacles
			iPoint left;
			float distance;
			bool movingup = speed.y < 0;
			if (movingup) 
			{
				left = Map::WorldToMap(position.x, position.y);
			}
			else 
			{
				left = Map::WorldToMap(position.x, position.y + height);
			}

			uint indexleft = left.y * Map::GetMapWidth() + left.x;

			if (movingup) 
			{ //estem anant cap a la dreta

				int lastcheckindex = indexleft % Map::GetMapWidth();
					for (int i = indexleft; i >= lastcheckindex; i -= Map::GetMapWidth()) {//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) {
							//get the position of the tile
							left.x = i % Map::GetMapWidth();
							left.y = i / Map::GetMapWidth();
							left = Map::MapToWorld(left.x, left.y + 1); //PROBLEM: WE DONT KNOW WHY THIS +1 OFFSET
							break;
						}
					}
				distance = position.y - left.y;
			}
			else 
			{//moving down

				uint lastcheckindex = indexleft % Map::GetMapWidth() + (Map::GetMapHeight() - 1) * Map::GetMapWidth();
					for (int i = indexleft; i <= lastcheckindex; i += Map::GetMapWidth()) 
					{//iterar les tilesets
						if (Map::GetTileProperty(i, "Blocked")) 
						{
							//get the position of the tile
							left.x = i % Map::GetMapWidth();
							left.y = i / Map::GetMapWidth();
							left = Map::MapToWorld(left.x, left.y-1); //PROBLEM: WE DONT KNOW WHY THIS -1 OFFSET 
							break;
						}
					}

				distance = left.y - position.y;
			}

			if (movingup) 
			{
				position.y += MAX(speed.y, -distance);
			}
			else 
			{
				position.y += MIN(speed.y, distance);
			}
			if (distance == 0)
				speed.y = 0;
		}
	}

}

void Player::Die()
{

	position = initialPos;

}

void Player::CoinMovement()
{
	if (position.x <= (Map::GetMapWidth() * Map::GetTileWidth()) - (app->win->width / 2))
	{
		coinPos.x = (app->player->GetPosition().x - app->render->camera.w / 2);

	}
	if (coinPos.x < 0)
	{
		coinPos.x = 0;

	}

	coinPos.y = (app->player->GetPosition().y - app->render->camera.h / 2);


	if (coinPos.y < 0)
	{
		coinPos.y = 0;

	}
}

void Player::HeartMovement()
{
	if (position.x <= (Map::GetMapWidth() * Map::GetTileWidth()) - (app->win->width / 2))
	{
		heartPos.x = (app->player->GetPosition().x - app->render->camera.w / 2) + 8;

	}
	if (heartPos.x < 8)
	{
		heartPos.x = 8;

	}

	heartPos.y = (app->player->GetPosition().y - app->render->camera.h / 2) + 40;


	if (heartPos.y < 0)
	{
		heartPos.y = 0;

	}
}


void Player::HeartCounterMovement()
{
	if (position.x <= (Map::GetMapWidth() * Map::GetTileWidth()) - (app->win->width / 2))
	{
		heartCounterPos.x = (app->player->GetPosition().x - app->render->camera.w / 2) + 30;

	}
	if (heartCounterPos.x < 30)
	{
		heartCounterPos.x = 30;

	}

	heartCounterPos.y = (app->player->GetPosition().y - app->render->camera.h / 2) + 40;


	if (heartCounterPos.y < 0)
	{
		heartCounterPos.y = 0;

	}
}

void Player::Lives(Module* mod)
{
	if (OnDeath())
	{
		--lives;
	}

	if (lives <= 0)
	{
		app->transitions->FadeToBlack(mod, app->loseScene);

		lives = fullLives;
	}
}


// TEMPORARY UI FUNCTIONS



