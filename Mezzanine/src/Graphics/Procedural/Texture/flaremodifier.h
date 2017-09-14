// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsproceduralflaremodifier_h
#define _graphicsproceduralflaremodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

#include "vector2.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will draw an illuminated circle that fades over a distance to a previous texture.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB FlareModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief The colour of the flare to draw.
                ColourValue FlareColour;
                /// @internal
                /// @brief The central position of the flare on the texture in relative coordinates.
                Vector2 FlareCenter;
                /// @internal
                /// @brief The radius of the flare to draw in relative coordinates.
                Vector2 FlareRadius;
                /// @internal
                /// @brief A modifier for the intensity of the flare colour.
                Real FlareBrightness;
            public:
                /// @brief Blank constructor.
                FlareModifier();
                /// @brief Class destructor.
                virtual ~FlareModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the flare to be rendered.
                /// @param Colour The colour of the flare to draw.  Initial Value: (1.0,1.0,1.0,1.0).
                /// @return Returns a reference to this.
                FlareModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the flare to be rendered.
                /// @param Red The amount of red colour in the flare to draw.  Initial Value: 1.0.
                /// @param Green The amount of green colour in the flare to draw.  Initial Value: 1.0.
                /// @param Blue The amount of blue colour in the flare to draw.  Initial Value: 1.0.
                /// @param Alpha The amount of alpha colour in the flare to draw.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                FlareModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the center position of the flare.
                /// @param Center The central position of the flare on the texture in relative coordinates.  Initial Value: (0.5,0.5).
                /// @return Returns a reference to this.
                FlareModifier& SetFlareCenter(const Vector2& Center);
                /// @brief Sets the center position of the flare.
                /// @param X The center position of the flare on the X axis in relative coordinates.  Initial Value: 0.5.
                /// @param Y The center position of the flare on the Y axis in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareCenter(const Real X, const Real Y);
                /// @brief Sets the center position of the flare on the X axis.
                /// @param X The center position of the flare on the X axis in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareCenterX(const Real X);
                /// @brief Sets the center position of the flare on the Y axis.
                /// @param Y The center position of the flare on the Y axis in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareCenterY(const Real Y);
                /// @brief Sets the radius size of the flare.
                /// @param Radius The radius of the flare to draw in relative coordinates.  Initial Value: (0.5,0.5).
                /// @return Returns a reference to this.
                FlareModifier& SetFlareRadius(const Vector2& Radius);
                /// @brief Sets the radius size of the flare.
                /// @param X The radius of the flare on the X axis in relative coordinates.  Initial Value: 0.5.
                /// @param Y The radius of the flare on the Y axis in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareRadius(const Real X, const Real Y);
                /// @brief Sets the radius size of the flare on the X axis.
                /// @param X The radius of the flare on the X axis in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareRadiusX(const Real X);
                /// @brief Sets the radius size of the flare on the Y axis.
                /// @param Y The radius of the flare on the Y axis in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareRadiusY(const Real Y);
                /// @brief Sets the brightness of the flare.
                /// @param Brightness A modifier for the intensity of the flare colour.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                FlareModifier& SetFlareBrightness(const Real Brightness);
            };//FlareModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
