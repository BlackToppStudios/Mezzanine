// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _graphicsviewport_cpp
#define _graphicsviewport_cpp

#include "Graphics/viewport.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/gamewindow.h"

#include "serialization.h"
#include "exception.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        Viewport::Viewport(CameraProxy* ViewportCamera, const Integer& ZOrder, GameWindow* ParentWindow) :
            Parent(ParentWindow),
            ViewportCam(ViewportCamera)
        {
            Ogre::Camera* ViewCam = ViewportCamera ? ViewportCamera->_GetGraphicsObject() : NULL;
            this->OgreViewport = this->Parent->_GetOgreWindowPointer()->addViewport(ViewCam,ZOrder);
        }

        Viewport::Viewport(const XML::Node& XMLNode, GameWindow* ParentWindow) :
            Parent(ParentWindow),
            ViewportCam(NULL)
            { this->ProtoDeSerialize(XMLNode); }

        Viewport::~Viewport()
            { this->Parent->_GetOgreWindowPointer()->removeViewport(this->OgreViewport->getZOrder()); }

        ///////////////////////////////////////////////////////////////////////////////
        // Camera and parent Management

        void Viewport::SetCamera(CameraProxy* ViewportCamera)
        {
            Ogre::Camera* ViewCam = ViewportCamera ? ViewportCamera->_GetGraphicsObject() : NULL;
            if( this->ViewportCam && this->ViewportCam != ViewportCamera )
                this->ViewportCam->CameraVP = NULL;

            this->OgreViewport->setCamera(ViewCam);
            this->ViewportCam = ViewportCamera;
            if( this->ViewportCam )
                this->ViewportCam->CameraVP = this;
        }

        CameraProxy* Viewport::GetCamera() const
            { return this->ViewportCam; }

        GameWindow* Viewport::GetParentWindow() const
            { return this->Parent; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Integer Viewport::GetZOrder() const
            { return this->OgreViewport->getZOrder(); }

        Mezzanine::OrientationMode Viewport::GetOrientationMode() const
        {
            #if OGRE_NO_VIEWPORT_ORIENTATIONMODE != 0
            return Mezzanine::OM_Degree_0;
            #else
            return static_cast<Mezzanine::OrientationMode>( OgreViewport->getOrientationMode() );
            #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Viewport Metrics Management

        void Viewport::SetDimensions(const Vector2& Position, const Vector2& Size)
            { this->SetDimensions(Position.X,Position.Y,Size.X,Size.Y); }

        void Viewport::SetDimensions(const Real& Left, const Real& Top, const Real& Width, const Real& Height)
            { this->OgreViewport->setDimensions(Left,Top,Width,Height); }

        Vector2 Viewport::GetPosition() const
            { return Vector2(this->GetLeft(),this->GetTop()); }

        Vector2 Viewport::GetSize() const
            { return Vector2(this->GetWidth(),this->GetHeight()); }

        Real Viewport::GetLeft() const
            { return this->OgreViewport->getLeft(); }

        Real Viewport::GetTop() const
            { return this->OgreViewport->getTop(); }

        Real Viewport::GetWidth() const
            { return this->OgreViewport->getWidth(); }

        Real Viewport::GetHeight() const
            { return this->OgreViewport->getHeight(); }

        Vector2 Viewport::GetActualPosition() const
            { return Vector2(this->GetActualLeft(),this->GetActualTop()); }

        Vector2 Viewport::GetActualSize() const
            { return Vector2(this->GetActualWidth(),this->GetActualHeight()); }

        Whole Viewport::GetActualLeft() const
            { return this->OgreViewport->getActualLeft(); }

        Whole Viewport::GetActualTop() const
            { return this->OgreViewport->getActualTop(); }

        Whole Viewport::GetActualWidth() const
            { return this->OgreViewport->getActualWidth(); }

        Whole Viewport::GetActualHeight() const
            { return this->OgreViewport->getActualHeight(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Viewport::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node ViewNode = ParentNode.AppendChild( Viewport::GetSerializableName() );

            if( ViewNode.AppendAttribute("Version").SetValue("1") &&
                ViewNode.AppendAttribute("ZOrder").SetValue( this->GetZOrder() ) )
                //ViewNode.AppendAttribute("CameraName").SetValue( this->GetCamera()->GetName() ) )
            {
                XML::Node PositionNode = ViewNode.AppendChild("Position");
                this->GetPosition().ProtoSerialize(PositionNode);

                XML::Node SizeNode = ViewNode.AppendChild("Size");
                this->GetSize().ProtoSerialize(SizeNode);

                return;
            }else{
                SerializeError("Create XML Attribute Values",Viewport::GetSerializableName(),true);
            }
        }

        void Viewport::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            // We need to perform some cleanup first, because Ogre doesn't let us change the viewport ZOrder.
            // Start with the camera
            CameraProxy* CurrCam = this->ViewportCam;
            this->ViewportCam = NULL;
            // Now the viewport itself
            if( this->OgreViewport ) {
                this->Parent->_GetOgreWindowPointer()->removeViewport( this->GetZOrder() );
                this->OgreViewport = NULL;
            }

            // Now do some deserializing
            XML::Attribute CurrAttrib;
            if( SelfRoot.Name() == Viewport::GetSerializableName() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    Integer ViewZOrder = 0;
                    Vector2 TempPos, TempSize;

                    CurrAttrib = SelfRoot.GetAttribute("ZOrder");
                    if( !CurrAttrib.Empty() )
                        ViewZOrder = CurrAttrib.AsInteger();

                    /*CurrAttrib = SelfRoot.GetAttribute("CameraName");
                    if( !CurrAttrib.Empty() )
                        this->SetCamera( CurrAttrib.AsString() );// */

                    XML::Node PositionNode = SelfRoot.GetChild("Position").GetFirstChild();
                    if( !PositionNode.Empty() ) {
                        TempPos.ProtoDeSerialize(PositionNode);
                    }

                    XML::Node SizeNode = SelfRoot.GetChild("Size").GetFirstChild();
                    if( !SizeNode.Empty() ) {
                        TempSize.ProtoDeSerialize(SizeNode);
                    }

                    this->OgreViewport = this->Parent->_GetOgreWindowPointer()->addViewport(NULL,ViewZOrder);
                    this->SetDimensions(TempPos,TempSize);
                    this->SetCamera(CurrCam);
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Viewport::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Viewport::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String Viewport::GetSerializableName()
            { return "Viewport"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Viewport* Viewport::_GetOgreViewport() const
            { return this->OgreViewport; }
    }//Graphics
}//Mezzanine

#endif
