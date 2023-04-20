#include "ModuleEvent.h"

namespace ofxModule {

	ModuleEvent::ModuleEvent(string moduleClass_, string id_,string address_ , ofJson message_, string destClass_)
	{
		moduleClass = moduleClass_;
		id = id_;
		message = message_;
		address = address_;
		type = MODULE_EVENT_MESSAGE;
		destClass = destClass_;

		texture = nullptr;
		image = nullptr;
	}

	ModuleEvent::~ModuleEvent()
	{

	}


}