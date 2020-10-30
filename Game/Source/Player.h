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
	bool Start();
	void Draw();
	void Move();
	bool Update(float dt);
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
	bool OnPlatform();

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
	float threshold;
	float jumpSpeed;

	SString texturePath;
	float acumulatedMs;
	float updateMsCycle;
	bool doLogic;
};