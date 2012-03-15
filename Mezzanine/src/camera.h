//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _Camera_h
#define _Camera_h

#include "attachable.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "quaternion.h"
#include "ray.h"
#include "vector3.h"
#include "worldnode.h"

namespace Ogre
{
    class Camera;
}

/// @file camera.h
/// @brief Declaration of the Mezzanine::Camera class

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::Camera to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::Camera to be serialized
/// @return this returns the ostream, now with the serialized data
/// @warning This does not attempt to store the camera aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not s the pointer to the camera manager. When a camera manager is serialized, this data is implicitly stored by the cameras location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Camera& Ev);

/// @brief Deserialize a Mezzanine::Camera
/// @param stream The istream to get the xml from to (re)make the Mezzanine::Camera.
/// @param Ev the Mezzanine::Camera to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::Camera that was recreated onto Ev.
/// @warning This does not attempt to store the camera aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not s the pointer to the camera manager. When a camera manager is serialized, this data is implicitly stored by the cameras location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Camera& Ev);

/// @brief Set all values of a Mezzanine::Camera from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::Camera.
/// @param Ev the Mezzanine::Camera to be reset.
/// @return This returns thexml::Node that was passed in.
/// @warning This does not attempt to de-serialize the name of the camera. This is not currently changeable after the creation of a camera. However, the cameramanager will correctly create name camera upon creation then deserialize the rest of the camera.
/// @warning This does not throw an exception if the camera could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
Mezzanine::xml::Node& MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::Camera& Ev);

#endif // \MEZZXML

namespace Mezzanine
{
    class CameraManager;
    class World;

    ///////////////////////////////////////////////////////////////////////////////
    /// @class Camera
    /// @headerfile camera.h
    /// @brief This is the camera class.
    /// @details This class contains all the functionality needed to manipulate an
    /// individual camera that has been created.
    /// @todo Fix all the extra occurences of the word Camera in Function names on the camera.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB Camera : public AttachableChild
    {
        public:
            /// @brief Values for storing how perspective should be interpretted
            enum ProjectionType
            {
                Orthographic    = 0,        ///< Not at all, objects at any distance are the same size.
                Perspective     = 1         ///< Normal perspective.
            };

        private:
            /// @internal
            friend xml::Node& MEZZ_LIB operator >> (xml::Node& OneNode, Camera& Ev);

            /// @internal
            /// @brief This is called by the called by the constructors, it is a single point of class initialization.
            /// @param Camera A pointer the graphics subsystem camera
            /// @param Manager The camera manager this will be attached to.
            void Construct(Ogre::Camera* Camera, CameraManager* Manager);

            /// @internal
            /// @brief Stores if yawing should be allowed.
            bool YawOnAxis;
            /// @internal
            /// @brief The axis that yawing may be disallowed on.
            Vector3 YawAxis;

        protected:
            friend class CameraManager;

            /// @internal
            /// @brief This is the Camera used by the graphics Subsystem, that this class wraps
            Ogre::Camera* Cam;

            /// @internal
            /// @brief This is the Camera manager that this camera is attached to.
            CameraManager* CamManager;

        public:
            /// @brief Basic Camera Constructor.
            /// @details This is the basic constructor for the Camera class.
            Camera(const String& Name, CameraManager* Manager);
            /// @brief Ogre Cam Constructor.
            /// @details This is for internal use only and shouldn't be called manually.
            Camera(Ogre::Camera* Camera, CameraManager* Manager);

            /// @brief Class Destructor.
            /// @details The Class Destructor.
            virtual ~Camera();

            /// @brief Gets the camera's set name.
            /// @return Returns a string containing the camera's name.
            ConstString& GetName() const;

            /// @brief Sets the type of projection to be used with this camera.
            /// @details By default, all cameras are enabled with Perspective projection.  This is the standard 3-dimentional
            /// view anyone would expect in a 3D world.  Orthographic projection is useful when displaying 2D worlds, or only
            /// 2 dimentions of a 3D world.  It enables you to see the entire side of an object without regard for camera perspective.
            /// Perspective can be thought of as a pyramid, with the camera at the top of the cone.  Orthographic would instead be a
            /// cube.
            /// @param Type The type of projection to be used.
            void SetCameraType(const ProjectionType& Type);
            /// @brief Get the type of projection used by the camera
            /// @return A ProjectionType that will identify the kind of projection this camera uses.
            Camera::ProjectionType GetCameraType() const;

            /// @brief Defines the size of the Orthographic projection window.
            /// @details This function will change the aspect ratio of the screen, determined by the values passed in.  To set the
            /// window size without changing the aspect ratio, call either the SetOrthoWindowHeight, or SetOrthoWindowWidth functions.
            /// @param Width The new width of the projection window.
            /// @param Height The new height of the projection window.
            void SetOrthoWindow(const Real& Width, const Real& Height);
            /// @brief Defines the size of the Orthographic projection window.
            /// @details This function will not change the aspect ratio of the screen, unlike SetOrthoWindow.  The aspect ratio will be
            /// preserved and the Width of the screen automatically recalculated based on the Height passed in.
            /// @param Height The new height of the projection window.
            void SetOrthoWindowHeight(const Real& Height);
            /// @brief Defines the size of the Orthographic projection window.
            /// @details This function will not change the aspect ratio of the screen, unlike SetOrthoWindow.  The aspect ratio will be
            /// preserved and the Height of the screen automatically recalculated based on the Width passed in.
            /// @param Width The new width of the projection window.
            void SetOrthoWindowWidth(const Real& Width);

            /// @brief Sets the location of a camera.
            /// @details Sets the location of the specified camera.
            /// @param Location The new location for the camera.
            void SetLocation(const Vector3& Location);
            /// @brief Gets the relative location of the camera.
            /// @details Gets the location of the camera, relative to any parent WorldNode.
            /// @return A Mezzanine::Vector3 with the location of the camera as though the Parent WorldNode were the origin.
            Vector3 GetLocation() const;

            /// @brief Sets the orientation of the camera.
            /// @details This function will set the orientation of the specified camera via a quaternion.
            /// @param Orientation The quaternion representing the new orientation.
            void SetOrientation(const Quaternion& Orientation);
            /// @brief Gets the direction the camera is facing.
            /// @return A Mezzanine::Quaternion representing how the camera is rotated.
            Quaternion GetOrientation() const;

            /// @brief Sets the Direction for the camera.
            /// @details Sets which axis the camera will look down for rendering.
            /// @param Direction The vector3 representing the axis to be used.
            void SetDirection(const Vector3& Direction);
            /// @brief Gets the direction the camera is currently facing.
            /// @return Returns a Vector3 representing the current direction the camera is facing.
            Vector3 GetDirection() const;

            /// @brief Sets the short range clip distance.
            /// @details Sets the distance at which objects are considered too close to render.
            /// @param NearDist A Real representing the distance.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the World.  You should understand your games scale before setting
            /// this number.
            void SetNearClipDistance(const Real& NearDist);

            /// @brief Sets the long range clip distance.
            /// @details Sets the distance at which objects are considered too far to render.
            /// @param FarDist A Real representing the distance.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the World.  You should understand your games scale before setting
            /// this number.
            void SetFarClipDistance(const Real& FarDist);
            /// @brief Sets the aspect ratio of the cameras veiw.
            /// @details This function will set the aspect ratio between the width and height of the cameras viewing area.
            /// @param Ratio A Real that represents the aspect ratio, where Ratio = width / height.
            void SetAspectRatio(const Real& Ratio);
            /// @brief Sets the direction the camera faces.
            /// @details Sets the direction the camera faces.  Will also take orientation into account.
            /// @param TargetLoc The location in the game world to look at.
            void LookAt(const Vector3& TargetLoc);
            /// @brief Moves the camera without factoring orientation.
            /// @details This function will move the camera along the provided vector based on world axes.
            /// @param ToMove The vector to move the camera by.
            void Move(const Vector3& ToMove);
            /// @brief Moves the camera while factoring orientation.
            /// @details This function will move the camera along the provided vector based on local axes.
            /// @param ToMove The vector to move the camera by.
            void MoveRelative(const Vector3& ToMove);

            /// @brief Sets whether to lock rotation around the Y axis.
            /// @details This function will lock rotations around the Y axis (or another axis if you specify).  This
            /// function is automatically called on by the camera constructor.
            /// @param UseFixed Enable or disable the locking of the axis.
            /// @param Axis The axis to lock, defaults to the Y axis.
            void SetFixedYawAxis(bool UseFixed, const Vector3& Axis);
            /// @brief Sets whether to lock rotation around the Y axis.
            /// @details This function will lock rotations around the Y axis.  This function is automatically called
            /// on by the camera constructor to enable camera yawing.
            /// @param UseFixed Enable or disable the locking of the axis.
            void SetFixedYawAxis(bool UseFixed);
            /// @brief Is fixed yaw enabled.
            /// @return True if it is enable, such as the default setting, or false if it is not enabled.
            bool IsFixedYawEnabled() const;
            /// @brief If fixed yaw is enabled, on which axis is yawing disabled.
            /// @return Returns a Vector3 of 0s if disable, otherwise this return the Fixed Yaw Axis.
            Vector3 GetFixedYawAxis() const;

            // @brief Enables or disables auto tracking for the camera.
            // @details This function can enable auto tracking of a given node you have created.
            // @param Enabled Bool value to enable or disable auto tracking for this camera.
            // @param Target Name of the node to be tracked.
            // @param Offset The offset of where the camera is to look from the target.  I.E. Always 5 units ahead, etc..
            //void SetAutoTracking(bool Enabled, String Target, Vector3 Offset);
            // @brief Enables or disables auto tracking for the camera.
            // @details This function can enable auto tracking of a given node you have created.
            // @param Enabled Bool value to enable or disable auto tracking for this camera.
            // @param Target Name of the node to be tracked.
            //void SetAutoTracking(bool Enabled, String Target);

            /// @brief Gets a Ray from the camera to the viewport.
            /// @details This will cast a ray from the camera to the viewport and return it.
            /// @param ScreenX A Real representing the relative location on screen, on the x axis(0.0-1.0).
            /// @param ScreenY A Real representing the relative location on screen, on the y axis(0.0-1.0).
            Ray GetCameraToViewportRay(const Real& ScreenX, const Real& ScreenY) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited From AttachableChild

            /// @copydoc AttachableBase::GetType()
            WorldAndSceneObjectType GetType() const;
            /// @copydoc WorldObject::SetScaling(Vector3&)
            void SetScaling(const Vector3& Scale);
            /// @copydoc WorldObject::GetScaling()
            Vector3 GetScaling() const;
            /// @copydoc AttachableChild::SetLocalLocation(Vector3&)
            void SetLocalLocation(const Vector3& Location);
            /// @copydoc AttachableChild::SetLocalOrientation(Quaternion&)
            void SetLocalOrientation(const Quaternion& Orientation);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal camera this camera is based on.
            /// @return Returns a pointer to the Ogre Camera this camera is based on.
            Ogre::Camera* GetOgreCamera() const;
    };//Camera
}//Mezzanine

#endif
