//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _world_h
#define _world_h

/**
 @mainpage Physgame
 The Physgame engine is an abstraction layer between less portable, less user friendly, more sophistciated
 libraries and the game you want to make. If we do our jobs right this will save time and effort making and porting
 games between a variety of platforms. If you link only against this library, not a single line of your
 Standard compliant C++ code should need to change between platforms. At this early stage we are proving the
 concept with "Catch!" our first sample game. It Currently runs on Linux, Windows and Mac OS X with an Identical
 codebase, when we are done with "Catch!" We want it to have one codebase, and downloadable in the Iphone
 app store, on the PS3, Wii downloadon Steam, and in a variety of linux repositories.

 To get the latest news on development checkout: http://gitorious.org/physgame
 Or check the webpage http://www.blacktoppstudios.com

 @section Engine Structure
  @subpage mainloop1 "Main Loop Flow"

  @ref phys::World "World - It integrates everything"

  @ref phys::EventManager "Events - Handling messages, event and interupts from the outisde"

  @subpage actorcontainer1 "Actor Container - Keeping track of our in game objects"

 @section Types Data Types
  @ref phys::Vector3

  @ref phys::Vector3WActor

  @ref phys::Ray

  @ref phys::Real

  @ref phys::Whole

  @ref phys::Quaternion

  @ref phys::MetaCode

 @section Classes Sophisticated Data Types
  @ref phys::ActorBase "Actors - Items in the world"

  @ref phys::EventBase

  @ref phys::GraphicsManager

 @section Optional Optional Engine Components
  @ref phys::WorldQueryTool
*/

//Includes and Forward Declarations
#include "crossplatform.h"
#include "eventbase.h"
#include "datatypes.h"
#include "vector3.h"
#include "scenemanager.h"
namespace phys
{
    class EventManager;
    class CameraManager;
    class PhysicsManager;
    class GraphicsManager;
    class SoundManager;
    class ResourceManager;
    //namespace UI
    //{
        class UIManager;
    //}
}
#include <list>
#include <string>
#include <sstream>

using namespace std;

//Other forward declarations
//forward Declarations so that we do not need #include "SDL.h"
class SDL_Surface;

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
/// @namespace phys
/// @brief The bulk of the engine components go in this namspace
/// @details This is where imporant classes like World, GraphicsManager, and
/// The eventmanager reside. For more detailed usage information please refer
/// to individual class documentation or the @ref index main page.
///////////////////////////////////////
namespace phys
{
    //Forward Declarations
    class ActorBase;
    class ActorContainerBase;
    class GraphicsManager;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class World
    /// @headerfile world.h
    /// @brief This is the main entry point for the entire library.
    /// @details The physworld coordinates and integrates all the underlying subsystems,
    /// Currently Ogre3d is used for 3d Graphics, Bullet is used for physics, and
    /// SDL is used for user input and window management. Games will need a
    /// container for all the playing pieces. It makes sense to tie all of this
    /// functionality into one world object.
    ///////////////////////////////////////
    class World
    {
        private:
            friend class PhysicsManager;

            //SDL Objects
            SDL_Surface *SDLscreen;
            bool HasSDLBeenInitialized;

            //Try to retrieve ogre settinss
            void LoadOgreSettings();

            //Manages the windows all the graphics will be drawn to
            void CreateRenderWindow();
            void DestroyRenderWindow();

            //Used by the constructors
            /// @internal
            /// @brief This is called by all the constructors so that the is one unified place to have all the settings made.
            /// @param GeographyLowerBounds This is the lower boundary of the phyiscs estimation.
            /// @param GeographyUpperbounds This is the upper boundary of the phyiscs estimation.
            /// @param MaxPhysicsProxies This is an estimation of the limit of bodies in the physics world.
            /// @param SceneManagerName This is the name to be given to the created Scene Manager.
            /// @param SceneType This is the type of Scene Manager to be created.
            /// @param LogFileName This is the place that log messages get sent to.
            /// @param ManagerToBeAdded This is a vector of manager pointers that will be used instead of creating the default ones
            void Construct( const Vector3 &GeographyLowerBounds,
                            const Vector3 &GeographyUpperbounds,
                            const unsigned short int &MaxPhysicsProxies,
                            std::string SceneManagerName,
                            SceneManager::SceneManagerType SceneType,
                            std::string LogFileName,
                            std::vector < ManagerBase* > ManagerToBeAdded);

            void SanityChecks();
            void TestLogger();
            template <class T> void OneLogTest(T Data, string DataType,string Message1 = "Logging and Throwing a ", string Message2 = "Logging a ");

            //Settings for Engine Functionality
            string WindowName;
            Whole TargetFrameLength;
            Whole FrameTime;

            /// @internal
            /// @brief This is a listing of the priority and the Manager, and a pointer to the manager.
            std::list< ManagerBase* > ManagerList;

        protected:
            /// @internal
            /// @brief A pointer to the one and only world
            static World* TheRealWorld;

        public:

            //Ogre objects
            /// @internal
            /// @brief This is the core of the Ogre rendering system.
            /// @details Even thought the largest part of ogre is 3d rendering, it does not belong in the graphics manager. It is also responsible for a
            /// large number other services, such as logging, multithreading, and timing.
            Ogre::Root* OgreRoot;

            /// @internal
            /// @brief
            //Ogre::RenderSystem* OgreRenderSystem;

            /// @internal
            /// @brief
            //Ogre::RenderWindow* OgreGameWindow;

            /// @internal
            /// @brief
            //Ogre::ResourceGrou pManager* OgreResource;

            /// @internal
            /// @brief
            //Ogre::Camera* OgreCamera;

            /// @internal
            /// @brief
            //Ogre::Viewport* OgreViewport;

            /// @internal
            /// @brief
            //Ogre::SceneManager* OgreSceneManager;

        ///////////////////////////////////////////////////////////////////////////////
        // Creation and Deletion methods
        ///////////////////////////////////////

            /// @brief Return a pointer to the world
            /// @return This returns a pointer the one and only phys::World
            static World* GetWorldPointer();

            /// @brief Descriptive constructor With Manager Pointers
            /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the physworld.
            /// @param GeographyLowerBounds_ The lower limiasked Jan 29 '09 at 9:27ts for the size of the physics simulation
            /// @param GeographyUpperbounds_ The Upper limits for the size of the physics simulation
            /// @param MaxPhysicsProxies_ This is the amount of Actors (Also called Proxies) allowed in a physics simulation.
            /// @param LogFileName This is the place that log messages get sent to.
            /// @warning Do not make a new world if one already exists. This can only cause problems
            World(  const Vector3 &GeographyLowerBounds_,
                    const Vector3 &GeographyUpperbounds_,
                    std::string SceneManagerName,
                    SceneManager::SceneManagerType SceneType,
                    const unsigned short int &MaxPhysicsProxies_=1024,
                    std::string LogFileName="Physgame.log" );

            /// @brief Descriptive constructor
            /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the physworld.
            /// This constructor provides no default arguments, but allows for maximum customization. In addition to everything the other
            /// constructors this one can accept a vector of pointers to managers. They will be add
            /// @param GeographyLowerBounds_ The lower limits for the size of the physics simulation
            /// @param GeographyUpperbounds_ The Upper limits for the size of the physics simulation
            /// @param MaxPhysicsProxies_ This is the amount of Actors (Also called Proxies) allowed in a physics simulation.
            /// @param LogFileName This is the place that log messages get sent to.
            /// @param ManagerToBeAdded This is a vector of manager pointers that will be used instead of creating new ones
            /// @warning Do not make a new world if one already exists. This can only cause problems.
            World(  const Vector3 &GeographyLowerBounds_,
                    const Vector3 &GeographyUpperbounds_,
                    const unsigned short int &MaxPhysicsProxies_,
                    std::string SceneManagerName,
                    SceneManager::SceneManagerType SceneType,
                    const std::string &LogFileName,
                    const std::vector <ManagerBase*> &ManagerToBeAdded);

            /// @brief Default constructor
            /// @details This simply performs the same work as the descriptive constructor with some sane, but small, limits. It will give you a world which expands for 100 units from the Origin, and only allows 10 Adows.
            /// @warning Do not make a new world if one already exists. This can only cause problems
            World();

            /// @brief Deconstructor
            /// @details This Tears down all the items created by the physworld, and safely frees any graphical resources, we will also delete any Objects passed into the
            /// Physworld by pointer. We will not delete any pointers we pass out (like from the Events from the Event manager)
            ~World();

        ///////////////////////////////////////////////////////////////////////////////
        // Logging
        ///////////////////////////////////////
            /// @brief Runtime Event logging Function
            /// @details Be careful with this function, even though it appears to be a template, it does not support every data type. If Physgame is
            /// Compiled as a Shared Object, Dynamic Linked Library, or some other kind of stand alone library It will only support data types
            /// that are called internally, Currently that list includes: string, char, short int, int, long int, unsigned short int, unsigned int
            /// unsigned long int, bool, float, double, long double, wchar_t, size_t, Real, Whole, String, Vector3, RawEvent and MetaCode.
            /// If compiled statically it should support any data type which supports output streams.
            /// @param Message This is what will be streamed to the log
            template <class T> void Log(T Message);

            /// @brief This is the preferred way to throw an exception currently
            /// @details This will log the Message, and will throw an exception with the Message included. Currently this supports all the Data
            /// type the Log function supports
            /// @param Message This will be streamed to the log, then used in a thrown exception.
            template <class T> void LogAndThrow(T Message);

        ///////////////////////////////////////////////////////////////////////////////
        // Window Management
        ///////////////////////////////////////
            /// @brief Retrieves the Current Window Title
            /// @details This gets the texts that the engine has stored for use in the title bar
            /// @return This returns a String Containing the Window Title
            std::string GetWindowName();

            /// @brief This can set the the Text in the titlebar
            /// @details This changes the text in the bar at the top of the game window in windowed mode. It can be changed at anytime.
            /// @param NewName This is the new text to be used in the titlebar.
            void SetWindowName(const String &NewName);

        ///////////////////////////////////////////////////////////////////////////////
        // Timing system methods
        ///////////////////////////////////////
            /// @brief Retrieves the amount of milliseconds we would like each iteration of the Main Loop to be
            /// @details In practice harI've done some more testing, started out digging through the code to see if there was anything wrong, there seemed to be random extra words inside our code that the compiler didn't mind that shouldn't be there.  like there was a " Schedule : " right after a line where a pointer was initialized.  I only found such occurrences in gamebase.cpp though, removed them and did two tests.  First test I was just randomly clicking and then I inadvertently threw the metal sphere down into the abyss, the sphere was really close to the camera when this happened, about to go over.  Second time I ran the game trying to click stuff, I simply couldn't click anything.  I even tried zooming in on the robots, although I can only get so close to them before I have to tamper with the camera, which I haven't done yet, but there were no results with the Robots.  The third time I was successfully able to stop two spheres and move them around the platform quite smoothly.  These spheres were slightly further away from the camera compared to the first run.  I stopped the wooden sphere and the second metal sphere.  I included a screen shot and a copy of my log for the third run.  The spheres in the screenshot are both in a rest position.dware performance or timing concerns can cause this goal to be unnaitanable or trivially easy. The main loop with actually
            /// pause execution until this amount of time is reach is main loop iteration, However, the mainloop will always skip waiting if hardware is overburdened.
            /// @return This returns a Whole with the current Value
            Whole GetTargetFrameTime();

            /// @brief This sets a new Target Time
            /// @details This sets a new time for each frame. Each iteration of the game loop will take around this long to run, but rarely exactly this long. Setting this value
            /// Higher can results in power savings (battery life), but setting it too High can cause choppiness. Settings this value higher can result in smoother gameplay, but
            /// set it too high, and system resources could becom completely taxed and power will be wasted.
            /// @param NewTargetTime The new length of time, in milliseconds.
            /// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
            void SetTargetFrameTime(const Whole &NewTargetTime);

            /// @brief This sets a new Target Frame Rate
            /// @details This sets a new time for each frame. This divides 1000 by the NewFrameRate, drops and floating point amount and uses that amount in an call to
            /// World::SetTargetFrameTime. For example a target frame rate of 40 with cause each frame to take 25 milliseconds, and a Framerate of 70 would take 14 ms
            /// @param NewFrameRate The new desired frame rate.
            /// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
            void SetTargetFrameRate(const Whole &NewFrameRate);

            /// @brief Gets the amount of time since the last time Rendering began.
            /// @details This returns, in milliseconds the amount of time since the frame started (since the last rendering began)
            /// @return This returns a whole number which can be used to aid in the timimg of various algorithms.
            Whole GetFrameTime();

            /// @internal
            /// @brief Sets the amount of time since the last time Rendering began.
            /// @param FrameTime_ This is the amount of time in  milliseconds.
            /// @details This sets, in milliseconds the amount of time since the frame started (since the last rendering began). Don't set this carelessely can screw up a lot of
            /// stuff, for the most part this should be by the rendering manager during the rendering process.
            void SetFrameTime( const Whole &FrameTime_ );

        ///////////////////////////////////////////////////////////////////////////////
        // Main Loop and Initialization
        ///////////////////////////////////////
            /// @brief This creates the game window and starts the game.
            /// @param CallMainLoop should the main loop be called
            /// @details Prior to this all of the physics and graphical object containers should have been loaded and prepared for use. There should be
            /// minimal delay from the time you call this and the game actually begins.
            /// This will automatically call the Main Loop unless passed false.
            void GameInit(const bool &CallMainLoop=true );

            /// @brief This Function house the main loop
            /// @details By default this is called from the function World.GameInit() this is were the bulk of the simulation is ran from, see @ref mainloop1
            void MainLoop();

            /// @brief This commits the log stream to the log
            /// @details This is called automatically during the main loop just before rendering.
            void DoMainLoopLogging();

        ///////////////////////////////////////////////////////////////////////////////
        // Upper Management
        ///////////////////////////////////////
            /// @brief This adds a manager, in the correct order, to the list that the world calls on
            /// @details Internally the world had a list of managers that is sorted by the ManagerBase::Priority. Everytime a manager is added,
            /// the list is searched for the sorted point to insert the manager at.
            /// @param ManagerToAdd The pointer to the manager to be added
            void AddManager(ManagerBase* ManagerToAdd);

            /// @brief This removes a manager by finding the matching pointer.
            /// @details Currently this just iterates through the list looking for the matching pointer, at some future point
            /// this could replaced with more sophisticated algorithm, but for now assume this operates in linear time.
            /// @param ManagerToRemove A pointer to the manager to be removed
            void RemoveManager(ManagerBase* ManagerToRemove);

            /// @brief This removes a manager of a specific type from the list
            /// @details This starts at the beginning (should be the lowest priority)of the list and iterates through looking for a matching type, at some future point
            /// this could replaced with more sophisticated algorithm, but for now assume this operates in linear time.
            /// @param ManagersToRemoveType The ManagerBase::ManagerTypeName of the manager to remove.
            /// @param WhichOne If not removing the first/only manager of the given type, which one by count are you erasing.
            void RemoveManager(const ManagerBase::ManagerTypeName &ManagersToRemoveType, short unsigned int WhichOne);

            /// @brief This is will find the manager of a given type
            /// @details Specifically this will iterate from lowest priority to highest priority, and return a pointer to the first Manager
            /// with a matching type found. If you specify WhichOne, it will the Nth+1 in the list matching the type (kind of like array subscript).
            /// @param ManagersToRemoveType
            /// @param WhichOne If not getting the first/only manager of the given type, get one.
            /// @return This returns a pointer to a ManagerBase, or a NULL pointer if no matching manager exists
            ManagerBase* GetManager(const ManagerBase::ManagerTypeName &ManagersToRemoveType, short unsigned int WhichOne=0);

            /// @brief Changes a Manager's time of execution.
            /// @details Searches through the Manager list and removes any previous entries to the changing manager, and add a new entry in the correct location.
            /// @param ManagerToChange A pointer to the manager that needs to be changed
            /// @param Priority_ the new desire priority/execution order of the Manager
            void UpdateManagerOrder(ManagerBase* ManagerToChange, short int Priority_);

            /// @brief This forces the list of managers to be resorted.
            /// @details This should only need to be called if the Priority attribute of a manager in the list has changed. This sorts the list of managers
            void UpdateManagerOrder();

            /// @brief This gets the ActorManager from the manager list.
            /// @param WhichOne If you have multiple ActorManagers this will choose which one to return.
            /// @return This returns a pointer to a ActorManager, or a NULL pointer if no matching manager exists.
            ActorContainerBase* GetActorManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the CameraManager from the manager list.
            /// @param WhichOne If you have multiple CameraManagers this will choose which one to return.
            /// @return This returns a pointer to a CameraManager, or a NULL pointer if no matching manager exists.
            CameraManager* GetCameraManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the EventManager from the manager list.
            /// @param WhichOne If you have multiple EventManagers this will choose which one to return.
            /// @return This returns a pointer to a EventManager, or a NULL pointer if no matching manager exists.
            EventManager* GetEventManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the GraphicsManager from the manager list.
            /// @param WhichOne If you have multiple GraphicsManagers this will choose which one to return.
            /// @return This returns a pointer to a GraphicsManager, or a NULL pointer if no matching manager exists.
            GraphicsManager* GetGraphicsManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the PhysicsManager from the manager list.
            /// @param WhichOne If you have multiple PhysicsManagers this will choose which one to return.
            /// @return This returns a pointer to a PhysicsManager, or a NULL pointer if no matching manager exists.
            PhysicsManager* GetPhysicsManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the SceneManager from the manager list.
            /// @param WhichOne If you have multiple SceneManagers this will choose which one to return.
            /// @return This returns a pointer to a SceneManager, or a NULL pointer if no matching manager exists.
            SceneManager* GetSceneManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the SoundManager from the manager list.
            /// @param WhichOne If you have multiple SoundManagers this will choose which one to return.
            /// @return This returns a pointer to a SoundManager, or a NULL pointer if no matching manager exists.
            SoundManager* GetSoundManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the ResourceManager from the manager list. These are responsible for reading and writing files on the disk.
            /// @param WhichOne If you have multiple ResourceManagers this will choose which one to return.
            /// @return This returns a pointer to a ResourceManager, or a NULL pointer if no matching manager exists.
            ResourceManager* GetResourceManager(const short unsigned int &WhichOne=0);

            /// @brief This gets the UIManager from the manager list.
            /// @param WhichOne If you have multiple UIManagers this will choose which one to return.
            /// @return This returns a pointer to a UIManager, or a NULL pointer if no matching manager exists.
            UIManager* GetUIManager(const short unsigned int &WhichOne=0);

            /// @brief This is another way to put data in the log.
            /// @details The contents of this will be commited to the log once per frame, just before rendering. Because of that do not
            /// use this for data that is likely to be required to debug something the frame something crashes. however, for other kinds of
            /// debugging data and creating in game logs and recreations, this can be very useful.
            std::stringstream LogStream;
    };
}
#endif
