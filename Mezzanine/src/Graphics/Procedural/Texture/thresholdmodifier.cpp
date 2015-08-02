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
#ifndef _graphicsproceduralthresholdmodifier_cpp
#define _graphicsproceduralthresholdmodifier_cpp

#include "Graphics/Procedural/Texture/thresholdmodifier.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ThresholdModifier::ThresholdModifier() :
                ThresholdColour(0.0,0.0,0.0,1.0),
                LowerLimit(0.0),
                UpperLimit(1.0),
                Threshold(ThresholdModifier::TM_Below),
                Replace(ThresholdModifier::RM_PixelIntensity)
                {  }

            ThresholdModifier::~ThresholdModifier()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void ThresholdModifier::Modify(TextureBuffer& Buffer)
            {
                Whole TargetWidth = Buffer.GetWidth();
                Whole TargetHeight = Buffer.GetHeight();

                for( Whole Y = 0 ; Y < TargetHeight ; ++Y )
                {
                    for( Whole X = 0 ; X < TargetWidth ; ++X )
                    {
                        const ColourValue Pixel = Buffer.GetPixel(X,Y);

                        if( this->Replace == ThresholdModifier::RM_PixelIntensity ) {
                            Real Intensity = ( Pixel.RedChannel + Pixel.GreenChannel + Pixel.BlueChannel ) / 3;

                            if( this->Threshold & ThresholdModifier::TM_Below && Intensity < this->LowerLimit ) {
                                Buffer.SetPixel(X,Y,this->ThresholdColour);
                            }else if( this->Threshold & ThresholdModifier::TM_Above && Intensity > this->UpperLimit ) {
                                Buffer.SetPixel(X,Y,this->ThresholdColour);
                            }
                        }else if( this->Replace == ThresholdModifier::RM_ColourChannels ) {
                            if( this->Threshold & ThresholdModifier::TM_Below && Pixel.RedChannel < this->LowerLimit ) {
                                Buffer.SetRedReal(X,Y,this->ThresholdColour.RedChannel);
                            }else if( this->Threshold & ThresholdModifier::TM_Above && Pixel.RedChannel > this->UpperLimit ) {
                                Buffer.SetRedReal(X,Y,this->ThresholdColour.RedChannel);
                            }

                            if( this->Threshold & ThresholdModifier::TM_Below && Pixel.GreenChannel < this->LowerLimit ) {
                                Buffer.SetGreenReal(X,Y,this->ThresholdColour.GreenChannel);
                            }else if( this->Threshold & ThresholdModifier::TM_Above && Pixel.GreenChannel > this->UpperLimit ) {
                                Buffer.SetGreenReal(X,Y,this->ThresholdColour.GreenChannel);
                            }

                            if( this->Threshold & ThresholdModifier::TM_Below && Pixel.BlueChannel < this->LowerLimit ) {
                                Buffer.SetBlueReal(X,Y,this->ThresholdColour.BlueChannel);
                            }else if( this->Threshold & ThresholdModifier::TM_Above && Pixel.BlueChannel > this->UpperLimit ) {
                                Buffer.SetBlueReal(X,Y,this->ThresholdColour.BlueChannel);
                            }
                        }
                    }
                }
            }

            String ThresholdModifier::GetName() const
                { return "ThresholdModifier"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            ThresholdModifier& ThresholdModifier::SetColour(const ColourValue& Colour)
                { this->ThresholdColour = Colour;  return *this; }

            ThresholdModifier& ThresholdModifier::SetColour(const Real Red, const Real Green, const Real Blue, const Real Alpha)
                { this->ThresholdColour.SetValues(Red,Green,Blue,Alpha);  return *this; }

            ThresholdModifier& ThresholdModifier::SetLowerLimit(const Real Lower)
                { this->LowerLimit = Lower;  return *this; }

            ThresholdModifier& ThresholdModifier::SetUpperLimit(const Real Upper)
                { this->UpperLimit = Upper;  return *this; }

            ThresholdModifier& ThresholdModifier::SetLimitRange(const Real Lower, const Real Upper)
                { this->LowerLimit = Lower;  this->UpperLimit = Upper;  return *this; }

            ThresholdModifier& ThresholdModifier::SetThresholdMode(const ThresholdModifier::ThresholdMode Mode)
                { this->Threshold = Mode;  return *this; }

            ThresholdModifier& ThresholdModifier::SetReplaceMode(const ThresholdModifier::ReplaceMode Mode)
                { this->Replace = Mode;  return *this; }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
