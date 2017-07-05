// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

The Mezzanine Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The Mezzanine Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with The Mezzanine Engine. If not, see <http://www.gnu.org/licenses/>.
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
#ifndef _ghostproxy_h
#define _ghostproxy_h

#include "Physics/collidableproxy.h"

class btPairCachingGhostObject;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy object for ghost objects with no contact response.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GhostProxy : public CollidableProxy
        {
        protected:
            /// @internal
            /// @brief GhostBody proxy used by the internal physics.
            btPairCachingGhostObject* PhysicsGhostBody;

            /// @internal
            /// @brief Used to create the physics representation of the ghost body.
            /// @param Mass The mass of the ghost body to be created.
            virtual void CreateGhostObject();
        public:
            /// @brief Class constructor.
            /// @param ID The unique ID assigned to this proxy.
            /// @param Creator A pointer to the manager that created this proxy.
            GhostProxy(const UInt32 ID, PhysicsManager* Creator);
            /// @brief Shape constructor.
            /// @param ID The unique ID assigned to this proxy.
            /// @param Shape A pointer to the collision shape that will be applied to this proxy.
            /// @param Creator A pointer to the manager that created this proxy.
            GhostProxy(const UInt32 ID, CollisionShape* Shape, PhysicsManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this proxy.
            GhostProxy(const XML::Node& SelfRoot, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~GhostProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc EntityProxy::GetComponentType() const
            virtual Mezzanine::ComponentType GetComponentType() const;

            /// @copydoc EntityProxy::AddToWorld()
            virtual void AddToWorld();
            /// @copydoc EntityProxy::RemoveFromWorld()
            virtual void RemoveFromWorld();

            ///////////////////////////////////////////////////////////////////////////////
            // Overlapping Proxy Access

            /// @brief Gets a proxy overlapping with the AABB of this ghost.
            /// @note This method is faster than getting an overlap by collision shape, but it is also much less accurate.
            /// @param Index The index of the proxy to retrieve.
            /// @return Returns a pointer to the overlapping proxy at the specified index.
            virtual CollidableProxy* GetAABBOverlappingProxy(const UInt32 Index);
            /// @brief Gets the number of proxies overlapping with the AABB of this ghost.
            /// @return Returns the number of proxies overlapping with the AABB of this ghost.
            virtual UInt32 GetNumAABBOverlappingProxies() const;

            /// @brief Gets a proxy overlapping with the collision shape of this ghost.
            /// @note The underlying physics implementation tries to predict contacts in order to accelerate physics steps.  Because of this in some situations an invalid
            /// contact/collision may be generated between collision shapes.  This method detects that and will return NULL if that is the case. @n @n
            /// Also note that getting overlaps by shape is slower than getting them by AABB.  Only use this if you need the extra accuracy.
            /// @param Index The index of the proxy to retrieve.
            /// @return Returns a pointer to the overlapping proxy at the specified index, or NULL if it is invalid.
            virtual CollidableProxy* GetShapeOverlappingProxy(const UInt32 Index);
            /// @brief Gets the number of proxies overlapping with the actual collision shape of this ghost.
            /// @return Returns the number of proxies overlapping with the actual shape of this ghost.
            virtual UInt32 GetNumShapeOverlappingProxies() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc EntityProxy::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc EntityProxy::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc EntityProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc EntityProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal ghost body physics proxy.
            /// @return Returns a pointer to the internal proxy this proxy is based on.
            virtual btPairCachingGhostObject* _GetPhysicsObject() const;
            /// @copydoc CollidableProxy::_GetBasePhysicsObject()
            virtual btCollisionObject* _GetBasePhysicsObject() const;

        };//GhostProxy
    }//Physics
}//Mezzanine

#endif
