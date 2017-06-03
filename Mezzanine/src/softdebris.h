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
#ifndef _softdebris_h
#define _softdebris_h

/// @file
/// @brief This file contains the declaration for the debris class that will compress and deform.

#include "debris.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class EntityProxy;
    }
    namespace Physics
    {
        class SoftProxy;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A deformable debris.
    ///////////////////////////////////////
    class MEZZ_LIB SoftDebris : public Debris
    {
    public:
        /// @brief An enum specifying the indexes of the proxies that compose a RigidDebris.
        enum ProxyIndexes
        {
            PI_SofProx = 0,
            PI_EntProx,
            ProxyCount
        };
    protected:
        /// @internal
        /// @brief Common construction method for SoftDebris.
        /// @param Mass The mass of the debris object.
        virtual void CreateSoftDebris(const Real Mass);
        /// @internal
        /// @brief Destruction method for RigidDebris.
        virtual void DestroySoftDebris();
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        SoftDebris(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param Mass The mass of the debris object.
        /// @param TheWorld A pointer to the world this object belongs to.
        SoftDebris(const String& Name, const Real Mass, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this object belongs to.
        SoftDebris(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~SoftDebris();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        /// @copydoc Mezzanine::WorldObject::GetType() const
        virtual WorldObjectType GetType() const;

        /// @brief Gets a pointer to the graphics portion of this debris.
        /// @remarks This is a convenience method that just calls "GetProxy(const UInt32 Types, Whole TypeIndex) const",
        /// setting Mezzanine::PT_Graphics_EntityProxy as the type.  Depending on the world configuration one may not
        /// exist, for example of no SceneManager was created with the world.  Please be aware of these configurations
        /// when working with these methods.
        /// @return Returns a pointer to the Entity proxy representing the graphics portion of this debris.
        virtual Graphics::EntityProxy* GetEntityProxy() const;
        /// @brief Gets a pointer to the physics portion of this debris.
        /// @remarks This is a convenience method that just calls "GetProxy(const UInt32 Types, Whole TypeIndex) const",
        /// setting Mezzanine::PT_Physics_SoftProxy as the type.  Depending on the world configuration one may not
        /// exist, for example of no PhysicsManager was created with the world.  Please be aware of these configurations
        /// when working with these methods.
        /// @return Returns a pointer to the Soft proxy representing the physics portion of this debris.
        virtual Physics::SoftProxy* GetSoftProxy() const;

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

        /// @copydoc Mezzanine::WorldObject::_Update(const Whole)
        virtual void _Update(const Whole Delta);
    };//SoftDebris

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of deformable Debris objects.
    ///////////////////////////////////////
    class MEZZ_LIB SoftDebrisFactory : public DebrisFactory
    {
    public:
        /// @brief Class constructor.
        SoftDebrisFactory();
        /// @brief Class destructor.
        virtual ~SoftDebrisFactory();

        /// @copydoc DebrisFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a SoftDebris object.
        /// @param Name The name to be given to this object.
        /// @param Mass The mass of the debris object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual SoftDebris* CreateSoftDebris(const String& Name, const Real Mass, World* TheWorld);
        /// @brief Creates a SoftDebris object.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual SoftDebris* CreateSoftDebris(const XML::Node& XMLNode, World* TheWorld);

        /// @copydoc DebrisFactory::CreateDebris(const String&, World*, const NameValuePairMap&)
        virtual Debris* CreateDebris(const String& Name, World* TheWorld, const NameValuePairMap& Params);
        /// @copydoc DebrisFactory::CreateDebris(const XML::Node&, World*)
        virtual Debris* CreateDebris(const XML::Node& XMLNode, World* TheWorld);
        /// @copydoc DebrisFactory::DestroyDebris(Debris* ToBeDestroyed)
        virtual void DestroyDebris(Debris* ToBeDestroyed);
    };//SoftDebrisFactory
}//Mezzanine

#endif
