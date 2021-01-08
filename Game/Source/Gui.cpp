#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "Input.h"
#include "Gui.h"
#include "Entity.h"
#include "Module.h"

Gui::Gui() : Module()
{
	name = "gui";
}

// Destructor
Gui::~Gui()
{
}

// Called before render is available
bool Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlasPath = conf.child("atlas").attribute("atlasPath").as_string("");

	return ret;
}

// Called before the first frame
bool Gui::Start()
{
	atlas = app->tex->Load(atlasPath.GetString());
	focusedUi = nullptr;
	return true;
}

// Update all guis
bool Gui::PreUpdate()
{
	return true;
}

bool Gui::Update(float dt) {
	if (MouseClick() && UiUnderMouse() != nullptr && UiUnderMouse()->interactuable) 
	{
		focusedUi = UiUnderMouse();
		if (focusedUi->module != nullptr) 
		{
			focusedUi->module->UiCallback(focusedUi);
		}
	}
	else if (MouseClick() && UiUnderMouse() == nullptr)
		focusedUi = nullptr;
	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		focusedUi = FocusNextElement(focusedUi);
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		if (focusedUi != nullptr && focusedUi->module != nullptr && focusedUi->type != UiTypes::EButton) 
		{
			focusedUi->module->UiCallback(focusedUi);
		}
	UpdateUi();
	DrawUi();
	return true;
}

// Called after all Updates
bool Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool Gui::CleanUp()
{
	LOG("Freeing GUI");
	app->tex->UnLoad(atlas);
	DeleteAllUiElements();
	return true;
}

// const getter for atlas
const SDL_Texture* Gui::GetAtlas() const
{
	return atlas;
}

void Gui::DeleteAllUiElements() {

	uiElementList.clear();
}

void Gui::RemoveUiElement(UiElement* element) {

	int index = uiElementList.find(element);
	if (index != -1) 
	{
		for (int i = 0; i < uiElementList.count(); ++i) 
		{
			if (uiElementList[i]->parent == element) 
				RemoveUiElement(uiElementList[i]);
		}
		uiElementList.del(uiElementList.At(index));
	}
	else
		LOG("UiElement to delete not found");
}

/*void Gui::RemoveUiChilds(UiElement* element)
{
	eastl::list<UiElement*>::iterator it;
	for (it = uiElementList.begin(); it != uiElementList.end();) {
		if ((*it)->parent == element) {
			app->gui->RemoveUiChilds((*it));
			delete (*it);
			it = uiElementList.erase(it);
		}
		else
			it++;
	}

	ListItem<UiElement*>* it = uiElementList.start;
	for (it; it != nullptr;) 
	{
		if (it->data->parent == element) {
			RemoveUiChilds(it->data);
			it = uiElementList.erase(it);
			it = uiElementList.del(it);
		}
		else
			it = it->next;
	}
}*/

void Gui::UpdateUi() 
{
	int dx, dy;
	app->input->GetMouseMotion(dx, dy);
	for (int i = 0; i < uiElementList.count(); ++i) 
	{
		uiElementList[i]->Update(dx, dy);
	}
}

void Gui::DrawUi() 
{

	for (int i = 0; i < uiElementList.count(); ++i)
	{
		uiElementList[i]->Draw(atlas);
	}
}

UiElement* Gui::UiUnderMouse() 
{

	int x, y;
	app->input->GetMousePosition(x, y);
	/*if (app->scene->IsEneabled()) 
	{
		x -= app->render->camera.x;
		y -= app->render->camera.y;
	}*/
	UiElement* Element = nullptr;
	for (int i = 0; i < uiElementList.count(); ++i)
	{
		if (uiElementList[i]->GetScreenPos().x < x && x < uiElementList[i]->GetScreenPos().x + uiElementList[i]->GetScreenRect().w && uiElementList[i]->GetScreenPos().y < y && y < uiElementList[i]->GetScreenPos().y + uiElementList[i]->GetScreenRect().h && (uiElementList[i]->interactuable || uiElementList[i]->draggable))
			Element = uiElementList[i];
	}
	return Element;
}

bool Gui::MouseClick() 
{
	return (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP);
}

void Gui::DraggUiElements(UiElement* parent, int dx, int dy) {

	//crec k falta el dragg del propi primer element
	for (int i = 0; i < uiElementList.count(); ++i)
	{
		if (uiElementList[i]->parent == parent) {
			uiElementList[i]->SetLocalPos(uiElementList[i]->GetLocalPos().x + dx, uiElementList[i]->GetLocalPos().y + dy);
			DraggUiElements(uiElementList[i], dx, dy);
		}
	}
}

UiElement* Gui::FocusNextElement(UiElement* current_element) {
	if (current_element == nullptr) 
	{
		for (int i = 0; i < uiElementList.count(); ++i) 
		{
			if (uiElementList[i]->interactuable)
				return uiElementList[i];
		}
	}
	else 
	{
		bool iteration = true;
		for (int i = uiElementList.find(current_element) + 1; i < uiElementList.count(); ++i) 
		{
			if (uiElementList[i]->interactuable)
				return uiElementList[i];
			if (i == uiElementList.count() - 1 && iteration) 
			{
				iteration = false;
				i = 0;
				if (uiElementList[i]->interactuable)
					return uiElementList[i];
			}
		}
	}
	return nullptr;
}

UiElement* Gui::AddImage(int x, int y, SDL_Rect source_rect, Module* elementmodule, UiElement* parent, bool useCamera, bool interactuable, bool draggeable) {
	UiElement* Image = new UiImage(x, y, source_rect, interactuable, draggeable, useCamera, parent, elementmodule);
	uiElementList.add(Image);
	return Image;
}

UiElement* Gui::AddText(int x, int y, const char* text, _TTF_Font* font, UiElement* parent, SDL_Color color, int size, bool useCamera, bool interactuable, bool draggeable, Module* elementmodule) {
	UiElement* Text = new UiText(x, y, text, size, color, interactuable, draggeable, useCamera, font, parent, elementmodule);
	uiElementList.add(Text);
	return Text;
}

UiElement* Gui::AddButton(int x, int y, SDL_Rect source_unhover, SDL_Rect source_hover, SDL_Rect source_click, Module* elementmodule, UiElement* parent, bool useCamera, bool interactuable, bool draggeable) {
	UiElement* Button = new UiButton(x, y, source_unhover, source_hover, source_click, interactuable, draggeable, useCamera, parent, elementmodule);
	uiElementList.add(Button);
	return Button;
}

UiElement* Gui::AddCheckBox(int x, int y, SDL_Rect uncheck, SDL_Rect check, Module* elementModule, UiElement* parent, bool useCamera, bool interactuable, bool draggable)
{
	UiElement* CheckBox = new UiCheckBox(x, y, uncheck, check, interactuable, draggable, useCamera, parent, elementModule);
	uiElementList.add(CheckBox);
	return CheckBox;
}

UiElement* Gui::AddHUDBar(int x, int y, int MaxValue, float* valueptr, SDL_Rect bar, SDL_Rect fill, SDL_Rect border, bool interactuable, bool draggeable, bool usecamera, UiElement* parent, Module* elementmodule) {
	UiElement* HUD = new UiHUDBars(x, y, MaxValue, valueptr, usecamera, bar, fill, border, interactuable, draggeable, parent, elementmodule);
	uiElementList.add(HUD);
	return HUD;
}

UiElement* Gui::AddSlider(int x, int y, int value, int maxvalue, bool interactuable, bool draggeable, bool usecamera, UiElement* parent, Module* elementmodule) {
	if (value > maxvalue)
		value = maxvalue;
	else if (value < 0)
		value = 0;
	UiElement* Slider = new UiSlider(x, y, value, maxvalue, interactuable, draggeable, usecamera, parent, elementmodule);
	uiElementList.add(Slider);
	return Slider;
}

/*UiElement* Gui::AddSlider(int x, int y, bool active, bool draggable,bool useCamera, UiElement* parent, Module* elementmodule, int sliderposition) {
	UiElement* Slider = new UiSlider(x, y, active, draggable, parent, elementmodule);
	uiElementList.push_back(Slider);
	return Slider;
}*/


UiElement::UiElement(int x, int y, int w, int h, bool interactuable, bool draggeable, bool useCamera, UiTypes uitype, UiElement* parent, Module* elementmodule) : type(uitype), parent(parent), module(elementmodule), uiRect({ x,y,w,h }), interactuable(interactuable), draggable(draggeable), useCamera(useCamera) { if (parent != nullptr)SetLocalPos(x, y); }

UiElement::~UiElement() {};

const iPoint UiElement::GetScreenPos() {
	iPoint position(uiRect.x, uiRect.y);
	return position;
}

const iPoint UiElement::GetLocalPos() {
	if (parent == nullptr) {
		iPoint position(uiRect.x, uiRect.y);
		return position;
	}
	else {
		iPoint position(uiRect.x - parent->GetScreenPos().x, uiRect.y - parent->GetScreenPos().y);
		return position;
	}
}

const SDL_Rect UiElement::GetScreenRect() {
	return uiRect;
}

const SDL_Rect UiElement::GetLocalRect() {
	return { GetLocalPos().x,GetLocalPos().y,uiRect.w,uiRect.h };
}

void UiElement::SetLocalPos(int x, int y) {
	if (parent == nullptr) {
		uiRect.x = x; uiRect.y = y;
	}
	else {
		uiRect.x = parent->GetScreenPos().x + x;
		uiRect.y = parent->GetScreenPos().y + y;
	}
}

bool UiElement::outofparent() {
	return (GetScreenPos().x + GetScreenRect().w * 0.5f < parent->GetScreenPos().x || GetScreenPos().x > parent->GetScreenPos().x + parent->GetScreenRect().w - GetScreenRect().w * 0.5f || GetScreenPos().y + GetScreenRect().h * 0.5f < parent->GetScreenPos().y || GetScreenPos().y > parent->GetScreenPos().y + parent->GetScreenRect().h - GetScreenRect().h * 0.5f);
}



UiImage::UiImage(int x, int y, SDL_Rect source_rect, bool interactuable, bool draggeable, bool useCamera, UiElement* parent, Module* elementmodule) :UiElement(x, y, source_rect.w, source_rect.h, interactuable, draggeable, useCamera, UiTypes::Image, parent, elementmodule), atlasRect(source_rect) {}
UiImage::~UiImage() {}

void UiImage::Update(int dx, int dy) {
	//fer que la imatge es mogui amb la camera
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && app->gui->focusedUi == this)
		app->gui->focusedUi = nullptr;
	if (app->gui->MouseClick() && app->gui->focusedUi == this)
		app->gui->focusedUi = nullptr;
	if (draggable && app->gui->MouseClick() && app->gui->UiUnderMouse() == this && dx != 0 && dy != 0) {
		SetLocalPos(GetLocalPos().x + dx, GetLocalPos().y + dy);
		app->gui->DraggUiElements(this, dx, dy);
	}
}

void UiImage::Draw(SDL_Texture* atlas) {
	if (parent == nullptr || !outofparent())
		app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &atlasRect, 1.0f, SDL_FLIP_NONE, useCamera);
}

UiText::UiText(int x, int y, const char* text, int size, SDL_Color color, bool interactuable, bool draggeable, bool useCamera, _TTF_Font* font, UiElement* parent, Module* elementmodule) : UiElement(x, y, size, size, interactuable, draggeable, useCamera, UiTypes::Text, parent, elementmodule), fontType(font), message(text), color(color), texture(app->font->Print(message.GetString(), color, fontType)) {}
UiText::~UiText() { app->tex->UnLoad(texture); }

void UiText::Draw(SDL_Texture* atlas) {
	if (parent == nullptr || !outofparent())
		app->render->DrawTexture(texture, GetScreenPos().x, GetScreenPos().y, NULL, 1.0f, SDL_FLIP_NONE, useCamera);
}

void UiText::Update(int dx, int dy) {
	//fer que el text es mogui amb la camera
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && app->gui->focusedUi == this)
		app->gui->focusedUi = nullptr;
	if (app->gui->MouseClick() && app->gui->focusedUi == this)
		app->gui->focusedUi = nullptr;
	if (draggable && app->gui->MouseClick() && app->gui->UiUnderMouse() == this && dx != 0 && dy != 0) {
		SetLocalPos(GetLocalPos().x + dx, GetLocalPos().y + dy);
		app->gui->DraggUiElements(this, dx, dy);
	}
}

void UiText::ChangeMessage(const char* newmessage) {
	app->tex->UnLoad(texture);
	message = newmessage;
	texture = app->font->Print(message.GetString(), color, fontType);
}

void UiText::ChangeColor(SDL_Color newcolor) {
	app->tex->UnLoad(texture);
	color = newcolor;
	texture = app->font->Print(message.GetString(), color, fontType);
}

UiCheckBox::UiCheckBox(int x, int y, SDL_Rect uncheck, SDL_Rect check, bool interactuable, bool draggeable, bool useCamera, UiElement* parent, Module* elementModule) :UiElement(x, y, uncheck.w, uncheck.h, interactuable, draggeable, useCamera, UiTypes::CheckBox, parent, elementModule), unchecked(uncheck), checked(check), currentState(CheckBoxState::UNCHECKED) {}
UiCheckBox::~UiCheckBox() {}

void UiCheckBox::Update(int dx, int dy)
{

	if (app->gui->MouseClick() && app->gui->focusedUi == this)
	{
		if (currentState == CheckBoxState::CHECKED) 
			currentState = CheckBoxState::UNCHECKED;
		else if (currentState == CheckBoxState::UNCHECKED) 
			currentState = CheckBoxState::CHECKED;
	}

}

void UiCheckBox::Draw(SDL_Texture* atlas)
{
	if (parent == nullptr || !outofparent())
	{ 
		switch (currentState) 
		{
			case CheckBoxState::UNCHECKED:
			app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &unchecked, 1.0f, SDL_FLIP_NONE, useCamera);
			break;
			case CheckBoxState::CHECKED:
			app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &checked, 1.0f, SDL_FLIP_NONE, useCamera);
			break;
		}
	}
	
}


UiButton::UiButton(int x, int y, SDL_Rect source_unhover, SDL_Rect source_hover, SDL_Rect source_click, bool interactuable, bool draggeable, bool useCamera, UiElement* parent, Module* elementmodule) :UiElement(x, y, source_unhover.w, source_unhover.h, interactuable, draggeable, useCamera, UiTypes::Button, parent, elementmodule), unhover(source_unhover), hover(source_hover), click(source_click), currentState(Button_state::unhovered) {}
UiButton::~UiButton() {}

void UiButton::Update(int dx, int dy) {
	if (app->gui->focusedUi == this)
		currentState = Button_state::hovered;
	else if (app->gui->UiUnderMouse() == this)
		currentState = Button_state::hovered;
	else
		currentState = Button_state::unhovered;
	if (app->gui->MouseClick() && app->gui->focusedUi == this) {
		currentState = Button_state::clicked;
		//app->gui->focusedUi = nullptr;
	}
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && app->gui->focusedUi == this) {
		currentState = Button_state::clicked;
		//app->gui->focusedUi = nullptr;
	}
	if (draggable && app->gui->MouseClick() && app->gui->UiUnderMouse() == this && dx != 0 && dy != 0) {
		SetLocalPos(GetLocalPos().x + dx, GetLocalPos().y + dy);
		app->gui->DraggUiElements(this, dx, dy);
	}
}

void UiButton::Draw(SDL_Texture* atlas) {
	if (parent == nullptr || !outofparent()) {
		switch (currentState) {
		case Button_state::unhovered:
			app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &unhover, 1.0f, SDL_FLIP_NONE, useCamera);
			break;
		case Button_state::hovered:
			app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &hover, 1.0f, SDL_FLIP_NONE, useCamera);
			break;
		case Button_state::clicked:
			app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &click, 1.0f, SDL_FLIP_NONE, useCamera);
			break;
		}
	}
}

UiHUDBars::UiHUDBars(int x, int y, uint MaxValue, float* valueptr, bool usecamera, SDL_Rect bar, SDL_Rect fill, SDL_Rect border, bool interactuable, bool draggeable, UiElement* parent, Module* elementmodule) : UiElement(x, y, bar.w, bar.h, interactuable, draggeable, usecamera, UiTypes::HUDBar, parent, elementmodule), border(border), fill(fill), fullBar(bar), value(valueptr), maxValue(MaxValue), currentBar(bar) {}

UiHUDBars::~UiHUDBars() {}

void UiHUDBars::Update(int dx, int dy) {
	currentBar.w = (int)((*value) / ((float)maxValue / (float)fullBar.w));
	if (currentBar.w < 0)
		currentBar.w = 0;
}

void UiHUDBars::Draw(SDL_Texture* atlas) {
	app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &border, 1.0f, SDL_FLIP_NONE, useCamera);
	app->render->DrawTexture(atlas, GetScreenPos().x + 1, GetScreenPos().y + 1, &fill, 1.0f, SDL_FLIP_NONE, useCamera);
	app->render->DrawTexture(atlas, GetScreenPos().x + 1, GetScreenPos().y + 1, &currentBar, 1.0f, SDL_FLIP_NONE, useCamera);
}

UiSlider::UiSlider(int x, int y, int InitialValue, int maxvalue, bool interactuable, bool draggeable, bool usecamera, UiElement* parent, Module* elementmodule) :UiElement(x, y, 168, 14, interactuable, draggeable, usecamera, UiTypes::Slider, parent, elementmodule), bar({ 1282, 560, 168, 14 }), unhovered({ 1282, 584, 20, 20 }), hovered({ 1282, 584, 20, 20 }), clicked({ 1307, 584, 20, 20 }), currentState(Button_state::unhovered), BarPos(GetScreenPos()), value(InitialValue), MaxValue(maxvalue) {
	int InitialX = (int)(((float)value / (float)MaxValue) * (float)(bar.w - clicked.w));
	if (parent == nullptr)
		SetLocalPos(BarPos.x + InitialX, GetLocalPos().y);
	else
		SetLocalPos((BarPos.x + InitialX - parent->GetScreenPos().x), GetLocalPos().y);
}

UiSlider::~UiSlider() {
}

void UiSlider::Update(int dx, int dy) {
	app->input->GetMousePosition(dx, dy);
	SDL_Rect SliderRect = { BarPos.x,BarPos.y, bar.w ,bar.h };
	SDL_Rect MouseRect = { dx,dy,1,1 };
	bool intersection = SDL_HasIntersection(&MouseRect, &SliderRect);

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && intersection) 
	{
		currentState = Button_state::clicked;
		//app->gui->focusedUi = this;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) 
	{
		if (intersection)
			currentState = Button_state::hovered;
		else 
			currentState = Button_state::unhovered;
		//app->gui->focusedUi = nullptr;
	}
	if (currentState == Button_state::clicked) {
		if (parent == nullptr)
			SetLocalPos(dx, GetLocalPos().y);
		else
			SetLocalPos((dx - parent->GetScreenPos().x), GetLocalPos().y);

		if (GetScreenPos().x > (BarPos.x + bar.w - clicked.w)) {
			if (parent == nullptr)
				SetLocalPos(BarPos.x + bar.w - clicked.w, GetLocalPos().y);
			else
				SetLocalPos((BarPos.x - parent->GetScreenPos().x) + bar.w - clicked.w, GetLocalPos().y);
		}
		else if (GetScreenPos().x < BarPos.x) {
			if (parent == nullptr)
				SetLocalPos(BarPos.x, GetLocalPos().y);
			else
				SetLocalPos(BarPos.x - parent->GetScreenPos().x, GetLocalPos().y);
		}
		value = (int)(((float)MaxValue / (float)(bar.w - clicked.w)) * (float)(GetScreenPos().x - BarPos.x));
		if (module != nullptr)
			module->UiCallback(this);
	}
}

void UiSlider::Draw(SDL_Texture* atlas) {
	app->render->DrawTexture(atlas, BarPos.x, BarPos.y, &bar, 1.0f, SDL_FLIP_NONE, useCamera);
	if (currentState == Button_state::clicked)
		app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &clicked, 1.0f, SDL_FLIP_NONE, useCamera);
	else
		app->render->DrawTexture(atlas, GetScreenPos().x, GetScreenPos().y, &unhovered, 1.0f, SDL_FLIP_NONE, useCamera);
}