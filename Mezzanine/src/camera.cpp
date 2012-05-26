
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
#ifndef _Camera_cpp
#define _Camera_cpp

#include "camera.h"
#include "cameramanager.h"
#include "stringtool.h"
#include "exception.h"
#include "scenemanager.h"
#include "viewport.h"
#include "world.h"
#include "xml.h"

#include <Ogre.h>

#include <memory>

/// @file camera.h
/// @brief Declaration of the Mezzanine::Camera class

namespace Mezzanine
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
        this->SetFixedYawAxis(true,Mezzanine::Vector3(0,1,0));
    }

    Camera::~Camera()
    {
        if(CamManager->SManager && CamManager->SManager->GetGraphicsWorldPointer())
            { CamManager->SManager->GetGraphicsWorldPointer()->destroyCamera(Cam); }
    }

    ConstString& Camera::GetName() const
    {
        return this->Cam->getName();
    }

    Viewport* Camera::GetViewport() const
    {
        return this->CameraVP;
    }

    void Camera::SetCameraType(const ProjectionType& Type)
    {
        if( Camera::Orthographic == Type )
        {
            this->Cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        }
        else if( Camera::Perspective == Type )
        {
            this->Cam->setProjectionType(Ogre::PT_PERSPECTIVE);
        }else{
            throw(Mezzanine::Exception("Unknown ProjectionType when attempting to set Camera Type."));
        }
    }

    Camera::ProjectionType Camera::GetCameraType() const
    {
        if( this->Cam->getProjectionType()==Ogre::PT_ORTHOGRAPHIC )
        {
            return Camera::Orthographic;

        }
        else if( this->Cam->getProjectionType()==Ogre::PT_PERSPECTIVE )
        {
            return Camera::Perspective;
        }else{
            throw(Mezzanine::Exception("Unknown ProjectionType when attempting to get Camera Type."));
        }
    }

    void Camera::SetOrthoWindow(const Real& Width, const Real& Height)
    {
        this->Cam->setOrthoWindow(Width, Height);
    }

    void Camera::SetOrthoWindowHeight(const Real& Height)
    {
        this->Cam->setOrthoWindowHeight(Height);
    }

    void Camera::SetOrthoWindowWidth(const Real& Width)
    {
        this->Cam->setOrthoWindowWidth(Width);
    }

    void Camera::SetLocation(const Vector3& Location)
    {
        this->Cam->setPosition(Location.GetOgreVector3());
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
    }

    Vector3 Camera::GetLocation() const
    {
        return Vector3(this->Cam->getPosition());
    }

    void Camera::SetOrientation(const Quaternion& Orientation)
    {
        this->Cam->setOrientation(Orientation.GetOgreQuaternion());
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
    }

    Quaternion Camera::GetOrientation() const
    {
        return Quaternion(this->Cam->getOrientation());
    }

    void Camera::SetDirection(const Vector3& Direction)
    {
        this->Cam->setDirection(Direction.GetOgreVector3());
    }

    Vector3 Camera::GetDirection() const
    {
        return Vector3(this->Cam->getDirection());
    }

    void Camera::SetNearClipDistance(const Real& NearDist)
    {
        this->Cam->setNearClipDistance(NearDist);
    }

    void Camera::SetFarClipDistance(const Real& FarDist)
    {
        this->Cam->setFarClipDistance (FarDist);
    }

    void Camera::SetAspectRatio(const Real& Ratio)
    {
        this->Cam->setAspectRatio(Ratio);
    }

    void Camera::LookAt(const Vector3& TargetLoc)
    {
        this->Cam->lookAt(TargetLoc.GetOgreVector3());
    }

    void Camera::Move(const Vector3& ToMove)
    {
        this->Cam->move(ToMove.GetOgreVector3());
    }

    void Camera::MoveRelative(const Vector3& ToMove)
    {
        this->Cam->moveRelative(ToMove.GetOgreVector3());
    }

    void Camera::SetFixedYawAxis(bool UseFixed, const Vector3& Axis)
    {
        this->Cam->setFixedYawAxis(UseFixed, Axis.GetOgreVector3());
        this->YawAxis = Axis;
        this->YawOnAxis = UseFixed;
    }

    void Camera::SetFixedYawAxis(bool UseFixed)
    {
        this->Cam->setFixedYawAxis(UseFixed);
        this->YawOnAxis = UseFixed;
    }

    bool Camera::IsFixedYawEnabled() const
        { return this->YawOnAxis; }

    Vector3 Camera::GetFixedYawAxis() const
    {
        if (this->YawOnAxis)
            { return this->YawAxis; }
        else
            { return Vector3(0,0,0); }
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

    Ray Camera::GetCameraToViewportRay(const Real& ScreenX, const Real& ScreenY) const
    {
        Ray R(this->Cam->getCameraToViewportRay(ScreenX, ScreenY));
        return R;
    }

    WorldAndSceneObjectType Camera::GetType() const
    {
        return Mezzanine::WSO_Camera;
    }

    void Camera::SetScaling(const Vector3& Scale)
    {
        // Do nothing, can't scale a camera.
    }

    Vector3 Camera::GetScaling() const
    {
        return Vector3(1,1,1);
    }

    void Camera::SetLocalLocation(const Vector3& Location)
    {
        LocalXform.Location = Location;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
    }

    void Camera::SetLocalOrientation(const Quaternion& Orientation)
    {
        LocalXform.Rotation = Orientation;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
    }

    Ogre::Camera* Camera::GetOgreCamera() const
    {
        return Cam;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited From AttachableChild

}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::Camera& Ev)
{

    //stream << "<Camera Version=\"1\" attachedto=\"1\" location=\"1\" orientation=\"1\" name=\"1\" />" << *(Ev.Cam) << "</Camera>";
    stream      << "<Camera Version=\"1\" Name=\"" << Ev.GetName() << "\" AttachedTo=\"" << (Ev.GetParent() ? Ev.GetParent()->GetName() : "") << "\" CameraPerspective=\"" << Ev.GetCameraType() << "\">"
                << "<Orientation>" << Ev.GetOrientation() << "</Orientation>"
                << "<Location>" << Ev.GetLocalLocation() << "</Location>";

    if ( Ev.IsFixedYawEnabled() )
        {stream << "<FixedYawAxis Enabled=\"1\">" << Ev.GetFixedYawAxis() << "</FixedYawAxis>"; }
    else
        {stream << "<FixedYawAxis Enabled=\"0\" />"; }

    stream      << "</Camera>";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Camera& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "Camera", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Camera& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("Camera") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetCameraType(static_cast<Mezzanine::Camera::ProjectionType>(OneNode.GetAttribute("CameraPerspective").AsInt()));
            Mezzanine::WorldNode * AttachPtr = Mezzanine::SceneManager::GetSingletonPtr()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            Mezzanine::Quaternion TempQuat(0,0,0,0);
            Mezzanine::Vector3 TempVec(0,0,0);

            for(Mezzanine::XML::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                Mezzanine::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'O':   //Orientation
                        if(Name==Mezzanine::String("Orientation"))
                        {
                            Child.GetFirstChild() >> TempQuat;
                            Ev.SetOrientation(TempQuat);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Camera: Includes unknown Element O-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==Mezzanine::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Camera: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    case 'F':   //FixedYawAxis
                        if(Name==Mezzanine::String("FixedYawAxis") && Child.GetAttribute("Enabled").AsBool())
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetFixedYawAxis(true,TempVec);
                        }else if(Name==Mezzanine::String("FixedYawAxis")){
                            Ev.SetFixedYawAxis(false);
                        }else{
                            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Camera: Includes unknown Element F-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Incompatible XML Version for Camera: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( Mezzanine::Exception("Incompatible XML Version for Camera: Not Version 1"));
        }
    }else{
        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a Camera, found a ", OneNode.Name())));
    }
}
#endif // \MEZZXML


#endif
