//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _light_h
#define _light_h

#include "datatypes.h"
#include "colourvalue.h"
#include "vector3.h"
#include "attachable.h"

namespace Ogre
{
    class Light;
}

namespace phys
{
    class SceneManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Light
    /// @headerfile light.h
    /// @brief This class is the class used for dynamic lighting within the scene.
    /// @details Dynamic lights come in 3 flavors: @n
    /// Directional - Used to simulate light from a very distant source.  Doesn't need a location, only
    /// a direction.  Light will hit all objects accordingly from that direction. @n
    /// Point - Used to simulate local light sources that emit light in all directions.  Doesn't need a
    /// direction, just a position. @n
    /// Spotlight - Used to simulate local light sources that emit light in one direction, such as a flashlight.
    /// Needs both a position and direction.  In addition needs values for falloff.
    ///////////////////////////////////////
    class Light : public Attachable
    {
        public:
            enum LightType
            {
                Directional,
                Point,
                Spotlight
            };
        protected:
            friend class Node;
            /// @brief The ogre light this class gets it's functionality from.
            Ogre::Light* OgreLight;
            /// @brief Pointer to the manager that created this class.
            SceneManager* Manager;
        public:
            /// @brief Standard initialization constructor.
            /// @param Name The name of this light.
            /// @param manager Pointer to the manager that this light is to be used in.
            Light(const String& Name, SceneManager* manager);
            /// @brief Internal Constructor
            /// @details This constructor should not be called on manually.
            /// @param light The Ogre light this class is based on.
            /// @param manager Pointer to the manager that this light is to be used in.
            Light(Ogre::Light* light, SceneManager* manager);
            /// @brief Class destructor.
            ~Light();
            /// @brief Gets the name of this light.
            /// @return Returns a string containing the name given to this light.
            String& GetName();
            /// @brief Sets the type of light this light is.
            /// @details The light types are listed with the class info.  Types are Directional, Point, and Spotlight.
            /// @param Type The enum value representing the type of light this is.
            void SetType(Light::LightType Type);
            /// @brief Sets the location from where the light will originate.
            /// @param Position A vector3 representing the location to set the light.
            void SetPosition(Vector3 Position);
            /// @brief Sets the direction the light will originate from.
            /// @param Direction A vector3 representing the direction the light will come from.
            void SetDirection(Vector3 Direction);
            /// @brief Sets the colour for the Diffuse light from this source.
            /// @details When rendering the final color of an object values of the colours of 3 types of lights are taken into account.
            /// The 3 types are: Diffuse, Specular, and Ambient.
            /// @param Red Real in the range of 0.0 to 1.0 determining the amount of red in the colour.
            /// @param Red green in the range of 0.0 to 1.0 determining the amount of green in the colour.
            /// @param Red blue in the range of 0.0 to 1.0 determining the amount of blue in the colour.
            void SetDiffuseColour(Real Red, Real Green, Real Blue);
            /// @brief Sets the colour for the Diffuse light from this source.
            /// @details This allows the setting of Diffuse color by a premade ColourValue.
            /// @param Colour ColourValue representing the color of the light to be set.
            void SetDiffuseColour(ColourValue &Colour);
            /// @brief Sets the colour for the Specular light from this source.
            /// @details When rendering the final color of an object values of the colours of 3 types of lights are taken into account.
            /// The 3 types are: Diffuse, Specular, and Ambient.
            /// @param Red Real in the range of 0.0 to 1.0 determining the amount of red in the colour.
            /// @param Red green in the range of 0.0 to 1.0 determining the amount of green in the colour.
            /// @param Red blue in the range of 0.0 to 1.0 determining the amount of blue in the colour.
            void SetSpecularColour(Real Red, Real Green, Real Blue);
            /// @brief Sets the colour for the Specular light from this source.
            /// @details This allows the setting of Specular color by a premade ColourValue.
            /// @param Colour ColourValue representing the color of the light to be set.
            void SetSpecularColour(ColourValue &Colour);
            /// @brief Sets the factors for the attenuation formula applied to this light.
            /// @details This function is only necessary when using a spotlight or point type of light.
            /// @param Range The absolute range of the light in world units.  Objects beyond this range will not be influenced by this light.
            /// @param Constant The constant of the attenuation, ranging from 0.0 to 1.0.  1.0 means never attenuate, 0.0 is complete attenuation.
            /// @param Linear The linear factor of the attentuation, ranging from 0.0 to 1.0.  1.0 means attenuate evenly over the distance.
            /// @param Quadratic The Quadratic factor of the attenuation.  This value adds curvature to the attenuation.
            void SetAttenuation(Real Range, Real Constant, Real Linear, Real Quadratic);
            /// @brief
            /// @details InnerAngle and OuterAngle should be input as Radians.
            /// @param InnerAngle
            /// @param OuterAngle
            /// @param Falloff
            void SetSpotlightRange(Real InnerAngle, Real OuterAngle, Real Falloff=1.0);
            /// @brief
            /// @param InnerAngle
            void SetSpotlightInnerAngle(Real InnerAngle);
            /// @brief
            /// @param OuterAngle
            void SetSpotlightOuterAngle(Real OuterAngle);
            /// @brief
            /// @param Falloff
            void SetSpotlightFalloff(Real Falloff);
            /// @brief Sets the lights power scale.
            /// @details The power scale of the light is a scaling factor indicating the relative power of the light.
            /// @param Power Real representing the factor by which to scale the power of the light.
            void SetPowerScale(Real Power);
            /// @brief
            /// @return Returns an enum value for this lights type.
            Light::LightType GetType();
            /// @brief
            /// @return Returns a vector3
            Vector3 GetPosition();
            /// @brief
            /// @return Returns a vector3
            Vector3 GetDirection();
            /// @brief
            /// @return Returns a colourvalue
            ColourValue GetDiffuseColour();
            /// @brief
            /// @return Returns a colourvalue
            ColourValue GetSpecularColour();
            /// @brief
            /// @return Returns a real
            Real GetAttenuationRange();
            /// @brief
            /// @return Returns a real
            Real GetAttenuationConstant();
            /// @brief
            /// @return Returns a real
            Real GetAttenuationLinear();
            /// @brief
            /// @return Returns a real
            Real GetAttenuationQuadric();
            /// @brief
            /// @return Returns a real
            Real GetSpotlightInnerAngle();
            /// @brief
            /// @return Returns a real
            Real GetSpotlightOuterAngle();
            /// @brief
            /// @return Returns a real
            Real GetSpotlightFalloff();
            /// @brief Gets the lights power scale.
            /// @return Returns a real representing the power scale of the light.
            Real GetPowerScale();
    };
}//phys

#endif
