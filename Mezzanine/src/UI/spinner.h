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
#ifndef _uispinner_h
#define _uispinner_h

#include "UI/pageprovider.h"
#include "UI/widgetfactory.h"
#include "UI/uienumerations.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        class EditBox;
        class FontData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the EventArguments class for when the spinvalue of a Spinner is updated.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SpinnerValueChangedArguments : public WidgetEventArguments
        {
        public:
            /// @brief The pre-update value of the calling Spinner.
            const Real OldSpinValue;
            /// @brief The post-update value of the calling Spinner.
            const Real NewSpinValue;

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Source The identification of the widget firing this event.
            /// @param OldValue The pre-update value of the calling Spinner.
            /// @param NewValue The post-update value of the calling Spinner.
            SpinnerValueChangedArguments(const String& Name, const String& Source, const Real& OldValue, const Real& NewValue)
                : WidgetEventArguments(Name,Source), OldSpinValue(OldValue), NewSpinValue(NewValue) {  }
            /// @brief Class destructor.
            virtual ~SpinnerValueChangedArguments() {  }
        };//SpinnerValueChangedArguments

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a simple widget for a numeric variable in a box.
        /// @details Like the Scrollbar Widget, the size provided will be for the area of all the
        /// elements of this widget combined.  In the case of separate format, if horizontal, the
        /// width of the increment and decrement buttons will be the same as the height or if vertical
        /// the height will be the same as the width provided.  In either case making the two buttons
        /// perfectly square in size, with the remaining space going to the display caption.  @n
        /// In the case of the together format, then half of the height is taken and that is used for
        /// both the width and height, and then they are placed on the appropriate side.
        ///////////////////////////////////////
        class MEZZ_LIB Spinner : public PageProvider
        {
        public:
            /// @brief String containing the type name for this class: "Spinner".
            static const String TypeName;
            /// @brief Event name for when the value in this Spinner is updated.
            static const String EventSpinValueChanged;
        protected:
            friend class SpinnerFactory;
            /// @internal
            /// @brief A pointer to the button that will increment the spin value.
            Button* IncrementSpin;
            /// @internal
            /// @brief A pointer to the button that will decrement the spin value.
            Button* DecrementSpin;
            /// @internal
            /// @brief A pointer to the EditBox that will display the spin value.
            EditBox* ValueDisplay;
            /// @internal
            /// @brief The current spin value of this Spinner.
            Real SpinValue;
            /// @internal
            /// @brief The amount to increase or decrease by when the spin value is altered via buttons.
            Real IncrementValue;
            /// @internal
            /// @brief The minimum value allowed to be expressed by this Spinner.
            Real MinValue;
            /// @internal
            /// @brief The maximum value allowed to be expressed by this Spinner.
            Real MaxValue;

            /// @copydoc QuadRenderable::CreateLayoutStrat()
            virtual void CreateLayoutStrat();
            /// @internal
            /// @brief Contains all the common necessary startup initializations for this class.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFont A pointer to the font to be used by the child EditBox.
            virtual void ConstructSpinner(const SpinnerStyle SpinStyle, FontData* EditFont);
            /// @internal
            /// @brief Ensures the provided value is within the configured limits.
            /// @param Value The value to verify.
            virtual void ClampToLimits(Real& Value);
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            Spinner(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFont A pointer to the font to be used by the child EditBox.
            /// @param Parent The parent Screen that created this widget.
            Spinner(const String& RendName, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFontName The name of the font to be used by the child EditBox.
            /// @param Parent The parent Screen that created this widget.
            Spinner(const String& RendName, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFont A pointer to the font to be used by the child EditBox.
            /// @param Parent The parent screen that created this renderable.
            Spinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFontName The name of the font to be used by the child EditBox.
            /// @param Parent The parent screen that created this renderable.
            Spinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Spinner will belong to.
            Spinner(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~Spinner();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Spinner Properties

            /// @brief Sets the value of this Spinner.
            /// @param Value A Real containing the new value of this Spinner.
            virtual void SetSpinValue(const Real Value);
            /// @brief Gets the value of this Spinner.
            /// @return Returns a Real containing the current value of this spinner.
            virtual Real GetSpinValue() const;
            /// @brief Sets the amount to increase or decrease by when the spinner value is altered by child buttons.
            /// @param Value A Real containing the amount to increase the spin value when incrememnted, or decrease the spin value when decremented.
            virtual void SetIncrementValue(const Real Value);
            /// @brief Gets the amount to increase or decrease by when the spinner value is altered by child buttons.
            /// @return Returns a Real containing the amount the spin value will go up or down when the child buttons are clicked.
            virtual Real GetIncrementValue() const;

            /// @brief Sets the minimum and maximum limits for the value this Spinner can have.
            /// @note Both of these values default to 0.0 after construction.
            /// @param Min The minumum value this Spinner can have.
            /// @param Max The maximum value this Spinner can have.
            virtual void SetSpinValueLimits(const Real Min, const Real Max);
            /// @brief Sets the minimum limit for the value this Spinner can have.
            /// @param Value The minumum value this Spinner can have.
            virtual void SetMinSpinValue(const Real Value);
            /// @brief Gets the minimum limit for the value this Spinner can have.
            /// @return Returns a Real containing the minimum allowed value of this Spinner.
            virtual Real GetMinSpinValue() const;
            /// @brief Sets the maximum limit for the value this spinner can have.
            /// @param Value The maximum value this Spinner can have.
            virtual void SetMaxSpinValue(const Real Value);
            /// @brief Gets the maximum limit for the value this spinner can have.
            /// @return Returns a Real containing the maximum allowed value of this Spinner.
            virtual Real GetMaxSpinValue() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Spinner Configuration

            /// @brief Sets the sizing and positioning policies of the children in this spinner to match the provided layout.
            /// @param Style A SpinnerStyle value representing how the children of this spinner should be layed out.
            virtual void SetButtonLayout(const SpinnerStyle Style);

            /// @brief Gets the increment Button of this Spinner.
            /// @return Returns a pointer to the increment Button.
            virtual Button* GetIncrement() const;
            /// @brief Gets the decrement Button of this Spinner.
            /// @return Returns a pointer to the decrement Button.
            virtual Button* GetDecrement() const;
            /// @brief Gets the EditBox displaying the value of this Spinner.
            /// @return Returns a pointer to the value display EditBox.
            virtual EditBox* GetValueDisplay() const;

            ///////////////////////////////////////////////////////////////////////////////
            // PageProvider Methods

            /// @copydoc PageProvider::GetMaxXPages() const
            virtual Real GetMaxXPages() const;
            /// @copydoc PageProvider::GetMaxYPages() const
            virtual Real GetMaxYPages() const;

            /// @copydoc PageProvider::GetCurrentXPage() const
            virtual Real GetCurrentXPage() const;
            /// @copydoc PageProvider::GetCurrentYPage() const
            virtual Real GetCurrentYPage() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            /// @brief Self logic to be executed when this Spinner has it's spin value updated.
            /// @param OldValue The previous value of this Spinner.
            /// @param NewValue The updated value of this Spinner.
            virtual void _OnSpinValueChanged(const Real OldValue, const Real NewValue);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
            virtual void _NotifyEvent(const EventArguments& Args);
            /// @copydoc PageProvider::_NotifyContainerUpdated()
            virtual void _NotifyContainerUpdated();
        };//Spinner

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for Spinner widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SpinnerFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            SpinnerFactory() {  }
            /// @brief Class destructor.
            virtual ~SpinnerFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new Spinner.
            /// @param RendName The name to be given to the created Spinner.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            /// @param Parent The screen the created Spinner will belong to.
            /// @return Returns a pointer to the created Spinner.
            virtual Spinner* CreateSpinner(const String& RendName, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent);
            /// @brief Creates a new Spinner.
            /// @param RendName The name to be given to the created Spinner.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFontName The name of the font to be used by the edit layer.
            /// @param Parent The screen the created Spinner will belong to.
            /// @return Returns a pointer to the created Spinner.
            virtual Spinner* CreateSpinner(const String& RendName, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent);
            /// @brief Creates a new Spinner.
            /// @param RendName The name to be given to the created Spinner.
            /// @param RendRect The dimensions that will be assigned to the created Spinner.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            /// @param Parent The screen the created Spinner will belong to.
            /// @return Returns a pointer to the created Spinner.
            virtual Spinner* CreateSpinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, FontData* EditFont, Screen* Parent);
            /// @brief Creates a new Spinner.
            /// @param RendName The name to be given to the created Spinner.
            /// @param RendRect The dimensions that will be assigned to the created Spinner.
            /// @param SpinStyle The layout of buttons this Spinner will have.
            /// @param EditFontName The name of the font to be used by the edit layer.
            /// @param Parent The screen the created Spinner will belong to.
            /// @return Returns a pointer to the created Spinner.
            virtual Spinner* CreateSpinner(const String& RendName, const UnifiedRect& RendRect, const SpinnerStyle SpinStyle, const String& EditFontName, Screen* Parent);
            /// @brief Creates a new Spinner.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Spinner will belong to.
            /// @return Returns a pointer to the created Spinner.
            virtual Spinner* CreateSpinner(const XML::Node& XMLNode, Screen* Parent);

            /// @copydoc WidgetFactory::CreateWidget(Screen*)
            virtual Widget* CreateWidget(Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const UnifiedRect&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const XML::Node&, Screen*)
            virtual Widget* CreateWidget(const XML::Node& XMLNode, Screen* Parent);
            /// @copydoc WidgetFactory::DestroyWidget(Widget*)
            virtual void DestroyWidget(Widget* ToBeDestroyed);
        };//SpinnerFactory
    }//UI
}//Mezzanine

#endif
