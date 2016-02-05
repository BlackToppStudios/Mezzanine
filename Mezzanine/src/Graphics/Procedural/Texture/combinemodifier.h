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
#ifndef _graphicsproceduralcombinemodifier_h
#define _graphicsproceduralcombinemodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Convenience class used by the CombineModifier class to describe what action to take with a specific texture to be combined.
            /// @details
            ///////////////////////////////////////
            struct CombineLayer
            {
                /// @brief A pointer to the texture that will be combined with the target texture in the modifier.
                TextureBuffer* TextureLayer;
                /// @brief The mode that will be used to combine the specified texture with the target texture being processed by the modifier.
                Whole Mode;
            };//CombineLayer

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will attempt to combine the content of multiple textures.
            /// @details To use this modifier, textures should be added in sequence based on the order in which you want to apply the textures to the target texture.  Added textures are
            /// not combined directly, rather the first texture is applied to the target texture and the second texture is applied to the result of that operation and so forth, from the
            /// first texture specified to the last.
            ///////////////////////////////////////
            class MEZZ_LIB CombineModifier : public TextureModifier
            {
            public:
                /// @brief An enum used to describe how a texture is to be combined with the other textures in the queue.
                /// @details Unless explicitly stated, modes in this enum do not operate on the Alpha Channel.
                enum CombineMode
                {
                    CM_Add_Clamp        = 1,  ///< Simple addition of colours.  This will generally make the result image brighter.  Overflowing colour channels will be clamped to the maximum value.
                    CM_Add_Wrap         = 2,  ///< Simple addition of colours.  This can make the image brighter or darker, because overflowing colour channels will wrap back to 0 and increment from there.
                    CM_Sub_Clamp        = 3,  ///< Simple subtraction of colours.  This will generally make the result image darker.  Underflowing colour channels will be clamped to the minimum value.
                    CM_Sub_Wrap         = 4,  ///< Simple subtraction of colours.  This can make the image brighter or darker, because underflowing colour channels will wrap back to the maximum value and decrement from there.
                    CM_Multiply         = 5,  ///< Multiplication of colours.  Results are then divided by 256.  This can make the image brighter or darker.  Overflowing colour channels will be clamped to the maximum value.
                    CM_Multiply_Double  = 6,  ///< Multiplication of colours.  Results are then divided by 128, causing the result to be double what you would get with CM_Multiply.  This will generally make the result image
                                              ///< brighter than CM_Multiply, but can also make it darker with low colour values.  Overflowing colour channels will be clamped to the maximum value.
                    CM_Blend            = 7,  ///< For each colour channel, add them together and divide by two.  Produces the average of both textures.
                    CM_Alpha            = 8,  ///< As CM_Blend, but only performs the operation on the Alpha channel.  Leaves other channels unmodified.
                    CM_Layer            = 9   ///< Uses the Alpha Channel from the current operating texture to fade the colours in the target texture.  The inverse of the Alpha Channel is applied to the colours of the current
                                              ///< operating texture.  The results are then added together.
                };

                /// @brief Basic container type for @ref CombineLayer storage by this class.
                typedef std::vector<CombineLayer>               CombineLayerContainer;
                /// @brief Iterator type for @ref CombineLayer instances stored by this class.
                typedef CombineLayerContainer::iterator         CombineLayerIterator;
                /// @brief Const Iterator type for @ref CombineLayer instances stored by this class.
                typedef CombineLayerContainer::const_iterator   ConstCombineLayerIterator;
            protected:
                /// @internal
                /// @brief The amount of colour from each layer that is to be used when combining textures.
                ColourValue CombineFilterColour;
                /// @internal
                /// @brief A container of all the layers to be applied to the target texture in sequence.
                CombineLayerContainer CombineLayers;

                /// @internal
                /// @brief Processes a single layer to combine it with the target image.
                /// @param Layer The texture being combined with the result image.
                /// @param Buffer The buffer being drawn to/combined with.
                void ProcessLayer(const CombineLayer& Layer, TextureBuffer& Buffer);
            public:
                /// @brief Blank constructor.
                CombineModifier();
                /// @brief Class destructor.
                virtual ~CombineModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Adds a texture to the end of the sequence to be processed.
                /// @param Layer A pointer to the texture being added.
                /// @param Mode The combine operation to be performed between the specified texture in the first argument and the target texture being processed by the modifier.
                /// @return Returns a reference to this.
                CombineModifier& AddTexture(TextureBuffer* Layer, const CombineMode Mode);
                /// @brief Removes all textures from all layers.
                /// @return Returns a reference to this.
                CombineModifier& RemoveAllTextures();
                /// @brief Sets which colour channels to filter (and by how much) from each layer texture.
                /// @param Colour The amount of colour from the layer texture to keep when combining.  Initial Value: (1.0,1.0,1.0,1.0).
                /// @return Returns a reference to this.
                CombineModifier& SetFilterColour(const ColourValue& Colour);
                /// @brief Sets which colour channels to filter (and by how much) from each layer texture.
                /// @param Red The amount of red colour from the layer texture to keep when combining.  Initial Value: 1.0.
                /// @param Green The amount of green colour from the layer texture to keep when combining.  Initial Value: 1.0.
                /// @param Blue The amount of blue colour from the layer texture to keep when combining.  Initial Value: 1.0.
                /// @param Alpha The amount of alpha colour from the layer texture to keep when combining.  Initial Value: 1.0.
                /// @return Returns a reference to this.
                CombineModifier& SetFilterColour(const Real Red, const Real Green, const Real Blue, const Real Alpha);
            };//CombineModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
