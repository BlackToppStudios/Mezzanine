// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _worldobject_h
#define _worldobject_h

#include "enumerations.h"
#include "transformableobject.h"
#include "Physics/physicsenumerations.h"

namespace Mezzanine
{
    class World;
    class WorldProxy;
    namespace Physics
    {
        class Collision;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class from which classes that are insertable into the physical world.
    ///////////////////////////////////////
    class MEZZ_LIB WorldObject : public TransformableObject
    {
    public:
        /// @brief Basic container type for WorldProxy storage by this class.
        typedef std::vector< WorldProxy* >             ProxyContainer;
        /// @brief Iterator type for WorldProxy instances stored by this class.
        typedef ProxyContainer::iterator               ProxyIterator;
        /// @brief Const Iterator type for WorldProxy instances stored by this class.
        typedef ProxyContainer::const_iterator         ConstProxyIterator;
    protected:
        /// @internal
        /// @brief A container of the proxies that share this objects transform.
        ProxyContainer Proxies;
        /// @internal
        /// @brief The name of the object.
        String ObjectName;
        /// @internal
        /// @brief A pointer to the proxy that is used for various queries, such as transform.
        WorldProxy* PrimaryProxy;
        /// @internal
        /// @brief This is the world this object belongs to and will be inserted in/removed from.
        World* ParentWorld;

        /// @internal
        /// @brief Destroys every WorldProxy in this object.
        void DestroyAllProxies();
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        WorldObject(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        WorldObject(const String& Name, World* TheWorld);
        /// @brief Class destructor.
        virtual ~WorldObject();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        /// @brief Gets the type of the object instance
        /// @return Returns the type of the object instance
        virtual Mezzanine::WorldObjectType GetType() const = 0;
        /// @brief Gets the name of this object.
        /// @return Returns a string containing the name of this object.
        virtual const String& GetName() const;
        /// @brief Gets the world this object currently belongs to.
        /// @return Returns a pointer to the world that owns this object.
        virtual World* GetWorld() const;

        /// @brief Gets whether or not this object is currently in the world.
        /// @remarks This will check the status of the first proxy (index 0) and query only it.
        /// If this object is empty the method will return false.
        /// @return Returns a Boole indicating if this object has been added to the world.
        virtual Boole IsInWorld() const;
        /// @brief Checks of the object is meant to have it's geometry/transform updated frequently.
        /// @remarks This will check the status of the first proxy (index 0) and query only it.
        /// If this object is empty the method will return true.
        /// @return Returns true if the object is meant to be stationary, false otherwise.
        virtual Boole IsStatic() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        /// @brief Adds a WorldProxy to this WorldObject.
        /// @remarks The order in which WorldProxy instances are inserted/stored slightly matters.  The first (index 0) proxy
        /// will be treated as the primary transform proxy.  When this object is queried for it's transform, that is the proxy
        /// that will be queried and thus used to represent this objects transform.  Prefab WorldObjects provided by the
        /// engine will have this taken care of for you.  However when using custom WorldObjects or heavily modifying/removing
        /// WorldProxy instances from a WorldObject, take care.
        /// @param ToAdd A pointer to the WorldProxy to be added.
        virtual void AddProxy(WorldProxy* ToAdd);
        /// @brief Removes a WorldProxy from this WorldObject.
        /// @warning This method is intended for use with custom WorldObject implementations.  Many WorldObjects in engine
        /// care about the placement of proxies and tampering with that can disrupt their operation.
        /// @param ToRemove A pointer to the WorldProxy to be removed.
        /// @return Returns a pointer to the removed WorldProxy or NULL if the WorldProxy was not in this WorldObject.
        virtual WorldProxy* RemoveProxy(WorldProxy* ToRemove);
        /// @brief Removes all WorldProxy instances of the specified types from this WorldObject.
        /// @warning This method is intended for use with custom WorldObject implementations.  Many WorldObjects in engine
        /// care about the placement of proxies and tampering with that can disrupt their operation.
        /// @param Types A bitfield of the types of WorldProxy instances to remove.
        /// @return Returns a container of the removed proxies.
        virtual ProxyContainer RemoveAllProxiesOfType(const UInt32 Types);
        /// @brief Removes all WorldProxy instances from this WorldObject.
        /// @warning This method is intended for use with custom WorldObject implementations.  Many WorldObjects in engine
        /// care about the placement of proxies and tampering with that can disrupt their operation.
        /// @return Returns a container of the removed proxies.
        virtual ProxyContainer RemoveAllProxies();

        /// @brief Gets the number of WorldProxy instances in this WorldObject.
        /// @return Returns the number of WorldProxies being stored in this WorldObject.
        virtual Whole GetNumProxies() const;
        /// @brief Gets the WorldProxy by index.
        /// @param Index The index of the WorldProxy to retrieve.
        /// @return Returns a pointer to the WorldProxy at the specified index.
        virtual WorldProxy* GetProxy(const Whole Index) const;
        /// @brief Gets the Nth WorldProxy among the proxies of the specified types.
        /// @param Types The types to be considered when getting the Nth WorldProxy.
        /// @param TypeIndex The Nth WorldProxy to retrieve.
        /// @return Returns a pointer to the WorldProxy at the specified index.
        virtual WorldProxy* GetProxy(const UInt32 Types, Whole TypeIndex) const;

        /// @brief Gets a container of the WorldProxies stored in this WorldObject.
        /// @return Returns a const reference to the internal WorldProxy storage.
        virtual const ProxyContainer& GetProxies() const;
        /// @brief Gets a container of the WorldProxies stored in this WorldObject.
        /// @param Types A bitfield of the types of WorldProxies to retrieve.
        /// @return Returns a container of all the proxies in this WorldObject of the specified type.
        virtual ProxyContainer GetProxies(const UInt32 Types) const;

        /// @brief Sets the primary proxy in this WorldObject.
        /// @remarks The primary proxy is responsible for being the WorldProxy to be queried where it doesn't
        /// make sense to query multiple proxies, such as if you want to know the location of the WorldObject.
        /// The GetLocation() method is directed to the primary proxy.  This is not limited to just location,
        /// and is also responsible for other transform and non-transform queries. @n @n
        /// Every WorldObject instance should have the PrimaryProxy set as a part of it's initialization
        /// before use.  Some WorldObjects will do this automatically.  Some may not.  Consult the WorldObjects
        /// documentation.
        /// @param Primary The WorldProxy to be set as the primary proxy.
        virtual void SetPrimaryProxy(WorldProxy* Primary);
        /// @brief Gets the primary proxy in this WorldObject.
        /// @return Returns a pointer to the primary proxy of this WorldObject.
        virtual WorldProxy* GetPrimaryProxy() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Working with the World

        /// @brief Adds the object to the World.
        virtual void AddToWorld();
        /// @brief Removes the object from the World.
        virtual void RemoveFromWorld();

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        /// @copydoc TransformableObject::SetTransform(const Transform&)
        virtual void SetTransform(const Transform& Trans);
        /// @copydoc TransformableObject::SetTransform(const Vector3&,const Quaternion&)
        virtual void SetTransform(const Vector3& Loc, const Quaternion& Ori);
        /// @copydoc TransformableObject::GetTransform() const
        virtual Transform GetTransform() const;

        /// @copydoc TransformableObject::SetLocation(const Vector3&)
        virtual void SetLocation(const Vector3& Loc);
        /// @copydoc TransformableObject::SetLocation(const Real, const Real, const Real)
        virtual void SetLocation(const Real X, const Real Y, const Real Z);
        /// @copydoc TransformableObject::GetLocation() const
        virtual Vector3 GetLocation() const;
        /// @copydoc TransformableObject::SetOrientation(const Quaternion&)
        virtual void SetOrientation(const Quaternion& Ori);
        /// @copydoc TransformableObject::SetOrientation(const Real, const Real, const Real, const Real)
        virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
        /// @copydoc TransformableObject::GetOrientation() const
        virtual Quaternion GetOrientation() const;
        /// @copydoc TransformableObject::SetScale(const Vector3&)
        virtual void SetScale(const Vector3& Sc);
        /// @copydoc TransformableObject::SetScale(const Real, const Real, const Real)
        virtual void SetScale(const Real X, const Real Y, const Real Z);
        /// @copydoc TransformableObject::GetScale() const
        virtual Vector3 GetScale() const;

        /// @copydoc TransformableObject::Translate(const Vector3&)
        virtual void Translate(const Vector3& Trans);
        /// @copydoc TransformableObject::Translate(const Real, const Real, const Real)
        virtual void Translate(const Real X, const Real Y, const Real Z);
        /// @copydoc TransformableObject::Yaw(const Real)
        virtual void Yaw(const Real Angle);
        /// @copydoc TransformableObject::Pitch(const Real)
        virtual void Pitch(const Real Angle);
        /// @copydoc TransformableObject::Roll(const Real)
        virtual void Roll(const Real Angle);
        /// @copydoc TransformableObject::Rotate(const Vector3&, const Real)
        virtual void Rotate(const Vector3& Axis, const Real Angle);
        /// @copydoc TransformableObject::Rotate(const Quaternion&)
        virtual void Rotate(const Quaternion& Rotation);
        /// @copydoc TransformableObject::Scale(const Vector3&)
        virtual void Scale(const Vector3& Sc);
        /// @copydoc TransformableObject::Scale(const Real, const Real, const Real)
        virtual void Scale(const Real X, const Real Y, const Real Z);

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
        virtual void ProtoSerialize(XML::Node& ParentNode) const;
        /// @brief Convert the properties of this class to an XML::Node ready for serialization.
        /// @param SelfRoot The root node containing all the serialized data for this instance.
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
        /// @brief Convert the proxies of this class to an XML::Node ready for serialization.
        /// @param SelfRoot The root node containing all the serialized data for this instance.
        virtual void ProtoSerializeProxies(XML::Node& SelfRoot) const;

        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the proxies of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeProxies(const XML::Node& SelfRoot);

        /// @brief Gets the most derived serializable name of this WorldObject.
        /// @note When creating a new WorldObject class verify this method has a valid return for it in order for serialization to work properly.
        /// @return Returns the name of the XML tag from the most derived class of "this".
        virtual String GetDerivedSerializableName() const;
        /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Utility function for altering or checking the World Object every frame.
        /// @param Delta The amount of time since the last update in microseconds.
        virtual void _Update(const Whole Delta) = 0;
        /// @internal
        /// @brief Updates the transforms of all the WorldProxy instances in this object except for one.
        /// @param Exclude The WorldProxy to be exempted from the sync (usually because it already has the updated transform).
        /// @param NewTrans The transform to be applied to each WorldProxy other than the Exclude.
        virtual void _SyncTransforms(WorldProxy* Exclude, const Transform& NewTrans);
    };//WorldObject
}//Mezzanine

#endif
