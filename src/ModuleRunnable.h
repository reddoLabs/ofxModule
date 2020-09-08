#ifndef _IModuleRunnable
#define _IModuleRunnable

#include "ModuleEvent.h"
#include "ModuleNotificator.h"


/// \brief interface for modules
///
/// every part of the app, except the core is defined as a module
/// modules can be loaded through json and run in a seperate thread

namespace ofxModule {

	class ModuleRunnable : public ModuleNotificator, public ofThread {

	public:

		/// \brief actual function that proceedes function of subclasses
		virtual void update() = 0;

		/// \brief behaviour when the module is stopped
		virtual void stopModule() {};

		/// \brief The class constructor
		///
		/// \param moduleClass the class
		/// \param moduleName the instance name
		/// \param isMultiThreaded indicates if the module should run in a seperate thread
		/// \param isStartThread should the thread directly startet after the construction (only applies if multiThreaded)
		/// \param fps the framerate the module should run (only applies if multiThreaded)
		ModuleRunnable(string moduleClass, string moduleName, bool isMultiThreaded = false, bool isStartThread = false, int fps = 60);
		ModuleRunnable(string moduleClass, string moduleName, string settingsPath, bool isMultiThreaded = false, bool isStartThread = false, int fps = 60);
		~ModuleRunnable();

		/// \brief indicates if the module runs its own thread
		bool getMultiThreaded();

		/// \brief the threaded function
		void threadedFunction();

		/// \brief enable receiving events from another module
		void registerModule(ModuleRunnable* module);

		string getModuleClass();
		string getModuleName();

		

		/// \brief sets the path of the settings json as well as the node in the json
		///
		/// Example "module/settings.json/modulname/stuff" 
		/// settings file: "module/settings.json"
		/// position in settings: {"modulname":{"settings":{"here you can find":"the module settings"}}
		void setSettingsPath(string path);

		/// \brief returns the path of the settings json as well as the node in the json
		string getSettingsPath();

		/// \brief load settings using the module settings path
		virtual void loadSettings();

		/// \brief save settings using the module settings path
		virtual void saveSettings();

		/// \brief load settings using a specified settings path
		virtual void loadSettings(string path);

		/// \brief save settings using a specified settings path
		virtual void saveSettings(string path);

		/// \called when all modules loaded
		virtual void onAllModulesLoaded() {};

		friend std::ostream& operator<< (std::ostream &out, const ModuleRunnable& module);

	protected:
		int fps;


		/// \brief called when module is created
		void initModule(string moduleClass, string moduleName, string settingsPath, bool isSingleThreaded, bool isStartThread, int fps);

		
		bool isMultiThreaded;

		/// \brief settings that can be defined in a json file
		ofJson settings;

	private:
		string settingsPath;
	};

	inline
		ostream& operator<<(ostream& os, const ModuleRunnable& module)
	{
		os << module.moduleName << "\t" << module.moduleClass << "\t" << (module.isIdle ? "true" : "false") << "\t" << (module.isMultiThreaded ? "true" : "false");
		return os;
	}

}
#endif
