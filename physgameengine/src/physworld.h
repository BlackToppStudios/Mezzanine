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
 @section Data Types
  @ref PhysVector3

  @ref PhysEvent
*/

//Includes and Forward Declarations
#include "physcrossplatform.h"
#include "physworldcallbackmanager.h"
#include "event.h"
#include "physeventmanager.h"
#include "physdatatypes.h"
#include "physvector.h"

using namespace phys;

#include <string>

using namespace std;

//forward Declarations so that we do not need #include "physvector.h"
//forward Declarations so that we do not need #include "gamesettings.h"
//Forward declarations to prevent infite loop of includes
namespace phys
{
    class GraphicsSettings;
}
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
		PhysVector3 GeographyLowerBounds;
		PhysVector3 GeographyUpperbounds;
		unsigned short int  MaxPhysicsProxies;

		btAxisSweep3* BulletBroadphase;
		btDefaultCollisionConfiguration* BulletCollisionConfiguration;
		btCollisionDispatcher* BulletDispatcher;
		btSequentialImpulseConstraintSolver* BulletSolver;
		btSoftRigidDynamicsWorld* BulletDynamicsWorld;

		//SDL Objects
		SDL_Surface *SDLscreen;
		bool HasSDLBeenInitialized;

		//Ogre objects
        Ogre::Root* OgreRoot;
        Ogre::RenderSystem* OgreRenderSystem;
        Ogre::RenderWindow* OgreGameWindow;
        Ogre::ResourceGroupManager* OgreResource;
		Ogre::SceneManager* OgreSceneManager;
		Ogre::Camera* OgreCamera;
		Ogre::Viewport* OgreViewport;

		//Players settings
		GraphicsSettings* PlayerSettings;

		//Try to retrieve ogre settinss
		void LoadOgreSettings();

		//Manages the windows all the graphics will be drawn to
		void CreateRenderWindow();
		void DestroyRenderWindow();

		//Used by the constructors
		void Construct(const PhysVector3 &GeographyLowerBounds, const PhysVector3 &GeographyUpperbounds, const unsigned short int &MaxPhysicsProxies);
        void SanityChecks();
        void TestLogger();
        template <class T> void OneLogTest(T Data, string DataType,string Message1 = "Logging and Throwing a ", string Message2 = "Logging a ");

        //Settings for Engine Functionality
        string WindowName;
        PhysWhole TargetFrameLength;

        PhysReal PhysicsStepsize;

        /// @brief Do Not Use this, This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        /// @warning This should be treated as an internal function, it is \b subject \b to \b change \b without \b warning and could be \b harmful to overall stability if used incorrectly
        friend void RenderPhysWorld(PhysWorld *TheWorld);

        friend class ActorBase;
	public:

        /// @todo TODO: Create a data member and access functions for FrameDelay, and a any other Phys game variables still be hardset

        /// @brief Descriptive constructor
        /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the physworld.
        /// @param GeographyLowerBounds The lower limits for the size of the physics simulation
        /// @param GeographyUpperbounds The Upper limits for the size of the physics simulation
        /// @param MaxPhysicsProxies This is the amount of Adows (Also called Actors or Proxies) allowed in a physics simulation.
		PhysWorld( const PhysVector3 &GeographyLowerBounds, const PhysVector3 &GeographyUpperbounds, const unsigned short int &MaxPhysicsProxies=1024);

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
    	/// @brief This is the preferred way to throw an exception currently
    	/// @details This will log the Message, and will throw an exception with the Message included. Currently this supports all the Data
    	/// type the Log function supports
    	/// @param Message This will be streamed to the log, then used in a thrown exception.
        template <class T> void LogAndThrow(T Message);

        /// @brief Retrieves the Current Window Title
    	/// @details This gets the texts that the engine has stored for use in the title bar
    	/// @return This returns a String Containing the Window Title
        std::string GetWindowName();
        /// @brief This can set the the Text in the titlebar
    	/// @details This changes the text in the bar at the top of the game window in windowed mode. Currently the changes only
    	/// works if this function is called before PhysWorld.GameInit but it is our TODO list to fix so it can be changed at anytime.
    	/// @param NewName This is the new text to be used in the titlebar.
        void SetWindowName(const PhysString &NewName);

        /// @brief Retrieves the amount of milliseconds we would like each iteration of the Main Loop to be
    	/// @details In practice hardware performance or timing concerns can cause this goal to be unnaitanable or trivially easy. However, the mainloop will always reduce
    	/// the actual amount of time to 0 when the hardware is overburdened.
    	/// @return This returns a PhysWhole with the current Value
        PhysWhole GetTargetFrameTime();
        /// @brief This sets a new Target Time
    	/// @details This sets a new time for each frame. Each iteration of the game loop will take around this long to run, but rarely exactly this long. Setting this value
        /// Higher can results in power savings (battery life), but setting it too High can cause choppiness. Settings this value higher can result in smoother gameplay, but
        /// set it too high, and system resources could becom completely taxed and power will be wasted.
    	/// @param NewTargetTime The new length of time, in milliseconds.
    	/// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
        void SetTargetFrameTime(const PhysWhole &NewTargetTime);
        /// @brief This sets a new Target Frame Rate
    	/// @details This sets a new time for each frame. This divides 1000 by the NewFrameRate, drops and floating point amount and uses that amount in an call to
    	/// PhysWorld::SetTargetFrameTime. For example a target frame rate of 40 with cause each frame to take 25 milliseconds, and a Framerate of 70 would take 14 ms
    	/// @param NewTargetTime The new desired frame rate.
    	/// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
        void SetTargetFrameRate(const PhysWhole &NewFrameRate);

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
		void MoveCamera(const PhysVector3 &Position, const PhysVector3 &LookAt);

        /// @brief This creates the game window and starts the game.
        /// @param CallMainLoop should the main loop be called
        /// @details Prior to this all of the physics and graphical object containers should have been loaded and prepared for use. There should be
        /// minimal delay from the time you call this and the game actually begins.
        /// This will automatically call the Main Loop unless passed falsed.
        void GameInit(const bool &CallMainLoop=true );

        /// @brief This Function house the main loop
        /// @details By default this is called from the function PhysWorld.GameInit() this is were the bulk of the simulation is ran from, see @ref mainloop1
        void MainLoop();

        /// @brief Performs all the items that would normally be performed during the game loop
        /// @param PreviousFrameTime This is the amount of time that has elapsed since this function was last called, so that physics can be done properly.
        /// @details This simply calls: DoMainLoopPhysics, DoMainLoopInputBuffering, DoMainLoopWindowManagerBuffering, DoMainLoopRender. This is
        /// useful for anyone wants to use as little of the existing main loop structure as possible, or does not want to run a certain Items
        /// each iteration of the main loop.
        void DoMainLoopAllItems(const PhysReal &PreviousFrameTime);

        /// @brief Increments physics by one step
        /// @param TimeElapsed This is the amount of time that has elapsed since this function was last called, required for proper physics
        /// @details Currently one step is about 1/60 of a second. This function is automatically called in the main loop if a Pre/Post-Physics Callback is
        /// set. This is the second step in the main loop chain of events. This is where we expect the majority of our collision events to come
        /// from although it is concievable that a game could manually insert those manually. This will not delete events it places
        /// in the event manager, that is the responsibility of the code that pulls out the event out.
        void DoMainLoopPhysics(const PhysReal &TimeElapsed);

        /// @brief Gathers user input from the OS and places events in the event manager
        /// @details This this is automatically called during the mainloop if you have set a Pre/PostInput callback. This will not delete events it places
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

        /// @brief Adds a location for graphical resources.
        /// @details This function will add a location on the disk to find files needed to create and
        /// manipulate graphical objects.
        /// @param Location The location on the file system the resource can be found.
        /// @param Type The kind of file system the location can be found in. @n
        /// Options are: filesystem, zip.
        /// @param Group The name of the group the resources at this location belong to.
        /// @param recursive Whether or not to search sub-directories.
        void AddResourceLocation(const PhysString &Location, const PhysString &Type, const PhysString &Group, const bool &recursive=false);

        /// @brief Prepares the resource for use.
        /// @details This function can be thought of as a preloader.  This will prepare the defined
        /// resource located on the disk for use.
        /// @param Name Name of the file/resource to be 'prepared'.
        /// @param Type The type of resource that the file is. @n
        /// Options are: Font, GpuProgram, HighLevelGpuProgram, Material, Mesh, Skeleton, Texture.
        /// @param Group Name of the group the resource belongs to.
        void DeclareResource(PhysString Name, PhysString Type, PhysString Group);

        /// @brief Makes a resource group ready to use.
        /// @details After adding all of your resources and declaring them as nessessary, this function
        /// is the final step.  After calling this function any and all resources within the defined group
        /// will be ready to use.  Do not initialize any more groups then you need to however, as that will
        /// take up memory and drop performance.
        /// @param Name Name of the resource group.
        void InitResourceGroup(PhysString Name);

        /// @brief Sets the gravity.
        /// @details Sets the strength and direction of gravity within the world.
        /// @param pgrav Vector3 representing the strength and direction of gravity.
        void SetGravity(PhysVector3 pgrav);

		/// @brief This is a point to the default Call BackManager
        /// @details All the callbacks that the main loop and the rest of physgame use are will be found in the callback manager point to by this.
		PhysWorldCallBackManager* CallBacks;

        /// @brief This is the default pointer to the Event Manager.
        /// @details This is the Event manager that all physworld members will place any events into.
		PhysEventManager* Events;
};
#endif
