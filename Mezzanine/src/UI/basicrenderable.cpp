//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uibasicrenderable_cpp
#define _uibasicrenderable_cpp

#include "uimanager.h"
#include "UI/basicrenderable.h"
#include "UI/renderablecontainer.h"
#include "UI/screen.h"
#include "UI/widget.h"
#include "world.h"

namespace Mezzanine
{
    namespace UI
    {
        BasicRenderable::BasicRenderable(const String& name, Screen* Parent)
            : Renderable(name,Parent)
        {
            PriAtlas = ParentScreen->GetPrimaryAtlas();
        }

        BasicRenderable::~BasicRenderable()
        {
        }

        void BasicRenderable::PushVertex(std::vector<VertexData>& VertVec, VertexData& Vert, const Real& X, const Real& Y, const Vector2& UV, const ColourValue& Colour, const String& Atlas)
        {
            Vert.Vert.Position.X = X;
            Vert.Vert.Position.Y = Y;
            Vert.Vert.Position.Z = 0;
            Vert.Vert.UV = UV;
            Vert.Vert.Colour = Colour;
            Vert.Atlas = Atlas;
            VertVec.push_back(Vert);
        }

        void BasicRenderable::PushTriangle(std::vector<VertexData>& VertVec, VertexData& Vert, const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& UV, const ColourValue& Colour, const String& Atlas)
        {
            PushVertex(VertVec,Vert,A.X,A.Y,UV,Colour,Atlas);
            PushVertex(VertVec,Vert,B.X,B.Y,UV,Colour,Atlas);
            PushVertex(VertVec,Vert,C.X,C.Y,UV,Colour,Atlas);
        }

        void BasicRenderable::PushQuad(std::vector<VertexData>& VertVec, VertexData& Vert, Vector2* Positions, Vector2* UVs, ColourValue* Colours, const String& Atlas)
        {
            PushVertex(VertVec,Vert,Positions[3].X,Positions[3].Y,UVs[3],Colours[3],Atlas);
            PushVertex(VertVec,Vert,Positions[1].X,Positions[1].Y,UVs[1],Colours[1],Atlas);
            PushVertex(VertVec,Vert,Positions[0].X,Positions[0].Y,UVs[0],Colours[0],Atlas);

            PushVertex(VertVec,Vert,Positions[3].X,Positions[3].Y,UVs[3],Colours[3],Atlas);
            PushVertex(VertVec,Vert,Positions[2].X,Positions[2].Y,UVs[2],Colours[2],Atlas);
            PushVertex(VertVec,Vert,Positions[1].X,Positions[1].Y,UVs[1],Colours[1],Atlas);
        }

        void BasicRenderable::PushQuad2(std::vector<VertexData>& VertVec, VertexData& Vert, Vector2* Positions, Vector2* UVs, const ColourValue& Colour, const String& Atlas)
        {
            PushVertex(VertVec,Vert,Positions[3].X,Positions[3].Y,UVs[3],Colour,Atlas);
            PushVertex(VertVec,Vert,Positions[1].X,Positions[1].Y,UVs[1],Colour,Atlas);
            PushVertex(VertVec,Vert,Positions[0].X,Positions[0].Y,UVs[0],Colour,Atlas);

            PushVertex(VertVec,Vert,Positions[3].X,Positions[3].Y,UVs[3],Colour,Atlas);
            PushVertex(VertVec,Vert,Positions[2].X,Positions[2].Y,UVs[2],Colour,Atlas);
            PushVertex(VertVec,Vert,Positions[1].X,Positions[1].Y,UVs[1],Colour,Atlas);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        void BasicRenderable::SetPrimaryAtlas(const String& Atlas)
        {
            this->PriAtlas = Atlas;
            _MarkDirty();
        }

        String BasicRenderable::GetPrimaryAtlas() const
        {
            return PriAtlas;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility Methods

        void BasicRenderable::SetVisible(bool Visible)
        {
            if(this->Visible == Visible)
                return;
            this->Visible = Visible;
            _MarkDirty();
        }

        bool BasicRenderable::IsVisible() const
        {
            return Visible && ParentScreen->IsVisible();
        }

        bool BasicRenderable::GetVisible() const
        {
            return Visible;
        }

        void BasicRenderable::Show()
        {
            SetVisible(true);
        }

        void BasicRenderable::Hide()
        {
            SetVisible(false);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RenderPriority Methods

        void BasicRenderable::SetRenderPriority(const UI::RenderPriority& Priority)
        {
            Renderable::SetRenderPriority(Priority);
            _MarkDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void BasicRenderable::_MarkDirty()
        {
            Dirty = true;
            if(ParentWidget)
                ParentWidget->_MarkDirty();
        }
    }//UI
}//Mezzanine

#endif
