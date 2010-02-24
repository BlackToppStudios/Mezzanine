#ifndef _physworld_h
#define _physworld_h
///////////////////////////////////////////////////////////////////////////////
// The game will need a container for all the playing pieces, and something to
//talk tothe screen and the physics engine. It makes sense to call this the
//world object. This is the

///////////////////////////////////////////////////////////////////////////////
//Includes and Forward Declarations

#include "physworldcallbackmanager.h"
#include "physevent.h"
#include "physeventmanager.h"
#include "physdatatypes.h"

#include <string>
#include <queue>



using namespace std;

//Not included to prevent infinite loops
//#include "physvector.h"
//#include "gamesettings.h"
//Forward declarations to prevent infite loop of includes
class Settings;
class PhysVector3;

//Other forward declarations
//#include "SDL.h"
class SDL_Surface;
union SDL_Event;


//#include "btBulletDynamicsCommon.h"
class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;


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


///////////////////////////////////////////////////////////////////////////////
// Physworld Class Declaration
///////////////////////////////////////
class physworld
{
	private:
		//Physics Items
		PhysVector3* GeographyLowerBounds;
		PhysVector3* GeographyUpperbounds;
		unsigned short int  MaxPhysicsProxies;

		btAxisSweep3* BulletBroadphase;
		btDefaultCollisionConfiguration* BulletCollisionConfiguration;
		btCollisionDispatcher* BulletDispatcher;
		btSequentialImpulseConstraintSolver* BulletSolver;
		btDiscreteDynamicsWorld* BulletDynamicsWorld;

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

		//Manages the windows all the graphics will be drawn to
		void CreateRenderWindow();
		void DestroyRenderWindow();

		//Used by the constructors
		void Construct(PhysVector3* GeographyLowerBounds, PhysVector3* GeographyUpperbounds, unsigned short int MaxPhysicsProxies);

		//SDL specific Items
		//This function will get all the events from SDL and Sort them into one of two Queues
		void PreProcessSDLEvents();
        queue<SDL_Event> SDL_WmEvents;
        queue<SDL_Event> SDL_UserInputEvents;

	public:
		physworld(PhysVector3* GeographyLowerBounds, PhysVector3* GeographyUpperbounds, unsigned short int MaxPhysicsProxies=1024);
		physworld();
		~physworld();

		//I am just extending what ogre provides for a logging system
        void Log(string Message);
        void Log(PhysWhole Message);
		void Log(PhysReal Message);
        void LogAndThrow(string Message);
        void LogAndThrow(PhysWhole Message);
		void LogAndThrow(PhysReal Message);


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
		void DoMainLoopWindowManagerBuffering();

		//used to set callback functions to be run in the main loop
		physworldCallBackManager* CallBacks;

		//This will be used to communicate with underlying sybsystems in a clean way
		PhysEventManager*  Events;
};
#endif
