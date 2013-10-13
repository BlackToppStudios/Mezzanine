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
#ifndef _attachable_h
#define _attachable_h

#include "transform.h"

/// @file attachable.h
/// @brief Contains the Mezzanine::Attachable Class and Mezzanine::Attachable::AttachableElement enumeration declarations

class btCollisionObject;

namespace Ogre
{
    class SceneNode;
    class MovableObject;
}

namespace Mezzanine
{
    //Forward Declarations
    class WorldNode;
    class AttachableBase;
    class AttachableParent;
    class AttachableChild;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class AttachableBase
    /// @headerfile attachable.h
    /// @brief This class is the base class for other attachable classes and is responsible for transform updates to attachables.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AttachableBase
    {
        protected:
            bool Updating;
            /// @internal
            /// @brief Gets the update status of another attachable.
            bool GetUpdating(AttachableBase* AB) const;
        public:
            /// @brief Class constructor.
            AttachableBase();
            /// @brief Class destructor.
            virtual ~AttachableBase();

            ///////////////////////////////////////////////////////////////////////////////
            // Conversion Functions

            /// @brief Converts a point in local space to the same point in global space.
            /// @param Location The point in local space to be converted.
            /// @return Returns a Vector3 representing the point in global space corresponding to the provided local space point.
            Vector3 ConvertLocalToGlobal(const Vector3& Location) const;
            /// @brief Converts a point in global space to the same point in local space.
            /// @param Location The point in global space to be converted.
            /// @return Returns a Vector3 representing the point in local space corresponding to the provided global space point.
            Vector3 ConvertGlobalToLocal(const Vector3& Location) const;
            /// @brief Converts a rotation in local space to the same rotation in global space.
            /// @param Orientation The rotation in local space to be converted.
            /// @return Returns a Quaternion representing the rotation in global space corresponding to the provided local space rotation.
            Quaternion ConvertLocalToGlobal(const Quaternion& Orientation) const;
            /// @brief Converts a rotation in global space to the same rotation in local space.
            /// @param Orientation The rotation in global space to be converted.
            /// @return Returns a Quaternion representing the rotation in local space corresponding to the provided global space rotation.
            Quaternion ConvertGlobalToLocal(const Quaternion& Orientation) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Functions
            /// @brief Gets the name of this attachable.
            /// @return Returns a const reference to a string containing the name of this attachable.
            virtual ConstString& GetName() const = 0;
            /// @brief Gets the type of World or Scene object this attachable is.
            /// @return Returns the type of World or Scene object this attachable is.
            virtual WorldObjectType GetType() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Functions

            /// @brief Sets the Location of this object.
            /// @param Location A vector3 representing the location of this object.
            virtual void SetLocation(const Vector3& Location) = 0;
            /// @brief Gets the Location of this object.
            /// @return Returns a vector3 representing the location of this object.
            virtual Vector3 GetLocation() const = 0;
            /// @brief Sets the orientation of this object.
            /// @param Orientation A Quaternion representing the orientation of this object.
            virtual void SetOrientation(const Quaternion& Orientation) = 0;
            /// @brief Gets the orientation of this object.
            /// @return Returns a quaternion representing the orientation of this object.
            virtual Quaternion GetOrientation() const = 0;
            /// @brief Sets the scale of this object.
            /// @param Scale A vector3 representing the scale to be applied to this object.
            virtual void SetScaling(const Vector3& Scale) = 0;
            /// @brief Gets the scale of this object.
            /// @return Returns a vector3 representing the scale being applied to this object.
            virtual Vector3 GetScaling() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
    };//attachablebase

    ///////////////////////////////////////////////////////////////////////////////
    /// @class AttachableParent
    /// @headerfile attachable.h
    /// @brief Base class for objects that can have attachables attached to them.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AttachableParent : virtual public AttachableBase
    {
        public:
            typedef std::vector<AttachableChild*> AttachableContainer;
            typedef AttachableContainer::iterator AttachableIterator;
            typedef AttachableContainer::const_iterator ConstAttachableIterator;
        protected:
            AttachableContainer Attached;
        public:
            #ifndef SWIG
            /// @brief Class constructor.
            AttachableParent();
            #endif

            /// @brief Class destructor.
            virtual ~AttachableParent();

            ///////////////////////////////////////////////////////////////////////////////
            // Attachment child management

            /// @brief Attaches an attachable element to this object.
            /// @param Target The Attachable to be attached.
            virtual void AttachObject(AttachableChild* Target);
            /// @brief Detaches an attachable element from this object.
            /// @param Target The Attachable to be detached.
            /// @details Detach an item is done in linear time relative to the amount of attached items.
            virtual void DetachObject(AttachableChild* Target);
            /// @brief Detaches all attachables currently attached.
            virtual void DetachAllChildren();
            /// @brief Gets the number of elements attached to this object.
            /// @return Returns the number of elements attached to this object.
            virtual Whole GetNumAttached() const;
            /// @brief Get a specific attached Item
            /// @param Index A number indicating which Attachable you want a pointer to. The WorldNode is like an Array starts at 0 and goes to WorldNode::GetNumAttached() - 1.
            /// @return A pointer to an Attachable Item attached to this.
            /// @throw This can throw an out of bounds std::exception if used incorrectly.
            virtual AttachableChild* GetAttached(const Whole& Index) const;
            /// @brief Get an AttachableIterator to the first object.
            /// @return An Iterator to the first object.
            AttachableIterator BeginChild();
            /// @brief Get an AttachableIterator to one past the last object.
            /// @return An Iterator to one past the last object.
            AttachableIterator EndChild();

#if !(defined(SWIG) && defined(MEZZLUA51)) // Stop Swig from making lua bindings but allow other languages
            /// @brief Get a ConstAttachableIterator to the first object.
            /// @return An Iterator to the first object.
            ConstAttachableIterator BeginChild() const;
            /// @brief Get a ConstAttachableIterator to one past the last object.
            /// @return An Iterator to one past the last object.
            ConstAttachableIterator EndChild() const;
#endif

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Recalculates the transforms of all children of this attachable.
            void _RecalculateAllChildTransforms();
    };//attachableparent

    ///////////////////////////////////////////////////////////////////////////////
    /// @class AttachableChild
    /// @headerfile attachable.h
    /// @brief This class is the base class for objects that can be attached to AttachableParent.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB AttachableChild : virtual public AttachableBase
    {
        protected:
            friend class AttachableParent;
            AttachableParent* Parent;
            bool LocalTransformDirty;
            bool GlobalTransformDirty;
            Transform LocalXform;
        public:
            /// @brief Class constructor.
            AttachableChild();
            /// @brief Class destructor.
            virtual ~AttachableChild();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Functions

            /// @brief Gets the parent of this child.
            /// @return Returns a pointer to the parent this object is attached to or NULL if it's not attached to anything.
            AttachableParent* GetParent() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Functions

            /// @brief Sets the Location of this object in local space.
            /// @param Location A vector3 representing the location of this object.
            virtual void SetLocalLocation(const Vector3& Location) = 0;
            /// @brief Gets the Location of this object in local space.
            /// @return Returns a vector3 representing the location of this object.
            Vector3 GetLocalLocation() const;
            /// @brief Sets the orientation of this object in local space.
            /// @param Orientation A Quaternion representing the orientation of this object.
            virtual void SetLocalOrientation(const Quaternion& Orientation) = 0;
            /// @brief Gets the orientation of this object in local space.
            /// @return Returns a quaternion representing the orientation of this object.
            Quaternion GetLocalOrientation() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Recalculates objects global transform from parent.
            /// @param FromParent Whether this is being invoked from the parent of this attachable.
            void _RecalculateGlobalTransform(bool FromParent = false);
            /// @brief Recalculates this objects local transform based on it's current global position.
            void _RecalculateLocalTransform();
    };//attachablechild
}//Mezzanine

#endif
