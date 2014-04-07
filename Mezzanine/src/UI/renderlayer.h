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

#ifndef _uirenderlayer_h
#define _uirenderlayer_h

#include "UI/quadrenderer.h"
#include "UI/rect.h"
#include "UI/uienumerations.h"

namespace Mezzanine
{
    namespace UI
    {
        class QuadRenderable;
        class Screen;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderLayer
        /// @headerfile renderlayer.h
        /// @brief This is the base class for the types of layers that can be added to a renderable.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RenderLayer : public QuadRenderer
        {
        public:
        protected:
            /// @internal
            /// @brief The scaling applied to this RenderLayer.
            Vector2 Scale;
            /// @internal
            /// @brief A pointer to the parent of this RenderLayer.
            QuadRenderable* Parent;
            /// @internal
            /// @brief Stores the index of this RenderLayer in it's parent QuadRenderable.
            Whole IndexID;
            /// @internal
            /// @brief The amount of rotation to be applied to this RenderLayer in radians.
            Real RotAngle;

            /// @internal
            /// @brief Applies rotation to a point in 2D space.
            /// @param Pointer A point in 2D space that will be rotated around another point.
            /// @param RotationCenter The point that will be rotated around.
            virtual void RotationTransform(Vector2& Point, const Vector2& RotationCenter);
            /// @internal
            /// @brief Applies rotation to a quad in 2D space.
            /// @note This overload will use the center of the quad defined by the 4 points as the point to be rotated around.
            /// @param TopLeft The top left corner of the quad to be rotated.
            /// @param TopRight The top right corner of the quad to be rotated.
            /// @param BottomLeft The bottom left corner of the quad to be rotated.
            /// @param BottomRight The bottom right corner of the quad to be rotated.
            virtual void RotationTransform(Vector2& TopLeft, Vector2& TopRight, Vector2& BottomLeft, Vector2& BottomRight);
            /// @internal
            /// @brief Applies rotation to a quad in 2D space.
            /// @param TopLeft The top left corner of the quad to be rotated.
            /// @param TopRight The top right corner of the quad to be rotated.
            /// @param BottomLeft The bottom left corner of the quad to be rotated.
            /// @param BottomRight The bottom right corner of the quad to be rotated.
            /// @param RotationCenter The point that will be rotated around.
            virtual void RotationTransform(Vector2& TopLeft, Vector2& TopRight, Vector2& BottomLeft, Vector2& BottomRight, const Vector2& RotationCenter);
            /// @internal
            /// @brief Applies rotation to a quad in 2D space.
            /// @note This overload will use the center of the quad defined by the 4 points as the point to be rotated around.
            /// @param RotRect An array of 4 Vector2s storing the quad corners to be rotated.
            virtual void RotationTransform(Vector2* RotRect);
            /// @internal
            /// @brief Applies rotation to a quad in 2D space.
            /// @param RotRect An array of 4 Vector2s storing the quad corners to be rotated.
            /// @param RotationCenter The point that will be rotated around.
            virtual void RotationTransform(Vector2* RotRect, const Vector2& RotationCenter);
        //public:
            /// @brief Class constructor.
            /// @param ParentRenderable The renderable that created this layer.
            RenderLayer(QuadRenderable* ParentRenderable);
        public:
            /// @brief Class destructor.
            virtual ~RenderLayer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the type of render layer this is.
            /// @return Returns a RenderLayerType describing the type of layer this is.
            virtual UI::RenderLayerType GetLayerType() const = 0;
            /// @brief Gets the index position of this RenderLayer in it's parent.
            /// @return Returns a Whole representing the index position of this RenderLayer in the parent QuadRenderable.
            virtual Whole GetIndex() const;
            /// @brief Notifies this RenderLayer that the group this layer belongs to has become the active group.
            /// @remarks This shouldn't need to ever be called manually and is automatically called when
            /// a QuadRenderable sets the parent group as the active RenderLayerGroup.  This method exists to reset the
            /// state of a layer (or group of layers) if necessary to achieve the visual effect desired.
            virtual void NotifyActive();
            /// @brief Notifies this RenderLayer that the group this layer belongs to is no longer the active group.
            /// @remarks This shouldn't need to ever be called manually and is automatically called when
            /// a QuadRenderable removes the parent group as the active RenderLayerGroup.  This method exists to reset the
            /// state of a layer (or group of layers) if necessary to achieve the visual effect desired.
            virtual void NotifyInactive();
            /// @brief Gets a rect containing the actual position and size of this layer.
            /// @note This Rect does have scaling applied, but not any set rotation of the layer.
            /// @return Returns a Rect containing the area of this layer in pixels.
            virtual Rect GetAreaRect() const;
            /// @brief Sets the scaling to be applied to this render layer.
            /// @param Scaling A Vector2 representing the X and Y scaling to be applied to this render layer.
            virtual void SetScale(const Vector2& Scaling);
            /// @brief Gets the scaling currently applied to this render layer.
            /// @return Returns a Vector2 containing the X and Y scaling applied to this render layer.
            virtual Vector2 GetScale() const;

            /// @brief Gets whether or not this is an LineLayer.
            /// @return Returns true if this is an LineLayer, false otherwise.
            virtual Boole IsLineLayer() const;
            /// @brief Gets whether or not this is an ImageLayer.
            /// @return Returns true if this is an ImageLayer, false otherwise.
            virtual Boole IsImageLayer() const;
            /// @brief Gets whether or not this is a SingleImageLayer.
            /// @return Returns true if this is a SingleImageLayer, false otherwise.
            virtual Boole IsSingleImageLayer() const;
            /// @brief Gets whether or not this is a MultiImageLayer.
            /// @return Returns true if this is a MultiImageLayer, false otherwise.
            virtual Boole IsMultiImageLayer() const;
            /// @brief Gets whether or not this is a TextLayer.
            /// @return Returns true if this is a TextLayer, false otherwise.
            virtual Boole IsTextLayer() const;
            /// @brief Gets whether or not this is a SingleLineTextLayer.
            /// @return Returns true if this is a SingleLineTextLayer, false otherwise.
            virtual Boole IsSingleLineTextLayer() const;
            /// @brief Gets whether or not this is a MultiLineTextLayer.
            /// @return Returns true if this is a MultiLineTextLayer, false otherwise.
            virtual Boole IsMultiLineTextLayer() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Rotation Methods

            /// @brief Sets the rotation to be applied to this renderable.
            /// @param Degrees The angle of rotation in degrees.
            virtual void SetRotationDegrees(const Real& Degrees);
            /// @brief Sets the rotation to be applied to this renderable.
            /// @param Radians The angle of rotation in radians.
            virtual void SetRotationRadians(const Real& Radians);
            /// @brief Gets the current rotation applied to this renderable in degrees.
            /// @return Returns a Real with the current rotation.
            virtual Real GetRotationDegrees() const;
            /// @brief Gets the current rotation applied to this renderable in radians.
            /// @return Returns a Real with the current rotation.
            virtual Real GetRotationRadians() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Accessor Methods

            /// @brief Gets the parent of this render layer.
            /// @return Returns a pointer to the parent of this render layer.
            virtual QuadRenderable* GetParent() const;
            /// @brief Gets the screen the parent renderable is being rendered on.
            /// @return Returns a pointer to the screen this layer belongs to.
            virtual Screen* GetScreen() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc SimpleRenderer::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc SimpleRenderer::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc SimpleRenderer::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc SimpleRenderer::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc SimpleRenderer::_MarkDirty()
            virtual void _MarkDirty();
            /// @internal
            /// @brief Notifies this RenderLayer that it's index in the parent QuadRenderable has been updated.
            /// @param Index The new index being given to this RenderLayer.
            virtual void _UpdateIndex(const Whole Index);
        };//RenderLayer
    }//UI
}//Mezzanine

#endif
