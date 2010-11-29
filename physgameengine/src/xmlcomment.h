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

#ifndef _xmlcomment_h
#define _xmlcomment_h

#include "crossplatformexport.h"
#include "xmlnode.h"

namespace ticpp
{
    class Comment;
}

namespace phys
{
    namespace xml
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class comment
        /// @headerfile xmlcomment.h
        /// @brief This is a container of nodes and elements and other XML stuff
        /// @details
        class PHYS_LIB Comment: public Node
        {
            protected:
                /// @brief Construct a comment using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::comment that this class will wrap.
                /// @param FirstTimeWrapped Set this to true if you are instantiating this for the first time, false if Meta is used in another phys::xml::someclass
                Comment (ticpp::Comment* Meta, bool FirstTimeUsed = false);

            public:

                /// @internal
                /// @brief This will find or create a pointer to the comment
                /// @param Meta A pointer to a ticpp::comment that returned Attribute will wrap
                /// @return A pointer to the phys::xml::comment that wraps Meta
                static Comment* GetPointerFromWrapped(ticpp::Comment* Meta);

                /// @brief Creates a Comment with the
                /// @param Text The text that will be in the comment
                Comment (const std::string& Text);

                /// @brief Creates an Empty
                Comment ();

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isComment
                virtual Base::XMLComponentType GetType() const;

                /// @brief Stream XML data into a phys::xml hierearchy
                /// @param In This is the stream the data comes from when you use the >> operator
                /// @return This returns a stream containing all the data that was streamed in, to allow for chaining >> calls
                /// @details Inherited from phys::xml::Node
                virtual std::istream& operator>> (std::istream &In);

                /// @brief Stream XML data out of a phys::xml hierearchy
                /// @param Out This is the stream the data goes to from when you use the << operator
                /// @return This returns a stream containing all the data that was streamed out, to allow for chaining << calls and retrieval of the data
                /// @details Inherited from phys::xml::Node
                virtual std::ostream& operator<< (std::ostream &Out);


        }; // /comment
    }// /xml
}// /phys

#endif
