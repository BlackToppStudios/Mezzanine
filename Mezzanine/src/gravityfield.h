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
#ifndef _gravityfield_h
#define _gravityfield_h

#include "areaeffect.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an implementation of the AreaEffect class that alters gravity in a region.
    /// @details This class is not a gravity well, where gravity is pulling to one point.  Instead this class uniformly
    /// pulls gravity in one direction that is different from the world gravity.
    ///////////////////////////////////////
    class MEZZ_LIB GravityField : public AreaEffect
    {
    protected:
        /// @internal
        /// @brief The stored value for this fields gravity.
        Vector3 Grav;
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        GravityField(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        GravityField(const String& Name, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @param TheWorld A pointer to the world this object belongs to.
        GravityField(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~GravityField();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Mezzanine::WorldObject::GetType() const
        virtual Mezzanine::WorldObjectType GetType() const;

        /// @copydoc Mezzanine::WorldObject::ApplyEffect()
        virtual void ApplyEffect();

        ///////////////////////////////////////////////////////////////////////////////
        // GravityField Properties

        /// @brief Sets the gravity force for this field.
        /// @param Gravity The vector3 representing the force and direction of gravity this field will have.
        virtual void SetFieldGravity(const Vector3& Gravity);
        /// @brief Gets the gravity of this field.
        /// @return Returns a vector3 representing the force and direction of gravity this field has.
        virtual Vector3 GetFieldGravity() const;

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
    };//GravityField

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of GravityField objects.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB GravityFieldFactory : public AreaEffectFactory
    {
    public:
        /// @brief Class constructor.
        GravityFieldFactory();
        /// @brief Class destructor.
        virtual ~GravityFieldFactory();

        /// @copydoc Mezzanine::WorldObjectFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a GravityField object.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual GravityField* CreateGravityField(const String& Name, World* TheWorld);
        /// @brief Creates a GravityField object.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual GravityField* CreateGravityField(const XML::Node& XMLNode, World* TheWorld);

        /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const String&, World*, const NameValuePairMap&)
        virtual AreaEffect* CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params);
        /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(XML::Node&)
        virtual AreaEffect* CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld);
        /// @copydoc Mezzanine::WorldObjectFactory::DestroyAreaEffect(AreaEffect*)
        virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
    };//GravityFieldFactory
}//Mezzanine

#endif
