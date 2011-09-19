//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _soundset_h
#define _soundset_h

#include "datatypes.h"
#include "sound.h"
#include <vector>

namespace phys
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a vector that stores sounds.
        /// @details This is a vector and can be use to store sounds that can be grouped together
        /// for similiar purposes or similiar content for easy tracking.
        ///////////////////////////////////////////////////////////////////////////////
        class PHYS_LIB SoundSet : public std::vector< Sound* >
        {
            private:
                /// @brief The name of the sound
                String Name;
            public:
                /// @brief a Simple counter to insure unique names of soundsets
                static Whole UnnamedInstanceCount;

                /// @brief Default constructor
                SoundSet()
                    { Name = StringCat("SoundSet",ToString(UnnamedInstanceCount++)); } // Name the First "SoundSet0" then the next "SoundSet1" and then "SoundSet2"...

                /// @brief Default constructor
                explicit SoundSet(const String& _Name) : Name(_Name)
                    { }

                /// @brief Get the name of the SoundSet
                /// @return The Name of this SoundSet
                const String& GetName() const
                    { return Name; }

                /// @brief Create an xml::Node describing this
                /// @return An xml::Node
                xml::Node ProtoSerialize() const;

                /// @brief Create an xml::Node describing this
                /// @return An xml::Node
                void ProtoDeSerialize(const xml::Node&);

                static String SerializableName();

        };//soundset
        /// Todo de/serialize sound set
    }//Audio
}//phys

#endif
