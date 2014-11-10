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
#ifndef _graphicsproceduraltexturegenerators_cpp
#define _graphicsproceduraltexturegenerators_cpp

#include "Graphics/Procedural/texturegenerators.h"
#include "Graphics/Procedural/noise.h"

#include "Graphics/image.h"

#include "MathTools/mathtools.h"
#include "vector3.h"

#include <cstdlib>

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            // TextureGenerator Methods

            TextureGenerator::TextureGenerator()
                {  }

            TextureGenerator::~TextureGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Texture* TextureGenerator::GenerateTexture(const Whole SquareSize, const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format) const
            {
                TextureBuffer TexBuf(SquareSize);
                this->AddToTextureBuffer(TexBuf);
                Texture* NewMesh = TexBuf.GenerateTexture(TexName,TexGroup,Format);
                return NewMesh;
            }

            Texture* TextureGenerator::GenerateTexture(const Whole TexWidth, const Whole TexHeight, const String& TexName, const String& TexGroup, const Graphics::PixelFormat Format) const
            {
                TextureBuffer TexBuf(TexWidth,TexHeight);
                this->AddToTextureBuffer(TexBuf);
                Texture* NewMesh = TexBuf.GenerateTexture(TexName,TexGroup,Format);
                return NewMesh;
            }

            TextureBuffer TextureGenerator::BuildTextureBuffer(const Whole SquareSize) const
            {
                TextureBuffer TexBuf(SquareSize);
                this->AddToTextureBuffer(TexBuf);
                return TexBuf;
            }

            TextureBuffer TextureGenerator::BuildTextureBuffer(const Whole TexWidth, const Whole TexHeight) const
            {
                TextureBuffer TexBuf(TexWidth,TexHeight);
                this->AddToTextureBuffer(TexBuf);
                return TexBuf;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // CellGenerator Methods

            CellGenerator::CellGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120),
                GenRegularity(128),
                GenDensity(8),
                GenMode(CellGenerator::CM_Grid),
                GenPattern(CellGenerator::CP_Both)
                {  }

            CellGenerator::~CellGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CellGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                Boole cfc;
                Real Coeff;

                srand(this->GenSeed);
                const Real Regularity = this->GenRegularity / 255.0;
                std::vector<Vector3> CellPoints( this->GenDensity * this->GenDensity );

                for( Whole Y = 0 ; Y < this->GenDensity ; ++Y )
                {
                    for( Whole X = 0 ; X < this->GenDensity ; ++X )
                    {
                        Real RandNum1 = static_cast<Real>( rand() ) / 65536.0;
                        Real RandNum2 = static_cast<Real>( rand() ) / 65536.0;
                        CellPoints[X + Y * this->GenDensity].X = ( X + 0.5 + ( RandNum1 - 0.5 ) * ( 1 - Regularity ) ) / this->GenDensity - 1.0 / Buffer.GetWidth();
                        CellPoints[X + Y * this->GenDensity].Y = ( Y + 0.5 + ( RandNum2 - 0.5 ) * ( 1 - Regularity ) ) / this->GenDensity - 1.0 / Buffer.GetHeight();
                        CellPoints[X + Y * this->GenDensity].Z = 0;
                    }
                }

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Vector3 PixelPos;
                        PixelPos.X = static_cast<Real>( X ) / static_cast<Real>( Buffer.GetWidth() ),
                        PixelPos.Y = static_cast<Real>( Y ) / static_cast<Real>( Buffer.GetHeight() );
                        PixelPos.Z = 0.0;

                        Real MinDist = 10.0;
                        Real NextMinDist = MinDist;
                        Integer Xo = X * this->GenDensity / Buffer.GetWidth();
                        Integer Yo = Y * this->GenDensity / Buffer.GetHeight();
                        for( Integer V = -1 ; V < 2 ; ++V )
                        {
                            Integer Vo = ( ( Yo + this->GenDensity + V ) % this->GenDensity ) * this->GenDensity;
                            for( Integer U = -1 ; U < 2 ; ++U )
                            {
                                Vector3 CellPos = CellPoints[ ( ( Xo + this->GenDensity + U ) % this->GenDensity ) + Vo ];
                                if( U == -1 && X * this->GenDensity < Buffer.GetWidth() )
                                    { CellPos.X -= 1; }
                                if( V == -1 && Y * this->GenDensity < Buffer.GetHeight() )
                                    { CellPos.Y -= 1; }
                                if( U == 1 && X * this->GenDensity >= Buffer.GetWidth() * ( this->GenDensity - 1 ) )
                                    { CellPos.X += 1; }
                                if( V == 1 && Y * this->GenDensity >= Buffer.GetHeight() * ( this->GenDensity - 1 ) )
                                    { CellPos.Y += 1; }

                                Real Norm = PixelPos.Distance(CellPos);
                                if( Norm < MinDist ) {
                                    NextMinDist = MinDist;
                                    MinDist = Norm;
                                }else if( Norm < NextMinDist ) {
                                    NextMinDist = Norm;
                                }
                            }
                        }

                        switch( this->GenPattern )
                        {
                            case CellGenerator::CP_Cross:
                            {
                                MinDist = MathTools::Clamp( 2 * NextMinDist * this->GenDensity - 1, Real(0.0), Real(1.0) );
                                break;
                            }
                            case CellGenerator::CP_Cone:
                            {
                                MinDist = MathTools::Clamp( 1 - MinDist * this->GenDensity, Real(0.0), Real(1.0) );
                                break;
                            }
                            default:
                            case CellGenerator::CP_Both:
                            {
                                MinDist = MathTools::Clamp( ( NextMinDist - MinDist ) * this->GenDensity, Real(0.0), Real(1.0) );
                                break;
                            }
                        }

                        switch( this->GenMode )
                        {
                            case CellGenerator::CM_Chessboard:
                            {
                                cfc = ( ( Xo & 1 ) ^ ( Yo & 1 ) ) != 0;
                                Coeff = ( 1 - 2 * cfc ) / 2.5;
                                Buffer.SetRedByte( X, Y, (UInt8)( ( cfc + Coeff * MinDist ) * this->GenColour.RedChannel * 255.0 ) );
                                Buffer.SetGreenByte( X, Y, (UInt8)( ( cfc + Coeff * MinDist ) * this->GenColour.GreenChannel * 255.0 ) );
                                Buffer.SetBlueByte( X, Y, (UInt8)( ( cfc + Coeff * MinDist ) * this->GenColour.BlueChannel * 255.0 ) );
                                break;
                            }
                            default:
                            case CellGenerator::CM_Grid:
                            {
                                Buffer.SetRedByte( X, Y, (UInt8)( MinDist * this->GenColour.RedChannel * 255.0 ) );
                                Buffer.SetGreenByte( X, Y, (UInt8)( MinDist * this->GenColour.GreenChannel * 255.0 ) );
                                Buffer.SetBlueByte( X, Y, (UInt8)( MinDist * this->GenColour.BlueChannel * 255.0 ) );
                                break;
                            }
                        }
                        Buffer.SetAlphaByte( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String CellGenerator::GetName() const
                { return "CellGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CellGenerator& CellGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            CellGenerator& CellGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            CellGenerator& CellGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            CellGenerator& CellGenerator::SetRegularity(const UInt8 Regularity)
            {
                this->GenRegularity = Regularity;
                return *this;
            }

            CellGenerator& CellGenerator::SetDensity(const Whole Density)
            {
                this->GenDensity = Density;
                return *this;
            }

            CellGenerator& CellGenerator::SetMode(const CellMode Mode)
            {
                this->GenMode = Mode;
                return *this;
            }

            CellGenerator& CellGenerator::SetPattern(const CellPattern Pattern)
            {
                this->GenPattern = Pattern;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // CloudGenerator Methods

            CloudGenerator::CloudGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120)
                {  }

            CloudGenerator::~CloudGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CloudGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                srand(this->GenSeed);
                int RandNum = rand();
                PerlinNoise Noise(8, 0.5, 1.0 / 32.0, 1.0);
                Real FilterLevel = 0.7;
                Real PreserveLevel = 0.3;

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = std::max( 0.0, std::min( 1.0, Noise.Noise2D( X + RandNum, Y + RandNum ) * 0.5 + 0.5 ) );
                        Buffer.SetRedByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.RedChannel * 255.0 + FilterLevel * this->GenColour.RedChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.GreenChannel * 255.0 + FilterLevel * this->GenColour.GreenChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.BlueChannel * 255.0 + FilterLevel * this->GenColour.BlueChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetAlphaByte( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String CloudGenerator::GetName() const
                { return "CloudGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CloudGenerator& CloudGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            CloudGenerator& CloudGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            CloudGenerator& CloudGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // GradientGenerator Methods

            GradientGenerator::GradientGenerator() :
                GenTopLeftColour(0.0,0.0,1.0,1.0),
                GenTopRightColour(0.0,1.0,0.0,1.0),
                GenBottomLeftColour(1.0,0.0,0.0,1.0),
                GenBottomRightColour(0.0,1.0,1.0,1.0)
                {  }

            GradientGenerator::~GradientGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void GradientGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                Real finv_WH = 1.0 / static_cast<Real>( Buffer.GetWidth() * Buffer.GetHeight() );
                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real A = static_cast<Real>( ( Buffer.GetWidth() - X ) * (Buffer.GetHeight() - Y ) ) * finv_WH;
                        Real B = static_cast<Real>( X * (Buffer.GetHeight() - Y ) ) * finv_WH;
                        Real C = static_cast<Real>( ( Buffer.GetWidth() - X ) * Y ) * finv_WH;
                        Real D = static_cast<Real>( X * Y ) * finv_WH;

                        Buffer.SetRedByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.RedChannel * A ) + ( this->GenTopRightColour.RedChannel * B ) + ( this->GenBottomLeftColour.RedChannel * C ) + ( this->GenBottomRightColour.RedChannel * D ) ) * 255.0 ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.GreenChannel * A ) + ( this->GenTopRightColour.GreenChannel * B ) + ( this->GenBottomLeftColour.GreenChannel * C ) + ( this->GenBottomRightColour.GreenChannel * D ) ) * 255.0 ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.BlueChannel * A ) + ( this->GenTopRightColour.BlueChannel * B ) + ( this->GenBottomLeftColour.BlueChannel * C ) + ( this->GenBottomRightColour.BlueChannel * D ) ) * 255.0 ) );
                        Buffer.SetAlphaByte( X, Y, (UInt8)( ( ( this->GenTopLeftColour.AlphaChannel * A ) + ( this->GenTopRightColour.AlphaChannel * B ) + ( this->GenBottomLeftColour.AlphaChannel * C ) + ( this->GenBottomRightColour.AlphaChannel * D ) ) * 255.0 ) );
                    }
                }
            }

            String GradientGenerator::GetName() const
                { return "GradientGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            GradientGenerator& GradientGenerator::SetTopLeftColour(const ColourValue& Colour)
            {
                this->GenTopLeftColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetTopLeftColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenTopLeftColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetTopRightColour(const ColourValue& Colour)
            {
                this->GenTopRightColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetTopRightColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenTopRightColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomLeftColour(const ColourValue& Colour)
            {
                this->GenBottomLeftColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomLeftColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenBottomLeftColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomRightColour(const ColourValue& Colour)
            {
                this->GenBottomRightColour = Colour;
                return *this;
            }

            GradientGenerator& GradientGenerator::SetBottomRightColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenBottomRightColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            GradientGenerator& GradientGenerator::SetColours(const ColourValue& TopLeft, const ColourValue& TopRight, const ColourValue& BottomLeft, const ColourValue& BottomRight)
            {
                this->GenTopLeftColour = TopLeft;
                this->GenTopRightColour = TopRight;
                this->GenBottomLeftColour = BottomLeft;
                this->GenBottomRightColour = BottomRight;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // ImageGenerator Methods

            ImageGenerator::ImageGenerator()
                {  }

            ImageGenerator::~ImageGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void ImageGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                Image ImportImg(this->File,this->Group);

                if( ImportImg.GetHeight() >= Buffer.GetHeight() && ImportImg.GetWidth() >= Buffer.GetWidth() ) {
                    for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                    {
                        for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                        {
                            Buffer.SetPixel(X,Y,ImportImg.GetColourAt(X,Y,0));
                        }
                    }
                }
            }

            String ImageGenerator::GetName() const
                { return "ImageGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            ImageGenerator& ImageGenerator::SetFile(const String& FileName, const String& GroupName)
            {
                this->File = FileName;
                this->Group = GroupName;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // LabyrinthGenerator Methods

            LabyrinthGenerator::LabyrinthGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120)
                {  }

            LabyrinthGenerator::~LabyrinthGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void LabyrinthGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                srand(this->GenSeed);
                int RandNum = rand();
                PerlinNoise Noise(1, 0.65, 1.0 / 16.0, 1.0);
                Real FilterLevel = 0.7;
                Real PreserveLevel = 0.3;

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = std::min( Real(1.0), MathTools::Abs( Noise.Noise2D( X + RandNum, Y + RandNum ) ) );
                        Buffer.SetRedByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.RedChannel * 255.0 + FilterLevel * this->GenColour.RedChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.GreenChannel * 255.0 + FilterLevel * this->GenColour.GreenChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.BlueChannel * 255.0 + FilterLevel * this->GenColour.BlueChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetAlphaByte( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String LabyrinthGenerator::GetName() const
                { return "LabyrinthGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            LabyrinthGenerator& LabyrinthGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            LabyrinthGenerator& LabyrinthGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            LabyrinthGenerator& LabyrinthGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // MarbleGenerator Methods

            MarbleGenerator::MarbleGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120)
                {  }

            MarbleGenerator::~MarbleGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void MarbleGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                srand(this->GenSeed);
                int RandNum = rand();
                PerlinNoise Noise(2, 0.65, 1.0 / 32.0, 1.0);
                Real XFact = 1.0 / 96.0;
                Real YFact = 1.0 / 48.0;
                Real FilterLevel = 0.7;
                Real PreserveLevel = 0.3;

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = std::min( Real(1.0), MathTools::Abs( MathTools::Sin( X * XFact + Y * YFact + Noise.Noise2D( X + RandNum, Y + RandNum ) ) * MathTools::GetPi() ) );
                        Buffer.SetRedByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.RedChannel * 255.0 + FilterLevel * this->GenColour.RedChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.GreenChannel * 255.0 + FilterLevel * this->GenColour.GreenChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.BlueChannel * 255.0 + FilterLevel * this->GenColour.BlueChannel * 255.0 * NoiseVal, 255.0 ) );
                        Buffer.SetAlphaByte( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String MarbleGenerator::GetName() const
                { return "MarbleGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            MarbleGenerator& MarbleGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            MarbleGenerator& MarbleGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            MarbleGenerator& MarbleGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // NoiseGenerator Methods

            NoiseGenerator::NoiseGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120),
                NType(Procedural::NT_White)
                {  }

            NoiseGenerator::~NoiseGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void NoiseGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                NoiseBase* NoiseGen = NULL;
                switch( this->NType )
                {
                    case Procedural::NT_Perlin:
                    {
                        NoiseGen = new PerlinNoise();
                        break;
                    }
                    case Procedural::NT_White:
                    default:
                    {
                        NoiseGen = new WhiteNoise(this->GenSeed);
                        break;
                    }
                }

                UInt8* Field = NoiseGen->Field2D( Buffer.GetWidth(), Buffer.GetHeight() );
                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = (Real)Field[Y * Buffer.GetWidth() + X];
                        Buffer.SetRedByte( X, Y, (UInt8)( NoiseVal * this->GenColour.RedChannel ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)( NoiseVal * this->GenColour.GreenChannel ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)( NoiseVal * this->GenColour.BlueChannel ) );
                        Buffer.SetAlphaByte( X, Y, (UInt8)( this->GenColour.AlphaChannel * 255.0 ) );
                    }
                }

                delete[] Field;
                delete NoiseGen;
            }

            String NoiseGenerator::GetName() const
                { return "NoiseGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            NoiseGenerator& NoiseGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            NoiseGenerator& NoiseGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            NoiseGenerator& NoiseGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            NoiseGenerator& NoiseGenerator::SetType(const Procedural::NoiseType Type)
            {
                this->NType = Type;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // SolidGenerator Methods

            SolidGenerator::SolidGenerator() :
                GenColour(1.0,1.0,1.0,1.0)
                {  }

            SolidGenerator::~SolidGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void SolidGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Buffer.SetPixel(X,Y,this->GenColour);
                    }
                }
            }

            String SolidGenerator::GetName() const
                { return "SolidGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            SolidGenerator& SolidGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            SolidGenerator& SolidGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // TextileGenerator Methods

            TextileGenerator::TextileGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120)
                {  }

            TextileGenerator::~TextileGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void TextileGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                srand(this->GenSeed);
                int RandNum = rand();
                PerlinNoise Noise(3, 0.65, 1.0 / 8.0, 1.0);
                Real FilterLevel = 0.7;
                Real PreserveLevel = 0.3;

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real NoiseVal = std::max( 0.0, std::min( 1.0, ( MathTools::Sin( X + Noise.Noise2D( X + RandNum, Y + RandNum ) ) + MathTools::Sin( Y + Noise.Noise2D( X + RandNum, Y + RandNum ) ) ) * 0.25 + 0.5 ) );
                        Buffer.SetRedByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.RedChannel * 255.0 + FilterLevel * this->GenColour.RedChannel * 255.0 * NoiseVal, 255.0) );
                        Buffer.SetGreenByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.GreenChannel * 255.0 + FilterLevel * this->GenColour.GreenChannel * 255.0 * NoiseVal, 255.0) );
                        Buffer.SetBlueByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.BlueChannel * 255.0 + FilterLevel * this->GenColour.BlueChannel * 255.0 * NoiseVal, 255.0) );
                        Buffer.SetAlphaByte( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String TextileGenerator::GetName() const
                { return "TextileGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            TextileGenerator& TextileGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            TextileGenerator& TextileGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            TextileGenerator& TextileGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // WoodGenerator Methods

            WoodGenerator::WoodGenerator() :
                GenColour(1.0,1.0,1.0,1.0),
                GenSeed(5120),
                GenRings(8)
                {  }

            WoodGenerator::WoodGenerator(const Whole Rings, const ColourValue& Colour) :
                GenColour(Colour),
                GenSeed(5120),
                GenRings(Rings)
                {  }

            WoodGenerator::WoodGenerator(const Whole Seed, const Whole Rings, const ColourValue& Colour) :
                GenColour(Colour),
                GenSeed(Seed),
                GenRings(Rings)
                {  }

            WoodGenerator::~WoodGenerator()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void WoodGenerator::AddToTextureBuffer(TextureBuffer& Buffer) const
            {
                srand(this->GenSeed);
                int RandNum = rand();
                Real FilterLevel = 0.7;
                Real PreserveLevel = 0.3;

                PerlinNoise Noise( 8, 0.5, 1.0 / 32.0, 0.05 );
                Whole w2 = Buffer.GetWidth() / 2;
                Whole h2 = Buffer.GetHeight() / 2;

                for( Whole Y = 0 ; Y < Buffer.GetHeight() ; ++Y )
                {
                    for( Whole X = 0 ; X < Buffer.GetWidth() ; ++X )
                    {
                        Real xv = ( (Real)(X - w2) ) / (Real)Buffer.GetWidth();
                        Real yv = ( (Real)(Y - h2) ) / (Real)Buffer.GetHeight();
                        Real NoiseVal = std::min<Real>( Real(1.0), MathTools::Abs( MathTools::Sin( ( sqrt(xv * xv + yv * yv) + Noise.Noise2D(X + RandNum, Y + RandNum) ) * MathTools::GetPi() * 2 * this->GenRings ) ) );
                        Buffer.SetRedByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.RedChannel * 255.0 + FilterLevel * this->GenColour.RedChannel * 255.0 * NoiseVal, Real(255.0) ) );
                        Buffer.SetGreenByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.GreenChannel * 255.0 + FilterLevel * this->GenColour.GreenChannel * 255.0 * NoiseVal, Real(255.0) ) );
                        Buffer.SetBlueByte( X, Y, (UInt8)std::min<Real>( PreserveLevel * this->GenColour.BlueChannel * 255.0 + FilterLevel * this->GenColour.BlueChannel * 255.0 * NoiseVal, Real(255.0) ) );
                        Buffer.SetAlphaByte( X, Y, this->GenColour.AlphaChannel );
                    }
                }
            }

            String WoodGenerator::GetName() const
                { return "WoodGenerator"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            WoodGenerator& WoodGenerator::SetColour(const ColourValue& Colour)
            {
                this->GenColour = Colour;
                return *this;
            }

            WoodGenerator& WoodGenerator::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->GenColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            WoodGenerator& WoodGenerator::SetSeed(const Whole Seed)
            {
                this->GenSeed = Seed;
                return *this;
            }

            WoodGenerator& WoodGenerator::SetRings(const Whole Rings)
            {
                this->GenRings = Rings;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
