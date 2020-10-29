#pragma once
#include "Point.h"

class Player { //public Entity ??
	Player();
	~Player();
	void Draw();
	void Move();
	void Update();
	void UpdateLogic();

	iPoint position; //unsigned int?
	uint width;
	uint height;
	fPoint speed;
	fPoint targetSpeed; //Si volem posar aceleracio
	float a; //Si volem posar acceleracio
	//float a;
	//flaot maxspeed
	//float
	//Collider

	//Jump
	//Pujar ladder
};