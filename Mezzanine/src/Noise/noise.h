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

#ifndef noise_h
#define noise_h

namespace Mezzanine
{
    /// @namespace Mezzanine::Noise
    /// @brief This namespace is for all of the utilities that are used to generate coherent noise for procedural subsystems/applications.
    /// @details Our Noise subsystem is a paper-thin conversion of LibNoise, and as such follows the same overall API as LibNoise.  It has
    /// only been lightly modified such that it follows the same code styling as the Mezzanine, or use pre-existing Mezzanine utilities
    /// instead of it's own where applicable.
    namespace Noise
    {

    }//Noise
}//Mezzanine

#include "Noise/noiseenumerations.h"
#include "Noise/noiseutilities.h"

#include "Noise/interpolate.h"
#include "Noise/noisegen.h"
#include "Noise/vectortable.h"

#include "Noise/Module/abs.h"
#include "Noise/Module/add.h"
#include "Noise/Module/billow.h"
#include "Noise/Module/blend.h"
#include "Noise/Module/cache.h"
#include "Noise/Module/checkerboard.h"
#include "Noise/Module/clamp.h"
#include "Noise/Module/const.h"
#include "Noise/Module/curve.h"
#include "Noise/Module/cylinders.h"
#include "Noise/Module/displace.h"
#include "Noise/Module/exponent.h"
#include "Noise/Module/invert.h"
#include "Noise/Module/max.h"
#include "Noise/Module/min.h"
#include "Noise/Module/modulebase.h"
#include "Noise/Module/multiply.h"
#include "Noise/Module/perlin.h"
#include "Noise/Module/power.h"
#include "Noise/Module/ridgedmulti.h"
#include "Noise/Module/rotatepoint.h"
#include "Noise/Module/scalebias.h"
#include "Noise/Module/scalepoint.h"
#include "Noise/Module/select.h"
#include "Noise/Module/spheres.h"
#include "Noise/Module/terrace.h"
#include "Noise/Module/translatepoint.h"
#include "Noise/Module/turbulence.h"
#include "Noise/Module/voronoi.h"

#include "Noise/Model/cylinder.h"
#include "Noise/Model/line.h"
#include "Noise/Model/plane.h"
#include "Noise/Model/sphere.h"

/// @page libnoise Mezzanine::Noise/libnoise Manual
///
/// @section noiseintro Introduction
///
/// libnoise is a portable C++ library that is used to generate <i>coherent
/// noise</i>, a type of smoothly-changing noise. libnoise can generate Perlin
/// noise, ridged multifractal noise, and other types of coherent noise.
///
/// Coherent noise is often used by graphics programmers to generate
/// natural-looking textures, planetary terrain, and other things. It can
/// also be used to move critters in a realistic way.
///
/// libnoise is known to compile using the following compilers on the
/// following platforms:
/// - Microsoft Visual C++ 5.0 under Microsoft Windows 2000 Service Pack 4
/// - gcc 3.3.4 under Gentoo Linux 10.0 (x86)
///
/// It is not known if libnoise will compile on 64-bit platforms, although
/// there is a good change that it will.
///
/// @section noise Noise Modules
///
/// In libnoise, coherent-noise generators are encapsulated in classes called
/// <i>noise modules</i>. There are many different types of noise modules.
/// Some noise modules can combine or modify the outputs of other noise
/// modules in various ways; you can join these modules together to generate
/// very complex coherent noise.
///
/// A noise module receives a 3-dimensional input value from the application,
/// computes the noise value given that input value, and returns the resulting
/// value back to the application.
///
/// If the application passes the same input value to a noise module, the
/// noise module returns the same output value.
///
/// All noise modules are derived from the Noise::Module::Module abstract
/// base class.
///
/// @section noisecontact Contact
///
/// Contact jas for questions about libnoise.  The spam-resistant email
/// address is jlbezigvins@gmzigail.com (For great email, take off every
/// <a href=http://www.planettribes.com/allyourbase/story.shtml>zig</a>.)

#endif
