// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Blank constructor.
            GridVector2();
            /// @brief Descriptive constructor.
            /// @param x The Cell(s) on the X axis.
            /// @param y The Cell(s) on the Y axis.
            GridVector2(const Whole x, const Whole y);

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Equality comparison operator.
            /// @param Other The other GridVector2 to compare with.
            /// @return Returns true if both GridVector2s contain the same values, false otherwise.
            Boole operator==(const GridVector2& Other);
            /// @brief Inequality comparison operator.
            /// @param Other The other GridVector2 to compare with.
            /// @return Returns true if this GridVector2 contains different values as the other GridVector2, false otherwise.
            Boole operator!=(const GridVector2& Other);
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

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Blank constructor.
            GridRect();
            /// @brief Descriptive constructor.
            /// @param XPos The Cell position on the X axis.
            /// @param YPos The Cell position on the Y axis.
            /// @param XSize The number of cells this rect occupies on the X axis.
            /// @param YSize The number of cells this rect occupies on the Y axis.
            GridRect(const Whole XPos, const Whole YPos, const Whole XSize, const Whole YSize);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the Left cell of this Rect.
            /// @return Returns the cell position of the Left of this Rect.
            Whole GetLeftCell() const;
            /// @brief Gets the Right cell of this Rect.
            /// @return Returns the cell position of the Right of this Rect.
            Whole GetRightCell() const;
            /// @brief Gets the Top cell of this Rect.
            /// @return Returns the cell position of the Top of this Rect.
            Whole GetTopCell() const;
            /// @brief Gets the Bottom cell of this Rect.
            /// @return Returns the cell position of the Bottom of this Rect.
            Whole GetBottomCell() const;

            /// @brief Gets whether or not this GridRect completely envelopes another GridRect.
            /// @param Other The other GridRect to compare to.
            /// @return Returns true if the provided GridRect is completely overlapped by this GridRect, false otherwise.
            Boole Envelopes(const GridRect& Other);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this rect should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const;
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            void ProtoDeSerialize(const XML::Node& SelfRoot);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Equality comparison operator.
            /// @param Other The other GridRect to compare with.
            /// @return Returns true if both GridRects represent the same transform, false otherwise.
            Boole operator==(const GridRect& Other);
            /// @brief Inequality comparison operator.
            /// @param Other The other GridRect to compare with.
            /// @return Returns true if this GridRect does not represent the same transform as the other GridRect, false otherwise.
            Boole operator!=(const GridRect& Other);
        };//GridRect

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a container class for placing child objects on a 2 dimensional grid.
        /// @details This class does not use a LayoutStrategy.
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
            using PagedContainer::AddChild;
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
            /// @copydoc PagedContainer::UpdateContainerDimensionsImpl(const Rect&,const Rect&)
            virtual void UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect);
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
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created GridContainer will belong to.
            GridContainer(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~GridContainer();
        //public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc PagedContainer::UpdateWorkAreaSize()
            virtual void UpdateWorkAreaSize();
            /// @copydoc PagedContainer::QuickUpdateWorkAreaSize(const UnifiedVec2&, Boole)
            virtual void QuickUpdateWorkAreaSize(const UnifiedVec2& ChildSize, Boole Adding);

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

            /// @brief Convenience method that will set the cell size of this grid via the desired number of cells within the viewing area on each axis.
            /// @param X The number of desired cells in the view area on the X axis.
            /// @param Y The number of desired cells in the view area on the Y axis.
            virtual void SetCellSize(const Whole X, const Whole Y);
            /// @brief Sets the Unified size of a single cell in this container.
            /// @note Padding still has to be taken into effect after this value is calculated.  So depending on the amount of
            /// padding to be applied, likely this won't be the full size available to a child occupying a cell. @n @n
            /// Relative portion of the cell size is relative to this containers view size.
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

            /// @brief Gets the total used size of the Grid based on the current children.
            /// @return Returns a GridVector2 containing the number of cells in this grid on each axis.
            virtual GridVector2 GetGridSize() const;

            /// @brief Sets the grid transform of a child in this container.
            /// @param Child A pointer to the child that is having it's grid transform updated.
            /// @param ChildTrans The grid transform to be given to the child.
            /// @return Returns true if a previous entry was updated, false if a new grid rect entry was inserted.
            virtual Boole SetChildGridRect(Widget* Child, const GridRect& ChildTrans);
            /// @brief Gets the grid transform of a child in this container.
            /// @param Child A pointer to the child to retrieve the grid transform of.
            /// @return Returns a grid rect representing the specified childs transform on this grid, or an empty rect if the child has no rect for this container.
            virtual GridRect GetChildGridRect(Widget* Child) const;
            /// @brief Removes the grid transform of a child from this container.
            /// @warning Use this method with extreme caution.  Removing a childs grid rect from this container can cause it to become invisible.
            /// @param Child A pointer to the child that is having it's grid transform removed.
            /// @return Returns true if the grid transform was successfully removed, false if the child had no grid rect in this container.
            virtual Boole RemoveChildGridRect(Widget* Child);

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            /// @copydoc QuadRenderable::AddChild(Widget*)
            /// @param GridTransform The position and size on the grid this child is to be given.
            virtual void AddChild(Widget* Child, const GridRect& GridTransform);
            /// @copydoc QuadRenderable::AddChild(Widget*,const UInt16)
            /// @param GridTransform The position and size on the grid this child is to be given.
            virtual void AddChild(Widget* Child, const UInt16 ZOrder, const GridRect& GridTransform);
            /// @copydoc QuadRenderable::RemoveChild(Widget*)
            virtual void RemoveChild(Widget* ToBeRemoved);
            /// @copydoc QuadRenderable::RemoveAllChildren()
            virtual void RemoveAllChildren();
            /// @copydoc QuadRenderable::DestroyChild(Widget*)
            virtual void DestroyChild(Widget* ToBeDestroyed);
            /// @copydoc QuadRenderable::DestroyAllChildren()
            virtual void DestroyAllChildren();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert the GridRects data of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeGridRects(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite the GridRects data of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            virtual void ProtoDeSerializeGridRects(const XML::Node& SelfRoot);
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
        };//GridContainer

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for GridContainer widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GridContainerFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            GridContainerFactory() {  }
            /// @brief Class destructor.
            virtual ~GridContainerFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new GridContainer.
            /// @param RendName The name to be given to the created GridContainer.
            /// @param Parent The screen the created GridContainer will belong to.
            /// @return Returns a pointer to the created GridContainer.
            virtual GridContainer* CreateGridContainer(const String& RendName, Screen* Parent);
            /// @brief Creates a new GridContainer.
            /// @param RendName The name to be given to the created GridContainer.
            /// @param RendRect The dimensions that will be assigned to the created GridContainer.
            /// @param Parent The screen the created GridContainer will belong to.
            /// @return Returns a pointer to the created GridContainer.
            virtual GridContainer* CreateGridContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new GridContainer.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created GridContainer will belong to.
            /// @return Returns a pointer to the created GridContainer.
            virtual GridContainer* CreateGridContainer(const XML::Node& XMLNode, Screen* Parent);

            /// @copydoc WidgetFactory::CreateWidget(Screen*)
            virtual Widget* CreateWidget(Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const UnifiedRect&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const XML::Node&, Screen*)
            virtual Widget* CreateWidget(const XML::Node& XMLNode, Screen* Parent);
            /// @copydoc WidgetFactory::DestroyWidget(Widget*)
            virtual void DestroyWidget(Widget* ToBeDestroyed);
        };//GridContainerFactory
    }//UI
}//Mezzanine

#endif
