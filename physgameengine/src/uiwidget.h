//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _uiwidget_h
#define _uiwidget_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector2.h"

namespace phys
{
    class UIManager;
    class UILayer;
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Widget
        /// @headerfile uiwidget.h
        /// @brief This class is the base class for widgets.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB Widget
        {
            public:
                enum WidgetType
                {
                    ButtonListBox,
                    CheckBox,
                    Scrollbar
                };
            protected:
                friend class phys::UIManager;
                UIManager* Manager;
                UILayer* Parent;
                Vector2 RelPosition;
                Vector2 RelSize;
                WidgetType Type;
                String Name;
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false) = 0;
            public:
                /// @brief Standard initialization constructor.
                /// @param parent The parent layer that created this widget.
                Widget(const String& name, UILayer* parent);
                /// @brief Standard destructor.
                virtual ~Widget();
                /// @brief Gets the type of widget this is.
                /// @return Returns an enum value representing the type of widget this is.
                WidgetType GetType();
                /// @brief Gets the name of this widget.
                /// @return Returns a String containing the name of this widget.
                virtual String& GetName();
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover() = 0;
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(Vector2 Position) = 0;
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition() = 0;
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(Vector2 Position) = 0;
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition() = 0;
                /// @brief Sets the relative size of this widget.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(Vector2 Size) = 0;
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize() = 0;
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(Vector2 Size) = 0;
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize() = 0;
        };//widget
    }//UI
}//phys

#endif
