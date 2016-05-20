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
#ifndef _worldproxy_h
#define _worldproxy_h

#include "transformableobject.h"

namespace Mezzanine
{
    class WorldObject;
    class WorldProxyManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for proxy objects belonging to the various 3D subsystems.
    ///////////////////////////////////////
    class MEZZ_LIB WorldProxy : public TransformableObject
    {
    protected:
        /// @internal
        /// @brief Pointer to the Object this proxy belongs to.
        WorldObject* ParentObject;
        /// @internal
        /// @brief The unique ID assigned to the type of proxy an instance is.
        UInt32 ProxyID;

        /// @internal
        /// @brief Implementation method for serializing additional sets of data.
        /// @param SelfRoot The root node containing all the serialized data for this instance.
        virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
        /// @internal
        /// @brief Implementation method for deseriailizing additional sets of data.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
    public:
        /// @brief Blank constructor.
        WorldProxy();
        /// @brief Normal/ID constructor.
        /// @param ID The unique ID assigned to the type of proxy an instance is.
        WorldProxy(const UInt32 ID);
        /// @brief Class destructor.
        virtual ~WorldProxy();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Accessor for the type of proxy.
        /// @return Returns enum value for the type of proxy this object is.
        virtual Mezzanine::ProxyType GetProxyType() const = 0;
        /// @brief Performs all the necessary task to ensure this object is connected to it's respective world and ready for use.
        virtual void AddToWorld() = 0;
        /// @brief Unhooks this proxy from it's respective world.
        virtual void RemoveFromWorld() = 0;
        /// @brief Gets whether or not this object is inside of it's world.
        /// @return Returns true if this proxy is inserted in it's respective subsystems world.
        virtual Boole IsInWorld() const = 0;
        /// @brief Checks of the object is meant to have it's geometry/transform updated frequently.
        /// @return Returns true if the object is meant to be stationary, false otherwise.
        virtual Boole IsStatic() const = 0;

        /// @brief Gets a pointer to the parent object controlling this proxy.
        /// @return Returns a pointer to the WorldObject controlling this proxy, or NULL if this proxy isn't bound to a WorldObject.
        virtual WorldObject* GetParentObject() const;
        /// @brief Gets the unique ID of this proxy.
        /// @remarks Proxy IDs are only unique to their family of proxies belonging to a specific subsystem.  Across subsystems IDs can be reused.
        /// @return Returns a UInt32 containing the unique ID for this proxy.
        virtual UInt32 GetProxyID() const;
        /// @brief Gets a pointer to this proxies creator.
        /// @return Returns a pointer to the WorldProxyManager that created this WorldProxy.
        virtual WorldProxyManager* GetCreator() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        /// @copydoc TransformableObject::SetTransform(const Transform&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetTransform(const Transform& Trans) = 0;
        /// @copydoc TransformableObject::SetTransform(const Vector3&,const Quaternion&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetTransform(const Vector3& Loc, const Quaternion& Ori) = 0;
        /// @copydoc TransformableObject::GetTransform() const
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual Transform GetTransform() const = 0;

        /// @copydoc TransformableObject::SetLocation(const Vector3&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetLocation(const Vector3& Loc) = 0;
        /// @copydoc TransformableObject::SetLocation(const Real, const Real, const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetLocation(const Real X, const Real Y, const Real Z) = 0;
        /// @copydoc TransformableObject::GetLocation() const
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual Vector3 GetLocation() const = 0;
        /// @copydoc TransformableObject::SetOrientation(const Quaternion&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetOrientation(const Quaternion& Ori) = 0;
        /// @copydoc TransformableObject::SetOrientation(const Real, const Real, const Real, const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W) = 0;
        /// @copydoc TransformableObject::GetOrientation() const
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual Quaternion GetOrientation() const = 0;
        /// @copydoc TransformableObject::SetScale(const Vector3&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetScale(const Vector3& Sc) = 0;
        /// @copydoc TransformableObject::SetScale(const Real, const Real, const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void SetScale(const Real X, const Real Y, const Real Z) = 0;
        /// @copydoc TransformableObject::GetScale() const
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual Vector3 GetScale() const = 0;

        /// @copydoc TransformableObject::Translate(const Vector3&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Translate(const Vector3& Trans) = 0;
        /// @copydoc TransformableObject::Translate(const Real, const Real, const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Translate(const Real X, const Real Y, const Real Z) = 0;
        /// @copydoc TransformableObject::Yaw(const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Yaw(const Real Angle) = 0;
        /// @copydoc TransformableObject::Pitch(const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Pitch(const Real Angle) = 0;
        /// @copydoc TransformableObject::Roll(const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Roll(const Real Angle) = 0;
        /// @copydoc TransformableObject::Rotate(const Vector3&, const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Rotate(const Vector3& Axis, const Real Angle) = 0;
        /// @copydoc TransformableObject::Rotate(const Quaternion&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Rotate(const Quaternion& Rotation) = 0;
        /// @copydoc TransformableObject::Scale(const Vector3&)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Scale(const Vector3& Sc) = 0;
        /// @copydoc TransformableObject::Scale(const Real, const Real, const Real)
        /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
        virtual void Scale(const Real X, const Real Y, const Real Z) = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
        virtual void ProtoSerialize(XML::Node& ParentNode) const;
        /// @brief Convert the properties of this class to an XML::Node ready for serialization.
        /// @param SelfRoot The root node containing all the serialized data for this instance.
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
        /// @brief Take the data stored in an XML Node and overwrite the properties of this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

        /// @brief Gets the most derived serializable name of this WorldProxy.
        /// @note When creating a new WorldProxy class verify this method has a valid return for it in order for serialization to work properly.
        /// @return Returns the name of the XML tag from the most derived class of "this".
        virtual String GetDerivedSerializableName() const;
        /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        /// @internal
        /// @brief Binds this proxy to a WorldObject.
        /// @param NewParent A pointer to the WorldObject taking possession of this proxy.
        void _Bind(WorldObject* NewParent);
    };//WorldProxy
}//Mezzanine

#endif
