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
#ifndef _uiverticalcontainer_cpp
#define _uiverticalcontainer_cpp

#include "UI/verticalcontainer.h"
#include "UI/verticallayoutstrategy.h"

#include "UI/verticalscrollbar.h"
#include "UI/spinner.h"

namespace Mezzanine
{
    namespace UI
    {
        const String VerticalContainer::TypeName = "VerticalContainer";

        ///////////////////////////////////////////////////////////////////////////////
        // VerticalContainer Methods

        VerticalContainer::VerticalContainer(Screen* Parent) :
            LinearContainer(Parent)
            { this->LayoutStrat = new LayoutStrategy(); }

        VerticalContainer::VerticalContainer(const String& RendName, Screen* Parent) :
            LinearContainer(RendName,Parent)
            { this->LayoutStrat = new LayoutStrategy(); }

        VerticalContainer::VerticalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            LinearContainer(RendName,RendRect,Parent)
            { this->LayoutStrat = new LayoutStrategy(); }

        VerticalContainer::VerticalContainer(const XML::Node& XMLNode, Screen* Parent) :
            LinearContainer(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        VerticalContainer::~VerticalContainer()
            { delete this->LayoutStrat; }

        void VerticalContainer::UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Get the actual work area dimensions, which should already be updated
            const Rect ParentRect = this->ParentQuad->GetRect();
            const Vector2 ActWorkArea = this->WorkAreaSize.CalculateActualDimensions( ParentRect.Size );

            // Update the provider data if needed
            this->YProvider->_NotifyContainerUpdated();

            // Pre-check setup
            const Real ActPadding = this->LinearPadding.CalculateActualDimension( ParentRect.Size.Y );
            Real YTarget = ( this->YProvider->GetCurrentYPage() - 1 ) * ActWorkArea.Y;
            Real CurrYPos = ActPadding * 0.5;
            this->VisibleChildren.clear();

            // Find the first one to get rendered going through by ZOrder
            ChildIterator ChildUpdateIt = this->ChildWidgets.begin();
            while( ChildUpdateIt != this->ChildWidgets.end() && CurrYPos < YTarget )
            {
                this->UpdateInvisibleChild( OldSelfRect, NewSelfRect, (*ChildUpdateIt) );
                // Increment our 'cursor' position
                CurrYPos += ( (*ChildUpdateIt)->GetActualSize().Y + ActPadding );
                ++ChildUpdateIt;
            }

            // If we've reached the end of the children, there is nothing to render
            if( ChildUpdateIt != this->ChildWidgets.end() )
            {
                // Setup any additional data for the next series of loops
                std::vector<Vector2> ChildSizeCache;
                Real TotalLinearSize = 0;

                // Also reset our cursor and target so we can better utilize the viewable space
                CurrYPos = ActPadding * 0.5;
                YTarget = NewSelfRect.Size.Y;

                while( ChildUpdateIt != this->ChildWidgets.end() /*&& CurrYPos < YTarget */ )
                {
                    // Due to having to abide by certain positioning rules, we can't determine the position until we have all the visible children
                    // So just find the size, see if we're passed the mark and if not then save it for later
                    Vector2 ChildSize = this->LayoutStrat->HandleChildSizing( OldSelfRect, NewSelfRect, (*ChildUpdateIt) );
                    // See if our child can fit
                    if( CurrYPos + ChildSize.Y < YTarget ) {
                        // Clamp the perpendicular axis, at least until we get proper expanding widgets
                        if( ChildSize.X > NewSelfRect.Size.X ) {
                            ChildSize.X = NewSelfRect.Size.X;
                        }
                        ChildSizeCache.push_back( ChildSize );
                        this->VisibleChildren.push_back( (*ChildUpdateIt) );
                        CurrYPos += ( ChildSize.Y + ActPadding );
                        TotalLinearSize += ( ChildSize.Y + ActPadding );
                    }else{
                        break;
                    }
                }

                // Set the remaining children (if any) in the corner with the rest
                while( ChildUpdateIt != this->ChildWidgets.end() )
                {
                    this->UpdateInvisibleChild( OldSelfRect, NewSelfRect, (*ChildUpdateIt) );
                    ++ChildUpdateIt;
                }

                // By now all our children have been processed, just have to focus on the last couple steps for visible children
                // Start by doing our alignment stuff, and trim off one increment of ActPadding since we were blindly applying it earlier
                TotalLinearSize -= ActPadding;
                switch( this->VisibleChildAlign )
                {
                    case UI::LA_TopLeft:      CurrYPos = ActPadding * 0.5;
                    case UI::LA_Center:       CurrYPos = ( NewSelfRect.Position.Y + ( NewSelfRect.Size.Y * 0.5 ) ) - ( TotalLinearSize * 0.5 );
                    case UI::LA_BottomRight:  CurrYPos = ( NewSelfRect.Position.Y + NewSelfRect.Size.Y ) - ( TotalLinearSize + ActPadding * 0.5);
                }

                for( Whole Y = 0 ; Y < this->VisibleChildren.size() ; ++Y )
                {
                    // Setup child transform data
                    const Rect OldChildRect = this->VisibleChildren.at(Y)->GetRect();
                    Rect NewChildRect;
                    // Set the Size
                    NewChildRect.Size = ChildSizeCache.at(Y);
                    // Set the Position
                    NewChildRect.Position.X = this->LayoutStrat->HandleChildHorizontalPositioning( OldSelfRect, NewSelfRect, NewChildRect.Size, this->VisibleChildren.at(Y) );
                    NewChildRect.Position.Y = CurrYPos;
                    // Perform the update
                    this->VisibleChildren.at(Y)->UpdateDimensions(OldChildRect,NewChildRect);
                    // Increment the cursor
                    CurrYPos += ( NewChildRect.Size.Y + ActPadding );
                }
                ChildSizeCache.clear();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& VerticalContainer::GetTypeName() const
            { return VerticalContainer::TypeName; }

        void VerticalContainer::SetProviders(PageProvider* XProv, PageProvider* YProv)
            { this->SetYProvider(YProv); }

        void VerticalContainer::SetXProvider(PageProvider* XProv)
            { /* Do Nothing */ }

        void VerticalContainer::SetYProvider(PageProvider* YProv)
        {
            if( this->YProvider != NULL && this->YProvider != YProv ) {
                this->UnbindProvider(this->YProvider);
            }
            this->YProvider = YProv;
            this->YProvider->_SetContainer(this);
            this->YProvider->_NotifyContainerUpdated();
        }

        void VerticalContainer::SetScrollbarProvider(VerticalScrollbar* Prov)
            { this->SetYProvider(Prov); }

        void VerticalContainer::SetSpinnerProvider(Spinner* Prov)
            { this->SetProviders(Prov,Prov); }

        ///////////////////////////////////////////////////////////////////////////////
        // Child Management

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void VerticalContainer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->LinearContainer::ProtoSerializeProperties(SelfRoot);
        }

        void VerticalContainer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->LinearContainer::ProtoDeSerializeProperties(SelfRoot);
        }

        String VerticalContainer::GetSerializableName()
        {
            return VerticalContainer::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        ///////////////////////////////////////////////////////////////////////////////
        // VerticalContainerFactory Methods

        String VerticalContainerFactory::GetWidgetTypeName() const
            { return VerticalContainer::TypeName; }

        VerticalContainer* VerticalContainerFactory::CreateVerticalContainer(const String& RendName, Screen* Parent)
            { return new VerticalContainer(RendName,Parent); }

        VerticalContainer* VerticalContainerFactory::CreateVerticalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new VerticalContainer(RendName,RendRect,Parent); }

        VerticalContainer* VerticalContainerFactory::CreateVerticalContainer(const XML::Node& XMLNode, Screen* Parent)
            { return new VerticalContainer(XMLNode,Parent); }

        Widget* VerticalContainerFactory::CreateWidget(Screen* Parent)
            { return new VerticalContainer(Parent); }

        Widget* VerticalContainerFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateVerticalContainer(RendName,Parent); }

        Widget* VerticalContainerFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateVerticalContainer(RendName,RendRect,Parent); }

        Widget* VerticalContainerFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateVerticalContainer(XMLNode,Parent); }

        void VerticalContainerFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<VerticalContainer*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
