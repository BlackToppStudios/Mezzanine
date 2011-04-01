//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector3.h"

/// @file attachable.h
/// @brief Contains the phys::Attachable Class and phys::Attachable::AttachableElement enumeration declarations

namespace Ogre
{
    class SceneNode;
}

namespace phys
{
    //Forward Declarations
    class WorldNode;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Attachable
    /// @headerfile attachable.h
    /// @brief This is just a base class to be used by elements that are attachable to worldnodes.
    /// @details This class is useless on it's own and should not be created manually. These can
    /// be created by the phys::SceneManager.
    ///////////////////////////////////////
    class PHYS_LIB Attachable
    {
        public:
            friend class WorldNode;

            /// @brief A list of possible types of this that could be attached to a WorldNode
            enum AttachableElement
            {
                None            = 0, ///< This is no-type of object, only used in error conditions
                Camera          = 1, ///< This is a phys::Camera
                Light           = 2, ///< This is a phys::Light
                ParticleEffect  = 3, ///< This is a phys::ParticleEffect
                WorldNode       = 4  ///< This is a phys::WorldNode
            };
        protected:
            ///////////////////////////////////////////////////////////////////////////////
            /// Data Members

            /// @brief Enum value representing the type of element this is.
            Attachable::AttachableElement ElementType;

            /// @brief The WorldNode this is attached to or null if not attached.
            phys::WorldNode* AttachedTo;

            ///////////////////////////////////////////////////////////////////////////////
            /// Set data

            /// @brief This changes the WorldNode that this thinks it is attached to.
            /// @param Target A pointer to the WorldNode this should think it is attached to.
            virtual void SetAttachedTo(phys::WorldNode* Target);

        public:
            ///////////////////////////////////////////////////////////////////////////////
            /// Construction

            /// @brief No initialization class constructor.
            Attachable();

            /// @brief Create this already attached
            /// @param _AttachedToWorldNode The WorldNode that this is attached to.
            Attachable(phys::WorldNode* _AttachedToWorldNode);

            /// @brief Class destructor.
            virtual ~Attachable();

            ///////////////////////////////////////////////////////////////////////////////
            /// Attachment management

            /// @brief Gets the WorldNode this thinks it is attached to.
            /// @return Returns a pointer to the WorldNode this is attached to.
            phys::WorldNode* GetAttachedTo() const;

            /// @brief Attach this to a WorldNode
            /// @param Target the Target WorldNode
            /// @details By default this calls AttachObjectFinal() on the worldnode. This can an should
            /// be overridden if there are an special behaviors that need to be performed when
            /// this is attached to a WorldNode. This is the function a WorldNode will call
            /// to attach an object to itself. If overridden this should definitely be called
            /// or the basic functionality should be duplicated.
            virtual void AttachTo(phys::WorldNode* Target);

            /// @brief Detach from the WorldNode
            /// @details This is the function a WorldNode will call on destruction or when
            /// detaching objects from itself. By default this just calls DetachObjectFinal()
            /// on the World Node. If overridden this should definitely be called or the
            /// basic functionality should be duplicated. If any Special Behaviors need to
            /// be performed when detaching from a node this is function to overide.
            virtual void DetachFrom();

            /// @internal
            /// @brief Does the Final Work to attach on the Attachable Side, should only be called by WorldNode::AttachFinal
            /// @param RawTarget Any Raw data that need to be passed to finalize attaching
            /// @param Target a pointer to the Worldnode being attached to.
            /// @details This Sets the attaching pointer appropriately, This must be done if overiding this functioning. Any
            /// other specific items relating directly to attaching should happen here. In general other attaching dependent
            /// behaviors should not go here. This is called from WorldNode::AttachObjectFinal().
            virtual void AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target);

            /// @internal
            /// @brief Does the Final Work to dettach on the Atachable Side, should only be called by WorldNode::DetachFinal
            /// @details This Sets the attached pointer to 0, This must be done if overiding this functioning. Any
            /// other specific items relating directly to detaching should happen here. In general other detaching dependent
            /// behaviors should not go here. This is called from WorldNode::DetachObjectFinal().
            /// @param RawTarget Any Raw data that need to be passed to finalize detaching
            virtual void DetachFromFinal(Ogre::SceneNode* RawTarget);

            ///////////////////////////////////////////////////////////////////////////////
            /// Pure Virtual Functions

            /// @brief Gets the type of element this is.
            /// @return Returns an enum value indicating what type of element this is.
            virtual Attachable::AttachableElement GetAttachableType() const = 0;

            /// @brief Gets the name of the attachable element.
            virtual ConstString& GetName() const = 0;

            /// @brief Set the location of this atachable object relative to the object it is attached to.
            /// @param vec The location.
            virtual void SetLocation(const Vector3& Vec) = 0;

            /// @brief Get the location of the attachable object, relative to the Object is is attached to.
            /// @return A Vector3 with the location.
            virtual Vector3 GetLocation() const = 0;
    };
}//phys

#endif
