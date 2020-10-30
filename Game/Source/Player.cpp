#include "Render.h"
#include "Textures.h"
#include "App.h"
#include "Map.h"
#include "List.h"
#include "Player.h"

Player::Player() : Module() {
	name.create("player");
	speed = { 0,0 };
	width = 16;
	height =16;
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

bool Player::Update(float dt) {
	//Get the input and update the movement variables accordingly
	Move();
	//Get the tiles under the player and check for some possible interactions (Ns si aixo s'auria de posar en el update logic)
	Draw();
	return true;
}

void Player::UpdateLogic() {
	Move();
}

void Player::Draw() {
	//Diferent tile drawings depending on the player action or state
	//Maybe some tiles will need to get fliped depending on the orientation
	app->render->DrawTexture(texture, position.x, position.y, &textureRect);
}

void Player::Move() {
	speed.x = -1;
	speed.y = 1;
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
			{//estem anant cap a l'esquerra

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
		}
	}
}