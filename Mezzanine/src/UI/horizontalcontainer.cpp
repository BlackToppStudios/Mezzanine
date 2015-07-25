// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#include <algorithm>

namespace Mezzanine
{
    namespace UI
    {
        const String HorizontalContainer::TypeName = "HorizontalContainer";

        ///////////////////////////////////////////////////////////////////////////////
        // HorizontalContainer Methods

        HorizontalContainer::HorizontalContainer(Screen* Parent) :
            LinearContainer(Parent)
            {  }

        HorizontalContainer::HorizontalContainer(const String& RendName, Screen* Parent) :
            LinearContainer(RendName,Parent)
            {  }

        HorizontalContainer::HorizontalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            LinearContainer(RendName,RendRect,Parent)
            {  }

        HorizontalContainer::HorizontalContainer(const XML::Node& XMLNode, Screen* Parent) :
            LinearContainer(Parent)
            { this->ProtoDeSerialize(XMLNode); }

        HorizontalContainer::~HorizontalContainer()
            {  }

        void HorizontalContainer::UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Clear our old data.
            this->VisibleChildren.clear();
            this->WorkAreaSize.SetIdentity();

            // Setup any additional data for the next series of loops.
            const Real ActPadding = this->LinearPadding.CalculateActualDimension( this->ActDims.Size.X );
            const Real HalfPadding = ActPadding * 0.5;
            std::vector< std::pair<Real,Vector2> > ChildTransformCache( this->ChildWidgets.size() );

            // To determine the visible children we need to know our view position in the work area as determined by the page provider.
            // To determine our page position we need to know our work area size, which can change when children have their dimensions updated.
            // So we first need to update the size on every child.  We can update our work area at the same time.  After that we can update our page provider and determine screen positions.
            Whole CurrIndex = 0;
            this->WorkAreaSize.X = HalfPadding;
            for( ChildIterator SizeIt = this->ChildWidgets.begin() ; SizeIt != this->ChildWidgets.end() ; ++SizeIt )
            {
                // Enforce our sizing rules.
                if( this->ForcedSizingRules & SE_OnUpdate ) {
                    (*SizeIt)->SetSizingPolicy( this->ChildSizing );
                }

                // Calculate the sizing with our utility strat, and offset position which we will use later.
                // Since the constantly updating work area size is tracking the same information as a cursor would, we'll use it as our position cursor.
                Vector2 ChildSize = this->LayoutStrat->HandleChildSizing(OldSelfRect,NewSelfRect,(*SizeIt));

                ChildTransformCache.at(CurrIndex).first = this->WorkAreaSize.X;
                ChildTransformCache.at(CurrIndex).second = ChildSize;

                // Update the cursor position and work area size.
                this->WorkAreaSize.X += ( ChildSize.X + ActPadding );
                if( this->WorkAreaSize.Y < ChildSize.Y )
                    this->WorkAreaSize.Y = ChildSize.Y;

                // Increment the index before we proceed.
                ++CurrIndex;
            }
            // Trim off half a padding at the end since we were blinding applying full padding for the space between two children.
            this->WorkAreaSize.X -= HalfPadding;

            // Now our work area is updated, so we can update our provider and get our target work area position.
            Real XTarget = 0;
            if( this->XProvider != NULL ) {
                this->XProvider->_NotifyContainerUpdated();
                Real XTargetLimit = ( this->WorkAreaSize.X + HalfPadding ) - NewSelfRect.Size.X;
                XTarget = std::min( ( this->XProvider->GetCurrentXPage() - 1 ) * NewSelfRect.Size.X, XTargetLimit );
            }

            // Set up our data for the loop (and the loop itself) that will go over all the children that will be "above" the visible children.
            // Reset the index since we are starting from the beginning again.
            CurrIndex = 0;
            ChildIterator ChildUpdateIt = this->ChildWidgets.begin();
            while( ChildUpdateIt != this->ChildWidgets.end() && ChildTransformCache.at(CurrIndex).first < XTarget )
            {
                const Rect OldChildRect = (*ChildUpdateIt)->GetRect();
                Rect NewChildRect;

                // Assign a dummy position since this will be invisible
                NewChildRect.Position = NewSelfRect.Position;
                NewChildRect.Size = ChildTransformCache.at(CurrIndex).second;

                // Perform the update
                (*ChildUpdateIt)->UpdateDimensions(OldChildRect,NewChildRect);

                // Hide the child
                (*ChildUpdateIt)->Hide();

                // Increment the iterator and index before we proceed.
                ++CurrIndex;
                ++ChildUpdateIt;
            }

            // If we've reached the end of the children, there is nothing to render.
            if( ChildUpdateIt != this->ChildWidgets.end() )
            {
                // If we're here, then we have some visible children.
                // Set our new target and create a variable that will track the total size of just the visible children.
                XTarget = NewSelfRect.Size.X;
                Real TotalLinearSize = HalfPadding;

                // Before we start a loop that will be altering the index, we need to mark the index of the first visible child in our cache.  This will be used in the final processing step.
                // Once saved, start looping over what will be the visible children.  We can (and should) use the index from the previous loop unaltered.
                const Whole VisibleStartIndex = CurrIndex;
                while( ChildUpdateIt != this->ChildWidgets.end() )
                {
                    Vector2 ChildSize = ChildTransformCache.at(CurrIndex).second;
                    // See if our child can fit
                    if( TotalLinearSize + ChildSize.X < XTarget ) {
                        this->VisibleChildren.push_back( (*ChildUpdateIt) );
                        TotalLinearSize += ( ChildSize.X + ActPadding );
                        // Increment the index and iterator before we proceed.
                        ++CurrIndex;
                        ++ChildUpdateIt;
                    }else{
                        break;
                    }
                }

                // Any remaining children will be invisible, so put them in the corner with the rest.
                while( ChildUpdateIt != this->ChildWidgets.end() )
                {
                    const Rect OldChildRect = (*ChildUpdateIt)->GetRect();
                    Rect NewChildRect;

                    // Assign a dummy position since this will be invisible
                    NewChildRect.Position = NewSelfRect.Position;
                    NewChildRect.Size = ChildTransformCache.at(CurrIndex).second;

                    // Perform the update
                    (*ChildUpdateIt)->UpdateDimensions(OldChildRect,NewChildRect);

                    // Hide the child
                    (*ChildUpdateIt)->Hide();

                    // Increment the iterator and index before we proceed.
                    ++CurrIndex;
                    ++ChildUpdateIt;
                }

                // Create the cursor which will be used for visible child placement.
                Real CurrXPos = 0;
                // By now all our children have been processed, just have to focus on the last couple steps for visible children.  Start by doing our alignment stuff.
                switch( this->VisibleChildAlign )
                {
                    case UI::LA_TopLeft:      CurrXPos = ( NewSelfRect.Position.X + HalfPadding );                                               break;
                    case UI::LA_Center:       CurrXPos = ( NewSelfRect.Position.X + ( NewSelfRect.Size.X * 0.5 ) ) - ( TotalLinearSize * 0.5 );  break;
                    case UI::LA_BottomRight:  CurrXPos = ( NewSelfRect.Position.X + NewSelfRect.Size.X ) - ( TotalLinearSize + HalfPadding );    break;
                }

                // Finally, we're at the final processing stage.  We have our visible children and enough data to determine their positions, along with their saved sizes from earlier.
                for( Whole VisIndex = 0 ; VisIndex < this->VisibleChildren.size() ; ++VisIndex )
                {
                    Widget* VisChild = this->VisibleChildren.at(VisIndex);
                    // Setup child transform data
                    const Rect OldChildRect = VisChild->GetRect();
                    Rect NewChildRect;
                    // Set the Size
                    NewChildRect.Size = ChildTransformCache.at(VisibleStartIndex + VisIndex).second;
                    // Set the Position
                    NewChildRect.Position.X = CurrXPos;
                    NewChildRect.Position.Y = this->LayoutStrat->HandleChildHorizontalPositioning( OldSelfRect, NewSelfRect, NewChildRect.Size, VisChild );
                    // Perform the update
                    VisChild->UpdateDimensions(OldChildRect,NewChildRect);
                    // Increment the cursor
                    CurrXPos += ( NewChildRect.Size.X + ActPadding );
                    // Finally show the child
                    VisChild->SetVisible( this->GetVisible() );
                }
                ChildTransformCache.clear();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void HorizontalContainer::UpdateWorkAreaSize()
        {
            this->WorkAreaSize.SetIdentity();
            const Real ActPadding = this->LinearPadding.CalculateActualDimension( this->ParentQuad ? this->ParentQuad->GetActualSize().X : 0 );
            const Real HalfPadding = ActPadding * 0.5;

            for( ChildIterator ChildIt = this->ChildWidgets.begin() ; ChildIt != this->ChildWidgets.end() ; ++ChildIt )
            {
                Vector2 ChildSize = (*ChildIt)->GetActualSize();
                this->WorkAreaSize.X += ( ChildSize.X + HalfPadding );
                if( this->WorkAreaSize.Y < ChildSize.Y )
                    this->WorkAreaSize.Y = ChildSize.Y;
            }
            this->WorkAreaSize.X += HalfPadding;
        }

        void HorizontalContainer::QuickUpdateWorkAreaSize(const UnifiedVec2& ChildSize, Boole Adding)
        {
            const Vector2 ActChildSize = ChildSize.CalculateActualDimensions( this->ActDims.Size );
            const Real ActPadding = this->LinearPadding.CalculateActualDimension( this->ParentQuad ? this->ParentQuad->GetActualSize().X : 0 );
            const Real HalfPadding = ActPadding * 0.5;

            if( Adding ) {
                this->WorkAreaSize.X += ( ActChildSize.X + HalfPadding );
                if( this->WorkAreaSize.Y < ActChildSize.Y )
                    this->WorkAreaSize.Y = ActChildSize.Y;
            }else{
                this->WorkAreaSize.X -= ( ActChildSize.X + HalfPadding );
            }
        }

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
        {
            HorizontalContainer* Ret = new HorizontalContainer(RendName,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        HorizontalContainer* HorizontalContainerFactory::CreateHorizontalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent)
        {
            HorizontalContainer* Ret = new HorizontalContainer(RendName,RendRect,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        HorizontalContainer* HorizontalContainerFactory::CreateHorizontalContainer(const XML::Node& XMLNode, Screen* Parent)
        {
            HorizontalContainer* Ret = new HorizontalContainer(XMLNode,Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

        Widget* HorizontalContainerFactory::CreateWidget(Screen* Parent)
        {
            HorizontalContainer* Ret = new HorizontalContainer(Parent);
            Ret->_SetLayoutStrat( new LayoutStrategy() );
            return Ret;
        }

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
