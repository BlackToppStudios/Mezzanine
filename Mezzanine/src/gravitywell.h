// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _gravitywell_h
#define _gravitywell_h

#include "areaeffect.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a gravity well implementation of the AreaEffect class.
    /// @details This class is not a gravity field, where gravity only pulls in one direction.  Instead this class will always
    /// pull objects inside it towards the field center.  @n
    /// This class works best with sphere's, but can be made to work with any shape.
    ///////////////////////////////////////
    class MEZZ_LIB GravityWell : public AreaEffect
    {
    protected:
        /// @brief how much does the Gravity attenuate.
        Real AttenAmount;
        /// @brief The amount of force exerted on other objects
        Real Strength;
        /// @brief How does gravity
        Mezzanine::AttenuationStyle AttenStyle;
        /// @brief Should world gravity ne ignored
        Boolean AllowWorldGrav;
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        GravityWell(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        GravityWell(const String& Name, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        /// @param TheWorld A pointer to the world this object belongs to.
        GravityWell(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~GravityWell();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Mezzanine::WorldObject::GetType() const
        virtual Mezzanine::WorldObjectType GetType() const;

        /// @copydoc Mezzanine::WorldObject::ApplyEffect()
        virtual void ApplyEffect();

        ///////////////////////////////////////////////////////////////////////////////
        // GravityWell Properties

        /// @brief Sets the strenth of the field.
        /// @remarks The direction of the field is based on the current position of the object in the field.
        /// Once that direction is calculated it will be multiplied by this value to determine the force the field will apply to the object.
        /// @param FieldStrength The strength the field will have when exerting force onto other objects.
        virtual void SetFieldStrength(const Real FieldStrength);
        /// @brief Gets the strength of the field.
        /// @return Returns a Real representing the value that is being multiplied by the direction to determine force appied to objects.
        virtual Real GetFieldStrength() const;
        /// @brief Sets whether or not world gravity should be removed for objects in this field.
        /// @remarks Changing this value while the field is in the world and active is not recommended.
        /// @param WorldGravity If true, then forces exerted by this field will be added to the world gravity, otherwise
        /// world gravity for objects inside will be set to zero.
        virtual void SetAllowWorldGravity(Boolean WorldGravity);
        /// @brief Gets whether or not world gravity is is removed for objects inside this field.
        /// @return Returns a bool indicating whether objects inside are affected by world gravity.
        virtual Boolean GetAllowWorldGravity() const;

        /// @brief Sets the attenuation for this field.
        /// @param Amount The amount of force that is dropped off per 1 unit of distance objects are from the AE center.
        /// @param Style The style of attenuation to apply, see the AttenuationStyle enum for more details.
        virtual void SetAttenuation(const Real Amount, const Mezzanine::AttenuationStyle Style);
        /// @brief Sets the method of attenuation of force over a distance.
        /// @param Style The style of attenuation to apply, see the AttenuationStyle enum for more details.
        virtual void SetAttenuationStyle(const Mezzanine::AttenuationStyle Style);
        /// @brief Gets the Style of attenuation applied.
        /// @return Returns the style of attenuation currently being used by this field.
        virtual Mezzanine::AttenuationStyle GetAttenuationStyle() const;
        /// @brief Sets the amount of force that is attenuated over distance.
        /// @param Amount The amount of force that is dropped off per 1 unit of distance objects are from the AE source.
        virtual void SetAttenuationAmount(const Real Amount);
        /// @brief Gets the amount force is attenuated over distance.
        /// @return Returns a Real representing the amount of force that is dropped per 1 unit of distance (with linear attenuation).
        virtual Real GetAttenuationAmount() const;

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
    };//GravityWell

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of GravityWell objects.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB GravityWellFactory : public AreaEffectFactory
    {
    public:
        /// @brief Class constructor.
        GravityWellFactory();
        /// @brief Class destructor.
        virtual ~GravityWellFactory();

        /// @copydoc Mezzanine::WorldObjectFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a GravityWell object.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual GravityWell* CreateGravityWell(const String& Name, World* TheWorld);
        /// @brief Creates a GravityWell object.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual GravityWell* CreateGravityWell(const XML::Node& XMLNode, World* TheWorld);

        /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const String&, World*, const NameValuePairMap&)
        virtual AreaEffect* CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params);
        /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(XML::Node&)
        virtual AreaEffect* CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld);
        /// @copydoc Mezzanine::WorldObjectFactory::DestroyAreaEffect(AreaEffect*)
        virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
    };//GravityWellFactory
}//Mezzanine

#endif
