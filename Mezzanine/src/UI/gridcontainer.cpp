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
#ifndef _uigridcontainer_cpp
#define _uigridcontainer_cpp

#include "UI/gridcontainer.h"
#include "UI/pageprovider.h"

#include "exception.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // GridVector2 Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        GridVector2::GridVector2() :
            X(0), Y(0)
            {  }

        GridVector2::GridVector2(const Whole x, const Whole y) :
            X(x), Y(y)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        Boolean GridVector2::operator==(const GridVector2& Other)
            { return ( this->X == Other.X && this->Y == Other.Y ); }

        Boolean GridVector2::operator!=(const GridVector2& Other)
            { return ( this->X != Other.X || this->Y != Other.Y ); }

        ///////////////////////////////////////////////////////////////////////////////
        // GridRect Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        GridRect::GridRect()
            {  }

        GridRect::GridRect(const Whole XPos, const Whole YPos, const Whole XSize, const Whole YSize) :
            Position(XPos,YPos),
            Size(XSize,YSize)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Whole GridRect::GetLeftCell() const
            { return this->Position.X; }

        Whole GridRect::GetRightCell() const
            { return this->Position.X + ( this->Size.X - 1); }

        Whole GridRect::GetTopCell() const
            { return this->Position.Y; }

        Whole GridRect::GetBottomCell() const
            { return this->Position.Y + ( this->Size.Y - 1); }

        Boolean GridRect::Envelopes(const GridRect& Other)
        {
            return ( this->GetLeftCell() <= Other.GetLeftCell() &&
                     this->GetRightCell() >= Other.GetRightCell() &&
                     this->GetTopCell() <= Other.GetTopCell() &&
                     this->GetBottomCell() >= Other.GetBottomCell() );
        }

        void GridRect::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfNode = ParentNode.AppendChild( GridRect::GetSerializableName() );

            if( SelfNode.AppendAttribute("Version").SetValue("1") &&
                SelfNode.AppendAttribute("XPos").SetValue( this->Position.X ) &&
                SelfNode.AppendAttribute("YPos").SetValue( this->Position.Y ) &&
                SelfNode.AppendAttribute("XSize").SetValue( this->Size.X ) &&
                SelfNode.AppendAttribute("YSize").SetValue( this->Size.Y ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",GridRect::GetSerializableName(),true);
            }
        }

        void GridRect::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( String(SelfRoot.Name()) == GridRect::GetSerializableName() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = SelfRoot.GetAttribute("XPos");
                    if( !CurrAttrib.Empty() )
                        this->Position.X = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("YPos");
                    if( !CurrAttrib.Empty() )
                        this->Position.Y = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("XSize");
                    if( !CurrAttrib.Empty() )
                        this->Size.X = CurrAttrib.AsWhole();

                    CurrAttrib = SelfRoot.GetAttribute("YSize");
                    if( !CurrAttrib.Empty() )
                        this->Size.Y = CurrAttrib.AsWhole();
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + GridRect::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,GridRect::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String GridRect::GetSerializableName()
            { return "GridRect"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        Boolean GridRect::operator==(const GridRect& Other)
            { return ( this->Position == Other.Position && this->Size == Other.Size ); }

        Boolean GridRect::operator!=(const GridRect& Other)
            { return ( this->Position != Other.Position || this->Size != Other.Size ); }

        ///////////////////////////////////////////////////////////////////////////////
        // GridContainer Static Members

        const String GridContainer::TypeName              = "GridContainer";

        ///////////////////////////////////////////////////////////////////////////////
        // GridContainer Methods

        GridContainer::GridContainer(Screen* Parent) :
            PagedContainer(Parent)
            {  }

        GridContainer::GridContainer(const String& RendName, Screen* Parent) :
            PagedContainer(RendName,Parent)
            {  }

        GridContainer::GridContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            PagedContainer(RendName,RendRect,Parent)
            {  }

        GridContainer::GridContainer(const XML::Node& XMLNode, Screen* Parent) :
            PagedContainer(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        GridContainer::~GridContainer()
            {  }

        void GridContainer::ProtoSerializeImpl(XML::Node& SelfRoot) const
        {
            this->PagedContainer::ProtoSerializeImpl(SelfRoot);
            this->ProtoSerializeGridRects(SelfRoot);
        }

        void GridContainer::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
        {
            this->PagedContainer::ProtoDeSerializeImpl(SelfRoot);
            this->ProtoDeSerializeGridRects(SelfRoot);
        }

        void GridContainer::UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Clear our old data.
            this->VisibleChildren.clear();
            this->WorkAreaSize.SetIdentity();

            // Get some of the data we're gonna use.
            const GridVector2 GridSize = this->GetGridSize();
            const Vector2 ActCellPadding = this->CellPadding.CalculateActualDimensions( this->ActDims.Size );
            const Vector2 ActCellSize = this->CellSize.CalculateActualDimensions( this->ActDims.Size );

            // Since we're here, may as well update the work area size.
            this->WorkAreaSize.X = ActCellSize.X * static_cast<Real>( GridSize.X );
            this->WorkAreaSize.Y = ActCellSize.Y * static_cast<Real>( GridSize.Y );

            // Figure out how many cells we can fit onto a page.
            const Real XCellsPerPage = this->ActDims.Size.X / ActCellSize.X;
            const Real YCellsPerPage = this->ActDims.Size.Y / ActCellSize.Y;
            const Real CurrXPage = ( this->XProvider != NULL ? this->XProvider->GetCurrentXPage() : 1 );
            const Real CurrYPage = ( this->YProvider != NULL ? this->YProvider->GetCurrentYPage() : 1 );

            // Only continue if we know there are cells that can fit into the view area.
            if( XCellsPerPage >= 1 && YCellsPerPage >= 1 ) {
                // We need to get the view position within the work area.
                Vector2 ViewPosition;
                ViewPosition.X = ( CurrXPage - 1 ) * this->ActDims.Size.X;
                ViewPosition.Y = ( CurrYPage - 1 ) * this->ActDims.Size.Y;

                // Next we need to convert the "view rect" into grid coordinates
                GridRect ViewRect;
                ViewRect.Position.X = static_cast<Whole>( CurrXPage * XCellsPerPage );
                ViewRect.Position.Y = static_cast<Whole>( CurrYPage * YCellsPerPage );
                ViewRect.Size.X = static_cast<Whole>( XCellsPerPage );
                ViewRect.Size.Y = static_cast<Whole>( YCellsPerPage );

                // We already know the ChildRects container is sorted by ZOrder.
                // So now we just need to iterate over them to see if the view rect envelopes any given child.
                for( ChildRectIterator RectIt = this->ChildRects.begin() ; RectIt != this->ChildRects.end() ; ++RectIt )
                {
                    // Check if our child is completely within the view area of the grid.
                    GridRect ChildGridRect = (*RectIt).second;
                    if( ViewRect.Envelopes( ChildGridRect ) ) {
                        // If so, perform our transform update.
                        const Rect OldChildRect = (*RectIt).first->GetRect();
                        Rect NewChildRect;

                        // Set the Size
                        NewChildRect.Size.X = ( ActCellSize.X * static_cast<Real>( ChildGridRect.Size.X ) ) - ActCellPadding.X;
                        NewChildRect.Size.Y = ( ActCellSize.Y * static_cast<Real>( ChildGridRect.Size.Y ) ) - ActCellPadding.Y;
                        // Set the Position
                        // First get the column/row we're working with.  Use base 0 for easier math later.
                        const Whole ChildColumn = ( ChildGridRect.Position.X - ViewRect.Position.X );
                        const Whole ChildRow = ( ChildGridRect.Position.Y - ViewRect.Position.Y );
                        // Now we can try to calculate the actual position.
                        // Multiply the size of a cell by the column/row we are in, which should get us the base offset for that child.
                        // Then add half the padding, since the padding is supposed to be the total on both sides of that axis.
                        // Lastly apply that to the container position to convert it into a screen position.
                        NewChildRect.Position.X = NewSelfRect.Size.X + ( ( ActCellSize.X * static_cast<Real>( ChildColumn ) ) + ( ActCellPadding.X * 0.5 ) );
                        NewChildRect.Position.Y = NewSelfRect.Size.Y + ( ( ActCellSize.Y * static_cast<Real>( ChildRow ) ) + ( ActCellPadding.Y * 0.5 ) );
                        // Perform the update.
                        (*RectIt).first->UpdateDimensions(OldChildRect,NewChildRect);

                        // Finally add it to the container of visible children.
                        (*RectIt).first->Show();
                        this->VisibleChildren.push_back( (*RectIt).first );
                    }else{
                        const Rect OldChildRect = (*RectIt).first->GetRect();
                        Rect NewChildRect;
                        NewChildRect.Size = OldChildRect.Size;
                        NewChildRect.Position = NewSelfRect.Position;
                        (*RectIt).first->UpdateDimensions(OldChildRect,NewChildRect);
                        (*RectIt).first->Hide();
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void GridContainer::UpdateWorkAreaSize()
        {
            GridVector2 GridSize = this->GetGridSize();
            Vector2 ActCellSize = this->CellSize.CalculateActualDimensions( this->ActDims.Size );

            this->WorkAreaSize.X = ActCellSize.X * static_cast<Real>( GridSize.X );
            this->WorkAreaSize.Y = ActCellSize.Y * static_cast<Real>( GridSize.Y );
        }

        void GridContainer::QuickUpdateWorkAreaSize(const UnifiedVec2& ChildSize, Boolean Adding)
        {
            // This method doesn't provide enough information to do what is intended for this container.
            // So do nothing.  Wait for the slow version to do the update.
        }

        const String& GridContainer::GetTypeName() const
        {
            return GridContainer::TypeName;
        }

        void GridContainer::SetProviders(PageProvider* XProv, PageProvider* YProv)
        {
            if( XProv != YProv ) {
                this->SetXProvider(XProv);
                this->SetYProvider(YProv);
            }else{
                // Clear out the X provider.
                if( this->XProvider != NULL && this->XProvider != XProv ) {
                    this->UnbindProvider(this->XProvider);
                }
                // Clear out the Y provider.
                if( this->YProvider != NULL && this->YProvider != YProv ) {
                    this->UnbindProvider(this->YProvider);
                }

                // We know the two providers are passed in, so just check the validity for the XProvider.
                if( XProv != NULL ) {
                    this->XProvider = XProv;
                    this->YProvider = YProv;

                    this->XProvider->_SetContainer(this);
                    this->XProvider->_NotifyContainerUpdated();
                }
            }
        }

        void GridContainer::SetXProvider(PageProvider* XProv)
        {
            if( XProv == this->YProvider ) {
                this->SetProviders(XProv,XProv);
            }else{
                if( this->XProvider != NULL && this->XProvider != XProv ) {
                    this->UnbindProvider(this->XProvider);
                }
                this->XProvider = XProv;
                this->XProvider->_SetContainer(this);
                this->XProvider->_NotifyContainerUpdated();
            }
        }

        void GridContainer::SetYProvider(PageProvider* YProv)
        {
            if( YProv == this->XProvider ) {
                this->SetProviders(YProv,YProv);
            }else{
                if( this->YProvider != NULL && this->YProvider != YProv ) {
                    this->UnbindProvider(this->YProvider);
                }
                this->YProvider = YProv;
                this->YProvider->_SetContainer(this);
                this->YProvider->_NotifyContainerUpdated();
            }
        }

        UnifiedVec2 GridContainer::GetCellClientArea() const
        {
            return CellSize - CellPadding;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // GridContainer Properties

        void GridContainer::SetCellSize(const Whole X, const Whole Y)
        {
            Real XRel = 1.0 / static_cast<Real>(X);
            Real YRel = 1.0 / static_cast<Real>(Y);
            this->SetCellSize( UnifiedVec2(XRel,YRel) );
        }

        void GridContainer::SetCellSize(const UnifiedVec2& Size)
            { this->CellSize = Size; }

        const UnifiedVec2& GridContainer::GetCellSize() const
            { return this->CellSize; }

        void GridContainer::SetCellPadding(const UnifiedVec2& Padding)
            { this->CellPadding = Padding; }

        const UnifiedVec2& GridContainer::GetCellPadding() const
            { return this->CellPadding; }

        ///////////////////////////////////////////////////////////////////////////////
        // GridContainer Configuration

        GridVector2 GridContainer::GetGridSize() const
        {
            GridVector2 Ret;
            for( ConstChildRectIterator RectIt = this->ChildRects.begin() ; RectIt != this->ChildRects.end() ; ++RectIt )
            {
                if( Ret.X < (*RectIt).second.GetRightCell() ) {
                    Ret.X = (*RectIt).second.GetRightCell();
                }
                if( Ret.Y < (*RectIt).second.GetBottomCell() ) {
                    Ret.Y = (*RectIt).second.GetBottomCell();
                }
            }
            return Ret;
        }

        Boolean GridContainer::SetChildGridRect(Widget* Child, const GridRect& ChildTrans)
        {
            UInt16 Zorder = Child->GetZOrder();
            ChildRectIterator RectEnd = this->ChildRects.end();
            ChildRectIterator InsIt = RectEnd;
            for( ChildRectIterator RectIt = this->ChildRects.begin() ; RectIt != RectEnd ; ++RectIt )
            {
                if( (*RectIt).first == Child ) {
                    (*RectIt).second = ChildTrans;
                    return false;
                }else if( (*RectIt).first->GetZOrder() > Zorder && InsIt != RectEnd ) {
                    InsIt = RectIt;
                }
            }
            this->ChildRects.insert(InsIt,ChildRectPair(Child,ChildTrans));
            return ( InsIt != RectEnd );
        }

        GridRect GridContainer::GetChildGridRect(Widget* Child) const
        {
            for( ConstChildRectIterator RectIt = this->ChildRects.begin() ; RectIt != this->ChildRects.end() ; ++RectIt )
            {
                if( (*RectIt).first == Child ) {
                    return (*RectIt).second;
                }
            }
            return GridRect();
        }

        Boolean GridContainer::RemoveChildGridRect(Widget* Child)
        {
            for( ChildRectIterator RectIt = this->ChildRects.begin() ; RectIt != this->ChildRects.end() ; ++RectIt )
            {
                if( (*RectIt).first == Child ) {
                    this->ChildRects.erase(RectIt);
                    return true;
                }
            }
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Child Management

        void GridContainer::AddChild(Widget* Child, const GridRect& GridTransform)
        {
            this->SetChildGridRect(Child,GridTransform);
            this->AddChild(Child);
        }

        void GridContainer::AddChild(Widget* Child, const UInt16 ZOrder, const GridRect& GridTransform)
        {
            this->SetChildGridRect(Child,GridTransform);
            this->AddChild(Child,ZOrder);
        }

        void GridContainer::RemoveChild(Widget* ToBeRemoved)
        {
            this->RemoveChildGridRect(ToBeRemoved);
            this->PagedContainer::RemoveChild(ToBeRemoved);
        }

        void GridContainer::RemoveAllChildren()
        {
            this->ChildRects.clear();
            this->PagedContainer::RemoveAllChildren();
        }

        void GridContainer::DestroyChild(Widget* ToBeDestroyed)
        {
            this->RemoveChildGridRect(ToBeDestroyed);
            this->PagedContainer::DestroyChild(ToBeDestroyed);
        }

        void GridContainer::DestroyAllChildren()
        {
            this->ChildRects.clear();
            this->PagedContainer::DestroyAllChildren();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void GridContainer::ProtoSerializeGridRects(XML::Node& SelfRoot) const
        {
            XML::Node GridRectsNode = SelfRoot.AppendChild( "ChildRects" );

            if( GridRectsNode.AppendAttribute("Version").SetValue("1") ) {
                for( ConstChildRectIterator ChildRectIt = this->ChildRects.begin() ; ChildRectIt != this->ChildRects.end() ; ++ChildRectIt )
                {
                    XML::Node ChildNode = GridRectsNode.AppendChild( "ChildRect" );

                    if( ChildNode.AppendAttribute("Version").SetValue("1") &&
                        ChildNode.AppendAttribute("ChildName").SetValue( (*ChildRectIt).first->GetName() ) )
                    {
                        XML::Node CellRectNode = ChildNode.AppendChild("CellRect");
                        (*ChildRectIt).second.ProtoSerialize(CellRectNode);

                        continue;
                    }else{
                        SerializeError("Create XML Version Attribute","ChildRect",true);
                    }
                }
            }else{
                SerializeError("Create XML Version Attribute","ChildRects",true);
            }
        }

        void GridContainer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->PagedContainer::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( GridContainer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                XML::Node CellSizeNode = PropertiesNode.AppendChild("CellSize");
                this->CellSize.ProtoSerialize(CellSizeNode);

                XML::Node CellPaddingNode = PropertiesNode.AppendChild("CellPadding");
                this->CellPadding.ProtoSerialize(CellPaddingNode);

                return;
            }else{
                SerializeError("Create XML Attribute Values",GridContainer::GetSerializableName() + "Properties",true);
            }
        }

        void GridContainer::ProtoDeSerializeGridRects(const XML::Node& SelfRoot)
        {
            this->ChildRects.clear();

            XML::Attribute CurrAttrib;
            XML::Node ChildRectsNode = SelfRoot.GetChild( "ChildRects" );

            if( !ChildRectsNode.Empty() ) {
                if( ChildRectsNode.GetAttribute("Version").AsInt() == 1 ) {
                    const GridRect EmptyRect;
                    for( XML::NodeIterator ChildRectNodeIt = ChildRectsNode.begin() ; ChildRectNodeIt != ChildRectsNode.end() ; ++ChildRectNodeIt )
                    {
                        if( (*ChildRectNodeIt).GetAttribute("Version").AsInt() == 1 ) {
                            GridRect CellRect;
                            String ChildName;

                            CurrAttrib = (*ChildRectNodeIt).GetAttribute("ChildName");
                            if( !CurrAttrib.Empty() )
                                ChildName = CurrAttrib.AsString();

                            XML::Node CellRectNode = SelfRoot.GetChild("CellRect").GetFirstChild();
                            if( !CellRectNode.Empty() )
                                CellRect.ProtoDeSerialize(CellRectNode);

                            if( !ChildName.empty() && CellRect != EmptyRect ) {
                                Widget* GridChild = this->GetChild(ChildName);
                                if( GridChild != NULL ) {
                                    this->ChildRects.push_back( ChildRectPair(GridChild,CellRect) );
                                }
                            }
                        }else{
                            MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("ChildRects") + ": Not Version 1.");
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + String("ChildRects") + ": Not Version 1.");
                }
            }
        }

        void GridContainer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->PagedContainer::ProtoDeSerializeProperties(SelfRoot);

            //XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( GridContainer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    XML::Node CellSizeNode = PropertiesNode.GetChild("CellSize");
                    this->CellSize.ProtoDeSerialize(CellSizeNode);

                    XML::Node CellPaddingNode = PropertiesNode.GetChild("CellPadding");
                    this->CellPadding.ProtoDeSerialize(CellPaddingNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (GridContainer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,GridContainer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String GridContainer::GetSerializableName()
        {
            return GridContainer::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        ///////////////////////////////////////////////////////////////////////////////
        // GridContainerFactory Methods

        String GridContainerFactory::GetWidgetTypeName() const
            { return GridContainer::TypeName; }

        GridContainer* GridContainerFactory::CreateGridContainer(const String& RendName, Screen* Parent)
            { return new GridContainer(RendName,Parent); }

        GridContainer* GridContainerFactory::CreateGridContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new GridContainer(RendName,RendRect,Parent); }

        GridContainer* GridContainerFactory::CreateGridContainer(const XML::Node& XMLNode, Screen* Parent)
            { return new GridContainer(XMLNode,Parent); }

        Widget* GridContainerFactory::CreateWidget(Screen* Parent)
            { return new GridContainer(Parent); }

        Widget* GridContainerFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateGridContainer(RendName,Parent); }

        Widget* GridContainerFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateGridContainer(RendName,RendRect,Parent); }

        Widget* GridContainerFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateGridContainer(XMLNode,Parent); }

        void GridContainerFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<GridContainer*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
