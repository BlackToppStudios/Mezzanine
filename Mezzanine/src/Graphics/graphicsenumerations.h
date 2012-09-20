//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _graphicsenumerations_h
#define _graphicsenumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the Graphics namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace Graphics
    {
        /// @enum AspectRatio
        /// @brief This is used primarily by the GraphicsManager to keep track of the supported
        /// and currently used aspect ratios.
        enum AspectRatio
        {
            AR_3_By_2,     ///< 1.5
            AR_4_By_3,     ///< 1.334
            AR_5_By_3,     ///< 1.667
            AR_5_By_4,     ///< 1.25
            AR_8_By_5,     ///< 1.6
            AR_16_By_9,    ///< 1.778
            AR_17_By_9,    ///< 1.889
            AR_1_85_By_1,  ///< 1.85
            AR_2_39_By_1   ///< 2.39
        };

        /// @enum RenderSystem
        /// @brief Used primarily by the graphics manager class during initialization.
        /// @details This enum specifies which Rendersystem is to be used for rendering.
        enum RenderSystem
        {
            RS_DirectX9  = 0,
            RS_DirectX10 = 1,
            RS_DirectX11 = 2,
            RS_OpenGL2   = 3,
            RS_OpenGL3   = 4,
            RS_OpenGL4   = 5,
            RS_OpenGLES1 = 6,
            RS_OpenGLES2 = 7
        };
    }//Graphics
}//Mezzanine

#endif
