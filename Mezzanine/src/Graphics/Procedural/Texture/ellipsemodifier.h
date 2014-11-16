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
#ifndef _graphicsproceduralellipsemodifier_h
#define _graphicsproceduralellipsemodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that draws a simple coloured ellipse onto the texture.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB EllipseModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief The colour of the ellipse to apply to the texture.
                ColourValue FillColour;
                /// @internal
                /// @brief The left edge of the ellipse in relative coordinates.
                Real PositionXRel;
                /// @internal
                /// @brief The top edge of the ellipse in relative coordinates.
                Real PositionYRel;
                /// @internal
                /// @brief The horizontal radius of the ellipse in relative coordinates.
                Real RadiusXRel;
                /// @internal
                /// @brief The vertical radius of the ellipse in relative coordinates.
                Real RadiusYRel;
                /// @internal
                /// @brief The left edge adjustment of the ellipse after relative coordinates are calculated.
                Integer PositionXAdj;
                /// @internal
                /// @brief The top edge adjustment of the ellipse after relative coordinates are calculated.
                Integer PositionYAdj;
                /// @internal
                /// @brief The horizontal radius adjustment of the ellipse after relative coordinates are calculated.
                Integer RadiusXAdj;
                /// @internal
                /// @brief The vertical radius adjustment of the ellipse after relative coordinates are calculated.
                Integer RadiusYAdj;

                /// @internal
                /// @brief Convenience method for placing a pixel for the ellipse.
                /// @param XPos The position on the X axis to place the pixel.
                /// @param YPos The position on the Y axis to place the pixel.
                /// @param Buffer A reference to the buffer being worked on.
                void PutPixel(const Integer XPos, const Integer YPos, TextureBuffer& Buffer);
            public:
                /// @brief Blank constructor.
                EllipseModifier();
                /// @brief Class destructor.
                virtual ~EllipseModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the ellipse to be generated.
                /// @param Colour The colour of the ellipse to generate.  Initial Value: 1.0, 1.0, 1.0, 1.0.
                /// @return Returns a reference to this.
                EllipseModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the ellipse to be generated.
                /// @param Red The Red component for the colour of the ellipse to generate.  Initial Value: 1.0.
                /// @param Green The Green component for the colour of the ellipse to generate.  Initial Value: 1.0.
                /// @param Blue The Blue component for the colour of the ellipse to generate.  Initial Value: 1.0.
                /// @param Alpha The Alpha component for the colour of the ellipse to generate.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                EllipseModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the colour of the ellipse to be generated.
                /// @param Red The Red component for the colour of the ellipse to generate.  Initial Value: 255.
                /// @param Green The Green component for the colour of the ellipse to generate.  Initial Value: 255.
                /// @param Blue The Blue component for the colour of the ellipse to generate.  Initial Value: 255.
                /// @param Alpha The Alpha component for the colour of the ellipse to generate.  Initial Value: 255.
                /// @return Returns a reference to this.
                EllipseModifier& SetColour(const UInt8 Red, const UInt8 Green, const UInt8 Blue, const UInt8 Alpha);

                /// @brief Sets the relative position and radius of this ellipse..
                /// @param XPosition The left edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param YPosition The top edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param XRadius The horizontal radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param YRadius The vertical radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                EllipseModifier& SetEllipse(const Real XPosition, const Real YPosition, const Real XRadius, const Real YRadius);
                /// @brief Sets the offsets for the position and radius of this ellipse.
                /// @param XPosition The left edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param YPosition The top edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param XRadius The horizontal radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param YRadius The vertical radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetEllipse(const Integer XPosition, const Integer YPosition, const Integer XRadius, const Integer YRadius);
                /// @brief Sets the relative and offset values for the ellipse.
                /// @param XPositionRel The left edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param YPositionRel The top edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param XRadiusRel The horizontal radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param YRadiusRel The vertical radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param XPositionAdj The left edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param YPositionAdj The top edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param XRadiusAdj The horizontal radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param YRadiusAdj The vertical radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetEllipse(const Real XPositionRel, const Real YPositionRel, const Real XRadiusRel, const Real YRadiusRel, const Integer XPositionAdj, const Integer YPositionAdj, const Integer XRadiusAdj, const Integer YRadiusAdj);

                /// @brief Sets the relative and offset values for the ellipse position.
                /// @param XPositionRel The left edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param YPositionRel The top edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param XPositionAdj The left edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param YPositionAdj The top edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetPosition(const Real XPositionRel, const Real YPositionRel, const Integer XPositionAdj, const Integer YPositionAdj);
                /// @brief Sets the relative and offset values for the ellipse radius.
                /// @param XRadiusRel The horizontal radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param YRadiusRel The vertical radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @param XRadiusAdj The horizontal radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @param YRadiusAdj The vertical radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetRadius(const Real XRadiusRel, const Real YRadiusRel, const Integer XRadiusAdj, const Integer YRadiusAdj);

                /// @brief Sets the relative component of the left edge.
                /// @param X The left edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                EllipseModifier& SetXPositionRel(const Real X);
                /// @brief Sets the relative component of the top edge.
                /// @param Y The top edge of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                EllipseModifier& SetYPositionRel(const Real Y);
                /// @brief Sets the relative component of the horizontal radius.
                /// @param X The horizontal radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                EllipseModifier& SetXRadiusRel(const Real X);
                /// @brief Sets the relative component of the vertical radius.
                /// @param Y The vertical radius of the ellipse in relative coordinates.  Initial Value: 0.5.
                /// @return Returns a reference to this.
                EllipseModifier& SetYRadiusRel(const Real Y);
                /// @brief Sets the absolute component of the left edge.
                /// @param X The left edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetXPositionAdj(const Integer X);
                /// @brief Sets the absolute component of the top edge.
                /// @param Y The top edge adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetYPositionAdj(const Integer Y);
                /// @brief Sets the absolute component for the horizontal radius.
                /// @param X The horizontal radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetXRadiusAdj(const Integer X);
                /// @brief Sets the absolute component for the vertical radius.
                /// @param Y The vertical radius adjustment of the ellipse after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                EllipseModifier& SetYRadiusAdj(const Integer Y);
            };//EllipseModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
