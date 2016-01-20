// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#include "dox/mainpage.h"

/**
 @page Mezzanine
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
  and most commonly created, deleted and otherwised managed through managers. Many of these classes like the @ref Mezzanine::Graphics::Light or @ref Mezzanine::RigidDebris are
  present inside the game simulation as well as having a presence as in the simulation visible to the user.

  Some classes are that really are just convient places to put functions, things like the @ref Mezzanine::StringTools are good examples of these logical grouping of functions.

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
            MT_PagingManager,          // Paging
            MT_PhysicsManager,         // Physics
            MT_SceneManager,           // Graphics
            MT_SoundScapeManager,      // Audio
            MT_TerrainManager,         // Mezzanine
            MT_VehicleManager,         // Mezzanine

   @subsubsection managerlist A list of Managers in the Mezzanine
    - @ref Mezzanine::ActorManager - Store and allows retrieval of Actors, when an actor is in this it is part of the game simulaiton
    - @ref Mezzanine::Audio::AudioManager - Play, stop and otherwise work with sound and music.
    - @ref Mezzanine::Physics::CollisionShapeManager - Store/create/delete shapes that can be used for physics and collision detection
    - @ref Mezzanine::EventManager - Get messages and events from the operating system (and user, but that will change)
    - @ref Mezzanine::Graphics::GraphicsManager - Adjust Resolution, refresh rate, color depth and thing like that.
    - @ref Mezzanine::Graphics::MeshManager - Manage Meshes that can be rendered to the the screen.
    - @ref Mezzanine::Network::NetworkManager - Manages the transmission and retreival of network traffic.
    - @ref Mezzanine::Physics::PhysicsManager - Control settings that have to do with the physics simulation.
    - @ref Mezzanine::ResourceManager - Loads Files and locate them in an efficient way.
    - @ref Mezzanine::Graphics::SceneManager - Control Lighting, skyboxes, particle effects etc...
    - @ref Mezzanine::TerrainManager - Work with terrain components like height maps.
    - @ref Mezzanine::UI::UIManager - Create and manage buttons, scrollbars and other widgets.

    @subsubsection The World, A manager of managers
        A thorough description needs to go here

  @subpage mainloop1 "Main Loop Flow"

  @ref Mezzanine::World "World - It integrates everything"

  @ref Mezzanine::EventManager "Events - Handling messages, event and interupts from the outisde"

  @subpage actorcontainer1 "Actor Container - Keeping track of our in game objects"

 @section Types Data Types

  @subsection CompoundTypes Compound Data Type

   @ref Mezzanine::ColourValue

   @ref Mezzanine::Input::MetaCode "Mezzanine::MetaCode - A unit of user input"

   @ref Mezzanine::Plane

   @ref Mezzanine::Quaternion

   @ref Mezzanine::Ray

   @ref Mezzanine::Transform

   @ref Mezzanine::Vector2

   @ref Mezzanine::Vector3

  @subsection SimpleDataTypes Simple Data Types

   @ref Mezzanine::Char8

   @ref Mezzanine::Integer

   @ref Mezzanine::Real

   @ref Mezzanine::String

   @ref Mezzanine::Whole

   @par There are also a variety of enumerations. Aspect Ratio...
   @todo Fill out the Simple Data Types with all the enumerations.

 @section Sophisticated Sophisticated Systems

  @ref Mezzanine::WorldObject "WorldObjects - Items in the world"

  @ref Mezzanine::EventBase

  @ref Mezzanine::Graphics::GraphicsManager

  @ref XMLManual

  @ref UIManual

  @ref ScriptingManual

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
  © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#include "datatypes.h"
#include "singleton.h"
#include "stringtool.h"

#include "Resource/resourceenumerations.h"
#include "XML/xml.h"

#ifndef SWIG
    #include "Threading/dagframescheduler.h"
#endif

//Other forward declarations
//forward Declarations so that we do not need #include "SDL.h"
class SDL_Surface;

//forward Declarations so that we do not need #include <Ogre.h>
namespace Ogre
{
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
    class EntresolManager;
    class EntresolManagerFactory;
    class World;
    class WorldManager;
    namespace Physics
    {
        class ManagerConstructionInfo;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Entresol
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
        /// @brief Basic container type for factories that construct known manager types.
        typedef std::map<String,EntresolManagerFactory*>   ManagerFactoryMap;
        /// @brief Basic container type for factories that construct known manager types.
        typedef std::vector<EntresolManagerFactory*>       ManagerFactoryVec;
        /// @brief Iterator type for manager factories stored by this class.
        typedef ManagerFactoryMap::iterator                ManagerFactoryIterator;
        /// @brief Const Iterator type for manager factories stored by this class.
        typedef ManagerFactoryMap::const_iterator          ConstManagerFactoryIterator;
        /// @brief Basic container type for normal manager storage in this class.
        typedef std::list<EntresolManager*>                ManagerContainer;
        /// @brief Iterator type for managers stored by this class.
        typedef ManagerContainer::iterator                 ManagerIterator;
        /// @brief Const Iterator type for managers stored by this class.
        typedef ManagerContainer::const_iterator           ConstManagerIterator;
        /// @brief Container type for passing pre-created managers into the Entresol upon creation.
        typedef std::vector<EntresolManager*>              ManagerVec;
        /// @brief Iterator type for containers of pre-created managers.
        typedef ManagerVec::iterator                       ManagerVecIterator;
        /// @brief Const Iterator type for containers of pre-created managers.
        typedef ManagerVec::const_iterator                 ConstManagerVecIterator;
        /// @brief Basic container type for World storage.
        typedef std::vector<World*>                        WorldContainer;
        /// @brief Iterator type for Worlds stored by this class.
        typedef WorldContainer::iterator                   WorldIterator;
        /// @brief Const Iterator type for Worlds stored by this class.
        typedef WorldContainer::const_iterator             ConstWorldIterator;
    private:
        /// @internal
        /// @brief This is a map containing all the registered manager factories.
        static ManagerFactoryMap ManagerFactories;
        /// @internal
        /// @brief The core structure responsible for our multi-threaded main loop.
        Threading::FrameScheduler WorkScheduler;
        /// @internal
        /// @brief This is a listing of the priority and the Manager, and a pointer to the manager.
        ManagerContainer ManagerList;
        /// @internal
        /// @brief This is a listing of the priority and the Manager, and a pointer to the manager.
        WorldContainer Worlds;
        /// @internal
        /// @brief Used to track Ogre specific details for the statically linked Particle plugin
        Ogre::ParticleFXPlugin* SubSystemParticleFXPlugin;
        /// @internal
        /// @brief Responsible for asynchronously flushing the logs to disk (or wherever they sync to).
        Threading::LogAggregator* Aggregator;
        /// @internal
        /// @brief Internal value used to indicate during the main loop something wants the loop to end.
        Int32 ManualLoopBreak;

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Initialization and Deinitialization Methods

        /// @internal
        /// @brief This is called by most of the constructors so that the is one unified place to have all the settings made.
        /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially othe low level resources.
        /// @param LogFileName This is the place that log messages get sent to. This is relative to the working directory of the application/game.
        /// @param ManagersToBeAdded This is a vector of manager pointers that will be used instead of creating the default ones
        void Construct(const String& EngineDataPath, const String& GraphicsLogFileName, const ManagerVec& ManagersToBeAdded);
        /// @internal
        /// @brief Used to intialize from XML
        /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially othe low level resources.
        /// @param ArchType Should This be looking for raw or zip or whatever kind of files.
        /// @param InitializerFile The Mezzanine MXI file to use to initialize the engine.
        void ConstructFromXML(const String& EngineDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile);

        /// @internal
        /// @brief If not already created create an instance of Ogre.
        void SetupOgre();
        /// @internal
        /// @brief Do any teardown required for Ogre, then destroy Ogre.
        void DestroyOgre();
        /// @internal
        /// @brief Do any teardown required for SDL that may have been missed by other managers.
        void DestroySDL();

        /// @internal
        /// @brief Create and direct streams used for logging that are not part of the FrameScheduler.
        /// @details This sets the name of the Ogre log file (so must be called after the SetupOgre
        /// function is called). This will be changed in the future to change/set the framescheduler
        /// log, in the meantime its only interaction with the framescheduler is adding a
        /// LogAggregator WorkUnit.
        /// @param LogFileName The name of the file to log to.
        void SetupLogging(const String& OgreLogFileName);
        /// @internal
        /// @brief Creates the logging components used by ogre and configures them appropriately.
        /// @param LogFileName The name of the file to log to.
        void SetupOgreLogging(const String& OgreLogFileName);
        /// @internal
        /// @brief Creates the appropriate streams for multi-threaded logging and configures them appropriately.
        void SetupInternalLogging();
        /// @internal
        /// @brief Closes log files/streams and removes any WorkUnits from Scheduling dedicated to logging.
        void DestroyLogging();

        /// @internal
        /// @brief Performs a small series of checks to verify assumptions made by the Mezzanine.
        void SanityChecks();
        /// @internal
        /// @brief Checks if all managers currently stored in this Entresol have been initialized.
        /// @return Returns true if all stored managers have been initialized, false otherwise.
        Boole VerifyManagerInitializations();
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
        Entresol(const String& EngineDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile = "Mezzanine.mxi");
        /// @brief Factory and initializer file constructor.
        /// @details This function expects an ".mxi" (Mezzanine XML Initializer) file.
        /// If the file provided is not one of this type this function will throw an exception. @n @n
        /// Also default factories are already added and thus do not need to be included in the Factory vector.
        /// @param CustomFactories A vector containing the additional factories to be registered before initializing the engine.
        /// @param EngineDataPath The directory where engine specific data resides. This is where it will search for the specified initializer file.
        /// @param ArchType The type of archive at the path provided.
        /// @param InitializerFile The file that describes how to initialize Mezzanine.
        Entresol(ManagerFactoryVec& CustomFactories, const String& EngineDataPath, const Resource::ArchiveType ArchType, const String& InitializerFile = "Mezzanine.mxi");
        /// @brief Descriptive constructor With Manager Pointers.
        /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially other low level resources.
        /// @param LogFileName This is the place that log messages get sent to.
        Entresol(const String& EngineDataPath, const String& LogFileName = "Mezzanine.log" );
        /// @brief Descriptive constructor.
        /// @details This constructor allows for an easier way to define the boundaries for items moving about inside the entresol.
        /// This constructor provides no default arguments, but allows for a lot of customization. In addition to everything the other
        /// constructors this one can accept a vector of pointers to managers which will be added.
        /// @param EngineDataPath The directory where engine specific data (as opposed to game/application data) reside, and it include the plugins file and potentially othe low level resources.
        /// @param LogFileName This is the place that log messages get sent to.
        /// @param ManagersToBeAdded This is a vector of manager pointers that will be used instead of creating new ones.
        Entresol(const String& EngineDataPath, const String& LogFileName, const ManagerVec& ManagersToBeAdded);
        /// @brief Default constructor.
        /// @details This simply performs the same work as the descriptive constructor with some sane defaults.
        /// @warning Do not make a new entresol if one already exists. This can only cause problems.
        Entresol();
        /// @brief Deconstructor.
        /// @details This Tears down all the items created by the entresol, and safely frees any graphical resources, we will also delete any Objects passed into the
        /// entresol by pointer. We will not delete any pointers we pass out (like from the Events from the Event manager).
        ~Entresol();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the core structure responsible for scheduling work in the Entresol main loop.
        /// @return Returns a reference to the FrameScheduler being used by this Entresol.
        Threading::FrameScheduler& GetScheduler();

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
        // Initialization and Deinitialization

        /// @brief This initializes all managers and Worlds currently in the Entresol.
        /// @remarks Worlds must be initialized prior to use.  Passing in "false" should only be done in Tools, Tests, or if additional configuration needs
        /// to be done in between Entresol managers and Worlds being initialized.
        /// @param InitWorlds Whether or not to initialize every World stored in this manager.  If false, then only Entresol managers will be initialized.
        void Initialize(const Boole InitWorlds);
        /// @brief This deinitializeds all managers and Worlds currently in the Entresol.
        void Deinitialize();

        /// @brief Initialize any default managers and any added after construction. This should be called before DoOneFrame()
        void PreMainLoopInit();

        ///////////////////////////////////////////////////////////////////////////////
        // Main Loop

        /// @brief This Function house the main loop.
        /// @details If using this you don't need to worry about initialization of managers or other pre main loop items.
        void MainLoop();
        /// @brief Run one frame
        /// @details This should only be called after Managers and other Pre Main loop items
        void DoOneFrame();

        /// @brief This makes the main loop end after it's current frame (or cancels a previous attempt to do so).
        /// @param Break If true this will break the main loop after the next iteration or the end of the current one. If false the main loop will just keep running.
        /// @details If called while not in the main loop, it will affect the next main loop iteration.
        /// This function is thread safe and can be called from any work unit at any time.
        void BreakMainLoop(Boole Break = true);
        /// @brief How many frames have elasped?
        /// @return A Whole containing the currect 0 based frame number.
        Whole GetFrameCount() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Factory Management

        /// @brief Adds/registers a manager factory with this Entresol, allowing it to be constructed through this API.
        /// @param ToBeAdded The manager factory to be added.
        static void AddManagerFactory(EntresolManagerFactory* ToBeAdded);
        /// @brief Removes a manager factory from this Entresol.
        /// @param ToBeRemoved A pointer to the manager factory that is to be removed.
        static void RemoveManagerFactory(EntresolManagerFactory* ToBeRemoved);
        /// @brief Removes a manager factory from this Entresol.
        /// @param ImplName The name of the manager implementation created by the factory to be removed.
        static void RemoveManagerFactory(const String& ImplName);
        /// @brief Removes and destroys a manager factory in this Entresol.
        /// @param ToBeRemoved A pointer to the manager factory that is to be removed and destroyed.
        static void DestroyManagerFactory(EntresolManagerFactory* ToBeRemoved);
        /// @brief Removes and destroys a manager factory in this Entresol.
        /// @param ImplName The name of the manager implementation created by the factory to be removed and destroyed.
        static void DestroyManagerFactory(const String& ImplName);
        /// @brief Destroys all manager factories in this Entresol.
        /// @warning The destruction of manager factories should only be done after the corresponding managers have been destroyed, otherwise this will cause an exception.
        static void DestroyAllManagerFactories();
        /// @brief Adds all the default manager factories provided by the engine to the Entresol.
        static void AddAllEngineDefaultManagerFactories();

        ///////////////////////////////////////////////////////////////////////////////
        // Upper Management

        /// @brief Creates a new manager.
        /// @param ManagerImplName The name of the manager implementation to create.
        /// @param Params A list of name-value pairs for the params that are to be used when creating the manager.
        /// @param AddToEntresol Whether or not to add the created manager to the Entresol after creation.
        /// @return Returns a pointer to the created manager.
        EntresolManager* CreateManager(const String& ManagerImplName, const NameValuePairList& Params, const Boole AddToEntresol = true);
        /// @brief Creates a new manager.
        /// @param ManagerImplName The name of the manager implementation to create.
        /// @param XMLNode An XML node containing all construction and initialization info for the manager to be created.
        /// @param AddToEntresol Whether or not to add the created manager to the Entresol after creation.
        /// @return Returns a pointer to the created manager.
        EntresolManager* CreateManager(const String& ManagerImplName, const XML::Node& XMLNode, const Boole AddToEntresol = true);
        /// @brief Destroys a manager.
        /// @param ToBeDestroyed The manager to be destroyed.
        void DestroyManager(EntresolManager* ToBeDestroyed);
        /// @brief Destroys all managers currently in the Entresol.
        /// @warning Do not call this in anything that is run during the main loop.  If you do you will have a bad time.
        void DestroyAllManagers();

        /// @brief This adds a manager, in the correct order, to the list that the Entresol calls on.
        /// @note This method will detect if a manager is being double inserted, and will silently fail in such a case.
        /// @param ManagerToAdd The pointer to the manager to be added.
        void AddManager(EntresolManager* ManagerToAdd);
        /// @brief This removes a manager by finding the matching pointer.
        /// @param ManagerToRemove A pointer to the manager to be removed.
        void RemoveManager(EntresolManager* ManagerToRemove);
        /// @brief This is will find the manager of a given type.
        /// @param RetrieveType The type ID of the manager to get.  Use ManagerBase::ManagerType enum values for this.
        /// @param WhichOne If not getting the first/only manager of the given type, get one.
        /// @return This returns a pointer to a ManagerBase, or a NULL pointer if no matching manager exists.
        EntresolManager* GetManager(const Whole RetrieveType, UInt16 WhichOne = 0);
        /// @brief This removes a manager of a specific type from the list.
        /// @param RemoveType The type ID of the manager to remove.  Use ManagerBase::ManagerType enum values for this.
        /// @param WhichOne If not removing the first/only manager of the given type, which one by count are you erasing.
        void RemoveManager(const Whole RemoveType, UInt16 WhichOne = 0);

        ///////////////////////////////////////////////////////////////////////////////
        // World Management

        /// @brief This creates a world and adds it to the World List.
        /// @param WorldName String name of the world.
        /// @return Returns a pointer to the created world.
        World* CreateWorld(const String& WorldName);
        /// @brief This creates a world and adds it to the World List.
        /// @param WorldName String name of the world.
        /// @param Managers A container of pre-made managers to be used by this world.
        /// @return Returns a pointer to the created world.
        World* CreateWorld(const String& WorldName, const std::vector <WorldManager*>& ManagerToBeAdded);
        /// @brief This creates a world and adds it to the World List.
        /// @param WorldName String name of the world.
        /// @param PhysicsInfo A ManagerConstructionInfo struct with data on how to configure the physics for this world.
        /// @param SceneType A string containing the name of the underlying scene type for this world.
        /// @return Returns a pointer to the created world.
        World* CreateWorld(const String& WorldName, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType);
        /// @brief This creates a world and adds it to the World List.
        /// @param WorldName String name of the world.
        /// @param ManagersToBeAdded A container of pre-made managers to be used by this world.
        /// @param PhysicsInfo A ManagerConstructionInfo struct with data on how to configure the physics for this world.
        /// @param SceneType A string containing the name of the underlying scene type for this world.
        /// @return Returns a pointer to the created world.
        World* CreateWorld(const String& WorldName, const std::vector<WorldManager*>& ManagersToBeAdded, const Physics::ManagerConstructionInfo& PhysicsInfo, const String& SceneType);

        /// @brief This adds a world to the list of worlds in the entresol.
        /// @param WorldToAdd The pointer to the world to be added.
        void AddWorld(World* WorldToBeAdded);

        /// @brief This gets a world from the world list by name.
        /// @param WorldName The string name of the world to retrieve.
        /// @return Returns a pointer to the world with the given name, otherwise returns NULL.
        World* GetWorld(const String& WorldName);
        /// @brief This gets a world from the world list by index.
        /// @param WorldIndex The index of the world to retrieve.
        /// @return Returns a pointer to the world at the specified index.
        World* GetWorld(const Whole Index);
        /// @brief This gets the number of worlds in the world list.
        /// @return Returns the number of worlds in the world list.
        UInt16 GetNumWorlds();
        /// @brief This removes the given world from the world list.
        /// @param WorldToBeRemoved Pointer to the world to be removed from the world list.
        /// @return Returns a pointer to the removed world.
        World* RemoveWorld(World* WorldToBeRemoved);
        /// @brief This removes the world with the given name from the world list.
        /// @param WorldName The name of the world to be removed.
        /// @return Returns a pointer to the removed world.
        World* RemoveWorldByName(const String& WorldName);
        /// @brief This removes all worlds from the world list.
        void RemoveAllWorlds();
        /// @brief This destroys the given world and removes it from the world list.
        /// @param WorldToBeDestroyed The name of the world to be destroyed.
        void DestroyWorld(World* WorldToBeDestroyed);
        /// @brief This destroys the world with the given name and removes it from the world list.
        /// @param WorldName The name of the world to be destroyed.
        void DestroyWorldByName(const String& WorldName);
        /// @brief This destroys all the worlds in the world list.
        void DestroyAllWorlds();

        /// @brief Pauses all animations, particles, and object movement throughout all worlds.
        /// @param Pause Pauses the world if true, unpauses if false.
        void PauseAllWorlds(Boole Pause);

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Logging

        /// @brief Runtime event and message logging.
        /// @param Message This is what will be streamed to the log.
        /// @throws Anything GetLogStream could throw.
        template <class T> void _Log(const T& Message)
            { this->_LogString(StringTools::ConvertToString(Message)); }
        /// @brief Log String directly with no conversion.
        /// @param Message The string to log.
        /// @throws Anything GetLogStream could throw.
        void _LogString(const String& Message);
        /// @brief A nearly threadsafe logging sink.
        /// @details If ID is not supplied and the thread is managed by the framescheduler this should be thread safe.
        /// @param ID Defaults to the id of the current thread, but can be used to retrieve the logger for any thread.
        /// @return A Logger that can be used by the thread with given ID or outside of Framescheduling in a non-threadsafe way.
        /// @exception This throws a ParametersRangeException if the thread is not managed by the frame scheduler or it Threading::FrameScheduler::CreateThreads() has not
        /// been called (It creates the ThreadSpecific resources that contain the Loggers).
        Logger& _GetLogStream(Threading::ThreadId ID = Threading::this_thread::get_id());
        /// @internal
        /// @brief This is used to asynchronously handle log messages.
        /// @return Returns a pointer to the aggregator responsible for synchronizing logs between threads.
        Threading::LogAggregator* _GetLogAggregator();
    };//Entresol
}//Mezzanine
#endif
