#pragma once
#include "Point.h"
#include "Module.h"
#include "Entity.h"
#include "Animation.h"
#include "SDL2_ttf-2.0.15/SDL_ttf.h"

class SDL_Rect;
class SDL_Texture;
class SString;
struct SDL_Color;
class Text;


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
	bool OnDeath();
	void Lives(Module* mod);
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
	void Draw(float dt);
	void Move();
	bool OnPlatform();
	
	bool Onchange();
	bool OnBlockedTile();
	bool OnLadder(iPoint position);
	bool SnapToLadder(bool onPlatform, bool down);
	bool OnSave();
	bool OnEnd();
	void MoveLadder();
	bool Load(pugi::xml_node& playerNode);
	bool Save(pugi::xml_node& playerNode) const;
	void CoinMovement();
	void HeartMovement();
	void HeartCounterMovement();
	
	

	SDL_Rect textureRect;
	SDL_Rect heartRect;
	SDL_Texture* texture;
	SDL_Texture* coinTexture;
	SDL_Texture* heartTexture;
	iPoint position; //unsigned int?
	iPoint coinPos;
	iPoint heartPos;
	iPoint heartCounterPos;
	int lives;
	int fullLives;
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
	SString coinTextPath;
	float acumulatedMs;
	float updateMsCycle;
	bool doLogic;
	uint jumpSound;
	iPoint initialPos;
	uint checkpointSound;
	bool flip;
	int checkpoint1x;
	int checkpoint1y;
	int checkpoint2x;
	int checkpoint2y;
	int checkpoint3x;
	int checkpoint3y;
	
	
	

	Animations idleAnimation;
	Animations runAnimation;
	Animations jumpAnimation;
	Animations climbAnimation;
	Animations deathAnimation;
	Animations attackAnimation;
	Animations* currentAnimation = nullptr;


	
	SDL_Color white;
	SDL_Texture* heartCounterTex;
	char numLives;

	Text* text;
	SDL_Texture* moveTut;

	
};