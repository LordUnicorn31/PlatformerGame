#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "Input.h"
#include "Map.h"
#include "SceneTitle.h"

#include "Transitions.h"
#include "Audio.h"
#include "Render.h"

SceneLogo::SceneLogo() : Module()
{
	name = "sceneLogo";
}

// Destructor
SceneLogo::~SceneLogo()
{
}


// Called before render is available
bool SceneLogo::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	rect = { 0,0,app->win->width, app->win->height };

	r = 0;

	logoImage = app->tex->Load("Assets/textures/logoImage.png");

	return true;
}

// Called each loop iteration
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{

	bool ret = true;

	app->render->DrawRectangle(rect, 150, 0, 0, (Uint8)r);

	
	app->render->DrawTexture(logoImage, 0, 0, NULL);


	/*if (app->input->GetKey(SDL_SCANCODE_SPACE))
	{
		app->transitions->FadeToBlack(app->sceneLogo, app->sceneTitle, 2.0f);
	}

	r++;

	if (r == 255)
	{
		r--;
		app->transitions->FadeToBlack(app->sceneLogo, app->sceneTitle, 2.0f);
	}*/

	
	return ret;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	return ret;
}


// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(logoImage);
	logoImage = nullptr;
	return true;
}

void SceneLogo::Init()
{
	enabled = true;

	active = true;
}
