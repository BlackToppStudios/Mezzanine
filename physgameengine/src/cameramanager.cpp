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
#ifndef _CameraManager_cpp
#define _CameraManager_cpp

#include "cameramanager.h"
#include "camera.h"
#include "world.h"

#include <Ogre.h>

#include <vector>
#include <sstream>

namespace phys
{
    CameraManager::CameraManager (World* world) : ManagerBase(world)
    {
        this->SceneManager = world->OgreSceneManager;
        this->DefaultCamera = NULL;
        this->ONodes=0;
        this->SNodes=0;
        this->Priority = -20;
    }

    CameraManager::~CameraManager ()
    {
        if(NULL!=DefaultCamera)
        {
            delete DefaultCamera;
        }
        while(!Cameras.empty())
        {
            Cameras.pop_back();
        }
        while(!Nodes.empty())
        {
            Nodes.pop_back();
        }
    }

    Ogre::Camera* CameraManager::FindCamera(String Name)
    {
        for( std::vector<Ogre::Camera*>::iterator c=Cameras.begin(); c!=Cameras.end(); c++ )
        {
            if ( Name == (*c)->getName() )
            {
                return *c;
            }
        }
        return NULL;
    }

    Ogre::SceneNode* CameraManager::FindNode(String Name)
    {
        for( std::vector<Ogre::SceneNode*>::iterator c=Nodes.begin(); c!=Nodes.end(); c++ )
        {
            if ( Name == (*c)->getName() )
            {
                return *c;
            }
        }
        return NULL;
    }

    String CameraManager::CreateCamera ()
    {
        if(NULL==DefaultCamera)
        {
            String Name="DefaultCamera";
            this->DefaultCamera = this->SceneManager->createCamera(Name);
            return Name;
        }
        std::stringstream namemaker;
        String Name="Camera";
        namemaker << Name << Cameras.size()+1;
        Ogre::Camera* tempcam = this->SceneManager->createCamera(namemaker.str());
        Cameras.push_back(tempcam);
        return namemaker.str();
    }

    Camera* CameraManager::CreateCameraPtr ()
    {
        if(NULL==DefaultCamera)
        {
            String Name="DefaultCamera";
            this->DefaultCamera = this->SceneManager->createCamera(Name);
            Camera* tempptr = new Camera(DefaultCamera, this);
            return tempptr;
        }
        std::stringstream namemaker;
        String Name="Camera";
        namemaker << Name << Cameras.size()+1;
        Ogre::Camera* tempcam = this->SceneManager->createCamera(namemaker.str());
        Cameras.push_back(tempcam);
        Camera* tempptr = new Camera(tempcam, this);
        return tempptr;
    }

    String CameraManager::CreateOrbitingNode (Vector3 Target, Vector3 RelativeLoc)
    {
        std::stringstream namemaker1;
        String Name1="OrbitingNode";
        this->ONodes++;
        namemaker1 << Name1 << ONodes;
        Ogre::SceneNode* orbitingnode = this->SceneManager->createSceneNode(namemaker1.str());
        Nodes.push_back(orbitingnode);
        //creation
        std::stringstream namemaker2;
        String Name2="CenterNode";
        namemaker2 << Name2 << ONodes;
        Ogre::SceneNode* centernode = this->SceneManager->createSceneNode(namemaker2.str());
        Nodes.push_back(centernode);
        //placement
        this->SceneManager->getRootSceneNode()->addChild(centernode);
        centernode->addChild(orbitingnode);
        centernode->setPosition(Target.GetOgreVector3());
        orbitingnode->setPosition(RelativeLoc.GetOgreVector3());
        orbitingnode->setAutoTracking(true, centernode);
        return namemaker1.str();
    }

    String CameraManager::CreateStandNode (Vector3 LookAt, Vector3 Location)
    {
        std::stringstream namemaker;
        String Name="StandNode";
        this->SNodes++;
        namemaker << Name << SNodes;
        Ogre::SceneNode* tempnode = this->SceneManager->createSceneNode(namemaker.str());
        this->SceneManager->getRootSceneNode()->addChild(tempnode);
        tempnode->setPosition(Location.GetOgreVector3());
        tempnode->lookAt(LookAt.GetOgreVector3(), Ogre::Node::TS_WORLD);
        Nodes.push_back(tempnode);
        return namemaker.str();
    }

    void CameraManager::ClearNodes ()
    {
        this->Nodes.erase(Nodes.begin(), Nodes.end());
        return;
    }

    void CameraManager::ClearCameras ()
    {
        this->Cameras.erase(Cameras.begin(), Cameras.end());
        return;
    }

    void CameraManager::SetLocation (Vector3 Location, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setPosition(Location.GetOgreVector3());
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setPosition(Location.GetOgreVector3());
        return;
    }

    void CameraManager::SetDirection (Vector3 Direction, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setDirection(Direction.GetOgreVector3());
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setDirection(Direction.GetOgreVector3());
        return;
    }

    void CameraManager::SetOrientation (Quaternion Orientation, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setOrientation(Orientation.GetOgreQuaternion());
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setOrientation(Orientation.GetOgreQuaternion());
        return;
    }

    void CameraManager::SetNearClipDistance (Real NearDist, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setNearClipDistance(NearDist);
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setNearClipDistance(NearDist);
        return;
    }

    void CameraManager::SetFarClipDistance (Real FarDist, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setFarClipDistance (FarDist);
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setFarClipDistance (FarDist);
        return;
    }

    void CameraManager::SetAspectRatio (Real Ratio, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setAspectRatio(Ratio);
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setAspectRatio(Ratio);
        return;
    }

    void CameraManager::LookAt (Vector3 TargetLoc, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->lookAt(TargetLoc.GetOgreVector3());
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->lookAt(TargetLoc.GetOgreVector3());
        return;
    }

    void CameraManager::SetFixedYawAxis (bool UseFixed, Vector3 Axis, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setFixedYawAxis(UseFixed, Axis.GetOgreVector3());
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setFixedYawAxis(UseFixed, Axis.GetOgreVector3());
        return;
    }

    void CameraManager::SetFixedYawAxis (bool UseFixed, String Name)
    {
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->setFixedYawAxis(UseFixed);
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setFixedYawAxis(UseFixed);
        return;
    }

    void CameraManager::SetAutoTracking (bool Enabled, String Target, Vector3 Offset, String Name)
    {
        if(Name=="DefaultCamera")
        {
            Ogre::SceneNode* Trgt = FindNode(Target);
            this->DefaultCamera->setAutoTracking(Enabled, Trgt, Offset.GetOgreVector3());
            return;
        }
        Ogre::SceneNode* Trgt = FindNode(Target);
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setAutoTracking(Enabled, Trgt, Offset.GetOgreVector3());
        return;
    }

    void CameraManager::SetAutoTracking (bool Enabled, String Target, String Name)
    {
        if(Name=="DefaultCamera")
        {
            Ogre::SceneNode* Trgt = FindNode(Target);
            this->DefaultCamera->setAutoTracking(Enabled, Trgt);
            return;
        }
        Ogre::SceneNode* Trgt = FindNode(Target);
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->setAutoTracking(Enabled, Trgt);
        return;
    }

    Ray CameraManager::GetCameraToViewportRay (Real Screenx, Real Screeny, String Name)
    {
        if(Name=="DefaultCamera")
        {
            Ray R(this->DefaultCamera->getCameraToViewportRay(Screenx, Screeny));
            return R;
        }
        Ogre::Camera* tempcam = FindCamera(Name);
        Ray R(tempcam->getCameraToViewportRay(Screenx, Screeny));
        return R;
    }

    String CameraManager::GetNodeAttachedToCamera (String Name)
    {
        if(Name=="DefaultCamera")
        {
            Ogre::SceneNode* tempnode = this->DefaultCamera->getParentSceneNode();
            return tempnode->getName();
        }
        Ogre::Camera* tempcam = FindCamera(Name);
        Ogre::SceneNode *tempnode = tempcam->getParentSceneNode();
        return tempnode->getName();
    }

    Vector3 CameraManager::GetNodeLocation (String Name)
    {
        Ogre::SceneNode* tempptr = FindNode(Name);
        Vector3 nodeloc;
        nodeloc.ExtractOgreVector3(tempptr->getPosition());
        return nodeloc;
    }

    Vector3 CameraManager::GetCameraRelativeLocation (String Name)
    {
        if(Name=="DefaultCamera")
        {
            Vector3 camloc(this->DefaultCamera->getPosition());
            return camloc;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        Vector3 camloc(tempptr->getPosition());
        return camloc;
    }

    Vector3 CameraManager::GetCameraGlobalLocation (String Name)
    {
        if(Name=="DefaultCamera")
        {
            Vector3 camloc(this->DefaultCamera->getRealPosition());
            return camloc;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        Vector3 camloc(tempptr->getRealPosition());
        return camloc;
    }

    void CameraManager::ZoomCamera(Real Zoom, String Name)
    {
        Ogre::Vector3 zoomlevel(0,0,Zoom);
        if(Name=="DefaultCamera")
        {
            this->DefaultCamera->moveRelative(zoomlevel);
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        tempptr->moveRelative(zoomlevel);
        return;
    }

    void CameraManager::ResetZoom (String Name)
    {
        Ogre::Vector3 loc;
        if(Name=="DefaultCamera")
        {
            loc = this->DefaultCamera->getPosition();
            Real zoom=loc.z;
            zoom=0-zoom;
            this->DefaultCamera->moveRelative(Ogre::Vector3(0,0,zoom));
            return;
        }
        Ogre::Camera* tempptr = FindCamera(Name);
        loc = tempptr->getPosition();
        Real zoom=loc.z;
        zoom=0-zoom;
        tempptr->moveRelative(Ogre::Vector3(0,0,zoom));
        return;
    }

    void CameraManager::AttachCameraToNode (String NodeName, String CamName)
    {
        Ogre::SceneNode* tempnode = FindNode(NodeName);
        if(CamName=="DefaultCamera")
        {
            tempnode->attachObject(this->DefaultCamera);
        }else{
            Ogre::Camera* tempcam = FindCamera(CamName);
            tempnode->attachObject(tempcam);
        }

        return;
    }

    void CameraManager::DetachCameraFromNode (String NodeName, String CamName)
    {
        Ogre::SceneNode* tempnode = FindNode(NodeName);
        Ogre::Camera* tempcam = FindCamera(CamName);
        tempnode->detachObject(tempcam);
        return;
    }

    void CameraManager::IncrementYOrbit (Real Radian, String Name)
    {
        Ogre::Radian Rad(Radian);
        Ogre::SceneNode* tempptr = FindNode(Name);
        tempptr->getParentSceneNode()->yaw(Rad);
    }

    // Inherited from ManagerBase
    void CameraManager::Initialize()
        {}

    void CameraManager::DoMainLoopItems()
        {}

    ManagerBase::ManagerTypeName CameraManager::GetType() const
        { return ManagerBase::CameraManager; }



}//phys
#endif
