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
struct Collider;


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
	int GetLives();
	int GetCoins();
	bool ImDead() const;
	bool Finished() const;
	bool GotCoin() const;

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
	void SetPlayerCollider();
	void OnCollision(Collider* c1, Collider* c2) override;

	SDL_Rect textureRect;
	SDL_Texture* texture;
	iPoint position;
	int lives;
	int fullLives;
	int coins;
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
	iPoint checkPoint1;
	iPoint checkPoint2;
	iPoint checkPoint3;

	bool checkPoint = true;
	
	
	Animations idleAnimation;
	Animations runAnimation;
	Animations jumpAnimation;
	Animations climbAnimation;
	Animations deathAnimation;
	Animations attackAnimation;
	Animations* currentAnimation = nullptr;

	bool onDeath = false;
	bool onEnd = false;
	bool gotCoin = false;
	Collider* playerCollider;
};