#include "ModuleNotificator.h"

namespace ofxModule {

	ModuleNotificator::ModuleNotificator()
	{
	}


	ModuleNotificator::~ModuleNotificator()
	{
	}

	void ModuleNotificator::notifyEvent(shared_ptr<ofFbo> fbo, string address, ofJson message) {
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.fbo = fbo;
			e.type = ModuleEvent::MODULE_EVENT_FBO;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(shared_ptr<ofTexture> texture, string address, ofJson message) {
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.texture = texture;
			e.type = ModuleEvent::MODULE_EVENT_TEXTURE;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(shared_ptr<ofImage> image, string address, ofJson message) {
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.image = image;
			e.type = ModuleEvent::MODULE_EVENT_IMAGE;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(shared_ptr<ofPixels> pixels, string address, ofJson message) {
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.pixels = pixels;
			e.type = ModuleEvent::MODULE_EVENT_PIXELS;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(vector<shared_ptr<ofTexture>> textures, string address, ofJson message)
	{
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.textures = textures;
			e.type = ModuleEvent::MODULE_EVENT_MULTIPLE_TEXTURE;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(vector<shared_ptr<ofImage>> images, string address, ofJson message)
	{
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.images = images;
			e.type = ModuleEvent::MODULE_EVENT_MULTIPLE_IMAGE;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(vector<shared_ptr<ofFbo>> fbos, string address, ofJson message)
	{
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			e.fbos = fbos;
			e.type = ModuleEvent::MODULE_EVENT_MULTIPLE_FBO;
			ofNotifyEvent(moduleEvent, e);
		}
	}

	void ModuleNotificator::notifyEvent(ModuleEvent e)
	{
		ofNotifyEvent(moduleEvent, e);
	}

	void ModuleNotificator::onModuleEvent(ModuleEvent & e)
	{
		if ((e.destClass == "" && e.destId == "") || e.destId == moduleName || e.destClass == moduleClass) {
			if (e.address == "setIdle") {
				if (e.message["value"].get<bool>() == true) setIsIdle(true);
				else setIsIdle(false);
			}
			proceedModuleEvent(e);

		}
	}

	void ModuleNotificator::notifyEvent(string address, ofJson message) {
		if (!isIdle) {
			ModuleEvent e = ModuleEvent(moduleClass, moduleName, address, message);
			ofNotifyEvent(moduleEvent, e);
		}
	}

	bool ModuleNotificator::getIsIdle() {
		return isIdle;
	}
	void ModuleNotificator::setIsIdle(bool isIdle_) {
		isIdle = isIdle_;
	}
	void ModuleNotificator::registerModuleEvent(ofEvent<ModuleEvent>& event)
	{
		ofAddListener(event, this, &ModuleNotificator::onModuleEvent);
	}
}