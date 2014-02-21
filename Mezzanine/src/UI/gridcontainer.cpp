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

namespace Mezzanine
{
    namespace UI
    {
        const String GridContainer::TypeName              = "GridContainer";

        ///////////////////////////////////////////////////////////////////////////////
        // GridContainer Methods

        GridContainer::GridContainer(Screen* Parent) :
            PagedContainer(Parent)
        {

        }

        GridContainer::GridContainer(const String& RendName, Screen* Parent) :
            PagedContainer(RendName,Parent)
        {

        }

        GridContainer::GridContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            PagedContainer(RendName,RendRect,Parent)
        {

        }

        GridContainer::~GridContainer()
        {

        }

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

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void GridContainer::UpdateWorkAreaSize()
        {

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

        ///////////////////////////////////////////////////////////////////////////////
        // Child Management

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
                        //(*ChildRectIt).second.ProtoSerialize(CellRectNode);

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
    }//UI
}//Mezzanine

#endif
