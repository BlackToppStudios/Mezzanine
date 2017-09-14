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

#ifndef _uieditbox_h
#define _uieditbox_h

#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class TextLayer;
        class FontData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Widget for handling the input and manipulation of text.
        /// @details This class does not use a LayoutStrategy.
        /// @remarks The auto-created text layer used for editing will be given a ZOrder of 5 on all auto-created
        /// RenderLayerGroups.  When the layer is to be edited, the EditBox will attempt to get the layer at ZOrder
        /// 5 in the active group.  If there is no active group, or no layer at ZOrder 5 in the active group, or the
        /// layer at ZOrder 5 isn't a text layer, then the edit will be blocked and not occur.
        ///////////////////////////////////////
        class MEZZ_LIB EditBox : public Widget
        {
        public:
            /// @brief String containing the type name for this class: "EditBox".
            static const String TypeName;
            /// @brief Event name for when the text in this widget has been updated.
            static const EventNameType EventTextUpdated;
            /// @brief Callback type for input filtering.
            typedef Boole (FilterCallback)(const Int32 Glyph);
        protected:
            /// @internal
            /// @brief Stores the screen position where the mouse is located during editing.  Used to find the end index for highlighting.
            Vector2 EditHighlightTarget;
            /// @internal
            /// @brief A pointer to the method that will filter any input before adding it as text to this EditBox.
            FilterCallback* InputFilter;
            /// @internal
            /// @brief Stores the index of the character initially highlighted while editing.
            Integer EditHighlightOrigin;
            /// @internal
            /// @brief Stores whether or not inputs into this widget will alter it's underlying text.
            Boole EditingEnabled;

            /// @copydoc Widget::HandleInputImpl(const Input::MetaCode& Code)
            virtual Boole HandleInputImpl(const Input::MetaCode& Code);
            /// @internal
            /// @brief Contains all the common necessary startup initializations for this class.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            virtual void ConstructEditBox(const RenderLayerType EditLayerType, FontData* EditFont);
            /// @internal
            /// @brief Convenience method that will check the configuration of this EditBox to determine if inputs will be consumed by this EditBox.
            virtual void UpdateEditMode();
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            EditBox(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            /// @param Parent The parent Screen that created this widget.
            EditBox(const String& RendName, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFontName The name of the font to be used by the edit layer.
            /// @param Parent The parent Screen that created this widget.
            EditBox(const String& RendName, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            /// @param Parent The parent screen that created this renderable.
            EditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFontName The name of the font to be used by the edit layer.
            /// @param Parent The parent screen that created this renderable.
            EditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created EditBox will belong to.
            EditBox(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~EditBox();
        //public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;
            /// @brief Checks to see if this EditBox is currently Editing.
            /// @note Editing in the context means that Editing has been set to enabled, this EditBox currently has the focus,
            /// and the cursor is enabled (which is handled automatically).  If these criteria are met then this EditBox will consume
            /// the appropriate inputs to display text.
            /// @return Returns true if this EditBox is currently editing and ready for inputs, false otherwise.
            virtual Boole IsCurrentlyEditing() const;

            /// @brief Sets the text in the edit layer of this EditBox.
            /// @param Text A string containing the text the edit layer of this EditBox is to display.
            virtual void SetText(const String& Text);
            /// @brief Gets the text in the edit layer of this EditBox.
            /// @return Returns a string containing the text in the text layer being edited by this EditBox.
            virtual String GetText() const;

            ///////////////////////////////////////////////////////////////////////////////
            // EditBox Properties

            /// @brief Sets wether or not this EditBox will consume inputs to generate text.
            /// @param Enable True to consume inputs, false otherwise.
            virtual void SetEditingEnabled(Boole Enable);
            /// @brief Gets wether or not this EditBox will consume inputs to generate text.
            /// @return Returns true if this EditBox is currently consuming inputs to generate text, false otherwise.
            virtual Boole GetEditingEnabled() const;

            ///////////////////////////////////////////////////////////////////////////////
            // EditBox Configuration

            /// @brief Sets the filter that will be used by this EditBox to determine if an input will be consumed.
            /// @param Callback A pointer to the c-style method that will filter this EditBoxs inputs.
            virtual void SetInputFilter(FilterCallback* Callback);
            /// @brief Gets the filter that is being used by this EditBox to determine if an input will be consumed.
            /// @return Returns a pointer to the currently set FilterCallback.
            virtual FilterCallback* GetIntputFilter() const;

            /// @brief Gets the layer that consumed inputs will be inserted into.
            /// @return Returns a pointer to the TextLayer that will be edited when this EditBox consumes inputs.
            virtual TextLayer* GetEditLayer() const;

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

            /// @brief Self logic to be executed when this EditBox updates its text.
            virtual void _OnTextUpdated();
            /// @copydoc Widget::_OnMouseDragStart()
            virtual void _OnMouseDragStart();
            /// @copydoc Widget::_OnMouseDragEnd()
            virtual void _OnMouseDragEnd();
            /// @copydoc Widget::_OnFocusGained()
            virtual void _OnFocusGained();
            /// @copydoc Widget::_OnFocusLost()
            virtual void _OnFocusLost();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

        };//EditBox

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for EditBox widgets.
        ///////////////////////////////////////
        class MEZZ_LIB EditBoxFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            EditBoxFactory() {  }
            /// @brief Class destructor.
            virtual ~EditBoxFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new EditBox.
            /// @param RendName The name to be given to the created EditBox.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            /// @param Parent The screen the created EditBox will belong to.
            /// @return Returns a pointer to the created EditBox.
            virtual EditBox* CreateEditBox(const String& RendName, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent);
            /// @brief Creates a new EditBox.
            /// @param RendName The name to be given to the created EditBox.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFontName The name of the font to be used by the edit layer.
            /// @param Parent The screen the created EditBox will belong to.
            /// @return Returns a pointer to the created EditBox.
            virtual EditBox* CreateEditBox(const String& RendName, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent);
            /// @brief Creates a new EditBox.
            /// @param RendName The name to be given to the created EditBox.
            /// @param RendRect The dimensions that will be assigned to the created EditBox.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFont A pointer to the font to be used by the edit layer.
            /// @param Parent The screen the created EditBox will belong to.
            /// @return Returns a pointer to the created EditBox.
            virtual EditBox* CreateEditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, FontData* EditFont, Screen* Parent);
            /// @brief Creates a new EditBox.
            /// @param RendName The name to be given to the created EditBox.
            /// @param RendRect The dimensions that will be assigned to the created EditBox.
            /// @param EditLayerType The type of text layer to be auto-created for editing.
            /// @param EditFontName The name of the font to be used by the edit layer.
            /// @param Parent The screen the created EditBox will belong to.
            /// @return Returns a pointer to the created EditBox.
            virtual EditBox* CreateEditBox(const String& RendName, const UnifiedRect& RendRect, const RenderLayerType EditLayerType, const String& EditFontName, Screen* Parent);
            /// @brief Creates a new EditBox.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created EditBox will belong to.
            /// @return Returns a pointer to the created EditBox.
            virtual EditBox* CreateEditBox(const XML::Node& XMLNode, Screen* Parent);

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
        };//EditBoxFactory
    }//UI
}//Mezzanine

#endif
