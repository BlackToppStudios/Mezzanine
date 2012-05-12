//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#include "UI/vertex.h"

namespace Mezzanine
{
    class Viewport;
    class UIManager;
    class SceneManager;
    namespace UI
    {
        class Button;
        class Widget;
        class Layer;
        struct ScreenInternalData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class AtlasAndPosition
        /// @headerfile uiscreen.h
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
        /// @headerfile uiscreen.h
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
        /// @details This class caches the Layer, it's verticies, and whether it needs to be redrawn.
        ///////////////////////////////////////
        struct IndexData
        {
            Layer* IndexLayer;
            bool RedrawNeeded;
            ScreenVertexData Vertices;

            IndexData() : IndexLayer(NULL), RedrawNeeded(true) {};
        };
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Screen
        /// @headerfile uiscreen.h
        /// @brief This class is a helper class for creating UI's.  It is responsible for storing and keeping
        /// track of all the elements of a single UI screen.
        /// @details UI's can optionally be divided up into Screens, or "pages".  Each screen is batched together
        /// for rendering, so keeping the amount of screens to a minimum will improve performance.
        ///////////////////////////////////////
        class MEZZ_LIB Screen
        {
            protected:
                UIManager* Manager;
                SceneManager* SceneMan;
                Viewport* GameViewport;
                ScreenInternalData* SID;
                bool OrientationChanged;
                bool Visible;
                bool CanRender;
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
                std::vector<Layer*> Layers;
                std::map<Whole,IndexData*> Indexes;

                virtual void PrepareRenderSystem();
                virtual void CreateVertexBuffer(const Whole& InitialSize = 32);
                virtual void DestroyVertexBuffer();
                virtual void ResizeVertexBuffer(const Whole& RequestedSize);
            public:
                /// @brief Internal constructor.
                /// @param name The name of this screen.
                /// @param Atlas The name of the primary atlas to be assigned to this screen.
                /// This can be overridden later, even by individual UI elements.
                /// @param WindowViewport The Viewport to which this screen belongs.
                Screen(const String& name, const String& Atlas, Viewport* WindowViewport);
                /// @brief Class destructor.
                virtual ~Screen();
                /// @brief Gets the name of this screen.
                /// @return Returns a string containing the name of this screen.
                virtual String& GetName();
                /// @brief Sets the screens visability.
                /// @param Visable A bool representing the visability of the screen.
                virtual void SetVisible(bool Visible);
                /// @brief Gets the screens visability.
                /// @return Returns a bool representing the visability of the screen.
                virtual bool IsVisible();
                /// @brief Forces the screen to be shown.
                virtual void Show();
                /// @brief Forces the screen to hide.
                virtual void Hide();
                /// @brief Creates a layer in the GUI screen to place GUI objects in.
                /// @details A GUI layer is exactly that, a layer of GUI objects.  You can have multiple GUI
                /// layers per screen.  The Zorder of the layer determines it's visability if there are multiple
                /// layers.  If the Zorder of one layer is higher then another in the same space, then the Zorder
                /// with the higher Zorder will be rendered...giving it the appearance of being on top of the other
                /// GUI object or objects.
                /// @param Name The name to be given to the layer.
                /// @param Zorder The layers Zorder, as explained above.
                virtual Layer* CreateLayer(const String& Name, const Whole& Zorder);
                /// @brief Gets an already created layer by name.
                /// @return Returns a pointer to the layer of the specified name.
                virtual Layer* GetLayer(const String& Name);
                /// @brief Gets an already created layer by it's index.
                /// @return Returns a pointer to the layer at the specified index.
                virtual Layer* GetLayer(const Whole& Index);
                /// @brief Gets an already created layer by it's Zorder.
                /// @return Returns a pointer to the layer with the specified Zorder.
                virtual Layer* GetLayerbyZorder(const Whole& Zorder);
                /// @brief Gets the number of layers created and stored in this class.
                /// @return Returns the number of layers this class is storing.
                virtual Whole GetNumLayers();
                /// @brief Destroy's a previously created layer.
                /// @param ToBeDestroyed The layer to be destroyed.
                virtual void DestroyLayer(Layer* ToBeDestroyed);
                /// @brief Gets the current viewport dimensions.
                /// @return Returns a Vector2 representing the current viewport dimensions.
                virtual const Vector2& GetViewportDimensions();
                /// @brief Gets the X axis Texel Offset for the current rendersystem.
                /// @return Retruns a real containing the texel offset to be applied to renderables on this screen.
                virtual Real GetTexelOffsetX() const;
                /// @brief Gets the Y axis Texel Offset for the current rendersystem.
                /// @return Retruns a real containing the texel offset to be applied to renderables on this screen.
                virtual Real GetTexelOffsetY() const;
                /// @brief Gets the button the mouse is over if any.
                /// @details This function searches only the visable layers contained in this screen.
                /// @return Returns the button the mouse is over, or NULL if there are none.
                virtual Button* CheckButtonMouseIsOver();
                /// @brief Gets the widget the mouse is over if any.
                /// @details This function searches only the visable layers contained in this screen.
                /// @return Returns the widget the mouse is over, or NULL if there are none.
                virtual Widget* CheckWidgetMouseIsOver();
                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set.
                virtual String GetPrimaryAtlas();
                /// @brief Checks to see if the viewport has changed in size.  If so it updates all the UI elements on the screen.
                virtual void CheckViewportSize();
                /// @brief Gets the Viewport this screen is currently rendering to.
                /// @return Returns a pointer to the Viewport this screen is applied to.
                virtual Viewport* GetViewport();

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
                virtual void _Transform(std::vector<VertexData>& Vertices, const Whole& Begin, const Whole& End);
                /// @internal
                /// @brief Clears the layer and index data and rebuilds in from the existing layers.
                virtual void _RecalculateIndexes();
                /// @internal
                /// @brief Clears the existing verticies and regenerates them for a specific index(layer).
                /// @param Index The Index corresponding to the layer you want regenerated.
                /// @param Force Whether or not to force a redraw even if the layer doesn't need it.
                virtual void _RedrawIndex(const UInt32& Index, bool Force);
                /// @internal
                /// @brief Clears all verticies for every layer and regenerates them for rendering.
                /// @param Force Whether or not to force a redraw on all layers regardless of if they need it.
                virtual void _RedrawAllIndexes(bool Force = false);
                /// @internal
                /// @brief Flags a layer for redraw before the next render.
                /// @param Index The ID or Index of the layer to be flagged for redraw.
                virtual void _RequestIndexRedraw(const UInt32& Index);
                /// @internal
                /// @brief Prepares all vertices for rendering to the screen.
                /// @param Force Whether or not to force preparation regardless of if they need it.
                virtual void _RenderVertices(bool Force = false);
        };//uiscreen
    }//ui
}//Mezzanine

#endif
