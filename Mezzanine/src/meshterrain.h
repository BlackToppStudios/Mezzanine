// © Copyright 2010 - 2015 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#ifndef _meshterrain_h
#define _meshterrain_h

#include "terrainbase.h"

///////////////////////////////////
// Forward Declares

namespace Mezzanine
{
    class MEZZ_LIB MeshTerrain : public TerrainBase
    {
        protected:
        public:
            /// @brief Class constructor.
            /// @details The class constructor.
            /// @param InitPosition The location for this terrain.
            /// @param name The name of the terrain.
            /// @param file The 3d mesh file that contains the 3d model the terrain will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            MeshTerrain(Vector3 InitPosition, String name, String file, String group);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~MeshTerrain();
    };
}


#endif // _meshterrain_h
