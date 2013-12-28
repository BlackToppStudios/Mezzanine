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
#ifndef _uihorizontalcontainer_cpp
#define _uihorizontalcontainer_cpp

#include "UI/horizontalcontainer.h"
#include "UI/horizontallayoutstrategy.h"

#include "UI/horizontalscrollbar.h"
#include "UI/spinner.h"

namespace Mezzanine
{
    namespace UI
    {
        const String HorizontalContainer::TypeName = "HorizontalContainer";

        ///////////////////////////////////////////////////////////////////////////////
        // HorizontalContainer Methods

        HorizontalContainer::HorizontalContainer(Screen* Parent) :
            LinearContainer(Parent)
            { this->LayoutStrat = new LayoutStrategy(); }

        HorizontalContainer::HorizontalContainer(const String& RendName, Screen* Parent) :
            LinearContainer(RendName,Parent)
            { this->LayoutStrat = new LayoutStrategy(); }

        HorizontalContainer::HorizontalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            LinearContainer(RendName,RendRect,Parent)
            { this->LayoutStrat = new LayoutStrategy(); }

        HorizontalContainer::HorizontalContainer(const XML::Node& XMLNode, Screen* Parent) :
            LinearContainer(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        HorizontalContainer::~HorizontalContainer()
            { delete this->LayoutStrat; }

        void HorizontalContainer::UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Get the actual work area dimensions, which should already be updated
            const Rect ParentRect = this->ParentQuad->GetRect();
            const Vector2 ActWorkArea = this->WorkAreaSize.CalculateActualDimensions( ParentRect.Size );

            // Update the provider data if needed
            this->XProvider->_NotifyContainerUpdated();

            // Pre-check setup
            const Real ActPadding = this->LinearPadding.CalculateActualDimension( ParentRect.Size.X );
            Real XTarget = ( this->XProvider->GetCurrentXPage() - 1 ) * ActWorkArea.X;
            Real CurrXPos = ActPadding * 0.5;
            this->VisibleChildren.clear();

            // Find the first one to get rendered going through by ZOrder
            ChildIterator ChildUpdateIt = this->ChildWidgets.begin();
            while( ChildUpdateIt != this->ChildWidgets.end() && CurrXPos < XTarget )
            {
                this->UpdateInvisibleChild( OldSelfRect, NewSelfRect, (*ChildUpdateIt) );
                // Increment our 'cursor' position
                CurrXPos += ( (*ChildUpdateIt)->GetActualSize().X + ActPadding );
                ++ChildUpdateIt;
            }

            // If we've reached the end of the children, there is nothing to render
            if( ChildUpdateIt != this->ChildWidgets.end() )
            {
                // Setup any additional data for the next series of loops
                std::vector<Vector2> ChildSizeCache;
                Real TotalLinearSize = 0;

                // Also reset our cursor and target so we can better utilize the viewable space
                CurrXPos = ActPadding * 0.5;
                XTarget = NewSelfRect.Size.X;

                while( ChildUpdateIt != this->ChildWidgets.end() /*&& CurrXPos < XTarget */ )
                {
                    // Due to having to abide by certain positioning rules, we can't determine the position until we have all the visible children
                    // So just find the size, see if we're passed the mark and if not then save it for later
                    Vector2 ChildSize = this->LayoutStrat->HandleChildSizing( OldSelfRect, NewSelfRect, (*ChildUpdateIt) );
                    // See if our child can fit
                    if( CurrXPos + ChildSize.X < XTarget ) {
                        // Clamp the perpendicular axis, at least until we get proper expanding widgets
                        if( ChildSize.Y > NewSelfRect.Size.Y ) {
                            ChildSize.Y = NewSelfRect.Size.Y;
                        }
                        ChildSizeCache.push_back( ChildSize );
                        this->VisibleChildren.push_back( (*ChildUpdateIt) );
                        CurrXPos += ( ChildSize.X + ActPadding );
                        TotalLinearSize += ( ChildSize.X + ActPadding );
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
                    case UI::LA_TopLeft:      CurrXPos = ActPadding * 0.5;
                    case UI::LA_Center:       CurrXPos = ( NewSelfRect.Position.X + ( NewSelfRect.Size.X * 0.5 ) ) - ( TotalLinearSize * 0.5 );
                    case UI::LA_BottomRight:  CurrXPos = ( NewSelfRect.Position.X + NewSelfRect.Size.X ) - ( TotalLinearSize + ActPadding * 0.5);
                }

                for( Whole X = 0 ; X < this->VisibleChildren.size() ; ++X )
                {
                    // Setup child transform data
                    const Rect OldChildRect = this->VisibleChildren.at(X)->GetRect();
                    Rect NewChildRect;
                    // Set the Size
                    NewChildRect.Size = ChildSizeCache.at(X);
                    // Set the Position
                    NewChildRect.Position.X = CurrXPos;
                    NewChildRect.Position.Y = this->LayoutStrat->HandleChildVerticalPositioning( OldSelfRect, NewSelfRect, NewChildRect.Size, this->VisibleChildren.at(X) );
                    // Perform the update
                    this->VisibleChildren.at(X)->UpdateDimensions(OldChildRect,NewChildRect);
                    // Increment the cursor
                    CurrXPos += ( NewChildRect.Size.X + ActPadding );
                }
                ChildSizeCache.clear();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& HorizontalContainer::GetTypeName() const
            { return HorizontalContainer::TypeName; }

        void HorizontalContainer::SetProviders(PageProvider* XProv, PageProvider* YProv)
            { this->SetXProvider(XProv); }

        void HorizontalContainer::SetXProvider(PageProvider* XProv)
        {
            if( this->XProvider != NULL && this->XProvider != XProv ) {
                this->UnbindProvider(this->XProvider);
            }
            this->XProvider = XProv;
            this->XProvider->_SetContainer(this);
            this->XProvider->_NotifyContainerUpdated();
        }

        void HorizontalContainer::SetYProvider(PageProvider* YProv)
            { /* Do Nothing */ }

        void HorizontalContainer::SetScrollbarProvider(HorizontalScrollbar* Prov)
            { this->SetXProvider(Prov); }

        void HorizontalContainer::SetSpinnerProvider(Spinner* Prov)
            { this->SetProviders(Prov,Prov); }

        ///////////////////////////////////////////////////////////////////////////////
        // Child Management

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void HorizontalContainer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->LinearContainer::ProtoSerializeProperties(SelfRoot);
        }

        void HorizontalContainer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->LinearContainer::ProtoDeSerializeProperties(SelfRoot);
        }

        String HorizontalContainer::GetSerializableName()
        {
            return HorizontalContainer::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        ///////////////////////////////////////////////////////////////////////////////
        // HorizontalContainerFactory Methods

        String HorizontalContainerFactory::GetWidgetTypeName() const
            { return HorizontalContainer::TypeName; }

        HorizontalContainer* HorizontalContainerFactory::CreateHorizontalContainer(const String& RendName, Screen* Parent)
            { return new HorizontalContainer(RendName,Parent); }

        HorizontalContainer* HorizontalContainerFactory::CreateHorizontalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
            { return new HorizontalContainer(RendName,RendRect,Parent); }

        HorizontalContainer* HorizontalContainerFactory::CreateHorizontalContainer(const XML::Node& XMLNode, Screen* Parent)
            { return new HorizontalContainer(XMLNode,Parent); }

        Widget* HorizontalContainerFactory::CreateWidget(Screen* Parent)
            { return new HorizontalContainer(Parent); }

        Widget* HorizontalContainerFactory::CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateHorizontalContainer(RendName,Parent); }

        Widget* HorizontalContainerFactory::CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent)
            { return this->CreateHorizontalContainer(RendName,RendRect,Parent); }

        Widget* HorizontalContainerFactory::CreateWidget(const XML::Node& XMLNode, Screen* Parent)
            { return this->CreateHorizontalContainer(XMLNode,Parent); }

        void HorizontalContainerFactory::DestroyWidget(Widget* ToBeDestroyed)
            { delete static_cast<HorizontalContainer*>( ToBeDestroyed ); }
    }//UI
}//Mezzanine

#endif
