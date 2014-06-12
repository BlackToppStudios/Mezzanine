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
#ifndef _graphicsproceduralcurvetrack_cpp
#define _graphicsproceduralcurvetrack_cpp

#include "Graphics/Procedural/curvetrack.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            CurveTrack::CurveTrack(const CurveTrack::AddressingMode Mode, const Boole Insert) :
                TrackAddressing(Mode),
                InsertPoint(Insert)
                {  }

            CurveTrack::~CurveTrack()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            CurveTrack& CurveTrack::AddKeyFrame(const Real Pos, const Real Value)
            {
                this->KeyFrames[Pos] = Value;
                return *this;
            }

            CurveTrack::AddressingMode CurveTrack::GetAddressingMode() const
                { return this->TrackAddressing; }

            Boole CurveTrack::IsInsertPoint() const
                { return this->InsertPoint; }

            Real CurveTrack::GetValue(const Real Pos) const
            {
                ConstKeyFrameIterator ItAfter = this->_GetKeyValueAfter(Pos);
                ConstKeyFrameIterator ItBefore = this->_GetKeyValueBefore(Pos);

                if( ItAfter == ItBefore )
                    return ItBefore->second;
                if( ItAfter == this->KeyFrames.begin() )
                    return ItAfter->second;

                Real x1 = ItBefore->first;
                Real y1 = ItBefore->second;
                Real x2 = ItAfter->first;
                Real y2 = ItAfter->second;
                return ( Pos - x1 ) / ( x2 - x1 ) * ( y2 - y1 ) + y1;
            }

            Real CurveTrack::GetValue(const Real AbsPos, const Real RelPos, const Whole Index) const
            {
                if( this->TrackAddressing == CurveTrack::AM_Absolute_Lineic )
                    return this->GetValue( AbsPos );
                if( this->TrackAddressing == CurveTrack::AM_Relative_Lineic )
                    return this->GetValue( RelPos );
                return this->GetValue( (Real)Index );
            }

            Real CurveTrack::GetFirstValue() const
                { return this->KeyFrames.begin()->second; }

            Real CurveTrack::GetLastValue() const
                { return (--this->KeyFrames.end())->second; }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            CurveTrack::ConstKeyFrameIterator CurveTrack::_GetKeyValueBefore(const Real Pos) const
            {
                ConstKeyFrameIterator FrameIt = this->KeyFrames.upper_bound(Pos);
                if( FrameIt == this->KeyFrames.begin() ) {
                    return FrameIt;
                }else{
                    return --FrameIt;
                }
            }

            CurveTrack::ConstKeyFrameIterator CurveTrack::_GetKeyValueBefore(const Real AbsPos, const Real RelPos, const Whole Index) const
            {
                if( this->TrackAddressing == CurveTrack::AM_Absolute_Lineic )
                    return this->_GetKeyValueBefore( AbsPos );
                if( this->TrackAddressing == CurveTrack::AM_Relative_Lineic )
                    return this->_GetKeyValueBefore( RelPos );
                return this->_GetKeyValueBefore( (Real)Index );
            }

            CurveTrack::ConstKeyFrameIterator CurveTrack::_GetKeyValueAfter(const Real Pos) const
            {
                ConstKeyFrameIterator FrameIt = this->KeyFrames.upper_bound(Pos);
                if( FrameIt == this->KeyFrames.end() ) {
                    return --FrameIt;
                }else{
                    return FrameIt;
                }
            }

            CurveTrack::ConstKeyFrameIterator CurveTrack::_GetKeyValueAfter(const Real AbsPos, const Real RelPos, const Whole Index) const
            {
                if( this->TrackAddressing == CurveTrack::AM_Absolute_Lineic )
                    return this->_GetKeyValueAfter( AbsPos );
                if( this->TrackAddressing == CurveTrack::AM_Relative_Lineic )
                    return this->_GetKeyValueAfter( RelPos );
                return this->_GetKeyValueAfter( (Real)Index );
            }
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
