#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneTitle.h"
#include "Input.h"
#include "Map.h"
#include "Transitions.h"
#include "Audio.h"
#include "SceneLogo.h"

SceneTitle::SceneTitle() : Module()
{
	name = "sceneTitle";
}

// Destructor
SceneTitle::~SceneTitle()
{
}


// Called before render is available
bool SceneTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool SceneTitle::Start()
{
	exitGame = false;
	titleImage = app->tex->Load("Assets/textures/titleImage.png");
    /*app->audio->PlayMusic("Assets/audio/music/Menu.ogg");*/
	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	bool ret = true;

	app->render->DrawTexture(titleImage, 0, 0, NULL);

	if (exitGame) {
		ret = false;
		exitGame = false;
	}

	return ret;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	
	bool ret = true;

	return ret;
}


// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(titleImage);
	titleImage = nullptr;


	return true;
}

void SceneTitle::Init()
{
	enabled = false;

	active = true;
}

