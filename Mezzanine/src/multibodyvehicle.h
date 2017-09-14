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
#ifndef _multibodyvehicle_h
#define _multibodyvehicle_h

/// @file
/// @brief This file contains the declaration for the MultiBodyVehicle class, which uses distinct objects for the wheels.

#include "vehicle.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A Vehicle class that utilizes constraints to compose a Vehicle from many objects.
    /// @details The primary objects expected to be distinct are the wheels.  They are completely separate
    /// objects that are attached and can be removed and reattached.  Their physical properties will
    /// determine a large number of behaviors for the vehicle.  This an expensive approach to vehicles.
    /// The Raycast Vehicle should be used any time it is appropriate to do so as it is much more efficient.
    ///////////////////////////////////////
    class MEZZ_LIB MultiBodyVehicle : public Vehicle
    {
    protected:
    public:
        /// @brief Class constructor.
        /// @param EntID The unique ID of the MultiBodyVehicle.
        /// @param TheWorld A pointer to the world this MultiBodyVehicle belongs to.
        MultiBodyVehicle(const EntityID& EntID, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this MultiBodyVehicle belongs to.
        MultiBodyVehicle(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~MultiBodyVehicle();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        /// @copydoc Mezzanine::Entity::GetEntityType() const
        virtual EntityType GetEntityType() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @copydoc Mezzanine::Entity::ProtoSerializeProperties(XML::Node&) const
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
        /// @copydoc Mezzanine::Entity::ProtoDeSerializeProperties(const XML::Node&)
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

        /// @copydoc Mezzanine::Entity::GetDerivedSerializableName() const
        virtual String GetDerivedSerializableName() const;
        /// @copydoc Mezzanine::Entity::GetSerializableName()
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc Mezzanine::Entity::_Update(const Whole)
        virtual void _Update(const Whole Delta);
    };//MultiBodyVehicle

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of MultiBodyVehicle objects.
    ///////////////////////////////////////
    class MEZZ_LIB MultiBodyVehicleFactory : public VehicleFactory
    {
    public:
        /// @brief Class constructor.
        MultiBodyVehicleFactory();
        /// @brief Class destructor.
        virtual ~MultiBodyVehicleFactory();

        /// @copydoc Mezzanine::EntityFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a MultiBodyVehicle.
        /// @param EntID The unique ID of the MultiBodyVehicle.
        /// @param TheWorld A pointer to the world this MultiBodyVehicle belongs to.
        virtual MultiBodyVehicle* CreateMultiBodyVehicle(const EntityID& EntID, World* TheWorld);

        /// @copydoc Mezzanine::EntityFactory::CreateEntity(const EntityID&, World*, const NameValuePairMap&)
        virtual Entity* CreateEntity(const EntityID& EntID, World* TheWorld, const NameValuePairMap& Params) override;
        /// @copydoc Mezzanine::EntityFactory::CreateEntity(const XML::Node&, World*)
        virtual Entity* CreateEntity(const XML::Node& XMLNode, World* TheWorld) override;
        /// @copydoc Mezzanine::EntityFactory::DestroyEntity(Entity*)
        virtual void DestroyEntity(Entity* ToBeDestroyed) override;
    };//MultiBodyVehicleFactory
}//Mezzanine

#endif
