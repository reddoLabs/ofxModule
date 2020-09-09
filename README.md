# ofxModule

An addon that allows to create reusable modules, configure them in a json and controlling them

Compatible with openFrameworks 0.11.0+

Generate all project files using the openFrameworks Project Generator.

## Getting started

Have a look at the sample projects to get started directly and have a small explanation here.

### Create a Module

A module can be child of the following types

* `ModuleRunnable` : a module that provides an update method and basic module communication
* `ModuleDrawable` : same as `ModuleRunnable`, but has an additional `draw()` function

A base Module could look like this

```cpp
namespace ofxModule {

class Communicator : public ModuleRunnable{
	
  public:
	
	Communicator(string moduleName = "Communicator"); // a module needs a name
    void update(); // the update function
    void stopModule(); // called when module stops
    
protected:
    void proceedModuleEvent(ModuleEvent& e); // manages event communication
    
    };
}
```

### Define parameters

You can define parameters in `settings.json`, that will be available while running the module.

```json
// settings.json

{
    "Communicator": {
        "name": "the nice gui",
        "multiplicator": 1.3
    }
}
```

The can be accessed during runtime by calling `settings`

```cpp
name = settings["name"].get<string>();
```

## Set up communication

### modules.json
Communication between modules is set up in `modules.json`.

You need to define a module with a `type` and optional an `id` and `inputs`. 
A module is only able to receive messages from its inputs. In the example `Presenter` and `Camera` can communicate between each other, since the have them as inputs.

```json
// modules.json

[
	{
		"type":"Camera",
        "id":"theCamera",
		"inputs":["Presenter"]
	},
	{
		"type":"Presenter",
		"inputs":["Camera"]
	}
]
```

### init modules

Modules are created and their communication is initialized by the `ModuleControl`. 

```cpp
// ofApp.h

#include "ofMain.h"
#include "ModuleControl.h"

using namespace ofxModule;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ModuleControl webModule;
};


// ofApp.cpp

void ofApp::setup(){
// Our app will consist of 2 modules that show how module communication works
	// Communicator : it receives a number, multiplies it and sends it to the presenter
	// Presenter : it sends a number, receives an answer from the communicator and shows ist on the screen
    
    // first we init the app by crawling the config (modules.json), 
	// searching for the specific components and adding them to the module control
    
    if (webModule.getModuleLoader()->getCountModulesConfig("Camera")) {
		
		// create a Camera and add it to the modules
        webModule.getModuleLoader()->addModule(new Camera());
    }
	if (webModule.getModuleLoader()->getCountModulesConfig("Presenter")) {
		webModule.getModuleLoader()->addModule(new Presenter());
	}
    
	// enables the communication between modules (set in modules.json)
	webModule.getModuleLoader()->initModuleCommunication();
}


void ofApp::update(){
	// update the modules
	webModule.getModuleRunner()->updateModules();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw the modules
    webModule.getModuleRunner()->drawModules();
}


```

### send and receive messages

Modules communicate through messages. This can be textual (json) and/or graphics (ofTexture, ofFbo, ofImage, ofPixels).
To send a message use `notifyEvent()`. Every message needs an address that can be checked from the receiving module.

#### Sending a message

```cpp
ofJson message;

// define the value
response["value"] = 22;

// spread the answer to all registered modules
// the function name is aMessage
notifyEvent("aMessage",response);
```

#### Receiving a message

```cpp
void Communicator::proceedModuleEvent(ModuleEvent & e)
	{
		// check the address type
		if (e.address == "aMessage") {
            ofLogNotice(e["value"].get<string>());
		}
	}
```

## Multithreading

A module can run in a seperate thread. 

`ModuleRunnable(string moduleClass, string moduleName, bool isMultiThreaded = false, bool isStartThread = false, int fps = 60);`

```cpp
// Module that runs in seperate thread

Communicator::Communicator(string moduleName):ModuleRunnable("Communicator", moduleName,true,true) {
}
```