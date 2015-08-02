// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _graphicsoglrendersyshelper_h_cpp
#define _graphicsoglrendersyshelper_h_cpp

#include "datatypes.h"
#include "Graphics/graphicsenumerations.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Creates the rendersystem and populates internal structures appropriately for use.
        void MEZZ_LIB CreateGLRenderSystem();
        /// @brief Shutsdown the rendersystem and removes it from internal structures.
        /// @note Currently does nothing because Ogres shutdown sequence is bad.
        void MEZZ_LIB DestroyGLRenderSystem();

        /// @brief Gets the Mezzanine type for the internal GL render system.
        /// @return Returns the RenderSystem enum value representing this type of internal render system.
        Graphics::RenderSystem MEZZ_LIB GetGLRenderSystemType();
        /// @brief Gets the long name of the GL render system.
        /// @return Returns a String containing the long name of the GL render system.
        String MEZZ_LIB GetGLRenderSystemName();
        /// @brief Gets the short name of the GL render system.
        /// @return Returns a String containing the short name of the GL render system.
        String MEZZ_LIB GetShortenedGLRenderSystemName();
    }//Graphics
}//Mezzanine

#endif
