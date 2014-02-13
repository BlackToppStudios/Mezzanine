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
#ifndef _uiquadrenderable_h
#define _uiquadrenderable_h

#include "datatypes.h"
#include "UI/uienumerations.h"
#include "UI/unifieddim.h"
#include "UI/positioninginfo.h"
#include "UI/sizinginfo.h"
#include "UI/renderable.h"
#include "UI/rect.h"

namespace Mezzanine
{
    namespace UI
    {
        class ScreenRenderData;
        class RenderLayer;
        class ImageLayer;
        class SingleLineTextLayer;
        class MultiLineTextLayer;
        class LayoutStrategy;
        class QuadRenderable;
        class Widget;
        //////////////////////////////////////////////////////////////////////////////
        /// @struct RenderLayerGroup
        /// @headerfile layeredrenderable.h
        /// @brief This class stores a group of render layers that can be set to be rendered.
        /// @details A QuadRenderable can only render one group of layers at a time, but a single layer can be added to
        /// as many RenderLayerGroup's as the user see's fit.
        ///////////////////////////////////////
        struct MEZZ_LIB RenderLayerGroup
        {
        public:
            /// @brief An std::pair type for storing ZOrders in relation to @ref RenderLayer instances.
            typedef std::pair<UInt16,RenderLayer*>        RenderLayerPair;
            /// @brief Basic container type for @ref RenderLayerPair storage by this class.
            typedef std::list<RenderLayerPair>            RenderLayerContainer;
            /// @brief Iterator type for @ref RenderLayerPair instances stored by this class.
            typedef RenderLayerContainer::iterator        RenderLayerIterator;
            /// @brief Const Iterator type for @ref RenderLayerPair instances stored by this class.
            typedef RenderLayerContainer::const_iterator  ConstRenderLayerIterator;
        protected:
            /// @internal
            /// @brief Container storing all the layers that belong to this group and their ZOrders.
            RenderLayerContainer RenderLayers;
            /// @internal
            /// @brief The name of this group.
            String GroupName;
            /// @internal
            /// @brief A pointer to the host QuadRenderable.
            QuadRenderable* ParentQuad;
        public:
            /// @brief Class constructor.
            /// @param Name The name to give to this RenderLayerGroup.
            /// @param Creator The Quad that owns this RenderLayerGroup.
            RenderLayerGroup(const String& Name, QuadRenderable* Creator);
            /// @brief Class destructor.
            ~RenderLayerGroup();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this RenderLayerGroup.
            /// @return Returns a const reference to the name of this RenderLayerGroup.
            const String& GetName() const;
            /// @brief Notifies this RenderLayerGroup that it has become the active group.
            /// @remarks This shouldn't need to ever be called manually and is automatically called when
            /// a QuadRenderable sets it as the active RenderLayerGroup.  This method exists to reset the
            /// state of a layer (or group of layers) if necessary to achieve the visual effect desired.
            void NotifyActive();
            /// @brief Notifies this RenderLayerGroup that it is no longer the active group.
            /// @remarks This is a straightforward counterpart to the "NotifyActive" method also on this
            /// class, and also shouldn't ever need to be called manually.
            void NotifyInactive();

            ///////////////////////////////////////////////////////////////////////////////
            // RenderLayer Management

            /// @brief Adds a layer to this group by it's ZOrder.
            /// @param RL The layer to add.
            /// @param ZOrder The ZOrder at which to add the layer.
            void AddLayer(RenderLayer* RL, const UInt16 ZOrder);
            /// @brief Gets a RenderLayer in this group by it's index.
            /// @param Index The index of the RenderLayer to retrieve.  Note: RenderLayers are sorted via ZOrder.
            /// @return Returns a pointer to the RenderLayer at the specified index.
            RenderLayer* GetLayer(const Whole Index) const;
            /// @brief Gets a RenderLayer in this group by it's ZOrder.
            /// @param ZOrder The ZOrder of the RenderLayer to retrieve.
            /// @return Returns a pointer to the layer at the specified ZOrder, or NULL if no layers exist at that ZOrder.
            RenderLayer* GetLayerByZOrder(const UInt16 ZOrder) const;
            /// @brief Gets the number of RenderLayers assigned to this group.
            /// @return Returns a UInt32 containing the number of RenderLayers in this group.
            UInt32 GetNumRenderLayers() const;
            /// @brief Swaps the layers contained by this group and another group.
            /// @param OtherGroup The other RenderLayerGroup to swap layers with.
            void SwapLayers(RenderLayerGroup* OtherGroup);
            /// @brief Removes a layer from this group.
            /// @param RL The RenderLayer to be removed.
            void RemoveLayer(RenderLayer* RL);
            /// @brief Removes every layer in this group, from this group.
            void RemoveAllLayers();

            /// @brief Gets an iterator to the first RenderLayer.
            /// @return Returns an iterator to the first RenderLayer being stored by this group.
            RenderLayerIterator RenderLayerBegin();
            /// @brief Gets an iterator to one passed the last RenderLayer.
            /// @return Returns an iterator to one passed the last RenderLayer being stored by this group.
            RenderLayerIterator RenderLayerEnd();
            /// @brief Gets a const iterator to the first RenderLayer.
            /// @return Returns a const iterator to the first RenderLayer being stored by this group.
            ConstRenderLayerIterator RenderLayerBegin() const;
            /// @brief Gets an iterator to one passed the last RenderLayer.
            /// @return Returns an iterator to one passed the last RenderLayer being stored by this group.
            ConstRenderLayerIterator RenderLayerEnd() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName();
        };//RenderLayerGroup

        ///////////////////////////////////////////////////////////////////////////////
        /// @class QuadRenderable
        /// @headerfile quadrenderable.h
        /// @brief This represents a nestable quad for an object in a GUI layout.
        /// @details QuadRenderables use "Unified" units for setting their position and size.  Unified units
        /// contain both relative and absolute information for their position on screen or inside their
        /// parent.  It is important to note that the Relative portion of the Unified unit is always
        /// calculated first.  @n @n
        /// The relative portions of the Unified units used to set the dimensions aren't relative to the
        /// screen size, but instead their parents size.  For the most part QuadRenderables are oblivious to
        /// the size of the screen they are in. @n @n
        /// It is important to note that the RenderLayer's stored directly by this class are all the
        /// layers that were created by this class, in creation order.  They aren't not what will be rendered,
        /// nor are they in the order they will be rendered in.  To get either of these bits of information you
        /// must check the active RenderLayerGroup and check the RenderLayers there. @n @n
        /// Also only one RenderLayerGroup may render at a time, but a single RenderLayer can be shared between
        /// as many groups under the same QuadRenderable as you want.
        ///////////////////////////////////////
        class MEZZ_LIB QuadRenderable : public Renderable
        {
        public:
            /// @brief Basic container type for @ref Widget storage by this class.
            typedef std::list<Widget*>                         ChildContainer;
            /// @brief Iterator type for @ref Widget instances stored by this class.
            typedef ChildContainer::iterator                   ChildIterator;
            /// @brief Const Iterator type for @ref Widget instances stored by this class.
            typedef ChildContainer::const_iterator             ConstChildIterator;
            /// @brief Reverse Iterator type for @ref Widget instances stored by this class.
            typedef ChildContainer::reverse_iterator           ReverseChildIterator;
            /// @brief Const Reverse Iterator type for @ref Widget instances stored by this class.
            typedef ChildContainer::const_reverse_iterator     ConstReverseChildIterator;
            /// @brief Basic container type for @ref RenderLayer storage by this class.
            typedef std::vector<RenderLayer*>                  RenderLayerContainer;
            /// @brief Iterator type for @ref RenderLayer instances stored by this class.
            typedef RenderLayerContainer::iterator             RenderLayerIterator;
            /// @brief Const Iterator type for @ref RenderLayer instances stored by this class.
            typedef RenderLayerContainer::const_iterator       ConstRenderLayerIterator;
            /// @brief Basic container type for @ref RenderLayerGroup storage by this class.
            typedef std::map<String,RenderLayerGroup*>         RenderLayerGroupContainer;
            /// @brief Iterator type for @ref RenderLayerGroup instances stored by this class.
            typedef RenderLayerGroupContainer::iterator        RenderLayerGroupIterator;
            /// @brief Const Iterator type for @ref RenderLayerGroup instances stored by this class.
            typedef RenderLayerGroupContainer::const_iterator  ConstRenderLayerGroupIterator;
            /// @brief An std::pair type for storing ZOrders in relation to a named @ref RenderLayerGroup.
            typedef std::pair<UInt16,String>                   GroupOrderEntry;
            /// @brief Container type for @ref GroupOrderEntry storage by this class.
            typedef std::vector<GroupOrderEntry>               GroupOrderEntryVector;
        protected:
            /// @internal
            /// @brief This is a container storing all the @ref RenderLayerGroup instances created by and belonging to this Quad.
            RenderLayerGroupContainer RenderLayerGroups;
            /// @internal
            /// @brief This is a container storing all the @ref RenderLayer instances created by and belonging to this Quad.
            RenderLayerContainer RenderLayers;
            /// @internal
            /// @brief This is a container storing all the children that belong to this Quad.
            ChildContainer ChildWidgets;
            /// @internal
            /// @brief This stores all the information needed to determine the specific behaviors this Quad should have when it's size is being updated.
            SizingInfo SizingPolicy;
            /// @internal
            /// @brief This stores all the information needed to determine the specific behaviors this Quad should have when it's position is being updated.
            PositioningInfo PositioningPolicy;
            /// @internal
            /// @brief The actual (pixel) position and size of this Quad on the screen it belongs to.
            Rect ActDims;
            /// @internal
            /// @brief This is a pointer to the Quad that owns this Quad and is responsible for transform updates applied to this Quad.
            QuadRenderable* ParentQuad;
            /// @internal
            /// @brief This is a pointer to the group of RenderLayers currently being used for rendering.
            RenderLayerGroup* ActiveGroup;
            /// @internal
            /// @brief This is a pointer to the strategy being used by this Quad to determine the positions and sizes of children during transform updates.
            LayoutStrategy* LayoutStrat;
            /// @internal
            /// @brief This is a pointer to the optional cache of vertex's belonging to this Quad and all of it's children.
            ScreenRenderData* VertexCache;
            /// @internal
            /// @brief This is the ZOrder of this Quad in relation to all other Quads in it's parent.
            UInt16 ZOrder;
            /// @internal
            /// @brief Controls whether or not this Quad will be considered for mouse hover checks.
            Boolean MousePassthrough;
            /// @internal
            /// @brief Controls whether or not this Quad and it's children will recieve automatic transform updates.
            Boolean ManualTransformUpdates;
            /// @internal
            /// @brief Determines whether or not this Quad needs all of it's layers refreshed.  Usually after a transform update.
            Boolean AllLayersDirty;

            /// @copydoc Renderable::ProtoSerializeImpl(XML::Node&) const
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeImpl(const XML::Node&)
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
            /// @internal
            /// @brief Overridable method that creates and assigns a layout strategy for a quadrenderable.
            /// @note Widgets by default will create the default LayoutStrategy implementation.  If a widget needs a different implementation (or none/NULL),
            /// then this method needs to be overridden and provide the appropriate type.
            virtual void CreateLayoutStrat();
            /// @internal
            /// @brief Adds all the vertices belonging to all the layers of this renderable to the provided vector.
            /// @param Vertices The vector to store the generated vertices.
            void AppendLayerVertices(std::vector<VertexData>& Vertices);
            /// @internal
            /// @brief Redraws all dirty layers.
            void CleanLayers();
            /// @internal
            /// @brief Resizes the container for RenderLayers in this QuadRenderable.
            /// @param NewSize The new capacity for RenderLayer storage.
            void ResizeLayers(const Whole NewSize);
        //public:
            /// @brief Blank constructor.
            /// @note This is primarily useful for (and used as) a basic constructor suitable for XML deserialization post-construction.
            /// @param Parent The parent screen that created this renderable.
            QuadRenderable(Screen* Parent);
            /// @brief Class constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent screen that created this renderable.
            QuadRenderable(const String& RendName, Screen* Parent);
            /// @brief Parent-less constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this quad's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            QuadRenderable(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            /// @note Any and all children of this quad at the time of it's destruction will be destroyed as well.
            /// If you want to preserve the children for whatever reason, remove them from the quad prior to destroying it.
            virtual ~QuadRenderable();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the currently set ZOrder of this QuadRenderable with it's parent.
            /// @details A QuadRenderable without a parent yet will have the default ZOrder of 0, which is a valid ZOrder when it gains a parent.
            /// In these cases it is recommended to check to see if this QuadRenderable has a parent.
            /// @return Returns a UInt16 representing this renderables ZOrder.
            virtual const UInt16& GetZOrder() const;
            /// @brief Gets the highest ZOrder among the children of this QuadRenderable.
            /// @note This method assumes the children are ordered.  If you alter the ZOrder of children often, ensure you call UpdateChildOrder after
            /// the alterations so this method will work.
            /// @return Returns a UInt16 representing the highest ZOrder of all the children in this QuadRenderable, or 0 if this quad has no children.
            virtual UInt16 GetHighestChildZOrder() const;
            /// @brief Gets the lowest ZOrder among the children of this QuadRenderable.
            /// @note This method assumes the children are ordered.  If you alter the ZOrder of children often, ensure you call UpdateChildOrder after
            /// the alterations so this method will work.
            /// @return Returns a UInt16 representing the lowest ZOrder of all the children in this QuadRenderable, or 0 if this quad has no children.
            virtual UInt16 GetLowestChildZOrder() const;
            /// @brief Updates the order of children in this QuadRenderable based on the ZOrder set on each child.
            virtual void UpdateChildOrder();

            /// @brief Checks to see if another Quad is overlapping with this one.
            /// @param Quad The other Quad to check for overlap.
            /// @return Returns true if this quad overlaps with the provided quad, false otherwise.
            virtual Boolean CheckOverlap(const QuadRenderable* Quad) const;
            /// @brief Checks to see if a point in 2D space is inside this quad.
            /// @param Point The point in 2D space to check.
            /// @return Returns true if the provided point is within this quad, false otherwise.
            virtual Boolean IsInside(const Vector2& Point) const;
            /// @brief Gets whether or not this QuadRenderable is a direct child of it's screen.
            /// @return Returns true if the screen is this QuadRenderable's parent, false otherwise.
            virtual Boolean IsChildOfScreen() const;

            /// @brief Populates all text lines in all text layers owned by this quad.
            /// @note Populating text lines can be a slow process, use with care.
            /// @param MaxWidth The maximum line width to assume for all text lines that are to be populated.
            virtual void PopulateTextLinesInLayers(const Real MaxWidth);
            /// @brief Gets the height needed for this quadrenderable to be able to completely display text in it's child text layers.
            /// @return Returns a Real representing the pixel height this quad needs to be to display it's largest batch of text among it's text layers.
            virtual Real GetIdealHeightForText() const;

            /// @brief Updates the dimensions of this QuadRenderable based on the transform of it's parent.
            /// @details This is a convenience function that will call the more descriptive version of "UpdateDimensions" on this objects parent using its
            /// existing dimensions as both parameters, causing all of its children to be updated (including this).  This method can be expensive based on
            /// the number of siblings this quad has.
            virtual void UpdateDimensions();
            /// @brief Updates the dimensions of the children in this QuadRenderable.
            /// @details This is a convenience function that will call the more descriptive version of "UpdateDimensions" on this object using its existing
            /// dimensions as both parameters, causing all of this quads children to be updated.  This method can be expensive based on
            /// the number of children this quad has.
            virtual void UpdateChildDimensions();
            /// @brief Updates the dimensions of this QuadRenderable based on the transform of it's parent.
            /// @details This function is called automatically by this objects parent when it changes in size or position, and should only need to be called
            /// manually if the positioning or sizing rules were updated manually.
            /// @param OldSelfRect The previous dimensions of this quad prior to it's dimensions being updated.
            /// @param NewSelfRect The updated and current dimensions of this quad.
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);

            /// @brief Sets whether or not this quad should be skipped when determining if the mouse is hovered over this quad.
            /// @note This does not affect the ability for the children of this quad to be detected and/or checked.
            /// @param Enable True if you want to have this quad be skipped for mouse hover checks, false if you want this quad checked.
            virtual void SetMousePassthrough(Boolean Enable);
            /// @brief Gets whether or not Mouse Passthrough is enabled.
            /// @return Returns true if Mouse Passthrough is enabled, false otherwise.
            virtual Boolean GetMousePassthrough() const;
            /// @brief Sets whether or not this quad has specific behaviors for it's transform updates and they should not be done automatically.
            /// @note Setting this to true will prevent child quads from being updated.  This setting does not mean that a quad will never be
            /// updated, just that it has special logic for doing so located elsewhere from the normal logic.  Example: Transform updates to be
            /// applied to the scroller of a scrollbar.
            /// @param Enable True to make this quad skip automatic updating and intent to provide that logic explicitly elsewhere, false for automatic transform updates.
            virtual void SetManualTransformUpdates(Boolean Enable);
            /// @brief Gets whether or not this quad will be automatically updated when parent transforms are updated.
            /// @return Returns true if this quad needs manual updating, false if it recieves automatic updates.
            virtual Boolean GetManualTransformUpdates() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Policy Methods

            /// @brief Sets the behavior to be used when this QuadRenderable is positioned.
            /// @note This method is not retroactive.  Setting this to a different value will not cause it to reposition.
            /// @param Policy The @ref PositioningInfo to be applied to this QuadRenderable.
            virtual void SetPositioningPolicy(const PositioningInfo& Policy);
            /// @brief Gets the current behavior this QuadRenderable will use when it is positioned.
            /// @return Returns a const @ref PositioningInfo reference describing how this QuadRenderable is positioned.
            virtual const PositioningInfo& GetPositioningPolicy() const;
            /// @brief Sets the behavior to be used when this QuadRenderable is sized.
            /// @note This method is not retroactive.  Setting this to a different value will not cause it to resize.
            /// @param Policy The @ref SizingInfo to be applied to this QuadRenderable.
            virtual void SetSizingPolicy(const SizingInfo& Policy);
            /// @brief Gets the current behavior this QuadRenderable will use when it is sized.
            /// @return Returns a const @ref SizingInfo reference describing how this QuadRenderable is sized.
            virtual const SizingInfo& GetSizingPolicy() const;

            /// @brief Sets the behavior this quad will have when it is positioned automatically.
            /// @note This method is not retroactive.  Setting this to a different value will not cause it to reposition.
            /// @param Rules The action to take when this is positioned.  See @ref PositioningFlags enum for more info.
            virtual void SetPositioningRules(const Whole Rules);
            /// @brief Gets the current behavior this quad will follow when it is positioned automatically.
            /// @return Returns a bitfield of @ref PositioningFlags reprensenting the action this quad will take when it is positioned.
            virtual Whole GetPositioningRules() const;
            /// @brief Sets the behavior this quad will have on the X axis when it is resized.
            /// @note This method is not retroactive.  Setting this to a different value will not cause it to resize.
            /// @param Rules The action to take when this is resized.  See @ref SizingRules enum for more info.
            virtual void SetHorizontalSizingRules(const Whole Rules);
            /// @brief Gets the current behavior this quad will follow for the X axis when it is resized.
            /// @return Returns a whole reprensenting the action this quad will take when it is resized.  See @ref SizingRules enum for more info.
            virtual Whole GetHorizontalSizingRules() const;
            /// @brief Sets the behavior this quad will have on the Y axis when it is resized.
            /// @note This method is not retroactive.  Setting this to a different value will not cause it to resize.
            /// @param Rules The action to take when this is resized.  See @ref SizingRules enum for more info.
            virtual void SetVerticalSizingRules(const Whole Rules);
            /// @brief Gets the current behavior this quad will follow for the Y axis when it is resized.
            /// @return Returns a whole reprensenting the action this quad will take when it is resized.  See @ref SizingRules enum for more info.
            virtual Whole GetVerticalSizingRules() const;

            /// @brief Sets the minimum size this quad is allowed to have.
            /// @note This function expects the provided UnifiedVec2 to be in pixels.
            /// @param Min The size this quad is not allowed to go under.
            virtual void SetMinSize(const UnifiedVec2& Min);
            /// @brief Gets the currently set minimum size for this quad.
            /// @return Returns a vector2 containing the minimum size for this quad in pixels.
            virtual UnifiedVec2 GetMinSize() const;
            /// @brief Sets the maximum size this quad is allowed to have.
            /// @note This function expects the provided UnifiedVec2 to be in pixels.
            /// @param Max The size this quad is not allowed to exceed.
            virtual void SetMaxSize(const UnifiedVec2& Max);
            /// @brief Gets the currently set maximum size for this quad.
            /// @return Returns a vector2 containing the maximum size for this quad in pixels.
            virtual UnifiedVec2 GetMaxSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // RenderLayer Management

            /// @brief Creates an ImageLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered.
            /// @return Returns a pointer to the created ImageLayer.
            ImageLayer* CreateImageLayer();
            /// @brief Creates an ImageLayer for this renderable and adds it to a RenderLayerGroup.
            /// @note If the requested group does not exist it will be created.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created ImageLayer should be added to.
            /// @return Returns a pointer to the created ImageLayer.
            ImageLayer* CreateImageLayer(const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates an ImageLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created ImageLayer.
            ImageLayer* CreateImageLayer(const GroupOrderEntryVector& Entrys);

            /// @brief Creats a SingleLineTextLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer();
            /// @brief Creats a SingleLineTextLayer for this renderable.
            /// @note If the requested group does not exist it will be created.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created TextLayer should be added to.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates a SingleLineTextLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const GroupOrderEntryVector& Entrys);
            /// @brief Creats a SingleLineTextLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered.
            /// @param FontName The name of the font to use when rendering characters on the created layer.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const String& FontName);
            /// @brief Creats a SingleLineTextLayer for this renderable.
            /// @note If the requested group does not exist it will be created.
            /// @param FontName The name of the font to use when rendering characters on the created layer.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created TextLayer should be added to.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const String& FontName, const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates a SingleLineTextLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created.
            /// @param FontName The name of the font to use when rendering characters on the created layer.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const String& FontName, const GroupOrderEntryVector& Entrys);
            /// @brief Creats a SingleLineTextLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered. @n @n
            /// This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const Real& LineHeight);
            /// @brief Creats a SingleLineTextLayer for this renderable.
            /// @note If the requested group does not exist it will be created. @n @n
            /// This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created TextLayer should be added to.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const Real& LineHeight, const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates a SingleLineTextLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created. @n @n
            /// This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created layer.
            SingleLineTextLayer* CreateSingleLineTextLayer(const Real& LineHeight, const GroupOrderEntryVector& Entrys);

            /// @brief Creats a MultiLineTextLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer();
            /// @brief Creats a MultiLineTextLayer for this renderable.
            /// @note If the requested group does not exist it will be created.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created TextLayer should be added to.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates a MultiLineTextLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const GroupOrderEntryVector& Entrys);
            /// @brief Creats a MultiLineTextLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered.
            /// @param FontName The name of the font to use when rendering characters on the created layer.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const String& FontName);
            /// @brief Creats a MultiLineTextLayer for this renderable.
            /// @note If the requested group does not exist it will be created.
            /// @param FontName The name of the font to use when rendering characters on the created layer.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created TextLayer should be added to.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const String& FontName, const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates a MultiLineTextLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created.
            /// @param FontName The name of the font to use when rendering characters on the created layer.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const String& FontName, const GroupOrderEntryVector& Entrys);
            /// @brief Creats a MultiLineTextLayer for this renderable.
            /// @note This will not add the created layer to any group, thus it must be added manually to be rendered. @n @n
            /// This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const Real& LineHeight);
            /// @brief Creats a MultiLineTextLayer for this renderable.
            /// @note If the requested group does not exist it will be created. @n @n
            /// This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created TextLayer should be added to.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const Real& LineHeight, const UInt16 ZOrder, const String& GroupName);
            /// @brief Creates a MultiLineTextLayer for this renderable and adds it to all the specified RenderLayerGroups at the provided ZOrders.
            /// @note If the requested groups do not exist they will be created. @n @n
            /// This constructor defaults to Screen Relative text, but this can be altered after construction.
            /// @param LineHeight The relative scalar to be used when determining the size of characters generated by this text layer.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the created layer should be added to.
            /// @return Returns a pointer to the created layer.
            MultiLineTextLayer* CreateMultiLineTextLayer(const Real& LineHeight, const GroupOrderEntryVector& Entrys);

            /// @brief Gets a RenderLayer belonging to this QuadRenderable by index.
            /// @param Index The index of the RenderLayer to retrieve.
            /// @return Returns a pointer to the RenderLayer at the specified index.
            RenderLayer* GetRenderLayer(const UInt32& Index) const;
            /// @brief Gets the number of RenderLayers created for this renderable.
            /// @return Returns a UInt32 containing the number of RenderLayers in this renderable.
            UInt32 GetNumRenderLayers() const;
            /// @brief Gets the number of RenderLayers that are visible in this renderable.
            /// @return Returns a UInt32 containing the number of RenderLayers that would be rendered if this renderable is visible.
            UInt32 GetNumVisibleRenderLayers() const;
            /// @brief Destroys a RenderLayer being stored by this renderable.
            /// @param ToBeDestroyed The RenderLayer to be destroyed.
            void DestroyRenderLayer(RenderLayer* ToBeDestroyed);
            /// @brief Destroys all RenderLayers being stored by this renderable.
            void DestroyAllRenderLayers();

            /// @brief Gets an iterator to the first RenderLayer.
            /// @return Returns an iterator to the first RenderLayer being stored by this renderable.
            RenderLayerIterator RenderLayerBegin();
            /// @brief Gets an iterator to one passed the last RenderLayer.
            /// @return Returns an iterator to one passed the last RenderLayer being stored by this renderable.
            RenderLayerIterator RenderLayerEnd();
            /// @brief Gets a const iterator to the first RenderLayer.
            /// @return Returns a const iterator to the first RenderLayer being stored by this renderable.
            ConstRenderLayerIterator RenderLayerBegin() const;
            /// @brief Gets an iterator to one passed the last RenderLayer.
            /// @return Returns an iterator to one passed the last RenderLayer being stored by this renderable.
            ConstRenderLayerIterator RenderLayerEnd() const;

            ///////////////////////////////////////////////////////////////////////////////
            // RenderLayerGroup Management

            /// @brief Sets the RenderLayerGroup that will be used to render this renderable.
            /// @param Name The name of the RenderLayerGroup that will be used.
            void SetActiveGroup(const String& Name);
            /// @brief Sets the RenderLayerGroup that will be used to render this renderable.
            /// @param Group Pointer to the RenderLayerGroup that will be used.
            void SetActiveGroup(RenderLayerGroup* Group);
            /// @brief Gets the current RenderLayerGroup used for rendering.
            /// @return Returns a pointer to the group currently being used to render this renderable.
            RenderLayerGroup* GetActiveGroup() const;
            /// @brief Checks to see if a RenderLayerGroup exists.
            /// @param Name The name of the RenderLayerGroup to check for.
            /// @return Returns true if the named RenderLayerGroup exists in this renderable.
            Boolean RenderLayerGroupExists(const String& Name) const;
            /// @brief Gets the number of RenderLayerGroup's created for this renderable.
            /// @return Returns a UInt32 containing the number of RenderLayerGroups in this renderable.
            UInt32 GetNumRenderLayerGroups() const;
            /// @brief Adds a RenderLayer to the specified group.
            /// @note If the requested group does not exist it will be created.
            /// @param Layer The RenderLayer to be added to the named group.
            /// @param ZOrder The ZOrder that will be given to this layer to determine the order it is rendered with other layers.
            /// @param GroupName The name of the group the created ImageLayer should be added to.
            void AddLayerToGroup(RenderLayer* Layer, const UInt16 ZOrder, const String& GroupName);
            /// @brief Adds a RenderLayer to multiple groups.
            /// @note If the requested groups do not exist they will be created.
            /// @param Layer The RenderLayer to be added to the named groups.
            /// @param Entrys A vector of std::pair's that contain the ZOrders and the names of the groups the provided layer should be added to.
            void AddLayerToGroups(RenderLayer* Layer, const GroupOrderEntryVector& Entrys);
            /// @brief Removes a single RenderLayer from a specified RenderLayerGroup.
            /// @param Layer The RenderLayer to be removed from the named group.
            /// @param GroupName The name of the RenderLayerGroup the provided layer will be removed from.
            void RemoveLayerFromGroup(RenderLayer* Layer, const String& GroupName);
            /// @brief Removes a single RenderLayer from all RenderLayerGroups owned by this QuadRenderable.
            /// @param Layer The RenderLayer to be removed from all groups.
            void RemoveLayerFromAllGroups(RenderLayer* Layer);

            /// @brief Creates a new RenderLayerGroup that can have.
            /// @exception This function will throw an exception if a group already exists with the provided name.
            /// @remarks The first RenderLayerGroup that is created will automatically be set to the ActiveGroup.  You can override
            /// this by calling "SetActiveGroup" manually after creating any additional RenderLayerGroup.
            /// @param Name The name to be given to the new RenderLayerGroup.
            /// @return Returns a pointer to the created RenderLayerGroup.
            RenderLayerGroup* CreateRenderLayerGroup(const String& Name);
            /// @brief Gets the named RenderLayerGroup or creates one with the specified name if it does not exist.
            /// @remarks This function is unlike the "create" function in that it won't throw an exception in an error condition,
            /// and it is unlike the "get" function in that it will not return a NULL pointer.  This function guarentee's returning
            /// a valid RenderLayerGroup with the given name. @n @n
            /// The first RenderLayerGroup that is created will automatically be set to the ActiveGroup.  You can override
            /// this by calling "SetActiveGroup" manually after creating any additional RenderLayerGroup.
            /// @param Name The name of the RenderLayerGroup to get or create.
            /// @return Returns a pointer to the named RenderLayerGroup.
            RenderLayerGroup* CreateOrRetrieveRenderLayerGroup(const String& Name);
            /// @brief Gets a RenderLayerGroup by name.
            /// @param Name The name of the RenderLayerGroup to retrieve.
            /// @return Returns a pointer to the named RenderLayerGroup, or NULL if it does not exist.
            RenderLayerGroup* GetRenderLayerGroup(const String& Name) const;
            /// @brief Destroy's a RenderLayerGroup by name.
            /// @param Name The name of the RenderLayerGroup to destroy.
            void DestroyRenderLayerGroup(const String& Name);
            /// @brief Destroy's a RenderLayerGroup by pointer.
            /// @param ToBeDestroyed The RenderLayerGroup to be destroyed.
            void DestroyRenderLayerGroup(RenderLayerGroup* ToBeDestroyed);
            /// @brief Destroy's all RenderLayerGroups being stored/managed by this QuadRenderable.
            void DestroyAllRenderLayerGroups();

            /// @brief Gets an iterator to the first RenderLayerGroup.
            /// @return Returns an iterator to the first RenderLayerGroup being stored by this renderable.
            RenderLayerGroupIterator RenderLayerGroupBegin();
            /// @brief Gets an iterator to one passed the last RenderLayerGroup.
            /// @return Returns an iterator to one passed the last RenderLayerGroup being stored by this renderable.
            RenderLayerGroupIterator RenderLayerGroupEnd();
            /// @brief Gets a const iterator to the first RenderLayerGroup.
            /// @return Returns a const iterator to the first RenderLayerGroup being stored by this renderable.
            ConstRenderLayerGroupIterator RenderLayerGroupBegin() const;
            /// @brief Gets an iterator to one passed the last RenderLayerGroup.
            /// @return Returns an iterator to one passed the last RenderLayerGroup being stored by this renderable.
            ConstRenderLayerGroupIterator RenderLayerGroupEnd() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            /// @brief Adds a Widget to this as a child of this quad.
            /// @warning Unless this method is being called from a @ref Screen or @ref LayoutContainer (or it's derived
            /// classes) this method is not intended for casual use.  Different QuadRenderable implementations have
            /// different expectations on their children and may need a specific quantity, or have them inserted in a
            /// specific order to achieve the behavior designed for them.  Use with care.
            /// @param Child A pointer to the Widget that is to become a child of this.
            virtual void AddChild(Widget* Child);
            /// @brief Adds a Widget to this as a child of this quad.
            /// @warning Unless this method is being called from a @ref Screen or @ref LayoutContainer (or it's derived
            /// classes) this method is not intended for casual use.  Different QuadRenderable implementations have
            /// different expectations on their children and may need a specific quantity, or have them inserted in a
            /// specific order to achieve the behavior designed for them.  Use with care.
            /// @param Child A pointer to the Widget that is to become a child of this.
            /// @param ZOrder The ZOrder to assign to the child renderable.
            virtual void AddChild(Widget* Child, const UInt16 ZOrder);
            /// @brief Gets a child by it's ZOrder.
            /// @note ZOrders do not have to be unique.  In the case of multiple identical ZOrders the first one found will be returned.
            /// @param Zorder The ZOrder on the child to search for.
            /// @return Returns a pointer to the requested child.
            virtual Widget* GetChild(const UInt16 Zorder) const;
            /// @brief Gets a child by it's name.
            /// @param RendName The name of the child to search for.
            /// @return Returns a pointer to the requested child.
            virtual Widget* GetChild(const String& RendName) const;
            /// @brief Gets the number of children in this QuadRenderable.
            /// @return Returns the number of children directly connected to this QuadRenderable.
            virtual Whole GetNumChildren() const;
            /// @brief Removes a child Widget from this quadrenderable.
            /// @note The child is NOT destroyed.
            /// @param ToBeRemoved A pointer to the child to be removed.
            virtual void RemoveChild(Widget* ToBeRemoved);
            /// @brief Removes all child Widgets from this QuadRenderable.
            /// @note The chilren are NOT destroyed.
            virtual void RemoveAllChildren();
            /// @brief Destroys a child Widget currently inside this QuadRenderable.
            /// @param ToBeDestroyed A pointer to the child to be destroyed.
            virtual void DestroyChild(Widget* ToBeDestroyed);
            /// @brief Destroys all child Widgets currently inside this QuadRenderable.
            virtual void DestroyAllChildren();

            /// @brief Gets an iterator to the first child Widget.
            /// @return Returns an iterator to the first Widget being stored by this QuadRenderable.
            ChildIterator ChildrenBegin();
            /// @brief Gets an iterator to one passed the last child Widget.
            /// @return Returns an iterator to one passed the last Widget being stored by this QuadRenderable.
            ChildIterator ChildrenEnd();
            /// @brief Gets a const iterator to the first child Widget.
            /// @return Returns a const iterator to the first Widget being stored by this QuadRenderable.
            ConstChildIterator ChildrenBegin() const;
            /// @brief Gets an iterator to one passed the last child Widget.
            /// @return Returns a const iterator to one passed the last Widget being stored by this QuadRenderable.
            ConstChildIterator ChildrenEnd() const;

            /// @brief Gets an iterator to the last Widget.
            /// @param Returns an iterator to the last Widget being stored by this QuadRenderable.
            ReverseChildIterator RChildrenBegin();
            /// @brief Gets an iterator to one before the first child Widget.
            /// @param Returns an iterator to one before the first child Widget being stored by this QuadRenderable.
            ReverseChildIterator RChildrenEnd();
            /// @brief Gets a const iterator to the last Widget.
            /// @param Returns a const iterator to the last Widget being stored by this QuadRenderable.
            ConstReverseChildIterator RChildrenBegin() const;
            /// @brief Gets a const iterator to one before the first child Widget.
            /// @param Returns a const iterator to one before the first child Widget being stored by this QuadRenderable.
            ConstReverseChildIterator RChildrenEnd() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @brief Sets the position this QuadRenderable will have within it's parent
            /// @param Position The top-left position relative to it's parents' rect.
            virtual void SetUnifiedPosition(const UnifiedVec2& Position);
            /// @brief Sets the size this QuadRenderable will have within it's parent.
            /// @param Size The width and height relative to it's parents' rect.
            virtual void SetUnifiedSize(const UnifiedVec2& Size);
            /// @brief Gets the position of this QuadRenderable as a Unified Vector2.
            /// @return Returns a UnifiedVec2 representing the relative position and pixel offset of this QuadRenderable.
            virtual UnifiedVec2 GetUnifiedPosition() const;
            /// @brief Gets the size of this QuadRenderable as a Unified Vector2.
            /// @return Returns a UnifiedVec2 representing the relative size and pixel offset of this QuadRenderable.
            virtual UnifiedVec2 GetUnifiedSize() const;
            /// @brief Gets the Unified positiona nd size of this QuadRenderable as a unified rect.
            /// @return Returns a UnifiedRect containing the position and size of this QuadRenderable.
            virtual UnifiedRect GetUnifiedRect() const;
            /// @brief Gets the pixel position of this widget.
            /// @return Returns a vector2 representing the pixel position of this widget.
            virtual Vector2 GetActualPosition() const;
            /// @brief Gets the pixel size of this widget.
            /// @return Returns a vector2 representing the pixel size of this widget.
            virtual Vector2 GetActualSize() const;
            /// @brief Gets this QuadRenderables' Rect.
            /// @note This function will always return a non-relative rect(units in pixels).
            /// @return Returns a Rect containing this QuadRenderables' Position and Size.
            virtual Rect GetRect() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets the parent of this quad.
            /// @return Returns a pointer to the parent object of this quad, or NULL if it's the root object.
            QuadRenderable* GetParent() const;
            /// @brief Gets the QuadRenderable after this one among the QuadRenderables owned by it's parent.
            /// @param Wrap Whether or not you want to return the first QuadRenderable owned by this Quad's parent if this Quad is last.
            /// @return Returns a pointer to the next QuadRenderable (by sort order).
            QuadRenderable* GetNextSibling(Boolean Wrap = true);
            /// @brief Gets the QuadRenderable before this one among the QuadRenderables owned by it's parent.
            /// @param Wrap Whether or not you want to return the last QuadRenderable owned by this Quad's parent if this Quad is first.
            /// @return Returns a pointer to the previous QuadRenderable (by sort order).
            QuadRenderable* GetPrevSibling(Boolean Wrap = true);
            /// @brief Gets the QuadRenderable that is both an ancestor of this quad, and a direct child of the screen.
            /// @note Since the screen is pretty much always the root and you can get the screen through other methods,
            /// that isn't factored in when searching.  If this quad is a direct child of the screen, this will be returned.
            /// @return Returns a pointer to the QuadRenderable that is both an ancestor of this quad, and a direct child of the screen.
            virtual QuadRenderable* GetTopMostQuad();

            ///////////////////////////////////////////////////////////////////////////////
            // VertexCaching Methods

            /// @brief Enables or disables caching of vertex's belonging to this and all child renderables.
            /// @remarks If enabled, this will create a local instance of ScreenRenderData.  Whenever this or
            /// any of this Quad's children are updated the cache will be flushed and regenerated.  When a request
            /// to append vertices is made, they will be appended from the local cache.  If needed the cache will
            /// be regenerated immediately. @n @n
            /// This will increase the memory consumed, but in some cases can dramatically increase the performance
            /// of redrawing a screen when only a small portion of the screen is changed.  By default all direct
            /// children of the screen have this enabled. @n @n
            /// It is also important to note that this cache is only used when the "_AppendVerticesCascading" method
            /// is called.
            /// @param Enable Whether to enable or disable this feature.
            void SetLocalVertexCaching(Boolean Enable);
            /// @brief Gets whether or not vertex caching is enabled for this Quad.
            /// @return Returns true if this Quad caches the vertex's of it and it's children, false otherwise.
            Boolean IsVertexCachingEnabled() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the RenderLayers of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeRenderLayers(XML::Node& SelfRoot) const;
            /// @brief Convert the RenderLayerGroups of this class to an XML::Node ready for seriailization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeRenderLayerGroups(XML::Node& SelfRoot) const;
            /// @brief Convert the child quads of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeChildQuads(XML::Node& SelfRoot) const;

            /// @copydoc Renderable::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the RenderLayers of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeRenderLayers(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the RenderLayerGroups of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeRenderLayerGroups(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the ChildQuads of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeChildQuads(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Ssts the ZOrder value for this renderable.
            /// @warning Under no circumstances would any user need to call this method themselves.  Doing so can damage how things are rendered.
            /// @param Zorder The ZOrder this renderable has among the renderables belonging to it's parent.
            virtual void _SetZOrder(const UInt16& Zorder);
            /// @internal
            /// @brief Notifies this QuadRenderable that it has been added to another QuadRenderable.
            /// @param NewParent A pointer to the QuadRenderable this is becoming the child of.
            virtual void _NotifyParenthood(QuadRenderable* NewParent);

            /// @copydoc Renderable::_MarkDirty()
            virtual void _MarkDirty();
            /// @internal
            /// @brief Tells this QuadRenderable to mark each of it's children (and their children) as dirty.
            /// @note This will also mark itself as dirty in the process.
            virtual void _MarkAllChildrenDirty();
            /// @internal
            /// @brief Tells this QuadRenderable that all of it's layers are dirty.
            virtual void _MarkAllLayersDirty();

            /// @copydoc Renderable::_AppendRenderData(ScreenRenderData& RenderData)
            virtual void _AppendRenderData(ScreenRenderData& RenderData);
            /// @internal
            /// @brief Appends the vertices of this renderable to another vector, and then does the same for this renderable's children.
            /// @param RenderData The vector of vertex's to append to.
            virtual void _AppendRenderDataCascading(ScreenRenderData& RenderData);
            /// @internal
            /// @brief Checks if there are available render data from this QuadRenderable (or it's subrenderables).
            /// @return Returns true if this QuadRenderable has render data that can be appended, false otherwise.
            virtual Boolean _HasAvailableRenderData() const;
        };//QuadRenderable
    }//UI
}//Mezzanine

#endif
