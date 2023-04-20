#include "ModuleControl.h"

namespace ofxModule {
	//------------------------------------------------------------------
	ModuleControl::ModuleControl() {
		// init module runner and loader
		moduleRunner = new ModuleRunner();
		moduleLoader = new ModuleLoader(moduleRunner);
	}

	ModuleLoader* ModuleControl::getModuleLoader() {
		return moduleLoader;
	}

	ModuleRunner* ModuleControl::getModuleRunner() {
		return moduleRunner;
	}

}