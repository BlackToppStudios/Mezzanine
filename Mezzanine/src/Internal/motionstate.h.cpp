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
#ifndef _internalmotionstate_h_cpp
#define _internalmotionstate_h_cpp

// Keeps this file form being documented by doxygen
/// @cond DontDocumentInternal

#include "btBulletDynamicsCommon.h"

#include "vector3.h"
#include "quaternion.h"

namespace Ogre
{
    class SceneNode;
}

namespace Mezzanine
{
    class TransformableObject;
    namespace Physics
    {
        class RigidProxy;
    }
    namespace Internal
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @class SimpleMotionState
        /// @headerfile motionstate.h.cpp
        /// @brief This class is used by the RigidProxy class to sync between the physics world and other subsystems.
        /// @details This class provides the link for position and orientation between all the worlds in the engine.
        /// This is called on every step(frame) of the world to sync the TransformableObject if it has moved.
        /// @attention This filename ends in .h.cpp which means it is a header and should not be compiled with the regular cpp files, just included by them, but it is also a
        /// source file and should not shipped with the DLL when the SDK is released. This is used for engine internals that need to be used by multiple classes.
        ///////////////////////////////////////
        class SimpleMotionState : public btMotionState
        {
        protected:
            /// @internal
            /// @brief Transform cache.  May remove in the future.
            btTransform WorldTrans;
            /// @internal
            /// @brief The calling rigid proxy that will be used to sync other objects' transforms to.
            Physics::RigidProxy* ParentObject;
            /// @internal
            /// @brief The other proxy being sync'd to the RigidProxy transform.
            TransformableObject* SyncObject;
        public:
            /// @brief Blank constructor.
            SimpleMotionState();
            /// @brief Class constructor.
            /// @param PO A pointer to the parent physics object.
            /// @param TO A pointer to the TransformableObject that will be sync'd with the physics object transform.
            SimpleMotionState(Physics::RigidProxy* PO, TransformableObject* TO);
            /// @brief Class destructor.
            virtual ~SimpleMotionState();

            /// @brief Sets the parent object to be updated.
            /// @param PO A pointer to the parent physics object.
            void SetParentObject(Physics::RigidProxy* PO);
            /// @brief Sets the proxy to be sync'd.
            /// @param TO A pointer to the TransformableObject that will be sync'd with the physics object transform.
            void SetSyncObject(TransformableObject* TO);

            /// @brief Sets the initial position.
            /// @note This information is only grabbed when the object is added to the world.
            /// @param Position The vector3 representing the location to be used.
            void SetPosition(const Vector3& Position);
            /// @brief Sets the initial orientation.
            /// @note This information is only grabbed when the object is added to the world.
            /// @param Orientation The vector3 representing the orientation to be used.
            void SetOrientation(const Quaternion& Orientation);

            /// @brief Sets the initial position.
            /// @note This function is called on by the physics world upon adding the Proxy to the world.
            /// @param worldTrans The location and orientation data.
            virtual void getWorldTransform(btTransform& worldTrans) const;
            /// @brief Updates the position and orientation.
            /// @remarks This function is called each step(frame) by the physics world to sync the physics and graphical worlds.
            /// @param worldTrans The location and orientation data.
            virtual void setWorldTransform(const btTransform& worldTrans);
        };//SimpleMotionState

        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @class MultiMotionState
        /// @headerfile motionstate.h.cpp
        /// @brief This class functions very similarly to the SimgpleMotionState, but it allows the syncing of multiple objects instead of just one.
        /// @attention This filename ends in .h.cpp which means it is a header and should not be compiled with the regular cpp files, just included by them, but it is also a
        /// source file and should not shipped with the DLL when the SDK is released. This is used for engine internals that need to be used by multiple classes.
        ///////////////////////////////////////
        class MultiMotionState : public btMotionState
        {
        public:
            /// @brief Basic container type for TransformableObject storage by this class.
            typedef std::vector< TransformableObject* >             TransformableObjectContainer;
            /// @brief Iterator type for TransformableObject instances stored by this class.
            typedef TransformableObjectContainer::iterator          TransformableObjectIterator;
            /// @brief Const Iterator type for TransformableObject instances stored by this class.
            typedef TransformableObjectContainer::const_iterator    ConstTransformableObjectIterator;
        protected:
            /// @internal
            /// @brief Transform cache.  May remove in the future.
            btTransform WorldTrans;
            /// @internal
            /// @brief A container of proxies being sync'd to the RigidProxy transform.
            TransformableObjectContainer SyncObjects;
            /// @internal
            /// @brief The calling rigid proxy that will be used to sync other objects' transforms to.
            Physics::RigidProxy* ParentObject;
        public:
            /// @brief Blank constructor.
            MultiMotionState();
            /// @brief Class constructor.
            /// @param PO A pointer to the parent physics object.
            MultiMotionState(Physics::RigidProxy* PO);
            /// @brief Class destructor.
            virtual ~MultiMotionState();

            /// @brief Sets the parent object to be updated.
            /// @param PO A pointer to the parent physics object.
            void SetParentObject(Physics::RigidProxy* PO);

            /// @brief Adds a TransformableObject to this motionstate, which will force it's transform to sync with the parent RigidProxy.
            /// @param ToBeAdded A pointer to the WorldObject being added.
            void AddSyncObject(TransformableObject* ToBeAdded);
            /// @brief Gets a TransformableObject being sync'd by this motionstate by it's index.
            /// @param Index The index of the sync object to retrieve.
            /// @return Returns a pointer to the TransformableObject at the specified Index.
            TransformableObject* GetSyncObject(const UInt32 Index) const;
            /// @brief Gets the number of WorldProxies being sync'd by this motionstate.
            /// @return Returns a UInt32 representing the number of WorldProxies being sync'd with the parent RigidProxy.
            UInt32 GetNumSyncObjects() const;
            /// @brief Removes a proxy from this motionstate, so it will no longer be sync'd with the parent RigidProxy.
            /// @param ToBeRemoved A pointer to the TransformableObject to be removed.
            void RemoveSyncObject(TransformableObject* ToBeRemoved);
            /// @brief Removes all WorldProxies being sync'd from this motionstate.
            void RemoveAllSyncObjects();

            /// @brief Sets the initial position.
            /// @note This information is only grabbed when the object is added to the world.
            /// @param Position The vector3 representing the location to be used.
            void SetPosition(const Vector3& Position);
            /// @brief Sets the initial orientation.
            /// @note This information is only grabbed when the object is added to the world.
            /// @param Orientation The vector3 representing the orientation to be used.
            void SetOrientation(const Quaternion& Orientation);

            /// @brief Sets the initial position.
            /// @note This function is called on by the physics world upon adding the Proxy to the world.
            /// @param worldTrans The location and orientation data.
            virtual void getWorldTransform(btTransform& worldTrans) const;
            /// @brief Updates the position and orientation.
            /// @remarks This function is called each step(frame) by the physics world to sync the physics and graphical worlds.
            /// @param worldTrans The location and orientation data.
            virtual void setWorldTransform(const btTransform& worldTrans);
        };//MultiMotionState
    }//Internal
}//Mezzanine

/// @endcond

#endif

