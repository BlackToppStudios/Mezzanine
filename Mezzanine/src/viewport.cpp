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
#ifndef _viewport_cpp
#define _viewport_cpp

#include "viewport.h"
#include "cameramanager.h"
#include "camera.h"
#include "gamewindow.h"

#include <Ogre.h>

namespace Mezzanine
{
    Viewport::Viewport(Camera* ViewportCamera, const Integer& ZOrder, GameWindow* ParentWindow)
        : Parent(ParentWindow),
          ViewportCam(ViewportCamera)
    {
        Ogre::Camera* ViewCam = ViewportCamera ? ViewportCamera->GetOgreCamera() : NULL;
        OgreViewport = Parent->_GetOgreWindowPointer()->addViewport(ViewCam,ZOrder);
    }

    Viewport::~Viewport()
    {
        Parent->_GetOgreWindowPointer()->removeViewport(OgreViewport->getZOrder());
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Camera and parent Management

    void Viewport::SetCamera(Camera* ViewportCamera)
    {
        Ogre::Camera* ViewCam = ViewportCamera ? ViewportCamera->GetOgreCamera() : NULL;
        if(ViewportCam && ViewportCam != ViewportCamera)
            ViewportCam->CameraVP = NULL;

        OgreViewport->setCamera(ViewCam);
        this->ViewportCam = ViewportCamera;
        if(ViewportCam)
            ViewportCam->CameraVP = this;
    }

    Camera* Viewport::GetViewportCamera()
    {
        return ViewportCam;
    }

    GameWindow* Viewport::GetParentWindow()
    {
        return Parent;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Viewport Metrics Management

    Integer Viewport::GetZOrder() const
    {
        return OgreViewport->getZOrder();
    }

    void Viewport::SetDimensions(const Vector2& Position, const Vector2& Size)
    {
        SetDimensions(Position.X,Position.Y,Size.X,Size.Y);
    }

    void Viewport::SetDimensions(const Real& Left, const Real& Top, const Real& Width, const Real& Height)
    {
        OgreViewport->setDimensions(Left,Top,Width,Height);
    }

    Real Viewport::GetLeft() const
    {
        return OgreViewport->getLeft();
    }

    Real Viewport::GetTop() const
    {
        return OgreViewport->getTop();
    }

    Real Viewport::GetWidth() const
    {
        return OgreViewport->getWidth();
    }

    Real Viewport::GetHeight() const
    {
        return OgreViewport->getHeight();
    }

    Whole Viewport::GetActualLeft() const
    {
        return OgreViewport->getActualLeft();
    }

    Whole Viewport::GetActualTop() const
    {
        return OgreViewport->getActualTop();
    }

    Whole Viewport::GetActualWidth() const
    {
        return OgreViewport->getActualWidth();
    }

    Whole Viewport::GetActualHeight() const
    {
        return OgreViewport->getActualHeight();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    Ogre::Viewport* Viewport::GetOgreViewport() const
    {
        return OgreViewport;
    }
}

#endif
