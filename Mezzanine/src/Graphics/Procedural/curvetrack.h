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
#ifndef _graphicsproceduralcurvetrack_h
#define _graphicsproceduralcurvetrack_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            class MEZZ_LIB CurveTrack
            {
            public:
                /// @brief Defines addressing mode for the track.
                enum AddressingMode
                {
                    AM_Absolute_Lineic  = 0,   ///< Use the distance from the start of the principal curve.
                    AM_Relative_Lineic  = 1,   ///< Use the relative distance from the start of the principal curve, considering the total length of main curve is 1.
                    AM_Point            = 2    ///< Right on the principal curve's key.
                };

                /// @brief Basic container type for the storage of KeyFrames in this class.
                typedef std::map<Real,Real>                 KeyFrameContainer;
                /// @brief Iterator type for KeyFrames stored in this class.
                typedef KeyFrameContainer::iterator         KeyFrameIterator;
                /// @brief Const Iterator type for KeyFrames stored in this class.
                typedef KeyFrameContainer::const_iterator   ConstKeyFrameIterator;
            protected:
                /// @brief The adressing mode of the track.
                AddressingMode TrackAddressing;
                /// @brief Determines whether we should add new points to principal curve if a key is defined here but not on principal curve.
                Boole InsertPoint;
                /// @brief Container storing all of the points along this track.
                KeyFrameContainer KeyFrames;
            public:
                /// @brief Class constructor.
                /// @param Mode The addressing mode this CurveTrack is to use for position queries.
                /// @param Insert Whether or not new points should be added to the principal curve if a key is defined here but not on principal curve.
                CurveTrack(const AddressingMode Mode = AM_Relative_Lineic, const Boole Insert = true);
                /// @brief Class destructor.
                ~CurveTrack();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Inserts a new Key/Value pair anywhere on the track.
                /// @param Pos The position in the curve to insert the value.
                /// @param Value The value to be inserted at the specified position.
                /// @return Returns a reference to this.
                CurveTrack& AddKeyFrame(const Real Pos, const Real Value);

                /// @brief Gets addressing mode of the curve.
                /// @return Returns an AddressingMode enum value representing how querys for track position are done.
                AddressingMode GetAddressingMode() const;
                /// @brief Gets whether we should add new points to principal curve if a key is defined here but not on principal curve.
                /// @return Returns true if new points will be added to the pricipal curve, false otherwise.
                Boole IsInsertPoint() const;

                /// @brief Gets the value on the current point.
                /// @param Pos The position in the curve to retrieve.
                /// @return Returns the value at the specified point.
                Real GetValue(const Real Pos) const;
                /// @brief Gets the value on a specified point, taking into account the addressing mode.
                /// @param AbsPos The Absolute position of the point in the curve.
                /// @param RelPos The Relative position of the point in the curve.
                /// @param Index The index of the point in the curve.
                /// @return Returns the value at the specified point.
                Real GetValue(const Real AbsPos, const Real RelPos, const Whole Index) const;
                /// @brief Gets the first value in the track.
                /// @return Returns the value of the first keyframe in this track.
                Real GetFirstValue() const;
                /// @brief Gets the last value in the track.
                /// @return Returns the value of the last keyframe in this track.
                Real GetLastValue() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Get the key value pair before a specified point.
                /// @note If current point is below minimum key, issues minimum key/value.
                /// @param Pos The key for the position in the curve to retrieve.
                /// @return Returns a const iterator to the key value pair before the specified point.
                ConstKeyFrameIterator _GetKeyValueBefore(const Real Pos) const;
                /// @internal
                /// @brief Get the key value pair before a specified point, taking into account addressing mode.
                /// @note If current point is below minimum key, issues minimum key/value.
                /// @param AbsPos The Absolute position of the point in the curve.
                /// @param RelPos The Relative position of the point in the curve.
                /// @param Index The index of the point in the curve.
                /// @return Returns a const iterator to the key value pair before the specified point.
                ConstKeyFrameIterator _GetKeyValueBefore(const Real AbsPos, const Real RelPos, const Whole Index) const;
                /// @internal
                /// @brief Get the key value pair after a specified point.
                /// @note If current point is above maximum key, issues maximum key/value.
                /// @param Pos The key for the position in the curve to retrieve.
                /// @return Returns a const iterator to the key value pair after the specified point.
                ConstKeyFrameIterator _GetKeyValueAfter(const Real Pos) const;
                /// @internal
                /// @brief Get the key value pair after a specified point, taking into account addressing mode.
                /// @note If current point is above maximum key, issues maximum key/value.
                /// @param AbsPos The Absolute position of the point in the curve.
                /// @param RelPos The Relative position of the point in the curve.
                /// @param Index The index of the point in the curve.
                /// @return Returns a const iterator to the key value pair after the specified point.
                ConstKeyFrameIterator _GetKeyValueAfter(const Real AbsPos, const Real RelPos, const Whole Index) const;
            };//CurveTrack
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
