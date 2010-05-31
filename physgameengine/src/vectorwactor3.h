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
#ifndef _vectorwactor3_h
#define _vectorwactor3_h

#include "vector3.h"
#include "actorbase.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class VectorWActor3
    /// @headerfile vectorwactor3.h
    /// @brief This class is used to store or transfer a position relative to an Actor.
    /// @details This class is simply a Pointer to an actor and a Vector3
    class VectorWActor3
    {
        public:
            /// @brief Default Constructor
            /// @details This creates a VectorWActor3 with a null Actor pointer and
            /// a Vector3 initialized to 0
            VectorWActor3();

            /// @brief Complete Constructor
            /// @details This fills the class with data that is passed in.
            /// @param Actor_ This is a pointer to an actor
            /// @param Vector_ This is a Vector3 that represents some amount, like motion or distance
            VectorWActor3(ActorBase* Actor_, Vector3 Vector_);

            /// @brief The pointer to the ActorBase
            ActorBase* Actor;

            /// @brief The pointer to the Vector3
            Vector3 Vector;
    };
}// /phys

#endif
