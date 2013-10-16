//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#include "attachable.h"
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
    /// @details
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
        /// @brief Basic container type for the current collisions applied to this object.
        typedef std::set< Physics::Collision* >        CollisionContainer;
        /// @brief Iterator type for the current collisions applied to this object.
        typedef CollisionContainer::iterator           CollisionIterator;
        /// @brief Const Iterator type for the current collisions applied to this object.
        typedef CollisionContainer::const_iterator     ConstCollisionIterator;
    protected:
        /// @internal
        /// @brief This member stores all existing collision events referencing this object.
        CollisionContainer CurrentCollisions;
        /// @internal
        /// @brief The name of the object.
        String ObjectName;
        /// @internal
        /// @brief This is the world this object belongs to and will be inserted in/removed from.
        World* ParentWorld;
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
        /// @return Returns a bool indicating if this object has been added to the world.
        virtual Bool IsInWorld() const = 0;

        /// @brief Checks of the object is static.
        /// @return Returns true if the object is static, false otherwise.
        virtual Bool IsStatic() const = 0;
        /// @brief Checks of the object is kinematic.
        /// @return Returns true if the object is kinematic, false otherwise.
        virtual Bool IsKinematic() const = 0;

        /// @brief Populates a container with all of the WorldProxies being used by this WorldObject.
        /// @param Proxies The container of proxies to be populated.
        virtual void GetProxies(ProxyContainer& Proxies) = 0;
        /// @brief Populates a container with all the WorldProxies being used by this WorldObject specified in a provided mask.
        /// @param Types The bitmask specifing the types of world proxies to populate the container with.
        /// @param Proxies The container of proxies to be populated.
        virtual void GetProxies(const UInt32 Types, ProxyContainer& Proxies) = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Working with the World

        /// @brief Adds the object to the World.
        virtual void AddToWorld() = 0;
        /// @brief Removes the object from the World.
        virtual void RemoveFromWorld() = 0;

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
        virtual void ProtoSerializeProxies(XML::Node& SelfRoot) const = 0;

        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the proxies of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        virtual void ProtoDeSerializeProxies(const XML::Node& SelfRoot) = 0;

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
        virtual void _Update() = 0;
        /// @internal
        /// @brief Notifies that a proxy belonging to this WorldObject is being forcibly destroyed, and it needs to update.
        /// @note Forced destruction should only happen if a subsystem is being shutdown at a time when the WorldObject is still operational.
        /// @param ToBeDestroyed The WorldProxy that is marked for destruction.
        virtual void _NotifyProxyDestroyed(WorldProxy* ToBeDestroyed) = 0;
        /// @internal
        /// @brief Notifies this World Object of a collision that is occuring with it.
        /// @param Col A pointer to the collision pertaining to this World Object.
        /// @param State The state of the collision pertaining to this World Object.
        virtual void _NotifyCollisionState(Physics::Collision* Col, const Physics::CollisionState State);
    };//WorldObject
}//Mezzanine

#endif
