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
#ifndef _graphicsproceduraledgedetectionmodifier_cpp
#define _graphicsproceduraledgedetectionmodifier_cpp

#include "Graphics/Procedural/Texture/edgedetectionmodifier.h"
#include "Graphics/Procedural/Texture/solidgenerator.h"
#include "Graphics/Procedural/Texture/blurmodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            EdgeDetectionModifier::EdgeDetectionModifier() :
                EdgeDetect(EdgeDetectionModifier::DM_Sobel),
                LowerThreshold(20),
                UpperThreshold(100),
                EdgeDetectionSigma(92)
                {  }

            EdgeDetectionModifier::~EdgeDetectionModifier()
                {  }

            void EdgeDetectionModifier::ProcessHomogenity(TextureBuffer& Buffer)
            {
                Integer TargetWidth = static_cast<Integer>( Buffer.GetWidth() );
                Integer TargetHeight = static_cast<Integer>( Buffer.GetHeight() );

                TextureBuffer TempBuffer(Buffer.GetWidth(),Buffer.GetHeight());
                SolidGenerator().SetColour(ColourValue::Black()).AddToTextureBuffer(TempBuffer);

                for( Integer Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Integer X = 0 ; X < TargetWidth ; ++X )
                    {
                        Real PixelAlpha = Buffer.GetAlphaReal(static_cast<Whole>(X),static_cast<Whole>(Y));
                        VectorBlock Block = this->GetBlock(X,Y,Buffer);
                        Vector3 CenterColour = Block[4];
                        Vector3 Difference(0.0,0.0,0.0);
                        for( Integer Row = 0 ; Row < 3 ; ++Row )
                        {
                            for( Integer Column = 0 ; Column < 3 ; ++Column )
                            {
                                if( Row != 1 || Column != 1 ) {
                                    Vector3 Temp = CenterColour - Block[Row * 3 + Column];
                                    if( MathTools::Abs(Temp.X) > Difference.X ) Difference.X = MathTools::Abs(Temp.X);
                                    if( MathTools::Abs(Temp.Y) > Difference.Y ) Difference.Y = MathTools::Abs(Temp.Y);
                                    if( MathTools::Abs(Temp.Z) > Difference.Z ) Difference.Z = MathTools::Abs(Temp.Z);
                                }
                            }
                        }
                        TempBuffer.SetPixel(static_cast<Whole>(X),static_cast<Whole>(Y),ColourValue(Difference.X,Difference.Y,Difference.Z,PixelAlpha));
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            void EdgeDetectionModifier::ProcessDifference(TextureBuffer& Buffer)
            {
                Integer TargetWidth = static_cast<Integer>( Buffer.GetWidth() );
                Integer TargetHeight = static_cast<Integer>( Buffer.GetHeight() );

                TextureBuffer TempBuffer(Buffer.GetWidth(),Buffer.GetHeight());
                SolidGenerator().SetColour(ColourValue::Black()).AddToTextureBuffer(TempBuffer);

                for( Integer Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Integer X = 0 ; X < TargetWidth ; ++X )
                    {
                        Real PixelAlpha = Buffer.GetAlphaReal(static_cast<Whole>(X),static_cast<Whole>(Y));
                        VectorBlock Block = this->GetBlock(X,Y,Buffer);
                        Vector3 Difference(0.0,0.0,0.0);
                        Vector3 Temp(0.0,0.0,0.0);

                        for( Integer Index = 0 ; Index < 3 ; ++Index )
                        {
                            Temp = Block[Index] - Block[6 + (2 - Index)];
                            if( MathTools::Abs(Temp.X) > Difference.X ) Difference.X = MathTools::Abs(Temp.X);
                            if( MathTools::Abs(Temp.Y) > Difference.Y ) Difference.Y = MathTools::Abs(Temp.Y);
                            if( MathTools::Abs(Temp.Z) > Difference.Z ) Difference.Z = MathTools::Abs(Temp.Z);
                        }
                        Temp = Block[5] - Block[3];
                        if( MathTools::Abs(Temp.X) > Difference.X ) Difference.X = MathTools::Abs(Temp.X);
                        if( MathTools::Abs(Temp.Y) > Difference.Y ) Difference.Y = MathTools::Abs(Temp.Y);
                        if( MathTools::Abs(Temp.Z) > Difference.Z ) Difference.Z = MathTools::Abs(Temp.Z);

                        TempBuffer.SetPixel(static_cast<Whole>(X),static_cast<Whole>(Y),ColourValue(Difference.X,Difference.Y,Difference.Z,PixelAlpha));
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            void EdgeDetectionModifier::ProcessSobel(TextureBuffer& Buffer)
            {
                Integer TargetWidth = static_cast<Integer>( Buffer.GetWidth() );
                Integer TargetHeight = static_cast<Integer>( Buffer.GetHeight() );

                TextureBuffer TempBuffer(Buffer.GetWidth(),Buffer.GetHeight());
                SolidGenerator().SetColour(ColourValue::Black()).AddToTextureBuffer(TempBuffer);

                for( Integer Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Integer X = 0 ; X < TargetWidth ; ++X )
                    {
                        Real PixelAlpha = Buffer.GetAlphaReal(static_cast<Whole>(X),static_cast<Whole>(Y));
                        VectorBlock Block = this->GetBlock(X,Y,Buffer);

                        Vector3 Temp1 = Block[0] + ( Block[1] * 2.0 ) + Block[2] - Block[6] - ( Block[7] * 2.0 ) - Block[8];
                        Temp1 = Vector3(MathTools::Abs(Temp1.X),MathTools::Abs(Temp1.Y),MathTools::Abs(Temp1.Z));
                        Vector3 Temp2 = Block[2] + ( Block[5] * 2.0 ) + Block[8] - Block[0] - ( Block[3] * 2.0 ) - Block[6];
                        Temp2 = Vector3(MathTools::Abs(Temp2.X),MathTools::Abs(Temp2.Y),MathTools::Abs(Temp2.Z));
                        Vector3 Difference = Temp1 + Temp2;

                        TempBuffer.SetPixel(static_cast<Whole>(X),static_cast<Whole>(Y),ColourValue(Difference.X,Difference.Y,Difference.Z,PixelAlpha));
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            void EdgeDetectionModifier::ProcessCanny(TextureBuffer& Buffer)
            {
                Integer TargetWidth = static_cast<Integer>( Buffer.GetWidth() );
                Integer TargetHeight = static_cast<Integer>( Buffer.GetHeight() );

                TextureBuffer TempBuffer(Buffer.GetWidth(),Buffer.GetHeight());
                SolidGenerator().SetColour(ColourValue::Black()).AddToTextureBuffer(TempBuffer);

                // STEP 1 - blur image
                BlurModifier().SetSigma(this->EdgeDetectionSigma).SetBlurType(BlurModifier::BT_Gaussian).Modify(Buffer);

                // STEP 2 - calculate magnitude and edge Orientations
                Real Div = 0.0;
                VectorContainer Orientations(TargetWidth * TargetHeight);
                VectorContainer Gradients(TargetWidth * TargetHeight);
                Vector3 Difference = Vector3(-std::numeric_limits<Real>::infinity(),-std::numeric_limits<Real>::infinity(),-std::numeric_limits<Real>::infinity());
                for( Integer Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Integer X = 0 ; X < TargetWidth ; ++X )
                    {
                        VectorContainer Block = this->GetBlock(X,Y,Buffer);
                        Vector3 TempVec1 = Block[2] + Block[8] - Block[0] - Block[6] + ( ( Block[5] - Block[3] ) * 2.0 );
                        Vector3 TempVec2 = Block[0] + Block[2] - Block[6] - Block[8] + ( ( Block[1] - Block[7] ) * 2.0 );
                        Gradients[Y * TargetWidth + X] = Vector3(MathTools::Sqrt(TempVec1.X * TempVec1.X + TempVec2.X * TempVec2.X), MathTools::Sqrt(TempVec1.Y * TempVec1.Y + TempVec2.Y * TempVec2.Y), MathTools::Sqrt(TempVec1.Z * TempVec1.Z + TempVec2.Z * TempVec2.Z));
                        if( Gradients[Y * TargetWidth + X].X > Difference.X ) Difference.X = Gradients[Y * TargetWidth + X].X;
                        if( Gradients[Y * TargetWidth + X].Y > Difference.Y ) Difference.Y = Gradients[Y * TargetWidth + X].Y;
                        if( Gradients[Y * TargetWidth + X].Z > Difference.Z ) Difference.Z = Gradients[Y * TargetWidth + X].Z;
                        Orientations[Y * TargetWidth + X].Zero();
                        if( TempVec1.X == 0.0 ) {
                            Orientations[Y * TargetWidth + X].X = ( TempVec2.X == 0.0 ) ? 0.0 : 90.0;
                        }else{
                            Div = TempVec2.X / TempVec1.X;
                            if( Div < 0.0 ) {
                                Orientations[Y * TargetWidth + X].X = 180.0 - ( MathTools::ATan(-Div) * MathTools::GetRadToDegMultiplier() );
                            }else{
                                Orientations[Y * TargetWidth + X].X = ( MathTools::ATan(Div) * MathTools::GetRadToDegMultiplier() );
                            }

                            if( Orientations[Y * TargetWidth + X].X < 22.5 ) {
                                Orientations[Y * TargetWidth + X].X = 0.0;
                            }else if( Orientations[Y * TargetWidth + X].X < 67.5 ) {
                                Orientations[Y * TargetWidth + X].X = 45.0;
                            }else if( Orientations[Y * TargetWidth + X].X < 112.5 ) {
                                Orientations[Y * TargetWidth + X].X = 90.0;
                            }else if( Orientations[Y * TargetWidth + X].X < 157.5 ) {
                                Orientations[Y * TargetWidth + X].X = 135.0;
                            }else{
                                Orientations[Y * TargetWidth + X].X = 0.0;
                            }
                        }
                        if( TempVec1.Y == 0.0 ) {
                            Orientations[Y * TargetWidth + X].Y = ( TempVec2.Y == 0.0 ) ? 0.0 : 90.0;
                        }else{
                            Div = TempVec2.Y / TempVec1.Y;
                            if( Div < 0.0 ) {
                                Orientations[Y * TargetWidth + X].Y = 180.0 - ( MathTools::ATan(-Div) * MathTools::GetRadToDegMultiplier() );
                            }else{
                                Orientations[Y * TargetWidth + X].Y = ( MathTools::ATan(Div) * MathTools::GetRadToDegMultiplier() );
                            }

                            if( Orientations[Y * TargetWidth + X].Y < 22.5 ) {
                                Orientations[Y * TargetWidth + X].Y = 0.0;
                            }else if( Orientations[Y * TargetWidth + X].Y < 67.5 ) {
                                Orientations[Y * TargetWidth + X].Y = 45.0;
                            }else if( Orientations[Y * TargetWidth + X].Y < 112.5 ) {
                                Orientations[Y * TargetWidth + X].Y = 90.0;
                            }else if( Orientations[Y * TargetWidth + X].Y < 157.5 ) {
                                Orientations[Y * TargetWidth + X].Y = 135.0;
                            }else{
                                Orientations[Y * TargetWidth + X].Y = 0.0;
                            }
                        }
                        if( TempVec1.Z == 0.0 ) {
                            Orientations[Y * TargetWidth + X].Z = ( TempVec2.Z == 0.0 ) ? 0.0 : 90.0;
                        }else{
                            Div = TempVec2.Z / TempVec1.Z;
                            if( Div < 0.0 ) {
                                Orientations[Y * TargetWidth + X].Z = 180.0 - ( MathTools::ATan(-Div) * MathTools::GetRadToDegMultiplier() );
                            }else{
                                Orientations[Y * TargetWidth + X].Z = ( MathTools::ATan(Div) * MathTools::GetRadToDegMultiplier() );
                            }

                            if( Orientations[Y * TargetWidth + X].Z < 22.5 ) {
                                Orientations[Y * TargetWidth + X].Z = 0.0;
                            }else if( Orientations[Y * TargetWidth + X].Z < 67.5 ) {
                                Orientations[Y * TargetWidth + X].Z = 45.0;
                            }else if( Orientations[Y * TargetWidth + X].Z < 112.5 ) {
                                Orientations[Y * TargetWidth + X].Z = 90.0;
                            }else if( Orientations[Y * TargetWidth + X].Z < 157.5 ) {
                                Orientations[Y * TargetWidth + X].Z = 135.0;
                            }else{
                                Orientations[Y * TargetWidth + X].Z = 0.0;
                            }
                        }
                    }
                }

                // STEP 3 - suppres non maximums
                for( Integer Y = 1 ; Y < ( TargetHeight - 1 ) ; ++Y )
                {
                    for( Integer X = 1 ; X < ( TargetWidth - 1 ) ; ++X )
                    {
                        Div = Gradients[Y * TargetWidth + X].X / Difference.X;
                        switch( static_cast<Integer>( Orientations[Y * TargetWidth + X].X ) )
                        {
                            default:
                            case 0:
                            {
                                if( ( Gradients[Y * TargetWidth + X].X < Gradients[Y * TargetWidth + (X - 1)].X ) || ( Gradients[Y * TargetWidth + X].X < Gradients[Y * TargetWidth + (X + 1)].X ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 45:
                            {
                                if( ( Gradients[Y * TargetWidth + X].X < Gradients[(Y + 1) * TargetWidth + (X - 1)].X ) || ( Gradients[Y * TargetWidth + X].X < Gradients[(Y - 1) * TargetWidth + (X + 1)].X ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 90:
                            {
                                if( ( Gradients[Y * TargetWidth + X].X < Gradients[(Y + 1) * TargetWidth + X].X ) || ( Gradients[Y * TargetWidth + X].X < Gradients[(Y - 1) * TargetWidth + X].X ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 135:
                            {
                                if( ( Gradients[Y * TargetWidth + X].X < Gradients[(Y + 1) * TargetWidth + (X + 1)].X ) || ( Gradients[Y * TargetWidth + X].X < Gradients[(Y - 1) * TargetWidth + (X - 1)].X ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                        }
                        TempBuffer.SetRedReal( static_cast<Whole>(X), static_cast<Whole>(Y), Div );
                        Div = Gradients[Y * TargetWidth + X].Y / Difference.Y;
                        switch( static_cast<Integer>( Orientations[Y * TargetWidth + X].Y ) )
                        {
                            default:
                            case 0:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Y < Gradients[Y * TargetWidth + (X - 1)].Y ) || ( Gradients[Y * TargetWidth + X].Y < Gradients[Y * TargetWidth + (X + 1)].Y ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 45:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Y < Gradients[(Y + 1) * TargetWidth + (X - 1)].Y ) || ( Gradients[Y * TargetWidth + X].Y < Gradients[(Y - 1) * TargetWidth + (X + 1)].Y ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 90:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Y < Gradients[(Y + 1) * TargetWidth + X].Y ) || ( Gradients[Y * TargetWidth + X].Y < Gradients[(Y - 1) * TargetWidth + X].Y ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 135:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Y < Gradients[(Y + 1) * TargetWidth + (X + 1)].Y ) || ( Gradients[Y * TargetWidth + X].Y < Gradients[(Y - 1) * TargetWidth + (X - 1)].Y ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                        }
                        TempBuffer.SetGreenReal( static_cast<Whole>(X), static_cast<Whole>(Y), Div );
                        Div = Gradients[Y * TargetWidth + X].Z / Difference.Z;
                        switch( static_cast<Integer>( Orientations[Y * TargetWidth + X].Z ) )
                        {
                            default:
                            case 0:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Z < Gradients[Y * TargetWidth + (X - 1)].Z ) || ( Gradients[Y * TargetWidth + X].Z < Gradients[Y * TargetWidth + (X + 1)].Z ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 45:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Z < Gradients[(Y + 1) * TargetWidth + (X - 1)].Z ) || ( Gradients[Y * TargetWidth + X].Z < Gradients[(Y - 1) * TargetWidth + (X + 1)].Z ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 90:
                            {
                                if( ( Gradients[Y * TargetWidth + X].Z < Gradients[(Y + 1) * TargetWidth + X].Z ) || ( Gradients[Y * TargetWidth + X].Z < Gradients[(Y - 1) * TargetWidth + X].Z ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                            case 135:
                            {
                                if( (Gradients[Y * TargetWidth + X].Z < Gradients[(Y + 1) * TargetWidth + (X + 1)].Z ) || ( Gradients[Y * TargetWidth + X].Z < Gradients[(Y - 1) * TargetWidth + (X - 1)].Z ) ) {
                                    Div = 0.0;
                                }
                                break;
                            }
                        }
                        TempBuffer.SetBlueReal( static_cast<Whole>(X), static_cast<Whole>(Y), Div );
                    }
                }

                // STEP 4 - hysteresis
                Buffer.SetData(TempBuffer);
                Real MaxThreshold = static_cast<Real>( this->UpperThreshold ) / 255.0;
                for( Integer Y = 1 ; Y < ( TargetHeight - 1 ) ; ++Y )
                {
                    for( Integer X = 1 ; X < ( TargetWidth - 1 ) ; ++X )
                    {
                        ColourValue Pixel = Buffer.GetPixel(static_cast<Whole>(X), static_cast<Whole>(Y));
                        if( Pixel.RedChannel < MaxThreshold )
                        {
                            if( Pixel.RedChannel < static_cast<Real>( this->LowerThreshold ) / 255.0 )
                                TempBuffer.SetRedReal( static_cast<Whole>(X), static_cast<Whole>(Y), 0.0 );
                            else
                            {
                                if( ( Buffer.GetRedReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y    )) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y    )) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X    ),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y + 1)) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X    ),static_cast<Whole>(Y + 1)) < MaxThreshold ) &&
                                    ( Buffer.GetRedReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y + 1)) < MaxThreshold ) )
                                {
                                    TempBuffer.SetRedReal( static_cast<Whole>(X), static_cast<Whole>(Y), 0.0 );
                                }
                            }
                        }
                        if( Pixel.GreenChannel < MaxThreshold )
                        {
                            if( Pixel.GreenChannel < static_cast<Real>( this->LowerThreshold ) / 255.0 )
                                TempBuffer.SetGreenReal( static_cast<Whole>(X), static_cast<Whole>(Y), 0.0 );
                            else
                            {
                                if( ( Buffer.GetGreenReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y    )) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y    )) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X    ),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y + 1)) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X    ),static_cast<Whole>(Y + 1)) < MaxThreshold ) &&
                                    ( Buffer.GetGreenReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y + 1)) < MaxThreshold ) )
                                {
                                    TempBuffer.SetGreenReal( static_cast<Whole>(X), static_cast<Whole>(Y), 0.0 );
                                }
                            }
                        }
                        if( Pixel.BlueChannel < MaxThreshold )
                        {
                            if( Pixel.BlueChannel < static_cast<Real>( this->LowerThreshold ) / 255.0 )
                                TempBuffer.SetBlueReal( static_cast<Whole>(X), static_cast<Whole>(Y), 0.0 );
                            else
                            {
                                if( ( Buffer.GetBlueReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y    )) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y    )) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X    ),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y - 1)) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X - 1),static_cast<Whole>(Y + 1)) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X    ),static_cast<Whole>(Y + 1)) < MaxThreshold ) &&
                                    ( Buffer.GetBlueReal(static_cast<Whole>(X + 1),static_cast<Whole>(Y + 1)) < MaxThreshold ) )
                                {
                                    TempBuffer.SetBlueReal( static_cast<Whole>(X), static_cast<Whole>(Y), 0.0 );
                                }
                            }
                        }
                    }
                }

                Buffer.SetData(TempBuffer);
            }

            EdgeDetectionModifier::VectorBlock EdgeDetectionModifier::GetBlock(const Integer X, const Integer Y, TextureBuffer& Buffer) const
            {
                VectorBlock Block(9);
                ColourValue Pixel = Buffer.GetPixel(static_cast<Whole>(X),static_cast<Whole>(Y));
                for( Integer Row = -1 ; Row < 2 ; ++Row )
                {
                    for( Integer Column = -1 ; Column < 2 ; ++Column )
                    {
                        Integer CurrIndex = ( Row + 1 ) * 3 + ( Column + 1 );
                        // Assign safe default of no change
                        Block[CurrIndex].SetValues(Pixel.RedChannel,Pixel.GreenChannel,Pixel.BlueChannel);
                        if( Row != 0 || Column != 0 ) {
                            if( ( X + Column ) < 0 || ( X + Column ) >= static_cast<Integer>( Buffer.GetWidth() ) ) {
                                continue;
                            }
                            if( ( Y + Row ) < 0 || ( Y + Row ) >= static_cast<Integer>( Buffer.GetHeight() ) ) {
                                continue;
                            }
                            // Apply the change now that we know we have a valid pixel that isn't the current Pixel
                            Block[CurrIndex].SetValues( Buffer.GetRedReal(static_cast<Whole>(X+Column),static_cast<Whole>(Y+Row)),
                                                        Buffer.GetGreenReal(static_cast<Whole>(X+Column),static_cast<Whole>(Y+Row)),
                                                        Buffer.GetBlueReal(static_cast<Whole>(X+Column),static_cast<Whole>(Y+Row)) );
                        }
                    }
                }
                return Block;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void EdgeDetectionModifier::Modify(TextureBuffer& Buffer)
            {
                switch( this->EdgeDetect )
                {
                    case EdgeDetectionModifier::DM_Homogenity:    this->ProcessHomogenity(Buffer);      break;
                    case EdgeDetectionModifier::DM_Difference:    this->ProcessDifference(Buffer);      break;
                    default:
                    case EdgeDetectionModifier::DM_Sobel:         this->ProcessSobel(Buffer);           break;
                    case EdgeDetectionModifier::DM_Canny:         this->ProcessCanny(Buffer);           break;
                }
            }

            String EdgeDetectionModifier::GetName() const
                { return "EdgeDetectionModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            EdgeDetectionModifier& EdgeDetectionModifier::SetDetectionType(const DetectionMode Mode)
            {
                this->EdgeDetect = Mode;
                return *this;
            }

            EdgeDetectionModifier& EdgeDetectionModifier::SetLowerThreshold(const UInt8 Low)
            {
                this->LowerThreshold = Low;
                return *this;
            }

            EdgeDetectionModifier& EdgeDetectionModifier::SetUpperThreshold(const UInt8 High)
            {
                this->UpperThreshold = High;
                return *this;
            }

            EdgeDetectionModifier& EdgeDetectionModifier::SetEdgeDetectionSigma(const UInt8 Sigma)
            {
                this->EdgeDetectionSigma = Sigma;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
