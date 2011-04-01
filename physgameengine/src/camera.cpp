
//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#include "exception.h"
#include "scenemanager.h"
#include "world.h"
#include "xml.h"

#include <Ogre.h>

#include <memory>

/// @file camera.h
/// @brief Declaration of the phys::Camera class

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
        this->SetFixedYawAxis(true,phys::Vector3(0,1,0));
    }

    Camera::~Camera()
    {
        CamManager->SManager->GetGraphicsWorldPointer()->destroyCamera(Cam);
    }

    ConstString& Camera::GetName() const
    {
        return this->Cam->getName();
    }

    void Camera::SetCameraType(const ProjectionType Type)
    {
        if( Camera::Orthographic == Type )
        {
            this->Cam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        }
        else if( Camera::Perspective == Type )
        {
            this->Cam->setProjectionType(Ogre::PT_PERSPECTIVE);
        }else{
            throw(phys::Exception("Unknown ProjectionType when attempting to set Camera Type."));
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
            throw(phys::Exception("Unknown ProjectionType when attempting to get Camera Type."));
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
    }
    void Camera::SetDirection(const Vector3& Direction)
    {
        this->Cam->setDirection(Direction.GetOgreVector3());
    }

    void Camera::SetOrientation(const Quaternion& Orientation)
    {
        this->Cam->setOrientation(Orientation.GetOgreQuaternion());
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

    Ray Camera::GetCameraToViewportRay(Real Screenx, Real Screeny) const
    {
        Ray R(this->Cam->getCameraToViewportRay(Screenx, Screeny));
        return R;
    }

    /*WorldNode* Camera::GetWorldNode() const
    {
        //Ogre::SceneNode* tempnode = this->Cam->getParentSceneNode();
        //return tempnode->getName();
    }*/

    Vector3 Camera::GetLocation() const
        { return  Vector3(this->Cam->getPosition()); }

    Vector3 Camera::GetRelativeLocation() const
        { return  Vector3(this->Cam->getPosition()); }

    Vector3 Camera::GetGlobalLocation() const
        { return  Vector3(this->Cam->getRealPosition()); }

    Quaternion Camera::GetOrientation() const
    {
        return Quaternion(this->Cam->getOrientation());
    }

    void Camera::ZoomCamera(const Real& Zoom)
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

    Ogre::Camera* Camera::GetOgreCamera() const
    {
        return Cam;
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Inherited From Attachable
    Attachable::AttachableElement Camera::GetAttachableType() const
        { return Attachable::Camera; }

    void Camera::AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target)
    {
        Attachable::AttachToFinal(RawTarget, Target);
        RawTarget->attachObject(this->Cam);
    }

    void Camera::DetachFromFinal(Ogre::SceneNode* RawTarget)
    {
        Attachable::DetachFromFinal(RawTarget);
        RawTarget->detachObject(this->Cam);
    }

}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::Camera& Ev)
{

    //stream << "<Camera Version=\"1\" attachedto=\"1\" location=\"1\" orientation=\"1\" name=\"1\" />" << *(Ev.Cam) << "</Camera>";
    stream      << "<Camera Version=\"1\" Name=\"" << Ev.GetName() << "\" AttachedTo=\"" << (Ev.GetAttachedTo() ? Ev.GetAttachedTo()->GetName() : "") << "\" CameraPerspective=\"" << Ev.GetCameraType() << "\">"
                << "<Orientation>" << Ev.GetOrientation() << "</Orientation>"
                << "<Location>" << Ev.GetRelativeLocation() << "</Location>";

    if ( Ev.IsFixedYawEnabled() )
        {stream << "<FixedYawAxis Enabled=\"1\">" << Ev.GetFixedYawAxis() << "</FixedYawAxis>"; }
    else
        {stream << "<FixedYawAxis Enabled=\"0\" />"; }

    stream      << "</Camera>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Camera& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "Camera", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::Camera& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("Camera") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.SetCameraType(static_cast<phys::Camera::ProjectionType>(OneNode.GetAttribute("CameraPerspective").AsInt()));
            phys::WorldNode * AttachPtr = phys::World::GetWorldPointer()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            phys::Quaternion TempQuat(0,0,0,0);
            phys::Vector3 TempVec(0,0,0);

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'O':   //Orientation
                        if(Name==phys::String("Orientation"))
                        {
                            Child.GetFirstChild() >> TempQuat;
                            Ev.SetOrientation(TempQuat);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for Camera: Includes unknown Element O-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==phys::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for Camera: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    case 'F':   //FixedYawAxis
                        if(Name==phys::String("FixedYawAxis") && Child.GetAttribute("Enabled").AsBool())
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetFixedYawAxis(true,TempVec);
                        }else if(Name==phys::String("FixedYawAxis")){
                            Ev.SetFixedYawAxis(false);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for Camera: Includes unknown Element F-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for Camera: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( phys::Exception("Incompatible XML Version for Camera: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a Camera, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML


#endif
