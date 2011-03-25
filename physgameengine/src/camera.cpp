
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
#ifndef _Camera_cpp
#define _Camera_cpp

#include "camera.h"
#include "cameramanager.h"
#include "scenemanager.h"
#include <Ogre.h>

namespace phys
{
    Camera::Camera(const String& Name, CameraManager* Manager)
    {
        Ogre::Camera* OgreCam = Manager->SManager->GetGraphicsWorldPointer()->createCamera(Name);
        this->Construct(OgreCam, Manager);
    }

    Camera::Camera(Ogre::Camera* Camera, CameraManager* Manager)
    {
        this->Construct(Camera, Manager);
    }

    void Camera::Construct(Ogre::Camera* Camera, CameraManager* Manager)
    {
        this->Cam = Camera;
        this->CamManager = Manager;
        this->SetNearClipDistance(5.0f);
        this->SetFarClipDistance(5000.0f);
        SetElementType(Attachable::Camera);
    }

    Camera::~Camera()
    {
        CamManager->SManager->GetGraphicsWorldPointer()->destroyCamera(Cam);
    }

    ConstString& Camera::GetName()
    {
        return this->Cam->getName();
    }

    void Camera::SetCameraType(ProjectionType Type)
    {
        if( Camera::Orthographic == Type )
        {
            this->Cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        }
        else if( Camera::Perspective == Type )
        {
            this->Cam->setProjectionType(Ogre::PT_PERSPECTIVE);
        }
    }

    void Camera::SetOrthoWindow(Real Width, Real Height)
    {
        this->Cam->setOrthoWindow(Width, Height);
    }

    void Camera::SetOrthoWindowHeight(Real Height)
    {
        this->Cam->setOrthoWindowHeight(Height);
    }

    void Camera::SetOrthoWindowWidth(Real Width)
    {
        this->Cam->setOrthoWindowWidth(Width);
    }

    void Camera::SetLocation(Vector3 Location)
    {
        this->Cam->setPosition(Location.GetOgreVector3());
    }

    void Camera::SetDirection(Vector3 Direction)
    {
        this->Cam->setDirection(Direction.GetOgreVector3());
    }

    Vector3 Camera::GetDirection()
    {
        Vector3 Dir(this->Cam->getDirection());
        return Dir;
    }

    void Camera::SetOrientation(Quaternion Orientation)
    {
        this->Cam->setOrientation(Orientation.GetOgreQuaternion());
    }

    void Camera::SetNearClipDistance(Real NearDist)
    {
        this->Cam->setNearClipDistance(NearDist);
    }

    void Camera::SetFarClipDistance(Real FarDist)
    {
        this->Cam->setFarClipDistance (FarDist);
    }

    void Camera::SetAspectRatio(Real Ratio)
    {
        this->Cam->setAspectRatio(Ratio);
    }

    void Camera::LookAt(Vector3 TargetLoc)
    {
        this->Cam->lookAt(TargetLoc.GetOgreVector3());
    }

    void Camera::SetFixedYawAxis(bool UseFixed, Vector3 Axis)
    {
        this->Cam->setFixedYawAxis(UseFixed, Axis.GetOgreVector3());
    }

    void Camera::SetFixedYawAxis(bool UseFixed)
    {
        this->Cam->setFixedYawAxis(UseFixed);
    }

    /*void Camera::SetAutoTracking(bool Enabled, String Target, Vector3 Offset)
    {
        Ogre::SceneNode* Trgt = CamManager->FindNode(Target);
        this->Cam->setAutoTracking(Enabled, Trgt, Offset.GetOgreVector3());
    }

    void Camera::SetAutoTracking(bool Enabled, String Target)
    {
        Ogre::SceneNode* Trgt = CamManager->FindNode(Target);
        this->Cam->setAutoTracking(Enabled, Trgt);
    }*/

    Ray Camera::GetCameraToViewportRay(Real Screenx, Real Screeny)
    {
        Ray R(this->Cam->getCameraToViewportRay(Screenx, Screeny));
        return R;
    }

    String Camera::GetNodeAttachedToCamera()
    {
        Ogre::SceneNode* tempnode = this->Cam->getParentSceneNode();
        return tempnode->getName();
    }

    Vector3 Camera::GetCameraRelativeLocation()
    {
        Vector3 camloc(this->Cam->getPosition());
        return camloc;
    }

    Vector3 Camera::GetCameraGlobalLocation()
    {
        Vector3 camloc(this->Cam->getRealPosition());
        return camloc;
    }

    void Camera::ZoomCamera(Real Zoom)
    {
        Ogre::Vector3 zoomlevel(0,0,Zoom);
        this->Cam->moveRelative(zoomlevel);
    }

    void Camera::ResetZoom()
    {
        Ogre::Vector3 loc;
        loc = this->Cam->getPosition();
        Real zoom=loc.z;
        zoom=0-zoom;
        this->Cam->moveRelative(Ogre::Vector3(0,0,zoom));
    }

    Ogre::Camera* Camera::GetOgreCamera()
    {
        return Cam;
    }
}//phys

#endif
