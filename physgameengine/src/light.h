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

#include "attachable.h"
#include "colourvalue.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector3.h"
#include "xml.h"

/// @file light.h
/// @brief Declaration of phys::Light

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
    /// direction, just a Location. @n
    /// Spotlight - Used to simulate local light sources that emit light in one direction, such as a flashlight.
    /// Needs both a Location and direction.  In addition needs values for falloff. @n
    /// Note: If attaching a light to a node, all transform information(Location and orientation) becomes relative
    /// to the nodes transform.
    ///////////////////////////////////////
    class PHYS_LIB Light : public Attachable
    {
        public:
            /// @brief What kind of light Source.
            enum LightType
            {
                Directional = 0,    ///< From one direction, like sunlight.
                Point       = 1,    ///< From a point in space, like a Torch, campfire, muzzle flash, Mutant Fireflies, bonfires, light bulbs, non-hooded lantern, the DnD D20 Light spell, explosions, and scotch tape separating from the roll in a unlit vacuum. There may be other uses, be creative.
                Spotlight   = 2     ///< From a point emanating in a cone, like a flashlight, hooded lantern, really bright computer screens, flood lights, older style space heaters, Concert lights, camera flashes, etc...
            };
        protected:
            /// @brief The ogre light this class gets it's functionality from.
            Ogre::Light* OgreLight;
            /// @brief Pointer to the manager that created this class.
            SceneManager* Manager;
        public:

            ///////////////////////////////////////////////////////////////////////////////
            /// Construction

            /// @brief Standard initialization constructor.
            /// @param Name The name of this light.
            /// @param manager Pointer to the manager that this light is to be used in.
            Light(const String& Name, SceneManager* manager);

            /// @internal
            /// @brief Internal Constructor
            /// @details This constructor should not be called on manually.
            /// @param light The Ogre light this class is based on.
            /// @param manager Pointer to the manager that this light is to be used in.
            Light(Ogre::Light* light, SceneManager* manager);

            /// @brief Class destructor.
            virtual ~Light();

            ///////////////////////////////////////////////////////////////////////////////
            /// Meta

            /// @brief Gets the name of this light.
            /// @return Returns a string containing the name given to this light.
            ConstString& GetName() const;

            /// @brief Sets the type of light this light is.
            /// @details The light types are listed with the class info.  Types are Directional, Point, and Spotlight.
            /// @param Type The enum value representing the type of light this is.
            void SetType(Light::LightType Type);
            /// @brief Gets the type of light that this light is.
            /// @return Returns an enum value for this lights type.
            Light::LightType GetType() const;

            ///////////////////////////////////////////////////////////////////////////////
            /// 3d navigation

            /// @brief Sets the location from where the light will originate.
            /// @param Location A vector3 representing the location to set the light.
            /// @details Not used in Directional Lights
            void SetLocation(const Vector3& Location);
            /// @brief Sets the direction the light will originate from.
            /// @param Direction A vector3 representing the direction the light will come from.
            void SetDirection(const Vector3& Direction);
            /// @details Not used in point lights

            /// @brief Gets the current location of the light.
            /// @return Returns a vector3 representing the location of the light.
            /// @details Not used in Directional Lights
            Vector3 GetLocation() const;
            /// @brief Gets the currently set direction of the light.
            /// @return Returns a vector3 representing the set direction of the light.
            /// @details Not used in point lights
            Vector3 GetDirection() const;

            ///////////////////////////////////////////////////////////////////////////////
            /// Colour

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
            void SetDiffuseColour(const ColourValue &Colour);
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
            void SetSpecularColour(const ColourValue &Colour);

            /// @brief Gets the current colour of the diffuse light.
            /// @return Returns a colourvalue representing the currently set Diffuse light.
            ColourValue GetDiffuseColour() const;
            /// @brief Gets the current colour of the specular light.
            /// @return Returns a colourvalue representing the currently set Specular light.
            ColourValue GetSpecularColour() const;

            ///////////////////////////////////////////////////////////////////////////////
            /// Attenuation

            /// @brief Sets the factors for the attenuation formula applied to this light.
            /// @details This function is only applicable when using a spotlight or point type of light.
            /// @param Range The absolute range of the light in world units.  Objects beyond this range will not be influenced by this light.
            /// @param Constant The constant of the attenuation, ranging from 0.0 to 1.0.  1.0 means never attenuate, 0.0 is complete attenuation.
            /// @param Linear The linear factor of the attentuation, ranging from 0.0 to 1.0.  1.0 means attenuate evenly over the distance.
            /// @param Quadratic The Quadratic factor of the attenuation.  This value adds curvature to the attenuation.
            void SetAttenuation(Real Range, Real Constant, Real Linear, Real Quadratic);
            /// @brief Gets the absolute range of attenuation in world units.
            /// @return Returns a real representing the absolute range of attenuation.
            Real GetAttenuationRange() const;
            /// @brief Gets the constant factor of the attenuation.
            /// @return Returns a real representing the constant factor of attenuation.
            Real GetAttenuationConstant() const;
            /// @brief Gets the linear factor of the attentuation.
            /// @return Returns a real representing the linear factor of attenuation.
            Real GetAttenuationLinear() const;
            /// @brief Gets the quadric factor of the attenuation.
            /// @return Returns a real representing the quadric factor of attenuation.
            Real GetAttenuationQuadric() const;

            ///////////////////////////////////////////////////////////////////////////////
            /// Spotlight specific methods

            /// @brief Defines the cone of light emitted by a spotlight.
            /// @details InnerAngle and OuterAngle should be input as Radians.
            /// @param InnerAngle Angle of the inner cone in radians.
            /// @param OuterAngle Angle of the outer cone in radions.
            /// @param Falloff The rate of falloff between the inner and outer cones.  1.0 means linear falloff.
            /// Less means slower falloff and higher means faster falloff.
            void SetSpotlightRange(Real InnerAngle, Real OuterAngle, Real Falloff=1.0);
            /// @brief Sets the Inner angle of the cone of light emitted by a spotlight.
            /// @param InnerAngle Angle of the inner cone in radians.
            void SetSpotlightInnerAngle(Real InnerAngle);
            /// @brief Sets the Outer angle of the cone of light emitted by a spotlight.
            /// @param OuterAngle Angle of the outer cone in radians.
            void SetSpotlightOuterAngle(Real OuterAngle);
            /// @brief Sets the rate of falloff of the cone of light emitted by a spotlight.
            /// @param Falloff The rate of falloff between the inner and outer cones.  1.0 means linear falloff.
            /// Less means slower falloff and higher means faster falloff.
            void SetSpotlightFalloff(Real Falloff);

            /// @brief Gets the Inner angle of the cone of light emitted by this spotlight.
            /// @return Returns a real representing the inner angle of this spotlight, in radians.
            Real GetSpotlightInnerAngle() const;
            /// @brief Gets the Outer angle of the cone of light emitted by this spotlight.
            /// @return Returns a real representing the outer angle of this spotlight, in radians.
            Real GetSpotlightOuterAngle() const;
            /// @brief Gets the rate of falloff of the cone of light emitted by this spotlight.
            /// @return Returns a real representing the falloff of the cone of light.
            Real GetSpotlightFalloff() const;

            ///////////////////////////////////////////////////////////////////////////////
            /// Effects

            /// @brief Gets the lights power scale.
            /// @return Returns a real representing the power scale of the light.
            /// @details Only used with HDR ( High Dynamic Range ) rendering
            Real GetPowerScale() const;

            /// @brief Sets the lights power scale.
            /// @details The power scale of the light is a scaling factor indicating the relative power of the light.
            /// @param Power Real representing the factor by which to scale the power of the light.
            void SetPowerScale(Real Power);

            ///////////////////////////////////////////////////////////////////////////////
            /// Inherited From Attachable

            /// @brief What kind of Attachable is this.
            /// @return An Attachable::GetAttachableType containing Attachable::Light.
            virtual Attachable::AttachableElement GetAttachableType() const;

            virtual void AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target);

            virtual void DetachFromFinal(Ogre::SceneNode* RawTarget);
    };
}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML

/// @brief Serializes the passed phys::Light to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::Light to be serialized
/// @return this returns the ostream, now with the serialized data
/// @warning This does not attempt to store the Light aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not s the pointer to the Light manager. When a Light manager is serialized, this data is implicitly stored by the Lights location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::Light& Ev);

/// @brief Deserialize a phys::Light
/// @param stream The istream to get the xml from to (re)make the phys::Light.
/// @param Ev the phys::Light to be deserialized.
/// @return this returns the ostream, advanced past the phys::Light that was recreated onto Ev.
/// @warning This does not attempt to store the Light aspect ratio. This is too often hardware dependent and may not be reliably re-serialized.
/// @warning This does not s the pointer to the Light manager. When a Light manager is serialized, this data is implicitly stored by the Lights location in the xml hierarchy, this is used instead. The Name of the manager is stored for possible future use.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Light& Ev);

/// @brief Set all values of a phys::Light from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::Light.
/// @param Ev the phys::Light to be reset.
/// @return This returns thexml::Node that was passed in.
/// @warning This does not attempt to de-serialize the name of the Light. This is not currently changeable after the creation of a Light. However, the Lightmanager will correctly create name Light upon creation then deserialize the rest of the Light.
/// @warning This does not throw an exception if the Light could not be attached to the appropriate worldnode. It is assumed that the worldnode will be able to adjust the pointer on this if it is deserialized second.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::Light& Ev);

#endif // \PHYSXML



#endif
