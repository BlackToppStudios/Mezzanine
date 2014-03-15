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
#ifndef _uidropdownlist_cpp
#define _uidropdownlist_cpp

#include "UI/uimanager.h"
#include "UI/dropdownlist.h"
#include "UI/listbox.h"
#include "UI/checkbox.h"
#include "UI/scrollbar.h"
#include "UI/screen.h"
#include "UI/verticalcontainer.h"
#include "UI/singlelinetextlayer.h"
#include "UI/horizontallayoutstrategy.h"

#include "stringtool.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // DropDownList Static Members

        const String DropDownList::TypeName = "DropDownList";

        ///////////////////////////////////////////////////////////////////////////////
        // DropDownList Methods

        DropDownList::DropDownList(Screen* Parent) :
            Widget(Parent)
            {  }

        DropDownList::DropDownList(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Widget(RendName,Parent)
            { this->ConstructDropDownList(Style); }

        DropDownList::DropDownList(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Widget(RendName,RendRect,Parent)
            { this->ConstructDropDownList(Style); }

        DropDownList::DropDownList(const XML::Node& XMLNode, Screen* Parent) :
            Widget(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        DropDownList::~DropDownList()
        {
            this->RemoveChild( this->SelectionDisplay );
            this->ParentScreen->DestroyWidget( this->SelectionDisplay );
            this->RemoveChild( this->ListToggle );
            this->ParentScreen->DestroyWidget( this->ListToggle );
            this->RemoveChild( this->SelectionList );
            this->ParentScreen->DestroyWidget( this->SelectionList );
        }

        void DropDownList::ConstructDropDownList(const UI::ScrollbarStyle& Style)
        {
            this->SelectionDisplay = this->ParentScreen->CreateWidget(this->Name+".Display",UnifiedRect(0,0,1,1,0,0,0,0));
            this->SelectionDisplay->SetHorizontalSizingRules(UI::SR_Fill_Available);
            this->SelectionDisplay->SetVerticalSizingRules(UI::SR_Unified_Dims);
            this->AddChild(this->SelectionDisplay,1);
            this->ListToggle = this->ParentScreen->CreateCheckBox(this->Name+".Toggle",UnifiedRect(0,0,1,1,0,0,0,0));
            this->ListToggle->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
            this->ListToggle->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
            this->ListToggle->SetVerticalSizingRules(UI::SR_Unified_Dims);
            this->AddChild(this->ListToggle,2);
            this->SelectionList = this->ParentScreen->CreateListBox(this->Name+".List",UnifiedRect(0,1,1,5,0,1,0,0),Style);
            this->AddChild(this->SelectionList,3);
            this->SelectionList->Hide();

            SingleLineTextLayer* DisplayText = this->SelectionDisplay->CreateSingleLineTextLayer();
            this->SelectionDisplay->AddLayerToGroup(DisplayText,5,"Normal");
            this->SelectionDisplay->AddLayerToGroup(DisplayText,5,"Hovered");

            this->ListToggle->Subscribe(CheckBox::EventSelected,this);
            this->ListToggle->Subscribe(CheckBox::EventDeselected,this);
            this->SelectionList->GetListContainer()->Subscribe(PagedContainer::EventChildSelected,this);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& DropDownList::GetTypeName() const
        {
            return DropDownList::TypeName;
        }

        void DropDownList::UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Update the personal data first
            this->ActDims = NewSelfRect;

            /// @todo Fix the need to remove and re-add this child.
            // Remove the list as a child because manual transform updates aren't properly checked in the linear stategies.
            this->RemoveChild( this->SelectionList );

            // Update the children
            this->LayoutStrat->Layout(OldSelfRect,NewSelfRect,this->ChildWidgets);

            // Updates for the other buttons are done, so now we can re-add the list
            this->AddChild( this->SelectionList );
            this->SelectionList->SetVisible( this->ListToggle->IsSelected() );

            // Update our width to the appropriate size
            this->SelectionList->SetScrollbarWidth( UnifiedDim(this->ListToggle->GetActualSize().X / NewSelfRect.Size.X,0.0) );
            // Next prepare the new rect for the selection list
            const Rect OldListRect = this->SelectionList->GetRect();
            Rect NewListRect;
            NewListRect.Size = this->LayoutStrat->HandleChildSizing(OldSelfRect,NewSelfRect,this->SelectionList);
            NewListRect.Position = this->LayoutStrat->HandleChildPositioning(OldSelfRect,NewSelfRect,NewListRect.Size,this->SelectionList);

            // Finally update the list
            this->SelectionList->UpdateDimensions(OldListRect,NewListRect);

            // We done got icky
            this->_MarkAllLayersDirty();
        }

        void DropDownList::UpdateCurrentSelection(Widget* NewSelection)
        {
            if( NewSelection != NULL ) {
                RenderLayerGroup* NewSelectionActive = NewSelection->GetActiveGroup();
                RenderLayerGroup* DisSelectionActive = this->SelectionDisplay->GetActiveGroup();
                if( NewSelectionActive != NULL && DisSelectionActive != NULL ) {
                    /// @todo This currently assumes the default ZOrder assigned to text layers in list items.  If that should change or be more conveniently
                    /// configurable, this should be updated.
                    RenderLayer* UncastedNewText = NewSelectionActive->GetLayerByZOrder(5);
                    RenderLayer* UncastedDisText = DisSelectionActive->GetLayerByZOrder(5);
                    if( ( UncastedNewText != NULL && UncastedNewText->IsTextLayer() ) &&
                        ( UncastedDisText != NULL && UncastedDisText->IsTextLayer() ) )
                    {
                        TextLayer* CastedNewText = static_cast<TextLayer*>( UncastedNewText );
                        TextLayer* CastedDisText = static_cast<TextLayer*>( UncastedDisText );
                        CastedDisText->SetDefaultFont( CastedNewText->GetDefaultFont() );
                        CastedDisText->SetAutoTextScale( CastedNewText->GetAutoTextScalingMode(), CastedNewText->GetAutoTextScalar() );
                        CastedDisText->SetText( CastedNewText->GetText() );
                    }
                }
            }
        }

        String DropDownList::GetSelectionText() const
        {
            RenderLayerGroup* DisSelectionActive = this->SelectionDisplay->GetActiveGroup();
            if( DisSelectionActive != NULL ) {
                RenderLayer* UncastedDisText = DisSelectionActive->GetLayerByZOrder(5);
                if( UncastedDisText != NULL && UncastedDisText->IsTextLayer() ) {
                    return static_cast<TextLayer*>( UncastedDisText )->GetText();
                }else{
                    return "";
                }
            }else{
                return "";
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility and Priority Methods

        void DropDownList::SetVisible(Boole CanSee)
        {
            if( CanSee ) {
                if( this->ListToggle->IsSelected() ) {
                    this->Widget::SetVisible(CanSee);
                }else{
                    this->_OnVisibilityShown();
                    this->SelectionDisplay->SetVisible(CanSee);
                    this->ListToggle->SetVisible(CanSee);
                }
            }else{
                this->Widget::SetVisible(CanSee);
            }
        }

        void DropDownList::Show()
        {
            if( this->ListToggle->IsSelected() ) {
                this->Widget::Show();
            }else{
                this->_OnVisibilityShown();
                this->SelectionDisplay->Show();
                this->ListToggle->Show();
            }
        }

        void DropDownList::Hide()
        {
            this->Widget::Hide();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // DropDownList Properties

        ///////////////////////////////////////////////////////////////////////////////
        // DropDownList Configuration

        Widget* DropDownList::GetSelectionDisplay() const
            { return this->SelectionDisplay; }

        CheckBox* DropDownList::GetListToggle() const
            { return this->ListToggle; }

        ListBox* DropDownList::GetSelectionList() const
            { return this->SelectionList; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void DropDownList::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);
        }

        void DropDownList::ProtoSerializeChildQuads(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeChildQuads(SelfRoot);
        }

        void DropDownList::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);
        }

        void DropDownList::ProtoDeSerializeChildQuads(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeChildQuads(SelfRoot);

            // Assign the SelectionDisplay
            this->SelectionDisplay = static_cast<Widget*>( this->GetChild(this->Name+".Display") );
            if( this->SelectionDisplay == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Selection Display not found after DropDownList deserialization.");
            }

            // Assign the ListToggle
            this->ListToggle = static_cast<CheckBox*>( this->GetChild(this->Name+".Toggle") );
            if( this->ListToggle == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"List Toggle not found after DropDownList deserialization.");
            }

            // Assign the SelectionList
            this->SelectionList = static_cast<ListBox*>( this->GetChild(this->Name+".List") );
            if( this->SelectionList == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Selection List not found after DropDownList deserialization.");
            }

            this->ListToggle->Subscribe(CheckBox::EventSelected,this);
            this->ListToggle->Subscribe(CheckBox::EventDeselected,this);
            this->SelectionList->GetListContainer()->Subscribe(PagedContainer::EventChildSelected,this);
            this->SelectionList->Hide();
        }

        String DropDownList::GetSerializableName()
        {
            return DropDownList::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void DropDownList::_NotifyEvent(EventArgumentsPtr Args)
        {
            WidgetEventArgumentsPtr WidArgs = CountedPtrCast<WidgetEventArguments>(Args);
            Widget* EventWidget = this->ParentScreen->GetWidget(WidArgs->WidgetName);

            if( EventWidget == this->ListToggle ) {
                if( WidArgs->EventName == CheckBox::EventSelected ) {
                    this->SelectionList->Show();
                }else if( WidArgs->EventName == CheckBox::EventDeselected ) {
                    this->SelectionList->Hide();
                }
            }

            if( EventWidget == this->SelectionList->GetListContainer() ) {
                if( WidArgs->EventName == PagedContainer::EventChildSelected ) {
                    ChildSelectedArgumentsPtr SelectedArgs = CountedPtrCast<ChildSelectedArguments>(WidArgs);
                    if( SelectedArgs->Selected ) {
                        Widget* NewSelected = this->ParentScreen->GetWidget( SelectedArgs->ChildName );
                        if( NewSelected != NULL ) {
                            this->UpdateCurrentSelection( NewSelected );
                            this->ListToggle->ManualSelect(false);
                        }
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // DropDownListFactory Methods

        String DropDownListFactory::GetWidgetTypeName() const
            { return DropDownList::TypeName; }

        DropDownList* DropDownListFactory::CreateDropDownList(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent)
        {
            DropDownList* Ret = new DropDownList(RendName,Style,Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        DropDownList* DropDownListFactory::CreateDropDownList(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent)
        {
            DropDownList* Ret = new DropDownList(RendName,RendRect,Style,Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        DropDownList* DropDownListFactory::CreateDropDownList(const XML::Node& XMLNode, Screen* Parent)
        {
            DropDownList* Ret = new DropDownList(XMLNode,Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        Widget* DropDownListFactory::CreateWidget(Screen* Parent)
        {
            DropDownList* Ret = new DropDownList(Parent);
            Ret->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            return Ret;
        }

        Widget* DropDownListFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateDropDownList(RendName,Style,Parent);
        }

        Widget* DropDownListFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateDropDownList(RendName,RendRect,Style,Parent);
        }

        Widget* DropDownListFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateDropDownList(XMLNode,Parent); }

        void DropDownListFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<DropDownList*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
