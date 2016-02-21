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
#ifndef _graphicsproceduralcellgenerator_h
#define _graphicsproceduralcellgenerator_h

#include "Graphics/Procedural/Texture/texturegenerator.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Create a texture consisting of cells aligned in a grid, or a chessboard.
            /// @details Cells can be irregular. If considered bump map, they have pyramid form.
            ///////////////////////////////////////
            class MEZZ_LIB CellGenerator : public TextureGenerator
            {
            public:
                /// @brief An enum to describe how the cells are layed out.
                enum CellMode
                {
                    CM_Grid,        ///< Cells are layed out in a normal grid pattern with their centers coloured in according to the set CellPattern.
                    CM_Chessboard   ///< Cells are layed out in a normal grid pattern, but will alternate the cell colour and background colour with each successive cell.
                };
                /// @brief An enum to describe how the cells are coloured.
                enum CellPattern
                {
                    CP_Both,   ///< Combines CP_Cross and CP_Cone
                    CP_Cross,  ///< Colours in the centers of each cell such that there is a cross connecting each of the four corners with increasing colour intensity towards the middle.
                    CP_Cone    ///< Colours in the centers of each cell such that there is a circle in the middle with increasing colour intensity towards the middle.
                };
            protected:
                /// @internal
                /// @brief The colour of the interior of the cells generated.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
                /// @internal
                /// @brief A value between 1 and 255 that determines how uniform the cells are.
                Whole GenRegularity;
                /// @internal
                /// @brief The number of oclumns and rows of cells to be generated.
                Whole GenDensity;
                /// @internal
                /// @brief Describes how the cells are layed out.
                CellMode GenMode;
                /// @internal
                /// @brief Describes how the cells are coloured.
                CellPattern GenPattern;
            public:
                /// @brief Class constructor.
                CellGenerator();
                /// @brief Class destructor.
                virtual ~CellGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the interior of the cells.
                /// @param Colour The colour of the interior of the cells generated.
                /// @return Returns a reference to this.
                CellGenerator& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the interior of the cells.
                /// @param Red The Red component for the cell top colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the cell top colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the cell top colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the cell top colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                CellGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                CellGenerator& SetSeed(const Whole Seed);
                /// @brief Set the regularity of texture.
                /// @remarks The maximum value of 255 creates identical cells. The minimum 0 creates random forms for each cells.
                /// @param Regularity A value between 1 and 255 that determines how uniform the cells are.  Initial Value: 128.
                /// @return Returns a reference to this.
                CellGenerator& SetRegularity(const UInt8 Regularity);
                /// @brief Set the density of cells in texture.
                /// @remarks This number needs to be > 0.
                /// @param Density The number of oclumns and rows of cells to be generated.  Initial Value: 8.
                /// @return Returns a reference to this.
                CellGenerator& SetDensity(const Whole Density);
                /// @brief Set the cell mode of texture.
                /// @param Mode The mode describing how the cells will be layed out.  Initial Value: CM_Grid.
                /// @return Returns a reference to this.
                CellGenerator& SetMode(const CellMode Mode);
                /// @brief Set the cell pattern of texture.
                /// @param Pattern The pattern describing how the cells will be coloured.  Initial Value: CP_Both.
                /// @return Returns a reference to this.
                CellGenerator& SetPattern(const CellPattern Pattern);
            };//CellGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
