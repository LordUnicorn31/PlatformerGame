#pragma once

#include "Module.h"
#include "list.h"
#include "Point.h"
#include "App.h"
#include "SDL.h"


struct SDL_Texture;
struct _TTF_Font;

#define CURSOR_WIDTH 2

enum class AviableEntities : unsigned char;
enum class EntityType : unsigned char;
class Entity;

//TODO: Namings!!!!
enum class UiTypes 
{
	UNKNOWN,
	IMAGE,
	TEXT,
	BUTTON,
	HUDBAR,
	SLIDER,
	CHECKBOX
};

class UiElement 
{
public:
	UiElement(int x, int y, int w, int h, bool interactuable, bool draggeable, bool useCamera, UiTypes uitype, UiElement* parent, Module* elementModule);
	virtual ~UiElement();
	virtual void Update(int dx, int dy) = 0;
	virtual void Draw(SDL_Texture* atlas) = 0;
	const SDL_Rect GetScreenRect();
	const SDL_Rect GetLocalRect();
	const iPoint GetLocalPos();
	const iPoint GetScreenPos();
	void SetLocalPos(int x, int y);
	bool OutOfParent();
	bool draggable;
	bool interactuable;
	UiTypes type;
	UiElement* parent;
	Module* module;
	bool useCamera;
	//bool debug;
private:
	SDL_Rect uiRect;
};

// ---------------------------------------------------
class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	void DeleteAllUiElements();
	void RemoveUiElement(UiElement* element);
	void UpdateUi();
	void DrawUi();
	//If the ui has a parent the x,y will be the local coordenates respect the parent
	UiElement* AddImage(int x, int y, SDL_Rect sourceRect, Module* elementModule = nullptr, UiElement* parent = nullptr, bool useCamera = false, bool interactuable = true, bool draggeable = false);
	//If the ui has a parent the x,y will be the local coordenates respect the parent
	UiElement* AddText(int x, int y, const char* text, _TTF_Font* font = nullptr, UiElement* parent = nullptr, SDL_Color color = { 255, 255, 255, 255 }, int size = 12, bool useCamera = false, bool interactuable = false, bool draggeable = false, Module* elementModule = nullptr);
	//If the ui has a parent the x,y will be the local coordenates respect the parent
	UiElement* AddButton(int x, int y, SDL_Rect sourceUnhover, SDL_Rect sourceHover, SDL_Rect sourceClick, Module* elementModule = nullptr, UiElement* parent = nullptr, bool useCamera = false, bool interactuable = true, bool draggeable = false);
	UiElement* AddHUDBar(int x, int y, int maxValue, float* valuePtr, SDL_Rect bar, SDL_Rect fill, SDL_Rect border, bool interactuable, bool draggeable, bool useCamera, UiElement* parent, Module* elementModule);
	UiElement* AddSlider(int x, int y, int value, int maxValue, bool interactuable, bool draggeable, bool usecamera, UiElement* parent, Module* elementModule);
	UiElement* AddCheckBox(int x, int y, SDL_Rect uncheck, SDL_Rect check, Module* elementModule = nullptr, UiElement* parent = nullptr, bool useCamera = false, bool interactuable = true, bool draggable = false);
	void DraggUiElements(UiElement* parent, int dx, int dy);
	UiElement* UiUnderMouse();
	bool MouseClick();
	UiElement* FocusNextElement(UiElement* current_element);
	UiElement* focusedUi;
	const SDL_Texture* GetAtlas() const;
	//void RemoveUiChilds(UiElement* element);

private:
	List<UiElement*> uiElementList;
	SDL_Texture* atlas;
	SString atlasPath;
};

class UiImage :public UiElement 
{
public:
	UiImage(int x, int y, SDL_Rect sourceRect, bool interactuable, bool draggeable, bool useCamera, UiElement* parent, Module* elementModule);
	~UiImage();
	void Draw(SDL_Texture* atlas)override;
	void Update(int dx, int dy)override;
	SDL_Rect atlasRect;
};

enum class Button_state 
{
	UNHOVERED,
	HOVERED,
	CLICKED
};

enum class CheckBoxState
{
	UNCHECKED,
	CHECKED,
	HOVERED
};

class UiButton :public UiElement 
{
public:
	UiButton(int x, int y, SDL_Rect sourceUnhover, SDL_Rect sourceHover, SDL_Rect sourceClick, bool interactuable, bool draggeable, bool useCamera, UiElement* parent, Module* elementModule);
	~UiButton();
	void Draw(SDL_Texture* atlas)override;
	void Update(int dx, int dy)override;
	SDL_Rect unhover;
	SDL_Rect hover;
	SDL_Rect click;
	Button_state currentState;
};

class UiCheckBox :public UiElement
{
public:
	UiCheckBox(int x, int y, SDL_Rect uncheck, SDL_Rect check, bool interactuable, bool draggable, bool useCamera, UiElement* parent, Module* elementModule);
	~UiCheckBox();
	void Draw(SDL_Texture* atlas)override;
	void Update(int dx, int dy)override;
	SDL_Rect checked;
	SDL_Rect unchecked;
	CheckBoxState currentState;
};

class UiText :public UiElement 
{
public:
	UiText(int x, int y, const char* text, int size, SDL_Color color, bool interactuable, bool draggeable, bool useCamera, _TTF_Font* font = nullptr, UiElement* parent = nullptr, Module* elementModule = nullptr);
	~UiText();
	void Draw(SDL_Texture* atlas)override;
	void Update(int dx, int dy)override;
	_TTF_Font* fontType;
	SString message;
	SDL_Color color;
	SDL_Texture* texture;
	void ChangeColor(SDL_Color newcolor);
	void ChangeMessage(const char* newmessage);
};

class UiHUDBars : public UiElement 
{
public:
	UiHUDBars(int x, int y, uint MaxValue, float* valueptr, bool useCamera, SDL_Rect bar, SDL_Rect fill, SDL_Rect border, bool interactuable, bool draggeable, UiElement* parent, Module* elementmodule);
	~UiHUDBars();
	void Draw(SDL_Texture* atlas)override;
	void Update(int dx, int dy)override;
	SDL_Rect border;
	SDL_Rect fill;
	SDL_Rect fullBar;
	const float* value;
	int maxValue;
	SDL_Rect currentBar;
	bool useCamera;
};

class UiSlider : public UiElement 
{
public:
	UiSlider(int x, int y, int InitialValue, int maxvalue, bool interactuable, bool draggeable, bool usecamera, UiElement* parent, Module* elementmodule);
	~UiSlider();
	void Draw(SDL_Texture* atlas)override;
	void Update(int dx, int dy)override;
	SDL_Rect bar;
	SDL_Rect unhovered;
	SDL_Rect hovered;
	SDL_Rect clicked;
	Button_state currentState;
	iPoint barPos;
	int value;
	int maxValue;
};
