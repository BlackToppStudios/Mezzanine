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
#ifndef _attachable_h
#define _attachable_h

#include "datatypes.h"

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Attachable
    /// @headerfile attachable.h
    /// @brief This is just a base class to be used by elements that are attachable to nodes.
    /// @details This class is useless on it's own and should not be created manually.
    ///////////////////////////////////////
    class Attachable
    {
        public:
            enum AttachableElement
            {
                Camera,
                Light,
                ParticleEffect
            };
        protected:
            /// @brief Enum value representing the type of element this is.
            Attachable::AttachableElement ElementType;
            /// @brief Sets the type of element this class is.
            /// @param Enum value representing the type of element to be set.
            void SetElementType(Attachable::AttachableElement Type);
        public:
            /// @brief No initialization class constructor.
            Attachable();
            /// @brief Class destructor.
            virtual ~Attachable();
            /// @brief Gets the type of element this is.
            /// @return Returns an enum value indicating what type of element this is.
            Attachable::AttachableElement GetElementType();
            /// @brief Gets the name of the attachable element.
            virtual String& GetName() = 0;
    };
}//phys

#endif
