#ifndef __CASTLESCENE_H__
#define __CASTLESCENE_H__

#include "Module.h"

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

	// Camera movement
	void CameraMovement();

private:
	SString mapName;
	SString audioPath;
};

#endif // __CASTLESCENE_H__