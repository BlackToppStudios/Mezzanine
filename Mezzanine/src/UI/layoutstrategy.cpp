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
#ifndef _uilayoutstrategy_cpp
#define _uilayoutstrategy_cpp

#include "UI/layoutstrategy.h"
#include "UI/widget.h"

#include "mathtool.h"
#include "exception.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        LayoutStrategy::LayoutStrategy()
            {  }

        LayoutStrategy::~LayoutStrategy()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Primary Methods

        void LayoutStrategy::Layout(const Rect& OldSelfRect, const Rect& NewSelfRect, const ChildContainer& ChildQuads)
        {
            Boolean QuadPositionUpdated = (OldSelfRect.Position != NewSelfRect.Position);
            Boolean QuadSizeUpdated = (OldSelfRect.Size != NewSelfRect.Size);
            for( ConstChildIterator ChildIt = ChildQuads.begin() ; ChildIt != ChildQuads.end() ; ++ChildIt )
            {
                QuadRenderable* Child = (*ChildIt);

                if( Child->GetManualTransformUpdates() )
                    continue;

                const Rect OldChildRect = Child->GetRect();
                Rect NewChildRect = OldChildRect;
                //NewChildRect.SetIdentity();

                if( QuadSizeUpdated ) {
                    NewChildRect.Size = this->HandleChildSizing(OldSelfRect,NewSelfRect,Child);
                }

                if( QuadSizeUpdated || QuadPositionUpdated ) {
                    NewChildRect.Position = this->HandleChildPositioning(OldSelfRect,NewSelfRect,NewChildRect.Size,Child);
                }

                Child->UpdateDimensions(OldChildRect,NewChildRect);
            }// for each child
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sub-task Methods

        Vector2 LayoutStrategy::HandleChildPositioning(const Rect& OldSelfRect, const Rect& NewSelfRect, const Vector2& NewChildSize, QuadRenderable* Child)
        {
            Vector2 NewPosition;
            const PositioningInfo& ChildPositioning = Child->GetPositioningPolicy();

            // Check for configurations that are just not doable
            if( ChildPositioning.PositionRules == UI::PF_Anchor_Prev_Offset && ChildPositioning.PositionRules == UI::PF_Anchor_Size )
                { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot use both the previous offset and quad size for the updated offset of a child quad.  They are mutually exclusive."); }

            // Resolve our position
            NewPosition.X = MathTools::Floor( this->HandleChildHorizontalPositioning(OldSelfRect,NewSelfRect,NewChildSize,Child) );
            NewPosition.Y = MathTools::Floor( this->HandleChildVerticalPositioning(OldSelfRect,NewSelfRect,NewChildSize,Child) );

            return NewPosition;
        }

        Real LayoutStrategy::HandleChildHorizontalPositioning(const Rect& OldSelfRect, const Rect& NewSelfRect, const Vector2& NewChildSize, QuadRenderable* Child)
        {
            const Real OldXPos = Child->GetActualPosition().X;
            const Real OldXSize = Child->GetActualSize().X;
            const PositioningInfo& ChildPositioning = Child->GetPositioningPolicy();
            // Rather than have a bunch of complicated checks to see what needs to be filled in, blindly assign Unified position
            // to the new rect, and allow simpler targetted checks fill in the exceptions
            Real Ret = NewSelfRect.Position.X + ChildPositioning.UPosition.X.CalculateActualDimension(NewSelfRect.Size.X);
            // Create a variable to store the old axis offset (the number of pixels to the left, or right if negative) on the X axis.
            Real Offset = 0;
            // Do our checks
            if( ChildPositioning.PositionRules & UI::PF_Anchor_Left && ChildPositioning.PositionRules & UI::PF_Anchor_Right ) { // Check if we're centered
                // Update the offset if we're configured to.
                if( ChildPositioning.PositionRules & UI::PF_Anchor_Prev_Offset ) {
                    // Get the old offset between the center of this quad and it's parent.
                    Offset = ( OldXPos + ( OldXSize * 0.5 ) ) - OldSelfRect.GetHorizontalCenter();
                }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Size ) {
                    // Use the new child size on the horizontal sxis.  Apply the Unified Dim on the X axis.
                    Offset = ChildPositioning.UPosition.X.CalculateActualDimension( NewChildSize.X );
                }
                // Then add that distance to the updated transform and then subtract the new half size.
                Ret = ( NewSelfRect.GetHorizontalCenter() + Offset ) - ( NewChildSize.X * 0.5 );
            }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Left ) { // Check if we're anchored to the left
                // Update the offset if we're configured to.
                if( ChildPositioning.PositionRules & UI::PF_Anchor_Prev_Offset ) {
                    // Get the old offset between the left edge of this quad and it's parent.
                    // Note: If the child is within the bounds of it's parent, this Offset should always be >= 0.
                    Offset = ( OldXPos - OldSelfRect.GetLeftEdge() );
                }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Size ) {
                    // Use the new child size on the horizontal sxis.  Apply the Unified Dim on the X axis.
                    Offset = ChildPositioning.UPosition.X.CalculateActualDimension( NewChildSize.X );
                }
                // Then add that distance to the updated transform.
                Ret = NewSelfRect.GetLeftEdge() + Offset;
            }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Right ) { // Check if we're anchored to the right
                // Update the offset if we're configured to.
                if( ChildPositioning.PositionRules & UI::PF_Anchor_Prev_Offset ) {
                    // Get the old offset between the right edge of this quad and it's parent.
                    // Note: If the child is within the bounds of it's parent, this Offset should always be <= 0.
                    Offset = ( ( OldXPos + OldXSize ) - OldSelfRect.GetRightEdge() );
                }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Size ) {
                    // Use the new child size on the horizontal sxis.  Apply the Unified Dim on the X axis.
                    Offset = ChildPositioning.UPosition.X.CalculateActualDimension( NewChildSize.X );
                }
                // Then add that distance to the updated transform.
                Ret = ( NewSelfRect.GetRightEdge() + Offset ) - NewChildSize.X;
            }
            return Ret;
        }

        Real LayoutStrategy::HandleChildVerticalPositioning(const Rect& OldSelfRect, const Rect& NewSelfRect, const Vector2& NewChildSize, QuadRenderable* Child)
        {
            const Real OldYPos = Child->GetActualPosition().Y;
            const Real OldYSize = Child->GetActualSize().Y;
            const PositioningInfo& ChildPositioning = Child->GetPositioningPolicy();
            // Rather than have a bunch of complicated checks to see what needs to be filled in, blindly assign Unified position
            // to the new rect, and allow simpler targetted checks fill in the exceptions
            Real Ret = NewSelfRect.Position.Y + ChildPositioning.UPosition.Y.CalculateActualDimension(NewSelfRect.Size.Y);
            // Create a variable to store the old axis offset (the number of pixels up, or down if negative) on the Y axis.
            Real Offset = 0;
            // Do our checks
            if( ChildPositioning.PositionRules & UI::PF_Anchor_Top && ChildPositioning.PositionRules & UI::PF_Anchor_Bottom ) { // Check if we're centered
                // Update the offset if we're configured to.
                if( ChildPositioning.PositionRules & UI::PF_Anchor_Prev_Offset ) {
                    // Get the old offset between the center of this quad and it's parent.
                    Offset = ( OldYPos + ( OldYSize * 0.5 ) ) - OldSelfRect.GetVerticalCenter();
                }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Size ) {
                    // Use the new child size on the vertical sxis.  Apply the Unified Dim on the Y axis.
                    Offset = ChildPositioning.UPosition.Y.CalculateActualDimension( NewChildSize.Y );
                }
                // Then add that distance to the updated transform and then subtract the new half size.
                Ret = ( NewSelfRect.GetVerticalCenter() + Offset ) - ( NewChildSize.Y * 0.5 );
            }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Top ) { // Check if we're anchored to the top
                // Update the offset if we're configured to.
                if( ChildPositioning.PositionRules & UI::PF_Anchor_Prev_Offset ) {
                    // Get the old offset between the top edge of this quad and it's parent.
                    // Note: If the child is within the bounds of it's parent, this Offset should always be >= 0.
                    Offset = ( OldYPos - OldSelfRect.GetTopEdge() );
                }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Size ) {
                    // Use the new child size on the vertical sxis.  Apply the Unified Dim on the X axis.
                    Offset = ChildPositioning.UPosition.Y.CalculateActualDimension( NewChildSize.Y );
                }
                // Then add that distance to the updated transform.
                Ret = NewSelfRect.GetTopEdge() + Offset;
            }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Bottom ) { // Check if we're anchored to the bottom
                // Update the offset if we're configured to.
                if( ChildPositioning.PositionRules & UI::PF_Anchor_Prev_Offset ) {
                    // Get the old offset between the bottom edge of this quad and it's parent.
                    // Note: If the child is within the bounds of it's parent, this Offset should always be <= 0.
                    Offset = ( ( OldYPos + OldYSize ) - OldSelfRect.GetBottomEdge() );
                }else if( ChildPositioning.PositionRules & UI::PF_Anchor_Size ) {
                    // Use the new child size on the vertical sxis.  Apply the Unified Dim on the Y axis.
                    Offset = ChildPositioning.UPosition.Y.CalculateActualDimension( NewChildSize.Y );
                }
                // Then add that distance to the updated transform.
                Ret = ( NewSelfRect.GetBottomEdge() + Offset ) - NewChildSize.Y;
            }
            return Ret;
        }

        Vector2 LayoutStrategy::HandleChildSizing(const Rect& OldSelfRect, const Rect& NewSelfRect, QuadRenderable* Child)
        {
            Vector2 NewSize;
            Vector2 OldSize = Child->GetActualSize();
            const SizingInfo& ChildSizing = Child->GetSizingPolicy();

            // Check for configurations that are just not doable
            if( ( ChildSizing.HorizontalRules == UI::SR_Match_Other_Axis || ChildSizing.HorizontalRules == UI::SR_Match_Other_Axis_Unified) &&
                ( ChildSizing.VerticalRules == UI::SR_Match_Other_Axis || ChildSizing.VerticalRules == UI::SR_Match_Other_Axis_Unified ) )
                { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Both axes of a SizingPolicy cannot attempt to match the other axis.  This creates a circular dependency."); }
            if( ChildSizing.HorizontalRules == UI::SR_Match_Other_Axis && ChildSizing.VerticalRules == UI::SR_Size_For_Text )
                { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot attempt to match vertical axis when it is sizing for text.  This creates a circular dependency."); }
            if( ChildSizing.HorizontalRules == UI::SR_Size_For_Text )
                { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot size for text on the horizontal axis."); }

            // In general, process the vertical dimension first.  This is because vertical resizing is likely to be less extreme compared to horizontal
            // in the case of transitioning from widescreen to standard viewing formats.  In general, horizontal is more likely to be the dependent axis.
            // That is of course unless Vertical is explicitly declared otherwise.  So check for that.
            if( ChildSizing.VerticalRules != UI::SR_Match_Other_Axis ) {
                // Vertical first
                NewSize.Y = MathTools::Ceil( this->HandleChildVerticalSizing(OldSelfRect,NewSelfRect,0,Child) );
                // Horizontal second
                NewSize.X = MathTools::Ceil( this->HandleChildHorizontalSizing(OldSelfRect,NewSelfRect,NewSize.Y,Child) );
            }else{
                // Horizontal first
                NewSize.X = MathTools::Ceil( this->HandleChildHorizontalSizing(OldSelfRect,NewSelfRect,0,Child) );
                // Vertical second
                NewSize.Y = MathTools::Ceil( this->HandleChildVerticalSizing(OldSelfRect,NewSelfRect,NewSize.X,Child) );
            }

            // Preserve the aspect ratio if we need to
            this->CheckChildAspectRatio(OldSize,NewSize,Child);

            // Clamp to our min and max sizes
            this->ClampChildToMinSize(NewSelfRect,NewSize,Child);
            this->ClampChildToMaxSize(NewSelfRect,NewSize,Child);

            return NewSize;
        }

        Real LayoutStrategy::HandleChildHorizontalSizing(const Rect& OldSelfRect, const Rect& NewSelfRect, const Real PrevAxisResult, QuadRenderable* Child)
        {
            const SizingInfo& ChildSizing = Child->GetSizingPolicy();
            Vector2 OldSize = Child->GetActualSize();
            // Horizontal second
            switch(ChildSizing.HorizontalRules)
            {
                case UI::SR_Unified_Dims:
                    { return ChildSizing.USize.X.CalculateActualDimension(NewSelfRect.Size.X);  break; }
                case UI::SR_Match_Other_Axis:
                    { return PrevAxisResult;                                                    break; }
                case UI::SR_Match_Other_Axis_Unified:
                    { return ChildSizing.USize.X.CalculateActualDimension(PrevAxisResult);      break; }
                case UI::SR_Fixed_Size:
                default:
                    { return OldSize.X;                                                         break; }
            }
        }

        Real LayoutStrategy::HandleChildVerticalSizing(const Rect& OldSelfRect, const Rect& NewSelfRect, const Real PrevAxisResult, QuadRenderable* Child)
        {
            const SizingInfo& ChildSizing = Child->GetSizingPolicy();
            Vector2 OldSize = Child->GetActualSize();
            // Vertical first
            switch(ChildSizing.VerticalRules)
            {
                case UI::SR_Unified_Dims:
                    { return ChildSizing.USize.Y.CalculateActualDimension(NewSelfRect.Size.Y);                         break; }
                case UI::SR_Match_Other_Axis:
                    { return PrevAxisResult;                                                                           break; }
                case UI::SR_Match_Other_Axis_Unified:
                    { return ChildSizing.USize.Y.CalculateActualDimension(PrevAxisResult);                             break; }
                case UI::SR_Size_For_Text:
                    { Child->PopulateTextLinesInLayers(PrevAxisResult);  return Child->GetIdealHeightForText() + 2.0;  break; }
                case UI::SR_Fixed_Size:
                default:
                    { return OldSize.Y;                                                                                break; }
            }
        }

        void LayoutStrategy::CheckChildAspectRatio(const Vector2& OldChildSize, Vector2& NewChildSize, QuadRenderable* Child)
        {
            const SizingInfo& ChildSizing = Child->GetSizingPolicy();
            // Do we care about aspect ratio?
            // Did we used to have a size?
            // And did the aspect ratio change?
            if( ChildSizing.RatioLock != UI::ARL_Ratio_Unlocked &&
                ( OldChildSize.X > 0.0 && OldChildSize.Y > 0.0) &&
                !MathTools::WithinTolerance( NewChildSize.X / NewChildSize.Y, OldChildSize.X / OldChildSize.Y, 0.01 ) )
            {
                Real XChange = NewChildSize.X / OldChildSize.X;
                Real YChange = NewChildSize.Y / OldChildSize.Y;

                if( ChildSizing.RatioLock != UI::ARL_Ratio_Y_Axis ) {
                    if( XChange > YChange ) {
                        if( ChildSizing.RatioLock == UI::ARL_Ratio_Locked_Expanding )
                            NewChildSize.Y = OldChildSize.Y * XChange;
                        else if( ChildSizing.RatioLock == UI::ARL_Ratio_Locked_Shrinking )
                            NewChildSize.X = OldChildSize.X * YChange;
                    }else if( YChange > XChange ) {
                        if( ChildSizing.RatioLock == UI::ARL_Ratio_Locked_Expanding )
                            NewChildSize.X = OldChildSize.X * YChange;
                        else if( ChildSizing.RatioLock == UI::ARL_Ratio_Locked_Shrinking )
                            NewChildSize.Y = OldChildSize.Y * XChange;
                    }
                }else{
                    NewChildSize.X = OldChildSize.X * YChange;
                }
            }
        }

        void LayoutStrategy::ClampChildToMinSize(const Rect& NewSelfRect, Vector2& NewChildSize, QuadRenderable* Child)
        {
            const SizingInfo& ChildSizing = Child->GetSizingPolicy();
            if( ChildSizing.MinSize.X.Rel != 0 && ChildSizing.MinSize.X.Abs != 0 &&
                ChildSizing.MinSize.Y.Rel != 0 && ChildSizing.MinSize.Y.Abs != 0 )
            {
                Vector2 TempVec = ChildSizing.MinSize.CalculateActualDimensions(NewSelfRect.Size);
                NewChildSize.X = std::max(NewChildSize.X,TempVec.X);
                NewChildSize.Y = std::max(NewChildSize.Y,TempVec.Y);
            }
        }

        void LayoutStrategy::ClampChildToMaxSize(const Rect& NewSelfRect, Vector2& NewChildSize, QuadRenderable* Child)
        {
            const SizingInfo& ChildSizing = Child->GetSizingPolicy();
            if( ChildSizing.MaxSize.X.Rel != 0 && ChildSizing.MaxSize.X.Abs != 0 &&
                ChildSizing.MaxSize.Y.Rel != 0 && ChildSizing.MaxSize.Y.Abs != 0 )
            {
                Vector2 TempVec = ChildSizing.MaxSize.CalculateActualDimensions(NewSelfRect.Size);
                NewChildSize.X = std::min(NewChildSize.X,TempVec.X);
                NewChildSize.Y = std::min(NewChildSize.Y,TempVec.Y);
            }
        }
    }//UI
}//Mezzanine

#endif
