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
#ifndef _simplevehicle_cpp
#define _simplevehicle_cpp

/// @file
/// @brief This file contains the implementation for the SimpleVehicle, which unlike other vehicle classes does not attempt to simulate wheels.

#include "simplevehicle.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // SimpleVehicle Methods

    SimpleVehicle::SimpleVehicle(const EntityID& EntID, World* TheWorld) :
        Vehicle(EntID,TheWorld)
    {

    }

    SimpleVehicle::SimpleVehicle(const XML::Node& SelfRoot, World* TheWorld) :
        Vehicle(TheWorld)
    {

    }

    SimpleVehicle::~SimpleVehicle()
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility and Configuration

    EntityType SimpleVehicle::GetEntityType() const
        { return Mezzanine::ET_SimpleVehicle; }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization

    void SimpleVehicle::ProtoSerializeProperties(XML::Node& SelfRoot) const
    {

    }

    void SimpleVehicle::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
    {

    }

    String SimpleVehicle::GetDerivedSerializableName() const
        { return SimpleVehicle::GetSerializableName(); }

    String SimpleVehicle::GetSerializableName()
        { return "SimpleVehicle"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void SimpleVehicle::_Update(const Whole Delta)
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    // SimpleVehicleFactory Methods

    SimpleVehicleFactory::SimpleVehicleFactory()
    {  }

    SimpleVehicleFactory::~SimpleVehicleFactory()
    {  }

    String SimpleVehicleFactory::GetTypeName() const
        { return SimpleVehicle::GetSerializableName(); }

    SimpleVehicle* SimpleVehicleFactory::CreateSimpleVehicle(const EntityID& EntID, World* TheWorld)
        { return new SimpleVehicle(EntID,TheWorld); }

    Entity* SimpleVehicleFactory::CreateEntity(const EntityID& EntID, World* TheWorld, const NameValuePairMap& Params)
    {
        return this->CreateSimpleVehicle(EntID,TheWorld);
    }

    Entity* SimpleVehicleFactory::CreateEntity(const XML::Node& XMLNode, World* TheWorld)
        { return new SimpleVehicle(XMLNode,TheWorld); }

    void SimpleVehicleFactory::DestroyEntity(Entity* ToBeDestroyed)
        { delete ToBeDestroyed; }
}//Mezzanine

#endif
