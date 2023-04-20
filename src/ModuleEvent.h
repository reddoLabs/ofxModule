#ifndef _ModuleEvent
#define _ModuleEvent

#include "ofMain.h"


/// \brief event class for communication between modules
/// the event class provides 4 types : messages, images, textures or fbos

namespace ofxModule {
	class ModuleEvent {

	public:
		enum EventType {
			MODULE_EVENT_MESSAGE,
			MODULE_EVENT_TEXTURE,
			MODULE_EVENT_IMAGE,
			MODULE_EVENT_FBO,
			MODULE_EVENT_PIXELS,
			MODULE_EVENT_MULTIPLE_FBO,
			MODULE_EVENT_MULTIPLE_IMAGE,
			MODULE_EVENT_MULTIPLE_TEXTURE
		};

		ModuleEvent(string moduleClass, string id, string address, ofJson message, string destClass = "");
		~ModuleEvent();

		string address = "";
		string moduleClass;
		string id;
		
		string destClass = "";
		string destId = "";

		ofJson message;
		shared_ptr<ofTexture> texture;
		shared_ptr<ofImage> image;
		shared_ptr<ofFbo> fbo;
		shared_ptr<ofPixels> pixels;
		vector<shared_ptr<ofTexture>> textures;
		vector < shared_ptr<ofImage>> images;
		vector < shared_ptr<ofFbo>> fbos;

		EventType type;
	};
}

#endif
