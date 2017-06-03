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
#ifndef _mousepicker_cpp
#define _mousepicker_cpp

#include "mousepicker.h"
#include "mousepickdragger.h"
#include "rayquery.h"
#include "world.h"
#include "worldobject.h"
#include "worldproxy.h"

#include "Input/mouse.h"

#include "Graphics/cameraproxy.h"
#include "Graphics/gamewindow.h"
#include "Graphics/viewport.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // MousePicker Methods

    MousePicker::MousePicker() :
        Selector(NULL),
        Query(NULL),
        Dragger(NULL)
        {  }

    MousePicker::MousePicker(Input::Mouse* Cursor, RayQuery* WorldQuery, MousePickDragger* WorldDragger) :
        Selector(Cursor),
        Query(WorldQuery),
        Dragger(WorldDragger)
        {  }

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization and Deinitialization

    void MousePicker::Initialize(Input::Mouse* Cursor, RayQuery* WorldQuery, MousePickDragger* WorldDragger)
    {
        this->Selector = Cursor;
        this->Query = WorldQuery;
        this->Dragger = WorldDragger;
    }

    void MousePicker::Deinitialize()
    {
        this->Selector = NULL;
        this->Query = NULL;
        this->Dragger = NULL;
    }

    Boole MousePicker::IsInitialized() const
        { return ( this->Selector != NULL ) && ( this->Query != NULL ) && ( this->Dragger != NULL ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    Input::Mouse* MousePicker::GetSelector() const
        { return this->Selector; }

    RayQuery* MousePicker::GetQuery() const
        { return this->Query; }

    MousePickDragger* MousePicker::GetDragger() const
        { return this->Dragger; }

    Ray MousePicker::GetMouseRay() const
    {
        Graphics::Viewport* HoveredViewport = this->Selector->GetHoveredViewport();
        Vector2 MousePos = this->Selector->GetViewportPosition();
        Ray MouseRay;
        if( HoveredViewport != NULL ) {
            MouseRay = Ray( HoveredViewport->GetCamera()->GetCameraToViewportRay(
                            MousePos.X / static_cast<Real>( HoveredViewport->GetActualWidth() ),
                            MousePos.Y / static_cast<Real>( HoveredViewport->GetActualHeight() ) ) );
        }
        return MouseRay;
    }

    World* MousePicker::GetMouseWorld() const
    {
        Graphics::Viewport* HoveredViewport = this->Selector->GetHoveredViewport();
        if( HoveredViewport != NULL ) {
            return HoveredViewport->GetCamera()->GetCreator()->GetWorld();
        }
        return NULL;
    }

    void MousePicker::Execute(const FilterDelegate& Filter)
    {
        Input::ButtonState LeftClickState = this->Selector->GetButtonState(1);
        if( LeftClickState >= Input::BUTTON_PRESSING ) {
            Ray MouseRay = this->GetMouseRay();
            if( LeftClickState == Input::BUTTON_PRESSING && !this->Dragger->IsDragging() ) {
                World* VisitingWorld = this->GetMouseWorld();
                if( VisitingWorld != NULL ) {
                    this->Query->SetWorld(VisitingWorld);
                    RayQueryHit Result = this->Query->GetFirstShapeResult(MouseRay);
                    if( Filter ? Result.IsDynamic() && Filter(Result) : Result.IsDynamic() ) {
                        this->Dragger->StartDragging(this->Dragger->GetBestProxy(Result.Object),Result.GetLocalHitLocation(),MouseRay);
                    }
                }
            }else if( LeftClickState == Input::BUTTON_DOWN ) {
                this->Dragger->ContinueDragging(MouseRay,this->Selector);
            }
        }else if( this->Dragger->IsDragging() ) {
            this->Dragger->StopDragging();
        }
    }
}//Mezzanine

#endif
