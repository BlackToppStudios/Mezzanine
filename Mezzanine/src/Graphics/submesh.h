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

#ifndef _graphicssubmesh_h
#define _graphicssubmesh_h

#include "datatypes.h"

namespace Ogre
{
    class SubMesh;
}//Ogre

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SubMesh
        /// @headerfile submesh.h
        /// @brief This class represents a sub-section of an overall mesh.
        /// @details Every mesh has at least one SubMesh.  Most meshes have only 1 or 2.  SubMeshes are often
        /// implemented in cases where one portion of the mesh needs to be animated or given a different
        /// material then another portion.
        ///////////////////////////////////////
        class MEZZ_LIB SubMesh
        {
            protected:
            public:
                /// @brief Class Constructor.
                SubMesh();
                /// @brief Class Destructor.
                ~SubMesh();
        };//SubMesh
    }//Graphics
}//Mezzanine

#endif
