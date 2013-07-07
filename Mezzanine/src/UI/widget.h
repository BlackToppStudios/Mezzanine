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
#ifndef _uiwidget_h
#define _uiwidget_h

#include "UI/renderable.h"
#include "UI/renderablerect.h"
#include "Input/inputenumerations.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        class WidgetListener;
        class BasicRenderable;
        class UIManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class InputCaptureData
        /// @headerfile uiwidget.h
        /// @brief This class contains all the utilities necessary for capturing input.
        /// @details This is commonly used for Text-based widgets, such as TextBox's, Spinners, and InputBox's.
        ///////////////////////////////////////
        class MEZZ_LIB InputCaptureData : public std::set<Input::InputCode>
        {
            protected:
                std::vector<Input::InputCode> CapturedCodes;
            public:
                /// @brief Class constructor.
                InputCaptureData();
                /// @brief Class destructor.
                ~InputCaptureData();
                /// @brief Adds a single input code to the list of codes to be captured.
                /// @param Code The code to be captured.
                void AddInput(const Input::InputCode& Code);
                /// @brief Adds a range of input codes to the list of codes to be captured.
                /// @details This function will fail silently if the lower code is higher then the upper code.
                /// @param Lower The first input code in the range to be entered.
                /// @param Upper The last input code in the range to be entered.
                void AddInputRange(const Input::InputCode& Lower, const Input::InputCode& Upper);
                /// @brief Gets the number of input codes this class is currently set to capture.
                /// @return Returns a Whole representing the number of input codes set to be captured.
                Whole GetNumCapturedInputs();
                /// @brief Gets a vector of inputs that have been captured this frame.
                /// @return Returns a pointer to the vector containing all the input codes captured this frame.
                std::vector<Input::InputCode>* GetCapturedInputs();
                /// @brief Checks this class to see if the provided code is set to be captured.
                /// @param Code The input code to check for.
                bool IsInputToBeCaptured(const Input::InputCode& Code);
                /// @brief Updates the list of captured inputs for the frame.
                /// @param InputCodes Vector of input codes to update the widget with.
                void UpdateCapturedInputs(std::vector<Input::InputCode>& InputCodes);
        };//inputcapturedata
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Widget
        /// @headerfile uiwidget.h
        /// @brief This class is the base class for all widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Widget : public Renderable
        {
            public:
                // Vertex data types
                typedef std::vector<VertexData> VertexContainer;
                // Renderable data types
                typedef std::list<Renderable*> RenderableList;
                typedef RenderableList::iterator RenderableIterator;
                typedef RenderableList::const_iterator ConstRenderableIterator;
                // Listener data types
                typedef std::vector<WidgetListener*> ListenerContainer;
                typedef ListenerContainer::iterator ListenerIterator;
                typedef ListenerContainer::const_iterator ConstListenerIterator;
                enum WidgetType
                {
                    W_Button,
                    W_ButtonListBox,
                    W_Cell,
                    W_CellGrid,
                    W_CheckBox,
                    W_DropDownList,
                    W_DropDownMenu,
                    W_GenericWidgetContainer,
                    W_ListBox,
                    W_Menu,
                    W_MenuWindow,
                    W_RadioButton,
                    W_Scrollbar,
                    W_Spinner,
                    W_TabSet,
                    W_TextBox,
                    W_Window
                };
            protected:
                friend class UIManager;
                friend class Screen;
                InputCaptureData* CaptureData;
                Widget* HoveredSubWidget;
                Widget* SubWidgetFocus;
                bool Hovered;
                WidgetType Type;

                ListenerContainer Listeners;
                RenderableList SubRenderables;
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false);
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false) = 0;
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible) = 0;
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl() = 0;
                /// @brief For use with sub-widget update/automation.
                virtual void SubWidgetUpdate(bool Force = false);
                /// @brief For use with sub-widget update/automation when the mouse isn't hovered.
                virtual void SubWidgetFocusUpdate(bool Force = false);
                /// @brief Adds a renderable as a subrenderable to this widget.
                virtual void AddSubRenderable(const UInt16& Zorder, Renderable* ToAdd);
                /// @brief Processes the captured inputs.  This is an empty function and should be overridden if making an input capturing widget.
                virtual void ProcessCapturedInputs();
            public:
                /// @brief Standard initialization constructor.
                /// @param Parent The parent Screen that created this widget.
                /// @param name The Name for the Widget.
                Widget(const String& name, Screen* Parent);
                /// @brief Standard destructor.
                virtual ~Widget();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                /// @brief Gets the type of widget this is.
                /// @return Returns an enum value representing the type of widget this is.
                virtual WidgetType GetType() const;
                /// @brief Checks if this is an input capturing widget.
                /// @return Returns a bool indicating whether or not this widget will capture input.
                virtual bool IsInputCaptureWidget() const;
                /// @brief Gets the result of the last mouse hover check.
                /// @return Returns whether or not the mouse was hovering over this widget during the last check.
                virtual bool IsHovered() const;
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();

                ///////////////////////////////////////////////////////////////////////////////
                // Visibility Methods

                /// @copydoc Renderable::SetVisible(bool visible)
                virtual void SetVisible(bool visible);
                /// @copydoc Renderable::GetVisible()
                virtual bool GetVisible() const;
                /// @copydoc Renderable::IsVisible()
                virtual bool IsVisible() const;
                /// @copydoc Renderable::Show()
                virtual void Show();
                /// @copydoc Renderable::Hide()
                virtual void Hide();

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Methods

                /// @brief Sets the Rect(Position and Size) of this Widget.
                /// @param Rect The Rect to set.
                virtual void SetRect(const RenderableRect& Rect);
                /// @brief Gets this Widgets' Rect.
                /// @param Relative Whether or not you want the Rect to be populated with Relative values.
                /// @return Returns a Rect containing this Widgets' Position and Size.
                virtual RenderableRect GetRect(bool Relative = true) const;
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(const Vector2& Position) = 0;
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition() const;
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(const Vector2& Position) = 0;
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition() const;
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(const Vector2& Size) = 0;
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize() const;
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(const Vector2& Size) = 0;
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Listener Methods

                /// @brief Sets the listener to be used by this widget.
                /// @param Listener The listener to be set for this widget.
                virtual void AddWidgetListener(WidgetListener* Listener);
                /// @brief Removes a listener currently being used by this widget.
                /// @param Listener The listener to be removed from this widget.
                virtual void RemoveWidgetListener(WidgetListener* Listener);

                ///////////////////////////////////////////////////////////////////////////////
                // Render Priority Methods

                /// @copydoc Renderable::SetRenderPriority(const UI::RenderPriority& Priority)
                virtual void SetRenderPriority(const UI::RenderPriority& Priority);

                ///////////////////////////////////////////////////////////////////////////////
                // Fetch Methods

                /// @brief Gets the hovered sub-widget within this widget, if any.
                /// @return Returns a pointer to the sub-widget within this widget the mouse is hovering over, or NULL if none.
                Widget* GetHoveredSubWidget() const;
                /// @brief Gets a pointer to the Widget at the bottom of the hovered SubWidget chain.
                /// @return Returns a pointer to the hovered sub-widget of the hovered sub-widget, etc., until you reach the end of the chain.
                Widget* GetBottomMostHoveredWidget();
                /// @brief Gets a pointer to the root widget in this chain of widgets.
                /// @return Returns a pointer to the parent of the parent, etc., until you reach the root widget on the screen.
                Widget* GetTopMostWidget();
                /// @brief Gets the screen this widget belongs to.
                /// @return Returns a pointer to the screen this eidget belongs to.
                Screen* GetParent() const;
                /// @brief Gets the data determining what input should be captured.
                /// @return Returns a pointer to the InputCaptureData, or NULL if this widget doesn't capture data.
                InputCaptureData* GetInputCaptureData() const;

                /// @brief Gets an iterator to the first renderable pair in this widget.
                RenderableIterator BeginRenderable();
                /// @brief Gets an iterator to one passed the last renderable pair in this widget.
                RenderableIterator EndRenderable();
                /// @brief Gets a const iterator to the first renderable pair in this widget.
                ConstRenderableIterator BeginRenderable() const;
                /// @brief Gets a const iterator to one passed the last renderable pair in this widget.
                ConstRenderableIterator EndRenderable() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @copydoc Renderable::_MarkDirty()
                virtual void _MarkDirty();
                /// @copydoc Renderable::_Redraw()
                virtual void _Redraw();
                /// @copydoc Renderable::_AppendVertices(ScreenVertexData& Vertices)
                virtual void _AppendVertices(ScreenVertexData& Vertices);
        };//widget

        ///////////////////////////////////////////////////////////////////////////////
        /// @class WidgetListener
        /// @headerfile uiwidget.h
        /// @brief This is a listener class for widgets.
        ///////////////////////////////////////
        class MEZZ_LIB WidgetListener
        {
            protected:
                Widget* Caller;
            public:
                /// @brief Class constructor.
                WidgetListener();
                /// @brief Class destructor.
                virtual ~WidgetListener();
                /// @brief Sets the Widget this listener belongs to.
                virtual void SetCaller(Widget* Caller);
                /// @brief Performs listener items immediately after hover checks complete.
                virtual void DoHoverItems() = 0;
                /// @brief Performs listener items just before widget updates start.
                virtual void DoPreUpdateItems() = 0;
                /// @brief Performs listener items immediately after updates complete.
                virtual void DoPostUpdateItems() = 0;
                /// @brief Performs listener items immediately after widget visibility changes.
                virtual void DoVisibilityChangeItems() = 0;
        };//WidgetListener
    }//UI
}//Mezzanine

#endif
