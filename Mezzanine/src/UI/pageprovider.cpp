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

#ifndef _uipageprovider_cpp
#define _uipageprovider_cpp

#include "UI/pageprovider.h"
#include "UI/pagedcontainer.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        PageProvider::PageProvider(Screen* Parent) :
            Widget(Parent),
            Container(NULL)
            {  }

        PageProvider::PageProvider(const String& RendName, Screen* Parent) :
            Widget(RendName,Parent),
            Container(NULL)
            {  }

        PageProvider::PageProvider(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Widget(RendName,RendRect,Parent),
            Container(NULL)
            {  }

        PageProvider::~PageProvider()
            {  }

        void PageProvider::ProtoSerializeImpl(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeImpl(SelfRoot);
            this->ProtoSerializePageData(SelfRoot);
        }

        void PageProvider::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeImpl(SelfRoot);
            this->ProtoDeSerializePageData(SelfRoot);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void PageProvider::ProtoSerializePageData(XML::Node& SelfRoot) const
        {
            if( this->Container != NULL ) {
                XML::Node PageDataNode = SelfRoot.AppendChild( PageProvider::GetSerializableName() + "PageData" );

                if( PageDataNode.AppendAttribute("Version").SetValue("1") &&
                    PageDataNode.AppendAttribute("ContainerName").SetValue( this->Container->GetName() ) &&
                    PageDataNode.AppendAttribute("ProviderConfig").SetValue( static_cast<Whole>( this->Container->GetProviderConfig(this) ) ) )
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values",PageProvider::GetSerializableName() + "PageData",true);
                }
            }
        }

        void PageProvider::ProtoDeSerializePageData(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node PageDataNode = SelfRoot.GetChild( PageProvider::GetSerializableName() + "PageData" );

            if( !PageDataNode.Empty() ) {
                if(PageDataNode.GetAttribute("Version").AsInt() == 1) {
                    String ContainerName;
                    PagedContainer::ProviderMode ProviderConfig;

                    CurrAttrib = PageDataNode.GetAttribute("ContainerName");
                    if( !CurrAttrib.Empty() )
                        ContainerName = CurrAttrib.AsString();

                    CurrAttrib = PageDataNode.GetAttribute("ProviderConfig");
                    if( !CurrAttrib.Empty() )
                        ProviderConfig = static_cast<PagedContainer::ProviderMode>( CurrAttrib.AsWhole() );

                    if( !ContainerName.empty() ) {
                        PagedContainer* ToSet = static_cast<PagedContainer*>( this->ParentScreen->GetWidget(ContainerName) );
                        if( ToSet != NULL ) {
                            switch( ProviderConfig )
                            {
                                case PagedContainer::PM_Single_X:   ToSet->SetXProvider(this);       break;
                                case PagedContainer::PM_Single_Y:   ToSet->SetYProvider(this);       break;
                                case PagedContainer::PM_Single_XY:  ToSet->SetProviders(this,this);  break;
                                default:                            /* Do Nothing */                 break;
                            }
                        }
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (PageProvider::GetSerializableName() + "PageData") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,PageProvider::GetSerializableName() + "PageData" + " was not found in the provided XML node, which was expected.");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void PageProvider::_SetContainer(PagedContainer* ToUpdate)
            { this->Container = ToUpdate; }
    }//UI
}//Mezzanine

#endif
