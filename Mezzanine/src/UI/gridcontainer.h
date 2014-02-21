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
        /// @brief Simple class used to represent positions or sizes on a grid.
        /// @details
        ///////////////////////////////////////
        struct MEZZ_LIB GridVector2
        {
            /// @brief The Cell(s) on the X axis.
            Whole X;
            /// @brief The Cell(s) on the Y axis.
            Whole Y;
        };//GridVector2

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Simple rect used to represent a complete transform on a grid.
        /// @details
        ///////////////////////////////////////
        struct MEZZ_LIB GridRect
        {
            /// @brief The Cell position of this rect.
            GridVector2 Position;
            /// @brief The Cell size of this rect.
            GridVector2 Size;

            /// @brief Gets the Left cell of this Rect.
            /// @return Returns the cell position of the Left of this Rect.
            inline Whole GetLeftCell() const
                { return this->Position.X; }
            /// @brief Gets the Right cell of this Rect.
            /// @return Returns the cell position of the Right of this Rect.
            inline Whole GetRightCell() const
                { return this->Position.X + this->Size.X; }
            /// @brief Gets the Top cell of this Rect.
            /// @return Returns the cell position of the Top of this Rect.
            inline Whole GetTopCell() const
                { return this->Position.Y; }
            /// @brief Gets the Bottom cell of this Rect.
            /// @return Returns the cell position of the Bottom of this Rect.
            inline Whole GetBottomCell() const
                { return this->Position.Y + this->Size.Y; }

            /// @brief Gets whether or not this GridRect completely envelopes another GridRect.
            /// @param Other The other GridRect to compare to.
            /// @return Returns true if the provided GridRect is completely overlapped by this GridRect, false otherwise.
            inline Boolean Envelopes(const GridRect& Other)
            {
                return ( this->GetLeftCell() <= Other.GetLeftCell() &&
                         this->GetRightCell() >= Other.GetRightCell() &&
                         this->GetTopCell() <= Other.GetTopCell() &&
                         this->GetBottomCell() >= Other.GetBottomCell() );
            }
        };//GridRect

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a container class for placing child objects on a 2 dimensional grid.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GridContainer : public PagedContainer
        {
        public:
            /// @brief Convenience typedef for a child of this container and it's grid rect.
            typedef std::pair<Widget*,GridRect>           ChildRectPair;
            /// @brief Container type for storing GridRects in relation to the children they apply to.
            typedef std::vector< ChildRectPair >          ChildRectContainer;
            /// @brief Iterator type for GridRects in relation to the children they apply to.
            typedef ChildRectContainer::iterator          ChildRectIterator;
            /// @brief Const Iterator type for GridRects in relation to the children they apply to.
            typedef ChildRectContainer::const_iterator    ConstChildRectIterator;

            /// @brief String containing the type name for this class: "GricContainer".
            static const String TypeName;
        protected:
            friend class GridContainerFactory;
            /// @internal
            /// @brief Container storing pairs of Grid rects and the children they apply to.
            ChildRectContainer ChildRects;
            /// @internal
            /// @brief The size of each cell in this grid.
            UnifiedVec2 CellSize;
            /// @internal
            /// @brief The amount of space on each side of each dimension of a given cell which will be considered unusable for the child occupying the cell.
            UnifiedVec2 CellPadding;

            /// @copydoc Renderable::ProtoSerializeImpl(XML::Node&) const
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeImpl(const XML::Node&)
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
            /// @copydoc PagedLayoutContainer::UpdateContainerDimensionsImpl(const Rect&,const Rect&)
            virtual void UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect);
        //public:
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
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc PagedContainer::UpdateWorkAreaSize()
            virtual void UpdateWorkAreaSize();
            /// @copydoc PagedContainer::QuickUpdateWorkAreaSize(const UnifiedVec2&, Boolean)
            virtual void QuickUpdateWorkAreaSize(const UnifiedVec2& ChildSize, Boolean Adding);

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            /// @copydoc PagedContainer::SetProviders(PageProvider*, PageProvider*)
            virtual void SetProviders(PageProvider* XProv, PageProvider* YProv);
            /// @copydoc PagedContainer::SetXProvider(PageProvider*)
            virtual void SetXProvider(PageProvider* XProv);
            /// @copydoc PagedContainer::SetYProvider(PageProvider*)
            virtual void SetYProvider(PageProvider* YProv);

            /// @brief Gets the area available to a child widget when it's placed in a single cell.
            /// @return Returns a Vector2 containing the size a child widget would have in this container.
            virtual UnifiedVec2 GetCellClientArea() const;

            ///////////////////////////////////////////////////////////////////////////////
            // GridContainer Properties

            /// @brief Convenience method that will set the cell size of this grid via the desired number of cells on each axis.
            /// @param X The number of desired cells in the view area on the X axis.
            /// @param Y The number of desired cells in the view area on the Y axis.
            virtual void SetCellSize(const Whole X, const Whole Y);
            /// @brief Sets the Unified size of a single cell in this container.
            /// @note Padding still has to be taken into effect after this value is calculated.  So depending on the amount of
            /// padding to be applied, likely this won't be the full size available to a child occupying a cell.
            /// @param Size A @ref UnifiedVec2 containing the size a single cell is to be given.
            virtual void SetCellSize(const UnifiedVec2& Size);
            /// @brief Gets the Unified size of a single cell in this container.
            /// @return Returns a @ref UnifiedVec2 containing the size of each cell in the grid.
            virtual const UnifiedVec2& GetCellSize() const;
            /// @brief Sets the Unified padding to be applied on each axis to each cell in this grid.
            /// @note The padding for each axis is the total for that axis.  So the actual padding applied to each edge is (padding * 0.5).
            /// @param Padding A @ref UnifiedVec2 containing the amount of padding to be applied on each axis for each cell.
            virtual void SetCellPadding(const UnifiedVec2& Padding);
            /// @brief Gets the Unified padding to be applied on each axis to each cell in this grid.
            /// @return Returns a @ref UnifiedVec2 containing the amount of padding to be applied to each cell in the grid.
            virtual const UnifiedVec2& GetCellPadding() const;

            ///////////////////////////////////////////////////////////////////////////////
            // GridContainer Configuration

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert the GridRects data of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeGridRects(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite the GridRects data of this object with it.
            /// @param SelfRoo tAn XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeGridRects(const XML::Node& SelfRoot);
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
        };//GridContainer
    }//UI
}//Mezzanine

#endif
