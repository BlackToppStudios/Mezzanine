// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef noisemodulecurve_cpp
#define noisemodulecurve_cpp

#include "Noise/Module/curve.h"
#include "Noise/interpolate.h"

#include "MathTools/arithmetic.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            Curve::Curve() :
                ModuleBase(this->GetSourceModuleCount()),
                m_pControlPoints (NULL)
                { this->m_controlPointCount = 0; }

            Curve::~Curve ()
            {
                delete[] this->m_pControlPoints;
            }

            Integer Curve::FindInsertionPos(const PreciseReal InputValue)
            {
                Integer insertionPos;
                for( insertionPos = 0 ; insertionPos < this->m_controlPointCount ; insertionPos++ )
                {
                    if( InputValue < this->m_pControlPoints[insertionPos].InputValue ) {
                        // We found the array index in which to insert the new control point.
                        // Exit now.
                        break;
                    }else if( InputValue == this->m_pControlPoints[insertionPos].InputValue ) {
                        // Each control point is required to contain a unique input value, so
                        // throw an exception.
                        MEZZ_EXCEPTION(Mezzanine::ExceptionBase::PARAMETERS_EXCEPTION,"The input value of a curve provided already exists.");
                    }
                }
                return insertionPos;
            }

            void Curve::InsertAtPos(const Integer insertionPos, const PreciseReal InputValue, const PreciseReal OutputValue)
            {
                // Make room for the new control point at the specified position within the
                // control point array.  The position is determined by the input value of
                // the control point; the control points must be sorted by input value
                // within that array.
                ControlPoint* newControlPoints = new ControlPoint[this->m_controlPointCount + 1];
                for( Integer i = 0 ; i < this->m_controlPointCount ; i++ )
                {
                    if( i < insertionPos ) {
                        newControlPoints[i] = this->m_pControlPoints[i];
                    }else{
                        newControlPoints[i + 1] = this->m_pControlPoints[i];
                    }
                }
                delete[] this->m_pControlPoints;
                this->m_pControlPoints = newControlPoints;
                ++this->m_controlPointCount;

                // Now that we've made room for the new control point within the array, add
                // the new control point.
                this->m_pControlPoints[insertionPos].InputValue  = InputValue;
                this->m_pControlPoints[insertionPos].OutputValue = OutputValue;
            }

            void Curve::AddControlPoint(const PreciseReal InputValue, const PreciseReal OutputValue)
            {
                // Find the insertion point for the new control point and insert the new
                // point at that position.  The control point array will remain sorted by
                // input value.
                Integer insertionPos = this->FindInsertionPos(InputValue);
                this->InsertAtPos(insertionPos, InputValue, OutputValue);
            }

            void Curve::ClearAllControlPoints()
            {
                delete[] this->m_pControlPoints;
                this->m_pControlPoints = NULL;
                this->m_controlPointCount = 0;
            }

            PreciseReal Curve::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                assert( this->m_pSourceModule[0] != NULL );
                assert( this->m_controlPointCount >= 4 );

                // Get the output value from the source module.
                PreciseReal sourceModuleValue = this->m_pSourceModule[0]->GetValue(X, Y, Z);

                // Find the first element in the control point array that has an input value
                // larger than the output value from the source module.
                Integer indexPos;
                for( indexPos = 0 ; indexPos < this->m_controlPointCount ; indexPos++ )
                {
                    if( sourceModuleValue < this->m_pControlPoints[indexPos].InputValue ) {
                        break;
                    }
                }

                // Find the four nearest control points so that we can perform cubic
                // interpolation.
                Integer index0 = MathTools::Clamp(indexPos - 2, 0, this->m_controlPointCount - 1);
                Integer index1 = MathTools::Clamp(indexPos - 1, 0, this->m_controlPointCount - 1);
                Integer index2 = MathTools::Clamp(indexPos    , 0, this->m_controlPointCount - 1);
                Integer index3 = MathTools::Clamp(indexPos + 1, 0, this->m_controlPointCount - 1);

                // If some control points are missing (which occurs if the value from the
                // source module is greater than the largest input value or less than the
                // smallest input value of the control point array), get the corresponding
                // output value of the nearest control point and exit now.
                if( index1 == index2 ) {
                    return this->m_pControlPoints[index1].OutputValue;
                }

                // Compute the alpha value used for cubic interpolation.
                PreciseReal input0 = this->m_pControlPoints[index1].InputValue;
                PreciseReal input1 = this->m_pControlPoints[index2].InputValue;
                PreciseReal alpha = (sourceModuleValue - input0) / (input1 - input0);

                // Now perform the cubic interpolation given the alpha value.
                return Noise::CubicInterpolate(
                    this->m_pControlPoints[index0].OutputValue,
                    this->m_pControlPoints[index1].OutputValue,
                    this->m_pControlPoints[index2].OutputValue,
                    this->m_pControlPoints[index3].OutputValue,
                    alpha);
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
