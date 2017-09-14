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
#ifndef _uiverticallayoutstrategy_cpp
#define _uiverticallayoutstrategy_cpp

#include "UI/verticallayoutstrategy.h"
#include "UI/widget.h"
#include "MathTools/mathtools.h"
#include "exception.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        VerticalLayoutStrategy::VerticalLayoutStrategy()
            {  }

        VerticalLayoutStrategy::~VerticalLayoutStrategy()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void VerticalLayoutStrategy::Layout(const Rect& OldSelfRect, const Rect& NewSelfRect, const ChildContainer& ChildQuads)
        {
            // See what needs updating
            //bool QuadPositionUpdated = (OldSelfRect.Position != NewSelfRect.Position);
            //bool QuadSizeUpdated = (OldSelfRect.Size != NewSelfRect.Size);

            // Setup our persistent loop data
            Real PrevBottomPos = NewSelfRect.Position.Y;
            Real NextTopPos = 0;
            ConstChildIterator ChildIt = ChildQuads.begin();
            while( ChildIt != ChildQuads.end() )
            {
                // Scan ahead for the next non-expander
                UInt32 ExpandingChildCount = 0;
                ConstChildIterator NextNonExpandingChild = ChildIt;
                while( NextNonExpandingChild != ChildQuads.end() && (*NextNonExpandingChild)->GetSizingPolicy().CanExpandVertically() )
                {
                    ++NextNonExpandingChild;
                    ++ExpandingChildCount;
                }

                // if we're on top of a non-expanding child, such as starting with one or have two in a row
                if( ExpandingChildCount == 0 ) {
                    // Do our updates and move on with the loop
                    QuadRenderable* NoExChild = (*NextNonExpandingChild);
                    if( !NoExChild->GetManualTransformUpdates() ) {
                        const Rect OldChildRect = NoExChild->GetRect();
                        Rect NewChildRect;

                        NewChildRect.Size = this->HandleChildSizing(OldSelfRect,NewSelfRect,NoExChild);
                        NewChildRect.Position = this->HandleChildPositioning(OldSelfRect,NewSelfRect,NewChildRect.Size,NoExChild);
                        NoExChild->UpdateDimensions(OldChildRect,NewChildRect);
                        PrevBottomPos = NewChildRect.Position.Y + NewChildRect.Size.Y;
                    }

                    // Increment if it's not the end node and move on
                    if( NextNonExpandingChild != ChildQuads.end() )
                        ++NextNonExpandingChild;
                    ChildIt = NextNonExpandingChild;
                    continue;
                }else{
                    // Create a variable to cache the bottom position
                    Real FixedBottomPos = 0;

                    // We got a range now so lets process the "fixed" child first if it is valid
                    if( NextNonExpandingChild != ChildQuads.end() ) {
                        QuadRenderable* NoExChild = (*NextNonExpandingChild);
                        if( !NoExChild->GetManualTransformUpdates() ) {
                            const Rect OldChildRect = NoExChild->GetRect();
                            Rect NewChildRect;

                            NewChildRect.Size = this->HandleChildSizing(OldSelfRect,NewSelfRect,NoExChild);
                            NewChildRect.Position = this->HandleChildPositioning(OldSelfRect,NewSelfRect,NewChildRect.Size,NoExChild);

                            this->CheckChildAspectRatio(OldChildRect.Size,NewChildRect.Size,NoExChild);
                            this->ClampChildToMinSize(NewSelfRect,NewChildRect.Size,NoExChild);
                            this->ClampChildToMaxSize(NewSelfRect,NewChildRect.Size,NoExChild);

                            NoExChild->UpdateDimensions(OldChildRect,NewChildRect);
                            NextTopPos = NewChildRect.Position.Y;
                            FixedBottomPos = NewChildRect.Position.Y + NewChildRect.Size.Y;
                        }
                    }else{
                        // Just use the bottom edge of the parent if the child isn't valid
                        NextTopPos = NewSelfRect.Position.Y + NewSelfRect.Size.Y;
                    }

                    // Set up the data for the range of expanding children
                    Real YPos = PrevBottomPos;
                    Real YSpacePerChild = (NextTopPos - PrevBottomPos) / ExpandingChildCount;
                    // Update the expanding children in the range
                    while( ChildIt != NextNonExpandingChild )
                    {
                        QuadRenderable* ExChild = (*ChildIt);
                        if( !ExChild->GetManualTransformUpdates() ) {
                            const Rect OldChildRect = ExChild->GetRect();
                            Rect NewChildRect;

                            NewChildRect.Size.X = ( ExChild->GetHorizontalSizingRules() == UI::SR_Fill_Available ? NewSelfRect.Size.Y : this->HandleChildHorizontalSizing(OldSelfRect,NewSelfRect,YSpacePerChild,ExChild) );
                            NewChildRect.Size.Y = YSpacePerChild;
                            NewChildRect.Position.X = this->HandleChildHorizontalPositioning(OldSelfRect,NewSelfRect,NewChildRect.Size,ExChild);
                            NewChildRect.Position.Y = YPos;

                            this->CheckChildAspectRatio(OldChildRect.Size,NewChildRect.Size,ExChild);
                            this->ClampChildToMinSize(NewSelfRect,NewChildRect.Size,ExChild);
                            this->ClampChildToMaxSize(NewSelfRect,NewChildRect.Size,ExChild);

                            ExChild->UpdateDimensions(OldChildRect,NewChildRect);
                        }
                        ++ChildIt;
                    }
                    PrevBottomPos = FixedBottomPos;
                }
                // We handled the object at the end of the range, so increment passed it and move on unless it is the end node
                if( NextNonExpandingChild != ChildQuads.end() )
                    ++NextNonExpandingChild;
                ChildIt = NextNonExpandingChild;
            }// for each child
        }
    }//UI
}//Mezzanine

#endif
