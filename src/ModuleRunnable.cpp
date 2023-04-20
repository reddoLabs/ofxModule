#include "ModuleRunnable.h"

namespace ofxModule {
	ModuleRunnable::ModuleRunnable(string moduleClass, string moduleName, bool isMultiThreaded, bool isStartThread, int fps)
	{
		initModule(moduleClass, moduleName, "settings.json/" + moduleName, isMultiThreaded, isStartThread, fps);
	}

	ModuleRunnable::ModuleRunnable(string moduleClass, string moduleName, string settingsPath, bool isMultiThreaded, bool isStartThread, int fps)
	{
		initModule(moduleClass, moduleName, settingsPath, isMultiThreaded, isStartThread, fps);
	}

	ModuleRunnable::~ModuleRunnable()
	{
		if (isMultiThreaded) {
			waitForThread(true);
		}
	}

	void ModuleRunnable::initModule(string moduleClass_, string moduleName_,string settingsPath_, bool isMultiThreaded_, bool isStartThread, int fps_) {
		fps = fps_;
		moduleClass = moduleClass_;
		moduleName = moduleName_;

		// decode settings path
		auto pathElems = ofSplitString(settingsPath_, "/");
		if (pathElems.size() >= 1 && ofIsStringInString(pathElems.back(), ".json")) {
			settingsPath = settingsPath_ + "/" + moduleName_;
		} else {
			settingsPath = settingsPath_;
		}

		loadSettings();
		loadEnvironmentVariables(".env");


		isIdle = false;
		isMultiThreaded = isMultiThreaded_;
		if (isMultiThreaded && isStartThread) startThread();
	}


	bool ModuleRunnable::getMultiThreaded()
	{
		return isMultiThreaded;
	}

	void ModuleRunnable::threadedFunction() {
		while (isThreadRunning())
		{
			sleep(1000 / fps);
			if (lock())
			{
				update();
				unlock();
			}
			else
			{
				ofLogWarning("ModuleRunnable::threadedFunction()") << "Unable to lock mutex of " + moduleName;
				sleep(1000);
			}
		}

	}

	void ModuleRunnable::registerModule(ModuleRunnable* module) {
		if (lock())
		{
			registerModuleEvent(module->moduleEvent);
			unlock();
		}
		else
		{
			ofLogWarning("ModuleRunnable::newModuleEvent()") << "Unable to lock mutex.";
			sleep(1000);
		}
	}

	string ModuleRunnable::getModuleClass() {
		return moduleClass;
	}

	string ModuleRunnable::getModuleName() {
		return moduleName;
	}

	void ModuleRunnable::setSettingsPath(string path)
	{
		settingsPath = path;
	}

	string ModuleRunnable::getSettingsPath()
	{
		return settingsPath;
	}

	void ModuleRunnable::loadSettings() {
		loadSettings(settingsPath);
	}

	void ModuleRunnable::saveSettings() {
		saveSettings(settingsPath);

	}
	void ofxModule::ModuleRunnable::loadSettings(string path)
	{
		// split file path and json path
		auto pathVar = ofSplitString(path, ".json");

		// load file
		ofJson sTemp = ofLoadJson(pathVar[0] + ".json");

		if (pathVar.size() >= 2) {
			// get the correct node in the json tree
			auto jsonPath = ofSplitString(pathVar[1], "/");
			for (size_t i = 0; i < jsonPath.size(); i++) {
				if (jsonPath[i] != "") sTemp = sTemp[jsonPath[i]];
			}
		}

		//set settings
		settings = sTemp;
		if (settings != nullptr) ofLogNotice("ModuleRunnable::loadSettings", moduleName + " settings loaded");
		else {
			ofLogNotice("ModuleRunnable::loadSettings", "No settings preset for " + moduleName);
			settings = ofJson();
		}
	}

	void ModuleRunnable::saveSettings(string path)
	{
		// split file path and json path
		auto pathVar = ofSplitString(path, ".json");

		// load file
		ofJson sSave = ofLoadJson(pathVar[0] + ".json");
		ofJson* sPointer = &sSave;

		if (pathVar.size() >= 2) {
			// get the correct node in the json tree
			auto jsonPath = ofSplitString(pathVar[1], "/");
			for (size_t i = 0; i < jsonPath.size(); i++) {
				if (jsonPath[i] != "") sPointer = &sSave[jsonPath[i]];
			}
		}

		*sPointer = settings;
		ofSavePrettyJson(pathVar[0] + ".json", sSave);
	}

	void ModuleRunnable::loadEnvironmentVariables(string path)
	{
		ofBuffer buffer = ofBufferFromFile(path);

		if (buffer.size()) {
			for(auto& line:buffer.getLines()){
				auto kvPair = ofSplitString(line, "=", false, true);
				if (kvPair.size() == 2) {
					envVars.insert(make_pair(kvPair.front(), kvPair.back()));
				}
				else {
					ofLogError("ModuleRunnable::loadEnvironmentVariable", line + " is no valid env variable.");
				}
			}
		}
	}


}
