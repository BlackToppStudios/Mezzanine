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

#ifndef noisemodulevoronoi_h
#define noisemodulevoronoi_h

#include "Noise/Module/modulebase.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module 
        {
    /// @addtogroup libnoise
    /// @{

    /// @addtogroup modules
    /// @{

    /// @addtogroup generatormodules
    /// @{

    /// Default displacement to apply to each cell for the
    /// Noise::Module::Voronoi noise module.
    const PreciseReal DEFAULT_VORONOI_DISPLACEMENT = 1.0;

    /// Default frequency of the Seed points for the Noise::Module::Voronoi
    /// noise module.
    const PreciseReal DEFAULT_VORONOI_FREQUENCY = 1.0;

    /// Default Seed of the noise function for the Noise::Module::Voronoi
    /// noise module.
    const Integer DEFAULT_VORONOI_SEED = 0;

    /// Noise module that outputs Voronoi cells.
    ///
    /// In mathematics, a <i>Voronoi cell</i> is a region containing all the
    /// points that are closer to a specific <i>seed point</i> than to any
    /// other Seed point.  These cells mesh with one another, producing
    /// polygon-like formations.
    ///
    /// By default, this noise module randomly places a Seed point within
    /// each unit cube.  By modifying the <i>frequency</i> of the Seed points,
    /// an application can change the distance between Seed points.  The
    /// higher the frequency, the closer together this noise module places
    /// the Seed points, which reduces the size of the cells.  To specify the
    /// frequency of the cells, call the SetFrequency() method.
    ///
    /// This noise module assigns each Voronoi cell with a random constant
    /// value from a coherent-noise function.  The <i>displacement value</i>
    /// controls the range of random values to assign to each cell.  The
    /// range of random values is +/- the displacement value.  Call the
    /// SetDisplacement() method to specify the displacement value.
    ///
    /// To modify the random positions of the Seed points, call the SetSeed()
    /// method.
    ///
    /// This noise module can optionally add the distance from the nearest
    /// Seed to the output value.  To enable this feature, call the
    /// EnableDistance() method.  This causes the points in the Voronoi cells
    /// to increase in value the further away that point is from the nearest
    /// Seed point.
    ///
    /// Voronoi cells are often used to generate cracked-mud terrain
    /// formations or crystal-like textures
    ///
    /// This noise module requires no source modules.
    class Voronoi : public ModuleBase
    {
    public:
        /// @brief Constructor.
        ///
        /// The default displacement value is set to
        /// Noise::Module::DEFAULT_VORONOI_DISPLACEMENT.
        ///
        /// The default frequency is set to
        /// Noise::Module::DEFAULT_VORONOI_FREQUENCY.
        ///
        /// The default Seed value is set to
        /// Noise::Module::DEFAULT_VORONOI_SEED.
        Voronoi();

        /// Sets the displacement value of the Voronoi cells.
        ///
        /// @param Displacement The displacement value of the Voronoi cells.
        ///
        /// This noise module assigns each Voronoi cell with a random constant
        /// value from a coherent-noise function.  The <i>displacement
        /// value</i> controls the range of random values to assign to each
        /// cell.  The range of random values is +/- the displacement value.
        void SetDisplacement(const PreciseReal Displacement)
        {
            this->m_displacement = Displacement;
        }
        /// Returns the displacement value of the Voronoi cells.
        ///
        /// @return Returns the displacement value of the Voronoi cells.
        ///
        /// This noise module assigns each Voronoi cell with a random constant
        /// value from a coherent-noise function.  The <i>displacement
        /// value</i> controls the range of random values to assign to each
        /// cell.  The range of random values is +/- the displacement value.
        PreciseReal GetDisplacement() const
        {
            return this->m_displacement;
        }

        /// Sets the frequency of the Seed points.
        ///
        /// @param Frequency The frequency of the Seed points.
        ///
        /// The frequency determines the size of the Voronoi cells and the
        /// distance between these cells.
        void SetFrequency(const PreciseReal Frequency)
        {
            this->m_frequency = Frequency;
        }
        /// Returns the frequency of the Seed points.
        ///
        /// @return Returns the frequency of the Seed points.
        ///
        /// The frequency determines the size of the Voronoi cells and the
        /// distance between these cells.
        PreciseReal GetFrequency() const
        {
            return this->m_frequency;
        }

        /// Enables or disables applying the distance from the nearest Seed
        /// point to the output value.
        ///
        /// @param Enable Specifies whether to apply the distance to the
        /// output value or not.
        ///
        /// Applying the distance from the nearest Seed point to the output
        /// value causes the points in the Voronoi cells to increase in value
        /// the further away that point is from the nearest Seed point.
        /// Setting this value to @a true (and setting the displacement to a
        /// near-zero value) causes this noise module to generate cracked mud
        /// formations.
        void EnableDistance(const Boole Enable = true)
        {
            this->m_enableDistance = Enable;
        }
        /// Determines if the distance from the nearest Seed point is applied
        /// to the output value.
        ///
        /// @returns
        /// - @a true if the distance is applied to the output value.
        /// - @a false if not.
        ///
        /// Applying the distance from the nearest Seed point to the output
        /// value causes the points in the Voronoi cells to increase in value
        /// the further away that point is from the nearest Seed point.
        Boole IsDistanceEnabled() const
        {
            return this->m_enableDistance;
        }

        /// Sets the Seed value used by the Voronoi cells
        ///
        /// @param Seed The Seed value.
        ///
        /// The positions of the Seed values are calculated by a
        /// coherent-noise function.  By modifying the Seed value, the output
        /// of that function changes.
        void SetSeed(const Integer Seed)
        {
            this->m_seed = Seed;
        }
        /// Returns the Seed value used by the Voronoi cells
        ///
        /// @return Returns the Seed value.
        ///
        /// The positions of the Seed values are calculated by a
        /// coherent-noise function.  By modifying the Seed value, the output
        /// of that function changes.
        Integer GetSeed() const
        {
            return this->m_seed;
        }

        /// @copydoc ModuleBase::GetSourceModuleCount() const
        virtual Integer GetSourceModuleCount() const
        {
            return 0;
        }

        /// @copydoc ModuleBase::GetValue(const PreciseReal, const PreciseReal, const PreciseReal) const
        virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const;
    protected:
        /// @internal
        /// @brief Scale of the random displacement to apply to each Voronoi cell.
        PreciseReal m_displacement;
        /// @internal
        /// @brief Determines if the distance from the nearest Seed point is applied to the output value.
        Boole m_enableDistance;
        /// @internal
        /// @brief Frequency of the Seed points.
        PreciseReal m_frequency;
        /// @internal
        /// @brief Seed value used by the coherent-noise function to determine the positions of the Seed points.
        Integer m_seed;
    };//Voronoi
    /// @}

    /// @}

    /// @}
        }//Module
    }//Noise
}//Mezzanine

#endif
