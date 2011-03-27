//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _actorterrain_h
#define _actorterrain_h

#include "actorbase.h"
#include "crossplatformexport.h"

class btRigidBody;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorTerrain
    /// @headerfile actorterrain.h
    /// @brief This is actor class for terrain.
    /// @details This class is intended to be used for terrain.  This class uses sub-classes with fewer features,
    /// but still more then enough features for static terrain.  This helps reduce the memory footprint of the
    /// class as well as processing speed.
    ///////////////////////////////////////
    class PHYS_LIB ActorTerrain : public ActorBase {
        protected:
            /// @internal
            /// @brief The physics data.
            btRigidBody* RigidBody;

            /// @brief Uses value already passed into this to create the physics shapes
            void CreateCollisionTerrain();
        public:
            /// @brief Class constructor.
            /// @details The class constructor.
            /// @param InitPosition The location for this terrain.
            /// @param name The name of the actor.
            /// @param file The 3d mesh file that contains the 3d model the actor will use.
            /// @param group The resource group where the 3d mesh and other related files can be found.
            ActorTerrain(Vector3 InitPosition, String name, String file, String group);

            /// @brief Class destructor.
            /// @details The class destructor.
            ~ActorTerrain();

            /// @brief Creates a collision shape from mesh file.
            /// @details This function will read the location of every verticy in the mesh file and use that to
            /// construct a triangle mesh shape and attach it to this objects collision shape.  This shoiuld
            /// be used with only with Static objects.
            /// @param UseAllSubmeshes If true, this will use the geometry of all submeshes of the model to make the shape.  Otherwise it'll only use the first submesh.
            void CreateShapeFromMeshStatic(bool UseAllSubmeshes = false);

            /// @brief Compatability function.
            /// @details This function does nothing, as this class isn't dynamic.  This is simply here to
            /// allow the engine to compile.
            void CreateShapeFromMeshDynamic(short unsigned int Accuracy, bool UseAllSubmeshes = false);

            /// @brief Retrieves the name of the object.
            /// @details This function will retrieve the name of the object,
            std::string GetName() const;

            virtual void AddObjectToWorld (World *TargetWorld);
            virtual void RemoveObjectFromWorld(World* TargetWorld);

            /// @internal
            /// @brief Get the Physics data raw from the physic subsystem
            /// @return Currently this returns a pointer to a btSoftBody.
            virtual btRigidBody* GetBulletObject();
    };
}

#endif
