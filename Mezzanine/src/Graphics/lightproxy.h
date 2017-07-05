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
#ifndef _graphicslightproxy_h
#define _graphicslightproxy_h

/// @file
/// @brief This file contains the declaration for the World proxy wrapping light functionality.

#include "Graphics/renderableproxy.h"
#include "Graphics/graphicsenumerations.h"
#include "colourvalue.h"

namespace Ogre
{
    class Light;
}

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy class for placing and manipulating lighting in the scene.
        ///////////////////////////////////////
        class MEZZ_LIB LightProxy : public RenderableProxy
        {
        protected:
            /// @brief A pointer to the internal Light this proxy is based on.
            Ogre::Light* GraphicsLight;

            /// @brief Creates an internal light to be used by the calling instance.
            virtual void CreateLight();
            /// @brief Destroys the internal light in use by this proxy.
            virtual void DestroyLight();
        public:
            /// @brief Blank constructor.
            /// @param ID The unique ID assigned to this LightProxy.
            /// @param Creator A pointer to the manager that created this proxy.
            LightProxy(const UInt32 ID, SceneManager* Creator);
            /// @brief Type constructor.
            /// @remarks The type provided here can be changed later.
            /// @param ID The unique ID assigned to this LightProxy.
            /// @param Type The type of light this light is to be constructed as.
            /// @param Creator A pointer to the manager that created this proxy.
            LightProxy(const UInt32 ID, const Graphics::LightType Type, SceneManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this proxy.
            LightProxy(const XML::Node& SelfRoot, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~LightProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc EntityProxy::GetComponentType() const
            virtual Mezzanine::ComponentType GetComponentType() const;
            /// @copydoc EntityProxy::IsStatic() const
            virtual Boole IsStatic() const;

            /// @brief Sets the direction the light will be emitted from this source.
            /// @note The direction is not used if this is a point light.
            /// @param Dir A Vector3 representing the direction the light will travel from this source.
            virtual void SetDirection(const Vector3& Dir);
            /// @brief Gets the direction the light from this source is being emitted.
            /// @note The direction is not used if this is a point light.
            /// @return Returns a Vector3 representing the direction this proxy is emitting light.
            virtual Vector3 GetDirection() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Light Properties

            /// @brief Sets the colour for the Diffuse light from this source.
            /// @param Diffuse ColourValue representing the color of the Diffuse light to be set.
            virtual void SetDiffuseColour(const ColourValue& Diffuse);
            /// @brief Gets the current colour of Diffuse light being emitted by this proxy.
            /// @return Returns a colourvalue representing the currently set Diffuse light colour.
            virtual ColourValue GetDiffuseColour() const;
            /// @brief Sets the colour for the Specular light from this source.
            /// @param Specular ColourValue representing the color of the Specular light to be set.
            virtual void SetSpecularColour(const ColourValue& Specular);
            /// @brief Gets the current colour of Specular light being emitted by this proxy.
            /// @return Returns a colourvalue representing the currently set Specular light colour.
            virtual ColourValue GetSpecularColour() const;

            /// @brief Sets the type of light this light is.
            /// @param Type The enum value representing the type of light this will become.
            virtual void SetType(const Graphics::LightType Type);
            /// @brief Gets the type of light that this light is.
            /// @return Returns an enum value representing this lights type.
            virtual Graphics::LightType GetType() const;
            /// @brief Sets the factors for the attenuation formula applied to this light.
            /// @param Range The absolute range of the light in world units.  Objects beyond this range will not be influenced by this light.
            /// @param Constant The constant of the attenuation, ranging from 0.0 to 1.0.  1.0 means never attenuate, 0.0 is complete attenuation.
            /// @param Linear The linear factor of the attentuation, ranging from 0.0 to 1.0.  1.0 means attenuate evenly over the distance.
            /// @param Quadratic The Quadratic factor of the attenuation.  This value adds curvature to the attenuation.
            virtual void SetAttenuation(const Real Range, const Real Constant, const Real Linear, const Real Quadratic);
            /// @brief Gets the absolute range of attenuation in world units.
            /// @return Returns a real representing the absolute range of attenuation.
            virtual Real GetAttenuationRange() const;
            /// @brief Gets the constant factor of the attenuation.
            /// @return Returns a real representing the constant factor of attenuation.
            virtual Real GetAttenuationConstant() const;
            /// @brief Gets the linear factor of the attentuation.
            /// @return Returns a real representing the linear factor of attenuation.
            virtual Real GetAttenuationLinear() const;
            /// @brief Gets the quadric factor of the attenuation.
            /// @return Returns a real representing the quadric factor of attenuation.
            virtual Real GetAttenuationQuadratic() const;
            /// @brief Sets the power scale of this light, which indicates it's relative power.
            /// @note The power scale is useful when accessed by shaders in HDR rendering.
            /// @param Scale The power scale to set for this light.
            virtual void SetPowerScale(const Real Scale);
            /// @brief Gets the power scale of this light, which indicates it's relative power.
            /// @note The power scale is useful when accessed by shaders in HDR rendering.
            /// @return Returns a Real representing this lights current power scale.
            virtual Real GetPowerScale() const;

            /// @brief Defines the cone of light emitted by a spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @remarks InnerAngle and OuterAngle should be input as Radians.
            /// @param InnerAngle Angle of the inner cone in radians.
            /// @param OuterAngle Angle of the outer cone in radions.
            /// @param Falloff The rate of falloff between the inner and outer cones.  1.0 means linear falloff.  Less means slower falloff and higher means faster falloff.
            virtual void SetSpotlightRange(const Real InnerAngle, const Real OuterAngle, const Real Falloff = 1.0);
            /// @brief Sets the Inner angle of the cone of light emitted by a spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @param Angle The angle of the inner cone in radians.
            virtual void SetSpotlightInnerAngle(const Real Angle);
            /// @brief Gets the Inner angle of the cone of light emitted by this spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @return Returns a real representing the inner angle of this spotlight, in radians.
            virtual Real GetSpotlightInnerAngle() const;
            /// @brief Sets the Outer angle of the cone of light emitted by a spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @param Angle The angle of the outer cone in radians.
            virtual void SetSpotlightOuterAngle(const Real Angle);
            /// @brief Gets the Outer angle of the cone of light emitted by this spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @return Returns a real representing the outer angle of this spotlight, in radians.
            virtual Real GetSpotlightOuterAngle() const;
            /// @brief Sets the rate of falloff of the cone of light emitted by a spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @param Falloff The rate of falloff between the inner and outer cones.  1.0 means linear falloff.  Less means slower falloff and higher means faster falloff.
            virtual void SetSpotlightFalloff(const Real Falloff);
            /// @brief Gets the rate of falloff of the cone of light emitted by this spotlight.
            /// @note These values are only used if this lights type is set to "Spotlight".
            /// @return Returns a real representing the falloff of the cone of light.
            virtual Real GetSpotlightFalloff() const;
            /// @brief Sets the near clip plane distance to be used by spotlights that use light clipping.
            /// @param NearClip A Real representing the near clip distance used by spotlights.
            virtual void SetSpotlightNearClipDistance(const Real NearClip);
            /// @brief Gets the near clip plane distance to be used by spotlights that use light clipping.
            /// @return Returns the near clip distance used by spotlights.
            virtual Real GetSpotlightNearClipDistance() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc EntityProxy::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc EntityProxy::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc EntityProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc EntityProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal light.
            /// @return Returns a pointer to the internal light this proxy is based on.
            virtual Ogre::Light* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const;
        };//LightProxy
    }//Graphics
}//Mezzanine

#endif
