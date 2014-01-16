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

#ifndef _uilinearcontainer_cpp
#define _uilinearcontainer_cpp

#include "UI/linearcontainer.h"
#include "UI/layoutstrategy.h"

namespace Mezzanine
{
    namespace UI
    {
        LinearContainer::LinearContainer(Screen* Parent) :
            PagedContainer(Parent),
            ForcedSizingRules(SE_None),
            VisibleChildAlign(UI::LA_TopLeft)
            {  }

        LinearContainer::LinearContainer(const String& RendName, Screen* Parent) :
            PagedContainer(RendName,Parent),
            ForcedSizingRules(SE_None),
            VisibleChildAlign(UI::LA_TopLeft)
            {  }

        LinearContainer::LinearContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            PagedContainer(RendName,RendRect,Parent),
            ForcedSizingRules(SE_None),
            VisibleChildAlign(UI::LA_TopLeft)
            {  }

        LinearContainer::~LinearContainer()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void LinearContainer::SetChildSizing(const SizingInfo& ForcedSize, const Whole Enforcement)
        {
            this->SetChildSize(ForcedSize);
            this->SetChildSizeEnforcement(Enforcement);
        }

        void LinearContainer::SetChildSize(const SizingInfo& ForcedSize)
            { this->ChildSizing = ForcedSize; }

        const SizingInfo& LinearContainer::GetChildSize() const
            { return this->ChildSizing; }

        void LinearContainer::SetChildSizeEnforcement(const Whole Enforcement)
            { this->ForcedSizingRules = Enforcement; }

        Whole LinearContainer::GetChildSizeEnforcement() const
            { return this->ForcedSizingRules; }

        void LinearContainer::SetPadding(const UnifiedDim& Padding)
            { this->LinearPadding = Padding; }

        const UnifiedDim& LinearContainer::GetPadding() const
            { return this->LinearPadding; }

        void LinearContainer::SetVisibleChildAlignment(const UI::LinearAlignment ChildAlign)
            { this->VisibleChildAlign = ChildAlign; }

        UI::LinearAlignment LinearContainer::GetVisibleChildAlignment() const
            { return this->VisibleChildAlign; }

        ///////////////////////////////////////////////////////////////////////////////
        // Child Management

        void LinearContainer::AddChild(Widget* Child)
        {
            if( this->ForcedSizingRules & SE_OnAdd ) {
                Child->SetSizingPolicy( this->ChildSizing );
            }

            this->QuadRenderable::AddChild(Child);
        }

        void LinearContainer::AddChild(Widget* Child, const UInt16 ZOrder)
        {
            this->QuadRenderable::AddChild(Child,ZOrder);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void LinearContainer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->PagedContainer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( LinearContainer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("ForcedSizingRules").SetValue( this->ForcedSizingRules ) &&
                PropertiesNode.AppendAttribute("VisibleChildAlign").SetValue( static_cast<Whole>( this->VisibleChildAlign ) ) )
            {
                XML::Node ChildSizingNode = PropertiesNode.AppendChild("ChildSizing");
                this->ChildSizing.ProtoSerialize( ChildSizingNode );
                XML::Node LinearPaddingNode = PropertiesNode.AppendChild("LinearPadding");
                this->LinearPadding.ProtoSerialize( LinearPaddingNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",LinearContainer::GetSerializableName() + "Properties",true);
            }
        }

        void LinearContainer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->PagedContainer::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( LinearContainer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    // Get the single data type properties
                    CurrAttrib = PropertiesNode.GetAttribute("ForcedSizingRules");
                    if( !CurrAttrib.Empty() )
                        this->ForcedSizingRules = CurrAttrib.AsWhole();

                    CurrAttrib = PropertiesNode.GetAttribute("VisibleChildAlign");
                    if( !CurrAttrib.Empty() )
                        this->VisibleChildAlign = static_cast<UI::LinearAlignment>( CurrAttrib.AsWhole() );

                    // Get the properties that need their own nodes
                    XML::Node ChildSizingNode = PropertiesNode.GetChild("ChildSizing").GetFirstChild();
                    if( !ChildSizingNode.Empty() )
                        this->PositioningPolicy.ProtoDeSerialize(ChildSizingNode);

                    XML::Node LinearPaddingNode = PropertiesNode.GetChild("LinearPadding").GetFirstChild();
                    if( !LinearPaddingNode.Empty() )
                        this->SizingPolicy.ProtoDeSerialize(LinearPaddingNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (PagedContainer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,PagedContainer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String LinearContainer::GetSerializableName()
        {
            return "LinearContainer";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods
    }//UI
}//Mezzanine

#endif
