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
#ifndef _uiscreen_h
#define _uiscreen_h

#include "matrix4x4.h"
#include "UI/glyph.h"
#include "UI/vertex.h"
#include "UI/renderablecontainer.h"

namespace Mezzanine
{
    class UIManager;
    class SceneManager;
    namespace Graphics
    {
        class Viewport;
    }
    namespace UI
    {
        class Button;
        class Widget;
        class Sprite;
        class TextureAtlas;
        struct ScreenInternalData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class AtlasAndPosition
        /// @headerfile screen.h
        /// @brief This class stores how the calls to Render are to be done.
        /// @details
        ///////////////////////////////////////
        struct AtlasAndPosition
        {
            String Atlas;
            Whole RenderCount;
            Whole RenderStart;
            Whole RenderEnd;

            AtlasAndPosition() : RenderCount(0), RenderStart(0), RenderEnd(0) {};
        };
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ScreenVertexData
        /// @headerfile screen.h
        /// @brief This class stores all vertices pertaining to a layer sorted by their priority for rendering.
        /// @details
        ///////////////////////////////////////
        class ScreenVertexData
        {
            public:
            std::vector<VertexData> LowVertices;
            std::vector<VertexData> MediumVertices;
            std::vector<VertexData> HighVertices;

            void Clear();
            Whole Size();
            VertexData& operator[](const Whole& Index);
        };
        ///////////////////////////////////////////////////////////////////////////////
        /// @class IndexData
        /// @headerfile uiscreen.h
        /// @brief This is a basic class for storing data relating to a specific zorder in the UI.
        /// @details This class caches a root widget, it's verticies, and whether it needs to be redrawn.
        ///////////////////////////////////////
        struct IndexData
        {
            Widget* IndexWidget;
            ScreenVertexData Vertices;

            IndexData() : IndexWidget(NULL) {};
        };
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Screen
        /// @headerfile screen.h
        /// @brief This class is a helper class for creating UI's.  It is responsible for storing and keeping
        /// track of all the elements of a single UI screen.
        /// @details UI's can optionally be divided up into Screens, or "pages".  Each screen is batched together
        /// for rendering, so keeping the amount of screens to a minimum will improve performance.
        ///////////////////////////////////////
        class MEZZ_LIB Screen : public ExtendedRenderableFactory
        {
            public:
                typedef std::map<UInt16,IndexData*>      IndexMap;
                typedef IndexMap::iterator               IndexIterator;
                typedef IndexMap::reverse_iterator       ReverseIndexIterator;
                typedef IndexMap::const_iterator         ConstIndexIterator;
                typedef IndexMap::const_reverse_iterator ConstReverseIndexIterator;
                typedef std::map<String,Widget*>         WidgetContainer;
                typedef WidgetContainer::iterator        WidgetIterator;
                typedef WidgetContainer::const_iterator  ConstWidgetIterator;
            protected:
                friend class Mezzanine::UIManager;
                Graphics::Viewport* GameViewport;
                ScreenInternalData* SID;
                bool OrientationChanged;
                bool Visible;
                bool IndexRedrawNeeded;
                bool IndexRedrawAll;
                bool ViewportSizeChanged;
                Whole VertexBufferSize;
                Mezzanine::OrientationMode Orientation;
                Vector2 KnownViewportSize;
                Vector2 InverseViewportSize;
                Vector3 Scale;
                Matrix4x4 VertexTransform;
                String Name;
                String PrimaryAtlas;
                std::vector<AtlasAndPosition> TextureByVertex;

                IndexMap WidgetsRoot;
                WidgetContainer Widgets;

                virtual SceneManager* GetSceneManager();
                virtual Screen* GetScreen();
                virtual Widget* CheckAndInsert(Widget* ToInsert);
                virtual void PrepareRenderSystem();
                virtual void CreateVertexBuffer(const Whole& InitialSize = 32);
                virtual void DestroyVertexBuffer();
                virtual void ResizeVertexBuffer(const Whole& RequestedSize);
            //public:
                /// @brief Internal constructor.
                /// @param name The name of this screen.
                /// @param Atlas The name of the primary atlas to be assigned to this screen.
                /// This can be overridden later, even by individual UI elements.
                /// @param WindowViewport The Viewport to which this screen belongs.
                Screen(const String& name, const String& Atlas, Graphics::Viewport* WindowViewport);
                /// @brief Class destructor.
                virtual ~Screen();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Utility and Visibility Methods

                /// @brief Gets the name of this screen.
                /// @return Returns a string containing the name of this screen.
                virtual String& GetName();
                /// @brief Sets the screens visability.
                /// @param visable A bool representing the visability of the screen.
                virtual void SetVisible(bool visible);
                /// @brief Gets the screens visability.
                /// @return Returns a bool representing the visability of the screen.
                virtual bool IsVisible();
                /// @brief Forces the screen to be shown.
                virtual void Show();
                /// @brief Forces the screen to hide.
                virtual void Hide();
                /// @brief Gets the current viewport dimensions.
                /// @return Returns a Vector2 representing the current viewport dimensions.
                virtual const Vector2& GetViewportDimensions();
                /// @brief Checks to see if the viewport has changed in size.  If so it updates all the UI elements on the screen.
                virtual void CheckViewportSize();
                /// @brief Gets the Viewport this screen is currently rendering to.
                /// @return Returns a pointer to the Viewport this screen is applied to.
                virtual Graphics::Viewport* GetViewport();

                ///////////////////////////////////////////////////////////////////////////////
                // Hover Checks

                /// @brief Gets the widget the mouse is over if any.
                /// @details This function searches only the visable layers contained in this screen.
                /// @return Returns the widget the mouse is over, or NULL if there are none.
                virtual Widget* CheckWidgetMouseIsOver();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @copydoc RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @copydoc RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text);
                /// @copydoc RenderableFactory::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableFactory::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& LabelText);
                /// @copydoc RenderableFactory::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle)
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableFactory::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableFactory::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableFactory::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableFactory::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableFactory::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect);
                /// @copydoc RenderableFactory::CreateEnclosedRenderableContainerWidget(ConstString& Name, const RenderableRect& Rect)
                virtual EnclosedRenderableContainerWidget* CreateEnclosedRenderableContainerWidget(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc ExtendedRenderableFactory::CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect)
                virtual Window* CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc ExtendedRenderableFactory::CreateMenu(ConstString& Name, const RenderableRect& Rect)
                virtual Menu* CreateMenu(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc ExtendedRenderableFactory::CreateOpenRenderableContainerWidget(ConstString& Name)
                virtual OpenRenderableContainerWidget* CreateOpenRenderableContainerWidget(ConstString& Name);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets

                /// @brief Adds a widget to the root of this screen for rendering.
                /// @param ZOrder The ZOrder the widget will be rendered at on this screen relative to other root widgets.
                /// @param ToAdd The widget to be added.
                virtual void AddRootWidget(const Whole& ZOrder, Widget* ToAdd);
                /// @brief Gets a widget in this screen by name.
                /// @param Name The name of the widget to get.
                /// @return Returns a pointer to the widget of the specified name.
                virtual Widget* GetWidget(ConstString& Name);
                /// @brief Gets a widget by it's ZOrder among the root widgets on this screen.
                /// @param ZOrder The ZOrder of the widget to get.
                /// @return Returns a pointer to the widget with the specified ZOrder.
                virtual Widget* GetWidget(const Whole& ZOrder);
                /// @brief Gets the number of widgets being used in this screen.
                /// @return Returns the number of widgets this screen is storing.
                virtual Whole GetNumWidgets();

                ///////////////////////////////////////////////////////////////////////////////
                // Atlas Query

                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set.
                virtual String GetPrimaryAtlas();
                /// @brief Gets the solid UV position from an Atlas.
                /// @param Atlas The name of the Atlas to get the Solid UV from.
                /// @return Returns a Vector2 with the location of Solid UV on the Atlas.
                Vector2 GetSolidUV(const String& Atlas) const;
                /// @brief Gets a sprite from an Atlas.
                /// @param SpriteName The name of the sprite to retrieve.
                /// @param Atlas The name of the Atlas to get the sprite from.
                /// @return Returns a pointer to the requested Sprite.
                Sprite* GetSprite(const String& SpriteName, const String& Atlas) const;
                /// @brief Gets the specified GlyphData from an Atlas.
                /// @param ID The identification number associated with the loaded GlyphData.
                /// @param Atlas The name of the atlas to check the specified GlyphData ID for.
                /// @return Returns a pointer to the requested GlyphData.
                GlyphData* GetGlyphData(const Whole& ID, const String& Atlas) const;
                /// @brief Gets the texture size of the specified Atlas.
                /// @param Atlas The name of the atlas to get the texture size of.
                /// @return Returns a Vector2 containing the size of the requested Atlas.
                Vector2 GetTextureSize(const String& Atlas) const;
                /// @brief Gets an atlas that has been loaded.
                /// @param Atlas The name of the Atlas to retrieve, usually stored as a filename.
                /// @return Returns a pointer to the requested Texture Atlas.
                TextureAtlas* GetAtlas(const String& Atlas) const;
                /// @brief Gets the ColourValue set at the specified index.
                /// @param Index The index of the Markup colour to get.
                /// @param Atlas The Atlas to check the markup colours of.
                /// @return Returns a ColourValue with the colour at the provided index, or White if the Index is invalid(or if that is the colour set, which would be silly).
                ColourValue GetMarkupColour(const Whole& Index, const String& Atlas) const;
                /// @brief Gets the X axis Texel Offset for the current rendersystem.
                /// @return Retruns a real containing the texel offset to be applied to renderables on this screen.
                virtual Real GetTexelOffsetX() const;
                /// @brief Gets the Y axis Texel Offset for the current rendersystem.
                /// @return Retruns a real containing the texel offset to be applied to renderables on this screen.
                virtual Real GetTexelOffsetY() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions

                /// @internal
                /// @brief Manually calls the UI system to render this screen.
                virtual void _RenderScreen();
                /// @internal
                /// @brief Forces an orientation mode change for this screen.
                /// @param Mode The orientation mode to be applied to the UI on this screen.
                virtual void _SetOrientation(const Mezzanine::OrientationMode& Mode);
                /// @internal
                /// @brief Updates the vertex positions so they are in front of the camera in world space.
                /// @param Vertices Vector of the vertices to be transformed.
                /// @param Begin The first Vertex to transform in the range.
                /// @param End The last Vertex to transform in the range.
                virtual void _Transform(ScreenVertexData& Vertices, const Whole& Begin, const Whole& End);
                /// @internal
                /// @brief Clears the existing verticies and regenerates them for a specific index(widget).
                /// @param Index The Index corresponding to the widget you want regenerated.
                virtual void _RedrawIndex(const UInt32& Index);
                /// @internal
                /// @brief Clears all verticies for every widget and regenerates them for rendering.
                /// @param Force Whether or not to force a redraw on all layers regardless of if they need it.
                virtual void _RedrawAllIndexes(bool Force = false);
                /// @internal
                /// @brief Flags a widget for redraw before the next render.
                /// @param Index The ID or Index of the widget to be flagged for redraw.
                virtual void _RequestIndexRedraw(const UInt16& Index);
                /// @internal
                /// @brief Prepares all vertices for rendering to the screen.
                /// @param Force Whether or not to force preparation regardless of if they need it.
                virtual void _RenderVertices(bool Force = false);
        };//uiscreen
    }//ui
}//Mezzanine

#endif
