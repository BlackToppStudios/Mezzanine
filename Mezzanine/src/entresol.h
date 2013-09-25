//© Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _entresol_h
#define _entresol_h

/**
 @mainpage Mezzanine
 The Mezzanine Engine isn't really an engine. It is glue holding other code and libraries together. It is a series of other open source libraries (sometimes
 less user friendly, and almost always more sophisticated) that are held together in a single API that will be stable after its 1.0 release. Others have
 attempted to do things like this before. Usually simple mistakes are made along the way that have huge negative impacts later in the design. A common mistake
 code is to copy and paste from open source projects with no plans for maintainability. This means that those projects get all the features and bugs
 from when they copied, without updates, new features and bugfixes since they started integrating the code. We too have copy and pasted, however we are
 keeping it up to date, and have a plan for upgrading each component we have brought in.

 There is no simple way to guarantee access to the most up to date libraries, this means there will meaningful work on integration. Many Linux distributions try
 to keep the most up to date shared libraries around. What happens when they don't ship what you need, ship something broken or an older library. What about
 Windows and Mac OS X which make no attempt to keep these kinds of software up to date. What if you do manage to get the software keep it up to date. After that you
 still have to work on confusing compiler and linker options. IDEs like QT Creator, Code::Blocks or Visual Studio aren't going to set that up for you. The
 Mezzanine Engine project depends on a dozen or more libaries, so would your project if it had high performance 3d graphics, easy to use 3d physics integrated
 with the graphics and 3d positional audio ready to run on Windows, Linux and Mac OS X. We are figuring it out once, getting it right ( if we aren't, tell us and
 we will fix it ), so it doesn't need to be done again and again, for each game.

 This is still in heavy development and is not in usable condition for all systems. Currently the synchronization of 3d graphics and physics works great. Some of
 the more advanced features of both physics and graphics to do not yet work. There are a number of other features that are in varied states of development. XML,
 User Interface, Lua Scripting, 3d Audio, Multithreading, Resource management...

 If we do our jobs right this will save time and effort making, updating and porting games between a variety of platforms. If you link only
 against this library, not a single line of your standard compliant C++ code should need to change between platforms. At this early stage we are proving the
 concept with "Catch!" our first sample game. It Currently runs on Linux, Windows and Mac OS X with an Identical
 codebase. When we are done with "Catch!" We want it to have one codebase (with no messy \#IFDEFs in game code for compatibility), and downloadable in the Iphone
 app store, in the Android Marketplace, on the PS3, Wii, downloadable on Steam, and in a variety of linux repositories.

 @section doxygen This Documention
 This documentation is automatically extracted from the source code of the Mezzanine. It is intended as a reference to describe the Mezzanine API. We hope that
 people with moderate or advanced C++ experience will be able to use this to increase their knowledge and get the most out of the Mezzanine. This is not a
 tutorial and is not likely to helpful to novice C++ developers. For more information please see the following:
 - An automatically generated list of issues with this documentation - http://mezzanineapi.github.io/DoxygenWarnings.txt
 - The BlackTopp Studios Inc. website - http://www.blacktoppstudios.com/
 - The Mezzanine Github page - https://github.com/BlackToppStudios/Mezzanine

 If you want to know more about Doxygen please see: http://www.stack.nl/~dimitri/doxygen/

 @section enginestructure Engine Structure
  The engine is laid out in a variety of classes of a few generalized roles.

  Some classes are focal points for functionality and internally unify a number of components to do what they do. These managers sometimes conceal a large amount
  of complexity and try to present a concise API. Managers ( Mezzanine::ManagerBase instances ) generally have a rigid place in the structure of the
  running program. This is where are the logic that says how something is done exists, for example "how does something get drawn on the screen?" or
  "how do you detect if two objects are colliding?". Managers are owned either by the Mezzanine::Entresol or a Mezzanine::World
  depending on their roles and technical limitations.

  The Mezzanine::Threading::FrameScheduler is responsible for running the work each task requires. Most managers mantain one or more of
  Mezzanine::Threading::iWorkUnit implementations/instances to do work each frame. The FrameScheduler performs some heuristics and manages when and where
  the workunits will be run. Generally these are created early in the lifetime of a game and destroyed near the end.

  Other classes are more similar to data, things like the @ref Mezzanine::Vector3 and the @ref Mezzanine::Quaternion. You can create these things whenever you like
  and generally have good support for integrating into a various parts of the system. In these classes there is a good deal of math and conversion routines. These
  are commonly passed around by more sophisticated classes for a variety of reasons. Some of these are small enough to pass around value and some are not.

  There are some classes that are somewhere between data and singular large-scale aggregration of functionality in managers. The functionality is carefully controlled
  and most commonly created, deleted and otherwised managed through managers. Many of these classes like the @ref Mezzanine::Light or @ref Mezzanine::ActorRigid are
  present inside the game simulation as well as having a presence as in the simulation visible to the user.

  Some classes are that really are just convient places to put functions, things like the @ref Mezzanine::ActorRigidDeSerializer and the @ref Mezzanine::StringTool are good
  examples of these logical grouping of functions.

  @subsection managers Managers

   The canonical source for the list of managers is the ManagerBase::ManagerType enum.

   A manager is simply a logical grouping of functionality. A number of them exist covering all the major functionality that a game using the the Mezzanine engine would
   need in most situations.

   All Managers inherit from ManagerBase. Before the switch to the  all support the basic callback and priority functionality because of this.

            // Entresol Managers       // Namespaces
            MT_AudioManager = 1,       // Audio
            MT_AnimationManager,       // Graphics
            MT_CollisionShapeManager,  // Physics
            MT_GraphicsManager,        // Graphics
            MT_EventManager,           // Mezzanine
            MT_InputManager,           // Input-
            MT_LogManager,             // Mezzanine
            MT_MaterialManager,        // Graphics
            MT_MeshManager,            // Graphics
            MT_NetworkManager,         // Network
            MT_ResourceManager,        // Resource
            MT_ScriptingManager,       // Scripting
            MT_UIManager,              // UI

            // World Managers          // Namespaces
            MT_ActorManager = 100,     // Mezzanine
            MT_AreaEffectManager,      // Mezzanine
            MT_CameraManager,          // Graphics
            MT_PagingManager,          // Paging
            MT_PhysicsManager,         // Physics
            MT_SceneManager,           // Graphics
            MT_SoundScapeManager,      // Audio
            MT_TerrainManager,         // Mezzanine
            MT_VehicleManager,         // Mezzanine

   @subsubsection managerlist A list of Managers in the Mezzanine
    - @ref Mezzanine::ActorManager - Store and allows retrieval of Actors, when an actor is in this it is part of the game simulaiton
    - @ref Mezzanine::AudioManager - Play, stop and otherwise work with sound and music.
    - @ref Mezzanine::CameraManager - Add/remove Multiple camera's from the world
    - @ref Mezzanine::CollisionShapeManager - Store/create/delete shapes that can be used for physics and collision detection
    - @ref Mezzanine::EventManager - Get messages and events from the operating system (and user, but that will change)
    - @ref Mezzanine::GraphicsManager - Adjust Resolution, refresh rate, color depth and thing like that.
    - @ref Mezzanine::MeshManager - Manage Meshes that can be rendered to the the screen.
    - @ref Mezzanine::NetworkManager - Manages the transmission and retreival of network traffic.
    - @ref Mezzanine::PhysicsManager - Control settings that have to do with the physics simulation.
    - @ref Mezzanine::ResourceManager - Loads Files and locate them in an efficient way.
    - @ref Mezzanine::SceneManager - Control Lighting, skyboxes, particle effects etc...
    - @ref Mezzanine::TimerManager - Create and delete a variety of timers.
    - @ref Mezzanine::TerrainManager - Work with terrain components like height maps.
    - @ref Mezzanine::UIManager - Create and manage buttons, scrollbars and other widgets.

    @subsubsection The World, A manager of managers
        A thorough description needs to go here

  @subpage mainloop1 "Main Loop Flow"

  @ref Mezzanine::World "World - It integrates everything"

  @ref Mezzanine::EventManager "Events - Handling messages, event and interupts from the outisde"

  @subpage actorcontainer1 "Actor Container - Keeping track of our in game objects"

 @section Types Data Types

  @subsection CompoundTypes Compound Data Type

   @ref Mezzanine::ColourValue

   @ref Mezzanine::MetaCode "Mezzanine::MetaCode - A unit of user input"

   @ref Mezzanine::Plane

   @ref Mezzanine::Quaternion

   @ref Mezzanine::Ray

   @ref Mezzanine::Transform

   @ref Mezzanine::Vector2

   @ref Mezzanine::Vector3

   @ref Mezzanine::Vector3WActor

  @subsection SimpleDataTypes Simple Data Types

   @ref Mezzanine::Character

   @ref Mezzanine::Integer

   @ref Mezzanine::Real

   @ref Mezzanine::String

   @ref Mezzanine::Whole

   @par There are also a variety of enumerations. Aspect Ratio...
   @todo Fill out the Simple Data Types with all the enumerations.

 @section Sophisticated Sophisticated Systems

  @ref Mezzanine::ActorBase "Actors - Items in the world"

  @ref Mezzanine::EventBase

  @ref Mezzanine::GraphicsManager

  @ref XMLManual

  @ref LuaManual

  @ref Serialization

  @ref ThreadingManual

 @section Licensing Licensing Information

  @ref GPLLicense "Mezzanine GNU Public License 3"

  @ref OriginalpugixmlLicense "XML MIT License"



 @page GPLLicense Mezzanine GNU Public License 3
  Here is a simple summary of the license terms. It is included at the top of every
  source and header file in the engine:
  \n \n
  @verbatim
  © Copyright 2010 - 2011 BlackTopp Studios Inc.
   This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.

   The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'

   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
  @endverbatim
  \n \n
  Here is the contents of gpl.txt that comes in the docs/ folder of with every
  complete repository of the engine:
  \n \n
  @verbinclude gpl.txt
*/

//And now for actual source code!
#include "stringtool.h"
#include "crossplatform.h"
#include "eventbase.h"
#include "exception.h"
#include "enumerations.h"
#include "datatypes.h"
#include "vector3.h"
#include "managerbase.h"
#include "singleton.h"

#include "Threading/dagframescheduler.h"

namespace Mezzanine
{
    // Forward declarations
    class ActorManager;
    class AreaEffectManager;
    class EventManager;
    class CameraManager;
    class ResourceManager;
    class NetworkManager;
    class ManagerFactory;
    namespace Audio
    {
        class AudioManager;
        class SoundScapeManager;
    }
    namespace Graphics
    {
        class GraphicsManager;
        class MeshManager;
        class SceneManager;
    }
    namespace Input
    {
        class InputManager;
    }
    namespace Physics
    {
        class PhysicsManager;
        class CollisionShapeManager;
        class ManagerConstructionInfo;
    }
    namespace UI
    {
        class UIManager;
    }
}

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

    class ParticleFXPlugin;
}

///////////////////////////////////////////////////////////////////////////////
/// @namespace Mezzanine
/// @brief The bulk of the engine components go in this namspace
/// @details This is where imporant classes like World, GraphicsManager, and
/// The eventmanager reside. For more detailed usage information please refer
/// to individual class documentation or the @ref index main page.
///////////////////////////////////////
namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Entresol
    /// @headerfile entresol.h
    /// @brief This is the main entry point for the entire library.
    /// @details The entresol coordinates and integrates all the underlying subsystems,
    /// Currently Ogre3d is used for 3d Graphics, Bullet is used for physics, and
    /// SDL is used for user input and window management. Games will need a
    /// container for all the playing pieces. It makes sense to tie all of this
    /// functionality into one core object.
    ///////////////////////////////////////
    class MEZZ_LIB Entresol : public Singleton<Entresol>
    {
        public:
            typedef std::map<String,ManagerFactory*> ManagerFactoryMap;
            typedef ManagerFactoryMap::iterator ManagerFactoryIterator;
            typedef ManagerFactoryMap::const_iterator ConstManagerFactoryIterator;
        private:
            /// @internal
            /// @brief The core structure responsible for our multi-threaded main loop.
            Threading::FrameScheduler WorkScheduler;

            //Used by the constructors
            /// @internal
            /// @brief This is called by most of the constructors so that the is one unified place to have all the settings made.
            /// @param PhysicsInfo All the info needed to initialize the physics subsystem.
            /// @param SceneType This is the type of Scene Manager to be created.
            /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially othe low level resources.
            /// @param LogFileName This is the place that log messages get sent to. This is relative to the working directory of the application/game.
            /// @param ManagerToBeAdded This is a vector of manager pointers that will be used instead of creating the default ones
            void Construct( const Physics::ManagerConstructionInfo& PhysicsInfo,
                            const String& SceneType,
                            const String& EngineDataPath,
                            const String& LogFileName,
                            const std::vector <ManagerBase*>& ManagerToBeAdded );

            /// @internal
            /// @brief Used to intialize from XML
            /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially othe low level resources.
            /// @param ArchType Should This be looking for raw or zip or whatever kind of files.
            /// @param InitializerFile The Mezzanine MXI file to use to initialize the engine.
            void ConstructFromXML(  const String& EngineDataPath,
                                    const ArchiveType ArchType,
                                    const String& InitializerFile );

            /// @brief Perform a series of checks that could change on certain system or from certain codechanges to alert us to any problems early.
            void SanityChecks();

            bool VerifyManagerInitializations();

            /// @internal
            /// @brief This is a map containing all the registered manager factories.
            ManagerFactoryMap ManagerFactories;
            /// @internal
            /// @brief This is a listing of the priority and the Manager, and a pointer to the manager.
            std::list< ManagerBase* > ManagerList;

            /// @internal
            /// @brief Used to track Ogre specific details for the statically linked Particle plugin
            Ogre::ParticleFXPlugin* SubSystemParticleFXPlugin;

            //Used to break the mainloop
            Int32 ManualLoopBreak;

            /// @internal
            /// @brief Responsible for asynchronously flushing the logs to disk (or wherever they sync to).
            Threading::LogAggregator* Aggregator;

            /// @internal
            /// @brief Create and direct streams used for logging that are not part of the FrameScheduler
            /// @details This sets the name of the Ogre log file (so must be called after the SetupOgre
            /// function is called). This will be changed in the future to change/set the framescheduler
            /// log, in the meantime its only interaction with the framescheduler is adding a
            /// LogAggregator WorkUnit.
            /// @param LogFileName The name of the file to log to. Some Subsystems will use this in addition to some other descriptor
            void SetupLogging(const String& LogFileName);
            /// @internal
            /// @brief Closes log files/streams and removes any WorkUnits from Scheduling dedicated to logging.
            void DestroyLogging();

            /// @internal
            /// @brief If not already created create an instance of Ogre.
            void SetupOgre();
            /// @internal
            /// @brief Do any teardown required for Ogre, then destroy Ogre.
            void DestroyOgre();

        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Creation and Deletion methods

            /// @brief Initializer file constructor.
            /// @details This function expects an ".mxi" (Mezzanine XML Initializer) file.
            /// If the file provided is not one of this type this function will throw an exception. @n @n
            /// When initializing factories in the XML file this constructor does not initialize any additional manager factories, so if they are called this will throw an exception.
            /// @param EngineDataPath The directory where engine specific data resides. This is where it will search for the specified initializer file.
            /// @param ArchType The type of archive at the path provided.
            /// @param InitializerFile The file that describes how to initialize Mezzanine.
            Entresol(const String& EngineDataPath, const ArchiveType ArchType, const String& InitializerFile = "Mezzanine.mxi");
            /// @brief Factory and initializer file constructor.
            /// @details This function expects an ".mxi" (Mezzanine XML Initializer) file.
            /// If the file provided is not one of this type this function will throw an exception. @n @n
            /// Also default factories are already added and thus do not need to be included in the Factory vector.
            /// @param CustomFactories A vector containing the additional factories to be registered before initializing the engine.
            /// @param EngineDataPath The directory where engine specific data resides. This is where it will search for the specified initializer file.
            /// @param ArchType The type of archive at the path provided.
            /// @param InitializerFile The file that describes how to initialize Mezzanine.
            Entresol(std::vector<ManagerFactory*>& CustomFactories, const String& EngineDataPath, const ArchiveType ArchType, const String& InitializerFile = "Mezzanine.mxi");
            /// @brief Descriptive constructor With Manager Pointers
            /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the world.
            /// @param PhysicsInfo All the info needed to initialize the physics subsystem.
            /// @param SceneType A cue to the scenemanager as to how rendering should occur.
            /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially other low level resources.
            /// @param LogFileName This is the place that log messages get sent to.
            Entresol(  const Physics::ManagerConstructionInfo& PhysicsInfo,
                       const String& SceneType,
                       const String& EngineDataPath,
                       const String& LogFileName = "Mezzanine.log" );
            /// @brief Descriptive constructor
            /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the entresol.
            /// This constructor provides no default arguments, but allows for maximum customization. In addition to everything the other
            /// constructors this one can accept a vector of pointers to managers which will be added.
            /// @param PhysicsInfo All the info needed to initialize the physics subsystem.
            /// @param PluginsFileName The filename of the plugins file to be loaded. This is relative to the EngineDataPath.
            /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially othe low level resources.
            /// @param LogFileName This is the place that log messages get sent to.
            /// @param SceneType A cue to the scenemanager as to how rendering should occur.
            /// @param ManagerToBeAdded This is a vector of manager pointers that will be used instead of creating new ones.
            Entresol(  const Physics::ManagerConstructionInfo& PhysicsInfo,
                       const String& SceneType,
                       const String& EngineDataPath,
                       const String& LogFileName,
                       const std::vector <ManagerBase*>& ManagerToBeAdded);
            /// @brief Default constructor.
            /// @details This simply performs the same work as the descriptive constructor with some sane, but small, limits. It will give you a entresol which expands for 100 units from the Origin, and only allows 10 Actors.
            /// @warning Do not make a new entresol if one already exists. This can only cause problems.
            Entresol();
            /// @brief Deconstructor.
            /// @details This Tears down all the items created by the entresol, and safely frees any graphical resources, we will also delete any Objects passed into the
            /// entresol by pointer. We will not delete any pointers we pass out (like from the Events from the Event manager).
            ~Entresol();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Pauses all animations, particles, and object movement throughout the world.
            /// @param Pause Pauses the world if true, unpauses if false.
            void PauseWorld(bool Pause);

            ///////////////////////////////////////////////////////////////////////////////
            // Logging

            /// @brief Runtime event and message logging.
            /// @param Message This is what will be streamed to the log
            /// @throws Anything GetLogStream could throw.
            template <class T> void Log(const T& Message)
                { this->LogString(StringTools::ConvertToString(Message)); }
            /// @brief Log String directly with no conversion
            /// @param message The string to log
            /// @throws Anything GetLogStream could throw.
            void LogString(const String& message);
            /// @brief A nearly threadsafe logging sink.
            /// @details If ID is not supplied and the thread is managed by the framescheduler this should be thread safe.
            /// @param ID Defaults to the id of the current thread, but can be used to retrieve the logger for any thread.
            /// @return A Logger that can be used by the thread with given ID or outside of Framescheduling in a non-threadsafe way.
            /// @throws A ParametersRangeException if the thread is not managed by the frame scheduler or it Threading::FrameScheduler::CreateThreads() has not
            /// been called (It creates the ThreadSpecific resources that contain the Loggers).
            Logger& GetLogStream(Threading::Thread::id ID = Threading::this_thread::get_id());

            /// @internal
            /// @brief This is used to asynchronously handle log messages.
            Threading::LogAggregator* GetLogAggregator();

            ///////////////////////////////////////////////////////////////////////////////
            // Timing system methods

            /// @brief This sets a new Target Frame Rate
            /// @details This sets a new time for each frame. This divides 1000 by the NewFrameRate, drops and floating point amount and uses that amount in an call to
            /// Entresol::SetTargetFrameTime. For example a target frame rate of 40 with cause each frame to take 25 milliseconds, and a Framerate of 70 would take 14 ms
            /// @param NewFrameRate The new desired frame rate.
            /// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
            void SetTargetFrameRate(const Whole NewFrameRate);
            /// @brief This sets a new target time in milliseconds.
            /// @details This sets a new time for each frame. Each iteration of the game loop will take around this long to run, but rarely exactly this long. Setting this value
            /// Higher can results in power savings (battery life), but setting it too High can cause choppiness. Settings this value higher can result in smoother gameplay, but
            /// set it too high, and system resources could becom completely taxed and power will be wasted.
            /// @param NewTargetTime The new length of time, in milliseconds.
            /// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
            void SetTargetFrameTimeMilliseconds(const Whole NewTargetTime);
            /// @brief This sets a new target time microseconds.
            /// @details This sets a new time for each frame. Each iteration of the game loop will take around this long to run, but rarely exactly this long. Setting this value
            /// Higher can results in power savings (battery life), but setting it too High can cause choppiness. Settings this value higher can result in smoother gameplay, but
            /// set it too high, and system resources could becom completely taxed and power will be wasted.
            /// @param NewTargetTime The new length of time, in microseconds.
            /// @warning Setting vary low or very High values could cause unknown errors, This is on our todo list of issues to fix.
            void SetTargetFrameTimeMicroseconds(const Whole NewTargetTime);
            /// @brief Retrieves the amount of milliseconds we would like each iteration of the Main Loop to be.
            /// @details In practice I've done some more testing, started out digging through the code to see if there was anything wrong, there seemed to be random extra words inside our code that the compiler didn't mind that shouldn't be there.  like there was a " Schedule : " right after a line where a pointer was initialized.  I only found such occurrences in gamebase.cpp though, removed them and did two tests.  First test I was just randomly clicking and then I inadvertently threw the metal sphere down into the abyss, the sphere was really close to the camera when this happened, about to go over.  Second time I ran the game trying to click stuff, I simply couldn't click anything.  I even tried zooming in on the robots, although I can only get so close to them before I have to tamper with the camera, which I haven't done yet, but there were no results with the Robots.  The third time I was successfully able to stop two spheres and move them around the platform quite smoothly.  These spheres were slightly further away from the camera compared to the first run.  I stopped the wooden sphere and the second metal sphere.  I included a screen shot and a copy of my log for the third run.  The spheres in the screenshot are both in a rest position.dware performance or timing concerns can cause this goal to be unnaitanable or trivially easy. The main loop with actually
            /// pause execution until this amount of time is reach is main loop iteration, However, the mainloop will always skip waiting if hardware is overburdened.
            /// @return This returns a Whole with the current value in milliseconds.
            Whole GetTargetFrameTimeMilliseconds() const;
            /// @brief Retrieves the amount of milliseconds we would like each iteration of the Main Loop to be.
            /// @details In practice I've done some more testing, started out digging through the code to see if there was anything wrong, there seemed to be random extra words inside our code that the compiler didn't mind that shouldn't be there.  like there was a " Schedule : " right after a line where a pointer was initialized.  I only found such occurrences in gamebase.cpp though, removed them and did two tests.  First test I was just randomly clicking and then I inadvertently threw the metal sphere down into the abyss, the sphere was really close to the camera when this happened, about to go over.  Second time I ran the game trying to click stuff, I simply couldn't click anything.  I even tried zooming in on the robots, although I can only get so close to them before I have to tamper with the camera, which I haven't done yet, but there were no results with the Robots.  The third time I was successfully able to stop two spheres and move them around the platform quite smoothly.  These spheres were slightly further away from the camera compared to the first run.  I stopped the wooden sphere and the second metal sphere.  I included a screen shot and a copy of my log for the third run.  The spheres in the screenshot are both in a rest position.dware performance or timing concerns can cause this goal to be unnaitanable or trivially easy. The main loop with actually
            /// pause execution until this amount of time is reach is main loop iteration, However, the mainloop will always skip waiting if hardware is overburdened.
            /// @return This returns a Whole with the current value in microseconds.
            Whole GetTargetFrameTimeMicroseconds() const;

            /// @brief Gets the amount of time since the last time the last frame took to run.
            /// @details This returns, in milliseconds the amount of time since the frame started.
            /// @return This returns a whole number which can be used to aid in the timimg of various algorithms.
            Whole GetLastFrameTimeMilliseconds() const;
            /// @brief Gets the amount of time since the last time the last frame took to run.
            /// @details This returns, in microseconds the amount of time since the frame started.
            /// @return This returns a whole number which can be used to aid in the timimg of various algorithms.
            Whole GetLastFrameTimeMicroseconds() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization

            /// @brief This initializes all managers and worlds currently in the Entresol.
            /// @param CallMainLoop Should the main loop be called.
            void EngineInit(const bool& CallMainLoop = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Main Loop

            /// @brief Gets the core structure responsible for scheduling work in the Entresol main loop.
            /// @return Returns a reference to the FrameScheduler being used by this Entresol.
            Threading::FrameScheduler& GetScheduler();

            /// @brief This Function house the main loop.
            /// @details If using this you don't need to worry about initialization of managers or other pre main loop items.
            void MainLoop();

            /// @brief Initialize any default managers and any added after construction. This should be called before DoOneFrame()
            void PreMainLoopInit();

            /// @brief Run one frame
            /// @details This should only be called after Managers and other Pre Main loop items
            void DoOneFrame();

            /// @brief This makes the main loop end after it's current frame (or cancels a previous attempt to do so).
            /// @param Break If true this will break the main loop after the next iteration or the end of the current one. If false the main loop will just keep running.
            /// @details If called while not in the main loop, it will affect the next main loop iteration.
            /// This function is thread safe and can be called from any work unit at any time.
            void BreakMainLoop(Bool Break = true);

            /// @brief How many frames have elasped?
            /// @return A Whole containing the currect 0 based frame number.
            Whole GetFrameCount() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Factory Management

            /// @brief Adds/registers a manager factory with this Entresol, allowing it to be constructed through this API.
            /// @param ToBeAdded The manager factory to be added.
            void AddManagerFactory(ManagerFactory* ToBeAdded);
            /// @brief Removes a manager factory from this Entresol.
            /// @param ToBeRemoved A pointer to the manager factory that is to be removed.
            void RemoveManagerFactory(ManagerFactory* ToBeRemoved);
            /// @brief Removes a manager factory from this Entresol.
            /// @param ImplName The name of the manager implementation created by the factory to be removed.
            void RemoveManagerFactory(const String& ImplName);
            /// @brief Removes and destroys a manager factory in this Entresol.
            /// @param ToBeRemoved A pointer to the manager factory that is to be removed and destroyed.
            void DestroyManagerFactory(ManagerFactory* ToBeRemoved);
            /// @brief Removes and destroys a manager factory in this Entresol.
            /// @param ImplName The name of the manager implementation created by the factory to be removed and destroyed.
            void DestroyManagerFactory(const String& ImplName);
            /// @brief Destroys all manager factories in this Entresol.
            /// @warning The destruction of manager factories should only be done after the corresponding managers have been destroyed, otherwise this will cause an exception.
            void DestroyAllManagerFactories();
            /// @brief Adds all the default manager factories provided by the engine to the Entresol.
            void AddAllEngineDefaultManagerFactories();

            ///////////////////////////////////////////////////////////////////////////////
            // Upper Management

            /// @brief Creates a new manager.
            /// @param ManagerImplName The name of the manager implementation to create.
            /// @param Params A list of name-value pairs for the params that are to be used when creating the manager.
            /// @param AddToEntresol Whether or not to add the created manager to the Entresol after creation.
            /// @return Returns a pointer to the created manager.
            ManagerBase* CreateManager(const String& ManagerImplName, NameValuePairList& Params, bool AddToEntresol = true);
            /// @brief Creates a new manager.
            /// @param ManagerImplName The name of the manager implementation to create.
            /// @param XMLNode An XML node containing all construction and initialization info for the manager to be created.
            /// @param AddToEntresol Whether or not to add the created manager to the Entresol after creation.
            /// @return Returns a pointer to the created manager.
            ManagerBase* CreateManager(const String& ManagerImplName, XML::Node& XMLNode, bool AddToEntresol = true);
            /// @brief Destroys a manager.
            /// @param ToBeDestroyed The manager to be destroyed.
            void DestroyManager(ManagerBase* ToBeDestroyed);
            /// @brief Destroys all managers currently in the Entresol.
            /// @warning Do not call this in anything that is run during the main loop.  If you do you will have a bad time.
            void DestroyAllManagers();

            /// @brief This adds a manager, in the correct order, to the list that the Entresol calls on.
            /// @param ManagerToAdd The pointer to the manager to be added.
            void AddManager(ManagerBase* ManagerToAdd);
            /// @brief This removes a manager by finding the matching pointer.
            /// @param ManagerToRemove A pointer to the manager to be removed.
            void RemoveManager(ManagerBase* ManagerToRemove);
            /// @brief This removes a manager of a specific type from the list
            /// @param ManagersToRemoveType The ManagerBase::ManagerTypeName of the manager to remove.
            /// @param WhichOne If not removing the first/only manager of the given type, which one by count are you erasing.
            void RemoveManager(const ManagerBase::ManagerType ManagersToRemoveType, UInt16 WhichOne = 0);
            /// @brief This is will find the manager of a given type.
            /// @param ManagersToRemoveType The ManagerBase::ManagerTypeName of the manager to get.
            /// @param WhichOne If not getting the first/only manager of the given type, get one.
            /// @return This returns a pointer to a ManagerBase, or a NULL pointer if no matching manager exists.
            ManagerBase* GetManager(const ManagerBase::ManagerType ManagersToRemoveType, UInt16 WhichOne = 0);

            /// @brief This gets the ActorManager from the manager list.
            /// @param WhichOne If you have multiple ActorManagers this will choose which one to return.
            /// @return This returns a pointer to a ActorManager, or a NULL pointer if no matching manager exists.
            ActorManager* GetActorManager(const UInt16 WhichOne = 0);
            /// @brief This gets the AreaEffectManager from the manager list.
            /// @param WhichOne If you have multiple AreaEffectManagers this will choose which one to return.
            /// @return This returns a pointer to a AreaEffectManager, or a NULL pointer if no matching manager exists.
            AreaEffectManager* GetAreaEffectManager(const UInt16 WhichOne = 0);
            /// @brief This gets the AudioManager from the manager list.
            /// @param WhichOne If you have multiple AudioManagers this will choose which one to return.
            /// @return This returns a pointer to a AudioManager, or a NULL pointer if no matching manager exists.
            Audio::AudioManager* GetAudioManager(const UInt16 WhichOne = 0);
            /// @brief This gets the CameraManager from the manager list.
            /// @param WhichOne If you have multiple CameraManagers this will choose which one to return.
            /// @return This returns a pointer to a CameraManager, or a NULL pointer if no matching manager exists.
            CameraManager* GetCameraManager(const UInt16 WhichOne = 0);
            /// @brief This gets the CollisionShapeManager from the manager list.
            /// @param WhichOne If you have multiple CollisionShapeManagers this will choose which one to return.
            /// @return This returns a pointer to a CollisionShapeManager, or a NULL pointer if no matching manager exists.
            Physics::CollisionShapeManager* GetCollisionShapeManager(const UInt16 WhichOne = 0);
            /// @brief This gets the EventManager from the manager list.
            /// @param WhichOne If you have multiple EventManagers this will choose which one to return.
            /// @return This returns a pointer to a EventManager, or a NULL pointer if no matching manager exists.
            EventManager* GetEventManager(const UInt16 WhichOne = 0);
            /// @brief This gets the GraphicsManager from the manager list.
            /// @param WhichOne If you have multiple GraphicsManagers this will choose which one to return.
            /// @return This returns a pointer to a GraphicsManager, or a NULL pointer if no matching manager exists.
            Graphics::GraphicsManager* GetGraphicsManager(const UInt16 WhichOne = 0);
            /// @brief This gets the InputManager from the manager list.
            /// @param WhichOne If you have multiple InputManagers this will choose which one to return.
            /// @return This returns a pointer to a InputManager, or a NULL pointer if no matching manager exists.
            Input::InputManager* GetInputManager(const UInt16 WhichOne = 0);
            /// @brief This gets the MeshManager from the manager list.
            /// @param WhichOne If you have multiple MeshManagers this will choose which one to return.
            /// @return This returns a pointer to a MeshManager, or a NULL pointer if no matching manager exists.
            Graphics::MeshManager* GetMeshManager(const UInt16 WhichOne = 0);
#ifdef MEZZNETWORK
            /// @brief This gets the NetworkManager from the manager list.
            /// @param WhichOne If you have multiple NetworkManagers this will choose which one to return.
            /// @return This returns a pointer to a NetworkManager, or a NULL pointer if no matching manager exists.
            NetworkManager* GetNetworkManager(const UInt16 WhichOne = 0);
#endif
            /// @brief This gets the PhysicsManager from the manager list.
            /// @param WhichOne If you have multiple PhysicsManagers this will choose which one to return.
            /// @return This returns a pointer to a PhysicsManager, or a NULL pointer if no matching manager exists.
            Physics::PhysicsManager* GetPhysicsManager(const UInt16 WhichOne = 0);
            /// @brief This gets the SceneManager from the manager list.
            /// @param WhichOne If you have multiple SceneManagers this will choose which one to return.
            /// @return This returns a pointer to a SceneManager, or a NULL pointer if no matching manager exists.
            Graphics::SceneManager* GetSceneManager(const UInt16 WhichOne = 0);
            /// @brief This gets the SoundScapeManager from the manager list.
            /// @param WhichOne If you have multiple SoundScapeManagers this will choose which one to return.
            /// @return This returns a pointer to a SoundScapeManager, or a NULL pointer if no matching manager exists.
            Audio::SoundScapeManager* GetSoundScapeManager(const UInt16 WhichOne = 0);
            /// @brief This gets the ResourceManager from the manager list. These are responsible for reading and writing files on the disk.
            /// @param WhichOne If you have multiple ResourceManagers this will choose which one to return.
            /// @return This returns a pointer to a ResourceManager, or a NULL pointer if no matching manager exists.
            ResourceManager* GetResourceManager(const UInt16 WhichOne = 0);
            /// @brief This gets the UIManager from the manager list.
            /// @param WhichOne If you have multiple UIManagers this will choose which one to return.
            /// @return This returns a pointer to a UIManager, or a NULL pointer if no matching manager exists.
            UI::UIManager* GetUIManager(const UInt16 WhichOne = 0);
    };//Entresol
}//Mezzanine
#endif
