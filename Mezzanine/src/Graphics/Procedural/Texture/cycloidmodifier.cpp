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
#ifndef _graphicsproceduralcycloidmodifier_cpp
#define _graphicsproceduralcycloidmodifier_cpp

#include "Graphics/Procedural/Texture/cycloidmodifier.h"

#include "MathTools/mathtools.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CycloidModifier::CycloidModifier() :
                CycloidColour(1.0,1.0,1.0,1.0),
                CycloidCenter(0.5,0.5),
                CycloidPenSize(1),
                Parameter_R(0),
                Parameter_r(0),
                Parameter_d(0),
                Parameter_e(0)
                { this->SetCycloidType(CycloidModifier::CT_Hypocycloid); }

            CycloidModifier::~CycloidModifier()
                {  }

            void CycloidModifier::ProcessHypocycloid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                Integer CurrX = 0;
                Integer CurrY = 0;
                Real Phi = 0;

                Integer SignedX = XPos + static_cast<Integer>( MathTools::Floor( this->Parameter_R + 0.5 ) );
                Integer SignedY = YPos;
                do
                {
                    Real DeltaX = ( this->Parameter_R - this->Parameter_r ) * MathTools::Cos(Phi) + this->Parameter_r * MathTools::Cos( ( ( this->Parameter_R - this->Parameter_r ) / this->Parameter_r ) * Phi );
                    Real DeltaY = ( this->Parameter_R - this->Parameter_r ) * MathTools::Sin(Phi) - this->Parameter_r * MathTools::Sin( ( ( this->Parameter_R - this->Parameter_r ) / this->Parameter_r ) * Phi );

                    CurrX = XPos + static_cast<Integer>( MathTools::Floor( DeltaX + 0.5 ) );
                    CurrY = YPos - static_cast<Integer>( MathTools::Floor( DeltaY + 0.5 ) );
                    this->Paint(CurrX,CurrY,Step,Buffer);

                    Phi += Step;
                }
                while( !( SignedX == CurrX && SignedY == CurrY && Phi < 100.0 * MathTools::GetPi()) || Phi < MathTools::GetTwoPi() );
            }

            void CycloidModifier::ProcessHypotrochoid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                Integer CurrX = 0;
                Integer CurrY = 0;
                Real Phi = 0;

                Integer SignedX = XPos + static_cast<Integer>( MathTools::Floor( ( this->Parameter_R - this->Parameter_r ) + this->Parameter_d + 0.5 ) );
                Integer SignedY = YPos;
                do
                {
                    Real DeltaX = ( this->Parameter_R - this->Parameter_r ) * MathTools::Cos(Phi) + this->Parameter_d * MathTools::Cos( ( ( this->Parameter_R - this->Parameter_r ) / this->Parameter_r ) * Phi);
                    Real DeltaY = ( this->Parameter_R - this->Parameter_r ) * MathTools::Sin(Phi) - this->Parameter_d * MathTools::Sin( ( ( this->Parameter_R - this->Parameter_r ) / this->Parameter_r ) * Phi);

                    CurrX = XPos + static_cast<Integer>( MathTools::Floor( DeltaX + 0.5 ) );
                    CurrY = YPos - static_cast<Integer>( MathTools::Floor( DeltaY + 0.5 ) );
                    this->Paint(CurrX,CurrY,Step,Buffer);

                    Phi += Step;
                }
                while( !( SignedX == CurrX && SignedY == CurrY && Phi < 100.0 * MathTools::GetPi() ) || Phi < MathTools::GetTwoPi() );
            }

            void CycloidModifier::ProcessEpicycloid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                Integer CurrX = 0;
                Integer CurrY = 0;
                Real Phi = 0;

                Integer SignedX = XPos + static_cast<Integer>( MathTools::Floor( (this->Parameter_R + this->Parameter_r) - this->Parameter_r + 0.5 ) );
                Integer SignedY = YPos;
                do
                {
                    Real DeltaX = ( this->Parameter_R + this->Parameter_r ) * MathTools::Cos(Phi) - this->Parameter_r * MathTools::Cos( ( (this->Parameter_R + this->Parameter_r) / this->Parameter_r) * Phi);
                    Real DeltaY = ( this->Parameter_R + this->Parameter_r ) * MathTools::Sin(Phi) - this->Parameter_r * MathTools::Sin( ( (this->Parameter_R + this->Parameter_r) / this->Parameter_r) * Phi);

                    CurrX = XPos + static_cast<Integer>( MathTools::Floor( DeltaX + 0.5 ) );
                    CurrY = YPos - static_cast<Integer>( MathTools::Floor( DeltaY + 0.5 ) );
                    this->Paint(CurrX,CurrY,Step,Buffer);

                    Phi += Step;
                }
                while( !( SignedX == CurrX && SignedY == CurrY && Phi < 100.0 * MathTools::GetPi() ) || Phi < MathTools::GetTwoPi() );
            }

            void CycloidModifier::ProcessEpitrochoid(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                Integer CurrX = 0;
                Integer CurrY = 0;
                Real Phi = 0;

                Integer SignedX = XPos + static_cast<Integer>( MathTools::Floor( (this->Parameter_R + this->Parameter_r) - this->Parameter_d + 0.5 ) );
                Integer SignedY = YPos;
                do
                {
                    Real DeltaX = ( this->Parameter_R + this->Parameter_r ) * MathTools::Cos(Phi) - this->Parameter_d * MathTools::Cos( ( (this->Parameter_R + this->Parameter_r) / this->Parameter_r ) * Phi);
                    Real DeltaY = ( this->Parameter_R + this->Parameter_r ) * MathTools::Sin(Phi) - this->Parameter_d * MathTools::Sin( ( (this->Parameter_R + this->Parameter_r) / this->Parameter_r ) * Phi);

                    CurrX = XPos + static_cast<Integer>( MathTools::Floor( DeltaX + 0.5 ) );
                    CurrY = YPos - static_cast<Integer>( MathTools::Floor( DeltaY + 0.5 ) );
                    this->Paint(CurrX,CurrY,Step,Buffer);

                    Phi += Step;
                }
                while( !( SignedX == CurrX && SignedY == CurrY && Phi < 100.0 * MathTools::GetPi() ) || Phi < MathTools::GetTwoPi() );
            }

            void CycloidModifier::ProcessRoseCurve(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                Integer CurrX = 0;
                Integer CurrY = 0;
                Real t = 0;
                Real k = this->Parameter_r / this->Parameter_d;
                Real ActualStep = Step / 10.0;

                //Integer SignedX = XPos;
                //Integer SignedY = YPos;
                do
                {
                    Real DeltaX = this->Parameter_R * MathTools::Cos(k * t) * MathTools::Sin(t);
                    Real DeltaY = this->Parameter_R * MathTools::Cos(k * t) * MathTools::Cos(t);

                    CurrX = XPos + static_cast<Integer>( MathTools::Floor( DeltaX + 0.5 ) );
                    CurrY = YPos - static_cast<Integer>( MathTools::Floor( DeltaY + 0.5 ) );
                    this->Paint(CurrX,CurrY,ActualStep,Buffer);

                    t += ActualStep;
                }
                while( t <= MathTools::GetTwoPi() );
            }

            void CycloidModifier::ProcessLissajousCurve(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                Integer CurrX = 0;
                Integer CurrY = 0;
                Real t = 0;
                Real ActualStep = Step / 10.0;

                //Integer SignedX = XPos;
                //Integer SignedY = YPos;
                do
                {
                    Real DeltaX = this->Parameter_R * MathTools::Sin( this->Parameter_r * t + this->Parameter_e );
                    Real DeltaY = this->Parameter_R * MathTools::Cos( this->Parameter_d * t + this->Parameter_e );

                    CurrX = XPos + static_cast<Integer>( MathTools::Floor( DeltaX + 0.5 ) );
                    CurrY = YPos - static_cast<Integer>( MathTools::Floor( DeltaY + 0.5 ) );
                    this->Paint(CurrX,CurrY,ActualStep,Buffer);

                    t += ActualStep;
                }
                while( t <= MathTools::GetTwoPi() );
            }

            void CycloidModifier::Paint(const Integer XPos, const Integer YPos, const Real Step, TextureBuffer& Buffer)
            {
                if( this->CycloidPenSize == 1 ) {
                    if( XPos < 0 || YPos < 0 || XPos >= static_cast<Integer>( Buffer.GetWidth() ) || YPos >= static_cast<Integer>( Buffer.GetHeight() ) ) {
                        return;
                    }
                    Buffer.SetPixel(XPos,YPos,this->CycloidColour);
                }else{
                    for( Real Phi = 0 ; Phi <= MathTools::GetTwoPi() ; Phi += Step )
                    {
                        Real DeltaX = MathTools::Cos(Phi);
                        Real DeltaY = MathTools::Sin(Phi);
                        for( Whole r = 0 ; r < this->CycloidPenSize ; ++r )
                        {
                            Integer CurrX = XPos + static_cast<Integer>( MathTools::Floor( static_cast<Real>(r) * DeltaX + 0.5 ) );
                            Integer CurrY = YPos - static_cast<Integer>( MathTools::Floor( static_cast<Real>(r) * DeltaY + 0.5 ) );
                            if( CurrX >= 0 && CurrY >= 0 && CurrX < static_cast<Integer>( Buffer.GetWidth() ) && CurrY < static_cast<Integer>( Buffer.GetHeight() ) ) {
                                Buffer.SetPixel(CurrX,CurrY,this->CycloidColour);
                            }
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void CycloidModifier::Modify(TextureBuffer& Buffer)
            {
                if( this->CycloidPenSize != 0 ) {
                    Integer XPos = static_cast<Integer>( static_cast<Real>( Buffer.GetWidth() * this->CycloidCenter.X ) );
                    Integer YPos = static_cast<Integer>( static_cast<Real>( Buffer.GetHeight() * this->CycloidCenter.Y ) );
                    Real Step = MathTools::GetPi() / static_cast<Real>( std::min( Buffer.GetHeight(), Buffer.GetWidth() ) );
                    switch( this->Type )
                    {
                        default:
                            { break; }
                        case CycloidModifier::CT_Hypocycloid:
                        {
                            this->ProcessHypocycloid(XPos,YPos,Step,Buffer);
                            break;
                        }
                        case CycloidModifier::CT_Hypotrochoid:
                        {
                            this->ProcessHypotrochoid(XPos,YPos,Step,Buffer);
                            break;
                        }
                        case CycloidModifier::CT_Epicycloid:
                        {
                            this->ProcessEpicycloid(XPos,YPos,Step,Buffer);
                            break;
                        }
                        case CycloidModifier::CT_Epitrochoid:
                        {
                            this->ProcessEpitrochoid(XPos,YPos,Step,Buffer);
                            break;
                        }
                        case CycloidModifier::CT_RoseCurve:
                        {
                            this->ProcessRoseCurve(XPos,YPos,Step,Buffer);
                            break;
                        }
                        case CycloidModifier::CT_LissajousCurve:
                        {
                            this->ProcessLissajousCurve(XPos,YPos,Step,Buffer);
                            break;
                        }
                    }
                }
            }

            String CycloidModifier::GetName() const
                { return "CycloidModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            CycloidModifier& CycloidModifier::SetCycloidType(const CycloidType ToDraw)
            {
                this->Type = ToDraw;
                return *this;
            }

            CycloidModifier& CycloidModifier::SetDefaultParameters(const Whole TextureWidth, const Whole TextureHeight)
            {
                Real Size = static_cast<Real>( std::min( TextureWidth, TextureHeight ) );
                switch( this->Type )
                {
                    default:
                    case CycloidModifier::CT_Hypocycloid:
                    {
                        this->Parameter_R = 3.0 / 6.0 * Size;
                        this->Parameter_r = 1.0 / 6.0 * Size;
                        this->Parameter_d = 0.0;
                        this->Parameter_e = 0.0;
                        break;
                    }
                    case CycloidModifier::CT_Hypotrochoid:
                    {
                        this->Parameter_R = 5.0 / 14.0 * Size;
                        this->Parameter_r = 3.0 / 14.0 * Size;
                        this->Parameter_d = 5.0 / 14.0 * Size;
                        this->Parameter_e = 0.0;
                        break;
                    }
                    case CycloidModifier::CT_Epicycloid:
                    {
                        this->Parameter_R = 3.0 / 10.0 * Size;
                        this->Parameter_r = 1.0 / 10.0 * Size;
                        this->Parameter_d = 0.0;
                        this->Parameter_e = 0.0;
                        break;
                    }
                    case CycloidModifier::CT_Epitrochoid:
                    {
                        this->Parameter_R = 3.0 / 10.0 * Size;
                        this->Parameter_r = 1.0 / 10.0 * Size;
                        this->Parameter_d = 1.0 / 20.0 * Size;
                        this->Parameter_e = 0.0;
                        break;
                    }
                    case CycloidModifier::CT_RoseCurve:
                    {
                        this->Parameter_R = 0.5 * Size;
                        this->Parameter_r = 4.0;
                        this->Parameter_d = 1.0;
                        this->Parameter_e = 0.0;
                        break;
                    }
                    case CycloidModifier::CT_LissajousCurve:
                    {
                        this->Parameter_R = 0.5 * Size;
                        this->Parameter_r = 5.0;
                        this->Parameter_d = 4.0;
                        this->Parameter_e = MathTools::GetHalfPi();
                        break;
                    }
                }
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidColour(const ColourValue& Colour)
            {
                this->CycloidColour = Colour;
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
            {
                this->CycloidColour.SetValues(Red,Green,Blue,Alpha);
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidCenter(const Vector2& Center)
            {
                this->CycloidCenter = Center;
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidCenter(const Real CenterX, const Real CenterY)
            {
                this->CycloidCenter.SetValues(CenterX,CenterY);
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidCenterX(const Real X)
            {
                this->CycloidCenter.X = X;
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidCenterY(const Real Y)
            {
                this->CycloidCenter.Y = Y;
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidParameter(const CycloidParameter Param, const Real Value)
            {
                switch( Param )
                {
                    case CycloidModifier::CP_Param_R:
                    {
                        this->Parameter_R = Value;
                        break;
                    }
                    case CycloidModifier::CP_Param_r:
                    {
                        this->Parameter_r = Value;
                        break;
                    }
                    case CycloidModifier::CP_Param_d:
                    {
                        this->Parameter_d = Value;
                        break;
                    }
                    case CycloidModifier::CP_Param_e:
                    {
                        this->Parameter_e = Value;
                        break;
                    }
                    case CycloidModifier::CP_Param_k:
                    {
                        switch( Type )
                        {
                            default:
                            case CycloidModifier::CT_Hypocycloid:
                            case CycloidModifier::CT_Hypotrochoid:
                            case CycloidModifier::CT_Epicycloid:
                            case CycloidModifier::CT_Epitrochoid:
                            {
                                this->Parameter_R = Value * this->Parameter_r;
                                break;
                            }
                            case CycloidModifier::CT_RoseCurve:
                            case CycloidModifier::CT_LissajousCurve:
                            {
                                this->Parameter_r = Value * this->Parameter_d;
                                break;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                return *this;
            }

            CycloidModifier& CycloidModifier::SetCycloidPenSize(const Real Size)
            {
                this->CycloidPenSize = Size;
                return *this;
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
