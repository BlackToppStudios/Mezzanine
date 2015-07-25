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
#ifndef _graphicsproceduralthresholdmodifier_h
#define _graphicsproceduralthresholdmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will set a pixels colour if it's intensity is outside a set threshold.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB ThresholdModifier : public TextureModifier
            {
            public:
                /// @enum ThresholdMode
                /// @brief An enum to describe when to replace the colour of a pixel.
                enum ThresholdMode
                {
                    TM_Above = 1,                            ///< Uses only the upper limit.  Any pixel with an intensity above the set upper limit will be replaced.
                    TM_Below = 2,                            ///< Uses only the lower limit.  Any pixel with an intensity below the set lower limit will be replaced.
                    TM_OutsideRange = TM_Below | TM_Above    ///< Uses both the upper and lower limits.  Any pixel with an intensity outside their range will be replaced.
                };
                /// @enum ReplaceMode
                /// @brief An enum to describe with what granularity pixels will be checked and replaced.
                enum ReplaceMode
                {
                    RM_PixelIntensity = 1,   ///< Checks against the total intensity of the pixel and will replace the entire pixel if it is outside the set threshold.
                    RM_ColourChannels = 2    ///< Checks each colour channel, and will replace just the colour channels that are outside the set threshold.
                };
            protected:
                /// @internal
                /// @brief The colour to apply if the pixel is outside the allowed threshold.
                ColourValue ThresholdColour;
                /// @internal
                /// @brief The lower limit of the threshold to check for.
                Real LowerLimit;
                /// @internal
                /// @brief The upper limit of the threshold to check for.
                Real UpperLimit;
                /// @internal
                /// @brief The mode that determines which limits are used for the allowed threshold.
                UInt16 Threshold;
                /// @internal
                /// @brief The mode that determines the granularity of checks to be performed.
                UInt16 Replace;
            public:
                /// @brief Blank constructor.
                ThresholdModifier();
                /// @brief Class destructor.
                virtual ~ThresholdModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour to use if a pixel is outside the set threshold.
                /// @param Colour The colour to apply if the pixel intensity is outside the allowed threshold.  Initial Value: (0.0,0.0,0.0,1.0).
                /// @return Returns a reference to this.
                ThresholdModifier& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour to use if a pixel is outside the set threshold.
                /// @param Red The amount of Red to apply if the pixel intensity is outside the allowed threshold.  Initial Value: 0.0.
                /// @param Green The amount of Green to apply if the pixel intensity is outside the allowed threshold.  Initial Value: 0.0.
                /// @param Blue The amount of Blue to apply if the pixel intensity is outside the allowed threshold.  Initial Value: 0.0.
                /// @param Alpha The amount of Alpha to apply if the pixel intensity is outside the allowed threshold.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                ThresholdModifier& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);

                /// @brief Sets the lower limit of the threshold.
                /// @param Lower The lower limit of the threshold to check for.  Initial Value: 0.0.
                /// @return Returns a reference to this.
                ThresholdModifier& SetLowerLimit(const Real Lower);
                /// @brief Sets the upper limit of the threshold.
                /// @param Upper The upper limit of the threshold to check for.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                ThresholdModifier& SetUpperLimit(const Real Upper);
                /// @brief Sets the lower and upper limit of the threshold.
                /// @param Lower The lower limit of the threshold to check for.  Initial Value: 0.0.
                /// @param Upper The upper limit of the threshold to check for.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                ThresholdModifier& SetLimitRange(const Real Lower, const Real Upper);

                /// @brief Sets which limits are to be considered for the threshold.
                /// @param Mode The mode that determines which limits are used for the allowed threshold.  Initial Value: TM_Below.
                /// @return Returns a reference to this.
                ThresholdModifier& SetThresholdMode(const ThresholdMode Mode);
                /// @brief Sets the components of the pixel to be replaced.
                /// @param Mode The mode that determines the granularity of checks to be performed.  Initial Value: RM_PixelIntensity.
                /// @return Returns a reference to this.
                ThresholdModifier& SetReplaceMode(const ReplaceMode Mode);
            };//ThresholdModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
