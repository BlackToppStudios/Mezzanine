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
#ifndef _raycastvehicle_h
#define _raycastvehicle_h

/// @file
/// @brief This file contains the declaration for the RaycastVehicle object, which consists of one primary object that uses raycasts to simulate the wheels.

#include "vehicle.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A Vehicle class that utilizes raycasts to simulation it's interaction with the ground.
    /// @details This is a very efficient Vehicle but lacks some deeper customization.  It's suitable
    /// for many/most vehicle simulations and should be considered first for vehicle simulation.  The
    /// chassis and wheels are treated as the same object.  Skeletally animated vehicle meshes are
    /// expected for linking to parts of the physical simulation.
    ///////////////////////////////////////
    class MEZZ_LIB RaycastVehicle : public Vehicle
    {
    protected:
    public:
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        RaycastVehicle(const String& Name, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this object belongs to.
        RaycastVehicle(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~RaycastVehicle();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        /// @copydoc Mezzanine::WorldObject::GetType() const
        virtual WorldObjectType GetType() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @copydoc Mezzanine::WorldObject::ProtoSerializeProperties(XML::Node&) const
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
        /// @copydoc Mezzanine::WorldObject::ProtoDeSerializeProperties(const XML::Node&)
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

        /// @copydoc Mezzanine::WorldObject::GetDerivedSerializableName() const
        virtual String GetDerivedSerializableName() const;
        /// @copydoc Mezzanine::WorldObject::GetSerializableName()
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @copydoc Mezzanine::WorldObject::_Update()
        virtual void _Update();
    };//RaycastVehicle

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of RaycastVehicle objects.
    ///////////////////////////////////////
    class MEZZ_LIB RaycastVehicleFactory
    {
    public:
        /// @brief Class constructor.
        RaycastVehicleFactory();
        /// @brief Class destructor.
        virtual ~RaycastVehicleFactory();

        /// @copydoc VehicleFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a RaycastVehicle object.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual RaycastVehicle* CreateRaycastVehicle(const String& Name, World* TheWorld);

        /// @copydoc VehicleFactory::CreateVehicle(const String&, World*, const NameValuePairMap&)
        virtual Vehicle* CreateVehicle(const String& Name, World* TheWorld, const NameValuePairMap& Params);
        /// @copydoc VehicleFactory::CreateVehicle(const XML::Node&, World*)
        virtual Vehicle* CreateVehicle(const XML::Node& XMLNode, World* TheWorld);
        /// @copydoc VehicleFactory::DestroyVehicle(Vehicle*)
        virtual void DestroyVehicle(Vehicle* ToBeDestroyed);
    };//RaycastVehicleFactory
}//Mezzanine

#endif
