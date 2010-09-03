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
#ifndef _managerbase_h
#define _managerbase_h

#include "worldgetset.h"

#include <iostream>

namespace phys
{
    class World;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ManagerBase
    /// @headerfile managerbase.h
    /// @brief This is the base class from which all the World Managers inherit.
    /// @details This creates a base set of functions that Managers are all
    /// expected to implement.
    ///////////////////////////////////////
    class ManagerBase
    {
        public:
            /// @enum ManagerTypeName
            /// @brief A listing of Manager TypeNames
            /// @details These will be returned by ManagerBase::GetType(), and will allow
            /// code using this to determine what type of Manager class they are working with
            /// and use this information to more safely cast to the correct manager if needed.
            enum ManagerTypeName
            {
                ActorContainerBase,
                CameraManager,
                EventManager,
                GraphicsManager,
                PhysicsManager,
                ResourceManager,
                SoundManager,
                UserCreated         /// This is what User created managers that do not derive from any other managers are expected to use to prevent confusion with game internals
            };

            /// @brief This makes working with Callback function pointer a bit easier.
            typedef bool (*Callback)();

            /// @brief Default Constructor
            /// @details This creates a default manager without a valid reference to the game world. The
            /// lack of such a reference could cause issues or change behavior of a Manager class. A Manager
            /// Must be given the pointer to the world it is suppposed to work with prior to the calling
            /// of ManagerBase::Initialize(). This will most likely be the constructor used when creating
            /// a Manager to pass into the constructor of a new phys::World.
            ManagerBase();

            /// @brief Simple Constructor
            /// @details This is the prefered constructor. This is used to "Attach" a manager to a phys::world. This is expected to
            /// configure everything that this manager will need except for items requiring integration with other managers.
            /// @param GameWorld_ This is the phys::World this Manager is expected to work with.
            ManagerBase(World* GameWorld_);

            /// @brief Deconstructor
            /// @details This is actuall
            virtual ~ManagerBase();

            /// @brief Configure Items requiring other Managers
            /// @details If you are using the phys::World this is called when phys::World::GameInit() is called. It is expected that
            /// by the time this is called either ManagerBase::ManagerBase(World*) or ManagerBase::SetGameWorld(World*) will have been called.
            /// This is where all configuration that requires atleast one other manager on the phys::World to exist.\n\n
            /// Yes we know it is spelled wrong, but are Zs cooler anyway.
            virtual void Initialize() = 0;

            /// @brief This gets the World that this manager is working with.
            /// @return This returns a phys::World* that is the same as the one store in this Manager
            virtual World* GetGameWorld() const;

            /// @brief This sets the phys::World that this Manager works with.
            /// @details It is expected that this won't change very much, and for some managers changing this at the wrong time could
            /// cause some very fundamental problems. In fact, managers should be designed so that they
            /// can swapped  out. For example swapping out event managers could allow for a swift re-mapping of game controls when
            /// a game switches modes.\n\n
            /// For managers that can be moved it is expected that this function will change the pointer on the phys::World for the
            /// appropriate manager to point to this manager. This simplifies the calls that will naturally come next. To detach a
            /// Manager from the world pass this method a NULL pointer. If the manager cannot be removed or detached it should
            /// throw and exception using World::LogAndThrow, and it must not fail to attach to a world (that means it must internally
            /// handle all exception raised by the attaching process or throw an "Unrecoverable Error")
            virtual void SetGameWorld( World* GameWorld_ );

            /// @brief This returns the type of this manager.
            /// @details This is intended to make using and casting from Manager base easier. With this is is possible to cast from
            /// ManagerBase to the correct Manager Type.
            /// @return This returns a ManagerTypeName to identify what this can be safely cast to.
            virtual ManagerTypeName GetType() const = 0;


        ///////////////////////////////////////////////////////////////////////////////
        // MainLoop and CallBack Management
        ///////////////////////////////////////

            /// @brief This assigns a function to be the callback function to run prior to the main loop
            /// @param PreMainCallback This is a pointer to a function that returns a bool and accepts no arguments, this is in the
            /// form of a ManagerBase::Callback. If SetPreMainLoopItems
            /// is passed 0, NULL or a null pointer, the callback will be forgetten and will not attempt to be called.
            virtual void SetPreMainLoopItems(Callback PreMainCallback);

            /// @brief This runs any callback that is required before the mainloop items are run.
            /// @details This will return whatever the callback returns, which is true to end the main loop after this frame, or true to continue it. If no callback
            /// is set this simply returns true, as to not interupt the mainloop.
            /// \n This is a great Function to override in Manager classes where the complexity of callbacks is not required. This would make coding items that need
            /// to be performed at specific times easier. If you do that, it would be a good idea to call this version of the function, just in case a callback
            /// is set.
            /// @return This returns a false to end the main loop, or a true if the main loop should continue.
            virtual bool PreMainLoopItems();

            /// @brief This returns the Callback that would be called before the main loop items
            /// @return This returns a ManagerBase::Callback which is a pointer to the callback function that will be called before the main loop items
            virtual Callback GetPreMainLoopItems() const;

            /// @brief This simply calls SetPreMainLoopItems() passing it 0.
            virtual void ErasePreMainLoopItems();

            /// @brief The main loop calls this once per frame
            /// @details This is where each manager is expected to put anything that needs to be run each iteration of the main loop
            virtual void DoMainLoopItems() = 0;

            /// @brief This assigns a function to be the callback function to run prior to the main loop
            /// @param PostMainCallback This is a pointer to a function that returns a bool and accepts no arguments, this is in the
            /// form of a ManagerBase::Callback. If SetPosMainLoopItems
            /// is passed 0, NULL or a null pointer, the callback will be forgetten and will not attempt to be called.
            virtual void SetPostMainLoopItems(Callback PostMainCallback);

            /// @brief This runs any callback that is required after the mainloop items are run.
            /// @details This will return whatever the callback returns, which is true to end the main loop after this frame, or true to continue it. If no callback
            /// is set this simply returns true, as to not interupt the mainloop.
            /// \n This is a great Function to override in Manager classes where the complexity of callbacks is not required. This would make coding items that need
            /// to be performed at specific times easier. If you do that, it would be a good idea to call this version of the function, just in case a callback
            /// is set.
            /// @return This returns a false to end the main loop, or a true if the main loop should continue.
            virtual bool PostMainLoopItems();

            /// @brief This returns the Callback that would be called before the main loop items
            /// @return This returns a ManagerBase::Callback which is a p ointer to the callback function that will be called after the main loop items
            virtual Callback GetPostMainLoopItems() const;

            /// @brief This simply calls SetPostMainLoopItems() passing it 0.
            virtual void ErasePostMainLoopItems();

            /// @brief This gets the Priority of this manager.
            /// @details This has no Set counterpart to allow the changing of a manager's priority. This is expected to be set by the manager, or expose a safe method
            /// to change it at that level.
            /// @return This returns a short int that represents the priority of the manager. Lower is executed first.
            virtual short int GetPriority();

            /// @brief This gets the Priority of this manager.
            /// @details This has no Set counterpart to allow the changing of a manager's priority. This is expected to be set by the manager, or expose a safe method
            /// to change it at that level.
            /// @return This returns a short int that represents the priority of the manager. Lower is executed first.
            virtual void SetPriority( short int Priority_ );

            /// @brief This Allows any manager to be sent to a stream. Primarily used for logging
            /// @return This returns a std::ostream& that contains the name, and maybe some data for the the streammed manager.
            std::string GetTypeName();

        protected:
            /// @internal
            /// @brief The actual pointer to the world
            World* GameWorld;

            /// @internal
            /// @brief This is a function pointer to the function that should be called before running Main Loop Items
            Callback PreMainLoop;

            /// @internal
            /// @brief This is a function pointer to the function that should be called after running Main Loop Items
            Callback PostMainLoop;

            /// @brief This is a weighting used by the main loop to decide what order the managers should be called in.
            /// @details A lower number gets called earlier in the Main loop. By default rendering the graphics occurs at
            /// priority 0.
            short int Priority;
    };// /ManagerBase

    //@brief The << operator required to enable logging of Manager's to get types
    //std::ostream& operator << (std::ostream& stream, const ManagerBase& ManagerToGuess);

} // /phys

#endif
