#ifndef _physworld_h
#define _physworld_h

/**
 @mainpage Physgame
 The Physgame engine is an abstraction layer between less portable, less user friendly, more sophistciated
 libraries and the game you want to make. If we do our jobs right this will save time and effort porting
 games between a variety of platforms. If you link only against this library, not a single line of your
 Standard compliant C++ code should need to change between platforms. At this early stage we are proving the
 concept with "Catch!" our first sample game. It Currently runs on Linux and Windows with an Identical
 codebase, when we are done with "Catch!" We want it to have one codebase, and downloadable in the Iphone
 app store, the Xbox store, on the PS3, on Steam, and in a variety of linux repositories.

 To get the latest news on development checkout: http://gitorious.org/physgame
 The wiki Which acts as our current Knowledge base: http://gitorious.org/physgame/pages/Home

 @section Engine Structure
  @subpage MainLoop "Main Loop Flow"

  @subpage CallbackManager "Call Back Manager"

  @subpage EventManager "Event Manager"

  @subpage Actor "Items in the world - Actor Class"

 */

//Includes and Forward Declarations
#include "physcrossplatform.h"
#include "physworldcallbackmanager.h"
#include "physevent.h"
#include "physeventmanager.h"
#include "physdatatypes.h"

#include <string>
#include <queue>

using namespace std;

//forward Declarations so that we do not need #include "physvector.h"
//forward Declarations so that we do not need #include "gamesettings.h"
//Forward declarations to prevent infite loop of includes
class Settings;
class PhysVector3;
class ActorBase;
//Other forward declarations
//forward Declarations so that we do not need #include "SDL.h"
class SDL_Surface;

//forward Declarations so that we do not need #include "btBulletDynamicsCommon.h"
class btAxisSweep3;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btSoftRigidDynamicsWorld;


//forward Declarations so that we do not need #include <Ogre.h>
namespace Ogre
{
	class Root;
	class RenderSystem;
	class RenderWindow;
    class ResourceGroupManager;
	class SceneManager;
	class Camera;
	class Viewport;
}


///////////////////////////////////////////////////////////////////////////////
/// @class PhysWorld
/// @headerfile physworld.h
/// @brief This is the main entry point for the entire library.
/// The physworld coordinates and integrates all the underlying subsystems,
/// Currently Ogre3d is used for 3d Graphics, Bullet is used for physics, and
/// SDL is used for user input and window management. Games will need a
/// container for all the playing pieces. It makes sense to tie all of this
/// functionality into one world object.
///////////////////////////////////////
class PhysWorld
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
		btSoftRigidDynamicsWorld* BulletDynamicsWorld;

		//SDL Objects
		SDL_Surface *SDLscreen;

		//Ogre objects
        Ogre::Root* OgreRoot;
        Ogre::RenderSystem* OgreRenderSystem;
        Ogre::RenderWindow* OgreGameWindow;
        Ogre::ResourceGroupManager* OgreResource;
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
        void TestLogger();
        template <class T> void OneLogTest(T Data, string DataType,string Message1 = "Logging and Throwing a ", string Message2 = "Logging a ");

		//SDL specific Items
		//This function will get all the events from SDL and Sort them into one of two Queues
		void PreProcessSDLEvents();
        queue<RawEvent*> SDL_WmEvents;
        queue<RawEvent*> SDL_UserInputEvents;

        /// Do Not Use this, This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        /// @warning This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        friend void RenderPhysWorld(PhysWorld *TheWorld);

	public:
        /// @brief Descriptive constructor
        /// This constructor allows for an easier way to define the boundaries for items moving about inside the physworld.
        /// @param GeographyLowerBounds The lower limits for the size of the physics simulation
        /// @param GeographyUpperbounds The Upper limits for the size of the physics simulation
        /// @param MaxPhysicsProxies This is the amount of Adows (Also called Actors or Proxies) allowed in a physics simulation.
		PhysWorld(PhysVector3* GeographyLowerBounds, PhysVector3* GeographyUpperbounds, unsigned short int MaxPhysicsProxies=1024);

        /// @brief Default constructor
        /// This simply performs the same work as the descriptive constructor with some sane, but small, limits. It will give you a world which expands for 100 units from the Origin, and only allows 10 Adows.
		PhysWorld();

        /// @brief Deconstructor
        /// This Tears down all the items create by the physworld, and safely frees any graphical resources, we will also delete any Objects passed into the
        /// Physworld by pointer. We will not delete any pointers we pass out (like from the Events from the Event manager)
		~PhysWorld();

		/// @brief Runtime Event logging Function
		/// @param Message This is what will be streamed to the log
		/// Be careful with this function, even though it appears to be a template, it does not support every data type. If Physgame is
		/// Compiled as a Shared Object, Dynamic Linked Library, or some other kind of stand alone library It will only support data types
		/// that are called internally, Currently that list includes: string, char, short int, int, long int, unsigned short int, unsigned int
		/// unsigned long int, bool, float, double, long double, wchar_t, size_t, PhysReal, PhysWhole, PhysString, and PhysVector3. If
		/// compiled statically it should support any data type which supports output streams.
    	template <class T> void Log(T Message);
    	/// @brief This is the preffered way to throw an exception currently
    	/// @param Message This will be streamed to the log, then used in a thrown exception.
    	/// This will log the Message, and will throw an exception with the Message included. Currently this supports all the Data
    	/// type the Log function supports
        template <class T> void LogAndThrow(T Message);

        /// @brief This Shows an Engine Generated Configuration Screen
        /// This could look like and could offer just about any option to the user. It is loosely expected to show Graphical Configuration
        /// options, like Vsync and Resolution, But it might ask some really silly stuff. I thnk this would be fine for smaller simpler
        /// Which have no other way to configure such things, but any sizable project should develop their own way to expose and manage
        /// user settings.
        bool ShowSystemSettingDialog();

		/// @brief This moves the camera relative to the world
		/// @param Position Where should the camera be seated
		/// @param LookAt Point the camera such that this poin is centered on the screen
		/// The parameters really do explain it. This puts the camera at an arbitrary point, pointing at an arbitrary point.
		void MoveCamera(PhysVector3 Position, PhysVector3 LookAt);

        /// @brief This creates the game window and starts the game.
        /// Prior to this all of the physics and graphical object containers should have been loaded and prepared for use. There should be
        /// minimal delay from the time you call this and the game actually begins.
        /// This is also where the Main Loop for the game is housed.
        void GameInit();

        /// @brief Performs all the items that would normally be performed during the game loop
        /// This simply calls: DoMainLoopPhysics, DoMainLoopInputBuffering, DoMainLoopWindowManagerBuffering, DoMainLoopRender. This is
        /// useful for anyone wants to use as little of the existing main loop structure as possible, or does not want to run a certain Items
        /// each iteration of the main loop.
        void DoMainLoopAllItems();

        /// @brief Increments physics by one step
        /// Currently one step is about 1/60 of a second. This function is automatically called in the main loop if a Pre-Physics Callback is
        /// set. This is the second step in the main loop chain of events. This is where we expect the majority of our colision events to come
        /// from although it is concievable that a game could manually insert those manually.

        void DoMainLoopPhysics();
		void DoMainLoopInputBuffering();
		void DoMainLoopWindowManagerBuffering();
		void DoMainLoopRender();

        void AddActor(ActorBase* ActorToAdd);


		//used to set callback functions to be run in the main loop
		PhysWorldCallBackManager* CallBacks;

		//This will be used to communicate with underlying sybsystems in a clean way
		PhysEventManager*  Events;
};
#endif
