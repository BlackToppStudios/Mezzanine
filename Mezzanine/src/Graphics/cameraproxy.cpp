// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicscameraproxy_cpp
#define _graphicscameraproxy_cpp

/// @file
/// @brief This file contains the implementation for the World proxy wrapping camera functionality.

#include "Graphics/cameraproxy.h"
#include "Graphics/scenemanager.h"
#include "Graphics/viewport.h"

#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>

namespace
{
    /// @internal
    /// @brief Converts an Ogre PolygonMode enum value to it's corresponding Mezzanine value.
    /// @param PolyMode The Ogre value to be converted.
    /// @return Returns the Mezzanine CameraPolyMode corresponding to the provided Ogre value.
    Mezzanine::Graphics::CameraPolyMode ConvertPolygonMode(const Ogre::PolygonMode PolyMode)
    {
        switch(PolyMode)
        {
            case Ogre::PM_POINTS:      return Mezzanine::Graphics::CPM_Points;      break;
            case Ogre::PM_WIREFRAME:   return Mezzanine::Graphics::CPM_WireFrame;   break;
            case Ogre::PM_SOLID:       return Mezzanine::Graphics::CPM_Solid;       break;
        }
        return Mezzanine::Graphics::CPM_Solid;
    }
    /// @internal
    /// @brief Converts a Mezzanine CameraPolyMode enum value to it's corresponding Ogre value.
    /// @param PolyMode The Mezzanine value to be converted.
    /// @return Returns the Ogre PolygonMode corresponding to the provided Mezzanine value.
    Ogre::PolygonMode ConvertPolygonMode(const Mezzanine::Graphics::CameraPolyMode PolyMode)
    {
        switch(PolyMode)
        {
            case Mezzanine::Graphics::CPM_Points:      return Ogre::PM_POINTS;      break;
            case Mezzanine::Graphics::CPM_WireFrame:   return Ogre::PM_WIREFRAME;   break;
            case Mezzanine::Graphics::CPM_Solid:       return Ogre::PM_SOLID;       break;
        }
        return Ogre::PM_SOLID;
    }

    /// @internal
    /// @brief Converts an Ogre ProjectionType enum value to it's corresponding Mezzanine type.
    /// @param ProjType The Ogre type to be converted.
    /// @return Returns the Mezzanine ProjectionType corresponding to the provided Ogre type.
    Mezzanine::Graphics::ProjectionType ConvertProjectionType(const Ogre::ProjectionType ProjType)
    {
        switch(ProjType)
        {
            case Ogre::PT_ORTHOGRAPHIC:   return Mezzanine::Graphics::PT_Orthographic;   break;
            case Ogre::PT_PERSPECTIVE:    return Mezzanine::Graphics::PT_Perspective;    break;
        }
        return Mezzanine::Graphics::PT_Perspective;
    }
    /// @internal
    /// @brief Converts a Mezzanine ProjectionType enum value to it's corresponding Ogre type.
    /// @param ProjType The Mezzanine type to be converted.
    /// @return Returns the Ogre ProjectionType corresponding to the provided Mezzanine type.
    Ogre::ProjectionType ConvertProjectionType(const Mezzanine::Graphics::ProjectionType ProjType)
    {
        switch(ProjType)
        {
            case Mezzanine::Graphics::PT_Orthographic:   return Ogre::PT_ORTHOGRAPHIC;   break;
            case Mezzanine::Graphics::PT_Perspective:    return Ogre::PT_PERSPECTIVE;    break;
        }
        return Ogre::PT_PERSPECTIVE;
    }

    /// @internal
    /// @brief Converts an Ogre OrientationMode enum value to it's corresponding Mezzanine type.
    /// @param OriMode The Ogre type to be converted.
    /// @return Returns the Mezzanine OrientationMode corresponding to the provided Ogre type.
    Mezzanine::Graphics::OrientationMode ConvertOrientationMode(const Ogre::OrientationMode OriMode)
    {
        switch(OriMode)
        {
            case Ogre::OR_DEGREE_0:         return Mezzanine::Graphics::OM_Degree_0;         break;
            case Ogre::OR_DEGREE_90:        return Mezzanine::Graphics::OM_Degree_90;        break;
            case Ogre::OR_DEGREE_180:       return Mezzanine::Graphics::OM_Degree_180;       break;
            case Ogre::OR_DEGREE_270:       return Mezzanine::Graphics::OM_Degree_270;       break;
        }
        return Mezzanine::Graphics::OM_Degree_0;
    }
    /// @internal
    /// @brief Converts a Mezzanine OrientationMode enum value to it's corresponding Ogre type.
    /// @param OriMode The Mezzanine type to be converted.
    /// @return Returns the Ogre OrientationMode corresponding to the provided Mezzanine type.
    Ogre::OrientationMode ConvertOrientationMode(const Mezzanine::Graphics::OrientationMode OriMode)
    {
        switch(OriMode)
        {
            case Mezzanine::Graphics::OM_Degree_0:         return Ogre::OR_DEGREE_0;         break;
            case Mezzanine::Graphics::OM_Degree_90:        return Ogre::OR_DEGREE_90;        break;
            case Mezzanine::Graphics::OM_Degree_180:       return Ogre::OR_DEGREE_180;       break;
            case Mezzanine::Graphics::OM_Degree_270:       return Ogre::OR_DEGREE_270;       break;
        }
        return Ogre::OR_DEGREE_0;
    }
}

namespace Mezzanine
{
    namespace Graphics
    {
        CameraProxy::CameraProxy(const UInt32 ID, SceneManager* Creator) :
            RenderableProxy(ID,Creator),
            GraphicsCamera(NULL),
            CameraVP(NULL),
            UseFixedYaw(true)
            { this->CreateCamera(); }

        CameraProxy::CameraProxy(const XML::Node& SelfRoot, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsCamera(NULL),
            CameraVP(NULL),
            UseFixedYaw(true)
        {
            this->CreateCamera();
            this->ProtoDeSerialize(SelfRoot);
        }

        CameraProxy::~CameraProxy()
            { this->DestroyCamera(); }

        void CameraProxy::CreateCamera()
        {
            this->GraphicsCamera = this->Manager->_GetGraphicsWorldPointer()->createCamera( CameraProxy::GenerateName() );
            this->GraphicsNode->attachObject( this->GraphicsCamera );
            this->GraphicsCamera->MovableObject::getUserObjectBindings().setUserAny( Ogre::Any( static_cast<RenderableProxy*>( this ) ) );
            this->GraphicsCamera->setVisibilityFlags(0);
            this->GraphicsCamera->setQueryFlags(0);
        }

        void CameraProxy::DestroyCamera()
        {
            if( this->GraphicsCamera ) {
                if( this->CameraVP ) {
                    this->CameraVP->SetCamera(NULL);
                }

                this->GraphicsNode->detachObject( this->GraphicsCamera );
                this->Manager->_GetGraphicsWorldPointer()->destroyCamera( this->GraphicsCamera );
            }
        }

        String CameraProxy::GenerateName()
        {
            static UInt32 NameCounter = 0;
            StringStream NameStream;
            NameStream << "Camera" << ++NameCounter;
            return NameStream.str();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType CameraProxy::GetProxyType() const
            { return Mezzanine::PT_Graphics_CameraProxy; }

        Boole CameraProxy::IsStatic() const
            { return false; }

        Viewport* CameraProxy::GetViewport() const
            { return this->CameraVP; }

        Boole CameraProxy::IsFixedYawEnabled() const
            { return this->UseFixedYaw; }

        void CameraProxy::LookAt(const Vector3& TargetLoc)
            { this->GraphicsNode->lookAt( TargetLoc.GetOgreVector3(), Ogre::Node::TS_WORLD ); }

        void CameraProxy::MoveRelative(const Vector3& ToMove)
            { this->GraphicsNode->translate( ToMove.GetOgreVector3(), Ogre::Node::TS_LOCAL ); }

        void CameraProxy::SetDirection(const Vector3& Dir)
            { this->SetOrientation( Vector3::Neg_Unit_Z().GetRotationToAxis( Dir ) ); }

        Vector3 CameraProxy::GetDirection() const
            { return ( this->GetOrientation() * Vector3::Neg_Unit_Z() ); }

        Ray CameraProxy::GetCameraToViewportRay(const Real ScreenX, const Real ScreenY) const
            { return Ray( this->GraphicsCamera->getCameraToViewportRay(ScreenX,ScreenY) ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Camera Properties

        void CameraProxy::SetPolygonMode(const Graphics::CameraPolyMode PolyMode)
            { this->GraphicsCamera->setPolygonMode( ConvertPolygonMode(PolyMode) ); }

        Graphics::CameraPolyMode CameraProxy::GetPolygonMode() const
            { return ConvertPolygonMode( this->GraphicsCamera->getPolygonMode() ); }

        void CameraProxy::SetProjectionType(const Graphics::ProjectionType ProjType)
            { this->GraphicsCamera->setProjectionType( ConvertProjectionType(ProjType) ); }

        Graphics::ProjectionType CameraProxy::GetProjectionType() const
            { return ConvertProjectionType( this->GraphicsCamera->getProjectionType() ); }

        void CameraProxy::SetOrientationMode(const Graphics::OrientationMode OriMode)
            { this->GraphicsCamera->setOrientationMode( ConvertOrientationMode(OriMode) ); }

        Graphics::OrientationMode CameraProxy::GetOrientationMode() const
            { return ConvertOrientationMode( this->GraphicsCamera->getOrientationMode() ); }

        void CameraProxy::SetOrthoWindow(const Real Width, const Real Height)
            { this->GraphicsCamera->setOrthoWindow(Width,Height); }

        void CameraProxy::SetOrthoWindowWidth(const Real Width)
            { this->GraphicsCamera->setOrthoWindowWidth(Width); }

        Real CameraProxy::GetOrthoWindowWidth() const
            { return this->GraphicsCamera->getOrthoWindowWidth(); }

        void CameraProxy::SetOrthoWindowHeight(const Real Height)
            { this->GraphicsCamera->setOrthoWindowHeight(Height); }

        Real CameraProxy::GetOrthoWindowHeight() const
            { return this->GraphicsCamera->getOrthoWindowHeight(); }

        void CameraProxy::SetNearClipDistance(const Real NearDist)
            { this->GraphicsCamera->setNearClipDistance(NearDist); }

        Real CameraProxy::GetNearClipDistance() const
            { return this->GraphicsCamera->getNearClipDistance(); }

        void CameraProxy::SetFarClipDistance(const Real FarDist)
            { this->GraphicsCamera->setFarClipDistance(FarDist); }

        Real CameraProxy::GetFarClipDistance() const
            { return this->GraphicsCamera->getFarClipDistance(); }

        void CameraProxy::SetFieldOfViewY(const Real FOV)
            { this->GraphicsCamera->setFOVy( Ogre::Radian(FOV) ); }

        Real CameraProxy::GetFieldOfViewY() const
            { return this->GraphicsCamera->getFOVy().valueRadians(); }

        void CameraProxy::SetAspectRatio(const Real Ratio)
            { this->GraphicsCamera->setAspectRatio(Ratio); }

        Real CameraProxy::GetAspectRatio() const
            { return this->GraphicsCamera->getAspectRatio(); }

        void CameraProxy::SetFixedYawAxis(const Boole UseFixed, const Vector3& Axis)
        {
            this->UseFixedYaw = UseFixed;
            this->FixedYawAxis = Axis;
            this->GraphicsCamera->setFixedYawAxis(UseFixed,Axis.GetOgreVector3());
        }

        Vector3 CameraProxy::GetFixedYawAxis() const
            { return this->FixedYawAxis; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void CameraProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( CameraProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("PolygonMode").SetValue( this->GetPolygonMode() ) &&
                PropertiesNode.AppendAttribute("ProjectionType").SetValue( this->GetProjectionType() ) &&
                PropertiesNode.AppendAttribute("OrientationMode").SetValue( this->GetOrientationMode() ) &&
                PropertiesNode.AppendAttribute("OrthoWidth").SetValue( this->GetOrthoWindowWidth() ) &&
                PropertiesNode.AppendAttribute("OrthoHeight").SetValue( this->GetOrthoWindowHeight() ) &&
                PropertiesNode.AppendAttribute("NearClipDistance").SetValue( this->GetNearClipDistance() ) &&
                PropertiesNode.AppendAttribute("FarClipDistance").SetValue( this->GetFarClipDistance() ) &&
                PropertiesNode.AppendAttribute("FieldOfView").SetValue( this->GetFieldOfViewY() ) &&
                PropertiesNode.AppendAttribute("AspectRatio").SetValue( this->GetAspectRatio() ) &&
                PropertiesNode.AppendAttribute("UseFixedYaw").SetValue( this->IsFixedYawEnabled() ) )
            {
                XML::Node FixedYawAxisNode = PropertiesNode.AppendChild("FixedYawAxis");
                this->GetFixedYawAxis().ProtoSerialize( FixedYawAxisNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",CameraProxy::GetSerializableName() + "Properties",true);
            }
        }

        void CameraProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( CameraProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    Boole UseFixed = true;
                    Vector3 FixedYaw = Vector3::Unit_Y();
                    Real OrthoWidth = 0, OrthoHeight = 0;

                    CurrAttrib = PropertiesNode.GetAttribute("PolygonMode");
                    if( !CurrAttrib.Empty() )
                        this->SetPolygonMode( static_cast<Graphics::CameraPolyMode>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("ProjectionType");
                    if( !CurrAttrib.Empty() )
                        this->SetProjectionType( static_cast<Graphics::ProjectionType>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("OrientationMode");
                    if( !CurrAttrib.Empty() )
                        this->SetOrientationMode( static_cast<Graphics::OrientationMode>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("OrthoWidth");
                    if( !CurrAttrib.Empty() )
                        OrthoWidth = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("OrthoHeight");
                    if( !CurrAttrib.Empty() )
                        OrthoHeight = CurrAttrib.AsReal();

                    this->SetOrthoWindow(OrthoWidth,OrthoHeight);

                    CurrAttrib = PropertiesNode.GetAttribute("NearClipDistance");
                    if( !CurrAttrib.Empty() )
                        this->SetNearClipDistance( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("FarClipDistance");
                    if( !CurrAttrib.Empty() )
                        this->SetFarClipDistance( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("FieldOfView");
                    if( !CurrAttrib.Empty() )
                        this->SetFieldOfViewY( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("AspectRatio");
                    if( !CurrAttrib.Empty() )
                        this->SetAspectRatio( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("UseFixedYaw");
                    if( !CurrAttrib.Empty() )
                        UseFixed = StringTools::ConvertToBool( CurrAttrib.AsString() );

                    XML::Node FixedYawAxisNode = PropertiesNode.GetChild("FixedYawAxis").GetFirstChild();
                    if( !FixedYawAxisNode.Empty() ) {
                        FixedYaw.ProtoDeSerialize(FixedYawAxisNode);
                    }

                    this->SetFixedYawAxis(UseFixed,FixedYaw);
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (CameraProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,CameraProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String CameraProxy::GetDerivedSerializableName() const
            { return CameraProxy::GetSerializableName(); }

        String CameraProxy::GetSerializableName()
            { return "CameraProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Camera* CameraProxy::_GetGraphicsObject() const
            { return this->GraphicsCamera; }

        Ogre::MovableObject* CameraProxy::_GetBaseGraphicsObject() const
            { return this->GraphicsCamera; }
    }//Graphics
}//Mezzanine

#endif
