//c Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _softproxy_h
#define _softproxy_h

#include "Physics/physicsproxy.h"

class btSoftBody;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy object for soft/compressable bodies.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SoftProxy : public PhysicsProxy
        {
        protected:
            /// @internal
            /// @brief SoftBody proxy used by the internal physics.
            btSoftBody* PhysicsSoftBody;

            /// @internal
            /// @brief Used to create the physics representation of the soft body.
            /// @param Mass The mass of the soft body to be created.
            virtual void CreateSoftObject(const Real Mass);
        public:
            /// @brief Class constructor.
            /// @param Mass The combined mass of the nodes in the soft body.
            /// @param Creator A pointer to the manager that created this proxy.
            SoftProxy(const Real Mass, PhysicsManager* Creator);
            /// @brief Class destructor.
            virtual ~SoftProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldProxy::GetProxyType() const
            virtual Mezzanine::ProxyType GetProxyType() const;

            /// @copydoc WorldProxy::AddToWorld()
            virtual void AddToWorld();
            /// @copydoc WorldProxy::RemoveFromWorld()
            virtual void RemoveFromWorld();

            ///////////////////////////////////////////////////////////////////////////////
            // Collision Settings

            /// @copydoc PhysicsProxy::SetCollisionShape(CollisionShape*)
            virtual void SetCollisionShape(CollisionShape* Shape);

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @copydoc WorldProxy::SetLocation(const Vector3&)
            virtual void SetLocation(const Vector3& Loc);
            /// @copydoc WorldProxy::SetLocation(const Real, const Real, const Real)
            virtual void SetLocation(const Real X, const Real Y, const Real Z);
            /// @copydoc WorldProxy::GetLocation() const
            virtual Vector3 GetLocation() const;
            /// @copydoc WorldProxy::SetOrientation(const Quaternion&)
            virtual void SetOrientation(const Quaternion& Ori);
            /// @copydoc WorldProxy::SetOrientation(const Real, const Real, const Real, const Real)
            virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
            /// @copydoc WorldProxy::GetOrientation() const
            virtual Quaternion GetOrientation() const;
            /// @copydoc PhysicsProxy::SetScale(const Vector3&)
            virtual void SetScale(const Vector3& Sc);
            /// @copydoc PhysicsProxy::SetScale(const Real, const Real, const Real)
            virtual void SetScale(const Real X, const Real Y, const Real Z);
            /// @copydoc WorldProxy::GetScale() const
            virtual Vector3 GetScale() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @copydoc PhysicsProxy::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the nodes (and their specific properties) of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSeriailzeNodes(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @copydoc PhysicsProxy::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the nodes (and their specific properties) of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSeriailzeNodes(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal soft body physics proxy.
            /// @return Returns a pointer to the internal proxy this proxy is based on.
            virtual btSoftBody* _GetPhysicsObject() const;
            /// @copydoc PhysicsProxy::_GetBasePhysicsObject()
            virtual btCollisionObject* _GetBasePhysicsObject() const;
        };//SoftProxy
    }//Physics
}//Mezzanine

#endif
