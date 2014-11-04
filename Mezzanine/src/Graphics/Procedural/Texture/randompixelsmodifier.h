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
#ifndef _graphicsproceduralrandompixelsmodifier_h
#define _graphicsproceduralrandompixelsmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifer that will draw coloured pixels at random positions on the texture.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB RandomPixelsModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief The colour of the randomly placed pixels.
                ColourValue PixelColour;
                /// @internal
                /// @brief The number of randomly placed pixels to generate.
                Whole PixelCount;
                /// @internal
                /// @brief The seed to be used for the randomly generatated texture positions the pixels will be drawn to.
                Whole GeneratorSeed;
            public:
                /// @brief Blank constructor.
                RandomPixelsModifier();
                /// @brief Class destructor.
                virtual ~RandomPixelsModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the pixels to be generated.
                /// @param Colour The colour of the randomly placed pixels.  Initial Value: 1.0, 1.0, 1.0, 1.0.
                /// @return Returns a reference to this.
                RandomPixelsModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the pixels to be generated.
                /// @param Red The Red component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @param Green The Green component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @param Blue The Blue component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @param Alpha The Alpha component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                RandomPixelsModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha);
                /// @brief Sets the colour of the pixels to be generated.
                /// @param Red The Red component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @param Green The Green component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @param Blue The Blue component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @param Alpha The Alpha component for the colour of the randomly generated pixels.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                RandomPixelsModifier& SetColour(const UInt8 Red, const UInt8 Green, const UInt8 Blue, const UInt8 Alpha);
                /// @brief Sets the number of pixels to generate.
                /// @remarks If this is left as zero then the number of pixels generated will be decided based on the textures size, taking the square root of both
                /// the width and height, adding them together, and multiplying by 10.
                /// @param Count The number of randomly placed pixels to generate.  Initial Value: 0.
                /// @return Returns a reference to this.
                RandomPixelsModifier& SetPixelCount(const Whole Count);
                /// @brief Sets the seed for the random number generator used to determine pixel positions.
                /// @param Seed The seed to be used for the randomly generatated texture positions the pixels will be drawn to.  Initial Value: 5120.
                /// @return Returns a reference to this.
                RandomPixelsModifier& SetSeed(const Whole Seed);
            };//RandomPixelsModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
