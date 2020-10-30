#include "Render.h"
#include "Textures.h"
#include "App.h"
#include "Map.h"
#include "List.h"
#include "Player.h"
#include "Input.h"

Player::Player() : Module() {
	name.create("player");
	speed = { 0.0f,0.0f };
	maxSpeed = 3.0f;
	jumpSpeed = -10.0f;
	terminalSpeed = 10.0f;
	width = 16;
	height = 16;
	a = 1.0f;
	threshold = 1.0f;
	doLogic = false;
	acumulatedMs = 0.0f;
	updateMsCycle = 16.66666666f; //A 60 fps
	targetSpeed = {0,0};
}

Player::~Player() {

}

bool Player::Awake(pugi::xml_node& playerNode)
{
	texturePath.create(playerNode.child_value("texture"));
	textureRect = { playerNode.child("textureRect").attribute("x").as_int(), playerNode.child("textureRect").attribute("y").as_int(), playerNode.child("textureRect").attribute("w").as_int(), playerNode.child("textureRect").attribute("h").as_int() };
	return true;
}

bool Player::Start()
{
	texture = app->tex->Load(texturePath.GetString());
	position = { 16, 2716};
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

bool Player::Update(float dt) {
	//Get the input and update the movement variables accordingly
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		targetSpeed.x = maxSpeed;
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		targetSpeed.x = -maxSpeed;
	else
		targetSpeed.x = 0;

	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		if (OnPlatform())
			speed.y = jumpSpeed;
	}

	if (OnPlatform())
		targetSpeed.y = 0;
	else
		targetSpeed.y = terminalSpeed;

	acumulatedMs += dt * 1000.0f;
	if (acumulatedMs >= updateMsCycle)
		doLogic = true;
	if (doLogic) 
	{
		fPoint direction = fPoint(Sign(targetSpeed.x - speed.x),Sign(targetSpeed.y - speed.y));
		speed.x += a * direction.x;
		speed.y += a * direction.y;
		if (Sign(targetSpeed.x - speed.x) != direction.x)
			speed.x = targetSpeed.x;
		if (Sign(targetSpeed.y - speed.y) != direction.y)
			speed.y = targetSpeed.y;
		Move();

		acumulatedMs = 0.0f;
		doLogic = false;
	}
	Draw();
	return true;
}

iPoint Player::GetPosition() const
{
	return position;
}

bool Player::OnPlatform()
{
	if (position.y % app->map->data.height != 0) //PROBLEM: How do we check if we have to check t columns or 1
	{
		iPoint left = app->map->WorldToMap(position.x, position.y + height + 1);
		uint leftIndex = left.y * app->map->data.height + left.x;
		ListItem<MapLayer*>* item = app->map->data.layers.start;
		for (item; item; item = item->next) //Problem: Maybe the function GetTileProperty should be the one iterating the layers
		{
			if (app->map->GetTileProperty(item->data->data[leftIndex], "Blocked"))
				return true;
		}
		iPoint right = app->map->WorldToMap(position.x + width, position.y + height + 1);
		uint rightIndex = right.y * app->map->data.height + right.x;
		item = app->map->data.layers.start;
		for (item; item; item = item->next)
		{
			if (app->map->GetTileProperty(item->data->data[rightIndex], "Blocked"))
				return true;
		}
		return false;
	}
	else
	{
		iPoint left = app->map->WorldToMap(position.x, position.y - 1);
		uint leftIndex = left.y * app->map->data.height + left.x;
		ListItem<MapLayer*>* item = app->map->data.layers.start;
		for (item; item; item = item->next)
		{
			if (app->map->GetTileProperty(item->data->data[leftIndex], "Blocked"))
				return true;
		}
		return false;
	}
}

void Player::Draw() {
	//Diferent tile drawings depending on the player action or state
	//Maybe some tiles will need to get fliped depending on the orientation
	app->render->DrawTexture(texture, position.x, position.y, &textureRect);
}

void Player::Move() {
	if (speed.x != 0) 
	{
		if (position.y % app->map->data.tileHeight != 0) 
		{ //check 2 rows of tiles to find obstacles
			iPoint top, bottom;
			float distance;
			bool movingright = speed.x > 0;
			if (movingright) 
			{
				top = app->map->WorldToMap(position.x + width, position.y);
				bottom = app->map->WorldToMap(position.x + width, position.y + height);
			}
			else 
			{
				top = app->map->WorldToMap(position.x, position.y);
				bottom = app->map->WorldToMap(position.x, position.y + height);
			}

			uint indextop = top.y * app->map->data.width + top.x;
			uint indexbottom = bottom.y * app->map->data.width + bottom.x;

			if (movingright) 
			{ //estem anant cap a la dreta

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				uint lastcheckindex = (indextop - (indextop % app->map->data.width)) + app->map->data.width -1;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indextop; i <= lastcheckindex; ++i) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							top.x = i % app->map->data.width;
							top.y = i / app->map->data.width;
							top = app->map->MapToWorld(top.x -1, top.y);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				item = app->map->data.layers.start;
				found = false;
				lastcheckindex = (indexbottom - (indexbottom % app->map->data.width)) + app->map->data.width -1;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indexbottom; i <= lastcheckindex; ++i) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							bottom.x = i % app->map->data.width;
							bottom.y = i / app->map->data.width;
							bottom = app->map->MapToWorld(bottom.x - 1, bottom.y);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				float topdistance = top.x - position.x;
				float bottomdistance = bottom.x - position.x;
				distance = MIN(topdistance, bottomdistance);
			}
			else 
			{//estem anant cap a l'esquerra

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				uint lastcheckindex = indextop - (indextop % app->map->data.width);
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i=indextop; i >= lastcheckindex; --i) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							top.x = i % app->map->data.width;
							top.y = i / app->map->data.width;
							top = app->map->MapToWorld(top.x + 1, top.y);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				item = app->map->data.layers.start;
				found = false;
				lastcheckindex = indexbottom - (indexbottom % app->map->data.width);
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indexbottom; i >= lastcheckindex; --i) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							bottom.x = i % app->map->data.width;
							bottom.y = i / app->map->data.width;
							bottom = app->map->MapToWorld(bottom.x + 1, bottom.y);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				float topdistance = position.x - top.x;
				float bottomdistance = position.x - bottom.x;
				distance = MIN(topdistance, bottomdistance);
			}

			if (movingright) 
			{
				position.x += MIN(speed.x, distance);
			}
			else 
			{
				position.x += MAX(speed.x, -distance);
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
				top = app->map->WorldToMap(position.x + width, position.y);
			}
			else 
			{
				top = app->map->WorldToMap(position.x, position.y);
			}

			uint indextop = top.y * app->map->data.width + top.x;

			if (movingright) 
			{ //estem anant cap a la dreta

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				uint lastcheckindex = (indextop - (indextop % app->map->data.width)) + app->map->data.width -1;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indextop; i <= lastcheckindex; ++i) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							top.x = i % app->map->data.width;
							top.y = i / app->map->data.width;
							top = app->map->MapToWorld(top.x -1, top.y);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				distance = top.x - position.x;
			}
			else 
			{//estem anant cap a l'esquerra

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				uint lastcheckindex = indextop - (indextop % app->map->data.width);
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indextop; i >= lastcheckindex; --i) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							top.x = i % app->map->data.width;
							top.y = i / app->map->data.width;
							top = app->map->MapToWorld(top.x + 1, top.y);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				
				distance = position.x - top.x;
			}

			if (movingright) 
			{
				position.x += MIN(speed.x, distance);
			}
			else 
			{
				position.x += MAX(speed.x, -distance);
			}
			if (distance == 0)
				speed.x = 0;
		}
	}

	//Repetir el process pel moviment en l'altre coordenada
	if (speed.y != 0) 
	{
		if (position.x % app->map->data.tileWidth != 0) 
		{ //check 2 columns of tiles to find obstacles
			iPoint left, right;
			float distance;
			bool movingup = speed.y < 0;
			if (movingup) 
			{
				left = app->map->WorldToMap(position.x, position.y);
				right = app->map->WorldToMap(position.x + width, position.y);
			}
			else 
			{
				left = app->map->WorldToMap(position.x, position.y + height);
				right = app->map->WorldToMap(position.x + width, position.y + height);
			}

			uint indexleft = left.y * app->map->data.width + left.x;
			uint indexright = right.y * app->map->data.width + right.x;

			if (movingup) 
			{ //estem anant cap amunt

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				int lastcheckindex = indexleft % app->map->data.width;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (int i = indexleft; i >= lastcheckindex; i = i - app->map->data.width) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							left.x = i % app->map->data.width;
							left.y = i / app->map->data.width;
							left = app->map->MapToWorld(left.x, left.y + 1);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				item = app->map->data.layers.start;
				found = false;
				lastcheckindex = indexright % app->map->data.width;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (int i = indexright; i >= lastcheckindex; i -= app->map->data.width) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							right.x = i % app->map->data.width;
							right.y = i / app->map->data.width;
							right = app->map->MapToWorld(right.x, right.y + 1);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				float leftdistance = position.y - left.y;
				float rightdistance = position.y - right.y;
				distance = MIN(leftdistance, rightdistance);
			}
			else 
			{//movingdown

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				uint lastcheckindex = indexleft % app->map->data.width + (app->map->data.height -1) * app->map->data.width;
				for (item; item; item = item->next) {//iterar les layers
					for (uint i = indexleft; i <= lastcheckindex; i += app->map->data.width) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							left.x = i % app->map->data.width;
							left.y = i / app->map->data.width;
							left = app->map->MapToWorld(left.x, left.y - 1);
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				item = app->map->data.layers.start;
				found = false;
				lastcheckindex = indexright % app->map->data.width + (app->map->data.height - 1) * app->map->data.width;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indexright; i <= lastcheckindex; i += app->map->data.width) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) {
							//get the position of the tile
							right.x = i % app->map->data.width;
							right.y = i / app->map->data.width;
							right = app->map->MapToWorld(right.x, right.y -1);
							found = true;
							break;
						}
					}
					if (found)
						break;
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
				left = app->map->WorldToMap(position.x, position.y);
			}
			else 
			{
				left = app->map->WorldToMap(position.x, position.y + height);
			}

			uint indexleft = left.y * app->map->data.width + left.x;

			if (movingup) 
			{ //estem anant cap a la dreta

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				int lastcheckindex = indexleft % app->map->data.width;
				for (item; item; item = item->next) {//iterar les layers
					for (int i = indexleft; i >= lastcheckindex; i -= app->map->data.width) {//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) {
							//get the position of the tile
							left.x = i % app->map->data.width;
							left.y = i / app->map->data.width;
							left = app->map->MapToWorld(left.x, left.y + 1); //PROBLEM: WE DONT KNOW WHY THIS +1 OFFSET
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
				distance = position.y - left.y;
			}
			else 
			{//moving down

				ListItem<MapLayer*>* item = app->map->data.layers.start;
				bool found = false;
				uint lastcheckindex = indexleft % app->map->data.width + (app->map->data.height - 1) * app->map->data.width;
				for (item; item; item = item->next) 
				{//iterar les layers
					for (uint i = indexleft; i <= lastcheckindex; i += app->map->data.width) 
					{//iterar les tilesets
						if (app->map->GetTileProperty(item->data->data[i], "Blocked")) 
						{
							//get the position of the tile
							left.x = i % app->map->data.width;
							left.y = i / app->map->data.width;
							left = app->map->MapToWorld(left.x, left.y-1); //PROBLEM: WE DONT KNOW WHY THIS -1 OFFSET 
							found = true;
							break;
						}
					}
					if (found)
						break;
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