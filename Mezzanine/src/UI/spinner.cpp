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
#ifndef _uispinner_cpp
#define _uispinner_cpp

#include "UI/spinner.h"
#include "UI/button.h"
#include "UI/editbox.h"
#include "UI/pagedcontainer.h"
#include "UI/screen.h"
#include "UI/font.h"
#include "UI/uimanager.h"

#include "stringtool.h"

#include <algorithm>
#include <sstream>

namespace
{
    Mezzanine::Boolean SpinnerFilter(const Mezzanine::Int32 Glyph)
    {
        // If the glyph is a period or a number, then it gets through.
        return ( Glyph == Mezzanine::Int32(46) || ( Glyph >= Mezzanine::Int32(48) && Glyph <= Mezzanine::Int32(57) ) );
    }
}

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Spinner Static Members

        const String Spinner::TypeName = "Spinner";
        const String Spinner::EventSpinValueChanged = "SpinValueChanged";

        ///////////////////////////////////////////////////////////////////////////////
        // Spinner Methods

        Spinner::Spinner(Screen* Parent) :
            PageProvider(Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            MinValue(0.0),
            MaxValue(0.0)
            {  }

        Spinner::Spinner(const String& RendName, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent) :
            PageProvider(RendName,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            MinValue(0.0),
            MaxValue(0.0)
            { this->ConstructSpinner(SpinStyle,EditFont); }

        Spinner::Spinner(const String& RendName, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent) :
            PageProvider(RendName,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            MinValue(0.0),
            MaxValue(0.0)
            { this->ConstructSpinner(SpinStyle,this->ParentScreen->GetFont(EditFontName,this->ParentScreen->GetPrimaryAtlas())); }

        Spinner::Spinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent) :
            PageProvider(RendName,RendRect,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            MinValue(0.0),
            MaxValue(0.0)
            { this->ConstructSpinner(SpinStyle,EditFont); }

        Spinner::Spinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent) :
            PageProvider(RendName,RendRect,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            MinValue(0.0),
            MaxValue(0.0)
            { this->ConstructSpinner(SpinStyle,this->ParentScreen->GetFont(EditFontName,this->ParentScreen->GetPrimaryAtlas())); }

        Spinner::Spinner(const XML::Node& XMLNode, Screen* Parent) :
            PageProvider(Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            MinValue(0.0),
            MaxValue(0.0)
            { this->ProtoDeSerialize(XMLNode); }

        Spinner::~Spinner()
        {
            this->RemoveChild( this->IncrementSpin );
            this->ParentScreen->DestroyWidget( this->IncrementSpin );
            this->RemoveChild( this->DecrementSpin );
            this->ParentScreen->DestroyWidget( this->DecrementSpin );
            this->RemoveChild( this->ValueDisplay );
            this->ParentScreen->DestroyWidget( this->ValueDisplay );
        }

        void Spinner::CreateLayoutStrat()
            { this->LayoutStrat = NULL; }

        void Spinner::ConstructSpinner(const SpinnerStyle SpinStyle, FontData* EditFont)
        {
            // Create the child widgets.
            this->ValueDisplay = this->ParentScreen->CreateEditBox(this->Name+".Display",UI::RLT_SingleLineText,EditFont);
            this->IncrementSpin = this->ParentScreen->CreateButton(this->Name+".Increment");
            this->DecrementSpin = this->ParentScreen->CreateButton(this->Name+".Decrement");

            if( SpinStyle == UI::Spn_Separate_Horizontal ) {

            }else if( SpinStyle == Spn_Separate_Vertical ) {

            }else if( SpinStyle == Spn_Together_Left ) {

            }else if( SpinStyle == Spn_Together_Right ) {

            }
        }

        void Spinner::ClampToLimits(Real& Value)
        {
            Value = std::max(Value,this->MinValue);
            Value = std::min(Value,this->MaxValue);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& Spinner::GetTypeName() const
            { return Spinner::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Spinner Properties

        void Spinner::SetSpinValue(const Real Value)
        {
            Real Temp = Value;
            this->ClampToLimits(Temp);
            if( this->SpinValue != Temp ) {
                this->_OnSpinValueChanged(this->SpinValue,Temp);
                this->ValueDisplay->SetText( StringTools::ConvertToString( Temp ) );
                this->SpinValue = Temp;
            }
        }

        Real Spinner::GetSpinValue() const
            { return this->SpinValue; }

        void Spinner::SetSpinValueLimits(const Real Min, const Real Max)
            { this->SetMinSpinValue(Min);  this->SetMaxSpinValue(Max); }

        void Spinner::SetMinSpinValue(const Real Value)
            { this->MinValue = Value; }

        Real Spinner::GetMinSpinValue() const
            { return this->MinValue; }

        void Spinner::SetMaxSpinValue(const Real Value)
            { this->MaxValue = Value; }

        Real Spinner::GetMaxSpinValue() const
            { return this->MaxValue; }

        ///////////////////////////////////////////////////////////////////////////////
        // Spinner Configuration

        Button* Spinner::GetIncrement() const
            { return this->IncrementSpin; }

        Button* Spinner::GetDecrement() const
            { return this->DecrementSpin; }

        EditBox* Spinner::GetValueDisplay() const
            { return this->ValueDisplay; }

        ///////////////////////////////////////////////////////////////////////////////
        // PageProvider Methods

        Real Spinner::GetMaxXPages() const
            { return this->MaxValue; }

        Real Spinner::GetMaxYPages() const
            { return this->MaxValue; }

        Real Spinner::GetCurrentXPage() const
            { return this->SpinValue; }

        Real Spinner::GetCurrentYPage() const
            { return this->SpinValue; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Spinner::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( Spinner::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("SpinValue").SetValue( this->GetSpinValue() ) &&
                PropertiesNode.AppendAttribute("MinValue").SetValue( this->GetMinSpinValue() ) &&
                PropertiesNode.AppendAttribute("MaxValue").SetValue( this->GetMaxSpinValue() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Spinner::GetSerializableName() + "Properties",true);
            }
        }

        void Spinner::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Spinner::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("SpinValue");
                    if( !CurrAttrib.Empty() )
                        this->SetSpinValue( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MinValue");
                    if( !CurrAttrib.Empty() )
                        this->SetMinSpinValue( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MaxValue");
                    if( !CurrAttrib.Empty() )
                        this->SetMaxSpinValue( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Spinner::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Spinner::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Spinner::GetSerializableName()
            { return Spinner::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Event Methods

        void Spinner::_OnSpinValueChanged(const Real OldValue, const Real NewValue)
        {
            SpinnerValueChangedArguments Args(Spinner::EventSpinValueChanged,this->Name,OldValue,NewValue);
            this->FireEvent(Args);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void Spinner::_NotifyEvent(const EventArguments& Args)
        {

        }

        void Spinner::_NotifyContainerUpdated()
        {
            if( this->Container != NULL ) {
                const Vector2 View = this->Container->GetActualSize();
                const Vector2 Work = this->Container->GetWorkAreaSize();

                PagedContainer::ProviderMode Config = this->Container->GetProviderConfig(this);
                if( Config == PagedContainer::PM_Single_X ) {

                }else if( Config == PagedContainer::PM_Single_Y ) {

                }else if( Config == PagedContainer::PM_Single_XY ) {

                }
            }
        }








        /*void Spinner::CreateHorizontalSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,ParentScreen->GetPrimaryAtlas());
            Vector2 IncPos = Position + IncrementOffset;
            Vector2 DecPos = Position + DecrementOffset;
            Vector2 ValPos = Position + ValueDisplayOffset;
            Increment = ParentScreen->CreateButton(Name+"Inc",Rect(IncPos,Vector2(Size.Y,Size.Y),false));
            Decrement = ParentScreen->CreateButton(Name+"Dec",Rect(DecPos,Vector2(Size.Y,Size.Y),false));
            ValueDisplay = ParentScreen->CreateCaption(Name+"Dis",Rect(ValPos,Vector2(Size.X - (Size.Y * 2),Size.Y),false),GlyphInfo.first,GetValueAsText());
            AddSubRenderable(0,Increment);
            AddSubRenderable(1,Decrement);
            AddSubRenderable(2,ValueDisplay);
            if(1 != GlyphInfo.second)
                ValueDisplay->SetTextScale(GlyphInfo.second);
        }

        void Spinner::CreateVerticalSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,ParentScreen->GetPrimaryAtlas());
            Vector2 IncPos = Position + IncrementOffset;
            Vector2 DecPos = Position + DecrementOffset;
            Vector2 ValPos = Position + ValueDisplayOffset;
            Increment = ParentScreen->CreateButton(Name+"Inc",Rect(IncPos,Vector2(Size.X,Size.X),false));
            Decrement = ParentScreen->CreateButton(Name+"Dec",Rect(DecPos,Vector2(Size.X,Size.X),false));
            ValueDisplay = ParentScreen->CreateCaption(Name+"Dis",Rect(ValPos,Vector2(Size.X,Size.Y - (Size.X * 2)),false),GlyphInfo.first,GetValueAsText());
            AddSubRenderable(0,Increment);
            AddSubRenderable(1,Decrement);
            AddSubRenderable(2,ValueDisplay);
            if(1 != GlyphInfo.second)
                ValueDisplay->SetTextScale(GlyphInfo.second);
        }

        void Spinner::CreateBoxSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,ParentScreen->GetPrimaryAtlas());
            Vector2 IncPos = Position + IncrementOffset;
            Vector2 DecPos = Position + DecrementOffset;
            Vector2 ValPos = Position + ValueDisplayOffset;
            Increment = ParentScreen->CreateButton(Name+"Inc",Rect(IncPos,Vector2(Size.Y * 0.5,Size.Y * 0.5),false));
            Decrement = ParentScreen->CreateButton(Name+"Dec",Rect(DecPos,Vector2(Size.Y * 0.5,Size.Y * 0.5),false));
            ValueDisplay = ParentScreen->CreateCaption(Name+"Dis",Rect(ValPos,Vector2(Size.X - (Size.Y * 0.5),Size.Y),false),GlyphInfo.first,GetValueAsText());
            AddSubRenderable(0,Increment);
            AddSubRenderable(1,Decrement);
            AddSubRenderable(2,ValueDisplay);
            if(1 != GlyphInfo.second)
                ValueDisplay->SetTextScale(GlyphInfo.second);
        }

        void Spinner::CheckValueLimits()
        {
            if(MinValue != 0 || MaxValue != 0)
            {
                if(Value < MinValue) Value = MinValue;
                if(Value > MaxValue) Value = MaxValue;
            }
        }

        String Spinner::GetValueAsText()
        {
            std::stringstream converter;
            String AsText;
            converter << Value;
            converter >> AsText;
            if(DecimalDisplay)
            {
                String Deci(".");
                if(DecimalPlaces + 1 > AsText.size()) AsText.insert(0,(DecimalPlaces+1) - AsText.size(),'0');
                AsText.insert(AsText.size() - DecimalPlaces,Deci);
            }
            return AsText;
        }

        void Spinner::UpdateImpl(bool Force)
        {
            ProcessCapturedInputs();
            if( HoveredSubWidget && (Widget::W_Button == HoveredSubWidget->GetType()) )
            {
                Input::ButtonState State = InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
                if(Input::BUTTON_PRESSING == State)
                {
                    if(HoveredSubWidget==Increment)
                        IncrementLock = false;
                    else if(HoveredSubWidget==Decrement)
                        DecrementLock = false;
                }
                else if(Input::BUTTON_LIFTING && (!IncrementLock || !DecrementLock))
                {
                    if(HoveredSubWidget==Increment && !IncrementLock)
                    {
                        Value++;
                        CheckValueLimits();
                        ValueDisplay->SetText(GetValueAsText());
                        IncrementLock = true;
                    }
                    else if(HoveredSubWidget==Decrement && !DecrementLock)
                    {
                        Value--;
                        CheckValueLimits();
                        ValueDisplay->SetText(GetValueAsText());
                        DecrementLock = true;
                    }
                }
            }
        }

        void Spinner::SetVisibleImpl(bool visible)
        {
            Increment->SetVisible(visible);
            Decrement->SetVisible(visible);
            ValueDisplay->SetVisible(visible);
        }

        bool Spinner::CheckMouseHoverImpl()
        {
            if(Increment->CheckMouseHover())
            {
                HoveredSubWidget = Increment;
                return true;
            }
            if(Decrement->CheckMouseHover())
            {
                HoveredSubWidget = Decrement;
                return true;
            }
            if(ValueDisplay->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                return true;
            }
            return false;
        }//*/
    }//UI
}//Mezzanine

#endif
