#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "Timer.h" //Vull fowarddeclarar aixo amb class Timer pero no em deixa
#include "PerfTimer.h" //Same :(

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Player;
class Transitions;
class LoseScene;
class SceneLogo;
class SceneTitle;
class CastleScene;
class EntityManager;
class Gui;
class Fonts;

class SString;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	//void GetSaveGames(List<SString> &list_to_fill) const;

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SaveGameNow() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Player* player;
	Transitions* transitions;
	LoseScene* loseScene;
	SceneLogo* sceneLogo;
	SceneTitle* sceneTitle;
	CastleScene* castleScene;
	EntityManager* entity;
	Gui* gui;
	Fonts* font;

	bool freeze;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	mutable bool wantToSave;
	bool wantToLoad;
	SString	saveDocumentName;

	// TODO 2: Create two new variables from pugui namespace:
	// a xml_document to store the config file and
	// two xml_node to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	Timer frameTime;
	PerfTimer timer;
	float avgFps;
	float secondStartUp;
	uint32 lastFrame;
	uint32 frameLast;
	char title1[256];
	float dt;
	uint64 frameCount = 0;
	Timer startUpTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	int cappedMs = -1;
	bool capNum;
	int cap;
};

extern App* app;

#endif	// __APP_H__