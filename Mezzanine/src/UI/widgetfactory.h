// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _uiwidgetfactory_h
#define _uiwidgetfactory_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace XML
    {
        class Node;
    }
    namespace UI
    {
        class Widget;
        class Screen;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class WidgetFactory
        /// @headerfile widgetfactory.h
        /// @brief This is a base class for factories that construct the widgets available to the UI subsystem.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB WidgetFactory
        {
        public:
            /// @brief Class constructor.
            WidgetFactory() {  }
            /// @brief Class destructor.
            virtual ~WidgetFactory() {  }

            /// @brief Gets the name of the Widget that is created by this factory.
            /// @return Returns the typename of the Widget created by this factory.
            virtual String GetWidgetTypeName() const = 0;

            /// @brief Creates a Widget of the type represented by this factory.
            /// @param Parent The screen the created Widget will belong to.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(Screen* Parent) = 0;
            /// @brief Creates a Widget of the type represented by this factory.
            /// @param RendName The name to be given to the created widget.
            /// @param Params A NameValuePairMap containing the params to be applied during construction.
            /// @param Parent The screen the created Widget will belong to.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent) = 0;
            /// @brief Creates a Widget of the type represented by this factory.
            /// @param RendName The name to be given to the created widget.
            /// @param RendRect The dimensions that will be assigned to the created Widget.
            /// @param Params A NameValuePairMap containing the params to be applied during construction.
            /// @param Parent The screen the created Widget will belong to.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent) = 0;
            /// @brief Creates a Widget from XML.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Widget will belong to.
            /// @return Returns a pointer to the Widget created.
            virtual Widget* CreateWidget(const XML::Node& XMLNode, Screen* Parent) = 0;
            /// @brief Destroys a Widget created by this factory.
            /// @param ToBeDestroyed A pointer to the Widget to be destroyed.
            virtual void DestroyWidget(Widget* ToBeDestroyed) = 0;
        };//WidgetFactory
    }//UI
}//Mezzanine

#endif
