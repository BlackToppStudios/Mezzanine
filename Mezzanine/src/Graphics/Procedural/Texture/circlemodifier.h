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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduralcirclemodifier_h
#define _graphicsproceduralcirclemodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that draws a simple coloured circle onto the texture.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB CircleModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief The colour of the circle to be drawn.
                ColourValue CircleColour;
                /// @internal
                /// @brief The radius of the circle.
                Whole CircleRadius;
                /// @internal
                /// @brief The position on the X axis of the circle.
                Whole CircleX;
                /// @internal
                /// @brief The position on the Y axis of the circle.
                Whole CircleY;

                /// @internal
                /// @brief Convenience method for placing a pixel for the circle.
                /// @param DeltaX The offset from the origin point on the X axis to place the pixel.
                /// @param DeltaY The offset from the origin point on the Y axis to place the pixel.
                /// @param Buffer A reference to the buffer being worked on.
                void PutPixel(const Whole DeltaX, const Whole DeltaY, TextureBuffer& Buffer);
            public:
                /// @brief Blank constructor.
                CircleModifier();
                /// @brief Class destructor.
                virtual ~CircleModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the circle.
                /// @param Colour The colour of the circle to be drawn.  Initial Value: 1.0,1.0,1.0,1.0.
                /// @return Returns a reference to this.
                CircleModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the circle.
                /// @param Red The red colour component of the circle to be drawn.  Initial Value: 1.0.
                /// @param Green The red colour component of the circle to be drawn.  Initial Value: 1.0.
                /// @param Blue The red colour component of the circle to be drawn.  Initial Value: 1.0.
                /// @param Alpha The red colour component of the circle to be drawn.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                CircleModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the radius of the circle.
                /// @param Radius The radius of the circle to be drawn.  Initial Value: 0.
                /// @return Returns a reference to this.
                CircleModifier& SetRadius(const Whole Radius);
                /// @brief Sets the pixel position of the center of the circle.
                /// @param X The position on the X axis of the circle.  Initial Value: 0.
                /// @param Y The position on the Y axis of the circle.  Initial Value: 0.
                /// @return Returns a reference to this.
                CircleModifier& SetPosition(const Whole X, const Whole Y);
                /// @brief Sets the pixel position of the center of the circle on the X axis.
                /// @param X The position on the X axis of the circle.  Initial Value: 0.
                /// @return Returns a reference to this.
                CircleModifier& SetPositionX(const Whole X);
                /// @brief Sets the pixel position of the center of the circle on the Y axis.
                /// @param Y The position on the Y axis of the circle.  Initial Value: 0.
                /// @return Returns a reference to this.
                CircleModifier& SetPositionY(const Whole Y);
            };//CircleModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
