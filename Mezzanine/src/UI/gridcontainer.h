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

#ifndef _uigridcontainer_h
#define _uigridcontainer_h

#include "UI/pagedcontainer.h"

namespace Mezzanine
{
    namespace UI
    {
        class HorizontalScrollbar;
        class VerticalScrollbar;
        class Spinner;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a container class for placing child objects on a 2 dimensional grid.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GridContainer : public PagedContainer
        {
        protected:
            /// @internal
            /// @brief The size of each cell in this grid.
            UnifiedVec2 CellSize;
            /// @internal
            /// @brief The amount of space on each side of each dimension of a given cell which will be considered unusable for the child occupying the cell.
            UnifiedVec2 CellPadding;

            /// @copydoc PagedLayoutContainer::UpdateVisibleChildren()
            virtual void UpdateVisibleChildren();
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            GridContainer(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            GridContainer(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            GridContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            virtual ~GridContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the area available to a child widget when it's placed in a single cell.
            /// @return Returns a Vector2 containing the size a child widget would have in this container.
            virtual UnifiedVec2 GetCellClientArea() const;

            /// @brief Sets a HorizontalScrollbar to be the provider for page data for this container.
            /// @note This container does not assume ownership of the widgets passed into this method.  Take care when deleting
            /// the provided widgets or you can get segfaults.
            /// @param HPage A pointer to the Horizontal Scrollbar which will determine which page this container is rendering.
            /// @param VPage A pointer to the Vertical Scrollbar which will determine which page this container is rendering.
            virtual void SetPageWidget(HorizontalScrollbar* HPage, VerticalScrollbar* VPage);
            /// @brief Sets a Spinner to be the provider for page data for this container.
            /// @note This container does not assume ownership of the widget passed into this method.  Take care when deleting
            /// the provided widget or you can get segfaults.
            /// @param Page A pointer to the Spinner which will determine which page this container is rendering.
            virtual void SetPageWidget(Spinner* Page);

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
        };//GridContainer
    }//UI
}//Mezzanine

#endif
