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
#include "UI/horizontallayoutstrategy.h"
#include "UI/verticallayoutstrategy.h"
#include "UI/uimanager.h"

#include "stringtool.h"
#include "MathTools/mathtools.h"

#include <algorithm>
#include <sstream>

namespace
{
    Mezzanine::Boole SpinnerFilter(const Mezzanine::Int32 Glyph)
    {
        // If the glyph is a period, a number, or a backspace then it gets through.
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
            IncrementValue(1.0),
            MinValue(-1),
            MaxValue(-1),
            OrderPriority(UI::OP_Horizontal_Vertical)
            {  }

        Spinner::Spinner(const String& RendName, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent) :
            PageProvider(RendName,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            IncrementValue(1.0),
            MinValue(-1),
            MaxValue(-1),
            OrderPriority(UI::OP_Horizontal_Vertical)
            { this->ConstructSpinner(SpinStyle,EditFont); }

        Spinner::Spinner(const String& RendName, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent) :
            PageProvider(RendName,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            IncrementValue(1.0),
            MinValue(-1),
            MaxValue(-1),
            OrderPriority(UI::OP_Horizontal_Vertical)
            { this->ConstructSpinner(SpinStyle,this->ParentScreen->GetFont(EditFontName,this->ParentScreen->GetPrimaryAtlas())); }

        Spinner::Spinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent) :
            PageProvider(RendName,RendRect,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            IncrementValue(1.0),
            MinValue(-1),
            MaxValue(-1),
            OrderPriority(UI::OP_Horizontal_Vertical)
            { this->ConstructSpinner(SpinStyle,EditFont); }

        Spinner::Spinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent) :
            PageProvider(RendName,RendRect,Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            IncrementValue(1.0),
            MinValue(-1),
            MaxValue(-1),
            OrderPriority(UI::OP_Horizontal_Vertical)
            { this->ConstructSpinner(SpinStyle,this->ParentScreen->GetFont(EditFontName,this->ParentScreen->GetPrimaryAtlas())); }

        Spinner::Spinner(const XML::Node& XMLNode, Screen* Parent) :
            PageProvider(Parent),
            IncrementSpin(NULL),
            DecrementSpin(NULL),
            ValueDisplay(NULL),
            SpinValue(0.0),
            IncrementValue(1.0),
            MinValue(-1),
            MaxValue(-1),
            OrderPriority(UI::OP_Horizontal_Vertical)
            { this->ProtoDeSerialize(XMLNode); }

        Spinner::~Spinner()
        {
            if( this->Container != NULL ) {
                this->Container->UnbindProvider(this);
            }

            this->RemoveChild( this->IncrementSpin );
            this->ParentScreen->DestroyWidget( this->IncrementSpin );
            this->RemoveChild( this->DecrementSpin );
            this->ParentScreen->DestroyWidget( this->DecrementSpin );
            this->RemoveChild( this->ValueDisplay );
            this->ParentScreen->DestroyWidget( this->ValueDisplay );
        }

        void Spinner::ConstructSpinner(const SpinnerStyle SpinStyle, FontData* EditFont)
        {
            this->AddEvent(Spinner::EventSpinValueChanged);

            // Create the child widgets.
            this->ValueDisplay = this->ParentScreen->CreateEditBox(this->Name+".Display",UI::RLT_SingleLineText,EditFont);
            this->IncrementSpin = this->ParentScreen->CreateButton(this->Name+".Increment");
            this->DecrementSpin = this->ParentScreen->CreateButton(this->Name+".Decrement");

            this->SetButtonLayout(SpinStyle);

            this->ValueDisplay->SetInputFilter( &SpinnerFilter );
            this->ValueDisplay->SetText( StringTools::ConvertToString( this->SpinValue ) );

            this->ValueDisplay->Subscribe(EditBox::EventTextUpdated,this);
            this->IncrementSpin->Subscribe(Button::EventDeactivated,this);
            this->DecrementSpin->Subscribe(Button::EventDeactivated,this);
        }

        void Spinner::ClampToLimits(Real& Value)
        {
            if( this->MinValue > 0.0 && this->MaxValue > 0.0 ) {
                Value = std::max(Value,this->MinValue);
                Value = std::min(Value,this->MaxValue);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        const String& Spinner::GetTypeName() const
            { return Spinner::TypeName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Spinner Properties

        void Spinner::SetSpinValue(Real Value)
        {
            this->ClampToLimits(Value);
            if( this->SpinValue != Value ) {
                this->_OnSpinValueChanged(this->SpinValue,Value);
                this->ValueDisplay->SetText( StringTools::ConvertToString( Value ) );
                this->SpinValue = Value;
            }
        }

        Real Spinner::GetSpinValue() const
            { return this->SpinValue; }

        void Spinner::SetIncrementValue(const Real Value)
            { this->IncrementValue = Value; }

        Real Spinner::GetIncrementValue() const
            { return this->IncrementValue; }

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

        void Spinner::SetOrderingPriority(const UI::OrderingPriority Order)
            { this->OrderPriority = Order; }

        UI::OrderingPriority Spinner::GetOrderingPriority() const
            { return this->OrderPriority; }

        ///////////////////////////////////////////////////////////////////////////////
        // Spinner Configuration

        void Spinner::SetButtonLayout(const SpinnerStyle Style)
        {
            this->RemoveChild( this->IncrementSpin );
            this->RemoveChild( this->DecrementSpin );
            this->RemoveChild( this->ValueDisplay );

            if( Style == UI::Spn_Separate_Horizontal || Style == Spn_Separate_Vertical ) {
                this->IncrementSpin->SetUnifiedSize( UnifiedVec2(1,1) );
                this->ValueDisplay->SetUnifiedSize( UnifiedVec2(1,1) );
                this->DecrementSpin->SetUnifiedSize( UnifiedVec2(1,1) );

                if( Style == UI::Spn_Separate_Horizontal ) {

                    this->DecrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->DecrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->DecrementSpin->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->DecrementSpin->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->ValueDisplay->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->ValueDisplay->SetHorizontalSizingRules(UI::SR_Fill_Available);
                    this->ValueDisplay->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->IncrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
                    this->IncrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->IncrementSpin->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->IncrementSpin->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->AddChild(this->DecrementSpin,1);
                    this->AddChild(this->ValueDisplay,2);
                    this->AddChild(this->IncrementSpin,3);

                    this->_SetLayoutStrat( new HorizontalLayoutStrategy() );
                }else if( Style == Spn_Separate_Vertical ) {
                    this->IncrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->IncrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->IncrementSpin->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->IncrementSpin->SetVerticalSizingRules(UI::SR_Match_Other_Axis);

                    this->ValueDisplay->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->ValueDisplay->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->ValueDisplay->SetVerticalSizingRules(UI::SR_Fill_Available);

                    this->DecrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->DecrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Bottom);
                    this->DecrementSpin->SetHorizontalSizingRules(UI::SR_Unified_Dims);
                    this->DecrementSpin->SetVerticalSizingRules(UI::SR_Match_Other_Axis);

                    this->AddChild(this->IncrementSpin,1);
                    this->AddChild(this->ValueDisplay,2);
                    this->AddChild(this->DecrementSpin,3);

                    this->_SetLayoutStrat( new VerticalLayoutStrategy() );
                }
            }else if( Style == Spn_Together_Left || Style == Spn_Together_Right ) {
                this->IncrementSpin->SetUnifiedSize( UnifiedVec2(1,0.5) );
                this->IncrementSpin->SetUnifiedPosition( UnifiedVec2(0,0) );

                this->ValueDisplay->SetUnifiedSize( UnifiedVec2(1,1) );
                this->ValueDisplay->SetUnifiedPosition( UnifiedVec2(0,0) );

                this->DecrementSpin->SetUnifiedSize( UnifiedVec2(1,0.5) );
                this->DecrementSpin->SetUnifiedPosition( UnifiedVec2(0,0) );

                if( Style == Spn_Together_Left ) {
                    this->IncrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->IncrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->IncrementSpin->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->IncrementSpin->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->DecrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->DecrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Bottom);
                    this->DecrementSpin->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->DecrementSpin->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->ValueDisplay->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
                    this->ValueDisplay->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->ValueDisplay->SetHorizontalSizingRules(UI::SR_Fill_Available);
                    this->ValueDisplay->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->AddChild(this->IncrementSpin,1);
                    this->AddChild(this->DecrementSpin,2);
                    this->AddChild(this->ValueDisplay,3);
                }else if( Style == Spn_Together_Right ) {
                    this->ValueDisplay->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
                    this->ValueDisplay->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->ValueDisplay->SetHorizontalSizingRules(UI::SR_Fill_Available);
                    this->ValueDisplay->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->IncrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
                    this->IncrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Top);
                    this->IncrementSpin->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->IncrementSpin->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->DecrementSpin->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
                    this->DecrementSpin->SetVerticalPositioningRules(UI::PF_Anchor_Bottom);
                    this->DecrementSpin->SetHorizontalSizingRules(UI::SR_Match_Other_Axis);
                    this->DecrementSpin->SetVerticalSizingRules(UI::SR_Unified_Dims);

                    this->AddChild(this->ValueDisplay,1);
                    this->AddChild(this->IncrementSpin,2);
                    this->AddChild(this->DecrementSpin,3);
                }

                this->_SetLayoutStrat( new HorizontalLayoutStrategy() );
            }
        }

        Button* Spinner::GetIncrement() const
            { return this->IncrementSpin; }

        Button* Spinner::GetDecrement() const
            { return this->DecrementSpin; }

        EditBox* Spinner::GetValueDisplay() const
            { return this->ValueDisplay; }

        ///////////////////////////////////////////////////////////////////////////////
        // PageProvider Methods

        Real Spinner::GetMaxXPages() const
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

        Real Spinner::GetMaxYPages() const
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

        Real Spinner::GetCurrentXPage() const
        {
            if( this->OrderPriority == UI::OP_Horizontal_Vertical ) {
                return MathTools::Ceil( MathTools::Mod( this->SpinValue , this->GetMaxYPages() ) + 1 );
            }else if( this->OrderPriority == OP_Vertical_Horizontal ) {
                return MathTools::Ceil( this->SpinValue / this->GetMaxXPages() );
            }
            return 1;
        }

        Real Spinner::GetCurrentYPage() const
        {
            if( this->OrderPriority == UI::OP_Horizontal_Vertical ) {
                return MathTools::Ceil( this->SpinValue / this->GetMaxYPages() );
            }else if( this->OrderPriority == OP_Vertical_Horizontal ) {
                return MathTools::Ceil( MathTools::Mod( this->SpinValue , this->GetMaxXPages() ) + 1 );
            }
            return 1;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Spinner::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( Spinner::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("SpinValue").SetValue( this->GetSpinValue() ) &&
                PropertiesNode.AppendAttribute("IncrementValue").SetValue( this->GetIncrementValue() ) &&
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

                    CurrAttrib = PropertiesNode.GetAttribute("IncrementValue");
                    if( !CurrAttrib.Empty() )
                        this->SetIncrementValue( CurrAttrib.AsReal() );

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
            if( this->Container != NULL ) {
                this->Container->UpdateVisibleChildren();
            }

            SpinnerValueChangedArgumentsPtr Args( new SpinnerValueChangedArguments(Spinner::EventSpinValueChanged,this->Name,OldValue,NewValue) );
            this->FireEvent(Args);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void Spinner::_NotifyEvent(EventArgumentsPtr Args)
        {
            WidgetEventArgumentsPtr WidArgs = CountedPtrCast<WidgetEventArguments>(Args);
            Widget* EventWidget = this->ParentScreen->GetWidget(WidArgs->WidgetName);
            if( EventWidget == NULL )
                return;

            if( EventWidget == this->ValueDisplay ) {
                // Squalch events to prevent an infinite loop.
                Boole OldMute = this->ValueDisplay->GetMuteEvents();
                this->ValueDisplay->SetMuteEvents(true);
                String ValueText = this->ValueDisplay->GetText();
                Real NewValue = StringTools::ConvertToReal(ValueText);
                this->SetSpinValue(NewValue);
                if( this->SpinValue != NewValue ) {
                    this->ValueDisplay->SetText( StringTools::ConvertToString( this->SpinValue ) );
                }
                this->ValueDisplay->SetMuteEvents(OldMute);
            }else if( EventWidget == this->IncrementSpin ) {
                Real Temp = this->SpinValue + this->IncrementValue;
                this->SetSpinValue(Temp);
            }else if( EventWidget == this->DecrementSpin ) {
                Real Temp = this->SpinValue - this->IncrementValue;
                this->SetSpinValue(Temp);
            }
        }

        void Spinner::_NotifyContainerUpdated()
        {
            if( this->Container != NULL ) {
                const Vector2 View = this->Container->GetActualSize();
                const Vector2 Work = this->Container->GetWorkAreaSize();

                if( View.X > 0 && View.Y > 0 ) {
                    PagedContainer::ProviderMode Config = this->Container->GetProviderConfig(this);
                    if( Config == PagedContainer::PM_Single_X ) {
                        Real MaxPages = MathTools::Ceil( Work.X / View.X );
                        this->SetSpinValueLimits(1,MaxPages);
                    }else if( Config == PagedContainer::PM_Single_Y ) {
                        Real MaxPages = MathTools::Ceil( Work.Y / View.Y );
                        this->SetSpinValueLimits(1,MaxPages);
                    }else if( Config == PagedContainer::PM_Single_XY ) {
                        Real MaxXPages = MathTools::Ceil( Work.X / View.X );
                        Real MaxYPages = MathTools::Ceil( Work.Y / View.Y );
                        this->SetSpinValueLimits(1,MaxXPages * MaxYPages);
                    }

                    this->SetSpinValue(this->SpinValue);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // SpinnerFactory Methods

        String SpinnerFactory::GetWidgetTypeName() const
            { return Spinner::TypeName; }

        Spinner* SpinnerFactory::CreateSpinner(const String& RendName, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent)
            { return new Spinner(RendName,SpinStyle,EditFont,Parent); }

        Spinner* SpinnerFactory::CreateSpinner(const String& RendName, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent)
            { return new Spinner(RendName,SpinStyle,EditFontName,Parent); }

        Spinner* SpinnerFactory::CreateSpinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent)
            { return new Spinner(RendName,RendRect,SpinStyle,EditFont,Parent); }

        Spinner* SpinnerFactory::CreateSpinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent)
            { return new Spinner(RendName,RendRect,SpinStyle,EditFontName,Parent); }

        Spinner* SpinnerFactory::CreateSpinner(const XML::Node& XMLNode, Screen* Parent)
            { return new Spinner(XMLNode,Parent); }

        Widget* SpinnerFactory::CreateWidget(Screen* Parent)
            { return new Spinner(Parent); }

        Widget* SpinnerFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
        {
            String EditFontName;
            UI::SpinnerStyle SpinStyle = UI::Spn_Together_Right;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("EditFontName");
            if( ParamIt != Params.end() )
                EditFontName = (*ParamIt).second;

            ParamIt = Params.find("SpinStyle");
            if( ParamIt != Params.end() )
                SpinStyle = static_cast<UI::SpinnerStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateSpinner(RendName,SpinStyle,EditFontName,Parent);
        }

        Widget* SpinnerFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
        {
            String EditFontName;
            UI::SpinnerStyle SpinStyle = UI::Spn_Together_Right;

            NameValuePairMap::const_iterator ParamIt;
            ParamIt = Params.find("EditFontName");
            if( ParamIt != Params.end() )
                EditFontName = (*ParamIt).second;

            ParamIt = Params.find("SpinStyle");
            if( ParamIt != Params.end() )
                SpinStyle = static_cast<UI::SpinnerStyle>( StringTools::ConvertToUInt32( (*ParamIt).second ) );

            return this->CreateSpinner(RendName,RendRect,SpinStyle,EditFontName,Parent);
        }

        Widget* SpinnerFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateSpinner(XMLNode,Parent); }

        void SpinnerFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<Spinner*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
