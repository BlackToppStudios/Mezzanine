//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#include "uispinner.h"
#include "uibutton.h"
#include "uicaption.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "uiviewportupdatetool.h"

#include "inputquerytool.h"
#include "stringtool.h"
#include "world.h"
#include <sstream>

namespace phys
{
    namespace UI
    {
        Spinner::Spinner(const String& name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight, Layer* parent)
            : Widget(name,parent),
              IncrementOffset(Vector2(0,0)),
              DecrementOffset(Vector2(0,0)),
              Value(0),
              MinValue(0),
              MaxValue(0),
              DecimalPlaces(0),
              DecimalDisplay(false),
              Editable(false),
              IncrementLock(true),
              DecrementLock(true),
              Increment(NULL),
              Decrement(NULL),
              ValueDisplay(NULL)
        {
            Type = Widget::W_Spinner;
            SpinLayout = SStyle;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();
            }

            if(UI::Spn_Separate == SpinLayout)
            {
                if(Rect.Size.X > Rect.Size.Y * 2)
                {
                    if(Rect.Relative)
                    {
                        const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                        Vector2 APos = Rect.Position * WinDim;
                        Vector2 ASize = Rect.Size * WinDim;
                        CalculateOffsets(ASize);
                        CreateHorizontalSpinner(APos,ASize,GlyphHeight);
                    }else{
                        CalculateOffsets(Rect.Size);
                        CreateHorizontalSpinner(Rect.Position,Rect.Size,GlyphHeight);
                    }
                }
                else if(Rect.Size.Y > Rect.Size.X * 2)
                {
                    if(Rect.Relative)
                    {
                        const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                        Vector2 APos = Rect.Position * WinDim;
                        Vector2 ASize = Rect.Size * WinDim;
                        CalculateOffsets(ASize);
                        CreateVerticalSpinner(APos,ASize,GlyphHeight);
                    }else{
                        CalculateOffsets(Rect.Size);
                        CreateVerticalSpinner(Rect.Position,Rect.Size,GlyphHeight);
                    }
                }
                else
                {
                    World::GetWorldPointer()->LogAndThrow("Spinner dimensions incompatible with this widget.");
                }
            }
            else if(UI::Spn_Together_Left == SpinLayout || UI::Spn_Together_Right == SpinLayout)
            {
                if(Rect.Relative)
                {
                    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                    Vector2 APos = Rect.Position * WinDim;
                    Vector2 ASize = Rect.Size * WinDim;
                    CalculateOffsets(ASize);
                    CreateBoxSpinner(APos,ASize,GlyphHeight);
                }else{
                    CalculateOffsets(Rect.Size);
                    CreateBoxSpinner(Rect.Position,Rect.Size,GlyphHeight);
                }
            }
            CaptureData = new InputCaptureData();
            CaptureData->AddInputRange(MetaCode::KEY_1,MetaCode::KEY_0);
            CaptureData->AddInputRange(MetaCode::KEY_KP_1,MetaCode::KEY_KP_PERIOD);
            CaptureData->AddInput(MetaCode::KEY_PERIOD);
            CaptureData->AddInput(MetaCode::KEY_RETURN);
            CaptureData->AddInput(MetaCode::KEY_KP_ENTER);
        }

        Spinner::~Spinner()
        {
            delete Increment;
            delete Decrement;
            delete ValueDisplay;
            delete CaptureData;
        }

        void Spinner::CreateHorizontalSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,Parent->GetParent()->GetPrimaryAtlas());
            Vector2 IncPos = Position + IncrementOffset;
            Vector2 DecPos = Position + DecrementOffset;
            Vector2 ValPos = Position + ValueDisplayOffset;
            Increment = new Button(Name+"Inc",RenderableRect(IncPos,Vector2(Size.Y,Size.Y),false),Parent);
            Decrement = new Button(Name+"Dec",RenderableRect(DecPos,Vector2(Size.Y,Size.Y),false),Parent);
            ValueDisplay = new Caption(Name+"Dis",RenderableRect(ValPos,Vector2(Size.X - (Size.Y * 2),Size.Y),false),GlyphInfo.first,GetValueAsText(),Parent);
            if(1 != GlyphInfo.second)
                ValueDisplay->SetTextScale(GlyphInfo.second);
        }

        void Spinner::CreateVerticalSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,Parent->GetParent()->GetPrimaryAtlas());
            Vector2 IncPos = Position + IncrementOffset;
            Vector2 DecPos = Position + DecrementOffset;
            Vector2 ValPos = Position + ValueDisplayOffset;
            Increment = new Button(Name+"Inc",RenderableRect(IncPos,Vector2(Size.X,Size.X),false),Parent);
            Decrement = new Button(Name+"Dec",RenderableRect(DecPos,Vector2(Size.X,Size.X),false),Parent);
            ValueDisplay = new Caption(Name+"Dis",RenderableRect(ValPos,Vector2(Size.X,Size.Y - (Size.X * 2)),false),GlyphInfo.first,GetValueAsText(),Parent);
            if(1 != GlyphInfo.second)
                ValueDisplay->SetTextScale(GlyphInfo.second);
        }

        void Spinner::CreateBoxSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight)
        {
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            Whole ActHeight = (Whole)(GlyphHeight * WinDim.Y);
            std::pair<Whole,Real> GlyphInfo = Manager->SuggestGlyphIndex(ActHeight,Parent->GetParent()->GetPrimaryAtlas());
            Vector2 IncPos = Position + IncrementOffset;
            Vector2 DecPos = Position + DecrementOffset;
            Vector2 ValPos = Position + ValueDisplayOffset;
            Increment = new Button(Name+"Inc",RenderableRect(IncPos,Vector2(Size.Y * 0.5,Size.Y * 0.5),false),Parent);
            Decrement = new Button(Name+"Dec",RenderableRect(DecPos,Vector2(Size.Y * 0.5,Size.Y * 0.5),false),Parent);
            ValueDisplay = new Caption(Name+"Dis",RenderableRect(ValPos,Vector2(Size.X - (Size.Y * 0.5),Size.Y),false),GlyphInfo.first,GetValueAsText(),Parent);
            if(1 != GlyphInfo.second)
                ValueDisplay->SetTextScale(GlyphInfo.second);
        }

        void Spinner::CalculateOffsets(const Vector2& Size)
        {
            switch (SpinLayout)
            {
                case UI::Spn_Separate:
                {
                    if(Size.X > Size.Y * 2)
                    {
                        IncrementOffset = Vector2(Size.X - Size.Y,0);
                        DecrementOffset = Vector2(0,0);
                        ValueDisplayOffset = Vector2(Size.Y,0);
                    }
                    else if(Size.Y > Size.X * 2)
                    {
                        IncrementOffset = Vector2(0,0);
                        DecrementOffset = Vector2(0,Size.Y - Size.X);
                        ValueDisplayOffset = Vector2(0,Size.X);
                    }
                    break;
                }
                case UI::Spn_Together_Left:
                {
                    IncrementOffset = Vector2(0,0);
                    DecrementOffset = Vector2(0,Size.Y * 0.5);
                    ValueDisplayOffset = Vector2(Size.Y * 0.5,0);
                    break;
                }
                case UI::Spn_Together_Right:
                {
                    IncrementOffset = Vector2(Size.X - (Size.Y * 0.5),0);
                    DecrementOffset = Vector2(Size.X - (Size.Y * 0.5),Size.Y * 0.5);
                    ValueDisplayOffset = Vector2(0,0);
                    break;
                }
            }
        }

        void Spinner::SetLocation(const Vector2& Position)
        {
            Increment->SetActualPosition(Position + IncrementOffset);
            Decrement->SetActualPosition(Position + DecrementOffset);
            ValueDisplay->SetActualPosition(Position + ValueDisplayOffset);
        }

        void Spinner::SetArea(const Vector2& Size)
        {
            switch (SpinLayout)
            {
                case UI::Spn_Separate:
                {
                    if(Size.X > Size.Y * 2)
                    {
                        Increment->SetActualSize(Vector2(Size.Y,Size.Y));
                        Decrement->SetActualSize(Vector2(Size.Y,Size.Y));
                        ValueDisplay->SetActualSize(Vector2(Size.X - (Size.Y * 2),Size.Y));
                    }
                    else if(Size.Y > Size.X * 2)
                    {
                        Increment->SetActualSize(Vector2(Size.X,Size.X));
                        Decrement->SetActualSize(Vector2(Size.X,Size.X));
                        ValueDisplay->SetActualSize(Vector2(Size.X,Size.Y - (Size.X * 2)));
                    }
                    break;
                }
                case UI::Spn_Together_Left:
                case UI::Spn_Together_Right:
                {
                    Increment->SetActualSize(Vector2(Size.Y * 0.5,Size.Y * 0.5));
                    Decrement->SetActualSize(Vector2(Size.Y * 0.5,Size.Y * 0.5));
                    ValueDisplay->SetActualSize(Vector2(Size.X - (Size.Y * 0.5),Size.Y));
                    break;
                }
            }
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
            if(HoveredButton)
            {
                MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    if(HoveredButton==Increment)
                        IncrementLock = false;
                    else if(HoveredButton==Decrement)
                        DecrementLock = false;
                }
                else if(MetaCode::BUTTON_LIFTING && (!IncrementLock || !DecrementLock))
                {
                    if(HoveredButton==Increment && !IncrementLock)
                    {
                        Value++;
                        CheckValueLimits();
                        ValueDisplay->SetText(GetValueAsText());
                        IncrementLock = true;
                    }
                    else if(HoveredButton==Decrement && !DecrementLock)
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
                HoveredButton = Increment;
                return true;
            }
            if(Decrement->CheckMouseHover())
            {
                HoveredButton = Decrement;
                return true;
            }
            if(ValueDisplay->CheckMouseHover())
            {
                HoveredButton = NULL;
                return true;
            }
            return false;
        }

        void Spinner::ProcessCapturedInputs()
        {
            if(!Editable)
                return;
            if(0 == CaptureData->GetNumCapturedInputs())
                return;
            std::vector<MetaCode::InputCode>* CCodes = CaptureData->GetCapturedInputs();
            bool EnterPressed = false;
            for( Whole X = 0 ; X < CCodes->size() ; X++ )
            {
                MetaCode::InputCode Current = CCodes->at(X);
                if(MetaCode::KEY_RETURN == Current || MetaCode::KEY_KP_ENTER == Current)
                {
                    EnterPressed = true;
                    continue;
                }else{
                    EditCache+=StringTool::ConvertToString(Current,false);
                }
            }
            if(EnterPressed)
            {
                std::stringstream TempStream;
                if(DecimalDisplay)
                    EditCache.erase(EditCache.find("."),1);
                TempStream << EditCache;
                TempStream >> Value;
                CheckValueLimits();
                ValueDisplay->SetText(GetValueAsText());
                EditCache.clear();
            }
        }

        void Spinner::SetSpinnerValue(const int& ValueToSet)
        {
            Value = ValueToSet;
            CheckValueLimits();
            ValueDisplay->SetText(GetValueAsText());
        }

        int Spinner::GetSpinnerValue()
        {
            return Value;
        }

        void Spinner::SetValueLimits(const int& Lower, const int& Upper)
        {
            MinValue = Lower;
            MaxValue = Upper;
        }

        void Spinner::SetEditable(bool Edit)
        {
            Editable = Edit;
        }

        bool Spinner::GetEditable()
        {
            return Editable;
        }

        void Spinner::EnableDecimalDisplay(bool Enable, const Whole& Places)
        {
            DecimalDisplay = Enable;
            DecimalPlaces = Places;
        }

        void Spinner::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 Adjusted = Position * Parent->GetParent()->GetViewportDimensions();
            SetLocation(Adjusted);
        }

        void Spinner::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            SetLocation(Position);
        }

        void Spinner::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            Vector2 Adjusted = Size * Parent->GetParent()->GetViewportDimensions();
            CalculateOffsets(Adjusted);
            SetArea(Adjusted);
            SetLocation(GetActualPosition());
        }

        void Spinner::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            CalculateOffsets(Size);
            SetArea(Size);
            SetLocation(GetActualPosition());
        }

        void Spinner::UpdateDimensions()
        {
            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            Increment->UpdateDimensions();
            Decrement->UpdateDimensions();
            ValueDisplay->UpdateDimensions();
            CalculateOffsets(Result.second);
            SetPosition(RelPosition);
        }

        Button* Spinner::GetIncrement()
        {
            return Increment;
        }

        Button* Spinner::GetDecrement()
        {
            return Decrement;
        }

        Caption* Spinner::GetValueDisplay()
        {
            return ValueDisplay;
        }
    }//ui
}//phys

#endif
