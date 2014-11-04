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
#ifndef _graphicsproceduraltexturegenerators_h
#define _graphicsproceduraltexturegenerators_h

#include "Graphics/Procedural/proceduralenumerations.h"
#include "Graphics/Procedural/texturebuffer.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A base class for texture generators that use the TextureBuffer class.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB TextureGenerator
            {
            protected:
            public:
                /// @brief Blank constructor.
                TextureGenerator();
                /// @brief Class destructor.
                virtual ~TextureGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Generates the Texture.
                /// @param SquareSize The width and height of the texture to create.  This value will be clamped to be at least 8.
                /// @param TexName The name to give to the generated texture.
                /// @param TexGroup The asset group to place the texture in.
                /// @param Format The pixel format for the generated Texture.
                /// @return Returns a pointer to the newly created Texture.
                Texture* GenerateTexture(const Whole SquareSize, const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;
                /// @brief Generates the Texture.
                /// @param TexWidth The pixel width of the texture to be generated.  This value will be clamped to be at least 8.
                /// @param TexHeight The pixel height of the texture to be generated.  This value will be clamped to be at least 8.
                /// @param TexName The name to give to the generated texture.
                /// @param TexGroup The asset group to place the texture in.
                /// @param Format The pixel format for the generated Texture.
                /// @return Returns a pointer to the newly created Texture.
                Texture* GenerateTexture(const Whole TexWidth, const Whole TexHeight, const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format = Graphics::PF_R8G8B8A8) const;
                /// @brief Creates a TextureBuffer with the pixels as configured in this generator.
                /// @param SquareSize The width and height of the texture to create.  This value will be clamped to be at least 8.
                /// @return Returns a new TextureBuffer with the generated pixels of this generator.
                TextureBuffer BuildTextureBuffer(const Whole SquareSize) const;
                /// @brief Creates a TextureBuffer with the pixels as configured in this generator.
                /// @param TexWidth The pixel width of the texture to be generated.  This value will be clamped to be at least 8.
                /// @param TexHeight The pixel height of the texture to be generated.  This value will be clamped to be at least 8.
                /// @return Returns a new TextureBuffer with the generated pixels of this generator.
                TextureBuffer BuildTextureBuffer(const Whole TexWidth, const Whole TexHeight) const;
                /// @brief Replaces and populates the pixels as configured in this generator to a TextureBuffer.
                /// @param Buffer The buffer to place this generators pixels in.
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const = 0;

                /// @brief Gets the name of this generator.
                /// @return Returns a String containing the name of this generator.
                virtual String GetName() const = 0;
            };//TextureGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Create a texture consisting of cells aligned in a grid, or a chessboard.
            /// @details Cells can be irregular. If considered bump map, they have pyramid form.
            ///////////////////////////////////////
            class MEZZ_LIB CellGenerator : public TextureGenerator
            {
            public:
                /// @enum CellMode
                /// @brief An enum to describe how the cells are layed out.
                enum CellMode
                {
                    CM_Grid,        ///< Cells are layed out in a normal grid pattern with their centers coloured in according to the set CellPattern.
                    CM_Chessboard   ///< Cells are layed out in a normal grid pattern, but will alternate the cell colour and background colour with each successive cell.
                };
                /// @enum CellPattern
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

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creates a cloud structured image.
            /// @details Creates a cloud structure from a specified perlin noise on a coloured background.
            ///////////////////////////////////////
            class MEZZ_LIB CloudGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The colour to be applied to the cloud.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
            public:
                /// @brief Class constructor.
                CloudGenerator();
                /// @brief Class destructor.
                virtual ~CloudGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the cloud.
                /// @param Colour The colour to be applied to the cloud to be generated.
                /// @return Returns a reference to this.
                CloudGenerator& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the cloud.
                /// @param Red The Red component for the cloud colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the cloud colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the cloud colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the cloud colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                CloudGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                CloudGenerator& SetSeed(const Whole Seed);
            };//CloudGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Fills full image with given colour gradients.
            /// @details Each corner of the image has unique colour.
            ///////////////////////////////////////
            class MEZZ_LIB GradientGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The colour of the top-left corner.
                ColourValue GenTopLeftColour;
                /// @internal
                /// @brief The colour of the top-right corner.
                ColourValue GenTopRightColour;
                /// @internal
                /// @brief The colour of the bottom-left corner.
                ColourValue GenBottomLeftColour;
                /// @internal
                /// @brief The colour of the bottom-right corner.
                ColourValue GenBottomRightColour;
            public:
                /// @brief Class constructor.
                GradientGenerator();
                /// @brief Class destructor.
                virtual ~GradientGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the top-left corner.
                /// @param Colour The colour to be applied to the top-left corner of the generated image.
                /// @return Returns a reference to this.
                GradientGenerator& SetTopLeftColour(const ColourValue& Colour);
                /// @brief Sets the colour of the top-left corner.
                /// @param Red The Red component for the top-left corner colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the top-left corner colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the top-left corner colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the top-left corner colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                GradientGenerator& SetTopLeftColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the colour of the top-right corner.
                /// @param Colour The colour to be applied to the top-right corner of the generated image.
                /// @return Returns a reference to this.
                GradientGenerator& SetTopRightColour(const ColourValue& Colour);
                /// @brief Sets the colour of the top-right corner.
                /// @param Red The Red component for the top-right corner colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the top-right corner colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the top-right corner colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the top-right corner colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                GradientGenerator& SetTopRightColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the colour of the bottom-left corner.
                /// @param Colour The colour to be applied to the bottom-left corner of the generated image.
                /// @return Returns a reference to this.
                GradientGenerator& SetBottomLeftColour(const ColourValue& Colour);
                /// @brief Sets the colour of the bottom-left corner.
                /// @param Red The Red component for the bottom-left corner colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the bottom-left corner colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the bottom-left corner colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the bottom-left corner colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                GradientGenerator& SetBottomLeftColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the colour of the bottom-right corner.
                /// @param Colour The colour to be applied to the bottom-right corner of the generated image.
                /// @return Returns a reference to this.
                GradientGenerator& SetBottomRightColour(const ColourValue& Colour);
                /// @brief Sets the colour of the bottom-right corner.
                /// @param Red The Red component for the bottom-right corner colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the bottom-right corner colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the bottom-right corner colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the bottom-right corner colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                GradientGenerator& SetBottomRightColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the colours of the image corners.
                /// @param TopLeft The colour to be applied to the top-left corner of the generated image.
                /// @param TopRight The colour to be applied to the top-right corner of the generated image.
                /// @param BottomLeft The colour to be applied to the bottom-left corner of the generated image.
                /// @param BottomRight The colour to be applied to the bottom-right corner of the generated image.
                /// @return Returns a reference to this.
                GradientGenerator& SetColours(const ColourValue& TopLeft, const ColourValue& TopRight, const ColourValue& BottomLeft, const ColourValue& BottomRight);
            };//GradientGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creats an image to be modified from a pre-existing image on disk.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB ImageGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The name of the file to load.
                String File;
                /// @internal
                /// @brief The name of the resource group the file belongs to.
                String Group;
            public:
                /// @brief Class constructor.
                ImageGenerator();
                /// @brief Class destructor.
                virtual ~ImageGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the file to be loaded by this generator.
                /// @param FileName The name of the file to load.
                /// @param GroupName The name of the resource group the file belongs to.
                /// @return Returns a reference to this.
                ImageGenerator& SetFile(const String& FileName, const String& GroupName);
            };//ImageGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creates a labyrinth structured image.
            /// @details Creates a labyrinth structure from a specified perlin noise on a coloured background.
            ///////////////////////////////////////
            class MEZZ_LIB LabyrinthGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The colour to be applied to the labyrinth.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
            public:
                /// @brief Class constructor.
                LabyrinthGenerator();
                /// @brief Class destructor.
                virtual ~LabyrinthGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the background.
                /// @param Colour The colour to be applied to the labyrinth to be generated.
                /// @return Returns a reference to this.
                LabyrinthGenerator& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the background.
                /// @param Red The Red component for the background colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the background colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the background colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the background colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                LabyrinthGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                LabyrinthGenerator& SetSeed(const Whole Seed);
            };//LabyrinthGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creates a marble structured image.
            /// @details Creates a marbel structure from a specified perlin noise on a coloured background.
            ///////////////////////////////////////
            class MEZZ_LIB MarbleGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The colour to be applied to the marble structure.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
            public:
                /// @brief Class constructor.
                MarbleGenerator();
                /// @brief Class destructor.
                virtual ~MarbleGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the marble structure.
                /// @param Colour The colour to be applied to the marble structure to be generated.
                /// @return Returns a reference to this.
                MarbleGenerator& SetColour(const ColourValue& Colour);
                /// @brief Set the colour of the marble structure.
                /// @param Red The Red component for the marble structure colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the marble structure colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the marble structure colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the marble structure colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                MarbleGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                MarbleGenerator& SetSeed(const Whole Seed);
            };//MarbleGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Fills full image with noise in a given colour.
            /// @details High quality noise with various noise algorithms.
            ///////////////////////////////////////
            class MEZZ_LIB NoiseGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The colour of the noise to be generated.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
                /// @internal
                /// @brief The type of noise generator to use when generating the image.
                Procedural::NoiseType NType;
            public:
                /// @brief Class constructor.
                NoiseGenerator();
                /// @brief Class destructor.
                virtual ~NoiseGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the background.
                /// @param Colour The colour to be applied to the noise to be generated.
                /// @return Returns a reference to this.
                NoiseGenerator& SetColour(const ColourValue& Colour);
                /// @brief Set the colour of the background.
                /// @param Red The Red component for the background colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the background colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the background colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the background colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                NoiseGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                NoiseGenerator& SetSeed(const Whole Seed);
                /// @brief Set the type of noise generation.
                /// @param Type Type of noise generator.  Initial Value: NT_White.
                /// @return Returns a reference to this.
                NoiseGenerator& SetType(const Procedural::NoiseType Type);
            };//NoiseGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Fills full image with given colour.
            /// @details Set all pixel to the same colour.
            ///////////////////////////////////////
            class MEZZ_LIB SolidGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The solid colour to generate.
                ColourValue GenColour;
            public:
                /// @brief Class constructor.
                SolidGenerator();
                /// @brief Class destructor.
                virtual ~SolidGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the background.
                /// @param Colour The solid colour to be applied to each pixel.
                /// @return Returns a reference to this.
                SolidGenerator& SetColour(const ColourValue& Colour);
                /// @brief Set the colour of the background.
                /// @param Red The Red component for the background colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the background colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the background colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the background colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                SolidGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
            };//SolidGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creates a textile structured image.
            /// @details Creates a textile structure from a specified perlin noise on a coloured background.
            ///////////////////////////////////////
            class MEZZ_LIB TextileGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The background colour of the textile to be generated.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
            public:
                /// @brief Class constructor.
                TextileGenerator();
                /// @brief Class destructor.
                virtual ~TextileGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the background.
                /// @param Colour The colour to be applied to the textile to be generated.
                /// @return Returns a reference to this.
                TextileGenerator& SetColour(const ColourValue& Colour);
                /// @brief Set the colour of the background.
                /// @param Red The Red component for the background colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the background colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the background colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the background colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                TextileGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                TextileGenerator& SetSeed(const Whole Seed);
            };//TextileGenerator

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief Creates a wood slice image.
            /// @details Creates a structure of annual rings from a specified perlin noise on a coloured background.
            ///////////////////////////////////////
            class MEZZ_LIB WoodGenerator : public TextureGenerator
            {
            protected:
                /// @internal
                /// @brief The background colour of the texture to be generated.
                ColourValue GenColour;
                /// @internal
                /// @brief The seed to be used for the random number generator.
                Whole GenSeed;
                /// @internal
                /// @brief The number of annual rings to be rendered.
                Whole GenRings;
            public:
                /// @brief Blank constructor.
                WoodGenerator();
                /// @brief Seedless constructor.
                /// @param Rings The number of annual rings.  Values under 3 will be ignored.
                /// @param Colour The colour to be applied to the wood to be generated.
                WoodGenerator(const Whole Rings, const ColourValue& Colour);
                /// @brief Descriptive constructor.
                /// @param Seed The seed value for the random number generator.
                /// @param Rings The number of annual rings.  Values under 3 will be ignored.
                /// @param Colour The colour to be applied to the wood to be generated.
                WoodGenerator(const Whole Seed, const Whole Rings, const ColourValue& Colour);
                /// @brief Class destructor.
                virtual ~WoodGenerator();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureGenerator::AddToTextureBuffer(TextureBuffer&) const
                virtual void AddToTextureBuffer(TextureBuffer& Buffer) const;
                /// @copydoc TextureGenerator::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the colour of the background.
                /// @param Colour The colour to be applied to the wood to be generated.
                /// @return Returns a reference to this.
                WoodGenerator& SetColour(const ColourValue& Colour);
                /// @brief Sets the colour of the background.
                /// @param Red The Red component for the background colour.  Range: [0.0, 1.0].
                /// @param Green The Green component for the background colour.  Range: [0.0, 1.0].
                /// @param Blue The Blue component for the background colour.  Range: [0.0, 1.0].
                /// @param Alpha The Alpha component for the background colour.  Range: [0.0, 1.0].
                /// @return Returns a reference to this.
                WoodGenerator& SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha = 1.0);
                /// @brief Sets the seed for the "random" number generator.
                /// @param Seed The seed value for the random number generator.  Initial Value: 5120.
                /// @return Returns a reference to this.
                WoodGenerator& SetSeed(const Whole Seed);
                /// @brief Sets the number of annual rings.
                /// @param Rings The number of annual rings.  Values under 3 will be ignored.  Initial Value: 8.
                /// @return Returns a reference to this.
                WoodGenerator& SetRings(const Whole Rings);
            };//WoodGenerator
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
