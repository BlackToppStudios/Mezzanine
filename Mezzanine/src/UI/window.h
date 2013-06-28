//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uiwindow_h
#define _uiwindow_h

#include "UI/renderablecontainerwidget.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Window
        /// @headerfile uiwindow.h
        /// @brief This is a container widget capable of holding any other widget.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Window : public EnclosedRenderableContainerWidget
        {
            protected:
                friend class ExtendedRenderableFactory;
                /// @internal
                /// @enum ResizeMode
                /// @brief Internal enum for handling a resizing via mouse.
                enum ResizeMode
                {
                    RM_None,
                    RM_TopLeft,
                    RM_TopRight,
                    RM_BottomLeft,
                    RM_BottomRight,
                    RM_Left,
                    RM_Right,
                    RM_Top,
                    RM_Bottom
                };
                Rectangle* WindowBack;
                //Caption* WindowTitle;
                Whole BorderWidth;
                ResizeMode CurrentRM;
                /// @brief Checks to see if a point is within the border of this window.
                void BorderAreaCheck(const Vector2& ScreenLoc);
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false);
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
            //public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the window.
                /// @param Rect The Rect representing the position and size of the window.
                /// @param PScreen The parent screen this window belongs to.
                Window(ConstString& name, const RenderableRect& Rect, Screen* PScreen);
                /// @brief Standard destructor.
                virtual ~Window();
            public:
                /// @brief Sets the relative position of this window.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this window.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this window.
                /// @param Position A vector2 representing the pixel position of this window.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the relative size of this window.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this window.
                virtual void SetSize(const Vector2& Size);
                /// @brief Sets the pixel size of this window.
                /// @param Size A vector2 representing the pixel size of this window.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Updates the dimensions of this widget to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                /// @brief Gets the background object of this window.
                /// @return Returns a pointer to the rectangle that is the background for this window.
                virtual Rectangle* GetWindowBack();
        };//window
    }//UI
}//Mezzanine

#endif
