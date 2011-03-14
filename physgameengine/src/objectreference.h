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
#ifndef _objectreference_h
#define _objectreference_h

#include "enumerations.h"

namespace phys
{
    class ObjectReference
    {
        protected:
            WorldObjectType Type;
            void* Object;
        public:
            /// @brief Class constructor.
            /// @param TheType The type of object this class is storing.
            /// @param TheObject The object to be stored.
            ObjectReference(const WorldObjectType& TheType, void* TheObject)
                : Type(TheType), Object(TheObject) {};
            /// @brief Class destructor.
            ~ObjectReference() {};
            /// @brief Gets the type of object this class references.
            /// @return Returns an enum value indicating what type of object this class references.
            WorldObjectType GetType() { return Type; };
            /// @brief Gets the object this class references.
            /// @return Returns a void pointer pointing to the object this class references.
            void* GetObject() { return Object; };
    };
}

#endif
