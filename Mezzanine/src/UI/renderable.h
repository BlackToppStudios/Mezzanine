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
#ifndef _uirenderable_h
#define _uirenderable_h

#include "uienumerations.h"
#include "UI/vertex.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Screen;
        class ScreenVertexData;
        class Widget;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Renderable
        /// @headerfile uirenderable.h
        /// @brief Basic class for all structures that get inserted into the rendering hierarchy.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Renderable
        {
            protected:
                friend class Widget;
                UIManager* Manager;
                Screen* ParentScreen;
                Widget* ParentWidget;
                bool Dirty;
                bool Visible;
                UInt16 ZOrder;
                UI::RenderPriority Priority;
                Vector2 RelPosition;
                Vector2 RelSize;
                String Name;
            //public:
                /// @brief Class constructor.
                /// @param name The name to be given to this renderable.
                /// @param Parent The parent screen that created this renderable.
                Renderable(const String& name, Screen* Parent);
                /// @brief Class destructor.
                virtual ~Renderable();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @brief Gets the name of this renderable.
                /// @return Returns a string containing the name of this renderable.
                virtual ConstString& GetName() const;
                /// @brief Gets the currently set ZOrder of this renderable with it's parent.
                /// @details A renderable without a parent yet will have the default ZOrder of 0, which is a valid ZOrder when it gains a parent.
                /// In these cases it is recommended to check to see if this renderable has a parent.
                /// @return Returns a UInt16 representing this renderables ZOrder.
                virtual const UInt16& GetZOrder() const;
                /// @brief Updates the dimensions of this renderable to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Visibility Methods

                /// @brief Sets the visibility of this renderable.
                /// @param visible Bool determining whether or not this renderable should be visible.
                virtual void SetVisible(bool visible) = 0;
                /// @brief Gets the visibility setting of this renderable.
                /// @return Returns a bool that is the current visibility setting of this renderable.
                virtual bool GetVisible() const = 0;
                /// @brief Gets whether or not this renderable is being drawn.
                /// @details This function will check the visibility of all parent objects to see if it is being
                /// drawn.  This will not tell you whether or not this renderable has it's own visibility setting
                /// enabled.  For that see: GetVisible().
                /// @return Returns a bool representing the visibility of this renderable.
                virtual bool IsVisible() const = 0;
                /// @brief Forces this renderable to be shown.
                virtual void Show() = 0;
                /// @brief Forces this renderable to hide.
                virtual void Hide() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Render Priority Methods

                /// @brief Sets the priority this renderable should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this renderable.
                virtual void SetRenderPriority(const UI::RenderPriority& Priority);
                /// @brief Gets the priority this renderable should be rendered with.
                /// @return Returns an enum value representing this renderables priority level.
                virtual UI::RenderPriority GetRenderPriority() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Ssts the ZOrder value for this renderable.
                /// @warning Under no circumstances would any user need to call this method themselves.  Doing so can damage how things are rendered.
                /// @param Zorder The ZOrder this renderable has among the renderables belonging to it's parent.
                virtual void _SetZOrder(const UInt16& Zorder);
                /// @internal
                /// @brief Marks this renderable as well as all parent objects as dirty.
                virtual void _MarkDirty() = 0;
                /// @internal
                /// @brief Regenerates the verticies in this renderable.
                virtual void _Redraw() = 0;
                /// @internal
                /// @brief Appends the vertices of this renderable to another vector.
                /// @param Vertices The vector of vertex's to append to.
                virtual void _AppendVertices(ScreenVertexData& Vertices) = 0;
        };//Renderable
    }//UI
}//Mezzanine

#endif
