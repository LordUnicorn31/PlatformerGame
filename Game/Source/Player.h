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
	fPoint speed;
	fPoint targetSpeed;
	float a;
	//float a;
	//flaot maxspeed
	//float
	//Collider

	//Jump
	//Pujar ladder
};