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
#ifndef _fieldofforce_h
#define _fieldofforce_h

#include "areaeffect.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is field that applies force in a direction, and doesn't tamper with gravity.
    /// @details This class is similiar to a gravity well in that it can attenuate, but different in
    /// that the direction is constant, the source of force(for calculating attenuation) can be outside the
    /// field itself, and the direction is constant.  @n
    /// Placing the source of attenuation inside the field will cause the object to accelerate as it gets close
    /// to the source, and then will be applied less force(but in the same direction) as it moves from the source.
    /// This behavior makes this class good for creating a booster-like AE.
    ///////////////////////////////////////
    class MEZZ_LIB FieldOfForce : public AreaEffect
    {
    protected:
        /// @internal
        /// @brief The user defined source if enabled.
        Vector3 AttenSource;
        /// @internal
        /// @brief The direction the force is applied.
        Vector3 Direction;
        /// @internal
        /// @brief How much the Gravity weakens over distance.
        Real AttenAmount;
        /// @internal
        /// @brief The amount of force exerted on other objects
        Real Strength;
        /// @internal
        /// @brief How gravity weakens over distance, if at all.
        Mezzanine::AttenuationStyle AttenStyle;
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        FieldOfForce(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        FieldOfForce(const String& Name, World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param Shape A pointer to the Collision shape that will be applied to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        FieldOfForce(const String& Name, Physics::CollisionShape* Shape, World* TheWorld);
        /// @brief XML constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        /// @param TheWorld A pointer to the world this object belongs to.
        FieldOfForce(const XML::Node& SelfRoot, World* TheWorld);
        /// @brief Class destructor.
        virtual ~FieldOfForce();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @copydoc Mezzanine::WorldObject::GetType() const
        virtual Mezzanine::WorldObjectType GetType() const;

        /// @copydoc Mezzanine::WorldObject::ApplyEffect()
        virtual void ApplyEffect();

        ///////////////////////////////////////////////////////////////////////////////
        // FieldOfForce Properties

        /// @brief Sets the strenth of the field.
        /// @param FieldStrength The strength the field will have when exerting force onto other objects.
        virtual void SetFieldStrength(const Real FieldStrength);
        /// @brief Gets the strength of the field.
        /// @return Returns a Real representing the value that is being multiplied by the direction to determine force appied to objects.
        virtual Real GetFieldStrength() const;
        /// @brief Sets the direction force is to be applied within this field.
        /// @param ForceDirection A vector3 representing the direction force is to be applied.
        virtual void SetDirectionOfForce(const Vector3& ForceDirection);
        /// @brief Gets the currenly set direction force is to be applied.
        /// @return Returns a vector3 representing the direction of force in this field.
        virtual Vector3 GetDirectionOfForce() const;

        /// @brief Sets the attenuation for this field.
        /// @param Amount The amount of force that is dropped off per 1 unit of distance objects are from the AE source.
        /// @param Style The style of attenuation to apply, see the AttenuationStyle enum for more details.
        /// @param Source A vector3 representing the source of force to use when calculating attenuation.
        virtual void SetAttenuation(const Real Amount, const Mezzanine::AttenuationStyle Style, const Vector3& Source);
        /// @brief Sets the method of attenuation of force over a distance.
        /// @param Style The style of attenuation to apply, see the AttenuationStyle enum for more details.
        virtual void SetAttenuationStyle(const Mezzanine::AttenuationStyle Style);
        /// @brief Gets the method of attenuation of force over a distance.
        /// @return Returns the style of attenuation currently being used by this field.
        virtual Mezzanine::AttenuationStyle GetAttenuationStyle() const;
        /// @brief Sets the amount of force that is attenuated over distance.
        /// @param Amount The amount of force that is dropped off per 1 unit of distance (with linear attenuation).
        virtual void SetAttenuationAmount(const Real Amount);
        /// @brief Gets the amount of force that is attenuated over distance.
        /// @return Returns a Real representing the amount of force that is dropped per 1 unit of distance (with linear attenuation).
        virtual Real GetAttenuationAmount() const;
        /// @brief Sets the source of the force for calculating attenuation.
        /// @param Source A vector3 representing the source of force to use when calculating attenuation.
        virtual void SetAttenuationSource(const Vector3& Source);
        /// @brief Gets the source of the force for calculating attenuation.
        /// @return Returns a Vector3 representing the source of the attenuating force.
        virtual Vector3 GetAttenuationSource() const;

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
    };//FieldOfForce

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A factory type for the creation of FieldOfForce objects.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB FieldOfForceFactory : public AreaEffectFactory
    {
    public:
        /// @brief Class constructor.
        FieldOfForceFactory();
        /// @brief Class destructor.
        virtual ~FieldOfForceFactory();

        /// @copydoc Mezzanine::WorldObjectFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a FieldOfForce object.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual FieldOfForce* CreateFieldOfForce(const String& Name, World* TheWorld);
        /// @brief Creates a FieldOfForce object.
        /// @param Name The name to be given to this object.
        /// @param AEShape A pointer to the Collision shape that will be applied to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual FieldOfForce* CreateFieldOfForce(const String& Name, Physics::CollisionShape* AEShape, World* TheWorld);
        /// @brief Creates a FieldOfForce object.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual FieldOfForce* CreateFieldOfForce(const XML::Node& XMLNode, World* TheWorld);

        /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const String&, World*, const NameValuePairMap&)
        virtual AreaEffect* CreateAreaEffect(const String& Name, World* TheWorld, const NameValuePairMap& Params);
        /// @copydoc Mezzanine::WorldObjectFactory::CreateAreaEffect(const XML::Node&)
        virtual AreaEffect* CreateAreaEffect(const XML::Node& XMLNode, World* TheWorld);
        /// @copydoc Mezzanine::WorldObjectFactory::DestroyAreaEffect(AreaEffect*)
        virtual void DestroyAreaEffect(AreaEffect* ToBeDestroyed);
    };//FieldOfForceFactory
}//Mezzanine

#endif
