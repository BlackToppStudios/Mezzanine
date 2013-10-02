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
#ifndef _worldnode_h
#define _worldnode_h

#include "attachable.h"
#include "enumerations.h"

#ifndef SWIG
    #include "XML/xml.h"
#endif

/// @file worldnode.h
/// @brief The declaration of the WorldNode, A class to facilitate navigation in 3d environment

namespace Ogre
{
    class SceneNode;
}

namespace Mezzanine
{
    class ActorBase;
    namespace Graphics
    {
        class SceneManager;
        class Camera;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldNode
    /// @headerfile worldnode.h
    /// @brief This is a helper class which non-physics objects, such as lights, cameras, ribbon trails, and
    /// particle effects may be attached to for enhanced effects.
    /// @details This class simply contains a group of functions for easy manipulation of movement and orientation
    /// as well as attaching elements to them.  This class should only be created through the scene manager. @n
    /// There are 4 types of nodes: @n
    /// Center - Works in conjunction with the orbit node and serves as it's center point for the orbit node to
    /// rotate around. @n
    /// Orbit - Counterpart to the Center node, this type does the orbiting. @n
    /// Stand - Stationary node that points in one direction, that is it. @n
    /// Free - A node with no rules governing how it moves. @n
    /// Note:  There are no rules restricting the use of nodes anyway, this enum is here simply to help
    /// indicate the intended use upon creation.
    ///////////////////////////////////////
    class MEZZ_LIB WorldNode : public AttachableParent, public AttachableChild
    {
        protected:
            AttachableBase* AutoTrackTarget;
            Graphics::SceneManager* Manager;
            bool FixedYaw;
            Vector3 FixedYawAxis;
            Vector3 AutoTrackOffset;
            Vector3 AutoTrackDirection;
            Vector3 NodeLoc;
            Vector3 NodeScale;
            Quaternion NodeOri;
            String NodeName;
        public:
            /// @brief Standard initialization constructor.
            /// @param Name The name of this node.
            /// @param SManager Pointer to the manager that this node is to be used in.
            WorldNode(const String& Name, Graphics::SceneManager* SManager);
            /// @brief Class destructor.
            virtual ~WorldNode();

            ///////////////////////////////////////////////////////////////////////////////
            // Basic Data

            /// @brief Gets the name of this node.
            /// @return Returns a string containing the name given to this node.
            ConstString& GetName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Navigation

            /// @brief Automatically sets the orientation needed to point this node at a location in the world.
            /// @param LookAt The location in the specified transform space to point at.
            /// @param TS The transform space to use for the position to look at.
            /// @param LocalDirection The local direction that will be aligned with the position being looked at.
            void LookAt(const Vector3& LookAt, const Mezzanine::TransformSpace& TS = Mezzanine::TS_World, const Vector3& LocalDirection = Vector3::Neg_Unit_Z());
            /// @brief Sets the direction this node is facing.
            /// @remarks At first glace it may seem odd why this function accepts two different direction vectors as arguements.  This function
            /// really just generates a rotation and applies it.  The Direction vector is where you want the LocalAxis(which can be any local axis)
            /// to be when computing the rotation.  So LocalAxis is the base, and Direction is the target.
            /// @param Direction The desired end direction in the specified transform space.
            /// @param TS The transform space to use for the direction specified.
            /// @param LocalAxis The axis in local space to use for lining up with the direction specified.
            void SetDirection(const Vector3& Direction, const Mezzanine::TransformSpace& TS = Mezzanine::TS_World, const Vector3& LocalAxis = Vector3::Neg_Unit_Z());
            /// @brief Makes this node constantly face another object.
            /// @details This function will make this node update it's orientation every frame automatically so
            /// that it's always facing the target object.
            /// @param Target The object to be tracked or NULL if you want to disable autotracking.
            /// @param LocalDirection The local axis that is to be facing the other object.
            /// @param Offset A vector3 representing the offset from the objects location to be tracked.
            /// @todo TODO: The WorldNode Does not Correctly Serialize AutoTracking.
            void SetAutoTracking(AttachableBase* Target, const Vector3& LocalDirection = Vector3::Neg_Unit_Z(), const Vector3& Offset = Vector3());
            /// @brief Gets the target currently being tracked.
            /// @return Returns a pointer to the target being currently tracked or NULL if none are being tracked.
            AttachableBase* GetAutoTrackingTarget() const;
            /// @brief Gets the local direction used to face at the autotracked target.
            /// @return Returns a const reference to a vector3 representing the facing direction of the autotracked target.
            const Vector3& GetAutoTrackingDirection() const;
            /// @brief Gets the offset from the target applied to the autotracked target.
            /// @return Returns a const reference to a vector3 representing the offset from the autotracked target being used.
            const Vector3& GetAutoTrackingOffset() const;
            /// @brief Applies a rotation from a quaternion.
            /// @remarks This function differs from SetOrientation() in that it won't override the previous rotation, but instead will merge this rotation with the existing rotation.
            /// @param Rotation The rotation to be applied.
            /// @param TS The transform space the rotation is to be applied in.
            void Rotate(const Quaternion& Rotation, const Mezzanine::TransformSpace& TS = Mezzanine::TS_Local);
            /// @brief Applies a rotation from an angle and an axis.
            /// @param Angle The angle to be applied in radians.
            /// @param Axis The axis on which the angle is to be applied.
            /// @param TS The transform space the rotation is to be applied in.
            void Rotate(const Real& Angle, const Vector3& Axis, const Mezzanine::TransformSpace& TS = Mezzanine::TS_Local);
            /// @brief Applies a rotation around the Y axis.
            /// @param Angle The angle in radians to rotate.
            /// @param TS The transform space the rotation is to be applied in.
            void Yaw(const Real& Angle, const Mezzanine::TransformSpace& TS = Mezzanine::TS_Local);
            /// @brief Applies a rotation around the X axis.
            /// @param Angle The angle in radians to rotate.
            /// @param TS The transform space the rotation is to be applied in.
            void Pitch(const Real& Angle, const Mezzanine::TransformSpace& TS = Mezzanine::TS_Local);
            /// @brief Applies a rotation around the Z axis.
            /// @param Angle The angle in radians to rotate.
            /// @param TS The transform space the rotation is to be applied in.
            void Roll(const Real& Angle, const Mezzanine::TransformSpace& TS = Mezzanine::TS_Local);
            /// @brief Tells this node to always yaw around a specific axis.  Useful for AutoTracking.
            /// @remarks It's important to note that this will only be corrected in methods that do not explictly set the orientation, such as with autotracking or use
            /// of the LookAt() function, or the Rotate() function.  Explicitly setting the orientation with SetOrientation() or SetLocalOrientation() will not be corrected.
            /// @param Enable Whether to enable to disable the use of a fixed yaw axis.
            /// @param FixedAxis The axis on which yawing will be fixed for any rotations applied.
            void SetFixedYawAxis(bool Enable, const Vector3& FixedAxis = Vector3::Unit_Y());
            /// @brief Gets whether or not Fixed Yaw is enabled.
            /// @return Returns true if Fixed Yaw is enabled, false otherwise.
            bool GetFixedYawAxisEnabled() const;
            /// @brief Gets the Axis used to yaw.
            /// @return Returns a const reference to a vector3 representing the Fixed Axis used.
            const Vector3& GetFixedYawAxis() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from Attachable classes

            /// @copydoc AttachableBase::SetLocation(Vector3&)
            void SetLocation(const Vector3& Location);
            /// @copydoc AttachableBase::GetLocation()
            Vector3 GetLocation() const;
            /// @copydoc AttachableBase::SetOrientation(Quaternion&)
            void SetOrientation(const Quaternion& Orientation);
            /// @copydoc AttachableBase::GetOrientation()
            Quaternion GetOrientation() const;
            /// @copydoc AttachableBase::SetScaling(Vector3&)
            void SetScaling(const Vector3& Scale);
            /// @copydoc AttachableBase::GetScaling()
            Vector3 GetScaling() const;
            /// @copydoc AttachableBase::GetType()
            WorldAndSceneObjectType GetType() const;
            /// @copydoc AttachableChild::SetLocalLocation(Vector3&)
            void SetLocalLocation(const Vector3& Location);
            /// @copydoc AttachableChild::SetLocalOrientation(Quaternion&)
            void SetLocalOrientation(const Quaternion& Orientation);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @brief Updates this object to continue tracking another object.
            void _UpdateTracking();
    };//node
}//Mezzanine



///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

#ifndef SWIG
/// @brief Serializes the passed Mezzanine::WorldNode to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::WorldNode to be serialized
/// @return this returns the ostream, now with the serialized data
/// @warning This does not added to the WorldNode manager. When a WorldNode manager is serialized, this data is implicitly stored by the WorldNodes location in the xml hierarchy, this is used instead. The Name of the Node is stored for possible future use.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::WorldNode& Ev);

/// @brief Deserialize a Mezzanine::WorldNode
/// @param stream The istream to get the xml from to (re)make the Mezzanine::WorldNode.
/// @param Ev the Mezzanine::WorldNode to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::WorldNode that was recreated onto Ev.
/// @warning This does not attempt to store the WorldNode aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not the pointer to the WorldNode manager. When a WorldNode manager is serialized, this data is implicitly stored by the WorldNodes location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::WorldNode& Ev);

/// @brief Set all values of a Mezzanine::WorldNode from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::WorldNode.
/// @param Ev the Mezzanine::WorldNode to be reset.
/// @return This returns theXML::Node that was passed in.
/// @warning This does not attempt to de-serialize the name of the WorldNode. This is not currently changeable after the creation of a WorldNode. However, the WorldNodemanager will correctly create name WorldNode upon creation then deserialize the rest of the WorldNode.
/// @warning This does not throw an exception if the WorldNode could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::WorldNode& Ev);
#endif // \SWIG


#endif
