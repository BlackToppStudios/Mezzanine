// � Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _physicscollidablerayquery_h
#define _physicscollidablerayquery_h

#include "rayquery.h"

namespace Mezzanine
{
    namespace Physics
    {
        class PhysicsManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for performing ray queries in the Physics subsystem.
        ///////////////////////////////////////
        class MEZZ_LIB CollidableRayQuery : public RayQuery
        {
        protected:
            /// @brief A pointer to the PhysicsManager to be queried.
            PhysicsManager* PhysicsMan;
            /// @brief A mask of the types of objects to return as results.
            UInt32 ProxyTypesFilter;
            /// @brief A filter for categories of physics objects to return as results.
            UInt32 ColFilter;
        public:
            /// @brief Class constructor.
            /// @param ToQuery A pointer to the PhysicsManager to be queried.
            CollidableRayQuery(PhysicsManager* ToQuery);
            /// @brief Class destructor.
            virtual ~CollidableRayQuery();

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @copydoc RayQuery::SetWorld(World*)
            virtual void SetWorld(World* ToQuery);
            /// @copydoc RayQuery::GetWorld() const
            virtual World* GetWorld() const;
            /// @copydoc RayQuery::SetProxyTypes(const UInt32)
            virtual void SetProxyTypes(const UInt32 Filter);
            /// @copydoc RayQuery::GetProxyTypes() const
            virtual UInt32 GetProxyTypes() const;
            /// @copydoc RayQuery::SetSubSystemFilter(const UInt32)
            /// @remarks This method uses the CollisionFilter enum for it's filtering. @n
            /// Internally, only 16 bits of the field are used (unsigned short).
            virtual void SetSubSystemFilter(const UInt32 Filter);
            /// @copydoc RayQuery::GetSubSystemFilter() const
            /// @remarks This method returns a CollisionFilter enum. @n
            /// Internally, only 16 bits of the field are used (unsigned short).
            virtual UInt32 GetSubSystemFilter() const;

            /// @brief Sets the pointer to the manager being used to perform the Ray query.
            /// @param Manager A pointer to the PhysicsManager to be queried.
            void SetManager(PhysicsManager* Manager);
            /// @brief Gets the pointer to the manager being used to perform the Ray query.
            /// @return Returns a pointer to the PhysicsManager being queried.
            PhysicsManager* GetManager() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fast Query

            /// @copydoc RayQuery::GetFirstAABBResult(const Ray&)
            virtual RayQueryHit GetFirstAABBResult(const Ray& Cast) const;
            /// @copydoc RayQuery::GetAllAABBResults(const Ray&, const Whole)
            virtual ResultContainer GetAllAABBResults(const Ray& Cast, const Whole Limit = 0) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Precise Query

            /// @copydoc RayQuery::GetFirstShapeResult(const Ray&)
            virtual RayQueryHit GetFirstShapeResult(const Ray& Cast) const;
            /// @copydoc RayQuery::GetAllShapeResults(const Ray&, const Whole)
            virtual ResultContainer GetAllShapeResults(const Ray& Cast, const Whole Limit = 0) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that this CollidableRayQuery should be appended to.
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag the CollidableRayQuery class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//CollidableRayQuery
    }//Physics
}//Mezzanine

#endif
