//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _CameraManager_h
#define _CameraManager_h

#include "vector3.h"
#include "datatypes.h"
#include "quaternion.h"
#include "ray.h"
#include "managerbase.h"

#include <vector>

namespace Ogre
{
    class Camera;
    class SceneNode;
    class SceneManager;
}

namespace phys
{
    class Camera;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CameraManager
    /// @headerfile cameramanager.h
    /// @brief This is the manager class for all camera functions.
    /// @details This class contains all the functionality of the use and manipulation of the camera. @n
    /// All functions that manipulate the camera will default to the default camera, so if you only use
    /// one camera you should never have to name the camera you want to use.
    ///////////////////////////////////////////////////////////////////////////////
    class CameraManager : public ManagerBase {
        private:
            friend class World;
            friend class Camera;
            Ogre::SceneManager* SceneManager;
            Ogre::Camera* DefaultCamera;
            std::vector< Ogre::Camera* > Cameras;
            std::vector< Ogre::SceneNode* > Nodes;
            unsigned short ONodes;
            unsigned short SNodes;
            Ogre::Camera* FindCamera(String Name);
            Ogre::SceneNode* FindNode(String Name);
        public:
            /// @brief Class Constructor.
            /// @details This is the class constructor.  This is automatcally called in the World.CreateRenderWindow()
            /// function and should never need to be called manually.
            /// @param SManager A pointer to the Scenemanager where you will be creating/manipulating all the cameras.
            CameraManager(Ogre::SceneManager* SManager);
            /// @brief Class Destructor.
            /// @details The calss Destuctor
            virtual ~CameraManager();
            /// @brief Creates a camera.
            /// @details This function will create a camera in the scene and return the string that is name of the
            /// camera created.  The first camera created will always be the Default camera, and is also created just
            /// after this class is constructed in the World.CreateRenderWindow() function.
            String CreateCamera();
            /// @brief Creates a camera and returns a pointer.
            /// @details This function does the same as the other CreateCamera function but will also return a pointer to
            /// the camera class instead of a string(being the name of the camera).
            Camera* CreateCameraPtr();
            /// @brief Creates a node that will orbit around a point.
            /// @details This will create 2 nodes in the scene, the first being the point in the world you want to orbit
            /// the second node around.  The second being the node that does the orbiting.  You can then attach a camera
            /// to the orbiting node for some interesting visuals of your scene.
            /// @param Target The location of the first node which you will be orbiting around.
            /// @param RelativeLoc The location of the node that will be in orbit relative to the first node.  Assume the
            /// first node is at Origin (0,0,0).
            String CreateOrbitingNode(Vector3 Target, Vector3 RelativeLoc);
            /// @brief Creates a stationary node that will look at a location.
            /// @details This will create a node that doesn't move, and will look at one location that you specify.  This
            /// node can then have cameras attached to it.
            /// @param LookAt The location you want the node to look at.  Automatically handles orientation.
            /// @param Location The location of the node itself.
            String CreateStandNode(Vector3 LookAt, Vector3 Location);
            /// @brief Deletes all nodes created by this manager.
            /// @details This will clear the container of nodes.  Including Orbiting nodes, Center nodes, and Stand nodes.
            void ClearNodes();
            /// @brief Deletes all cameras except for the first camera.
            /// @details This will clear the container of cameras.  The default camera is not stored in this container
            /// however, so it is spared from this wipe.
            void ClearCameras();
            /// @brief Sets the location of a camera.
            /// @details Sets the location of the specified camera.
            /// @param Location The new location for the camera.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetLocation(Vector3 Location, String Name="DefaultCamera");
            /// @brief Sets the Direction for the camera.
            /// @details Sets which axis the camera will look down for rendering.
            /// @param Direction The vector3 representing the axis to be used.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetDirection(Vector3 Direction, String Name="DefaultCamera");
            /// @brief Sets the orientation of the camera.
            /// @details This function will set the orientation of the specified camera via a quaternion.
            /// @param Orientation The quaternion representing the new orientation.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetOrientation(Quaternion Orientation, String Name="DefaultCamera");
            /// @brief Sets the short range clip distance.
            /// @details Sets the distance at which objects are considered too close to render.
            /// @param NearDist A Real representing the distance.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the physgame.  You should understand your games scale before setting
            /// this number.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetNearClipDistance(Real NearDist, String Name="DefaultCamera");
            /// @brief Sets the long range clip distance.
            /// @details Sets the distance at which objects are considered too far to render.
            /// @param FarDist A Real representing the distance.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the physgame.  You should understand your games scale before setting
            /// this number.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetFarClipDistance(Real FarDist, String Name="DefaultCamera");
            /// @brief Sets the aspect ratio of the cameras veiw.
            /// @details This function will set the aspect ratio between the width and height of the cameras viewing area.
            /// @param Ratio A Real that represents the aspect ratio, where Ratio = width / height.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetAspectRatio(Real Ratio, String Name="DefaultCamera");
            /// @brief Sets the direction the camera faces.
            /// @details Sets the direction the camera faces.  Will also take orientation into account.
            /// @param TargetLoc The location in the game world to look at.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void LookAt(Vector3 TargetLoc, String Name="DefaultCamera");
            /// @brief Sets whether to lock rotation around the Y axis.
            /// @details This function will lock rotations around the Y axis (or another axis if you specify).  This
            /// function is automatically called on by the camera constructor.
            /// @param UseFixed Enable or disable the locking of the axis.
            /// @param Axis The axis to lock, defaults to the Y axis.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetFixedYawAxis(bool UseFixed, Vector3 Axis, String Name="DefaultCamera");
            /// @brief Sets whether to lock rotation around the Y axis.
            /// @details This function will lock rotations around the Y axis.  This function is automatically called
            /// on by the camera constructor.
            /// @param UseFixed Enable or disable the locking of the axis.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetFixedYawAxis(bool UseFixed, String Name="DefaultCamera");
            /// @brief Enables or disables auto tracking for the camera.
            /// @details This function can enable auto tracking of a given node you have created.
            /// @param Enabled Bool value to enable or disable auto tracking for this camera.
            /// @param Target Name of the node to be tracked.
            /// @param Offset The offset of where the camera is to look from the target.  I.E. Always 5 units ahead, etc..
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetAutoTracking(bool Enabled, String Target, Vector3 Offset, String Name="DefaultCamera");
            /// @brief Enables or disables auto tracking for the camera.
            /// @details This function can enable auto tracking of a given node you have created.
            /// @param Enabled Bool value to enable or disable auto tracking for this camera.
            /// @param Target Name of the node to be tracked.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void SetAutoTracking(bool Enabled, String Target, String Name="DefaultCamera");
            /// @brief Gets a Ray from the camera to the viewport.
            /// @details This will cast a ray from the camera to the viewport and return it.
            /// @param Screenx A Real representing the relative location on screen, on the x axis(0.0-1.0).
            /// @param Screeny A Real representing the relative location on screen, on the y axis(0.0-1.0).
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            Ray GetCameraToViewportRay(Real Screenx, Real Screeny, String Name="DefaultCamera");
            /// @brief Gets the node attached to a camera.
            /// @details This will return a string that is the name of the node the specified camera is attached to if any.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            String GetNodeAttachedToCamera(String Name="DefaultCamera");
            /// @brief Gets the location of a node.
            /// @details Gets the location of the node specified.
            /// @param Name Name of the node to get the location of.
            Vector3 GetNodeLocation(String Name);
            /// @brief Gets the relative location of a camera.
            /// @details Gets the location of the camera, relative to any connected nodes, specified.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            Vector3 GetCameraRelativeLocation(String Name="DefaultCamera");
            /// @brief Gets the global location of a camera.
            /// @details Gets the real world location of the camera specified.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            Vector3 GetCameraGlobalLocation(String Name="DefaultCamera");
            /// @brief Will zoom in or out the camera.
            /// @details This function will zoom in the camera by the amount specified.
            /// @param Zoom A Real of how much to zoom in by.  Note:  This number directly corolates to the dimentions
            /// you provide in the constructor for the physgame.  You should understand your games scale before setting
            /// this number.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void ZoomCamera(Real Zoom, String Name="DefaultCamera");
            /// @brief Resets the zoom level back to the default.
            /// @details This function will return the zoom level back to normal.  Note this function will only work if the camera is attached to a node.
            /// @param Name The name of the camera to be manipulated.  Defaults to the Default camera.
            void ResetZoom(String Name="DefaultCamera");
            /// @brief Attaches a camera to a node.
            /// @details Attaches the specified camera to the specified node.
            /// @param NodeName The name of the node to attach the camera to.
            /// @param CamName The name of the camera to be manipulated.  Defaults to the Default camera.
            void AttachCameraToNode(String NodeName, String CamName="DefaultCamera");
            /// @brief Detaches a camera from a node.
            /// @details Detaches the specified camera from the specified node.
            /// @param NodeName The name of the node to detach the camera from.
            /// @param CamName The name of the camera to be manipulated.  Defaults to the Default camera.
            void DetachCameraFromNode(String NodeName, String CamName="DefaultCamera");
            /// @brief Increments the orbit by the amount specified.
            /// @details Increments the orbit of the specified node by the amount specified.
            /// @param Radian The amount you wish to increment the orbit in Radians.
            /// @param Name The name of the orbiting node you wish to increment the orbit for.
            void IncrementYOrbit(Real Radian, String Name);

            //Inherited From ManagerBase
            /// @brief Empty Initializor
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason
            virtual void Initialize();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::CameraManager
            virtual ManagerTypeName GetType() const;

    };
}//phys
#endif
