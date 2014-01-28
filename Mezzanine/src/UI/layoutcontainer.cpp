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
#ifndef _uilayoutcontainer_cpp
#define _uilayoutcontainer_cpp

#include "UI/layoutcontainer.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        const String LayoutContainer::TypeName = "LayoutContainer";

        LayoutContainer::LayoutContainer(Screen* Parent) :
            Widget(Parent)
            {  }

        LayoutContainer::LayoutContainer(const String& RendName, Screen* Parent) :
            Widget(RendName,Parent)
            {  }

        LayoutContainer::LayoutContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent) :
            Widget(RendName,RendRect,Parent)
            {  }

        LayoutContainer::~LayoutContainer()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void LayoutContainer::UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect)
        {
            // Update the personal data first
            this->ActDims = NewSelfRect;

            // Perform the container specific update logic
            this->UpdateContainerDimensionsImpl(OldSelfRect,NewSelfRect);

            /*// Update the children, if any
            if( this->VisibleChildren.empty() == false )
                this->LayoutStrat->Layout(OldSelfRect,NewSelfRect,this->VisibleChildren);//*/

            // We done got icky
            this->_MarkAllLayersDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Child Management

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void LayoutContainer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->Widget::ProtoSerializeProperties(SelfRoot);
        }

        void LayoutContainer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->Widget::ProtoDeSerializeProperties(SelfRoot);
        }

        String LayoutContainer::GetSerializableName()
        {
            return LayoutContainer::TypeName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void LayoutContainer::_AppendRenderDataCascading(ScreenRenderData& RenderData)
        {
            // Update the children based on page positions and the container size so we can grab the proper vertices.
            if(this->VertexCache) {
                if( this->Dirty || this->AllLayersDirty ) {
                    this->VertexCache->Clear();
                    this->_AppendRenderData(*VertexCache);
                    for( VisibleChildIterator ChildIt = this->VisibleChildren.begin() ; ChildIt != this->VisibleChildren.end() ; ++ChildIt )
                    {
                        if( (*ChildIt)->IsVisible() ) {
                            (*ChildIt)->_AppendRenderDataCascading(*VertexCache);
                        }
                    }
                }
                RenderData.Append(*VertexCache);
            }else{
                this->_AppendRenderData(RenderData);
                for( VisibleChildIterator It = this->VisibleChildren.begin() ; It != this->VisibleChildren.end() ; ++It )
                {
                    if( (*It)->IsVisible() ) {
                        (*It)->_AppendRenderDataCascading(RenderData);
                    }
                }
            }
        }
    }//UI
}//Mezzanine

#endif
