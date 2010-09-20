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

#ifndef _xmlbase_cpp
#define _xmlbase_cpp

#define TIXML_USE_TICPP

#include "xmlbase.h"

namespace phys
{

    namespace xml
    {
        Base::~Base()
        { }

        int Base::Row () const
        { return this->Wrapped->Row();}

        int Base::Column () const
            { return this->Wrapped->Column(); }

        /*
        void Base::SetUserData (void *user)
            { this->Wrapped->SetUserData(user); }
        void* Base::GetUserData ()
            { this->Wrapped->GetUserData(); }
        const void* Base::GetUserData () const
            { this->Wrapped->GetUserData(); }
        */

        /// @TODO Decide what to do with User data... I think remove it, even from the TiXML we are shipping with, then wrap the following SetCondenseWhiteSpace, IsWhiteSpaceCondensed, EncodeString, and those listed on http://ticpp.googlecode.com/svn/docs/classticpp_1_1Base-members.html
    }// /xml
}// /phys

//std::ostream& operator << (std::ostream& stream, const phys::xml::Base& x)
//{}


#endif
