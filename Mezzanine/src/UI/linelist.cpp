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
#ifndef _uilinelist_cpp
#define _uilinelist_cpp

#include "UI/linelist.h"
#include "UI/uienumerations.h"
#include "UI/uimanager.h"
#include "UI/screen.h"
#include "UI/simplerenderer.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // LineListRenderer Methods

        class MEZZ_LIB LineListRenderer : public SimpleRenderer
        {
        protected:
            LineList* Parent;
            /// @copydoc SimpleRenderer::RedrawImpl(Boole)
            virtual void RedrawImpl(Boole Force)
            {
                if(!this->Parent->IsVisible())
                {
                    this->Dirty = false;
                    return;
                }

                const LineList::PointVector& Positions = this->Parent->GetPoints();
                const ColourValue& Colour = this->Parent->GetLineColour();
                const Real& Thickness = this->Parent->GetLineThickness();
                Boole IsClosed = this->Parent->IsClosed();

                if(Positions.size() < 2)
                    return;

                VertexData Temp;
                Real HalfThickness = Thickness * 0.5;
                Vector2 PerpNorm, LastLeft, LastRight, ThisLeft, ThisRight, UV = this->Parent->GetScreen()->GetWhitePixel(PriAtlas);
                size_t Index = 1;
                for(  ; Index < Positions.size() ; Index++ )
                {
                    PerpNorm  = (Positions[Index] - Positions[Index - 1]).Perpendicular().Normalize();
                    LastLeft  = Positions[Index -1 ] - PerpNorm * HalfThickness;
                    LastRight = Positions[Index -1 ] + PerpNorm * HalfThickness;
                    ThisLeft  = Positions[Index] - PerpNorm * HalfThickness;
                    ThisRight = Positions[Index] + PerpNorm * HalfThickness;

                    // Triangle A
                    this->PushVertex(LastRight.X,LastRight.Y,UV,Colour,PriAtlas);      // Left/Bottom
                    this->PushVertex(ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);        // Right/Top
                    this->PushVertex(LastLeft.X,LastLeft.Y,UV,Colour,PriAtlas);        // Left/Top
                    // Triangle B
                    this->PushVertex(LastRight.X,LastRight.Y,UV,Colour,PriAtlas);      // Left/Bottom
                    this->PushVertex(ThisRight.X,ThisRight.Y,UV,Colour,PriAtlas);      // Right/Bottom
                    this->PushVertex(ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);        // Right/Top
                }

                if(IsClosed)
                {
                    Index = Positions.size() - 1;
                    PerpNorm  = (Positions[0] - Positions[Index]).Perpendicular().Normalize();
                    LastLeft  = Positions[Index] - PerpNorm * HalfThickness;
                    LastRight = Positions[Index] + PerpNorm * HalfThickness;
                    ThisLeft  = Positions[0] - PerpNorm * HalfThickness;
                    ThisRight = Positions[0] + PerpNorm * HalfThickness;

                    // Triangle A
                    this->PushVertex(LastRight.X,LastRight.Y,UV,Colour,PriAtlas);       // Left/Bottom
                    this->PushVertex(ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);         // Right/Top
                    this->PushVertex(LastLeft.X,LastLeft.Y,UV,Colour,PriAtlas);         // Left/Top
                    // Triangle B
                    this->PushVertex(LastRight.X,LastRight.Y,UV,Colour,PriAtlas);       // Left/Bottom
                    this->PushVertex(ThisRight.X,ThisRight.Y,UV,Colour,PriAtlas);       // Right/Bottom
                    this->PushVertex(ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);         // Right/Top
                }
            }
        public:
            /// @brief Class constructor.
            /// @param LL The parent LineList this renderer is rendering.
            LineListRenderer(LineList* LL) :
                Parent(LL)
                {  }
            /// @brief Class destructor.
            virtual ~LineListRenderer()
                {  }

            /// @copydoc UI::SimpleRenderer::_MarkDirty()
            virtual void _MarkDirty()
            {
                if(this->Dirty)
                    return;
                this->Parent->_MarkDirty();
                this->Dirty = true;
            }
        };//LineListRenderer

        ///////////////////////////////////////////////////////////////////////////////
        // LineList Methods

        LineList::LineList(const String& RendName, Screen* PScreen) :
            Renderable(RendName,PScreen),
            Renderer(NULL),
            Thickness(1.0),
            Closed(false)
        {
            this->Colour = ColourValue::Black();
            this->Renderer = new LineListRenderer(this);
        }

        LineList::~LineList()
        {
            delete this->Renderer;
        }

        LineList& LineList::Begin(const Whole& LineThickness, const ColourValue& LineColour)
        {
            this->Positions.clear();
            this->Thickness = LineThickness;
            this->Colour = LineColour;
            //_MarkDirty();
            return *this;
        }

        LineList& LineList::AddPoint(const Real& X, const Real& Y)
        {
            this->AddPoint(Vector2(X,Y));
            return *this;
        }

        LineList& LineList::AddPoint(const Vector2& Position)
        {
            this->Positions.push_back(Position * this->GetScreen()->GetViewportDimensions());
            return *this;
        }

        LineList& LineList::AddActualPoint(const Real& X, const Real& Y)
        {
            this->AddPoint(Vector2(X,Y));
            return *this;
        }

        LineList& LineList::AddActualPoint(const Vector2& Position)
        {
            this->Positions.push_back(Position);
            return *this;
        }

        void LineList::End(Boole Closed)
        {
            this->Closed = Closed;
            //_MarkDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Renderable::RenderableType LineList::GetRenderableType() const
        {
            return Renderable::RT_LineList;
        }

        const LineList::PointVector& LineList::GetPoints() const
        {
            return this->Positions;
        }

        Boole LineList::IsClosed() const
        {
            return this->Closed;
        }

        const ColourValue& LineList::GetLineColour() const
        {
            return this->Colour;
        }

        const Real& LineList::GetLineThickness() const
        {
            return this->Thickness;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility Methods

        void LineList::SetVisible(Boole CanSee)
        {
            if( this->Visible == CanSee )
                return;
            this->Visible = CanSee;
            //_MarkDirty();
        }

        Boole LineList::GetVisible() const
        {
            return this->Visible;
        }

        Boole LineList::IsVisible() const
        {
            return (this->Visible && ParentScreen->IsVisible());
        }

        void LineList::Show()
        {
            this->SetVisible(true);
        }

        void LineList::Hide()
        {
            this->SetVisible(false);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void LineList::UpdateDimensions()
        {
            /*Vector2 OldMid = ViewportUpdateTool::GetOldSize() * 0.5;
            Vector2 NewMid = ViewportUpdateTool::GetNewSize() * 0.5;

            for( Whole Index = 0 ; Index < Positions.size() ; Index++ )
            {
                Positions[Index] = (Positions[Index] - OldMid) + NewMid;
            }// */
            //_MarkDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void LineList::_MarkDirty()
        {
            this->Dirty = true;
        }

        void LineList::_AppendRenderData(ScreenRenderData& RenderData)
        {
            if( this->Renderer->_IsDirty() )
                this->Renderer->_Redraw(false);
            this->Renderer->_AppendVertices(RenderData.Vertices);
        }
    }//UI
}//Mezzanine

#endif
