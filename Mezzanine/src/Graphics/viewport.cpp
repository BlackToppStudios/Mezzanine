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
#ifndef _graphicsviewport_cpp
#define _graphicsviewport_cpp

#include "Graphics/viewport.h"
#include "Graphics/cameramanager.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/gamewindow.h"

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

        Viewport::~Viewport()
        {
            this->Parent->_GetOgreWindowPointer()->removeViewport(this->OgreViewport->getZOrder());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Camera and parent Management

        void Viewport::SetCamera(CameraProxy* ViewportCamera)
        {
            Ogre::Camera* ViewCam = ViewportCamera ? ViewportCamera->_GetGraphicsObject() : NULL;
            if(ViewportCam && ViewportCam != ViewportCamera)
                ViewportCam->CameraVP = NULL;

            OgreViewport->setCamera(ViewCam);
            this->ViewportCam = ViewportCamera;
            if(ViewportCam)
                ViewportCam->CameraVP = this;
        }

        CameraProxy* Viewport::GetViewportCamera() const
            { return this->ViewportCam; }

        GameWindow* Viewport::GetParentWindow() const
            { return this->Parent; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Integer Viewport::GetZOrder() const
            { return OgreViewport->getZOrder(); }

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

        Real Viewport::GetLeft() const
            { return this->OgreViewport->getLeft(); }

        Real Viewport::GetTop() const
            { return this->OgreViewport->getTop(); }

        Real Viewport::GetWidth() const
            { return this->OgreViewport->getWidth(); }

        Real Viewport::GetHeight() const
            { return this->OgreViewport->getHeight(); }

        Whole Viewport::GetActualLeft() const
            { return this->OgreViewport->getActualLeft(); }

        Whole Viewport::GetActualTop() const
            { return this->OgreViewport->getActualTop(); }

        Whole Viewport::GetActualWidth() const
            { return this->OgreViewport->getActualWidth(); }

        Whole Viewport::GetActualHeight() const
            { return this->OgreViewport->getActualHeight(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Viewport* Viewport::GetOgreViewport() const
            { return this->OgreViewport; }
    }//Graphics
}//Mezzanine

#endif
