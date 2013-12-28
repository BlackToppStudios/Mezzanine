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
#ifndef _graphicscameraproxy_h
#define _graphicscameraproxy_h

/// @file
/// @brief This file contains the declaration for the World proxy wrapping camera functionality.

#include "Graphics/renderableproxy.h"
#include "Graphics/graphicsenumerations.h"
#include "colourvalue.h"
#include "ray.h"

namespace Ogre
{
    class Camera;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Viewport;
        class CameraManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy class for placing and manipulating a camera in the scene.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB CameraProxy : public RenderableProxy
        {
        protected:
            friend class Viewport;

            /// @internal
            /// @brief A string containing the forward facing name of this proxy.
            String CamName;
            /// @internal
            /// @brief A vector3 representing the fixed axis on which this camera will always Yaw/rotate.
            Vector3 FixedYawAxis;
            /// @internal
            /// @brief A pointer to the CameraManager that created this camera.
            CameraManager* CamManager;
            /// @internal
            /// @brief A pointer to the internal Camera this proxy is based on.
            Ogre::Camera* GraphicsCamera;
            /// @internal
            /// @brief This is a pointer to the Viewport this camera is attached to, if any.
            Viewport* CameraVP;
            /// @internal
            /// @brief A bool storing whether or not the FixedYawAxis is being used.
            Boolean UseFixedYaw;

            /// @internal
            /// @brief Creates an internal Camera to be used by the calling instance.
            /// @param Name The name to be given to this CameraProxy.
            virtual void CreateCamera(const String& Name);
            /// @internal
            /// @brief Destroys the internal Camera in use by this proxy.
            virtual void DestroyCamera();
            /// @internal
            /// @brief Generates a name for this Camera to placate the internal system.
            /// @note This will no longer be necessary in after the switch to Ogre 2.0.
            /// @return Returns a string containing the auto-generated name of this Camera.
            static String GenerateName();
        public:
            /// @brief Class constructor.
            /// @param Name The name to be given to this CameraProxy.
            /// @param Creator A pointer to the manager that created this proxy.
            CameraProxy(const String& Name, CameraManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            /// @param Creator A pointer to the manager that created this proxy.
            CameraProxy(const XML::Node& SelfRoot, CameraManager* Creator);
            /// @brief Class destructor.
            virtual ~CameraProxy();

            /// @todo Wrap LOD related methods (Camera/Bias).
            /// @todo Wrap reflection methods?

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldProxy::GetProxyType() const
            virtual Mezzanine::ProxyType GetProxyType() const;
            /// @copydoc WorldProxy::GetCreator() const
            virtual WorldManager* GetCreator() const;
            /// @brief Gets the Viewport this camera is attached to, if any.
            /// @return Returns a pointer to the Viewport this camera is rendering to, or NULL if not attached.
            virtual Viewport* GetViewport() const;

            /// @brief Gets whether this cameras Yaw rotation is based on a fixed axis.
            /// @return Returns true if a fixed Yaw axis is enabled and in use, false otherwise.
            virtual Boolean IsFixedYawEnabled() const;

            /// @brief Sets the direction the camera faces based on a 3D point.
            /// @param TargetLoc The location in the world to look at.
            virtual void LookAt(const Vector3& TargetLoc);
            /// @brief Sets the direction the light will be emitted from this source.
            /// @note The direction is not used if this is a point light.
            /// @param Dir A Vector3 representing the direction the light will travel from this source.
            virtual void SetDirection(const Vector3& Dir);
            /// @brief Gets the direction the light from this source is being emitted.
            /// @note The direction is not used if this is a point light.
            /// @return Returns a Vector3 representing the direction this proxy is emitting light.
            virtual Vector3 GetDirection() const;

            /// @brief Gets a Ray from the camera to the viewport.
            /// @param ScreenX A Real representing the relative location on screen, on the x axis(0.0 - 1.0).
            /// @param ScreenY A Real representing the relative location on screen, on the y axis(0.0 - 1.0).
            virtual Ray GetCameraToViewportRay(const Real ScreenX, const Real ScreenY) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Camera Properties

            /// @brief Gets the name of this Camera.
            /// @return Returns a const reference to a string containing the cameras name.
            virtual const String& GetName() const;

            /// @brief Sets the debug rendering mode for this camera.
            /// @param PolyMode A CameraPolyMode enum value expressing the mode to be set.
            virtual void SetPolygonMode(const Graphics::CameraPolyMode PolyMode);
            /// @brief Gets the debug rendering mode for this camera.
            /// @return Returns a CameraPolyMode enum value representing the current mode of this camera.
            virtual Graphics::CameraPolyMode GetPolygonMode() const;
            /// @brief Sets the type of projection to be used with this camera.
            /// @remarks By default, all cameras are enabled with Perspective projection.  This is the standard 3-dimentional
            /// view anyone would expect in a 3D world.  Orthographic projection is useful when displaying 2D worlds, or only
            /// 2 dimentions of a 3D world.  It enables you to see the entire side of an object without regard for camera perspective.
            /// Perspective can be thought of as a pyramid, with the camera at the top of the cone.  Orthographic would instead be a cube.
            /// @param ProjType A ProjectionType enum value that expresses the type of projection to be used.
            virtual void SetProjectionType(const Graphics::ProjectionType ProjType);
            /// @brief Get the type of projection used by the camera.
            /// @return A ProjectionType enum value that will identify the kind of projection this camera uses.
            virtual Graphics::ProjectionType GetProjectionType() const;
            /// @brief Sets the orientation mode of this cameras frustrum.
            /// @param OriMode An OrientationMode enum value that expresses the type of orientation this cameras frustrum will use.
            virtual void SetOrientationMode(const Graphics::OrientationMode OriMode);
            /// @brief Gets the orientation mode of this cameras frustrum.
            /// @return An OrientationMode enum value that will identify the kind of orientation this cameras frustrum uses.
            virtual Graphics::OrientationMode GetOrientationMode() const;

            /// @brief Defines the size of the Orthographic projection window in world units.
            /// @remarks This function can change the aspect ratio of the screen, determined by the values passed in.  To set the
            /// window size without changing the aspect ratio, call either the SetOrthoWindowHeight, or SetOrthoWindowWidth functions.
            /// @param Width The new width of the projection window.
            /// @param Height The new height of the projection window.
            virtual void SetOrthoWindow(const Real Width, const Real Height);
            /// @brief Defines the size of the Orthographic projection window in world units.
            /// @remarks This function will not change the aspect ratio of the screen, unlike SetOrthoWindow.  The aspect ratio will be
            /// preserved and the Height of the screen automatically recalculated based on the Width passed in.
            /// @param Width The new width of the projection window.
            virtual void SetOrthoWindowWidth(const Real Width);
            /// @brief Gets the current width of the Orthographic projection window in world units.
            /// @return Returns a Real represnting the world unit width of the projection window when Orthographic projection is enabled.
            virtual Real GetOrthoWindowWidth() const;
            /// @brief Defines the size of the Orthographic projection window in world units.
            /// @remarks This function will not change the aspect ratio of the screen, unlike SetOrthoWindow.  The aspect ratio will be
            /// preserved and the Width of the screen automatically recalculated based on the Height passed in.
            /// @param Height The new height of the projection window.
            virtual void SetOrthoWindowHeight(const Real Height);
            /// @brief Gets the current height of the Orthographic projection window in world units.
            /// @return Returns a Real represnting the world unit height of the projection window when Orthographic projection is enabled.
            virtual Real GetOrthoWindowHeight() const;

            /// @brief Sets the distance in world units at which objects are considered too close to render.
            /// @param NearDist A Real representing the distance.
            virtual void SetNearClipDistance(const Real NearDist);
            /// @brief Gets the distance in world units at which objects are considered too close to render.
            /// @return Returns a Real representing the distance from the camera to the near clipping plane.
            virtual Real GetNearClipDistance() const;
            /// @brief Sets the distance in world units at which objects are considered too far to render.
            /// @param FarDist A Real representing the distance.
            virtual void SetFarClipDistance(const Real FarDist);
            /// @brief Gets the distance in world units at which objects are considered too far to render.
            /// @return Returns a Real representing the distance from the camera to the far clipping plane.
            virtual Real GetFarClipDistance() const;

            /// @brief Sets the vertical field of view of the camera frustrum.
            /// @remarks Higher values (90+ degrees, 1.57079633+ radians) result in wide-angle, fish-eye views.  Low values (30- degrees, 0.523598776- radians)
            /// result in telescopic views.  Normal values are between 45 degrees(0.785398163 radians) and 60 degrees(1.04719755 radians).
            /// @param FOV The field of view in radians.
            virtual void SetFieldOfViewY(const Real FOV);
            /// @brief Gets the vertical field of view of the camera frustrum.
            /// @return Returns a Real representing the current vertical field of view of this camera in radians.
            virtual Real GetFieldOfViewY() const;

            /// @brief Sets the aspect ratio of the cameras veiw.
            /// @param Ratio A Real that represents the camera frustrum viewport aspect ratio, where Ratio = width / height.
            virtual void SetAspectRatio(const Real Ratio);
            /// @brief Gets the aspect ratio of the cameras veiw.
            /// @return Returns a Real representing the aspect ratio for the camera frustrum viewport.
            virtual Real GetAspectRatio() const;

            /// @brief Sets whether or not to lock rotation around the Y axis.
            /// @remarks This function is automatically called on by the camera constructor.
            /// @param UseFixed Enable or disable the locking of the axis.
            /// @param Axis The axis to lock, defaults to the Y axis.
            virtual void SetFixedYawAxis(const Boolean UseFixed, const Vector3& Axis = Vector3::Unit_Y());
            /// @brief If fixed yaw is enabled, on which axis is yawing disabled.
            /// @return Returns a Vector3 of 0s if disable, otherwise this return the Fixed Yaw Axis.
            virtual Vector3 GetFixedYawAxis() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc WorldProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal camera.
            /// @return Returns a pointer to the internal camera this proxy is based on.
            virtual Ogre::Camera* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const;
        };// ©ameraProxy
    }//Graphics
}//Mezzanine

#endif
