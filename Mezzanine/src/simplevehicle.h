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
#ifndef _simplevehicle_h
#define _simplevehicle_h

/// @file
/// @brief This file contains the declaration for the SimpleVehicle, which unlike other vehicle classes does not attempt to simulate wheels.

#include "vehicle.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A vehicle class that doesn't attempt to do anything fancy.
    /// @details The other Vehicle classes focus on how to simulate wheels in various ways.  But for vehicles without wheels, or even vehicles
    /// with a very different wheel implementation can be simulated with this Vehicle class.  It doesn't do any tricks with wheels, just provides
    /// a simple base to work with.
    ///////////////////////////////////////
    class MEZZ_LIB SimpleVehicle : public Vehicle
    {
    protected:
    public:
        /// @brief Class constructor.
        /// @param EntID The unique ID of the SimpleVehicle.
        /// @param TheWorld A pointer to the world this SimpleVehicle belongs to.
        SimpleVehicle(const EntityID& EntID, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this SimpleVehicle belongs to.
        SimpleVehicle(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~SimpleVehicle();

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
    };//SimpleVehicle

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of SimpleVehicle objects.
    ///////////////////////////////////////
    class MEZZ_LIB SimpleVehicleFactory : public VehicleFactory
    {
    public:
        /// @brief Class constructor.
        SimpleVehicleFactory();
        /// @brief Class destructor.
        virtual ~SimpleVehicleFactory();

        /// @copydoc Mezzanine::EntityFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a SimpleVehicle.
        /// @param EntID The unique ID of the SimpleVehicle.
        /// @param TheWorld A pointer to the world this SimpleVehicle belongs to.
        virtual SimpleVehicle* CreateSimpleVehicle(const EntityID& EntID, World* TheWorld);

        /// @copydoc Mezzanine::EntityFactory::CreateEntity(const EntityID&, World*, const NameValuePairMap&)
        virtual Entity* CreateEntity(const EntityID& EntID, World* TheWorld, const NameValuePairMap& Params) override;
        /// @copydoc Mezzanine::EntityFactory::CreateEntity(const XML::Node&, World*)
        virtual Entity* CreateEntity(const XML::Node& XMLNode, World* TheWorld) override;
        /// @copydoc Mezzanine::EntityFactory::DestroyEntity(Entity*)
        virtual void DestroyEntity(Entity* ToBeDestroyed) override;
    };//SimpleVehicleFactory
}//Mezzanine

#endif
