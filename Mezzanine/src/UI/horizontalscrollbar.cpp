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
#ifndef _uihorizontalscrollbar_cpp
#define _uihorizontalscrollbar_cpp

#include "UI/horizontalscrollbar.h"
#include "UI/button.h"
#include "UI/screen.h"
#include "UI/pagedcontainer.h"
#include "UI/uimanager.h"

#include "UI/horizontallayoutstrategy.h"

#include "stringtool.h"
#include "MathTools/mathtools.h"
#include "Input/metacode.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // HorizontalScrollbar Methods
        ///////////////////////////////////////

        const String HorizontalScrollbar::TypeName = "HorizontalScrollbar";

        HorizontalScrollbar::HorizontalScrollbar(Screen* Parent) :
            Scrollbar(Parent)
            {  }

        HorizontalScrollbar::HorizontalScrollbar(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Scrollbar(RendName,Parent)
            { this->ConstructHorizontalScrollbar(Style); }

        HorizontalScrollbar::HorizontalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Scrollbar(RendName,RendRect,Parent)
            { this->ConstructHorizontalScrollbar(Style); }

        HorizontalScrollbar::HorizontalScrollbar(const XML::Node& XMLNode, Screen* Parent) :
            Scrollbar(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        HorizontalScrollbar::~HorizontalScrollbar()
        {
            if( this->Container != NULL ) {
                this->Container->UnbindProvider(this);
            }

            this->RemoveChild( this->Scroller );
            this->ParentScreen->DestroyWidget( this->Scroller );
            this->RemoveChild( this->ScrollBack );
            this->ParentScreen->DestroyWidget( this->ScrollBack );
            this->RemoveChild( this->UpLeftButton );
            this->ParentScreen->DestroyWidget( this->UpLeftButton );
            this->RemoveChild( this->DownRightButton );
            this->ParentScreen->DestroyWidget( this->DownRightButton );
        }

        void HorizontalScrollbar::ConstructHorizontalScrollbar(const UI::ScrollbarStyle& ScrollStyle)
        {
            // Create the rects we'll use
            UnifiedVec2 ChildPosition(0,0,0,0);
            UnifiedVec2 ChildSize(1,1,0,0);
            UnifiedRect ChildRect(ChildPosition,ChildSize);
            if( UI::SB_NoButtons == ScrollStyle )
            {
                // Create our objects
                this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                this->ScrollBack->SetHorizontalSizingRules(UI::SR_Fill_Available);
                this->ScrollBack->SetVerticalSizingRules(UI::SR_Unified_Dims);
                this->AddChild(this->ScrollBack,0);

                this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                this->Scroller->SetManualTransformUpdates(true);
                this->AddChild(this->Scroller,1);
            }else{
                //Get the position for all items involved and configure their offsets
                if(UI::SB_Separate == ScrollStyle)
                {
                    // Create our objects
                    this->UpLeftButton = ParentScreen->CreateButton(this->Name+".LeftButton",ChildRect);
                    this->UpLeftButton->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->UpLeftButton->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->UpLeftButton->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->UpLeftButton,0);

                    this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                    this->ScrollBack->SetHorizontalSizingRules(UI::SR_Fill_Available);
                    this->ScrollBack->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->ScrollBack,1);

                    this->DownRightButton = ParentScreen->CreateButton(this->Name+".RightButton",ChildRect);
                    this->DownRightButton->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
                    this->DownRightButton->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->DownRightButton->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->DownRightButton,2);

                    this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                    this->Scroller->SetManualTransformUpdates(true);
                    this->AddChild(this->Scroller,3);
                }
                else if(UI::SB_TogetherDownRight == ScrollStyle)
                {
                    // Create our objects
                    this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                    this->ScrollBack->SetHorizontalSizingRules(UI::SR_Fill_Available);
                    this->ScrollBack->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->ScrollBack,0);

                    this->UpLeftButton = ParentScreen->CreateButton(this->Name+".LeftButton",UnifiedRect(UnifiedVec2(-1,-1,0,0),ChildSize));
                    this->UpLeftButton->SetHorizontalPositioningRules(UI::PF_Anchor_Right | UI::PF_Anchor_SelfSize);
                    this->UpLeftButton->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->UpLeftButton->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->UpLeftButton,1);

                    this->DownRightButton = ParentScreen->CreateButton(this->Name+".RightButton",ChildRect);
                    this->DownRightButton->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
                    this->DownRightButton->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->DownRightButton->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->DownRightButton,2);

                    this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                    this->Scroller->SetManualTransformUpdates(true);
                    this->AddChild(this->Scroller,3);
                }
                else if(UI::SB_TogetherUpLeft == ScrollStyle)
                {
                    // Create our objects
                    this->UpLeftButton = ParentScreen->CreateButton(this->Name+".LeftButton",ChildRect);
                    this->UpLeftButton->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->UpLeftButton->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->UpLeftButton->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->UpLeftButton,0);

                    this->DownRightButton = ParentScreen->CreateButton(this->Name+".RightButton",UnifiedRect(UnifiedVec2(1,1,0,0),ChildSize));
                    this->DownRightButton->SetHorizontalPositioningRules(UI::PF_Anchor_Left | UI::PF_Anchor_SelfSize);
                    this->DownRightButton->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->DownRightButton->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->DownRightButton,1);

                    this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                    this->ScrollBack->SetHorizontalSizingRules(UI::SR_Fill_Available);
                    this->ScrollBack->SetVerticalSizingRules(UI::SR_Unified_Dims);
                    this->AddChild(this->ScrollBack,2);

                    this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                    this->Scroller->SetManualTransformUpdates(true);
                    this->AddChild(this->Scroller,3);
                }
            }

            Rect TempDims;
            if( this->ActDims != TempDims ) {
                TempDims = this->ActDims;
                this->UpdateDimensions(TempDims,TempDims);
            }

            this->SubscribeToChildEvents();
        }

        Real HorizontalScrollbar::GetUpperScrollLimit() const
        {
            return this->ScrollBack->GetActualPosition().X + this->ScrollBack->GetActualSize().X;
        }

        Real HorizontalScrollbar::GetLowerScrollLimit() const
        {
            return this->ScrollBack->GetActualPosition().X;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& HorizontalScrollbar::GetTypeName() const
        {
            return HorizontalScrollbar::TypeName;
        }

        void HorizontalScrollbar::SetScrollerValue(const Real& Value)
        {
            if(Value > 1 || Value < 0)
                return;

            const Rect ScrollBackRect = this->ScrollBack->GetRect();
            const Rect OldScrollerRect = this->Scroller->GetRect();
            Rect NewScrollerRect;

            NewScrollerRect.Size.X = OldScrollerRect.Size.X;
            NewScrollerRect.Size.Y = OldScrollerRect.Size.Y;
            NewScrollerRect.Position.X = ScrollBackRect.Position.X + ( ( this->GetScrollRange() - NewScrollerRect.Size.X ) * Value );
            NewScrollerRect.Position.Y = ScrollBackRect.Position.Y;

            this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);
        }

        Real HorizontalScrollbar::GetScrollerValue() const
        {
            Real RelPos = this->Scroller->GetActualPosition().X - this->GetLowerScrollLimit();
            Real RelLimit = this->GetScrollRange() - this->Scroller->GetActualSize().X;
            return ( RelLimit > 0 ? RelPos / RelLimit : 0 );
        }

        void HorizontalScrollbar::SetScrollerSize(const Real& Size)
        {
            if(Size > 1 || Size < 0)
                return;

            this->ScrollerSize = Size;
            const Rect ScrollBackRect = this->ScrollBack->GetRect();
            const Rect OldScrollerRect = this->Scroller->GetRect();
            Rect NewScrollerRect;

            Real ScrollValue = this->GetScrollerValue();
            NewScrollerRect.Size.X = ScrollBackRect.Size.X * Size;
            NewScrollerRect.Size.Y = ScrollBackRect.Size.Y;
            NewScrollerRect.Position.X = ScrollBackRect.Position.X + ( ( this->GetScrollRange() - NewScrollerRect.Size.X ) * ScrollValue );
            NewScrollerRect.Position.Y = ScrollBackRect.Position.Y;

            this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);
        }

        Real HorizontalScrollbar::GetScrollerSize() const
        {
            //const Real ScrollBackX = this->ScrollBack->GetActualSize().X;
            //return ( ScrollBackX > 0 ? this->Scroller->GetActualSize().X / ScrollBackX : 0 );
            return ScrollerSize;
        }

        void HorizontalScrollbar::UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Update the personal data first
            this->ActDims = NewSelfRect;

            // Get the data regarding the scroller we'll need after everything else is updated
            const Real ScrollValue = this->GetScrollerValue();
            const Real ScrollSize = this->GetScrollerSize();
            const Rect OldScrollerRect = this->Scroller->GetRect();

            // Update the children
            this->LayoutStrat->Layout(OldSelfRect,NewSelfRect,this->ChildWidgets);

            // Next prepare the new rect for the scroller
            const Rect ScrollBackRect = this->ScrollBack->GetRect();
            Rect NewScrollerRect;
            NewScrollerRect.Size.X = ScrollBackRect.Size.X * ScrollSize;
            NewScrollerRect.Size.Y = ScrollBackRect.Size.Y;
            NewScrollerRect.Position.X = ScrollBackRect.Position.X + ( ( this->GetScrollRange() - NewScrollerRect.Size.Y ) * ScrollValue );
            NewScrollerRect.Position.Y = ScrollBackRect.Position.Y;

            // Finally update the scroller
            this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

            // We done got icky
            this->_MarkAllLayersDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        void HorizontalScrollbar::SetVisible(Boole CanSee)
        {
            if( this->AutoHideScroll && CanSee ) {
                if( this->GetMaxXPages() > 1.0 ) {
                    this->Widget::SetVisible(CanSee);
                }
            }else{
                this->Widget::SetVisible(CanSee);
            }
        }

        void HorizontalScrollbar::Show()
        {
            if( this->AutoHideScroll ) {
                if( this->GetMaxXPages() > 1.0 ) {
                    this->Widget::Show();
                }
            }else{
                this->Widget::Show();
            }
        }

        void HorizontalScrollbar::Hide()
        {
            this->Widget::Hide();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // PageProvider Methods

        Real HorizontalScrollbar::GetMaxXPages() const
        {
            if( this->Container != NULL ) {
                const Real ContainerXSize = this->Container->GetActualSize().X;
                if( ContainerXSize > 0 ) {
                    Real Ret = MathTools::Ceil( this->Container->GetWorkAreaSize().X / ContainerXSize );
                    return ( Ret > 1 ? Ret : 1 );
                }
            }
            return 1;
        }

        Real HorizontalScrollbar::GetMaxYPages() const
            { return 1; }

        Real HorizontalScrollbar::GetCurrentXPage() const
            { return ( (this->GetMaxXPages() - 1) * this->GetScrollerValue() ) + 1; }

        Real HorizontalScrollbar::GetCurrentYPage() const
            { return 1; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void HorizontalScrollbar::ProtoSerializeChildQuads(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeChildQuads(SelfRoot);
        }

        void HorizontalScrollbar::ProtoDeSerializeChildQuads(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeChildQuads(SelfRoot);

            // Assign the ScrollBack
            this->ScrollBack = static_cast<Button*>( this->GetChild(this->Name+".ScrollBack") );
            if( this->ScrollBack == NULL ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"ScrollBack button not found after HorizontalScrollbar deserialization.");
            }

            // Assign the Scroller
            this->Scroller = static_cast<Button*>( this->GetChild(this->Name+".Scroller") );
            if( this->Scroller == NULL ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Scroller button not found after HorizontalScrollbar deserialization.");
            }

            if( this->GetNumChildren() > 2 )
            {
                // Assign the LeftButton
                this->UpLeftButton = static_cast<Button*>( this->GetChild(this->Name+".LeftButton") );
                if( this->UpLeftButton == NULL ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"LeftButton button not found after HorizontalScrollbar deserialization.");
                }

                // Assign the RightButton
                this->DownRightButton = static_cast<Button*>( this->GetChild(this->Name+".RightButton") );
                if( this->DownRightButton == NULL ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"RightButton button not found after HorizontalScrollbar deserialization.");
                }
            }

            this->SubscribeToChildEvents();
        }

        String HorizontalScrollbar::GetSerializableName()
            { return HorizontalScrollbar::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void HorizontalScrollbar::_NotifyContainerUpdated()
        {
            if( this->Container != NULL ) {
                // Update the scroller size
                Real XView = this->Container->GetActualSize().X;
                Real XWork = this->Container->GetWorkAreaSize().X;
                if( XWork > 0 ) {
                    this->SetScrollerSize( XView / XWork );
                }else{
                    this->SetScrollerSize( 1.0 );
                }

                // AutoHide check
                /*if( this->AutoHideScroll ) {
                    if( this->GetMaxXPages() <= 1.0 ) {
                        this->Hide();
                    }else{
                        this->Show();
                    }
                }// */
            }
        }

        Boole HorizontalScrollbar::_MouseScroll(const Vector2& MouseDelta)
        {
            if( this->ChildLock == this->Scroller && MouseDelta.X != 0 )
            {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                NewScrollerRect.Position.X = OldScrollerRect.Position.X + MouseDelta.X;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;

                NewScrollerRect.Position.X = std::min(NewScrollerRect.Position.X,this->GetUpperScrollLimit() - OldScrollerRect.Size.X);
                NewScrollerRect.Position.X = std::max(NewScrollerRect.Position.X,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }
            return false;
        }

        Boole HorizontalScrollbar::_MouseWheelScroll(const Integer Direction)
        {
            if( Direction == Input::DIRECTIONALMOTION_UPLEFT ) {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                Real ScrollDist = -(this->ScrollBack->GetActualSize().X * this->IncrementDistance);
                NewScrollerRect.Position.X = OldScrollerRect.Position.X + ScrollDist;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;

                NewScrollerRect.Position.X = std::min(NewScrollerRect.Position.X,this->GetUpperScrollLimit() - OldScrollerRect.Size.X);
                NewScrollerRect.Position.X = std::max(NewScrollerRect.Position.X,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }else if( Direction == Input::DIRECTIONALMOTION_DOWNRIGHT ) {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                Real ScrollDist = this->ScrollBack->GetActualSize().X * this->IncrementDistance;
                NewScrollerRect.Position.X = OldScrollerRect.Position.X + ScrollDist;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;

                NewScrollerRect.Position.X = std::min(NewScrollerRect.Position.X,this->GetUpperScrollLimit() - OldScrollerRect.Size.X);
                NewScrollerRect.Position.X = std::max(NewScrollerRect.Position.X,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }
            return false;
        }

        Boole HorizontalScrollbar::_ScrollBackScroll(const Vector2& HitPosition)
        {
            if( this->ChildLock == this->ScrollBack )
            {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                const Real ScrollDist = this->ScrollBack->GetActualSize().X * this->IncrementDistance;
                Rect NewScrollerRect(OldScrollerRect);

                // Early escape in case of error, but in general this shouldn't happen.
                if( HitPosition.X < 0.0 || HitPosition.Y < 0.0 )
                    return false;

                if( HitPosition.X < OldScrollerRect.Position.X ) { // Check for left
                    NewScrollerRect.Position.X = ( OldScrollerRect.Position.X - OldScrollerRect.Size.X <= this->GetLowerScrollLimit() ?
                                                   this->GetLowerScrollLimit() : OldScrollerRect.Position.X - ScrollDist );
                    NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;
                }else if( HitPosition.X > OldScrollerRect.Position.X + OldScrollerRect.Size.X ) { // Check for right
                    NewScrollerRect.Position.X = ( OldScrollerRect.Position.X + ( OldScrollerRect.Size.X * 2 ) >= this->GetUpperScrollLimit() ?
                                                   this->GetUpperScrollLimit() - OldScrollerRect.Size.X : OldScrollerRect.Position.X + ScrollDist );
                    NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;
                }

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }
            return false;
        }

        Boole HorizontalScrollbar::_ButtonScroll(Button* TheButton)
        {
            if( this->ChildLock == this->UpLeftButton ) {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                Real ScrollDist = -(this->ScrollBack->GetActualSize().X * this->IncrementDistance);
                NewScrollerRect.Position.X = OldScrollerRect.Position.X + ScrollDist;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;

                NewScrollerRect.Position.X = std::min(NewScrollerRect.Position.X,this->GetUpperScrollLimit() - OldScrollerRect.Size.X);
                NewScrollerRect.Position.X = std::max(NewScrollerRect.Position.X,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }else if( this->ChildLock == this->DownRightButton ) {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                Real ScrollDist = this->ScrollBack->GetActualSize().X * this->IncrementDistance;
                NewScrollerRect.Position.X = OldScrollerRect.Position.X + ScrollDist;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y;

                NewScrollerRect.Position.X = std::min(NewScrollerRect.Position.X,this->GetUpperScrollLimit() - OldScrollerRect.Size.X);
                NewScrollerRect.Position.X = std::max(NewScrollerRect.Position.X,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // HorizontalScrollbarFactory Methods

        String HorizontalScrollbarFactory::GetWidgetTypeName() const
            { return HorizontalScrollbar::TypeName; }

        HorizontalScrollbar* HorizontalScrollbarFactory::CreateHorizontalScrollbar(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent)
        {
            HorizontalScrollbar* Ret = new HorizontalScrollbar(RendName,Style,Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        HorizontalScrollbar* HorizontalScrollbarFactory::CreateHorizontalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent)
        {
            HorizontalScrollbar* Ret = new HorizontalScrollbar(RendName,RendRect,Style,Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        HorizontalScrollbar* HorizontalScrollbarFactory::CreateHorizontalScrollbar(const XML::Node& XMLNode, Screen* Parent)
        {
            HorizontalScrollbar* Ret = new HorizontalScrollbar(XMLNode,Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        Widget* HorizontalScrollbarFactory::CreateWidget(Screen* Parent)
        {
            HorizontalScrollbar* Ret = new HorizontalScrollbar(Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        Widget* HorizontalScrollbarFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateHorizontalScrollbar(RendName,Style,Parent);
        }

        Widget* HorizontalScrollbarFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateHorizontalScrollbar(RendName,RendRect,Style,Parent);
        }

        Widget* HorizontalScrollbarFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateHorizontalScrollbar(XMLNode,Parent); }

        void HorizontalScrollbarFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<HorizontalScrollbar*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
