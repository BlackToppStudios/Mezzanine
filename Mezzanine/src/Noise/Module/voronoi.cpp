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

#ifndef noisemodulevoronoi_cpp
#define noisemodulevoronoi_cpp

#include "Noise/Module/voronoi.h"
#include "Noise/noisegen.h"

#include "MathTools/constants.h"
#include "MathTools/arithmetic.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module 
        {
            Voronoi::Voronoi() :
                ModuleBase(this->GetSourceModuleCount()),
                m_displacement(Module::DEFAULT_VORONOI_DISPLACEMENT),
                m_enableDistance(false),
                m_frequency(Module::DEFAULT_VORONOI_FREQUENCY),
                m_seed(Module::DEFAULT_VORONOI_SEED)
                {  }

            PreciseReal Voronoi::GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const
            {
                // This method could be more efficient by caching the Seed values.  Fix later.

                PreciseReal CenterX = X * this->m_frequency;
                PreciseReal CenterY = Y * this->m_frequency;
                PreciseReal CenterZ = Z * this->m_frequency;

                Integer xInt = (CenterX > 0.0? (Integer)CenterX: (Integer)CenterX - 1);
                Integer yInt = (CenterY > 0.0? (Integer)CenterY: (Integer)CenterY - 1);
                Integer zInt = (CenterZ > 0.0? (Integer)CenterZ: (Integer)CenterZ - 1);

                PreciseReal minDist = 2147483647.0;
                PreciseReal xCandidate = 0;
                PreciseReal yCandidate = 0;
                PreciseReal zCandidate = 0;

                // Inside each unit cube, there is a Seed point at a random position.  Go
                // through each of the nearby cubes until we find a cube with a Seed point
                // that is closest to the specified position.
                for (Integer zCur = zInt - 2; zCur <= zInt + 2; zCur++) 
                {
                    for (Integer yCur = yInt - 2; yCur <= yInt + 2; yCur++) 
                    {
                        for (Integer xCur = xInt - 2; xCur <= xInt + 2; xCur++) 
                        {
                            // Calculate the position and distance to the Seed point inside of
                            // this unit cube.
                            PreciseReal xPos = xCur + Noise::ValueNoise3D(xCur, yCur, zCur, m_seed    );
                            PreciseReal yPos = yCur + Noise::ValueNoise3D(xCur, yCur, zCur, m_seed + 1);
                            PreciseReal zPos = zCur + Noise::ValueNoise3D(xCur, yCur, zCur, m_seed + 2);
                            PreciseReal xDist = xPos - CenterX;
                            PreciseReal yDist = yPos - CenterY;
                            PreciseReal zDist = zPos - CenterZ;
                            PreciseReal dist = xDist * xDist + yDist * yDist + zDist * zDist;

                            if( dist < minDist ) {
                                // This Seed point is closer to any others found so far, so record
                                // this Seed point.
                                minDist = dist;
                                xCandidate = xPos;
                                yCandidate = yPos;
                                zCandidate = zPos;
                            }
                        }
                    }
                }

                PreciseReal value;
                if( this->m_enableDistance ) {
                    // Determine the distance to the nearest Seed point.
                    PreciseReal xDist = xCandidate - CenterX;
                    PreciseReal yDist = yCandidate - CenterY;
                    PreciseReal zDist = zCandidate - CenterZ;
                    value = ( MathTools::Sqrt( xDist * xDist + yDist * yDist + zDist * zDist ) ) * MathTools::GetSqrtThree() - 1.0;
                }else{
                    value = 0.0;
                }

                // Return the calculated distance with the displacement value applied.
                return value + ( this->m_displacement * (PreciseReal)Noise::ValueNoise3D(
                        (Integer)( MathTools::Floor(xCandidate) ),
                        (Integer)( MathTools::Floor(yCandidate) ),
                        (Integer)( MathTools::Floor(zCandidate) ) ) );
            }
        }//Module
    }//Noise
}//Mezzanine

#endif
