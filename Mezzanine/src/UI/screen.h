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
#ifndef _uiscreen_h
#define _uiscreen_h

#include "matrix4x4.h"
#include "UI/glyph.h"
#include "UI/vertex.h"
#include "UI/quadrenderable.h"
#include "UI/widget.h"
#include "UI/widgetfactory.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Viewport;
        class SceneManager;
    }
    namespace UI
    {
        class Button;
        class CheckBox;
        class DropDownList;
        class EditBox;
        class GridContainer;
        class HorizontalContainer;
        class HorizontalScrollbar;
        class LineList;
        class ListBox;
        class MenuButton;
        class MenuEntry;
        class RadioButton;
        class Scrollbar;
        class Spinner;
        class TabSet;
        class VerticalContainer;
        class VerticalScrollbar;
        class Widget;
        class Window;

        class Sprite;
        class TextureAtlas;
        class MouseHoverStrategy;
        class MarkupParser;
        class UIManager;
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
            AtlasAndPosition(const String& AtlasName) : Atlas(AtlasName), RenderCount(0), RenderStart(0), RenderEnd(0) {};
        };//AtlasAndPosition

        ///////////////////////////////////////////////////////////////////////////////
        /// @class ScreenRenderData
        /// @headerfile screen.h
        /// @brief This class stores all vertices pertaining to a layer sorted by their priority for rendering.
        /// @details
        ///////////////////////////////////////
        class ScreenRenderData
        {
        public:
            std::vector<VertexData> LowVertices;
            std::vector<VertexData> MediumVertices;
            std::vector<VertexData> HighVertices;

            /// @brief Clears all Vertex vectors.
            void Clear();
            /// @brief Gets the combined size of all Vertex vectors.
            Whole Size();
            /// @brief Appends the contents of another ScreenRenderData to the end of this.
            /// @param OtherData The other ScreenRenderData to append.
            void Append(ScreenRenderData& OtherData);
            /// @brief Array access operator spanning all 3 Vertex vectors.
            /// @param Index The index to access.
            VertexData& operator[](const Whole& Index);
        };//ScreenRenderData

        ///////////////////////////////////////////////////////////////////////////////
        /// @class Screen
        /// @headerfile screen.h
        /// @brief This class is a helper class for creating UI's.  It is responsible for storing and keeping
        /// track of all the elements of a single UI screen.
        /// @details UI's can optionally be divided up into Screens.  Each screen is batched together
        /// for rendering, so keeping the amount of screens to a minimum will improve performance.
        ///////////////////////////////////////
        class MEZZ_LIB Screen : public QuadRenderable
        {
        public:
            /// @brief Basic container type for @ref Widget storage by this class.
            typedef std::map<String,Widget*>                WidgetContainer;
            /// @brief Iterator type for @ref Widget instances stored by this class.
            typedef WidgetContainer::iterator               WidgetIterator;
            /// @brief Const Iterator type for @ref Widget instances stored by this class.
            typedef WidgetContainer::const_iterator         ConstWidgetIterator;
            /// @brief Basic container type for @ref Widget storage by this class.
            typedef std::map<String,WidgetFactory*>         WidgetFactoryContainer;
            /// @brief Iterator type for @ref Widget instances stored by this class.
            typedef WidgetFactoryContainer::iterator        WidgetFactoryIterator;
            /// @brief Const Iterator type for @ref Widget instances stored by this class.
            typedef WidgetFactoryContainer::const_iterator  ConstWidgetFactoryIterator;
            /// @brief Basic container type for the storage of render data on this screen.
            typedef std::vector<AtlasAndPosition>           TextureVertexContainer;
            /// @brief Callback type for child processing.
            typedef bool (ChildCallback)(QuadRenderable* Quad);
        protected:
            friend class Mezzanine::UI::UIManager;

            /// @internal
            /// @brief A container storing a mapping of the textures needed to render each section of vertices.
            TextureVertexContainer TextureByVertex;
            /// @internal
            /// @brief A container storing the actual vertices to be rendered.
            ScreenRenderData VertexCache;
            /// @internal
            /// @brief A container storing all the factories for the widgets supported by this screen.
            WidgetFactoryContainer WidgetFactories;
            /// @internal
            /// @brief A container storing all the created Widgets owned by this screen.
            WidgetContainer Widgets;

            /// @internal
            /// @brief The transformation matrix used to update vertex transforms if needed.
            Matrix4x4 VertexTransform;
            /// @internal
            /// @brief The name of the this screens primary atlas for texture lookups.
            String PrimaryAtlas;
            /// @internal
            /// @brief The scaling to be applied to all vertices that are generated by this screen.
            /// @note This member is set once and barely used, could be replaced with individual constructions where used.
            Vector3 Scale;
            /// @internal
            /// @brief The pixel position on this screen where the mouse clicked on a Widget.
            Vector2 MouseHitPosition;
            /// @internal
            /// @brief The inverse size (1/size) of the viewport this screen is bound to in pixels.
            Vector2 InverseSize;

            /// @internal
            /// @brief A pointer to the class storing all sensative internal data THAT IS NOT FOR YOUR EYES!
            ScreenInternalData* SID;
            /// @internal
            /// @brief A pointer to the UIManager that owns this screen.
            UIManager* UIMan;
            /// @internal
            /// @brief A pointer to the viewport this screen is bound to.
            Graphics::Viewport* GameViewport;
            /// @internal
            /// @brief A pointer to the strategy to be used when detecting the hovered widget.
            MouseHoverStrategy* MouseStrat;

            /// @internal
            /// @brief The current orientation mode of this screen.
            Mezzanine::OrientationMode Orientation;

            /// @internal
            /// @brief Gets a pointer to the SceneManager connected to this screens viewport.
            /// @return Returns a pointer to the SceneManager that will be rendering this screen.
            virtual Graphics::SceneManager* GetSceneManager() const;
            /// @internal
            /// @brief Gets a registered WidgetFactory that creates the specified type of widget.
            /// @exception If a widget factor of the specified widget type isn't registered then a II_IDENTITY_NOT_FOUND_EXCEPTION will be thrown.
            /// @param WidgetTypeName The name of the widget type to retrieve the factory for.
            /// @return Returns a pointer to the specified factory.  This pointer will always be valid if an exception isn't thrown.
            virtual WidgetFactory* GetWidgetFactoryExcept(const String& WidgetTypeName);
            /// @internal
            /// @brief Verifies uniqueness of a widget and inserts it if it is.
            /// @exception If a widget with the same name already exists in this Screen, a II_DUPLICATE_IDENTITY_EXCEPTION will be thrown.
            /// @param ToInsert The widget to be verified and inserted.
            /// @return Returns a pointer to the same widget that was passed into this method.
            virtual Widget* CheckAndInsertExcept(Widget* ToInsert);

            /// @internal
            /// @brief Sets up all the necessary projection and world matrices for UI rendering.
            virtual void PrepareRenderSystem();
            /// @internal
            /// @brief Creates a new Vertex Buffer for vertices generated by the UI system.
            /// @param InitialSize The amount of space to allocate for the initial buffer, in number of vertices.
            virtual void CreateVertexBuffer(const Whole& InitialSize);
            /// @internal
            /// @brief Destroys the Vertex Buffer storing all the UI vertices generated by this screen.
            virtual void DestroyVertexBuffer();
            /// @internal
            /// @brief Resizes the Vertex Buffer.
            /// @note The Vertex Buffer will not shrink, only grow.  Passing in a smaller size will do nothing.
            /// @param RequestedSize The new size for the existing buffer.
            virtual void ResizeVertexBuffer(const Whole& RequestedSize);
        //public:
            /// @brief Internal constructor.
            /// @param RendName The name of this screen.
            /// @param Atlas The name of the primary atlas to be assigned to this screen.
            /// This can be overridden later, even by individual UI elements.
            /// @param WindowViewport The Viewport to which this screen belongs.
            /// @param Manager A pointer to the UI manager creating this screen.
            Screen(const String& RendName, const String& Atlas, Graphics::Viewport* WindowViewport, UIManager* Manager);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Manager A pointer to the UI manager creating this screen.
            Screen(const XML::Node& XMLNode, UIManager* Manager);
            /// @brief Class destructor.
            virtual ~Screen();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility and Visibility Methods

            /// @copydoc Renderable::SetVisible(Bool)
            virtual void SetVisible(Boolean CanSee);
            /// @copydoc Renderable::GetVisible() const
            virtual Boolean GetVisible() const;
            /// @copydoc Renderable::IsVisible() const
            virtual Boolean IsVisible() const;
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            /// @copydoc Renderable::GetRenderableType() const
            virtual RenderableType GetRenderableType() const;

            /// @brief Gets the current viewport dimensions.
            /// @return Returns a Vector2 representing the current viewport dimensions.
            virtual const Vector2& GetViewportDimensions() const;
            /// @brief Checks to see if the viewport has changed in size.  If so it updates all the UI elements on the screen.
            virtual void CheckViewportSize();
            /// @brief Gets the Viewport this screen is currently rendering to.
            /// @return Returns a pointer to the Viewport this screen is applied to.
            virtual Graphics::Viewport* GetViewport() const;
            /// @brief Gets the UIManager this screen belongs to.
            /// @return Returns a pointer to the UI manager that owns this screen.
            virtual UIManager* GetManager() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Mouse Hover Methods

            /// @brief Sets the strategy to use when detect which object the mouse is hovered over.
            /// @note This method will replace whatever method is currently set if one is set.
            /// @param Strategy A pointer to the strategy to use.
            virtual void SetMouseHoverStrategy(MouseHoverStrategy* Strategy);
            /// @brief Gets the MouseHoverStrategy currently being used by this screen.
            /// @return Returns a pointer to the currently set MouseHoverStrategy.
            virtual MouseHoverStrategy* GetMouseHoverStrategy() const;
            /// @brief Gets the quad the mouse is over if any.
            /// @param MousePos The current mouse position.
            /// @return Returns the Widget the mouse is over, or NULL if there are none.
            virtual Widget* FindHoveredWidget(const Vector2& MousePos);
            /// @brief Gets the mouse position from the last call to "FindHoveredQuad(const Vector2&).
            /// @note Screens have a 1:1 ratio to viewports, not windows.  The returned coordinates is in viewport space.
            /// @warning This will not perform any additional checks.  Only returns the result of the last check.
            /// Verify the screen this is being called on is the one with the mouse over if prior to calling or you may get outdated information.
            /// @return Returns a valid point on this screen if the last check was valid, or (-1,-1) if the check failed.
            virtual const Vector2& GetMouseHitPosition() const;

            ///////////////////////////////////////////////////////////////////////////////
            // WidgetFactory Management

            /// @brief Adds/registers a widget factory with this Screen, allowing it to be constructed through this API.
            /// @param ToBeAdded The widget factory to be added.
            void AddWidgetFactory(WidgetFactory* ToBeAdded);
            /// @brief Removes a widget factory from this Screen.
            /// @param ToBeRemoved A pointer to the widget factory that is to be removed.
            void RemoveWidgetFactory(WidgetFactory* ToBeRemoved);
            /// @brief Removes a widget factory from this Screen.
            /// @param ImplName The name of the widget implementation created by the factory to be removed.
            void RemoveWidgetFactory(const String& ImplName);
            /// @brief Removes and destroys a widget factory in this Screen.
            /// @param ToBeDestroyed A pointer to the widget factory that is to be removed and destroyed.
            void DestroyWidgetFactory(WidgetFactory* ToBeDestroyed);
            /// @brief Removes and destroys a widget factory in this Screen.
            /// @param ImplName The name of the widget implementation created by the factory to be removed and destroyed.
            void DestroyWidgetFactory(const String& ImplName);
            /// @brief Destroys all widget factories in this Screen.
            /// @warning The destruction of widget factories should only be done after the corresponding managers have been destroyed, otherwise this will cause an exception.
            void DestroyAllWidgetFactories();
            /// @brief Adds all the default widget factories provided by the engine to the Screen.
            void AddAllDefaultWidgetFactories();

            ///////////////////////////////////////////////////////////////////////////////
            // Widget Management

            /// @brief Creates a widget from an XML::Node.
            /// @exception This method will throw an exception if the WidgetNode is not named after a known widget.
            /// @param WidgetNode The XML node populated with data needed to construct a widget.
            /// @return Returns a pointer to the created widget.
            virtual Widget* CreateWidget(const XML::Node& WidgetNode);

            /// @brief Gets a widget in this screen by name.
            /// @param Name The name of the widget to get.
            /// @return Returns a pointer to the widget of the specified name.
            virtual Widget* GetWidget(const String& Name);
            /// @brief Gets the number of widgets being used in this screen.
            /// @return Returns the number of widgets this screen is storing.
            virtual Whole GetNumWidgets();

            /// @brief Destroys a widget.
            /// @param ToBeDestroyed Pointer to the widget you want destroyed.
            virtual void DestroyWidget(Widget* ToBeDestroyed);
            /// @brief Destroys all widgets being stored by this screen.
            virtual void DestroyAllWidgets();

            ///////////////////////////////////////////////////////////////////////////////
            // Convenience Widget Creation Methods

            /// @brief Creates a generic widget.
            /// @note This is not a polymorphic create method.  It will case an instance of the widget base class.
            /// @param Name The name to be given to this Widget.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(const String& Name);
            /// @brief Creates a generic widget.
            /// @note This is not a polymorphic create method.  It will case an instance of the widget base class.
            /// @param Name The name to be given to this Widget.
            /// @param RendRect The rect describing this Widget's transform relative to it's parent.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(const String& Name, const UnifiedRect& RendRect);
            /// @brief Creates a Button.
            /// @param Name The name to be given to this Button.
            /// @return Returns a pointer to the created Button.
            virtual Button* CreateButton(const String& Name);
            /// @brief Creates a Button.
            /// @param Name The name to be given to this Button.
            /// @param RendRect The rect describing this Button's transform relative to it's parent.
            /// @return Returns a pointer to the created Button.
            virtual Button* CreateButton(const String& Name, const UnifiedRect& RendRect);
            /// @brief Creates a MenuButton.
            /// @param Name The name to be given to this MenuButton.
            /// @return Returns a pointer to the created MenuButton.
            virtual MenuButton* CreateMenuButton(const String& Name);
            /// @brief Creates a MenuButton.
            /// @param Name The name to be given to this MenuButton.
            /// @param RendRect The rect describing this MenuButton's transform relative to it's parent.
            /// @return Returns a pointer to the created MenuButton.
            virtual MenuButton* CreateMenuButton(const String& Name, const UnifiedRect& RendRect);
            /// @brief Creates a CheckBox.
            /// @param Name The name of the CheckBox.
            /// @return Returns a pointer to the created CheckBox.
            virtual CheckBox* CreateCheckBox(const String& Name);
            /// @brief Creates a CheckBox.
            /// @param Name The name of the CheckBox.
            /// @param RendRect The Rect representing the position and size of the CheckBox.
            /// @return Returns a pointer to the created CheckBox.
            virtual CheckBox* CreateCheckBox(const String& Name, const UnifiedRect& RendRect);
            /// @brief Creates a Scrollbar aligned on the X axis.
            /// @param Name The name of the HorizontalScrollbar.
            /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
            /// @return Returns a pointer to the created HorizontalScrollbar.
            virtual HorizontalScrollbar* CreateHorizontalScrollbar(const String& Name, const UI::ScrollbarStyle Style);
            /// @brief Creates a Scrollbar aligned on the X axis.
            /// @param Name The name of the HorizontalScrollbar.
            /// @param RendRect The Rect representing the position and size of the HorizontalScrollbar.
            /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
            /// @return Returns a pointer to the created HorizontalScrollbar.
            virtual HorizontalScrollbar* CreateHorizontalScrollbar(const String& Name, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style);
            /// @brief Creates a Scrollbar aligned on the Y axis.
            /// @param Name The name of the VerticalScrollbar.
            /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
            /// @return Returns a pointer to the created VerticalScrollbar.
            virtual VerticalScrollbar* CreateVerticalScrollbar(const String& Name, const UI::ScrollbarStyle Style);
            /// @brief Creates a Scrollbar aligned on the Y axis.
            /// @param Name The name of the VerticalScrollbar.
            /// @param RendRect The Rect representing the position and size of the VerticalScrollbar.
            /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
            /// @return Returns a pointer to the created VerticalScrollbar.
            virtual VerticalScrollbar* CreateVerticalScrollbar(const String& Name, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style);

            /// @brief Creates a MenuEntry.
            /// @param Name The name to be given to this MenuEntry.
            /// @return Returns a pointer to the created MenuEntry.
            virtual MenuEntry* CreateMenuEntry(const String& Name);
            /// @brief Creates a MenuEntry.
            /// @param Name The name to be given to this MenuEntry.
            /// @param RendRect The rect describing this MenuEntry's transform relative to it's parent.
            /// @return Returns a pointer to the created MenuEntry.
            virtual MenuEntry* CreateMenuEntry(const String& Name, const UnifiedRect& RendRect);

            /// @brief Creates a widget container aligned on the X axis.
            /// @param RendName The name to be given to this renderable.
            /// @return Returns a pointer to the created HorizontalContainer.
            virtual HorizontalContainer* CreateHorizontalContainer(const String& RendName);
            /// @brief Creates a widget container aligned on the X axis.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @return Returns a pointer to the created HorizontalContainer.
            virtual HorizontalContainer* CreateHorizontalContainer(const String& RendName, const UnifiedRect& RendRect);
            /// @brief Creates a widget container aligned on the Y axis.
            /// @param RendName The name to be given to this renderable.
            /// @return Returns a pointer to the created VerticalContainer.
            virtual VerticalContainer* CreateVerticalContainer(const String& RendName);
            /// @brief Creates a widget container aligned on the Y axis.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @return Returns a pointer to the created VerticalContainer.
            virtual VerticalContainer* CreateVerticalContainer(const String& RendName, const UnifiedRect& RendRect);

            /*/// @brief Creates a List Box.
            /// @return Returns a pointer to the created List Box.
            /// @param Name The name of the List Box.
            /// @param RendRect The Rect representing the position and size of the List Box.
            /// @param ScrollStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
            virtual ListBox* CreateListBox(ConstString& Name, const Rect& RendRect, const UI::ScrollbarStyle& ScrollStyle);
            /// @brief Creates a Spinner.
            /// @return Returns a pointer to the created Spinner.
            /// @param Name The Name for the Widget.
            /// @param RendRect The Rect representing the position and size of the Spinner.
            /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
            /// @param GlyphHeight The desired relative height of the text you want.
            virtual Spinner* CreateSpinner(ConstString& Name, const Rect& RendRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
            /// @brief Creates a scrolled cell grid.
            /// @return Returns a pointer to the created ScrolledCellGrid.
            /// @param Name The name of the widget.
            /// @param RendRect The Rect representing the position and size of the ScrolledCellGrid.
            /// @param Thickness The width of the vertical scrollbar in relative units.  The same amount of actual pixels is used
            /// to determine the height of the horizontal scrollbar.
            /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
            virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const Rect& RendRect, const Real& Thickness, const UI::ScrollbarStyle& Style);
            /// @brief Creates a paged cell grid.
            /// @return Returns a pointer to the created PagedCellGrid.
            /// @param Name The name of the widget.
            /// @param RendRect The Rect representing the position and size of the PagedCellGrid.
            /// @param SpnRect The Rect representing the position and size of the Spinner.
            /// @param SStyle The style of spinner to create.
            /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
            virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const Rect& RendRect, const Rect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
            /// @brief Creates a drop down list.
            /// @return Returns a pointer to the created DropDownList.
            /// @param Name The Name for the Widget.
            /// @param RendRect The renderable rect representing the position and size of this widget.
            /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
            /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar class for more information.
            virtual DropDownList* CreateDropDownList(ConstString& Name, const Rect& RendRect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
            /// @brief Creates a tabset.
            /// @return Returns a pointer to the created tabset.
            /// @param Name The Name for the Widget.
            /// @param SetRect The Rect representing the position and size of all the Renderable Sets generated by the tabset.
            virtual TabSet* CreateTabSet(ConstString& Name, const Rect& SetRect);

            /// @brief Creates a Window.
            /// @return Returns a pointer to the created Window.
            /// @param Name The name of the Window.
            /// @param RendRect The Rect representing the position and size of the Window.
            virtual Window* CreateWidgetWindow(ConstString& Name, const Rect& RendRect);//*/

            ///////////////////////////////////////////////////////////////////////////////
            // Atlas Query

            /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
            /// @param Atlas The name of the atlas to be used.
            virtual void SetPrimaryAtlas(const String& Atlas);
            /// @brief Gets the currently set primary atlas.
            /// @return Returns a string containing the name of the primary atlas that is set.
            virtual String GetPrimaryAtlas();
            /// @brief Gets the position of the white pixel from an Atlas.
            /// @param Atlas The name of the Atlas to get the white pixel from.
            /// @return Returns a Vector2 with the location of white pixel on the Atlas.
            Vector2 GetWhitePixel(const String& Atlas) const;
            /// @brief Gets a sprite from an Atlas.
            /// @param SpriteName The name of the sprite to retrieve.
            /// @param Atlas The name of the Atlas to get the sprite from.
            /// @return Returns a pointer to the requested Sprite.
            Sprite* GetSprite(const String& SpriteName, const String& Atlas) const;
            /// @brief Gets the specified FontData from an Atlas.
            /// @param FontName The name of the Font to retrieve.
            /// @param Atlas The name of the atlas to check the specified FontData for.
            /// @return Returns a pointer to the requested FontData.
            FontData* GetFont(const String& FontName, const String& Atlas) const;
            /// @brief Gets the texture size of the specified Atlas.
            /// @param Atlas The name of the atlas to get the texture size of.
            /// @return Returns a Vector2 containing the size of the requested Atlas.
            Vector2 GetTextureSize(const String& Atlas) const;
            /// @brief Gets an atlas that has been loaded.
            /// @param Atlas The name of the Atlas to retrieve, usually stored as a filename.
            /// @return Returns a pointer to the requested Texture Atlas.
            TextureAtlas* GetAtlas(const String& Atlas) const;
            /// @brief Gets the X axis Texel Offset for the current rendersystem.
            /// @return Retruns a real containing the texel offset to be applied to renderables on this screen.
            Real GetTexelOffsetX() const;
            /// @brief Gets the Y axis Texel Offset for the current rendersystem.
            /// @return Retruns a real containing the texel offset to be applied to renderables on this screen.
            Real GetTexelOffsetY() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Other Query

            /// @brief Checks to see if a MarkupParser has already been registsered under a specific name.
            /// @param ParserName The name of the MarkupParser to check for.
            /// @return Returns true if a MarkupParser is registered under the specified name.
            bool IsMarkupParserRegistered(const String& ParserName) const;
            /// @brief Gets a MarkupParser by it's registered name.
            /// @param ParserName The name of the MarkupParser to retrieve.
            /// @return Returns a pointer to the requested MarkupParser, or NULL if none are registered under the specified name.
            UI::MarkupParser* GetMarkupParser(const String& ParserName) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Functions

            /// @copydoc Renderable::_MarkDirty()
            void _MarkDirty();
            /// @copydoc QuadRenderable::_MarkAllLayersDirty()
            virtual void _MarkAllLayersDirty();
            /// @internal
            /// @brief Manually calls the UI system to render this screen.
            void _RenderScreen();
            /// @internal
            /// @brief Forces an orientation mode change for this screen.
            /// @param Mode The orientation mode to be applied to the UI on this screen.
            void _SetOrientation(const Mezzanine::OrientationMode& Mode);
            /// @internal
            /// @brief Updates the vertex positions so they are in front of the camera in world space.
            /// @param RenderData Vector of the vertices to be transformed.
            /// @param Begin The first Vertex to transform in the range.
            /// @param End The last Vertex to transform in the range.
            void _Transform(ScreenRenderData& RenderData, const Whole& Begin, const Whole& End);
            /// @internal
            /// @brief Prepares all vertices for rendering to the screen.
            /// @param Force Whether or not to force preparation regardless of if they need it.
            void _RenderVertices(bool Force = false);

            /// @internal
            /// @brief Processes all children of this screen by their zorder.
            /// @param CB The callback implementing how the children are to be processed.
            /// @return Returns the result of the callback, and exits early if true.
            template<typename Callback>
            Boolean _ProcessAllChildren(Callback* CB)
            {
                for( ChildIterator ChildIt = this->ChildrenBegin() ; ChildIt != this->ChildrenEnd() ; ++ChildIt )
                {
                    if( Screen::_ProcessAllChildren((*ChildIt),CB) )
                        return true;
                }
                return false;
            }
            /// @internal
            /// @brief Processes all children of this screen in reverse zorder.
            /// @param CB The callback implementing how the children are to be processed.
            /// @return Returns the result of the callback, and exits early if true.
            template<typename Callback>
            Boolean _ReverseProcessAllChildren(Callback* CB)
            {
                for( ReverseChildIterator RChildIt = this->RChildrenBegin() ; RChildIt != this->RChildrenEnd() ; ++RChildIt )
                {
                    if( Screen::_ReverseProcessAllChildren((*RChildIt),CB) )
                        return true;
                }
                return false;
            }
            /// @internal
            /// @brief Processes all children of this screen by their zorder.
            /// @param Wid The Widget whose children need to be processed.
            /// @param CB The callback implementing how the children are to be processed.
            /// @return Returns the result of the callback, and exits early if true.
            template<typename Callback>
            static Boolean _ProcessAllChildren(Widget* Wid, Callback* CB)
            {
                if( (*CB)(Wid) )
                    return true;
                for( ChildIterator ChildIt = Wid->ChildrenBegin() ; ChildIt != Wid->ChildrenEnd() ; ++ChildIt )
                {
                    if( Screen::_ProcessAllChildren((*ChildIt),CB) )
                        return true;
                }
                return false;
            }
            /// @internal
            /// @brief Processes all children of this screen in reverse zorder.
            /// @param Wid The Widget whose children need to be processed.
            /// @param CB The callback implementing how the children are to be processed.
            /// @return Returns the result of the callback, and exits early if true.
            template<typename Callback>
            static Boolean _ReverseProcessAllChildren(Widget* Wid, Callback* CB)
            {
                for( ReverseChildIterator RChildIt = Wid->RChildrenBegin() ; RChildIt != Wid->RChildrenEnd() ; ++RChildIt )
                {
                    if( Screen::_ReverseProcessAllChildren((*RChildIt),CB) )
                        return true;
                }
                if( (*CB)(Wid) )
                    return true;

                return false;
            }
        };//uiscreen
    }//ui
}//Mezzanine

#endif
