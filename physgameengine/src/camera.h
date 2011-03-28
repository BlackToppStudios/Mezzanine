//© Copyright 2010 BlackTopp Studios Inc.
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
/// @brief Declaration of the phys::Camera class

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML

/// @brief Serializes the passed phys::Camera to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::Camera to be serialized
/// @return this returns the ostream, now with the serialized data
/// @warning This does not attempt to store the camera aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not s the pointer to the camera manager. When a camera manager is serialized, this data is implicitly stored by the cameras location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::Camera& Ev);

/// @brief Deserialize a phys::Camera
/// @param stream The istream to get the xml from to (re)make the phys::Camera.
/// @param Ev the phys::Camera to be deserialized.
/// @return this returns the ostream, advanced past the phys::Camera that was recreated onto Ev.
/// @warning This does not attempt to store the camera aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not s the pointer to the camera manager. When a camera manager is serialized, this data is implicitly stored by the cameras location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Camera& Ev);

/// @brief Set all values of a phys::Camera from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::Camera.
/// @param Ev the phys::Camera to be reset.
/// @return This returns thexml::Node that was passed in.
/// @warning This does not attempt to de-serialize the name of the camera. This is not currently changeable after the creation of a camera. However, the cameramanager will correctly create name camera upon creation then deserialize the rest of the camera.
/// @warning This does not throw an exception if the camera could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::Camera& Ev);

#endif // \PHYSXML

namespace phys
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
    class PHYS_LIB Camera : public Attachable
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
            friend xml::Node& PHYS_LIB operator >> (xml::Node& OneNode, Camera& Ev);


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
            void SetCameraType(ProjectionType Type);

            /// @brief Get the type of projection used by the camera
            /// @return A ProjectionType that will identify the kind of projection this camera uses.
            Camera::ProjectionType GetCameraType() const;

            /// @brief Defines the size of the Orthographic projection window.
            /// @details This function will change the aspect ratio of the screen, determined by the values passed in.  To set the
            /// window size without changing the aspect ratio, call either the SetOrthoWindowHeight, or SetOrthoWindowWidth functions.
            /// @param Width The new width of the projection window.
            /// @param Height The new height of the projection window.
            void SetOrthoWindow(Real Width, Real Height);
            /// @brief Defines the size of the Orthographic projection window.
            /// @details This function will not change the aspect ratio of the screen, unlike SetOrthoWindow.  The aspect ratio will be
            /// preserved and the Width of the screen automatically recalculated based on the Height passed in.
            /// @param Height The new height of the projection window.
            void SetOrthoWindowHeight(Real Height);
            /// @brief Defines the size of the Orthographic projection window.
            /// @details This function will not change the aspect ratio of the screen, unlike SetOrthoWindow.  The aspect ratio will be
            /// preserved and the Height of the screen automatically recalculated based on the Width passed in.
            /// @param Width The new width of the projection window.
            void SetOrthoWindowWidth(Real Width);

            /// @brief Sets the location of a camera.
            /// @details Sets the location of the specified camera.
            /// @param Location The new location for the camera.
            virtual void SetLocation(const Vector3& Location);
            /// @brief Sets the Direction for the camera.
            /// @details Sets which axis the camera will look down for rendering.
            /// @param Direction The vector3 representing the axis to be used.
            void SetDirection(const Vector3& Direction);
            /// @brief Sets the orientation of the camera.
            /// @details This function will set the orientation of the specified camera via a quaternion.
            /// @param Orientation The quaternion representing the new orientation.
            void SetOrientation(const Quaternion& Orientation);
            /// @brief Sets the short range clip distance.
            /// @details Sets the distance at which objects are considered too close to render.
            /// @param NearDist A Real representing the distance.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the physgame.  You should understand your games scale before setting
            /// this number.

            void SetNearClipDistance(Real NearDist);
            /// @brief Sets the long range clip distance.
            /// @details Sets the distance at which objects are considered too far to render.
            /// @param FarDist A Real representing the distance.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the physgame.  You should understand your games scale before setting
            /// this number.
            void SetFarClipDistance(Real FarDist);
            /// @brief Sets the aspect ratio of the cameras veiw.
            /// @details This function will set the aspect ratio between the width and height of the cameras viewing area.
            /// @param Ratio A Real that represents the aspect ratio, where Ratio = width / height.
            void SetAspectRatio(Real Ratio);
            /// @brief Sets the direction the camera faces.
            /// @details Sets the direction the camera faces.  Will also take orientation into account.
            /// @param TargetLoc The location in the game world to look at.
            void LookAt(Vector3 TargetLoc);

            /// @brief Sets whether to lock rotation around the Y axis.
            /// @details This function will lock rotations around the Y axis (or another axis if you specify).  This
            /// function is automatically called on by the camera constructor.
            /// @param UseFixed Enable or disable the locking of the axis.
            /// @param Axis The axis to lock, defaults to the Y axis.
            void SetFixedYawAxis(bool UseFixed, Vector3 Axis);
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
            /// @param Screenx A Real representing the relative location on screen, on the x axis(0.0-1.0).
            /// @param Screeny A Real representing the relative location on screen, on the y axis(0.0-1.0).
            Ray GetCameraToViewportRay(Real Screenx, Real Screeny) const;

            /*/// @brief Gets the WorldNode the camera is attached to if any.
            /// @details This will return a pointer to the phys::WorldNode that this camera is attached to if any. If none, then 0 is returned.
            WorldNode* GetWorldNode() const;*/


            /// @brief Gets the relative location of the camera.
            /// @details Gets the location of the camera, relative to any parent WorldNode.
            /// @return A phys::Vector3 with the location of the camera as though the Parent WorldNode were the origin.
            virtual Vector3 GetLocation() const;
            /// @brief Gets the relative location of the camera.
            /// @details Gets the location of the camera, relative to any parent WorldNode.
            /// @return A phys::Vector3 with the location of the camera as though the Parent WorldNode were the origin.
            Vector3 GetRelativeLocation() const;
            /// @brief Gets the global/absolute location of the camera.
            /// @return A phys::Vector3 containing the location of this object as an offset from the global origin.
            Vector3 GetGlobalLocation() const;
            /// @brief Gets the direction the camera is facing.
            /// @return A phys::Quaternion representing how the camera is rotated.
            Quaternion GetOrientation() const;

            /// @brief Will zoom in or out the camera.
            /// @details This function will zoom in the camera by the amount specified.
            /// @param Zoom A Real of how much to zoom in by.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the physgame.  You should understand your games scale before setting
            /// this number.
            void ZoomCamera(Real Zoom);

            /// @brief Resets the zoom level back to the default.
            /// @details This function will return the zoom level back to normal.  Note this function will only work if
            /// the camera is attached to a node.
            void ResetZoom();

            /// @internal
            /// @brief Gets the internal camera this camera is based on.
            /// @return Returns a pointer to the Ogre Camera this camera is based on.
            Ogre::Camera* GetOgreCamera() const;

            ///////////////////////////////////////////////////////////////////////////////
            /// Inherited From Attachable

            /// @brief What kind of Attachable is this.
            /// @return An Attachable::GetAttachableType containing Attachable::Camera.
            virtual Attachable::AttachableElement GetAttachableType() const;

            virtual void AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target);

            virtual void DetachFromFinal(Ogre::SceneNode* RawTarget);
    };
}//phys

#endif
