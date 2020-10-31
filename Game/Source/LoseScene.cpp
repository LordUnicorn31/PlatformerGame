#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Input.h"
#include "Map.h"
#include "Transitions.h"
#include "Audio.h"
#include "LoseScene.h"

LoseScene::LoseScene() : Module()
{
	name = "loseScene";
}

// Destructor
LoseScene::~LoseScene()
{
}

// Called before render is available
bool LoseScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool LoseScene::Start()
{
	exitGame = false;

	background = app->tex->Load("Assets/textures/logoBackground.png");
	loseText = app->tex->Load("Assets/textures/logo.png");
	/*app->audio->PlayMusic("Resources/audio/music/epic_fall.ogg");*/

	return true;
}

// Called each loop iteration
bool LoseScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LoseScene::Update(float dt)
{
	bool ret = true;

	

	app->render->DrawTexture(background, 0, 0, NULL, true, 1.5);
	app->render->DrawTexture(loseText, app->win->width * 1.55f, app->win->height, NULL, true, 0.2);

	if (exitGame) {
		ret = false;
		exitGame = false;
	}

	return ret;
}

// Called each loop iteration
bool LoseScene::PostUpdate()
{
	
	bool ret = true;

	return ret;
}


// Called before quitting
bool LoseScene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(background);
	app->tex->UnLoad(loseText);
	/*app->audio->UnloadMusic();*/


	return true;
}

void LoseScene::Init()
{
	enabled = false;

	active = true;
}

