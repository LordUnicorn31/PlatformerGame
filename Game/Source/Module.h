#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class UiElement;
struct Collider;

class Module
{
public:

	Module() : active(false)
	{}

	virtual void Init()
	{
		active = true;
		enabled = true;
	}

	// Called before render is available
	// TODO 5: Sending config file to all modules
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void UiCallback(UiElement* element) {

	}

	void Enable()
	{
		if (enabled == false) 
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled)
		{
			enabled = false;
			CleanUp();
		}
	}

	const bool isEnabled()
	{
		return enabled;
	}

public:

	SString name;
	bool active;
	virtual void OnCollision(Collider*, Collider*) {};
protected:
	bool enabled = true;

};

#endif // __MODULE_H__