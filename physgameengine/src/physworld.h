#ifndef _physworld_h
#define _physworld_h
///////////////////////////////////////////////////////////////////////////////
// The game will need a container for all the playing pieces, and something to
//talk tothe screen and the physics engine. It makes sense to call this the
//world object. This is the

///////////////////////////////////////////////////////////////////////////////
//Includes and Forward Declarations

#include <string>

using namespace std;

//#include <Ogre.h>
namespace Ogre
{
	class Root;
	class RenderSystem;
	class RenderWindow;
	class SceneManager;
	class Camera;
	class Viewport;
}

//#include "SDL.h"
class SDL_Surface;

//Not included to prevent infinite loops
#include "physworldcallbackmanager.h"
#include "physeventmanager.h"
//#include "gamebase.h"
//#include "physvector.h"
//Forward declarations to prevent infite loop of includes
//class physworldFrameListener;
class Settings;

//physworld.cpp
class PhysVector3;

///////////////////////////////////////////////////////////////////////////////
// Physworld Class Declaration
class physworld
{
	private:
		//TODO Add Bullet Physics world
		//dWorldID OdeWorld;

		//SDL Objects
		SDL_Surface *SDLscreen;

		//Ogre objects
        Ogre::Root* OgreRoot;
        Ogre::RenderSystem* OgreRenderSystem;
        Ogre::RenderWindow* OgreGameWindow;
		Ogre::SceneManager* OgreSceneManager;
		Ogre::Camera* OgreCamera;
		Ogre::Viewport* OgreViewport;

		//Players settings
		Settings* PlayerSettings;

		//Try to retrieve ogre settinss
		void LoadOgreSettings();

		//makes the windows all the graphics will be drawn to
		void CreateRenderWindow();

	public:
		physworld();
		~physworld();

		//I am just extending what ogre provides for a logging system
        void Log(string Message);
        void LogAndThrow(string Message);

        //I plan on deprecating this thing soon and building our own settings system
        bool ShowSystemSettingDialog();

		//Change the camera angle;
		void MoveCamera(PhysVector3 Position, PhysVector3 LookAt);

        //Starts the Game
        void GameInit();

        //Functions to run during the game loop.
        void DoMainLoopAllItems();
        void DoMainLoopPhysics();
		void DoMainLoopInputBuffering();

		//used to set callback functions to be run in the main loop
		physworldCallBackManager* CallBacks;

		//This will be used to communicate with underlying sybsystems in a clean way
		PhysEventManager*  Events;

};
#endif
