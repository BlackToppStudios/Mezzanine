// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _uirenderlayergroup_h
#define _uirenderlayergroup_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace UI
    {
        class RenderLayer;
        class QuadRenderable;
        //////////////////////////////////////////////////////////////////////////////
        /// @brief This class stores a group of render layers that can be set to be rendered.
        /// @details A QuadRenderable can only render one group of layers at a time, but a single layer can be added to
        /// as many RenderLayerGroup's as the user see's fit.
        ///////////////////////////////////////
        class MEZZ_LIB RenderLayerGroup
        {
        public:
            /// @brief An std::pair type for storing ZOrders in relation to @ref RenderLayer instances.
            typedef std::pair<UInt16,RenderLayer*>          RenderLayerPair;
            /// @brief Basic container type for @ref RenderLayerPair storage by this class.
            typedef std::vector<RenderLayerPair>            RenderLayerContainer;
            /// @brief Iterator type for @ref RenderLayerPair instances stored by this class.
            typedef RenderLayerContainer::iterator          RenderLayerIterator;
            /// @brief Const Iterator type for @ref RenderLayerPair instances stored by this class.
            typedef RenderLayerContainer::const_iterator    ConstRenderLayerIterator;
        protected:
            /// @internal
            /// @brief Container storing all the layers that belong to this group and their ZOrders.
            RenderLayerContainer RenderLayers;
            /// @internal
            /// @brief A pointer to the host QuadRenderable.
            QuadRenderable* ParentQuad;
            /// @internal
            /// @brief The ID of this group.
            UInt16 GroupID;
        public:
            /// @brief Class constructor.
            /// @param ID The unique ID to give to this RenderLayerGroup.
            /// @param Creator The Quad that owns this RenderLayerGroup.
            RenderLayerGroup(const UInt16 ID, QuadRenderable* Creator);
            /// @brief Class destructor.
            ~RenderLayerGroup();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the ID of this RenderLayerGroup.
            /// @return Returns a UInt16 representing the ID of this RenderLayerGroup.
            UInt16 GetGroupID() const;
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
    }//UI
}//Mezzanine

#endif
