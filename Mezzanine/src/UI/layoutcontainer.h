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
#ifndef _uilayoutcontainer_h
#define _uilayoutcontainer_h

#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for container objects that automatically position it's children.
        /// @details The central concept to all layout containers is that the transforms on their children can be partially or
        /// entirely generated automatically.  Depending on the LayoutContainer implementation setting the transforms of children
        /// will either be ignored, or cause errors.  LayoutContainers are designed with an unknown amount of children in mind. @n @n
        /// Also as a result of the level of control over the child transforms and the complexity of data that could go into generating
        /// them, new LayoutStrategy classes are generally not defined for LayoutContainers.  Attempting to do so would mean highly
        /// specific implementations to handle each containers data which breaks the design intent of generic transform updates that
        /// LayoutStrategy classes usually have, making sharing the logic impractical.  Instead, parts of the layout logic is built
        /// into the class implementation and other parts using specific methods on the LayoutStrategy base class.  So sharing of
        /// exact layout logic doesn't occur like it might in other widgets.
        ///////////////////////////////////////
        class MEZZ_LIB LayoutContainer : public Widget
        {
        public:
            /// @brief Basic container type for Visible @ref QuadRenderable storage by this class.
            typedef std::vector<QuadRenderable*>           VisibleChildContainer;
            /// @brief Iterator type for Visible @ref QuadRenderable instances stored by this class.
            typedef VisibleChildContainer::iterator        VisibleChildIterator;
            /// @brief Const Iterator type for Visible @ref QuadRenderable instances stored by this class.
            typedef VisibleChildContainer::const_iterator  ConstVisibleChildIterator;

            /// @brief String containing the type name for this class: "LayoutContainer".
            static const String TypeName;
        protected:
            /// @internal
            /// @brief A container of children that meet the criteria for rendering in this container.
            VisibleChildContainer VisibleChildren;

            /// @internal
            /// @brief The container specific logic for updating it's dimensions.
            virtual void UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect) = 0;
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            LayoutContainer(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            LayoutContainer(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            LayoutContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            virtual ~LayoutContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc QuadRenderable::UpdateDimensions(const Rect&, const Rect&)
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc QuadRenderable::_AppendRenderDataCascading(ScreenRenderData&)
            virtual void _AppendRenderDataCascading(ScreenRenderData& RenderData);
        };//LayoutContainer
    }//UI
}//Mezzanine

#endif
