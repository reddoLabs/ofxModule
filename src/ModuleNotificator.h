#pragma once
#include "ModuleEvent.h"

namespace ofxModule {
	class ModuleNotificator
	{
	public:
		ModuleNotificator();
		~ModuleNotificator();

		/// \brief notification of other modules
		///
		/// predefined message parameters
		/// ------------------------------
		/// "modulName" : only a modul with this name can receive the message
		/// "moduleClass" : only a modul of this class can receive the message
		/// "setIdle" : true/false  enables or deactivates the module
		void notifyEvent(string address, ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(shared_ptr<ofTexture> texture, string address = "texture", ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(shared_ptr<ofImage> image, string address = "image", ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(shared_ptr<ofFbo> fbo, string address = "fbo", ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(shared_ptr<ofPixels> pixels, string address = "pixels", ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(vector<shared_ptr<ofTexture>> textures, string address = "textures", ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(vector < shared_ptr<ofImage>> images, string address = "images", ofJson message = ofJson());
		/// \brief notification of other modules
		void notifyEvent(vector<shared_ptr<ofFbo>> fbo, string address = "fbos", ofJson message = ofJson());
		void notifyEvent(ModuleEvent e);

		/// \brief event to communicate with registered other modules
		///
		/// It can contain a json, texture or image data
		ofEvent<ModuleEvent> moduleEvent;

		/// \brief called when a module event enters, specific event reactions can be defined in proceedModuleEvent(ModuleEvent& e)
		void onModuleEvent(ModuleEvent& e);

		/// \brief indicates the idle state
		///
		/// an idle class does neither update or draw
		bool getIsIdle();

		/// \brief sets the idle state
		///
		/// an idle class does neither update or draw
		void setIsIdle(bool isIdle);

		void registerModuleEvent(ofEvent<ModuleEvent>& event);


	protected:
		/// \brief function for sublasses to react on input
		virtual void proceedModuleEvent(ModuleEvent& e) {};

		string moduleClass;
		string moduleName;

		bool isIdle = false;
	};
}

