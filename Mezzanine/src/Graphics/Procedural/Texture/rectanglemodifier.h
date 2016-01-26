// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _graphicsproceduralrectanglemodifier_h
#define _graphicsproceduralrectanglemodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that draws a simple coloured rectangle onto the texture.
            /// @details This modifier will use both relative and absolute coordinates to place the the rectangle on the texture, first resolving
            /// the relative component and then adding the absolute component.
            ///////////////////////////////////////
            class MEZZ_LIB RectangleModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief The colour of the rectangle to apply to the texture.
                ColourValue FillColour;
                /// @internal
                /// @brief The left edge of the rectangle in relative coordinates.
                Real PositionXRel;
                /// @internal
                /// @brief The top edge of the rectangle in relative coordinates.
                Real PositionYRel;
                /// @internal
                /// @brief The horizontal size of the rectangle in relative coordinates.
                Real SizeXRel;
                /// @internal
                /// @brief The vertical size of the rectangle in relative coordinates.
                Real SizeYRel;
                /// @internal
                /// @brief The left edge adjustment of the rectangle after relative coordinates are calculated.
                Integer PositionXAdj;
                /// @internal
                /// @brief The top edge adjustment of the rectangle after relative coordinates are calculated.
                Integer PositionYAdj;
                /// @internal
                /// @brief The horizontal size adjustment of the rectangle after relative coordinates are calculated.
                Integer SizeXAdj;
                /// @internal
                /// @brief The vertical size adjustment of the rectangle after relative coordinates are calculated.
                Integer SizeYAdj;
            public:
                /// @brief Blank constructor.
                RectangleModifier();
                /// @brief Class destructor.
                virtual ~RectangleModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the rectangle to be generated.
                /// @param Colour The colour of the rectangle to generate.  Initial Value: 1.0, 1.0, 1.0, 1.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the rectangle to be generated.
                /// @param Red The Red component for the colour of the rectangle to generate.  Initial Value: 1.0.
                /// @param Green The Green component for the colour of the rectangle to generate.  Initial Value: 1.0.
                /// @param Blue The Blue component for the colour of the rectangle to generate.  Initial Value: 1.0.
                /// @param Alpha The Alpha component for the colour of the rectangle to generate.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the colour of the rectangle to be generated.
                /// @param Red The Red component for the colour of the rectangle to generate.  Initial Value: 255.
                /// @param Green The Green component for the colour of the rectangle to generate.  Initial Value: 255.
                /// @param Blue The Blue component for the colour of the rectangle to generate.  Initial Value: 255.
                /// @param Alpha The Alpha component for the colour of the rectangle to generate.  Initial Value: 255.
                /// @return Returns a reference to this.
                RectangleModifier& SetColour(const UInt8 Red, const UInt8 Green, const UInt8 Blue, const UInt8 Alpha);

                /// @brief Sets the relative position and size of this rectangle..
                /// @param XPosition The left edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @param YPosition The top edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @param XSize The horizontal size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @param YSize The vertical size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetRectangle(const Real XPosition, const Real YPosition, const Real XSize, const Real YSize);
                /// @brief Sets the offsets for the position and size of this rectangle.
                /// @param XPosition The left edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param YPosition The top edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param XSize The horizontal size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param YSize The vertical size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetRectangle(const Integer XPosition, const Integer YPosition, const Integer XSize, const Integer YSize);
                /// @brief Sets the relative and offset values for the rectangle.
                /// @param XPositionRel The left edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @param YPositionRel The top edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @param XSizeRel The horizontal size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @param YSizeRel The vertical size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @param XPositionAdj The left edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param YPositionAdj The top edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param XSizeAdj The horizontal size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param YSizeAdj The vertical size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetRectangle(const Real XPositionRel, const Real YPositionRel, const Real XSizeRel, const Real YSizeRel, const Integer XPositionAdj, const Integer YPositionAdj, const Integer XSizeAdj, const Integer YSizeAdj);

                /// @brief Sets the relative and offset values for the rectangle position.
                /// @param XPositionRel The left edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @param YPositionRel The top edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @param XPositionAdj The left edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param YPositionAdj The top edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetPosition(const Real XPositionRel, const Real YPositionRel, const Integer XPositionAdj, const Integer YPositionAdj);
                /// @brief Sets the relative and offset values for the rectangle size.
                /// @param XSizeRel The horizontal size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @param YSizeRel The vertical size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @param XSizeAdj The horizontal size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @param YSizeAdj The vertical size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetSize(const Real XSizeRel, const Real YSizeRel, const Integer XSizeAdj, const Integer YSizeAdj);

                /// @brief Sets the relative component of the left edge.
                /// @param X The left edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetXPositionRel(const Real X);
                /// @brief Sets the relative component of the top edge.
                /// @param Y The top edge of the rectangle in relative coordinates.  Initial Value: 0.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetYPositionRel(const Real Y);
                /// @brief Sets the relative component of the horizontal size.
                /// @param X The horizontal size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetXSizeRel(const Real X);
                /// @brief Sets the relative component of the vertical size.
                /// @param Y The vertical size of the rectangle in relative coordinates.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                RectangleModifier& SetYSizeRel(const Real Y);
                /// @brief Sets the absolute component of the left edge.
                /// @param X The left edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetXPositionAdj(const Integer X);
                /// @brief Sets the absolute component of the top edge.
                /// @param Y The top edge adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetYPositionAdj(const Integer Y);
                /// @brief Sets the absolute component for the horizontal size.
                /// @param X The horizontal size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetXSizeAdj(const Integer X);
                /// @brief Sets the absolute component for the vertical size.
                /// @param Y The vertical size adjustment of the rectangle after relative coordinates are calculated.  Initial Value: 0.
                /// @return Returns a reference to this.
                RectangleModifier& SetYSizeAdj(const Integer Y);
            };//RectangleModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
