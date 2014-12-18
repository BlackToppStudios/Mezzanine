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
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef noisemoduleterrace_cpp
#define noisemoduleterrace_cpp

#include "Noise/Module/terrace.h"

#include "Noise/interpolate.h"

#include "exception.h"
#include "MathTools/arithmetic.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module 
        {
            Terrace::Terrace() :
                ModuleBase(this->GetSourceModuleCount()),
                m_controlPointCount(0),
                m_invertTerraces(false),
                m_pControlPoints(NULL)
                {  }

            Terrace::~Terrace ()
            {
                delete[] this->m_pControlPoints;
            }

            Integer Terrace::FindInsertionPos(const PreciseReal Value)
            {
                Integer insertionPos;
                for( insertionPos = 0 ; insertionPos < this->m_controlPointCount ; insertionPos++ ) 
                {
                    if( Value < this->m_pControlPoints[insertionPos] ) {
                        // We found the array index in which to insert the new control point.
                        // Exit now.
                        break;
                    }else if( Value == this->m_pControlPoints[insertionPos] ) {
                        // Each control point is required to contain a unique value, so throw
                        // an exception.
                        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The input value of a terrace provided already exists.");
                    }
                }
                return insertionPos;
            }

            void Terrace::InsertAtPos(const Integer InsertionPos, const PreciseReal Value)
            {
                // Make room for the new control point at the specified position within
                // the control point array.  The position is determined by the value of
                // the control point; the control points must be sorted by value within
                // that array.
                PreciseReal* newControlPoints = new PreciseReal[this->m_controlPointCount + 1];
                for( Integer i = 0 ; i < this->m_controlPointCount ; i++ ) 
                {
                    if( i < InsertionPos ) {
                        newControlPoints[i] = this->m_pControlPoints[i];
                    }else{
                        newControlPoints[i + 1] = this->m_pControlPoints[i];
                    }
                }
                delete[] this->m_pControlPoints;
                this->m_pControlPoints = newControlPoints;
                ++this->m_controlPointCount;

                // Now that we've made room for the new control point within the array,
                // add the new control point.
                this->m_pControlPoints[InsertionPos] = Value;
            }

            void Terrace::AddControlPoint(const PreciseReal Value)
            {
                // Find the insertion point for the new control point and insert the new
                // point at that position.  The control point array will remain sorted by
                // value.
                Integer insertionPos = this->FindInsertionPos(Value);
                this->InsertAtPos(insertionPos, Value);
            }

            void Terrace::ClearAllControlPoints()
            {
                delete[] this->m_pControlPoints;
                this->m_pControlPoints = NULL;
                this->m_controlPointCount = 0;
            }
            
            void Terrace::MakeControlPoints(const Integer ControlPointCount)
            {
                if( ControlPointCount < 2 ) {
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Requesting fewer than 2 control points for a Terrace.  The minimum allowed is 2.");
                }

                this->ClearAllControlPoints();

                PreciseReal terraceStep = 2.0 / ((PreciseReal)ControlPointCount - 1.0);
                PreciseReal curValue = -1.0;
                for( Integer i = 0 ; i < (Integer)ControlPointCount ; i++ ) 
                {
                    this->AddControlPoint(curValue);
                    curValue += terraceStep;
                }
            }

            PreciseReal Terrace::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                assert( this->m_pSourceModule[0] != NULL );
                assert( this->m_controlPointCount >= 2 );

                // Get the output value from the source module.
                PreciseReal sourceModuleValue = this->m_pSourceModule[0]->GetValue(X, Y, Z);

                // Find the first element in the control point array that has a value
                // larger than the output value from the source module.
                Integer indexPos;
                for( indexPos = 0 ; indexPos < m_controlPointCount ; indexPos++ ) 
                {
                    if( sourceModuleValue < this->m_pControlPoints[indexPos] ) {
                        break;
                    }
                }

                // Find the two nearest control points so that we can map their values
                // onto a quadratic curve.
                Integer index0 = MathTools::Clamp(indexPos - 1, 0, this->m_controlPointCount - 1);
                Integer index1 = MathTools::Clamp(indexPos    , 0, this->m_controlPointCount - 1);

                // If some control points are missing (which occurs if the output value from
                // the source module is greater than the largest value or less than the
                // smallest value of the control point array), get the value of the nearest
                // control point and exit now.
                if( index0 == index1 ) {
                    return this->m_pControlPoints[index1];
                }

                // Compute the alpha value used for linear interpolation.
                PreciseReal value0 = this->m_pControlPoints[index0];
                PreciseReal value1 = this->m_pControlPoints[index1];
                PreciseReal alpha = ( sourceModuleValue - value0 ) / ( value1 - value0 );
                if( this->m_invertTerraces ) {
                    alpha = 1.0 - alpha;
                    std::swap(value0, value1);
                }

                // Squaring the alpha produces the terrace effect.
                alpha *= alpha;

                // Now perform the linear interpolation given the alpha value.
                return Noise::LinearInterpolate(value0, value1, alpha);
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
