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
#ifndef _areaeffect_h
#define _areaeffect_h

#include "worldobject.h"
#include "colourvalue.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class EntityProxy;
        class ParticleSystemProxy;
    }
    namespace Physics
    {
        class GhostProxy;
        class CollisionShape;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This class is used to define area's in the world that have unique effects.
    /// @details Common uses for this class are for gravity fields, and explosions.  But can be made to do more. @n
    /// Note: This is a base class intended to be derived from.  This class cannot be created itself.  To make an
    /// AreaEffect class that does what you want it to, simple inherit from this class with an AE class of your own,
    /// and define the ApplyEffect() function to do what you want your effect to do.
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffect : public WorldObject
    {
    public:
        /// @brief Basic container type for Object storage by this class.
        typedef std::vector< WorldObject* >         ObjectContainer;
        /// @brief Iterator type for Object instances stored by this class.
        typedef ObjectContainer::iterator           ObjectIterator;
        /// @brief Const Iterator type for Object instances stored by this class.
        typedef ObjectContainer::const_iterator     ConstObjectIterator;
    protected:
        /// @internal
        /// @brief Container for actors within the field area.
        ObjectContainer OverlappingObjects;
        /// @internal
        /// @brief Container of actors that have been added since last frame.
        ObjectContainer AddedObjects;
        /// @internal
        /// @brief Container of actors that have been removed since last frame.
        ObjectContainer RemovedObjects;
        /// @internal
        /// @brief Container of proxies that representing this AE field.
        ProxyContainer AEProxies;
        /// @internal
        /// @brief A pointer to the ghost powering this AE field.
        Physics::GhostProxy* Ghost;

        /// @internal
        /// @brief Common constructor method for AreaEffect base class.
        /// @param Shape A pointer to the collision shape that will be applied to this object.
        virtual void CreateAreaEffect(Physics::CollisionShape* Shape);
        /// @internal
        /// @brief Common destructor method for AreaEffect base class.
        virtual void DestroyAreaEffect();
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        AreaEffect(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        AreaEffect(const String& Name, World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param Shape A pointer to the collision shape that will be applied to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        AreaEffect(const String& Name, Physics::CollisionShape* Shape, World* TheWorld);
        /// @brief Class destructor.
        virtual ~AreaEffect();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Mezzanine::WorldObject::GetType() const
        virtual WorldObjectType GetType() const;

        /// @brief Gets a pointer to the physics portion of this AreaEffect.
        /// @return Returns a pointer to the Ghost proxy representing the physics portion of this AreaEffect.
        virtual Physics::GhostProxy* GetGhostProxy() const;

        /// @brief Defines and applies the effect of the field.
        /// @details When inheriting this class, this function is what defines the effect the field has. @n
        /// This function will be called on by the physics manager and shouldn't be called manually.
        virtual void ApplyEffect() = 0;

        /// @copydoc Mezzanine::WorldObject::IsInWorld() const
        virtual Boole IsInWorld() const;

        /// @copydoc Mezzanine::WorldObject::IsStatic() const
        virtual Boole IsStatic() const;
        /// @copydoc Mezzanine::WorldObject::IsKinematic() const
        virtual Boole IsKinematic() const;

        /// @copydoc Mezzanine::WorldObject::GetProxies(ProxyContainer&)
        virtual void GetProxies(ProxyContainer& Proxies);
        /// @copydoc Mezzanine::WorldObject::GetProxies(const UInt32, ProxyContainer&)
        virtual void GetProxies(const UInt32 Types, ProxyContainer& Proxies);

        ///////////////////////////////////////////////////////////////////////////////
        // Working with the World

        /// @copydoc Mezzanine::WorldObject::AddToWorld()
        virtual void AddToWorld();
        /// @copydoc Mezzanine::WorldObject::RemoveFromWorld()
        virtual void RemoveFromWorld();

        ///////////////////////////////////////////////////////////////////////////////
        // Overlapping Object Management

        /// @brief Gets the number of objects currently overlapping with this AE.
        /// @return Returns the number of objects inside this AE.
        UInt32 GetNumOverlappingObjects() const;
        /// @brief Gets the number of objects added to this AE since the last update.
        /// @note Updates are usually once per frame, but can be manipulated to be more or less frequent.
        /// @return Returns the number of new objects overlapping with this AE.
        UInt32 GetNumAddedObjects() const;
        /// @brief Gets the number of objects removed from this AE since the last update.
        /// @note Updates are usually once per frame, but can be manipulated to be more or less frequent.
        /// @return Returns the number of objects that were found to no longer be overlapping during the last update.
        UInt32 GetNumRemovedObjects() const;
        /// @brief Gets the list of objects within this field.
        /// @return Returns the list of objects contained within this field.
        ObjectContainer& GetOverlappingObjects();
        /// @brief Gets the list of objects that have been added to the list since the last simulation step.
        /// @return Returns the vector storing all the objects that have been added to the list since the last simulation step.
        ObjectContainer& GetAddedObjects();
        /// @brief Gets the list of objects that have been removed from the list since the last simulation step.
        /// @return Returns the vector storing all the objects that have been removed from the list since the last simulation step.
        ObjectContainer& GetRemovedObjects();

        ///////////////////////////////////////////////////////////////////////////////
        // AreaEffect Properties

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

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
        virtual void Scale(const Vector3& Scale);
        /// @copydoc TransformableObject::Scale(const Real, const Real, const Real)
        virtual void Scale(const Real X, const Real Y, const Real Z);

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @copydoc Mezzanine::WorldObject::ProtoSerializeProperties(XML::Node& SelfRoot) const
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
        /// @copydoc Mezzanine::WorldObject::ProtoSerializeProxies(XML::Node&) const
        virtual void ProtoSerializeProxies(XML::Node& SelfRoot) const;

        /// @copydoc Mezzanine::WorldObject::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
        /// @copydoc Mezzanine::WorldObject::ProtoDeSerializeProxies(const XML::Node&)
        virtual void ProtoDeSerializeProxies(const XML::Node& SelfRoot);

        /// @copydoc Mezzanine::WorldObject::GetDerivedSerializableName() const
        virtual String GetDerivedSerializableName() const;
        /// @copydoc Mezzanine::WorldObject::GetSerializableName()
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc Mezzanine::WorldObject::_Update()
        virtual void _Update();
        /// @copydoc Mezzanine::WorldObject::_NotifyProxyDestroyed(WorldProxy*)
        virtual void _NotifyProxyDestroyed(WorldProxy* ToBeDestroyed);
    };//AreaEffect

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of AreaEffect objects.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffectFactory
    {
    public:
        /// @brief Class constructor.
        AreaEffectFactory() {  }
        /// @brief Class destructor.
        virtual ~AreaEffectFactory() {  }

        /// @brief Gets the name of the AreaEffect that is created by this factory.
        /// @return Returns the typename of the AreaEffect created by this factory.
        virtual String GetTypeName() const = 0;

        /// @brief Creates a AreaEffect of the type represented by this factory.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        /// @param Params A NameValuePairList containing the params to be applied during construction.
        /// @return Returns a pointer to the AreaEffect created.
        virtual AreaEffect* CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params) = 0;
        /// @brief Creates a AreaEffect from XML.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        /// @return Returns a pointer to the AreaEffect created.
        virtual AreaEffect* CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld) = 0;
        /// @brief Destroys a AreaEffect created by this factory.
        /// @param ToBeDestroyed A pointer to the AreaEffect to be destroyed.
        virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed) = 0;
    };//AreaEffectFactory
}//Mezzanine

#endif
