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

#include <vector>

namespace Ogre
{
    class Camera;
    class SceneNode;
    class SceneManager;
}

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CameraManager
    /// @headerfile CameraManager.h
    /// @brief This is the manager class for all camera functions.
    /// @details This class contains all the functionality of the use and manipulation of the camera.
    ///////////////////////////////////////////////////////////////////////////////
    class CameraManager {
        private:
            friend class World;
            Ogre::SceneManager* SceneManager;
            Ogre::Camera* DefaultCamera;
            std::vector< Ogre::Camera* > Cameras;
            std::vector< Ogre::SceneNode* > Nodes;
            unsigned short ONodes;
            unsigned short SNodes;
            Ogre::Camera* FindCamera(String Name);
            Ogre::SceneNode* FindNode(String Name);
        public:
            CameraManager(Ogre::SceneManager* SManager);
            virtual ~CameraManager();
            String CreateCamera();
            String CreateOrbitingNode(Vector3 Target, Vector3 RelativeLoc);
            String CreateStandNode(Vector3 LookAt, Vector3 Location);
            void ClearNodes();
            void ClearCameras();
            void SetLocation(Vector3 Location, String Name="DefaultCamera");
            void SetDirection(Vector3 Direction, String Name="DefaultCamera");
            void SetOrientation(Quaternion Orientation, String Name="DefaultCamera");
            void SetNearClipDistance(Real NearDist, String Name="DefaultCamera");
            void SetFarClipDistance(Real FarDist, String Name="DefaultCamera");
            void SetAspectRatio(Real Ratio, String Name="DefaultCamera");
            void LookAt(Vector3 TargetLoc, String Name="DefaultCamera");
            void SetFixedYawAxis(bool UseFixed, Vector3 Axis, String Name="DefaultCamera");
            void SetFixedYawAxis(bool UseFixed, String Name="DefaultCamera");
            void SetAutoTracking(bool Enabled, String Target, Vector3 Offset, String Name="DefaultCamera");
            void SetAutoTracking(bool Enabled, String Target, String Name="DefaultCamera");
            String GetNodeAttachedToCamera(String Name="DefaultCamera");
            void AttachCameraToNode(String NodeName, String CamName="DefaultCamera");
            void DetachCameraFromNode(String NodeName, String CamName="DefaultCamera");
            void IncrementYOrbit(Real Radian, String Name);
    };
}//phys
#endif
