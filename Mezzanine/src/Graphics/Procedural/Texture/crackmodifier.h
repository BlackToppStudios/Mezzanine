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
#ifndef _graphicsproceduralcrackmodifier_h
#define _graphicsproceduralcrackmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will generate random coloured lines on a texture.
            /// @details Crack starting points are generated randomly, and can be configured to have varying or non-varying lengths.
            /// Cracks will bend based on normals generated from the parameter image.  Normals are calculated from the red and green
            /// channels exclusively.  Values on the Blue channel have no effect.  @n @n
            /// Additionally the Alpha channel in the parameter image can be used to filter which pixels are allowed to generate
            /// lines.  This is done internally by comparing the Alpha channel value to the normal length.
            ///////////////////////////////////////
            class MEZZ_LIB CrackModifier : public TextureModifier
            {
            public:
                /// @enum LengthMode
                /// @brief An enum used to set how the length of each generated crack is to be determined.
                enum LengthMode
                {
                    LM_Constant      = 1,  ///< The modifier will use the configured length unaltered.
                    LM_Random        = 2,  ///< The modifier will randomize the pixel length of the cracks between zero and double the configured length.
                    LM_NormalBased   = 3   ///< The modifier will generate the pixel length of a crack between the configured value and a value larger based on the normal length set in the parameter texture.
                };
                /// @enum QualityMode
                /// @brief An enum used to set the quality
                enum QualityMode
                {
                    QM_LowQuality    = 1,  ///< Use the configured colour for cracks without further modification.
                    QM_Blend         = 2,  ///< Combines the colours from the image being modified and the configured crack colour.
                    QM_SubPixel      = 3   ///< Attempts to blend the colours of all surrounding pixels for a more smooth appearance.
                };
            protected:
                /// @internal
                /// @brief The base colour to give to the pixels forming the cracks.
                ColourValue CrackColour;
                /// @internal
                /// @brief The parameter image to pull normals the cracks are to move along from.  Alpha channel and colours may also be used.
                TextureBuffer* NormalsTexture;
                /// @internal
                /// @brief The number of cracks to generate.
                Whole CrackCount;
                /// @internal
                /// @brief The seed for the random number generator used to place the cracks.
                Whole GeneratorSeed;
                /// @internal
                /// @brief The mode in which the final length of each crack is determined.  See LengthMode enum for more details.
                LengthMode CrackLengthMode;
                /// @internal
                /// @brief The mode in which the colours at or near the crack are to be determined.  See QualityMode enum for more details.
                QualityMode CrackQualityMode;
                /// @internal
                /// @brief The pixel length of the cracks to be generated.
                UInt8 CrackLength;
                /// @internal
                /// @brief Determines the amount of "stutter" to the cracks normal curve (or non-curve depending on the parameter image).
                UInt8 CrackVariation;
            public:
                /// @brief Blank constructor.
                CrackModifier();
                /// @brief Class destructor.
                virtual ~CrackModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the normals texture to use for generating cracks.
                /// @param Normals A pointer to the Texture to use for reading normals that cracks will be generated along.  Initial Value: NULL.
                /// @return Returns a reference to this.
                CrackModifier& SetNormalsTexture(TextureBuffer* Normals);
                /// @brief Sets the colour of the generated cracks.
                /// @param Colour The base colour to give to the pixels forming the cracks.  Initial Value: (1.0,1.0,1.0,1.0).
                /// @return Returns a reference to this.
                CrackModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the generated cracks.
                /// @param Red The amount of red to give to the pixels forming the cracks.  Initial Value: 1.0.
                /// @param Green The amount of green to give to the pixels forming the cracks.  Initial Value: 1.0.
                /// @param Blue The amount of blue to give to the pixels forming the cracks.  Initial Value: 1.0.
                /// @param Alpha The amount of alpha to give to the pixels forming the cracks.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                CrackModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets how many cracks to create.
                /// @param Count The number of cracks to generate.  Initial Value: 100.
                /// @return Returns a reference to this.
                CrackModifier& SetCrackCount(const Whole Count);
                /// @brief Sets any additional configuration for the length of the cracks.
                /// @param Length The mode in which the final length of each crack is determined.  See LengthMode enum for more details.  Initial Value: LM_Constant.
                /// @return Returns a reference to this.
                CrackModifier& SetCrackLengthMode(const LengthMode Length);
                /// @brief Sets any additional configuration for the colour/sampling of the crack colour.
                /// @param Quality The mode in which the colours at or near the crack are to be determined.  See QualityMode enum for more details.  Initial Value: QM_LowQuality.
                /// @return Returns a reference to this.
                CrackModifier& SetCrackQualityMode(const QualityMode Quality);
                /// @brief Sets the length of each crack that is created.  May not be used verbatim.
                /// @param Length The pixel length of the cracks to be generated.  Initial Value: 255.
                /// @return Returns a reference to this.
                CrackModifier& SetCrackLength(const UInt8 Length);
                /// @brief Sets the amount of wobble or stutter to the cracks that are created.
                /// @param Variation Determines the amount of "stutter" to the cracks normal curve (or non-curve depending on the parameter image).  Initial Value: 64.
                /// @return Returns a reference to this.
                CrackModifier& SetCrackVariation(const UInt8 Variation);
                /// @brief Sets the seed for the random number generator used to determine crack starting positions.
                /// @param Seed The seed to be used for the randomly generatated positions to place the cracks.  Initial Value: 5120.
                /// @return Returns a reference to this.
                CrackModifier& SetGeneratorSeed(const Whole Seed);
            };//CrackModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
