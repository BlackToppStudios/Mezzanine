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
#ifndef _graphicsproceduralalphamaskmodifier_h
#define _graphicsproceduralalphamaskmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will use a mask image to set the alpha channels of a source image.
            /// @details Depending on how the members are configured, an AlphaMaskModifier can operate in one of three modes:
            /// <ol><li>Extract alpha channel as an opaque monochrome bitmap or</li><li>Multiply alpha channel with parameter image pixel intensity, or</li><li>Use parameter image as color alpha mask.</li></ol> @n
            /// Mode 1 is accomplished by leaving or setting the MaskTexture member to NULL, regardless of the ColourMask member.  Mode 2 is accomplished by setting the MaskTexture member to a valid texture and
            /// leaving or setting the ColourMask member to false.  Mode 3 is accomplished by setting the MaskTexture member to a valid texture and setting the ColourMask member to true.
            ///////////////////////////////////////
            class MEZZ_LIB AlphaMaskModifier : public TextureModifier
            {
            protected:
                /// @internal
                /// @brief A pointer to the texture to use as the Alpha Mask.
                TextureBuffer* MaskTexture;
                /// @internal
                /// @brief Whether or not to use normalized colours from the mask for calculating the alpha channel, instead of the intensity.
                Boole ColourMask;
            public:
                /// @brief Blank constructor.
                AlphaMaskModifier();
                /// @brief Class destructor.
                virtual ~AlphaMaskModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                /// @exception The MaskTexture and the Buffer to be modified must be the same size.  Otherwise an INVALID_STATE_EXCEPTION will be thrown.
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the texture that will be used as the mask.
                /// @param Mask A pointer to the texture to use as the Alpha Mask.  Initial Value: NULL.
                /// @return Returns a reference to this.
                AlphaMaskModifier& SetMaskTexture(TextureBuffer* Mask);
                /// @brief Sets whether or not the colour of the mask will be used instead of the intensity.
                /// @param UseColour Whether or not to use normalized colours from the mask for calculating the alpha channel, instead of the intensity.  Initial Value: False.
                /// @return Returns a reference to this.
                AlphaMaskModifier& SetColourMask(const Boole UseColour);
            };//AlphaMaskModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
