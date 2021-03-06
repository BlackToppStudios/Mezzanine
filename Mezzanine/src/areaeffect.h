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
#ifndef _areaeffect_h
#define _areaeffect_h

#include "entity.h"
#include "entityfactory.h"
#include "colourvalue.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class ItemProxy;
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
    class MEZZ_LIB AreaEffect : public Entity
    {
    public:
        /// @brief Basic container type for Object storage by this class.
        typedef std::vector< Entity* >              ObjectContainer;
        /// @brief Iterator type for Object instances stored by this class.
        typedef ObjectContainer::iterator           ObjectIterator;
        /// @brief Const Iterator type for Object instances stored by this class.
        typedef ObjectContainer::const_iterator     ConstObjectIterator;
    protected:
        /// @brief Container for actors within the field area.
        ObjectContainer OverlappingObjects;
        /// @brief Container of actors that have been added since last frame.
        ObjectContainer AddedObjects;
        /// @brief Container of actors that have been removed since last frame.
        ObjectContainer RemovedObjects;
        /// @brief An optional pointer to an object this AreaEffect will follow.
        Entity* SyncTarget;

        /// @brief Common constructor method for AreaEffect base class.
        /// @param Shape A pointer to the collision shape that will be applied to this object.
        virtual void CreateAreaEffect(Physics::CollisionShape* Shape);
        /// @internal
        /// @brief Common destructor method for AreaEffect base class.
        virtual void DestroyAreaEffect();
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this AreaEffect belongs to.
        AreaEffect(World* TheWorld);
        /// @brief Class constructor.
        /// @param EntID The unique ID of the AreaEffect.
        /// @param TheWorld A pointer to the world this AreaEffect belongs to.
        AreaEffect(const EntityID& EntID, World* TheWorld);
        /// @brief Class constructor.
        /// @param EntID The unique ID of the AreaEffect.
        /// @param Shape A pointer to the collision shape that will be applied to this AreaEffect.
        /// @param TheWorld A pointer to the world this AreaEffect belongs to.
        AreaEffect(const EntityID& EntID, Physics::CollisionShape* Shape, World* TheWorld);
        /// @brief Class destructor.
        virtual ~AreaEffect();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Mezzanine::Entity::GetType() const
        virtual EntityType GetType() const;
        /// @brief Gets a pointer to the physics portion of this AreaEffect.
        /// @return Returns a pointer to the Ghost proxy representing the physics portion of this AreaEffect.
        virtual Physics::GhostProxy* GetGhostProxy() const;

        /// @brief Defines and applies the effect of the field.
        /// @details When inheriting this class, this function is what defines the effect the field has. @n
        /// This function will be called on by the physics manager and shouldn't be called manually.
        virtual void ApplyEffect();

        /// @brief Gets the Entity this AreaEffect will sync it's transform with.
        /// @param ToSync A pointer to the Entity to sync transforms with.
        virtual void SetSyncTarget(Entity* ToSync);
        /// @brief Gets the Entity this AreaEffect is syncing it's transform with.
        /// @return Returns a pointer to the Entity that will be sync'd with.
        virtual Entity* GetSyncTarget() const;

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
        // Serialization

        /// @copydoc Mezzanine::Entity::ProtoSerializeProperties(XML::Node& SelfRoot) const
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
        /// @copydoc Mezzanine::Entity::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

        /// @copydoc Mezzanine::Entity::GetDerivedSerializableName() const
        virtual String GetDerivedSerializableName() const;
        /// @copydoc Mezzanine::Entity::GetSerializableName()
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc Mezzanine::Entity::_Update(const Whole)
        virtual void _Update(const Whole Delta);
    };//AreaEffect

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of AreaEffect objects.
    ///////////////////////////////////////
    class MEZZ_LIB AreaEffectFactory : public EntityFactory
    {
    public:
        /// @brief Class constructor.
        AreaEffectFactory() = default;
        /// @brief Class destructor.
        virtual ~AreaEffectFactory() = default;
    };//AreaEffectFactory
}//Mezzanine

#endif
