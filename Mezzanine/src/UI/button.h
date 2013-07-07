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
#ifndef _uibutton_h
#define _uibutton_h

#include "colourvalue.h"
#include "uienumerations.h"
#include "UI/activatable.h"

namespace Mezzanine
{
    namespace UI
    {
        class Caption;
        class Screen;
        class ButtonCallback;
        class UIManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Button
        /// @headerfile uibutton.h
        /// @brief This class is a helper class, specifically for use as a button.
        /// @details Unlike rectangles and captions, this class can be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class MEZZ_LIB Button : public Activatable
        {
            protected:
                friend class UIManager;
                friend class RenderableFactory;
                Caption* Clickable;
                /// @copydoc UpdateImpl(bool Force)
                virtual void UpdateImpl(bool Force = false);
                /// @copydoc SetVisibleImpl(bool visible)
                virtual void SetVisibleImpl(bool visible);
                /// @copydoc CheckMouseHoverImpl()
                virtual bool CheckMouseHoverImpl();
                /// @brief Sets the actual location of this widget.
                virtual void SetLocation(const Vector2& Location);
                /// @brief Sets the actual size of this widget.
                virtual void SetArea(const Vector2& Area);
            //public:
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param PScreen Pointer to the Screen that created this button.
                Button(ConstString& name, const RenderableRect& Rect, Screen* PScreen);
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                /// @param PScreen Pointer to the Screen that created this button.
                Button(ConstString& name, const RenderableRect& Rect, const Whole& Glyph, const String& Text, Screen* PScreen);
                /// @brief Internal constructor
                /// @param name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the button.
                /// @param PScreen Pointer to the Screen that created this button.
                Button(ConstString& name, const RenderableRect& Rect, const Real& LineHeight, const String& Text, Screen* PScreen);
                /// @brief Class destructor.
                virtual ~Button();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @copydoc UpdateDimensions()
                virtual void UpdateDimensions();

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Methods

                /// @copydoc SetPosition(const Vector2& Position)
                virtual void SetPosition(const Vector2& Position);
                /// @copydoc SetActualPosition(const Vector2& Position)
                virtual void SetActualPosition(const Vector2& Position);
                /// @copydoc SetSize(const Vector2& Size)
                virtual void SetSize(const Vector2& Size);
                /// @copydoc SetActualSize(const Vector2& Size)
                virtual void SetActualSize(const Vector2& Size);

                ///////////////////////////////////////////////////////////////////////////////
                // Fetch Methods

                /// @brief Gets the internal clickable renderable used by this button.
                /// @return Returns a pointer to the renderable being used as the clickable surface of this button.
                Caption* GetClickable() const;
        };//button
    }//UI
}//Mezzanine

#endif
