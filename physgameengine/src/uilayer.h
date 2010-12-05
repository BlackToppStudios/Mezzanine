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
#ifndef _uilayer_h
#define _uilayer_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector2.h"
#include "uiscrollbar.h"

#include <map>

namespace Gorilla
{
    class Screen;
    class Layer;
    class Rectangle;
}

namespace phys
{
    class UIManager;
    namespace UI
    {
        class Widget;
        class Rectangle;
        class Caption;
        class Button;
        class TextButton;
        class MarkupText;
        class CheckBox;
    }
    ///////////////////////////////////////////////////////////////////////////////
    /// @class UILayer
    /// @headerfile uilayer.h
    /// @brief This class is the basic container class for UI elements.
    /// @details A layer is a container for widgets and other UI elements that are visable.
    ///////////////////////////////////////
    class PHYS_LIB UILayer
    {
        protected:
            Gorilla::Layer* GorillaLayer;
            Gorilla::Screen* Parent;
            UIManager* Manager;
            String Name;
            std::vector<UI::Button*> Buttons;
            std::vector<UI::Rectangle*> Rectangles;
            std::vector<UI::Caption*> Captions;
            std::vector<UI::MarkupText*> MarkupTexts;
            std::vector<UI::Widget*> Widgets;
        public:
            /// @brief Internal constructor.
            /// @param name The name of this layer.
            /// @param GLayer The Gorilla Layer this Layer is based on.
            /// @param GScreen The gorilla parent screen to this layer.
            UILayer(const String& name, Gorilla::Layer* GLayer, Gorilla::Screen* GScreen);
            /// @brief Class destructor.
            ~UILayer();
            ///////////////////////////////////////////////////////////////////////////////
            // Non-check related Utility Functions
            ///////////////////////////////////////
            /// @brief Gets the name of this layer.
            /// @return Returns a string containing the name of this layer.
            String& GetName();
            /// @brief Sets the layers' visability.
            /// @param Visable A bool representing the visability of the layer.
            void SetVisible(bool Visible);
            /// @brief Gets the layers' visability.
            /// @return Returns a bool representing the visability of the layer.
            bool GetVisible();
            /// @brief Forces the layer to be shown.
            void Show();
            /// @brief Forces the layer to hide.
            void Hide();
            ///////////////////////////////////////////////////////////////////////////////
            // Creating and working with All Basic UI Elements
            ///////////////////////////////////////
            /// @brief Creates a button within this layer.
            /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
            /// @return Returns a pointer to the created button.
            /// @param Name The name of the button.
            /// @param Position The top left position of the button.
            /// @param Size The size of the Button.
            UI::Button* CreateButton(String& Name, Vector2 Position, Vector2 Size);
            /// @brief Creates a text button within this layer.
            /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
            /// @return Returns a pointer to the created button.
            /// @param Name The name of the button.
            /// @param Position The top left position of the button.
            /// @param Size The size of the Button.
            /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
            /// @param Text Any text you want printed on the button.
            UI::TextButton* CreateTextButton(String& Name, Vector2 Position, Vector2 Size, Whole Glyph, String Text);
            /// @brief Gets an already created button by name.
            /// @return Returns a pointer to the button of the specified name.
            UI::Button* GetButton(String& Name);
            /// @brief Gets an already created button by index.
            /// @return Returns a pointer to the button at the specified index.
            UI::Button* GetButton(Whole Index);
            /// @brief Gets the number of buttons created and stored in this class.
            /// @return Returns the number of buttons this class is storing.
            Whole GetNumButtons();
            /// @brief Destroys a button.
            /// @param ToBeDestroyed Pointer to the button you want destroyed.
            void DestroyButton(UI::Button* ToBeDestroyed);
            /// @brief Creates a rectangle within this layer.
            /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0). @n
            /// Rectangles are innately put behind all captions, so z-order is not necessary.
            /// @param Position The top left position of the button.
            /// @param Size The size of the Button.
            UI::Rectangle* CreateRectangle(Vector2 Position, Vector2 Size);
            /// @brief Gets an already created rectangle by index.
            /// @return Returns a pointer to the rectangle at the specified index.
            UI::Rectangle* GetRectangle(Whole Index);
            /// @brief Gets the number of rectangles created and stored in this class.
            /// @return Returns the number of rectangles this class is storing.
            Whole GetNumRectangles();
            /// @brief Destroys a rectangle.
            /// @param ToBeDestroyed Pointer to the rectangle you want destroyed.
            void DestroyRectangle(UI::Rectangle* ToBeDestroyed);
            /// @brief Creates a caption within this layer.
            /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
            /// @return Returns a pointer to the created caption.
            /// @param Name The name of this caption.
            /// @param Position The top left position of the caption.
            /// @param Size The size of the Button.
            /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
            /// @param Text Any text you want printed on the caption.
            UI::Caption* CreateCaption(String& Name, Vector2 Position, Vector2 Size, Whole Glyph, String Text);
            /// @brief Gets an already created caption by name.
            /// @return Returns a pointer to the caption of the specified name.
            UI::Caption* GetCaption(String& Name);
            /// @brief Gets an already created caption by index.
            /// @return Returns a pointer to the caption at the specified index.
            UI::Caption* GetCaption(Whole Index);
            /// @brief Gets the number of captions created and stored in this class.
            /// @return Returns the number of captions this class is storing.
            Whole GetNumCaptions();
            /// @brief Destroys a caption.
            /// @param ToBeDestroyed Pointer to the caption you want destroyed.
            void DestroyCaption(UI::Caption* ToBeDestroyed);
            /// @brief Creates a markup text within this layer.
            /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
            /// @return Returns a pointer to the created markup text.
            /// @param Name The name of this markup text.
            /// @param Position The top left position of the markup text.
            /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
            /// @param Text Any text you want printed on the markup text.
            UI::MarkupText* CreateMarkupText(String& Name, Vector2 Position, Whole Glyph, String Text);
            /// @brief Gets an already created markup text by name.
            /// @return Returns a pointer to the markup text of the specified name.
            UI::MarkupText* GetMarkupText(String& Name);
            /// @brief Gets an already created markup text by index.
            /// @return Returns a pointer to the markup text at the specified index.
            UI::MarkupText* GetMarkupText(Whole Index);
            /// @brief Gets the number of markup texts created and stored in this class.
            /// @return Returns the number of markup texts this class is storing.
            Whole GetNumMarkupTexts();
            /// @brief Destroys a markup text.
            /// @param ToBeDestroyed Pointer to the markup text you want destroyed.
            void DestroyMarkupText(UI::MarkupText* ToBeDestroyed);
            ///////////////////////////////////////////////////////////////////////////////
            // Working with all Widgets
            ///////////////////////////////////////
            /// @brief Gets an already created widget by name.
            /// @return Returns a pointer to the widget of the specified name.
            UI::Widget* GetWidget(String& Name);
            /// @brief Gets an already created widget by index.
            /// @return Returns a pointer to the widget at the specified index.
            UI::Widget* GetWidget(Whole Index);
            /// @brief Gets the number of widgets created and stored in this class.
            /// @return Returns the number of widgets this class is storing.
            Whole GetNumWidgets();
            /// @brief Destroys a widget.
            /// @param ToBeDestroyed Pointer to the widget you want destroyed.
            void DestroyWidget(UI::Widget* ToBeDestroyed);
            ///////////////////////////////////////////////////////////////////////////////
            // Creating Widgets
            ///////////////////////////////////////
            /// @brief Creates a Scrollbar within this layer.
            /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
            /// @return Returns a pointer to the created Scrollbar.
            /// @param Name The name of the Scrollbar.
            /// @param Position The top left position of the Scrollbar.
            /// @param Size The size of the Scrollbar.
            /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
            UI::Scrollbar* CreateScrollbar(String& Name, Vector2 Position, Vector2 Size, UI::Scrollbar::BarStyle Style);
            ///////////////////////////////////////////////////////////////////////////////
            // UI Element and Widget Checks
            ///////////////////////////////////////
            /// @brief Gets the button the mouse is over if any.
            /// @details This function searches only the buttons contained in this layer.
            /// @return Returns the button the mouse is over, or NULL if there are none.
            UI::Button* CheckButtonMouseIsOver();
            /// @brief Gets the widget the mouse is over if any.
            /// @details This function searches only the widgets contained in this layer.
            /// @return Returns the widget the mouse is over, or NULL if there are none.
            UI::Widget* CheckWidgetMouseIsOver();
            ///////////////////////////////////////////////////////////////////////////////
            // Internal Functions
            ///////////////////////////////////////
            /// @internal
            /// @brief Gets the internal gorilla layer pointer.
            Gorilla::Layer* GetGorillaLayer();
    };//uilayer
}//phys

#endif
