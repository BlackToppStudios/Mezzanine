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

#ifndef _uistackedcontainer_h
#define _uistackedcontainer_h

#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class StackButton;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for containers that stack their children one on top of the other.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB StackedContainer : public Widget
        {
        public:
            /// @brief String containing the type name for this class: "StackedContainer".
            static const String TypeName;
        protected:
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            StackedContainer(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            StackedContainer(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            StackedContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            virtual ~StackedContainer();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            ///////////////////////////////////////////////////////////////////////////////
            // StackedContainer Configuration

            /// @brief Binds a StackedButton to this container according to the provided config value.
            /// @param Config A UInt16 that represents how the provided button will be added to the containers configuration.  Check implementation for more information.
            /// @param ConfigButton The button to be added to this containers configuration.
            virtual void SetButtonConfig(const UInt16 Config, StackButton* ConfigButton) = 0;
            /// @brief Gets the role of the specified StackedButton for this StackedContainer.
            /// @param ConfigButton The button to check this StackedContainer for.
            /// @return Returns a UInt16 representing how the specified StackButton is being used by this StackedContainer.
            virtual UInt16 GetButtonConfig(const StackButton* ConfigButton) const = 0;

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

            /// @internal
            /// @brief Notifies this StackedContainer a button has been selected.
            /// @param Selected The StackedContainer button that was selected.
            virtual void _NotifyButtonSelected(StackButton* Selected) = 0;
        };//StackedContainer
    }//UI
}//Mezzanine

#endif
