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
#ifndef _uiverticalscrollbar_cpp
#define _uiverticalscrollbar_cpp

#include "UI/verticalscrollbar.h"
#include "UI/button.h"
#include "UI/screen.h"
#include "UI/pagedcontainer.h"
#include "UI/uimanager.h"

#include "UI/verticallayoutstrategy.h"

#include "Input/metacode.h"
#include "stringtool.h"
#include "MathTools/mathtools.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // VerticalScrollbar Methods
        ///////////////////////////////////////

        const String VerticalScrollbar::TypeName = "VerticalScrollbar";

        VerticalScrollbar::VerticalScrollbar(Screen* Parent) :
            Scrollbar(Parent)
            {  }

        VerticalScrollbar::VerticalScrollbar(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Scrollbar(RendName,Parent)
            { this->ConstructVerticalScrollbar(Style); }

        VerticalScrollbar::VerticalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Scrollbar(RendName,RendRect,Parent)
            { this->ConstructVerticalScrollbar(Style); }

        VerticalScrollbar::VerticalScrollbar(const XML::Node& XMLNode, Screen* Parent) :
            Scrollbar(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        VerticalScrollbar::~VerticalScrollbar()
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

        void VerticalScrollbar::ConstructVerticalScrollbar(const UI::ScrollbarStyle& ScrollStyle)
        {
            // Create the rects we'll use
            UnifiedVec2 ChildPosition(0,0,0,0);
            UnifiedVec2 ChildSize(1,1,0,0);
            UnifiedRect ChildRect(ChildPosition,ChildSize);
            if( UI::SB_NoButtons == ScrollStyle )
            {
                // Create our objects
                this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                this->ScrollBack->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                this->ScrollBack->SetVerticalSizingRules(UI::SR_Fill_Available);
                this->AddChild(this->ScrollBack,0);

                this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                this->Scroller->SetManualTransformUpdates(true);
                this->AddChild(this->Scroller,1);
            }else{
                //Get the position for all items involved and configure their offsets
                if(UI::SB_Separate == ScrollStyle)
                {
                    // Create our objects
                    this->UpLeftButton = ParentScreen->CreateButton(this->Name+".UpButton",ChildRect);
                    this->UpLeftButton->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->UpLeftButton->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->UpLeftButton->SetVerticalSizingRules(UI::SR_Match_Other_Axis);
                    this->AddChild(this->UpLeftButton,0);

                    this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                    this->ScrollBack->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->ScrollBack->SetVerticalSizingRules(UI::SR_Fill_Available);
                    this->AddChild(this->ScrollBack,1);

                    this->DownRightButton = ParentScreen->CreateButton(this->Name+".DownButton",ChildRect);
                    this->DownRightButton->SetVerticalPositioningRules(UI::PF_Anchor_Bottom);
                    this->DownRightButton->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->DownRightButton->SetVerticalSizingRules(UI::SR_Match_Other_Axis);
                    this->AddChild(this->DownRightButton,2);

                    this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                    this->Scroller->SetManualTransformUpdates(true);
                    this->AddChild(this->Scroller,3);
                }
                else if(UI::SB_TogetherDownRight == ScrollStyle)
                {
                    // Create our objects
                    this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                    this->ScrollBack->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->ScrollBack->SetVerticalSizingRules(UI::SR_Fill_Available);
                    this->AddChild(this->ScrollBack,0);

                    this->UpLeftButton = ParentScreen->CreateButton(this->Name+".UpButton",UnifiedRect(UnifiedVec2(-1,-1,0,0),ChildSize));
                    this->UpLeftButton->SetVerticalPositioningRules(UI::PF_Anchor_Bottom | UI::PF_Anchor_SelfSize);
                    this->UpLeftButton->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->UpLeftButton->SetVerticalSizingRules(UI::SR_Match_Other_Axis);
                    this->AddChild(this->UpLeftButton,1);

                    this->DownRightButton = ParentScreen->CreateButton(this->Name+".DownButton",ChildRect);
                    this->DownRightButton->SetVerticalPositioningRules(UI::PF_Anchor_Bottom);
                    this->DownRightButton->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->DownRightButton->SetVerticalSizingRules(UI::SR_Match_Other_Axis);
                    this->AddChild(this->DownRightButton,2);

                    this->Scroller = ParentScreen->CreateButton(this->Name+".Scroller",ChildRect);
                    this->Scroller->SetManualTransformUpdates(true);
                    this->AddChild(this->Scroller,3);
                }
                else if(UI::SB_TogetherUpLeft == ScrollStyle)
                {
                    // Create our objects
                    this->UpLeftButton = ParentScreen->CreateButton(this->Name+".UpButton",ChildRect);
                    this->UpLeftButton->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->UpLeftButton->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->UpLeftButton->SetVerticalSizingRules(UI::SR_Match_Other_Axis);
                    this->AddChild(this->UpLeftButton,0);

                    this->DownRightButton = ParentScreen->CreateButton(this->Name+".DownButton",UnifiedRect(UnifiedVec2(1,1,0,0),ChildSize));
                    this->DownRightButton->SetVerticalPositioningRules(UI::PF_Anchor_Top | UI::PF_Anchor_SelfSize);
                    this->DownRightButton->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->DownRightButton->SetVerticalSizingRules(UI::SR_Match_Other_Axis);
                    this->AddChild(this->DownRightButton,1);

                    this->ScrollBack = ParentScreen->CreateButton(this->Name+".ScrollBack",ChildRect);
                    this->ScrollBack->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->ScrollBack->SetVerticalSizingRules(UI::SR_Fill_Available);
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

        Real VerticalScrollbar::GetUpperScrollLimit() const
        {
            return this->ScrollBack->GetActualPosition().Y + this->ScrollBack->GetActualSize().Y;
        }

        Real VerticalScrollbar::GetLowerScrollLimit() const
        {
            return this->ScrollBack->GetActualPosition().Y;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& VerticalScrollbar::GetTypeName() const
        {
            return VerticalScrollbar::TypeName;
        }

        void VerticalScrollbar::SetScrollerValue(const Real& Value)
        {
            if(Value > 1 || Value < 0)
                return;

            const Rect ScrollBackRect = this->ScrollBack->GetRect();
            const Rect OldScrollerRect = this->Scroller->GetRect();
            Rect NewScrollerRect;

            NewScrollerRect.Size.X = OldScrollerRect.Size.X;
            NewScrollerRect.Size.Y = OldScrollerRect.Size.Y;
            NewScrollerRect.Position.X = ScrollBackRect.Position.X;
            NewScrollerRect.Position.Y = ScrollBackRect.Position.Y + ( ( this->GetScrollRange() - NewScrollerRect.Size.Y ) * Value );

            this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);
        }

        Real VerticalScrollbar::GetScrollerValue() const
        {
            Real RelPos = this->Scroller->GetActualPosition().Y - this->GetLowerScrollLimit();
            Real RelLimit = this->GetScrollRange() - this->Scroller->GetActualSize().Y;
            return ( RelLimit > 0 ? RelPos / RelLimit : 0 );
        }

        void VerticalScrollbar::SetScrollerSize(const Real& Size)
        {
            if(Size > 1 || Size < 0)
                return;

            this->ScrollerSize = Size;
            const Rect ScrollBackRect = this->ScrollBack->GetRect();
            const Rect OldScrollerRect = this->Scroller->GetRect();
            Rect NewScrollerRect;

            Real ScrollValue = this->GetScrollerValue();
            NewScrollerRect.Size.X = ScrollBackRect.Size.X;
            NewScrollerRect.Size.Y = ScrollBackRect.Size.Y * Size;
            NewScrollerRect.Position.X = ScrollBackRect.Position.X;
            NewScrollerRect.Position.Y = ScrollBackRect.Position.Y + ( ( this->GetScrollRange() - NewScrollerRect.Size.Y ) * ScrollValue );

            this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);
        }

        Real VerticalScrollbar::GetScrollerSize() const
        {
            //const Real ScrollBackY = this->ScrollBack->GetActualSize().Y;
            //return ( ScrollBackY > 0 ? this->Scroller->GetActualSize().Y / ScrollBackY : 0 );
            return ScrollerSize;
        }

        void VerticalScrollbar::UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect)
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
            NewScrollerRect.Size.X = ScrollBackRect.Size.X;
            NewScrollerRect.Size.Y = ScrollBackRect.Size.Y * ScrollSize;
            NewScrollerRect.Position.X = ScrollBackRect.Position.X;
            NewScrollerRect.Position.Y = ScrollBackRect.Position.Y + ( ( this->GetScrollRange() - NewScrollerRect.Size.Y ) * ScrollValue );

            // Finally update the scroller
            this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

            // We done got icky
            this->_MarkAllLayersDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        void VerticalScrollbar::SetVisible(Boole CanSee)
        {
            if( this->AutoHideScroll && CanSee ) {
                if( this->GetMaxYPages() > 1.0 ) {
                    this->Widget::SetVisible(CanSee);
                }
            }else{
                this->Widget::SetVisible(CanSee);
            }
        }

        void VerticalScrollbar::Show()
        {
            if( this->AutoHideScroll ) {
                if( this->GetMaxYPages() > 1.0 ) {
                    this->Widget::Show();
                }
            }else{
                this->Widget::Show();
            }
        }

        void VerticalScrollbar::Hide()
        {
            this->Widget::Hide();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // PageProvider Methods

        Real VerticalScrollbar::GetMaxXPages() const
            { return 1; }

        Real VerticalScrollbar::GetMaxYPages() const
        {
            if( this->Container != NULL ) {
                const Real ContainerYSize = this->Container->GetActualSize().Y;
                if( ContainerYSize > 0 ) {
                    Real Ret = MathTools::Ceil( this->Container->GetWorkAreaSize().Y / ContainerYSize );
                    return ( Ret > 1 ? Ret : 1 );
                }
            }
            return 1;
        }

        Real VerticalScrollbar::GetCurrentXPage() const
            { return 1; }

        Real VerticalScrollbar::GetCurrentYPage() const
            { return ( (this->GetMaxYPages() - 1) * this->GetScrollerValue() ) + 1; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void VerticalScrollbar::ProtoSerializeChildQuads(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeChildQuads(SelfRoot);
        }

        void VerticalScrollbar::ProtoDeSerializeChildQuads(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeChildQuads(SelfRoot);

            // Assign the ScrollBack
            this->ScrollBack = static_cast<Button*>( this->GetChild(this->Name+".ScrollBack") );
            if( this->ScrollBack == NULL ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"ScrollBack button not found after VerticalScrollbar deserialization.");
            }

            // Assign the Scroller
            this->Scroller = static_cast<Button*>( this->GetChild(this->Name+".Scroller") );
            if( this->Scroller == NULL ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Scroller button not found after VerticalScrollbar deserialization.");
            }

            if( this->GetNumChildren() > 2 )
            {
                // Assign the LeftButton
                this->UpLeftButton = static_cast<Button*>( this->GetChild(this->Name+".UpButton") );
                if( this->UpLeftButton == NULL ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"UpButton button not found after VerticalScrollbar deserialization.");
                }

                // Assign the RightButton
                this->DownRightButton = static_cast<Button*>( this->GetChild(this->Name+".DownButton") );
                if( this->DownRightButton == NULL ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"DownButton button not found after VerticalScrollbar deserialization.");
                }
            }

            this->SubscribeToChildEvents();
        }

        String VerticalScrollbar::GetSerializableName()
            { return VerticalScrollbar::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void VerticalScrollbar::_NotifyContainerUpdated()
        {
            if( this->Container != NULL ) {
                // Update the scroller size
                Real YView = this->Container->GetActualSize().Y;
                Real YWork = this->Container->GetWorkAreaSize().Y;
                if( YWork > 0 ) {
                    this->SetScrollerSize( YView / YWork );
                }else{
                    this->SetScrollerSize( 1.0 );
                }

                // AutoHide check
                /*if( this->AutoHideScroll ) {
                    if( this->GetMaxYPages() <= 1.0 ) {
                        this->Hide();
                    }else{
                        this->Show();
                    }
                }//*/
            }
        }

        Boole VerticalScrollbar::_MouseScroll(const Vector2& MouseDelta)
        {
            if( this->ChildLock == this->Scroller && MouseDelta.Y != 0 )
            {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y + MouseDelta.Y;

                NewScrollerRect.Position.Y = std::min(NewScrollerRect.Position.Y,this->GetUpperScrollLimit() - OldScrollerRect.Size.Y);
                NewScrollerRect.Position.Y = std::max(NewScrollerRect.Position.Y,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }
            return false;
        }

        Boole VerticalScrollbar::_MouseWheelScroll(const Integer Direction)
        {
            if( Direction == Input::DIRECTIONALMOTION_UPLEFT ) {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                Real ScrollDist = -(this->ScrollBack->GetActualSize().Y * this->IncrementDistance);
                NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y + ScrollDist;

                NewScrollerRect.Position.Y = std::min(NewScrollerRect.Position.Y,this->GetUpperScrollLimit() - OldScrollerRect.Size.Y);
                NewScrollerRect.Position.Y = std::max(NewScrollerRect.Position.Y,this->GetLowerScrollLimit());

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

                Real ScrollDist = this->ScrollBack->GetActualSize().Y * this->IncrementDistance;
                NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y + ScrollDist;

                NewScrollerRect.Position.Y = std::min(NewScrollerRect.Position.Y,this->GetUpperScrollLimit() - OldScrollerRect.Size.Y);
                NewScrollerRect.Position.Y = std::max(NewScrollerRect.Position.Y,this->GetLowerScrollLimit());

                this->Scroller->UpdateDimensions(OldScrollerRect,NewScrollerRect);

                const Real NewScrollerValue = this->GetScrollerValue();
                if( OldScrollerValue != NewScrollerValue ) {
                    this->_OnScrollValueChanged(OldScrollerValue,NewScrollerValue);
                }

                return true;
            }
            return false;
        }

        Boole VerticalScrollbar::_ScrollBackScroll(const Vector2& HitPosition)
        {
            if( this->ChildLock == this->ScrollBack )
            {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                const Real ScrollDist = this->ScrollBack->GetActualSize().Y * this->IncrementDistance;
                Rect NewScrollerRect(OldScrollerRect);

                // Early escape in case of error, but in general this shouldn't happen.
                if( HitPosition.X < 0.0 || HitPosition.Y < 0.0 )
                    return false;

                if( HitPosition.Y < OldScrollerRect.Position.Y ) { // Check for above
                    NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                    NewScrollerRect.Position.Y = ( OldScrollerRect.Position.Y - OldScrollerRect.Size.Y <= this->GetLowerScrollLimit() ?
                                                   this->GetLowerScrollLimit() : OldScrollerRect.Position.Y - ScrollDist );
                }else if( HitPosition.Y > OldScrollerRect.Position.Y + OldScrollerRect.Size.Y ) { // Check for below
                    NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                    NewScrollerRect.Position.Y = ( OldScrollerRect.Position.Y + ( OldScrollerRect.Size.Y * 2 ) >= this->GetUpperScrollLimit() ?
                                                   this->GetUpperScrollLimit() - OldScrollerRect.Size.Y : OldScrollerRect.Position.Y + ScrollDist );
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

        Boole VerticalScrollbar::_ButtonScroll(Button* TheButton)
        {
            if( this->ChildLock == this->UpLeftButton ) {
                const Real OldScrollerValue = this->GetScrollerValue();
                const Rect OldScrollerRect = this->Scroller->GetRect();
                Rect NewScrollerRect;
                NewScrollerRect.Size = OldScrollerRect.Size;

                Real ScrollDist = -(this->ScrollBack->GetActualSize().Y * this->IncrementDistance);
                NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y + ScrollDist;

                NewScrollerRect.Position.Y = std::min(NewScrollerRect.Position.Y,this->GetUpperScrollLimit() - OldScrollerRect.Size.Y);
                NewScrollerRect.Position.Y = std::max(NewScrollerRect.Position.Y,this->GetLowerScrollLimit());

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

                Real ScrollDist = this->ScrollBack->GetActualSize().Y * this->IncrementDistance;
                NewScrollerRect.Position.X = OldScrollerRect.Position.X;
                NewScrollerRect.Position.Y = OldScrollerRect.Position.Y + ScrollDist;

                NewScrollerRect.Position.Y = std::min(NewScrollerRect.Position.Y,this->GetUpperScrollLimit() - OldScrollerRect.Size.Y);
                NewScrollerRect.Position.Y = std::max(NewScrollerRect.Position.Y,this->GetLowerScrollLimit());

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
        // VerticalScrollbarFactory Methods

        String VerticalScrollbarFactory::GetWidgetTypeName() const
            { return VerticalScrollbar::TypeName; }

        VerticalScrollbar* VerticalScrollbarFactory::CreateVerticalScrollbar(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent)
        {
            VerticalScrollbar* Ret = new VerticalScrollbar(RendName,Style,Parent);
            Ret->_SetLayoutStrat( new VerticalLayoutStrategy() );
            return Ret;
        }

        VerticalScrollbar* VerticalScrollbarFactory::CreateVerticalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent)
        {
            VerticalScrollbar* Ret = new VerticalScrollbar(RendName,RendRect,Style,Parent);
            Ret->_SetLayoutStrat( new VerticalLayoutStrategy() );
            return Ret;
        }

        VerticalScrollbar* VerticalScrollbarFactory::CreateVerticalScrollbar(const XML::Node& XMLNode, Screen* Parent)
        {
            VerticalScrollbar* Ret = new VerticalScrollbar(XMLNode,Parent);
            Ret->_SetLayoutStrat( new VerticalLayoutStrategy() );
            return Ret;
        }

        Widget* VerticalScrollbarFactory::CreateWidget(Screen* Parent)
        {
            VerticalScrollbar* Ret = new VerticalScrollbar(Parent);
            Ret->_SetLayoutStrat( new VerticalLayoutStrategy() );
            return Ret;
        }

        Widget* VerticalScrollbarFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateVerticalScrollbar(RendName,Style,Parent);
        }

        Widget* VerticalScrollbarFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateVerticalScrollbar(RendName,RendRect,Style,Parent);
        }

        Widget* VerticalScrollbarFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateVerticalScrollbar(XMLNode,Parent); }

        void VerticalScrollbarFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<VerticalScrollbar*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
