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
#ifndef _graphicsproceduralabnormalsmodifier_cpp
#define _graphicsproceduralabnormalsmodifier_cpp

#include "Graphics/Procedural/Texture/abnormalsmodifier.h"

#include "MathTools/mathtools.h"
#include "quaternion.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            AbnormalsModifier::AbnormalsModifier() :
                Axis(0.0,0.0,1.0),
                Angle(0.0),
                AbnormalsTexture(NULL),
                Mirror(AbnormalsModifier::AMF_None),
                Compensation(AbnormalsModifier::AC_Normal),
                Sensitivity(127)
                {  }

            AbnormalsModifier::~AbnormalsModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void AbnormalsModifier::Modify(TextureBuffer& Buffer)
            {
                Quaternion TempQuat;
                Vector3 TempVec;
                Real Sum;

                Whole AbnormalsWidth = this->AbnormalsTexture->GetWidth();
                Whole AbnormalsHeight = this->AbnormalsTexture->GetHeight();
                Whole TargetWidth = Buffer.GetWidth();
                Whole TargetHeight = Buffer.GetHeight();
                Quaternion Rotation(this->Angle,this->Axis);

                if( AbnormalsWidth != TargetWidth && AbnormalsHeight != TargetHeight ) {
                    MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"The AbnormalsTexture size and the target texture size must be the same.");
                }

                for( Whole Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Whole X = 0 ; X < TargetWidth ; ++X )
                    {
                        ColourValue Pixel = Buffer.GetPixel(X,Y);
                        Quaternion PixelQuat(0.0, ( ( Pixel.RedChannel * 255.0 ) - 127.5 ) / 127.5, ( ( Pixel.BlueChannel * 255.0 ) - 127.5 ) / 127.5, ( ( Pixel.GreenChannel * 255.0 ) - 127.5 ) / 127.5 );

                        if( this->AbnormalsTexture != NULL ) {
                            Pixel = this->AbnormalsTexture->GetPixel(X,Y);
                            switch( this->Compensation )
                            {
                                case AbnormalsModifier::AC_Normal:
                                {
                                    TempQuat = Quaternion(0.0, ( Pixel.RedChannel * 255.0 ) - 127.5, ( Pixel.BlueChannel * 255.0 ) - 127.5, ( Pixel.GreenChannel * 255.0 ) - 127.5);
                                    PixelQuat = PixelQuat * static_cast<Real>( 1 - this->Sensitivity );
                                    PixelQuat = PixelQuat + TempQuat * ( static_cast<Real>(this->Sensitivity) / 127.5 );
                                    break;
                                }
                                case AbnormalsModifier::AC_Height:
                                {
                                    Sum = ( ( Pixel.RedChannel + Pixel.GreenChannel + Pixel.BlueChannel ) / 3.0 ) * 255.0;
                                    TempQuat = Quaternion( Real( MathTools::GetTwoPi() * Sum / 765.0 * this->Sensitivity ), Vector3(0.0,1.0,0.0) );
                                    Rotation = Rotation * TempQuat;
                                    break;
                                }
                                case AbnormalsModifier::AC_Quaternion:
                                {
                                    TempVec = Vector3( ( Pixel.RedChannel * 255.0 ) - 127.5, ( Pixel.BlueChannel * 255.0 ) - 127.5, ( Pixel.GreenChannel * 255.0 ) - 127.5);
                                    TempQuat = Quaternion( Real(2.0 / 255.0 * MathTools::GetPi() * Pixel.AlphaChannel * this->Sensitivity), TempVec );
                                    Rotation = Rotation * TempQuat;
                                    break;
                                }
                            }
                        }

                        PixelQuat = Rotation * PixelQuat * Rotation.GetInverse();
                        PixelQuat.Normalize();

                        // Red
                        Real RedVal = ( PixelQuat.X * 0.5 ) + 0.5;
                        if( this->Mirror & AbnormalsModifier::AMF_X ) {
                            Buffer.SetRedReal(X,Y,1.0 - RedVal);
                        }else{
                            Buffer.SetRedReal(X,Y,RedVal);
                        }
                        // Green
                        Real GreenVal = ( PixelQuat.Z * 0.5 ) + 0.5;
                        if( this->Mirror & AbnormalsModifier::AMF_Z ) {
                            Buffer.SetGreenReal(X,Y,1.0 - GreenVal);
                        }else{
                            Buffer.SetGreenReal(X,Y,GreenVal);
                        }
                        // Blue
                        Real BlueVal = ( PixelQuat.Y * 0.5 ) + 0.5;
                        if( this->Mirror & AbnormalsModifier::AMF_Y ) {
                            Buffer.SetBlueReal(X,Y,1.0 - BlueVal);
                        }else{
                            Buffer.SetBlueReal(X,Y,BlueVal);
                        }
                    }
                }
            }

            String AbnormalsModifier::GetName() const
                { return "AbnormalsModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            AbnormalsModifier& AbnormalsModifier::SetAbnormalsTexture(TextureBuffer* Abnormals)
                { this->AbnormalsTexture = Abnormals;  return *this; }

            AbnormalsModifier& AbnormalsModifier::SetAngle(const Real ModifierAngle)
                { this->Angle = ModifierAngle;  return *this; }

            AbnormalsModifier& AbnormalsModifier::SetAxis(const Vector3& ModifierAxis)
                { this->Axis = ModifierAxis;  return *this; }

            AbnormalsModifier& AbnormalsModifier::SetAxis(const Real X, const Real Y, const Real Z)
                { this->Axis.SetValues(X,Y,Z);  return *this; }

            AbnormalsModifier& AbnormalsModifier::SetSensitivity(const UInt8 ModifierSensitivity)
                { this->Sensitivity = ModifierSensitivity;  return *this; }

            AbnormalsModifier& AbnormalsModifier::SetCompensation(const AbnormalsCompensation AbnormComp)
                { this->Compensation = AbnormComp;  return *this; }

            AbnormalsModifier& AbnormalsModifier::SetMirror(const Whole AbnormMirror)
                { this->Mirror = AbnormMirror;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
