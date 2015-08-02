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
#ifndef _uieditbox_cpp
#define _uieditbox_cpp

#include "UI/editbox.h"
#include "UI/textlayer.h"
#include "UI/singlelinetextlayer.h"
#include "UI/multilinetextlayer.h"
#include "UI/renderlayergroup.h"
#include "UI/textcursor.h"
#include "UI/font.h"
#include "UI/screen.h"

#include "serialization.h"
#include "exception.h"
#include "stringtool.h"

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // EditBox Static Members

        const String EditBox::TypeName = "EditBox";
        const String EditBox::EventTextUpdated = "TextUpdated";

        ///////////////////////////////////////////////////////////////////////////////
        // EditBox Methods

        EditBox::EditBox(Screen* Parent) :
            Widget(Parent),
            EditHighlightTarget(-1,-1),
            InputFilter(NULL),
            EditHighlightOrigin(-1),
            EditingEnabled(true)
            {  }

        EditBox::EditBox(const String& RendName, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent) :
            Widget(RendName,Parent),
            EditHighlightTarget(-1,-1),
            InputFilter(NULL),
            EditHighlightOrigin(-1),
            EditingEnabled(true)
            { this->ConstructEditBox(EditLayerType,EditFont); }

        EditBox::EditBox(const String& RendName, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent) :
            Widget(RendName,Parent),
            EditHighlightTarget(-1,-1),
            InputFilter(NULL),
            EditHighlightOrigin(-1),
            EditingEnabled(true)
            { this->ConstructEditBox(EditLayerType,this->ParentScreen->GetFont(EditFontName,this->ParentScreen->GetPrimaryAtlas())); }

        EditBox::EditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent) :
            Widget(RendName,RendRect,Parent),
            EditHighlightTarget(-1,-1),
            InputFilter(NULL),
            EditHighlightOrigin(-1),
            EditingEnabled(true)
            { this->ConstructEditBox(EditLayerType,EditFont); }

        EditBox::EditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent) :
            Widget(RendName,RendRect,Parent),
            EditHighlightTarget(-1,-1),
            InputFilter(NULL),
            EditHighlightOrigin(-1),
            EditingEnabled(true)
            { this->ConstructEditBox(EditLayerType,this->ParentScreen->GetFont(EditFontName,this->ParentScreen->GetPrimaryAtlas())); }

        EditBox::EditBox(const XML::Node& XMLNode, Screen* Parent) :
            Widget(Parent),
            EditHighlightTarget(-1,-1),
            InputFilter(NULL),
            EditHighlightOrigin(-1),
            EditingEnabled(true)
            { this->ProtoDeSerialize(XMLNode); }

        EditBox::~EditBox()
            {  }

        void EditBox::CreateLayoutStrat()
            { this->LayoutStrat = NULL; }

        Boole EditBox::HandleInputImpl(const Input::MetaCode& Code)
        {
            // Verify we can consume inputs.
            if( this->IsCurrentlyEditing() ) {
                // Get the layer and get the cursor.
                TextLayer* EditLayer = this->GetEditLayer();
                TextCursor* EditCursor = EditLayer->GetCursor();
                // Get our highlight indexes.
                const Integer HighlightStart = EditLayer->GetHighlightStart();
                const Integer HighlightEnd = EditLayer->GetHighlightEnd();

                // Start our actual checks.
                if( Code.GetCode() == Input::OSTEXTINPUT &&
                    ( this->InputFilter != NULL ? this->InputFilter( Code.GetMetaValue() ) : true ) )
                {
                    // If there is a selection range we need to clear it before inserting.
                    if( HighlightStart > -1 && HighlightEnd > -1 ) {
                        // Set our post erase position.
                        EditCursor->SetCursorIndex(HighlightStart);
                        EditLayer->RemoveCharacterRange(HighlightStart,HighlightEnd);
                    }
                    // Finally perform our insert.
                    EditLayer->GetCursor()->InsertCharacterAtCursor( Code.GetMetaValue() );
                    this->_OnTextUpdated();
                    return true;
                }else if( Code.GetCode() == Input::KEY_BACKSPACE && Code.GetMetaValue() == Input::BUTTON_PRESSING ) {
                    /// @todo Enable repeat backspace somehow.
                    if( HighlightStart > -1 && HighlightEnd > -1 ) {
                        EditCursor->SetCursorIndex(HighlightStart);
                        EditLayer->RemoveCharacterRange(HighlightStart,HighlightEnd);
                    }else{
                        EditCursor->RemoveLeftCharacter();
                    }
                    this->_OnTextUpdated();
                    return true;
                }else if( Code.GetCode() == Input::KEY_LEFT && Code.GetMetaValue() == Input::BUTTON_PRESSING ) {
                    /// @todo Enable repeat move somehow.
                    if( HighlightStart > -1 && HighlightEnd > -1 ) {
                        EditCursor->SetCursorIndex(HighlightStart);
                        EditLayer->ClearHighlights();
                    }else{
                        EditCursor->MoveCursorLeft();
                    }
                    return true;
                }else if( Code.GetCode() == Input::KEY_RIGHT && Code.GetMetaValue() == Input::BUTTON_PRESSING ) {
                    /// @todo Enable repeat move somehow.
                    if( HighlightStart > -1 && HighlightEnd > -1 ) {
                        EditCursor->SetCursorIndex(HighlightEnd);
                        EditLayer->ClearHighlights();
                    }else{
                        EditCursor->MoveCursorRight();
                    }
                    return true;
                }else if( !this->IsDragged() && Code.GetCode() == Input::MOUSEBUTTON_1 && Code.GetMetaValue() == Input::BUTTON_PRESSING ) {
                    if( HighlightStart > -1 && HighlightEnd > -1 ) {
                        EditLayer->ClearHighlights();
                    }
                    // Find the offset position of the mouse and use that to update our cursor position.
                    TextLayer::CharIndexPair Result = EditLayer->GetIndexAtOffset( this->ParentScreen->GetMouseHitPosition() - this->ActDims.Position );
                    if( !Result.first )
                        return false;
                    // Perform the update.
                    EditCursor->SetCursorIndex( Result.second );
                    return true;
                }else if( this->IsDragged() && ( Code.GetCode() == Input::MOUSEABSOLUTEHORIZONTAL || Code.GetCode() == Input::MOUSEABSOLUTEVERTICAL ) ) {
                    // Assign the horizontal or vertical value to the target, then check if we have a complete target.
                    if( Code.GetCode() == Input::MOUSEABSOLUTEHORIZONTAL ) {
                        this->EditHighlightTarget.X = Code.GetMetaValue();
                    }else if( Code.GetCode() == Input::MOUSEABSOLUTEVERTICAL ) {
                        this->EditHighlightTarget.Y = Code.GetMetaValue();
                    }
                    if( this->EditHighlightTarget.X > -1 && this->EditHighlightTarget.Y > -1 ) {
                        // When the mouse is updated, the absolute position for both axes is send out regardless of change.
                        // So we'd only get to this point if we've processed both events.  Convert our target to a usable index.
                        TextLayer::CharIndexPair Result = EditLayer->GetIndexAtOffset( this->EditHighlightTarget - this->ActDims.Position );
                        if( !Result.first )
                            return false;
                        // The mouse can move to earlier characters in the squence, so sort out which index is the actual start in this set.
                        Integer IndexMin = std::min(this->EditHighlightOrigin,Result.second);
                        Integer IndexMax = std::max(this->EditHighlightOrigin,Result.second);
                        if( IndexMax == Result.second && IndexMin != Result.second ) {
                            // If we're dragging to the right, we want the mouse cursor to be on the edge of the highlights, so reduce the index to be used for highlighting.
                            --IndexMax;
                        }
                        // If there was a change, update it.
                        if( IndexMin != HighlightStart || IndexMax != HighlightEnd ) {
                            EditLayer->Highlight(IndexMin,IndexMax);
                        }
                        // Update our cursor position
                        EditCursor->SetCursorIndex(Result.second);
                    }
                    return true;
                }
            }
            return false;
        }

        void EditBox::ConstructEditBox(const RenderLayerType EditLayerType, FontData* EditFont)
        {
            // Create our events.
            this->AddEvent(EditBox::EventTextUpdated);

            // Create our edit layer.
            TextLayer* EditLayer = NULL;
            if( EditLayerType == UI::RLT_SingleLineText ) {
                EditLayer = this->CreateSingleLineTextLayer();
            }else if( EditLayerType == UI::RLT_MultiLineText ) {
                EditLayer = this->CreateMultiLineTextLayer();
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid RenderLayer type passed in to EditBox constructor.");
            }

            // Set the font separately since setting the font on construction can only be done via string. todo?
            EditLayer->SetDefaultFont(EditFont);

            // Add the edit layer to the default groups.
            this->AddLayerToGroup(EditLayer,5,Widget::WG_Normal);
            this->AddLayerToGroup(EditLayer,5,Widget::WG_Hovered);
        }

        void EditBox::UpdateEditMode()
        {
            TextLayer* EditLayer = this->GetEditLayer();
            if( EditLayer != NULL ) {
                if( this->EditingEnabled && this->HasFocus() ) {
                    EditLayer->SetCursorEnabled(true);
                }else{
                    EditLayer->ClearHighlights();
                    EditLayer->SetCursorEnabled(false);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& EditBox::GetTypeName() const
            { return EditBox::TypeName; }

        Boole EditBox::IsCurrentlyEditing() const
        {
            TextLayer* EditLayer = this->GetEditLayer();
            if( EditLayer != NULL ) {
                TextCursor* EditCursor = EditLayer->GetCursor();
                return ( ( EditCursor != NULL ) && this->HasFocus() && this->EditingEnabled );
            }
            return false;
        }

        void EditBox::SetText(const String& Text)
        {
            TextLayer* EditLayer = this->GetEditLayer();
            if( EditLayer != NULL ) {
                EditLayer->SetText(Text);
                this->_OnTextUpdated();
            }
        }

        String EditBox::GetText() const
        {
            TextLayer* EditLayer = this->GetEditLayer();
            if( EditLayer != NULL ) {
                return EditLayer->GetText();
            }
            return "";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // EditBox Properties

        void EditBox::SetEditingEnabled(Boole Enable)
        {
            if( this->EditingEnabled != Enable ) {
                this->EditingEnabled = Enable;
                this->UpdateEditMode();
            }
        }

        Boole EditBox::GetEditingEnabled() const
            { return this->EditingEnabled; }

        ///////////////////////////////////////////////////////////////////////////////
        // EditBox Configuration

        void EditBox::SetInputFilter(EditBox::FilterCallback* Callback)
            { this->InputFilter = Callback; }

        EditBox::FilterCallback* EditBox::GetIntputFilter() const
            { return this->InputFilter; }

        TextLayer* EditBox::GetEditLayer() const
        {
            RenderLayer* UncastedLayer = this->ActiveGroup->GetLayerByZOrder(5);
            if( UncastedLayer != NULL && UncastedLayer->IsTextLayer() ) {
                return static_cast<TextLayer*>(UncastedLayer);
            }
            return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void EditBox::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( EditBox::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("EditingEnabled").SetValue( this->EditingEnabled ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",EditBox::GetSerializableName() + "Properties",true);
            }
        }

        void EditBox::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( EditBox::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("EditingEnabled");
                    if( !CurrAttrib.Empty() )
                        this->SetEditingEnabled( CurrAttrib.AsBool() );
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (EditBox::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,EditBox::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String EditBox::GetSerializableName()
        {
            return EditBox::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void EditBox::_OnTextUpdated()
        {
            WidgetEventArgumentsPtr Args( new WidgetEventArguments(EditBox::EventTextUpdated,this->Name) );
            this->FireEvent(Args);
        }

        void EditBox::_OnMouseDragStart()
        {
            TextLayer* EditLayer = this->GetEditLayer();
            if( EditLayer != NULL ) {
                TextLayer::CharIndexPair Result = EditLayer->GetIndexAtOffset( this->ParentScreen->GetMouseHitPosition() - this->ActDims.Position );
                if( Result.first ) {
                    this->EditHighlightOrigin = Result.second;
                }
            }
            this->Widget::_OnMouseDragStart();
        }

        void EditBox::_OnMouseDragEnd()
        {
            this->EditHighlightTarget.SetValues(-1,-1);
            this->EditHighlightOrigin = -1;
            this->Widget::_OnMouseDragEnd();
        }

        void EditBox::_OnFocusGained()
        {
            this->Widget::_OnFocusGained();
            this->UpdateEditMode();
        }

        void EditBox::_OnFocusLost()
        {
            this->Widget::_OnFocusLost();
            this->UpdateEditMode();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        ///////////////////////////////////////////////////////////////////////////////
        // EditBoxFactory Methods

        String EditBoxFactory::GetWidgetTypeName() const
            { return EditBox::TypeName; }

        EditBox* EditBoxFactory::CreateEditBox(const String& RendName, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent)
            { return new EditBox(RendName,EditLayerType,EditFont,Parent); }

        EditBox* EditBoxFactory::CreateEditBox(const String& RendName, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent)
            { return new EditBox(RendName,EditLayerType,EditFontName,Parent); }

        EditBox* EditBoxFactory::CreateEditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent)
            { return new EditBox(RendName,RendRect,EditLayerType,EditFont,Parent); }

        EditBox* EditBoxFactory::CreateEditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent)
            { return new EditBox(RendName,RendRect,EditLayerType,EditFontName,Parent); }

        EditBox* EditBoxFactory::CreateEditBox(const XML::Node& XMLNode, Screen* Parent)
            { return new EditBox(XMLNode,Parent); }

        Widget* EditBoxFactory::CreateWidget(Screen* Parent)
            { return new EditBox(Parent); }

        Widget* EditBoxFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
        {
            String EditFontName;
            UI::RenderLayerType EditLayerType = UI::RLT_MultiLineText;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("EditFontName");
            if( ParamIt != Params.end() )
                EditFontName = (*ParamIt).second;

            ParamIt = Params.find("EditLayerType");
            if( ParamIt != Params.end() )
                EditLayerType = static_cast<UI::RenderLayerType>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateEditBox(RendName,EditLayerType,EditFontName,Parent);
        }

        Widget* EditBoxFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
        {
            String EditFontName;
            UI::RenderLayerType EditLayerType = UI::RLT_MultiLineText;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("EditFontName");
            if( ParamIt != Params.end() )
                EditFontName = (*ParamIt).second;

            ParamIt = Params.find("EditLayerType");
            if( ParamIt != Params.end() )
                EditLayerType = static_cast<UI::RenderLayerType>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateEditBox(RendName,RendRect,EditLayerType,EditFontName,Parent);
        }

        Widget* EditBoxFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateEditBox(XMLNode,Parent); }

        void EditBoxFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<EditBox*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
