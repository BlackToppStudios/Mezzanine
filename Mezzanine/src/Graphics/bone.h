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
#ifndef _graphicsbone_h
#define _graphicsbone_h

#include "transformableobject.h"

namespace Ogre
{
    class Bone;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Skeleton;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class encapsulates the bones contained in a skeleton used for animation.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Bone : public TransformableObject
        {
            protected:
                friend class Skeleton;
                /// @internal
                /// @brief The pointer to the internal Bone this is based on.
                Ogre::Bone* GraphicsBone;
                /// @internal
                /// @brief The host skeleton this bone belongs to.
                Skeleton* Host;
            //public:
                /// @internal
                /// @brief Internal constructor.
                /// @param HostSkel The parent skeleton this bone blongs to.
                /// @param InternalBone The internal bone this Bone is based on.
                Bone(Skeleton* HostSkel, Ogre::Bone* InternalBone);
                /// @brief Class destructor.
                ~Bone();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @brief Gets the host skeleton this bone belongs to.
                /// @return Returns a pointer to the parent skeleton.
                Skeleton* GetHost() const;
                /// @brief Gets the name of this Bone.
                /// @return Returns a const reference to a string containing the name of this bone.
                const String& GetName() const;
                /// @brief Gets the unique identifying number belonging to this bone.
                /// @return Returns a UInt16 representing the identifier unique to this bone within it's parent skeleton.
                UInt16 GetHandle() const;
                /// @brief Sets whether or not this bone is to be manually controlled.
                /// @param Manual True to allow this bone to be explicitly altered, false to make it only accept animation updates.
                void SetManuallyControlled(bool Manual);
                /// @brief Gets whether or not this bone is being manually controlled.
                /// @return Returns true if this bone can be directly manipulated, false otherwise.
                bool GetManuallyControlled() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Child Methods

                /// @brief Creates a new bone in the parent skeleton as a child of this bone.
                /// @param Handle The handle to be given to the bone being created.  Must be unique to the parent skeleton.
                /// @param Trans The translation to apply to the created bone.
                /// @param Rot The rotation to apply to the created bone.
                /// @return Returns a pointer to the created child bone.
                Bone* CreateChild(const UInt16 Handle, const Vector3& Trans, const Quaternion& Rot);
                /// @brief Gets the number of child bones contained by this bone.
                /// @return Returns a UInt16 containing the number of bones that are children of this bone.
                UInt16 GetNumChildren() const;
                /// @brief Gets a child bone by it's index.
                /// @param Index The index of the bone to retrieve.
                /// @return Returns a pointer to the requested Bone.
                Bone* GetChild(const UInt16 Index) const;
                /// @brief Gets a child bone by it's name.
                /// @param Name The name of the bone to retrieve.
                /// @return Returns a pointer to the requested Bone.
                Bone* GetChild(const String& Name) const;
                /// @brief Removes a bone from this bone.
                /// @note This does not remove the bone from the skeleton.
                /// @param ToBeRemoved The child bone to be removed from this bone.
                void RemoveChild(Bone* ToBeRemoved);
                /// @brief Removes a bone from this bone by index.
                /// @note This does not remove the bone from the skeleton.
                /// @param Index The index of the child bone to be removed from this bone.
                void RemoveChild(const UInt16 Index);
                /// @brief Removes a bone from this bone.
                /// @note This does not remove the bone from the skeleton.
                /// @param Name The name of the child bone to be removed from this bone.
                void RemoveChild(const String& Name);

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Methods

                /// @copydoc TransformableObject::SetLocation(const Vector3& Location)
                void SetLocation(const Vector3& Location);
                /// @copydoc TransformableObject::SetLocation(const Real X, const Real Y, const Real Z)
                void SetLocation(const Real X, const Real Y, const Real Z);
                /// @copydoc TransformableObject::GetLocation() const
                Vector3 GetLocation() const;
                /// @copydoc TransformableObject::SetOrientation(const Quaternion& Orientation)
                void SetOrientation(const Quaternion& Orientation);
                /// @copydoc TransformableObject::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
                void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
                /// @copydoc TransformableObject::GetOrientation() const
                Quaternion GetOrientation() const;
                /// @copydoc TransformableObject::SetScale(const Vector3& Scale)
                void SetScale(const Vector3& Scale);
                /// @copydoc TransformableObject::SetScale(const Real X, const Real Y, const Real Z)
                void SetScale(const Real X, const Real Y, const Real Z);
                /// @copydoc TransformableObject::GetScale() const
                Vector3 GetScale() const;

                /// @copydoc TransformableObject::Translate(const Vector3& Trans, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Translate(const Vector3& Trans, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Translate(const Real X, const Real Y, const Real Z, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Translate(const Real X, const Real Y, const Real Z, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Yaw(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Yaw(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Pitch(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Pitch(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Roll(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Roll(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Rotate(const Vector3& Axis, const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Rotate(const Vector3& Axis, const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Rotate(const Quaternion& Rotation, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent)
                void Rotate(const Quaternion& Rotation, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent);
                /// @copydoc TransformableObject::Scale(const Vector3& Scale)
                void Scale(const Vector3& Scale);
                /// @copydoc TransformableObject::Scale(const Real X, const Real Y, const Real Z)
                void Scale(const Real X, const Real Y, const Real Z);

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Gets the internal bone pointer.
                /// @return Returns a shared pointer pointing to the internal bone.
                Ogre::Bone* _GetInternalBone() const;
        };//Bone
    }//Graphics
}//Mezzanine

#endif

