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
#ifndef _uilistbox_cpp
#define _uilistbox_cpp

#include "UI/listbox.h"
#include "UI/uimanager.h"
#include "UI/screen.h"
#include "UI/verticalscrollbar.h"
#include "UI/verticalcontainer.h"
#include "UI/singlelinetextlayer.h"
#include "UI/multilinetextlayer.h"
#include "UI/layoutstrategy.h"

#include "Input/inputmanager.h"
#include "Input/metacode.h"
#include "Input/mouse.h"

#include "entresol.h"
#include "serialization.h"
#include "exception.h"

#include <cmath>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Static Members

        const String ListBox::TypeName = "ListBox";

        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Methods

        ListBox::ListBox(Screen* Parent) :
            Widget(Parent),
            ListItemFont(NULL),
            Ordering(ListBox::LIO_BottomInsert)
            { this->LayoutStrat = new LayoutStrategy(); }

        ListBox::ListBox(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Widget(RendName,Parent),
            ListItemFont(NULL),
            Ordering(ListBox::LIO_BottomInsert)
            { this->ConstructListBox(Style); }

        ListBox::ListBox(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent) :
            Widget(RendName,RendRect,Parent),
            ListItemFont(NULL),
            Ordering(ListBox::LIO_BottomInsert)
            { this->ConstructListBox(Style); }

        ListBox::ListBox(const XML::Node& XMLNode, Screen* Parent) :
            Widget(Parent),
            ListItemFont(NULL),
            Ordering(ListBox::LIO_BottomInsert)
            { this->ProtoDeSerialize(XMLNode); }

        ListBox::~ListBox()
        {
            this->ListContainer->UnbindProvider( this->ListScroll );
            this->ParentScreen->DestroyWidget( this->ListScroll );
            this->ParentScreen->DestroyWidget( this->ListContainer );
            delete this->LayoutStrat;
        }

        Boolean ListBox::HandleInputImpl(const Input::MetaCode& Code)
        {
            if( Code.GetCode() == Input::MOUSEWHEELVERTICAL && this->ActDims.IsInside( this->ParentScreen->GetMouseHitPosition() ) ) {
                if( Code.GetMetaValue() == Input::DIRECTIONALMOTION_UPLEFT ) {
                    return this->ListScroll->_ButtonScroll( this->ListScroll->GetUpLeftButton() );
                }else if( Code.GetMetaValue() == Input::DIRECTIONALMOTION_DOWNRIGHT ) {
                    return this->ListScroll->_ButtonScroll( this->ListScroll->GetDownRightButton() );
                }
            }
            return false;
        }

        void ListBox::ConstructListBox(const UI::ScrollbarStyle& Style)
        {
            this->ListScroll = this->ParentScreen->CreateVerticalScrollbar(this->Name+".Scroll",Style);
            this->ListScroll->SetAutoHide(true);
            this->ListContainer = this->ParentScreen->CreateVerticalContainer(this->Name+".Container");
            this->ListContainer->SetYProvider( this->ListScroll );

            this->ListScroll->SetUnifiedPosition(UnifiedVec2(0.92,0.0,0.0,0.0));
            this->ListScroll->SetUnifiedSize(UnifiedVec2(0.08,1.0,0.0,0.0));
            this->ListContainer->SetUnifiedPosition(UnifiedVec2(0.0,0.0,0.0,0.0));
            this->ListContainer->SetUnifiedSize(UnifiedVec2(0.92,1.0,0.0,0.0));

            SizingInfo ListItemSizing(UI::SR_Unified_Dims,UI::SR_Size_For_Text,UnifiedVec2(1.0,0.0,0.0,0.0));
            this->ListContainer->SetChildSizing(ListItemSizing,LinearContainer::SE_OnAdd);
        }

        ListBox::ListItem* ListBox::CreateListItem(const String& ItemName)
        {
            UInt16 ListItemZOrder = 0;
            if( this->GetNumListItems() > 0 ) {
                if( this->Ordering == ListBox::LIO_BottomInsert ) {
                    ListItemZOrder = ( this->ListContainer->GetHighestChildZOrder() + 1 );
                }else if( this->Ordering == ListBox::LIO_TopInsert ) {
                    ListItemZOrder = ( this->ListContainer->GetLowestChildZOrder() - 1 );
                }else{
                    ListItemZOrder = std::numeric_limits<UInt16>::max() / 2;
                }
            }else{
                ListItemZOrder = std::numeric_limits<UInt16>::max() / 2;
            }

            ListItem* NewItem = this->ParentScreen->CreateWidget(ItemName);
            this->ListContainer->AddChild(NewItem,ListItemZOrder);
            return NewItem;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Properties

        void ListBox::SetListItemFont(FontData* Font)
            { this->ListItemFont = Font; }

        void ListBox::SetListItemFont(const String& FontName)
            { this->ListItemFont = this->ParentScreen->GetFont(FontName,this->ParentScreen->GetPrimaryAtlas()); }

        FontData* ListBox::GetListItemFont() const
            { return this->ListItemFont; }

        void ListBox::SetListItemOrdering(ListBox::ListItemOrdering Order)
            { this->Ordering = Order; }

        ListBox::ListItemOrdering ListBox::GetListItemOrdering() const
            { return this->Ordering; }

        ///////////////////////////////////////////////////////////////////////////////
        // ListBox Configuration

        ListBox::ListItem* ListBox::CreateSingleLineListItem(const String& ItemName, const String& Text)
        {
            if( this->ListItemFont == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Font is not set when creating ListItem \"" + ItemName + "\" in ListBox \"" + this->Name + "\".");
            }
            ListItem* NewItem = this->CreateListItem(ItemName);
            SingleLineTextLayer* ItemLayer = NewItem->CreateSingleLineTextLayer();
            ItemLayer->SetDefaultFont( this->ListItemFont );
            ItemLayer->SetText( Text );
            ItemLayer->SetTextlineVerticalAlignment(UI::LA_Center);

            NewItem->AddLayerToGroup(ItemLayer,5,"Normal");
            NewItem->AddLayerToGroup(ItemLayer,5,"Hovered");
            return NewItem;
        }

        ListBox::ListItem* ListBox::CreateMultiLineListItem(const String& ItemName, const String& Text)
        {
            if( this->ListItemFont == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Font is not set when creating ListItem \"" + ItemName + "\" in ListBox \"" + this->Name + "\".");
            }
            ListItem* NewItem = this->CreateListItem(ItemName);
            MultiLineTextLayer* ItemLayer = NewItem->CreateMultiLineTextLayer();
            ItemLayer->SetDefaultFont( this->ListItemFont );
            ItemLayer->SetText( Text );
            ItemLayer->SetTextlineVerticalAlignment(UI::LA_Center);

            NewItem->AddLayerToGroup(ItemLayer,5,"Normal");
            NewItem->AddLayerToGroup(ItemLayer,5,"Hovered");
            return NewItem;
        }

        ListBox::ListItem* ListBox::GetListItem(const String& Name) const
            { return this->ListContainer->GetChild(Name); }

        Whole ListBox::GetNumListItems() const
            { return this->ListContainer->GetNumChildren(); }

        void ListBox::DestroyListItem(ListItem* ToBeDestroyed)
            { this->ListContainer->DestroyChild(ToBeDestroyed); }

        void ListBox::DestroyAllListItems()
            { this->ListContainer->DestroyAllChildren(); }

        VerticalScrollbar* ListBox::GetListScroll() const
            { return this->ListScroll; }

        VerticalContainer* ListBox::GetListContainer() const
            { return this->ListContainer; }

        ListBox::ListItem* ListBox::GetLastSelectedListItem() const
            { return this->ListContainer->GetLastFocusedWidget(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void ListBox::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( ListBox::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("ListItemFont").SetValue( this->ListItemFont->GetName() ) &&
                PropertiesNode.AppendAttribute("Ordering").SetValue( this->GetListItemOrdering() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",ListBox::GetSerializableName() + "Properties",true);
            }
        }

        void ListBox::ProtoSerializeChildQuads(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeChildQuads(SelfRoot);
        }

        void ListBox::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( ListBox::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("ListItemFont");
                    if( !CurrAttrib.Empty() )
                        this->SetListItemFont( CurrAttrib.AsString() );

                    CurrAttrib = PropertiesNode.GetAttribute("Ordering");
                    if( !CurrAttrib.Empty() )
                        this->SetListItemOrdering( static_cast<ListBox::ListItemOrdering>( CurrAttrib.AsWhole() ) );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (ListBox::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ListBox::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        void ListBox::ProtoDeSerializeChildQuads(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeChildQuads(SelfRoot);

            // Assign the ListScroll
            this->ListScroll = static_cast<VerticalScrollbar*>( this->GetChild(this->Name+".Scroll") );
            if( this->ListScroll == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"List scrollbar not found after ListBox deserialization.");
            }

            // Assign the ListContainer
            this->ListContainer = static_cast<VerticalContainer*>( this->GetChild(this->Name+".Container") );
            if( this->ListContainer == NULL ) {
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"List container not found after ListBox deserialization.");
            }
        }

        String ListBox::GetSerializableName()
        {
            return ListBox::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods



        ///////////////////////////////////////////////////////////////////////////////
        // MenuEntryFactory Methods

        String ListBoxFactory::GetWidgetTypeName() const
            { return ListBox::TypeName; }

        ListBox* ListBoxFactory::CreateListBox(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent)
            { return new ListBox(RendName,Style,Parent); }

        ListBox* ListBoxFactory::CreateListBox(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent)
            { return new ListBox(RendName,RendRect,Style,Parent); }

        ListBox* ListBoxFactory::CreateListBox(const XML::Node& XMLNode, Screen* Parent)
            { return new ListBox(XMLNode,Parent); }

        Widget* ListBoxFactory::CreateWidget(Screen* Parent)
            { return new ListBox(Parent); }

        Widget* ListBoxFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateListBox(RendName,Style,Parent);
        }

        Widget* ListBoxFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
        {
            UI::ScrollbarStyle Style = UI::SB_NoButtons;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("ScrollbarStyle");
            if( ParamIt != Params.end() )
                Style = static_cast<UI::ScrollbarStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateListBox(RendName,RendRect,Style,Parent);
        }

        Widget* ListBoxFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateListBox(XMLNode,Parent); }

        void ListBoxFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<ListBox*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
