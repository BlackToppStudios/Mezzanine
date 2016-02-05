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
#ifndef _uilayoutstrategy_h
#define _uilayoutstrategy_h

#include "UI/rect.h"

namespace Mezzanine
{
    namespace UI
    {
        class Widget;
        class QuadRenderable;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for the algorithms used by QuadRenderables to determine how they should update their dimensions.
        /// @details This class has default and implementations for the algorithms that handle the sizing, positioning, as well as how and when
        /// those methods are called.  These methods can be overridden or expanded upon as neccessary to achieve any specific effect a particular
        /// @ref Widget or @ref QuadRenderable may need.  @n @n
        /// It is important to note that the base LayoutStrategy class does not contain any support for expanding widgets that use the "SR_Fill_Available"
        /// size rule.
        ///////////////////////////////////////
        class MEZZ_LIB LayoutStrategy
        {
        public:
            /// @brief Basic container type for @ref QuadRenderable storage by this class.
            typedef std::vector<Widget*>                       ChildContainer;
            /// @brief Iterator type for @ref QuadRenderable instances stored by this class.
            typedef ChildContainer::iterator                   ChildIterator;
            /// @brief Const Iterator type for @ref QuadRenderable instances stored by this class.
            typedef ChildContainer::const_iterator             ConstChildIterator;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            LayoutStrategy();
            /// @brief Class destructor.
            virtual ~LayoutStrategy();

            ///////////////////////////////////////////////////////////////////////////////
            // Primary Methods

            /// @brief Updates the dimensions of a collection of QuadRenderables
            /// @param OldSelfRect The pre-update dimensions of the calling quad.
            /// @param NewSelfRect The post-update dimensions of the calling quad.
            /// @param ChildQuads A container of all the Quads to be updated by this strategy.
            virtual void Layout(const Rect& OldSelfRect, const Rect& NewSelfRect, const ChildContainer& ChildQuads);

            ///////////////////////////////////////////////////////////////////////////////
            // Sub-task Methods

            /// @brief Handles the positioning of a child that needs it's dimensions updated.
            /// @param OldSelfRect The previous dimensions of the calling quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param NewChildSize The post-update size of the child being updated.
            /// @param Child The child to update.
            /// @return Returns the childs updated position.
            virtual Vector2 HandleChildPositioning(const Rect& OldSelfRect, const Rect& NewSelfRect, const Vector2& NewChildSize, QuadRenderable* Child);
            /// @brief Handles the positioning of a child on the X axis.
            /// @param OldSelfRect The previous dimensions of the calling quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param NewChildSize The post-update size of the child being updated.
            /// @param Child The child to update.
            /// @return Returns the childs updated position on the X axis.
            virtual Real HandleChildHorizontalPositioning(const Rect& OldSelfRect, const Rect& NewSelfRect, const Vector2& NewChildSize, QuadRenderable* Child);
            /// @brief Handles the positioning of a child on the Y axis.
            /// @param OldSelfRect The previous dimensions of the calling quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param NewChildSize The post-update size of the child being updated.
            /// @param Child The child to update.
            /// @return Returns the childs updated position on the Y axis.
            virtual Real HandleChildVerticalPositioning(const Rect& OldSelfRect, const Rect& NewSelfRect, const Vector2& NewChildSize, QuadRenderable* Child);
            /// @brief Handles the sizing of a child that needs it's dimensions updated.
            /// @param OldSelfRect The previous dimensions of the calling quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param Child The child to update.
            /// @return Returns the childs updated size.
            virtual Vector2 HandleChildSizing(const Rect& OldSelfRect, const Rect& NewSelfRect, QuadRenderable* Child);
            /// @brief Handles the sizing of a child on the X axis.
            /// @param OldSelfRect The previous dimensions of the calling quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param PrevAxisResult The result of the previous axis, in case the settings require it.
            /// @param Child The child to update.
            /// @return Returns the childs updated size on the X axis.
            virtual Real HandleChildHorizontalSizing(const Rect& OldSelfRect, const Rect& NewSelfRect, const Real PrevAxisResult, QuadRenderable* Child);
            /// @brief Handles the sizing of a child on the Y axis.
            /// @param OldSelfRect The previous dimensions of the calling quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param PrevAxisResult The result of the previous axis, in case the settings require it.
            /// @param Child The child to update.
            /// @return Returns the childs updated size on the Y axis.
            virtual Real HandleChildVerticalSizing(const Rect& OldSelfRect, const Rect& NewSelfRect, const Real PrevAxisResult, QuadRenderable* Child);
            /// @brief Preserves a child's aspect ratio if it is configured to do so.
            /// @param OldChildSize The previous size of the child prior to the dimension update.
            /// @param NewChildSize The updated and current dimensions of the calling quad.
            /// @param Child The child to update.
            virtual void CheckChildAspectRatio(const Vector2& OldChildSize, Vector2& NewChildSize, QuadRenderable* Child);
            /// @brief Clamps the updated size to the set minimum size for the child quad.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param NewChildSize The updated size to clamp to the set minimum size for the child.
            /// @param Child The child to update.
            virtual void ClampChildToMinSize(const Rect& NewSelfRect, Vector2& NewChildSize, QuadRenderable* Child);
            /// @brief Clamps the updated size to the set maximum size for the child quad.
            /// @param NewSelfRect The updated and current dimensions of the calling quad.
            /// @param NewChildSize The updated size to clamp to the set maximum size for the child.
            /// @param Child The child to update.
            virtual void ClampChildToMaxSize(const Rect& NewSelfRect, Vector2& NewChildSize, QuadRenderable* Child);
        };//LayoutStrategy
    }//UI
}//Mezzanine

#endif
