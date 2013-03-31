//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uibasicrenderable_h
#define _uibasicrenderable_h

#include "UI/renderable.h"
#include "UI/sprite.h"

namespace Mezzanine
{
    namespace UI
    {
        class Widget;
        class RenderableFactory;
        class RenderableContainer;
        class ExtendedRenderableFactory;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class BasicRenderable
        /// @headerfile uibasicrenderable.h
        /// @brief Base class for renderables that generate verticies.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB BasicRenderable : public Renderable
        {
            protected:
                friend class RenderableFactory;
                friend class Widget;
                Vector2 ActPosition;
                Vector2 ActSize;
                String PriAtlas;
                std::vector<VertexData> RenderVertices;
                /// @brief Collects all the relevant information for a single vertex and pushes it to a vector.
                void PushVertex(std::vector<VertexData>& VertVec, VertexData& Vert, const Real& X, const Real& Y, const Vector2& UV, const ColourValue& Colour, const String& Atlas);
                /// @brief Pushes vertex information for a triangle to a vector.  Equivalent to calling "PushVertex" three times.
                void PushTriangle(std::vector<VertexData>& VertVec, VertexData& Vert, const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& UV, const ColourValue& Colour, const String& Atlas);
                /// @brief Pushes vertex information for a quad to a vector.  Equivalent to calling "PushTriangle" twice.
                void PushQuad(std::vector<VertexData>& VertVec, VertexData& Vert, Vector2* Positions, Vector2* UVs, ColourValue* Colours, const String& Atlas);
                /// @brief Pushes vertex information for a quad to a vector.  Equivalent to calling "PushTriangle" twice.
                void PushQuad2(std::vector<VertexData>& VertVec, VertexData& Vert, Vector2* Positions, Vector2* UVs, const ColourValue& Colour, const String& Atlas);
            //public:
                /// @brief Class constructor.
                /// @param name The name to be given to this renderable.
                /// @param Parent The parent screen that created this renderable.
                BasicRenderable(const String& name, Screen* Parent);
                /// @brief Class destructor.
                virtual ~BasicRenderable();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set, or an empty string if none.
                virtual String GetPrimaryAtlas() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Visibility Methods

                /// @copydoc Renderable::SetVisible(bool visible)
                virtual void SetVisible(bool visible);
                /// @copydoc Renderable::GetVisible()
                virtual bool GetVisible() const;
                /// @copydoc Renderable::IsVisible()
                virtual bool IsVisible() const;
                /// @copydoc Renderable::Show()
                virtual void Show();
                /// @copydoc Renderable::Hide()
                virtual void Hide();

                ///////////////////////////////////////////////////////////////////////////////
                // Render Priority Methods

                /// @copydoc Renderable::SetRenderPriority(const UI::RenderPriority& Priority)
                virtual void SetRenderPriority(const UI::RenderPriority& Priority);

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @copydoc Renderable::_MarkDirty()
                virtual void _MarkDirty();
                /// @copydoc Renderable::_Redraw()
                virtual void _Redraw() = 0;
                /// @copydoc Renderable::_AppendVertices(ScreenVertexData& Vertices)
                virtual void _AppendVertices(ScreenVertexData& Vertices) = 0;
        };//BasicRenderable
    }//UI
}//Mezzanine

#endif
