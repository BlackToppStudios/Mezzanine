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
#ifndef _entity_h
#define _entity_h

#include "enumerations.h"
#include "transformableobject.h"
#include "sortedvector.h"
#include "entityfactory.h"

#ifndef SWIG
    #include "iteratorrange.h"
#endif

namespace Mezzanine
{
    class World;
    class EntityComponent;
    class EntityProxy;

    /// @brief A comparison method for EntityComponent instances for sorting.
    /// @param CompFirst The first Component to compare.
    /// @param CompSecond The second Component to compare.
    /// @return Returns true if the first Component should come before the second Component.
    Boole MEZZ_LIB ComponentCompare(EntityComponent* CompFirst, EntityComponent* CompSecond);

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for objects that exist in the World.
    ///////////////////////////////////////
    class MEZZ_LIB Entity : public TransformableObject
    {
    public:
        /// @brief Basic container type for EntityComponent storage by this class.
        using ComponentContainer = std::vector< EntityComponent* >;
        /// @brief Iterator type for EntityComponent instances stored by this class.
        using ComponentIterator = ComponentContainer::iterator;
        /// @brief Const Iterator type for EntityComponent instances stored by this class.
        using ConstComponentIterator = ComponentContainer::const_iterator;
        /// @brief Convenience type for passing around a range of components.
        using ComponentRange = IteratorRange<ComponentIterator>;
        /// @brief Convenience type for passing around a const range of components.
        using ConstComponentRange = IteratorRange<ConstComponentIterator>;
    protected:
        /// @brief A container of the components that share this objects transform.
        ComponentContainer Components;
        /// @brief The name of the object.
        String ObjectName;
        /// @brief A pointer to the proxy that is used for various queries, such as transform.
        EntityProxy* PrimaryProxy;
        /// @brief This is the world this object belongs to and will be inserted in/removed from.
        World* ParentWorld;
        /// @brief The index of the component container where proxies begin.
        UInt32 ProxyStartIndex;

        /// @brief Destroys every EntityComponent in this object.
        void DestroyAllComponents();
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        Entity(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        Entity(const String& Name, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this object belongs to.
        Entity(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~Entity() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        /// @brief Gets the type of the object instance
        /// @return Returns the type of the object instance
        virtual Mezzanine::EntityType GetEntityType() const;
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
        // Component Management

        /// @brief Adds a EntityComponent to this Entity.
        /// @param ToAdd A pointer to the EntityComponent to be added.
        virtual void AddComponent(EntityComponent* ToAdd);
        /// @brief Removes a EntityComponent from this Entity.
        /// @warning This method is intended for use with custom Entity implementations.  Many WorldEntities in engine
        /// care about the placement of Components and tampering with that can disrupt their operation.
        /// @param ToRemove A pointer to the EntityComponent to be removed.
        /// @return Returns a pointer to the removed EntityComponent or nullptr if the EntityComponent was not in this Entity.
        virtual EntityComponent* RemoveComponent(EntityComponent* ToRemove);
        /// @brief Removes all EntityComponent instances of the specified types from this Entity.
        /// @warning This method is intended for use with custom Entity implementations.  Many WorldEntities in engine
        /// care about the placement of Components and tampering with that can disrupt their operation.
        /// @param Type The type of EntityComponent to be removed.
        /// @return Returns a container of the removed Components.
        virtual Entity::ComponentContainer RemoveAllComponentsOfType(const ComponentType Type);
        /// @brief Removes all EntityComponent instances of the specified types from this Entity.
        /// @warning This method is intended for use with custom Entity implementations.  Many WorldEntities in engine
        /// care about the placement of Components and tampering with that can disrupt their operation.
        /// @param TypeFirst The first type in the range to be considered when removing Components.
        /// @param TypeLast The last type in the range to be considered when removing Components.
        /// @return Returns a container of the removed Components.
        virtual Entity::ComponentContainer RemoveAllComponentsOfTypes(const ComponentType TypeFirst, const ComponentType TypeLast);
        /// @brief Removes all EntityComponent instances from this Entity.
        /// @warning This method is intended for use with custom Entity implementations.  Many WorldEntities in engine
        /// care about the placement of Components and tampering with that can disrupt their operation.
        /// @return Returns a container of the removed Components.
        virtual Entity::ComponentContainer RemoveAllComponents();

        /// @brief Gets the number of EntityComponent instances in this Entity.
        /// @return Returns the number of WorldProxies being stored in this Entity.
        virtual Whole GetNumComponents() const;
        /// @brief Gets the EntityComponent by index.
        /// @param Index The index of the EntityComponent to retrieve.
        /// @return Returns a pointer to the EntityComponent at the specified index.
        virtual EntityComponent* GetComponent(const Whole Index) const;
        /// @brief Gets the Nth EntityComponent among the Components of the specified types.
        /// @param Type The type to be considered when getting the Nth EntityComponent.
        /// @param TypeIndex The Nth EntityComponent to retrieve.
        /// @return Returns a pointer to the EntityComponent at the specified index.
        virtual EntityComponent* GetComponent(const ComponentType Type, Whole TypeIndex) const;
        /// @brief Gets the Nth EntityComponent among the Components of the specified types.
        /// @param TypeFirst The first type in the range to be considered when getting the Nth component.
        /// @param TypeLast The last type in the range to be considered when getting the Nth component.
        /// @param TypeIndex The Nth EntityComponent to retrieve.
        /// @return Returns a pointer to the EntityComponent at the specified index.
        virtual EntityComponent* GetComponent(const ComponentType TypeFirst, const ComponentType TypeLast, Whole TypeIndex) const;

    #ifndef SWIG
        /// @brief Gets the range of non-proxy components in this Entity.
        /// @return Returns an iterator pair to the non-proxy portion of the components stored in this Entity.
        virtual Entity::ComponentRange GetNonProxyRange();
        /// @brief Gets the const range of non-proxy components in this Entity.
        /// @return Returns a const iterator pair to the non-proxy portion of the components stored in this Entity.
        virtual Entity::ConstComponentRange GetNonProxyRange() const;
        /// @brief Gets the range of proxy components in this Entity.
        /// @return Returns an iterator pair to the proxy portion of the components stored in this Entity.
        virtual Entity::ComponentRange GetProxyRange();
        /// @brief Gets the const range of proxy components in this Entity.
        /// @return Returns a const iterator pair to the proxy portion of the components stored in this Entity.
        virtual Entity::ConstComponentRange GetProxyRange() const;
    #endif // SWIG

        /// @brief Gets a container of the WorldProxies stored in this Entity.
        /// @return Returns a const reference to the internal EntityComponent storage.
        virtual const Entity::ComponentContainer& GetComponents() const;
        /// @brief Gets a container of the WorldProxies stored in this Entity.
        /// @param Type The type of EntityComponent to retrieve.
        /// @return Returns a container of all the Components in this Entity of the specified type.
        virtual Entity::ComponentContainer GetComponents(const ComponentType Type) const;
        /// @brief Gets a container of the WorldProxies stored in this Entity.
        /// @param TypeFirst The first type in the range to be considered when getting the Nth component.
        /// @param TypeLast The last type in the range to be considered when getting the Nth component.
        /// @return Returns a container of all the Components in this Entity of the specified type.
        virtual Entity::ComponentContainer GetComponents(const ComponentType TypeFirst, const ComponentType TypeLast) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        /// @brief Sets the primary proxy in this Entity.
        /// @remarks The primary proxy is responsible for being the Component to be queried where it doesn't
        /// make sense to query multiple Components, such as if you want to know the location of the Entity.
        /// The GetLocation() method is directed to the primary proxy.  This is not limited to just location,
        /// and is also responsible for other transform and non-transform queries. @n @n
        /// Every Entity instance should have the PrimaryProxy set as a part of it's initialization
        /// before use.  Some Entitys will do this automatically.  Some may not.  Consult the WorldEntities
        /// documentation.
        /// @param Primary The EntityProxy to be set as the primary proxy.
        virtual void SetPrimaryProxy(EntityProxy* Primary);
        /// @brief Gets the primary proxy in this Entity.
        /// @return Returns a pointer to the primary proxy of this Entity.
        virtual EntityProxy* GetPrimaryProxy() const;

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
        /// @brief Convert the Components of this class to an XML::Node ready for serialization.
        /// @param SelfRoot The root node containing all the serialized data for this instance.
        virtual void ProtoSerializeComponents(XML::Node& SelfRoot) const;

        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the Components of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeComponents(const XML::Node& SelfRoot);

        /// @brief Gets the most derived serializable name of this Entity.
        /// @note When creating a new Entity class verify this method has a valid return for it in order for serialization to work properly.
        /// @return Returns the name of the XML tag from the most derived class of "this".
        virtual String GetDerivedSerializableName() const;
        /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Performs any object specific behaviors that need to be updated per tick.
        /// @param Delta The amount of time since the last update in microseconds.
        virtual void _Update(const Whole Delta);
        /// @internal
        /// @brief Updates the transforms of all the EntityComponent instances in this object except for one.
        /// @param Exclude The EntityComponent to be exempted from the sync (usually because it already has the updated transform).
        /// @param NewTrans The transform to be applied to each EntityComponent other than the Exclude.
        virtual void _SyncTransforms(EntityProxy* Exclude, const Transform& NewTrans);
    };//Entity

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of generic Entity objects.
    ///////////////////////////////////////
    class MEZZ_LIB GenericEntityFactory : public EntityFactory
    {
    public:
        /// @brief Class constructor.
        GenericEntityFactory() = default;
        /// @brief Class destructor.
        virtual ~GenericEntityFactory() = default;

        /// @brief Gets the name of the Entity that is created by this factory.
        /// @return Returns the typename of the Entity created by this factory.
        virtual String GetTypeName() const;

        /// @copydoc Mezzanine::EntityFactory::CreateEntity(const String&, World*, const NameValuePairMap&)
        virtual Entity* CreateEntity(const String& Name, World* TheWorld, const NameValuePairMap& Params) override;
        /// @copydoc Mezzanine::EntityFactory::CreateEntity(const XML::Node&)
        virtual Entity* CreateEntity(const XML::Node& XMLNode, World* TheWorld) override;
        /// @copydoc Mezzanine::EntityFactory::DestroyEntity(Entity*)
        virtual void DestroyEntity(Entity* ToBeDestroyed) override;
    };//GenericEntityFactory
}//Mezzanine

#endif
