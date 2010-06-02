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
#ifndef _worldquerytool_h
#define _worldquerytool_h

#include "world.h"
#include "actorbase.h"
#include "vectorwactor3.h"
#include "metacode.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldQueryTool
    /// @headerfile worldquerytool.h
    /// @brief This provides a number of optional tools for working with a phys::World
    /// @details Currently this allows for more seamless mouse use, including 'picking'
    /// of objects with the mouse, and associated functionality.
    ///////////////////////////////////////
    class WorldQueryTool
    {
        private:
            /// @internal
            /// @brief This does all the work for building the class
            /// @details This simply assigns all the values passed in appropriately
            /// where they need to go
            /// @param GameWorld This is the phys::World we are querying
            void contruct(World* GameWorld_);

            /// @internal
            /// @brief This is gameworld we will be querying
            World* GameWorld;

        public:
            /// @brief Basic Constructor
            /// @details This creates a WorldQueryTool Ready to run queries on the the world you pass it.
            /// @param GameWorld_ This is a pointer to the phys::World to be queried
            WorldQueryTool(World* GameWorld_);

            /// @brief This
            /// @details
            /// @param
            /// @return
            Whole GetMouseX();

            /// @brief
            /// @details
            /// @param
            /// @return
            Whole GetMouseY();

            /// @brief
            /// @details
            /// @param
            /// @return
            bool IsMouseButtonPushed(MetaCode::InputCode);

            /// @brief
            /// @details
            /// @param
            /// @return
            VectorWActor3 GetActorUnderMouse();

            /// @brief
            /// @details
            /// @param
            /// @return
            ActorBase* GetActorFromRay(Vector3 Origin, Vector3 Destination);

            /// @brief This gathers any user-input/event data that might be queryed
            /// @details This should be called periodcally (ideally in the post user input callback) to allow this
            /// to gather data from the phys::World 's event manager. When called this will drop prior event data
            /// and any relevant queries will come from this new data. At the caller's discretion this method
            /// can properly delete any events pulled from the event manager
            /// @param ClearEventsFromEventMgr If set to true, This method will properly remove any events it pulls from the event manager.
            void GatherEvents(bool ClearEventsFromEventMgr = false);
    };

}



#endif
