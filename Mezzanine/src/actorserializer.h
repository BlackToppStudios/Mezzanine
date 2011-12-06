//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _actorserializer_h
#define _actorserializer_h

#include "actorrigid.h"
#include "actormanager.h"
#include "datatypes.h"
#include "serialization.h"
#include "xmldoc.h"

/// @file actorserializer.h
/// @brief This is where the declarations of any Serializers or DeSerializers for actors will go.


namespace Mezzanine
{

    /// @brief This creates Rigid body actors and inserts them into a given ActorManager.
    class MEZZ_LIB ActorRigidDeSerializer : public DeSerializer <ActorRigid>
    {
        #ifdef MEZZXML

        protected:
            /// This should point to a specific Actor Manager
            ActorManager* Target;

        public:

            /// @brief Constructor
            /// @param DeSerializeTarget A pointer to the actor manager that will be populated with deserialized actors.
            explicit ActorRigidDeSerializer(ActorManager* DeSerializeTarget)
                    : Target(DeSerializeTarget)
            {}

            /// @brief Accepts one xml::Node with an actor in it and create a new ActorRigid
            /// @details In addition to returning the created actor it's pointer is also added to the Target ActorManager
            /// @param OneNode The node that should have an ActorRigid in it
            /// @return A pointer to the freshly created actor
            virtual ActorRigid* ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Should indicate the Name of the node that ProtoDeSerializeAll all accepts (when deserializing raw xml text).
            /// @warning If The ActorManager Passed in at construction is null, this will do very little other than eat cpu and memory bandwidth.
            /// @return A String containing "RigidActors".
            virtual String ContainerName() const;

        #endif // \MEZZXML
    };
}






#endif
