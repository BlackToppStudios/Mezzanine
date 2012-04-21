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
#ifndef _uiviewportupdatetool_cpp
#define _uiviewportupdatetool_cpp

#include "uiviewportupdatetool.h"
#include "uirectangle.h"
#include "uiwidget.h"

namespace Mezzanine
{
    namespace UI
    {
        Vector2 ViewportUpdateTool::OldSize = Vector2(0,0);
        Vector2 ViewportUpdateTool::NewSize = Vector2(0,0);
        Real ViewportUpdateTool::MiddleTolerance = 0.5;
        ViewportUpdateTool::ResizeMode ViewportUpdateTool::RSM = RSM_Scale_By_Height;
        ViewportUpdateTool::RepositionMode ViewportUpdateTool::RPM = RPM_Scale_Quadrents_By_Height;

        ViewportUpdateTool::ViewportUpdateTool()
        {
        }

        ViewportUpdateTool::~ViewportUpdateTool()
        {
        }

        Vector2 ViewportUpdateTool::RSM_ScaleAll(const Vector2& RendSize)
        {
            Vector2 RelSize = RendSize / OldSize;
            return (RelSize * NewSize);
        }

        Vector2 ViewportUpdateTool::RSM_ScaleByHeight(const Vector2& RendSize)
        {
            Vector2 NewRendSize;
            //Vector2 RelSize = RenderableSize / OldSize;
            Real Scale = NewSize.Y / OldSize.Y;
            NewRendSize.X = RendSize.X * Scale;
            NewRendSize.Y = RendSize.Y * Scale;
            return NewRendSize;
        }

        Vector2 ViewportUpdateTool::RPM_ScaleAll(const Vector2& RendPosition)
        {
            Vector2 RelPos = RendPosition / OldSize;
            return (RelPos * NewSize);
        }

        Vector2 ViewportUpdateTool::RPM_ScaleQuadrents(const Vector2& RendPosition, const Vector2& RendSize, const Vector2& UpdatedRendSize)
        {
            Vector2 NewRendPos;
            Vector2 RelPos = RendPosition / OldSize;
            Vector2 Middle;
            Middle.X = RendPosition.X + (RendSize.X * 0.5);
            Middle.Y = RendPosition.Y + (RendSize.Y * 0.5);

            bool Left = (Middle.X < (OldSize.X - (MiddleTolerance * OldSize.X)) * 0.5);
            bool Right = (Middle.X > (OldSize.X + (MiddleTolerance * OldSize.X)) * 0.5);
            bool Top = (Middle.Y < (OldSize.Y - (MiddleTolerance * OldSize.Y)) * 0.5);
            bool Bottom = (Middle.Y > (OldSize.Y + (MiddleTolerance * OldSize.Y)) * 0.5);

            if(Top)
            {
                Real RelDist = RelPos.Y;
                NewRendPos.Y = RelDist * NewSize.Y;
            }else if(Bottom){
                Real RelDist = (OldSize.Y - (RendPosition.Y + RendSize.Y)) / OldSize.Y;
                NewRendPos.Y = (NewSize.Y - (NewSize.Y * RelDist)) - UpdatedRendSize.Y;
            }else{
                Real RelMiddle = Middle.Y / OldSize.Y;
                NewRendPos.Y = (RelMiddle * NewSize.Y) - (UpdatedRendSize.Y * 0.5);
            }

            if(Left)
            {
                Real RelDist = RelPos.X;
                NewRendPos.X = RelDist * NewSize.X;
            }else if(Right){
                Real RelDist = (OldSize.X - (RendPosition.X + RendSize.X)) / OldSize.X;
                NewRendPos.X = (NewSize.X - (NewSize.X * RelDist)) - UpdatedRendSize.X;
            }else{
                Real RelMiddle = Middle.X / OldSize.X;
                NewRendPos.X = (RelMiddle * NewSize.X) - (UpdatedRendSize.X * 0.5);
            }

            return NewRendPos;
        }

        Vector2 ViewportUpdateTool::RPM_ScaleQuadrentsByHeight(const Vector2& RendPosition, const Vector2& RendSize, const Vector2& UpdatedRendSize)
        {
            Vector2 NewRendPos;
            Vector2 RelPos = RendPosition / OldSize;
            Vector2 Middle;
            Middle.X = RendPosition.X + (RendSize.X * 0.5);
            Middle.Y = RendPosition.Y + (RendSize.Y * 0.5);

            bool Left = (Middle.X < (OldSize.X - (MiddleTolerance * OldSize.X)) * 0.5);
            bool Right = (Middle.X > (OldSize.X + (MiddleTolerance * OldSize.X)) * 0.5);
            bool Top = (Middle.Y < (OldSize.Y - (MiddleTolerance * OldSize.Y)) * 0.5);
            bool Bottom = (Middle.Y > (OldSize.Y + (MiddleTolerance * OldSize.Y)) * 0.5);

            Real Scale = NewSize.Y / OldSize.Y;

            if(Top)
            {
                Real RelDist = RelPos.Y;
                NewRendPos.Y = RelDist * NewSize.Y;
            }else if(Bottom){
                Real RelDist = (OldSize.Y - (RendPosition.Y + RendSize.Y)) / OldSize.Y;
                NewRendPos.Y = (NewSize.Y - (NewSize.Y * RelDist)) - UpdatedRendSize.Y;
            }else{
                Real RelMiddle = Middle.Y / OldSize.Y;
                NewRendPos.Y = (RelMiddle * NewSize.Y) - (UpdatedRendSize.Y * 0.5);
            }

            if(Left)
            {
                NewRendPos.X = RendPosition.X * Scale;
            }else if(Right){
                Real Dist = OldSize.X - (RendPosition.X + RendSize.X);
                NewRendPos.X = (NewSize.X - (Dist * Scale)) - UpdatedRendSize.X;
            }else{
                Real ScreenMiddle = OldSize.X * 0.5;
                if(Middle.X < ScreenMiddle - 1)  //left
                {
                    Real MidDiff = ((RendPosition.X + RendSize.X) - ScreenMiddle) * Scale;
                    NewRendPos.X = ((NewSize.X * 0.5) + MidDiff) - UpdatedRendSize.X;
                }else if(Middle.X > ScreenMiddle + 1){  //right
                    Real MidDiff = (RendPosition.X - ScreenMiddle) * Scale;
                    NewRendPos.X = (NewSize.X * 0.5) + MidDiff;
                }else{
                    Real RelMiddle = Middle.X / OldSize.X;
                    NewRendPos.X = (RelMiddle * NewSize.X) - (UpdatedRendSize.X * 0.5);
                }
            }

            return NewRendPos;
        }

        Vector2 ViewportUpdateTool::RPM_AbsoluteQuadrents(const Vector2& RendPosition, const Vector2& RendSize, const Vector2& UpdatedRendSize)
        {
            Vector2 NewRendPos;
            Vector2 RelPos = RendPosition / OldSize;
            Vector2 Middle;
            Middle.X = RendPosition.X + (RendSize.X * 0.5);
            Middle.Y = RendPosition.Y + (RendSize.Y * 0.5);

            bool Left = (Middle.X < (OldSize.X - (MiddleTolerance * OldSize.X)) * 0.5);
            bool Right = (Middle.X > (OldSize.X + (MiddleTolerance * OldSize.X)) * 0.5);
            bool Top = (Middle.Y < (OldSize.Y - (MiddleTolerance * OldSize.Y)) * 0.5);
            bool Bottom = (Middle.Y > (OldSize.Y + (MiddleTolerance * OldSize.Y)) * 0.5);

            if(Top)
            {
                NewRendPos.Y = RendPosition.Y;
            }else if(Bottom){
                NewRendPos.Y = (NewSize.Y - (RendPosition.Y + RendSize.Y)) - UpdatedRendSize.Y;
            }else{
                Real RelMiddle = Middle.Y / OldSize.Y;
                NewRendPos.Y = (RelMiddle * NewSize.Y) - (UpdatedRendSize.Y * 0.5);
            }

            if(Left)
            {
                NewRendPos.X = RendPosition.X;
            }else if(Right){
                NewRendPos.X = (NewSize.X - (RendPosition.X + RendSize.X)) - UpdatedRendSize.X;
            }else{
                Real RelMiddle = Middle.X / OldSize.X;
                NewRendPos.X = (RelMiddle * NewSize.X) - (UpdatedRendSize.X * 0.5);
            }

            return NewRendPos;
        }

        Vector2 ViewportUpdateTool::GetUpdatedSize(const Vector2& RendSize)
        {
            Vector2 NewRendSize;

            // Find the new size
            switch(RSM)
            {
                case RSM_Scale_All:
                    NewRendSize = RSM_ScaleAll(RendSize);
                    break;
                case RSM_Scale_By_Height:
                    NewRendSize = RSM_ScaleByHeight(RendSize);
                    break;
                case RSM_Nothing:
                default:
                    NewRendSize = RendSize;
                    break;
            }

            return NewRendSize;
        }

        Vector2 ViewportUpdateTool::GetUpdatedPosition(const Vector2& RendPosition, const Vector2& RendSize, const Vector2& UpdatedRendSize)
        {
            Vector2 NewPosition;

            // Find the new position
            switch(RPM)
            {
                case RPM_Scale_All:
                    NewPosition = RPM_ScaleAll(RendPosition);
                    break;
                case RPM_Scale_Quadrents:
                    NewPosition = RPM_ScaleQuadrents(RendPosition,RendSize,UpdatedRendSize);
                    break;
                case RPM_Scale_Quadrents_By_Height:
                    NewPosition = RPM_ScaleQuadrentsByHeight(RendPosition,RendSize,UpdatedRendSize);
                    break;
                case RPM_Absolute_Quadrents:
                    NewPosition = RPM_AbsoluteQuadrents(RendPosition,RendSize,UpdatedRendSize);
                    break;
                case RPM_Nothing:
                default:
                    NewPosition = RendPosition;
                    break;
            }

            return NewPosition;
        }

        void ViewportUpdateTool::ConfigureSizes(const Vector2& OldVSize, const Vector2& NewVSize)
        {
            OldSize = OldVSize;
            NewSize = NewVSize;
        }

        void ViewportUpdateTool::ConfigureModes(const ResizeMode& SizeMode, const RepositionMode& PositionMode)
        {
            RSM = SizeMode;
            RPM = PositionMode;
        }

        void ViewportUpdateTool::ConfigureMiddleSize(const Real& MidSize)
        {
            MiddleTolerance = MidSize;
        }

        const Vector2& ViewportUpdateTool::GetOldSize()
        {
            return OldSize;
        }

        const Vector2& ViewportUpdateTool::GetNewSize()
        {
            return NewSize;
        }

        const Real& ViewportUpdateTool::GetMiddleSize()
        {
            return MiddleTolerance;
        }

        const ViewportUpdateTool::ResizeMode& ViewportUpdateTool::GetResizeMode()
        {
            return RSM;
        }

        const ViewportUpdateTool::RepositionMode& ViewportUpdateTool::GetRepositionMode()
        {
            return RPM;
        }


        void ViewportUpdateTool::UpdateRectangleRenderable(Rectangle* ToBeUpdated)
        {
            // Setup the data to be used
            Vector2 RendActSize = ToBeUpdated->GetActualSize();
            Vector2 RendActPos = ToBeUpdated->GetActualPosition();

            // Get the updated data
            Vector2 NewRendSize = GetUpdatedSize(RendActSize);
            Vector2 NewPosition = GetUpdatedPosition(RendActPos,RendActSize,NewRendSize);

            // Now apply the new values
            ToBeUpdated->SetActualSize(NewRendSize);
            ToBeUpdated->SetActualPosition(NewPosition);
        }

        WidgetResult ViewportUpdateTool::UpdateWidget(Widget* ToBeUpdated)
        {
            // Setup the data to be used
            Vector2 WidActSize = ToBeUpdated->GetSize() * OldSize;
            Vector2 WidActPos = ToBeUpdated->GetPosition() * OldSize;

            // Get the updated data
            Vector2 NewSize = GetUpdatedSize(WidActSize);
            Vector2 NewPosition = GetUpdatedPosition(WidActPos,WidActSize,NewSize);

            // Now apply the new values
            //ToBeUpdated->SetActualSize(NewSize);
            //ToBeUpdated->SetActualPosition(NewPosition);
            return WidgetResult(NewPosition,NewSize);
        }
    }//UI
}//Mezzanine

#endif
