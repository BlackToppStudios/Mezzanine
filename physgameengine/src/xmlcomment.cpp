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

#ifndef _xmlcomment_cpp
#define _xmlcomment_cpp

#include "xmlcomment.h"

#define TIXML_USE_TICPP
#include <ticpp.h>

namespace phys
{
    namespace xml
    {
        Comment::Comment (ticpp::Comment* Meta, bool FirstTimeUsed)
        {
            this->Wrapped = Meta;
            if (FirstTimeUsed)
                { this->TakeOwnerOfWrapped(); }
        }

        Comment* Comment::GetPointerFromWrapped(ticpp::Comment* Meta)
        {
            return 0;
        }

        Comment::Comment(const std::string& Text)
        {
            this->Wrapped = new ticpp::Comment(Text);
            this->TakeOwnerOfWrapped();
        }

        std::istream& Comment::operator>> (std::istream &In)
            { return In >> *(static_cast <ticpp::Comment*>(this->Wrapped)); }

        std::ostream& Comment::operator<< (std::ostream &Out)
            { return Out << *(static_cast <ticpp::Comment*>(this->Wrapped)); }

    }// /xml
}// /phys

/// @brief Streaming output operator for XML comments
/// @details This converts the data of an XML comment into a stream Ideal for sending to a log or cout
/// @param stream This is the stream we send our data to.
/// @return This returns an std::ostream which now contains our data.
// Commented out due to compiler error, despite above include the compiler doesn't seem to know what an ostream is.
//std::ostream& operator<< (std::ostream& stream, const phys::xml::Comment& x);


#endif
