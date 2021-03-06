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
#ifndef _rigiddebris_h
#define _rigiddebris_h

/// @file
/// @brief This file contains the declaration for the debris class that does not deform.

#include "debris.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class ItemProxy;
        class Mesh;
    }
    namespace Physics
    {
        class RigidProxy;
        class CollisionShape;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A non-deformable debris.
    ///////////////////////////////////////
    class MEZZ_LIB RigidDebris : public Debris
    {
    protected:
        /// @internal
        /// @brief Common construction method for RigidDebris.
        /// @param Mass The mass of the debris object.
        virtual void CreateRigidDebris(const Real Mass);
        /// @internal
        /// @brief Verbose construction method for RigidDebris.
        /// @param Mass The mass of the debris object.
        /// @param DebMesh A pointer to the Graphics mesh that will be applied to the new RigidDebris.
        /// @param DebShape A pointer to the Collision shape that will be applied to the new RigidDebris.
        virtual void CreateRigidDebris(const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape);
        /// @internal
        /// @brief Destruction method for RigidDebris.
        virtual void DestroyRigidDebris();
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        RigidDebris(World* TheWorld);
        /// @brief Class constructor.
        /// @param EntID The unique ID of the RigidDebris.
        /// @param Mass The mass of the RigidDebris.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        RigidDebris(const EntityID& EntID, const Real Mass, World* TheWorld);
        /// @brief Class constructor.
        /// @param EntID The unique ID of the RigidDebris.
        /// @param Mass The mass of the RigidDebris.
        /// @param DebMesh A pointer to the Graphics mesh that will be applied to the new RigidDebris.
        /// @param DebShape A pointer to the Collision shape that will be applied to the new RigidDebris.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        RigidDebris(const EntityID& EntID, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        RigidDebris(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~RigidDebris();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        /// @copydoc Mezzanine::Entity::GetEntityType() const
        virtual EntityType GetEntityType() const;

        /// @brief Gets a pointer to the graphics portion of this debris.
        /// @remarks This is a convenience method that just calls "GetProxy(const UInt32 Types, Whole TypeIndex) const",
        /// setting Mezzanine::CT_Graphics_EntityProxy as the type.  Depending on the world configuration one may not
        /// exist, for example of no SceneManager was created with the world.  Please be aware of these configurations
        /// when working with these methods.
        /// @return Returns a pointer to the Item proxy representing the graphics portion of this debris.
        virtual Graphics::ItemProxy* GetItemProxy() const;
        /// @brief Gets a pointer to the physics portion of this debris.
        /// @remarks This is a convenience method that just calls "GetProxy(const UInt32 Types, Whole TypeIndex) const",
        /// setting Mezzanine::CT_Physics_RigidProxy as the type.  Depending on the world configuration one may not
        /// exist, for example of no PhysicsManager was created with the world.  Please be aware of these configurations
        /// when working with these methods.
        /// @return Returns a pointer to the Rigid proxy representing the physics portion of this debris.
        virtual Physics::RigidProxy* GetRigidProxy() const;

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
    };//RigidDebris

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of non-deformable Debris objects.
    ///////////////////////////////////////
    class MEZZ_LIB RigidDebrisFactory : public DebrisFactory
    {
    public:
        /// @brief Class constructor.
        RigidDebrisFactory();
        /// @brief Class destructor.
        virtual ~RigidDebrisFactory();

        /// @copydoc DebrisFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a RigidDebris.
        /// @param EntID The unique ID of the RigidDebris.
        /// @param Mass The mass of the RigidDebris.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        virtual RigidDebris* CreateRigidDebris(const EntityID& EntID, const Real Mass, World* TheWorld);
        /// @brief Creates a RigidDebris.
        /// @param EntID The unique ID of the RigidDebris.
        /// @param Mass The mass of the RigidDebris.
        /// @param DebMesh A pointer to the Graphics mesh that will be applied to the RigidDebris.
        /// @param DebShape A pointer to the Collision shape that will be applied to the RigidDebris.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        virtual RigidDebris* CreateRigidDebris(const EntityID& EntID, const Real Mass, Graphics::Mesh* DebMesh, Physics::CollisionShape* DebShape, World* TheWorld);
        /// @brief Creates a RigidDebris.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this RigidDebris belongs to.
        virtual RigidDebris* CreateRigidDebris(const XML::Node& XMLNode, World* TheWorld);

        /// @copydoc EntityFactory::CreateEntity(const EntityID&, World*, const NameValuePairMap&)
        virtual Entity* CreateEntity(const EntityID& EntID, World* TheWorld, const NameValuePairMap& Params) override;
        /// @copydoc EntityFactory::CreateEntity(const XML::Node&, World*)
        virtual Entity* CreateEntity(const XML::Node& XMLNode, World* TheWorld) override;
        /// @copydoc EntityFactory::DestroyEntity(Entity* ToBeDestroyed)
        virtual void DestroyEntity(Entity* ToBeDestroyed) override;
    };//RigidDebrisFactory
}//Mezzanine

#endif
