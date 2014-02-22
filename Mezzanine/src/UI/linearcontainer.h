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

#ifndef _uilinearcontainer_h
#define _uilinearcontainer_h

#include "UI/pagedcontainer.h"

namespace Mezzanine
{
    namespace UI
    {
        class LinearContainerLayoutStrategy;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a container class for placing child objects in succession on a single axis.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB LinearContainer : public PagedContainer
        {
        public:
            /// @enum SizeEnforcement
            /// @brief This enum is used to determine if and when a child should be forced to a specific size for this container.
            enum SizeEnforcement
            {
                SE_None = 0,     ///< No sizing enforcement of any kind is to occur to the children of this container.
                SE_OnAdd = 1,    ///< The preset size for children of this container will only be applied when they are added to this container, ensuring the same start size for all children.
                SE_OnUpdate = 2  ///< The preset size for children of this container will be enforced constantly on each update, preventing children from being rendered at any other size.
            };
        protected:
            /// @internal
            /// @brief The size given to children layed out by this container.
            SizingInfo ChildSizing;
            /// @internal
            /// @brief The amount of space to add to both sides of a child object on the relevant axis.
            UnifiedDim LinearPadding;
            /// @internal
            /// @brief The current enforcement for child size in this container.
            Whole ForcedSizingRules;
            /// @internal
            /// @brief The alignment to be given to the sequence of children visible in this container.
            UI::LinearAlignment VisibleChildAlign;
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            LinearContainer(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            LinearContainer(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            LinearContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            virtual ~LinearContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets both the size and enforcement rules for forced child sizing in this container.
            /// @note The sizing provided here works just like any other normal child sizing does, with the relative parts being based on the the parent container (this).
            /// @param ForcedSize A SizingInfo describing how all children should size themselves on dimension updates.
            /// @param Enforcement A bitmask containing when the child sizing will be enforced by this container.  See SizeEnforcement enum for more details.
            virtual void SetChildSizing(const SizingInfo& ForcedSize, const Whole Enforcement);
            /// @brief Sets the size to be given to children processed by this container if forced sizing is enabled.
            /// @note The sizing provided here works just like any other normal child sizing does, with the relative parts being based on the the parent container (this).
            /// @param ForcedSize A SizingInfo describing how all children should size themselves on dimension updates.
            virtual void SetChildSize(const SizingInfo& ForcedSize);
            /// @brief Gets the size to be given to children processed by this container if forced sizing is enabled.
            /// @return Returns a const SizeInfo reference to the sizing given to children processed by this container if forced sizing is enabled.
            virtual const SizingInfo& GetChildSize() const;
            /// @brief Sets when the set child sizing will be applied to any given child.
            /// @param Enforcement A bitmask containing when the child sizing will be enforced by this container.  See SizeEnforcement enum for more details.
            virtual void SetChildSizeEnforcement(const Whole Enforcement);
            /// @brief Gets when the set child sizing will be applied to any given child.
            /// @return Returns a bitmask describing when the child sizing will be enforced by this container.  See SizeEnforcement enum for more details.
            virtual Whole GetChildSizeEnforcement() const;

            /// @brief Sets the spacing to be split between both sides of each child quad on the relevant axis.
            /// @note The unified dimension passed in is relative to the parent container(this)
            /// @param Padding The amount of space to apply on both sides of each child.
            virtual void SetPadding(const UnifiedDim& Padding);
            /// @brief Gets the spacing being split between both sides of each child quad on the relevant axis.
            /// @return Returns a const UnifiedDim reference representing the amount of space to apply on both sides of each child.
            virtual const UnifiedDim& GetPadding() const;

            /// @brief Sets the alignment for visible children of this container when they don't take up all the visible space provided by the container.
            /// @param ChildAlign The alignment to be given to visible children of this linear container.
            virtual void SetVisibleChildAlignment(const UI::LinearAlignment ChildAlign);
            /// @brief Gets the alignment for visible children of this container when they don't take up all the visible space provided by the container.
            /// @return Returns a LinearAlignment value representing the alignment of visible children of this linear container.
            virtual UI::LinearAlignment GetVisibleChildAlignment() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            /// @copydoc QuadRenderable::AddChild(Widget*)
            virtual void AddChild(Widget* Child);
            /// @copydoc QuadRenderable::AddChild(Widget*,const UInt16)
            virtual void AddChild(Widget* Child, const UInt16 ZOrder);

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
        };//LinearContainer
    }//UI
}//Mezzanine

#endif
