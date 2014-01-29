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
#ifndef _uilinelist_h
#define _uilinelist_h

#include "colourvalue.h"
#include "uienumerations.h"
#include "UI/renderable.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class VertexData;
        class ScreenRenderData;
        class LineListRenderer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class LineList
        /// @headerfile uilinelist.h
        /// @brief This is an object comprised of a series of lines.
        /// @details This class isn't an object, but rather just a series of lines.  As such it
        /// doesn't have a position or size.  The position functions exist only to create additional
        /// points for the lines to connect.
        ///////////////////////////////////////
        class MEZZ_LIB LineList : public Renderable
        {
            public:
                /// @brief Basic container type for the storage of all the points in 2D space that create the line to be rendered.
                typedef std::vector<Vector2> PointVector;
            protected:
                friend class ExtendedRenderableFactory;
                /// @internal
                /// @brief The colour of the line.
                ColourValue Colour;
                /// @internal
                /// @brief The points in 2D space that create this line.
                PointVector Positions;
                /// @internal
                /// @brief The internal renderer responsible for generating this lines vertices.
                LineListRenderer* Renderer;
                /// @internal
                /// @brief The pixel thickness of the each line segment in this linelist.
                Real Thickness;
                /// @internal
                /// @brief A bool indicating whether or not an additional segment should be generated between the first and last points.
                Boolean Closed;
            //public:
                /// @brief Class constructor.
                /// @param RendName The name to give to this Linelist.
                /// @param PScreen Pointer to the parent Screen that created this linelist.
                LineList(const String& RendName, Screen* PScreen);
                /// @brief Class destructor.
                virtual ~LineList();
            public:
                /// @brief Starts a new line list.
                /// @details If this function is called while lines have already been defined, it will
                /// clear the current list of lines and start a new list.
                /// @param LineThickness The thickness of the line to draw in pixels.
                /// @param LineColour The colour of the line.
                /// @return Returns a reference to this.
                LineList& Begin(const Whole& LineThickness, const ColourValue& LineColour);
                /// @brief Adds a new point/line to the list via 2 reals.
                /// @param X Relative coordinate on the X vector.
                /// @param Y Relative coordinate on the Y vector.
                /// @return Returns a reference to this.
                LineList& AddPoint(const Real& X, const Real& Y);
                /// @brief Adds a new point/line to the list via a vector2.
                /// @param Position A vector2 representing the relative position on screen.
                /// @return Returns a reference to this.
                LineList& AddPoint(const Vector2& Position);
                /// @brief Adds a new point/line to the list via 2 reals.
                /// @param X Coordinate on the X vector.
                /// @param Y Coordinate on the Y vector.
                /// @return Returns a reference to this.
                LineList& AddActualPoint(const Real& X, const Real& Y);
                /// @brief Adds a new point/line to the list via a vector2.
                /// @param Position A vector2 representing the position on screen.
                /// @return Returns a reference to this.
                LineList& AddActualPoint(const Vector2& Position);
                /// @brief Finalizes the list and prepares it for rendering.
                /// @param Closed Whether or not the line list connects back to it's starting position.  If
                /// true this will create one last line connecting the last provided position with the first.
                void End(Boolean Closed = false);

                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @copydoc Renderable::GetRenderableType() const
                RenderableType GetRenderableType() const;
                /// @brief Gets the vector of points stored by this linelist.
                /// @return Returns a const reference to the vector of points stored by this linelist.
                const PointVector& GetPoints() const;
                /// @brief Gets whether or not this linelist is enclosed.
                /// @return Returns true if this linelist has an extra line connecting the first and last entries.
                Boolean IsClosed() const;
                /// @brief Gets the colour of this linelist.
                /// @return Returns a const reference to the colourvalue for this linelist.
                const ColourValue& GetLineColour() const;
                /// @brief Gets the thickness of the line generated by this linelist.
                /// @return Returns a const reference to the real storing the line thickness for this linelist.
                const Real& GetLineThickness() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Visibility Methods

                /// @copydoc Renderable::SetVisible(Boolean)
                virtual void SetVisible(Boolean CanSee);
                /// @copydoc Renderable::GetVisible()
                virtual Boolean GetVisible() const;
                /// @copydoc Renderable::IsVisible()
                virtual Boolean IsVisible() const;
                /// @copydoc Renderable::Show()
                virtual void Show();
                /// @copydoc Renderable::Hide()
                virtual void Hide();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @brief Updates the dimensions of this QuadRenderable based on the transform of it's parent.
                /// @details This function is called automatically when a parent changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @copydoc Renderable::_MarkDirty()
                virtual void _MarkDirty();
                /// @copydoc UI::Renderable::_AppendRenderData()
                virtual void _AppendRenderData(ScreenRenderData& RenderData);
        };//LineList
    }//UI
}//Mezzanine

#endif
