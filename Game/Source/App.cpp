#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Player.h"
#include "Transitions.h"
#include "SceneLose.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "CastleScene.h"


#include "Defs.h"
#include "Log.h"
#include "Timer.h"
#include "SString.h"

#include <iostream>
#include <sstream>

#ifdef OPTICKPROFILE
#include "optick.h"
#endif // OPTICKPROFILE

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args), saveDocumentName("savegame.xml")
{
	PERF_START(timer);
	wantToSave = wantToLoad = false;
	freeze = false;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	player = new Player();
	transitions = new Transitions();
	loseScene = new LoseScene();
	sceneLogo = new SceneLogo();
	sceneTitle = new SceneTitle();
	castleScene = new CastleScene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(scene);
	AddModule(castleScene);
	AddModule(player);
	AddModule(transitions);
	AddModule(loseScene);
	AddModule(sceneLogo);
	AddModule(sceneTitle);
	AddModule(audio);
	

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(timer);
}

// Destructor
App::~App()
{
	// release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();

	configFile.reset();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(timer);
	// TODO 3: Load config.xml file using load_file() method from the xml_document class.
	bool ret = LoadConfig();

	// TODO 4: Read the title from the config file
	pugi::xml_node configApp;
	configApp = config.child("app");
	title.create(configApp.child("title").child_value());
	cap = configApp.attribute("cap").as_int(-1);
	capNum = cap;
	if (cap > 0)
	{
		cappedMs = 1000 / cap;
	}
	win->SetTitle(title.GetString());

	if(ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// TODO 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(timer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(timer);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if(item->data->isEnabled())
			ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(timer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
#ifdef OPTICKPROFILE
	OPTICK_EVENT();
#endif // OPTICKPROFILE
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// TODO 3: Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	pugi::xml_parse_result result = configFile.load_file("config.xml");

	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	dt = frameTime.ReadSec();
	if (freeze)
		dt = 0;
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
#ifdef OPTICKPROFILE
	OPTICK_EVENT();
#endif // OPTICKPROFILE
	// This is a good place to call Load / Save functions
	if (wantToSave == true)
		SaveGameNow();

	if (wantToLoad == true)
		LoadGameNow();

	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	avgFps = float(frameCount) / startupTime.ReadSec();
	secondstartUp = startupTime.ReadSec();
	lastFrame = frameTime.Read();
	frameLast = prevLastSecFrameCount;

	sprintf_s(title1, 256, "FPS: %i Avg.FPS: %.2f Last Frame Ms: %u VSync: %d",
		frameLast, avgFps, lastFrame, app->render->vSync);
	app->win->SetTitle(title1);

	if (cappedMs > 0 && lastFrame < cappedMs)
	{
		SDL_Delay(cappedMs - lastFrame);
	}

}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false || !pModule->isEnabled()) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
#ifdef OPTICKPROFILE
	OPTICK_EVENT();
#endif // OPTICKPROFILE

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false || !pModule->isEnabled()) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
#ifdef OPTICKPROFILE
	OPTICK_EVENT();
#endif // OPTICKPROFILE

	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false || !pModule->isEnabled()) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

bool App::LoadGameNow()
{
	bool ret = true;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(saveDocumentName.GetString());

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", saveDocumentName.GetString());

		root = data.child("game_state");

		ListItem<Module*>* it = modules.start;
		for (it; it != NULL && ret == true; it = it->next) {
			pugi::xml_node ModuleNode = root.child( it->data->name.GetString());
			ret = it->data->Load(ModuleNode);
		}

		data.reset();
		if (ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (it != NULL) ? it->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", saveDocumentName.GetString(), result.description());

	wantToLoad = false;
	return ret;
}

bool App::SaveGameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", saveDocumentName.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	ListItem<Module*>* it = modules.start;
	for (it; it != NULL && ret == true; it = it->next) {
		pugi::xml_node ModuleNode = root.append_child(it->data->name.GetString());
		ret = it->data->Save(ModuleNode);
	}

	if (ret == true)
	{
		data.save_file(saveDocumentName.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (it != NULL) ? it->data->name.GetString() : "unknown");

	data.reset();
	wantToSave = false;
	return ret;
}

void App::LoadGame()
{
	wantToLoad = true;
}

void App::SaveGame() const
{
	wantToSave = true;
}

/*
void App::GetSaveGames(List<SString>& list_to_fill) const
{
}
*/

// Called before quitting
bool App::CleanUp()
{
	PERF_START(timer);
	//PREGUNTA: PK ENS FAN FER EL CLEAN UP DELS MODULS AL REVES??? DONA EL ASSERT AL DESTRUIR EL RENDERER ABANS D'AVERR DESTRUIT LES TEXTURES!!?? PK NO SIMPLEMENT ITEREM LA LLISTA DE MODULS EN ORDRE?
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->next;
	}

	PERF_PEEK(timer);

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}


