#ifndef __CASTLESCENE_H__
#define __CASTLESCENE_H__

#include "Module.h"
#include "FlyEnemy.h"
#include "PatrolEnemy.h"
#include "WanderEnemy.h"

struct SDL_Texture;

class CastleScene : public Module
{
public:

	CastleScene();

	// Destructor
	virtual ~CastleScene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	void Init();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SString mapName;
	SString audioPath;
	SString mapPath;
	bool onDeath;
	iPoint mapBat;
	iPoint mapZombie;
	iPoint mapSkeleton;
	PatrolEnemy* skeleton = nullptr;
	FlyEnemy* bat = nullptr;
	WanderEnemy* zombie = nullptr;
};

#endif // __CASTLESCENE_H__
