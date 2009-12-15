#ifndef _physworld_h
#define _physworld_h
///////////////////////////////////////////////////////////////////////////////
// The game will need a container for all the playing pieces, and something to
//talk tothe screen and the physics engine. It makes sense to call this the
//world object. This is the

///////////////////////////////////////////////////////////////////////////////
//Includes
#include <ode/ode.h>
#include <Ogre.h>

#include "SDL.h"

//Not included to prevent infinite loops
//#include "physworldframelistener.h"
//#include "gamebase.h"

using namespace std;

//Forward declarations to prevent infite loop of includes
class physworldFrameListener;
class Settings;

///////////////////////////////////////////////////////////////////////////////
// Physworld Class Declaration
class physworld
{
	private:
		//the ODE Physics world
		dWorldID OdeWorld;

		//SDL Objects
		SDL_Surface* SDLscreen;

		//Ogre objects
        Ogre::Root* OgreRoot;
        Ogre::RenderSystem* OgreRenderSystem;
        Ogre::RenderWindow* OgreGameWindow;
		Ogre::SceneManager* OgreSceneManager;
		Ogre::Camera* OgreCamera;
		Ogre::Viewport* OgreViewport;
		physworldFrameListener* OgreFrameListener;

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
		void MoveCamera(Ogre::Vector3 Position, Ogre::Vector3 LookAt);

        //Starts the Game
        void GameInit();

        //Functions to run during the game loop.
        void DoMainLoopAllItems();
        void DoMainLoopPhysics();
		void DoMainLoopInputBuffering();


};
#endif
