//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _CameraManager_cpp
#define _CameraManager_cpp

#include "cameramanager.h"
#include "scenemanager.h"
#include "graphicsmanager.h"
#include "camera.h"
#include "cameracontroller.h"

#include <Ogre.h>

#include <vector>
#include <sstream>

namespace Mezzanine
{
    template<> CameraManager* Singleton<CameraManager>::SingletonPtr = 0;

    CameraManager::CameraManager()
        : SceneMan(NULL)
    {
        if(SceneManager::SingletonValid())
            this->SceneMan = SceneManager::GetSingletonPtr();
        this->Priority = 40;
    }

#ifdef MEZZXML
    CameraManager::CameraManager(XML::Node& XMLNode)
        : SceneMan(NULL)
    {
        if(SceneManager::SingletonValid())
            this->SceneMan = SceneManager::GetSingletonPtr();
        this->Priority = 40;
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
    }
#endif

    CameraManager::~CameraManager()
    {
        DestroyAllCameraControllers();
        DestroyAllCameras();
    }

    Camera* CameraManager::FindCamera(const String& Name)
    {
        for( std::vector<Camera*>::iterator c=Cameras.begin(); c!=Cameras.end(); c++ )
        {
            if ( Name == (*c)->GetName() )
            {
                return *c;
            }
        }
        return NULL;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Camera Management

    Camera* CameraManager::CreateCamera()
    {
        StringStream CamName;
        CamName << "Camera" << Cameras.size() + 1;
        return CreateCamera(CamName.str());
    }

    Camera* CameraManager::CreateCamera(const String& Name)
    {
        if(!SceneMan)
        {
            if(SceneManager::SingletonValid()) SceneMan = SceneManager::GetSingletonPtr();
            else { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a camera before the SceneManager is created.  This is not supported."); }
        }
        Camera* tempcam = new Camera(Name, this);
        Cameras.push_back(tempcam);
        return tempcam;
    }

    Camera* CameraManager::GetCamera(const String& Name)
    {
        Camera* tempcam = FindCamera(Name);
        return tempcam;
    }

    Camera* CameraManager::GetCamera(const Whole& Index)
    {
        return Cameras[Index];
    }

    Whole CameraManager::GetNumCameras()
    {
        return Cameras.size();
    }

    void CameraManager::DestroyAllCameras()
    {
        Camera* camera = NULL;
        for( std::vector< Camera* >::iterator it = Cameras.begin() ; it != Cameras.end() ; it++ )
        {
            camera = (*it);
            delete camera;
        }
        Cameras.clear();
        return;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Camera Controller Management

    CameraController* CameraManager::GetOrCreateCameraController(Camera* Controlled)
    {
        std::map<Camera*,CameraController*>::iterator CamIt = CameraControllers.find(Controlled);
        if(CamIt == CameraControllers.end())
        {
            CameraController* Controller = new CameraController(Controlled);
            CameraControllers[Controlled] = Controller;
            return Controller;
        }else{
            return (*CamIt).second;
        }
    }

    void CameraManager::DestroyCameraController(CameraController* ToBeDestroyed)
    {
        if(CameraControllers.empty())
            return;
        for( std::map<Camera*,CameraController*>::iterator CamIt = CameraControllers.begin() ; CamIt != CameraControllers.end() ; CamIt++ )
        {
            if(ToBeDestroyed == (*CamIt).second)
            {
                delete (*CamIt).second;
                CameraControllers.erase(CamIt);
                return;
            }
        }
    }

    void CameraManager::DestroyCameraController(Camera* ControlledCam)
    {
        if(CameraControllers.empty())
            return;
        std::map<Camera*,CameraController*>::iterator CamIt = CameraControllers.find(ControlledCam);
        if(CamIt != CameraControllers.end())
        {
            delete (*CamIt).second;
            CameraControllers.erase(CamIt);
            return;
        }
    }

    void CameraManager::DestroyAllCameraControllers()
    {
        if(CameraControllers.empty())
            return;
        for( std::map<Camera*,CameraController*>::iterator CamIt = CameraControllers.begin() ; CamIt != CameraControllers.end() ; CamIt++ )
        {
            delete (*CamIt).second;
        }
        CameraControllers.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    SceneManager* CameraManager::GetScene() const
    {
        return SceneMan;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited from ManagerBase

    void CameraManager::Initialize()
    {
        if(!SceneMan)
        {
            if(SceneManager::SingletonValid())
            {
                SceneMan = SceneManager::GetSingletonPtr();
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to initiailze CameraManager when SceneManager has not yet been constructed.  The SceneManager is a dependancy of the CameraManager.");
            }
        }
        Initialized = true;
    }

    void CameraManager::DoMainLoopItems()
        {}

    ManagerBase::ManagerType CameraManager::GetInterfaceType() const
        { return ManagerBase::CameraManager; }

    String CameraManager::GetImplementationTypeName() const
        { return "DefaultCameraManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultCameraManagerFactory Methods

    DefaultCameraManagerFactory::DefaultCameraManagerFactory()
    {
    }

    DefaultCameraManagerFactory::~DefaultCameraManagerFactory()
    {
    }

    String DefaultCameraManagerFactory::GetManagerTypeName() const
    {
        return "DefaultCameraManager";
    }

    ManagerBase* DefaultCameraManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(CameraManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return CameraManager::GetSingletonPtr();
        }else return new CameraManager();
    }

    ManagerBase* DefaultCameraManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(CameraManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return CameraManager::GetSingletonPtr();
        }else return new CameraManager(XMLNode);
    }

    void DefaultCameraManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine
#endif
