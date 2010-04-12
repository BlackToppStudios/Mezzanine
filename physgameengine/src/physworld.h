//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _physworld_h
#define _physworld_h

/**
 @mainpage Physgame
 The Physgame engine is an abstraction layer between less portable, less user friendly, more sophistciated
 libraries and the game you want to make. If we do our jobs right this will save time and effort making and porting
 games between a variety of platforms. If you link only against this library, not a single line of your
 Standard compliant C++ code should need to change between platforms. At this early stage we are proving the
 concept with "Catch!" our first sample game. It Currently runs on Linux and Windows with an Identical
 codebase, when we are done with "Catch!" We want it to have one codebase, and downloadable in the Iphone
 app store, the Xbox store, on the PS3, on Steam, and in a variety of linux repositories.

 To get the latest news on development checkout: http://gitorious.org/physgame
 The wiki Which acts as our current Knowledge base: http://gitorious.org/physgame/pages/Home

 @section Engine Structure
  @subpage mainloop1 "Main Loop Flow"

  //@subpage CallbackManager "Call Back Manager"

  //@subpage EventManager "Event Manager"

  //@subpage Actor "Items in the world - Actor Class"

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
/// @details The physworld coordinates and integrates all the underlying subsystems,
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

        /// @brief Do Not Use this, This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        /// @warning This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        friend void RenderPhysWorld(PhysWorld *TheWorld);

	public:

        /// @todo TODO: Create a data member and access functions for Window name, FrameDelay, and a any other Phys game variables still be hardset

        /// @brief Descriptive constructor
        /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the physworld.
        /// @param GeographyLowerBounds The lower limits for the size of the physics simulation
        /// @param GeographyUpperbounds The Upper limits for the size of the physics simulation
        /// @param MaxPhysicsProxies This is the amount of Adows (Also called Actors or Proxies) allowed in a physics simulation.
		PhysWorld(PhysVector3* GeographyLowerBounds, PhysVector3* GeographyUpperbounds, unsigned short int MaxPhysicsProxies=1024);

        /// @brief Default constructor
        /// @details This simply performs the same work as the descriptive constructor with some sane, but small, limits. It will give you a world which expands for 100 units from the Origin, and only allows 10 Adows.
		PhysWorld();

        /// @brief Deconstructor
        /// @details This Tears down all the items create by the physworld, and safely frees any graphical resources, we will also delete any Objects passed into the
        /// Physworld by pointer. We will not delete any pointers we pass out (like from the Events from the Event manager)
		~PhysWorld();

		/// @brief Runtime Event logging Function
		/// @details Be careful with this function, even though it appears to be a template, it does not support every data type. If Physgame is
		/// Compiled as a Shared Object, Dynamic Linked Library, or some other kind of stand alone library It will only support data types
		/// that are called internally, Currently that list includes: string, char, short int, int, long int, unsigned short int, unsigned int
		/// unsigned long int, bool, float, double, long double, wchar_t, size_t, PhysReal, PhysWhole, PhysString, PhysVector3, RawEvent and MetaCode.
		/// If compiled statically it should support any data type which supports output streams.
		/// @param Message This is what will be streamed to the log
    	template <class T> void Log(T Message);
    	/// @brief This is the preffered way to throw an exception currently
    	/// @details This will log the Message, and will throw an exception with the Message included. Currently this supports all the Data
    	/// type the Log function supports
    	/// @param Message This will be streamed to the log, then used in a thrown exception.
        template <class T> void LogAndThrow(T Message);

        /// @brief This Shows an Engine Generated Configuration Screen
        /// @details This could look like and could offer just about any option to the user. It is loosely expected to show Graphical Configuration
        /// options, like Vsync and Resolution, But it might ask some really silly stuff. I thnk this would be fine for smaller simpler
        /// Which have no other way to configure such things, but any sizable project should develop their own way to expose and manage
        /// user settings.
        bool ShowSystemSettingDialog();

		/// @brief This moves the camera relative to the world
		/// @details The parameters really do explain it. This puts the camera at an arbitrary point, pointing at an arbitrary point.
        /// @param Position Where should the camera be seated
		/// @param LookAt Point the camera such that this poin is centered on the screen
		void MoveCamera(PhysVector3 Position, PhysVector3 LookAt);

        /// @brief This creates the game window and starts the game.
        /// @param CallMainLoop should the main loop be called
        /// @details Prior to this all of the physics and graphical object containers should have been loaded and prepared for use. There should be
        /// minimal delay from the time you call this and the game actually begins.
        /// This will automatically call the Main Loop unless passed falsed.
        void GameInit( bool CallMainLoop=true );

        void MainLoop();

        /// @brief Performs all the items that would normally be performed during the game loop
        /// @details This simply calls: DoMainLoopPhysics, DoMainLoopInputBuffering, DoMainLoopWindowManagerBuffering, DoMainLoopRender. This is
        /// useful for anyone wants to use as little of the existing main loop structure as possible, or does not want to run a certain Items
        /// each iteration of the main loop.
        void DoMainLoopAllItems();

        /// @brief Increments physics by one step
        /// @details Currently one step is about 1/60 of a second. This function is automatically called in the main loop if a Pre-Physics Callback is
        /// set. This is the second step in the main loop chain of events. This is where we expect the majority of our collision events to come
        /// from although it is concievable that a game could manually insert those manually. This will not delete events it places
        /// in the event manager, that is the responsibility of the code that pulls out the event out.
        void DoMainLoopPhysics();

        /// @brief Gathers user input from the OS and places events in the event manager
        /// @details This this is automatically called during the mainloop if you have set a PreInput callback. This will not delete events it places
        /// in the event manager, that is the responsibility of the code that pulls out the event out.
		void DoMainLoopInputBuffering();

		/// @brief Creates events for each Window manger
		/// @details This gather information from system/windows manager events, such as windows  minimization, maximization, program exit, window hidden
		/// window shown, and a few other similar types of events. This makes events out of the information and places them in the event manager
		/// This will not delete events it places in the event manager, that is the responsibility of the code that pulls out the event out.
		void DoMainLoopWindowManagerBuffering();

        /// @brief This forces the screen to be re-rendered
        /// @details This renders the screen based on the status of all in game actors. This is automatically called in the main loop.
		void DoMainLoopRender();

        /// @brief The adds and Actor to the physworld.
        /// @details The takes over for manager an Actor, and makes sure that it's physics status and 3d graphics status are properly handled. Once an
        /// actor has been passed into the Physworld using this, the physworld handle deleting it.
        /// @param ActorToAdd This is a pointer to the actor to be added
        void AddActor(ActorBase* ActorToAdd);

		/// @brief This is a point to the default Call BackManager
        /// @details All the callbacks that the main loop and the rest of physgame use are will be found in the callback manager point to by this.
		PhysWorldCallBackManager* CallBacks;

        /// @brief This is the default pointer to the Event Manager.
        /// @details This is the Event manager that all physworld members will place any events into.
		PhysEventManager* Events;
};
#endif
