// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _graphicsresolution_h
#define _graphicsresolution_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This stores all the basic configuration options a game window supports.
        /// @details
        ///////////////////////////////////////
        struct MEZZ_LIB Resolution
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The pixel width of the window.
            Whole Width;
            /// @brief The pixel height of the window.
            Whole Height;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Blank constructor.
            Resolution() :
                Width(0),
                Height(0)
                {  }

            /// @brief Descriptive constructor.
            /// @param ResWidth The pixel width of the window.
            /// @param ResHeight The pixel height of the window.
            Resolution(const Whole ResWidth, const Whole ResHeight) :
                Width(ResWidth),
                Height(ResHeight)
                {  }

            /// @brief Class destructor.
            ~Resolution()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the width and height of this resolution.
            /// @param ResWidth The pixel width of the window.
            /// @param ResHeight The pixel height of the window.
            void SetResolution(const Whole ResWidth, const Whole ResHeight)
            {
                this->Width = ResWidth;
                this->Height = ResHeight;
            }

            /// @brief Converts the values of this Resolution to a human readable string.
            /// @remarks The format for the presented string is "[Width]x[Height]".  For example: "1024x768".
            /// @return Returns a string containing the width and height of this resolution.
            String GetAsString() const
            {
                StringStream ResStream;
                ResStream << this->Width << "x" << this->Height;
                return ResStream.str();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Assignment operator.
            /// @param Other The other Resolution to copy from.
            /// @return Returns a reference to this.
            Resolution& operator=(const Resolution& Other)
            {
                this->Width = Other.Width;
                this->Height = Other.Height;
                return *this;
            }
        };//Resolution
    }//Graphics
}//Mezzanine

#endif
