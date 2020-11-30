#pragma once
#include "Point.h"
#include "Module.h"

class SDL_Rect;
class SDL_Texture;
class SString;

class Player : public Module{ //public Entity ??
public:
	Player();
	~Player();
	bool Awake(pugi::xml_node&playerNode);
	void Init();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Die();
	iPoint GetPosition() const;
	/*
	fPoint targetSpeed; //Si volem posar aceleracio
	float a; //Si volem posar acceleracio
	//float a;
	//flaot maxspeed
	//float
	//Collider

	//Jump
	//Pujar ladder
	*/
private:
	void Draw();
	void Move();
	bool OnPlatform();
	bool OnDeath();
	bool Onchange();
	bool OnBlockedTile();
	bool OnLadder(iPoint position);
	bool SnapToLadder(bool onPlatform, bool down);
	bool OnSave();
	void MoveLadder();
	bool Load(pugi::xml_node& playerNode);
	bool Save(pugi::xml_node& playerNode) const;
	

	SDL_Rect textureRect;
	SDL_Texture* texture;
	iPoint position; //unsigned int?
	uint width;
	uint height;
	fPoint speed;
	float terminalSpeed;
	float a;
	fPoint targetSpeed;
	float maxSpeed;
	float jumpSpeed;
	bool onLadder;
	float ladderSpeed;
	SString texturePath;
	float acumulatedMs;
	float updateMsCycle;
	bool doLogic;
	uint jumpSound;
	iPoint initialPos;
	uint checkpointSound;
	
};