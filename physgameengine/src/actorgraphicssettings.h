//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _actorgraphicssettings_h
#define _actorgraphicssettings_h

#include "datatypes.h"
#include "colourvalue.h"

namespace Ogre
{
    class Entity;
    class MaterialPtr;
}

namespace phys
{
    namespace internal
    {
        class InternalActorGraphicsSettings;
    }
    class ActorBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ActorGraphicsSettings
    /// @headerfile actorgraphicssettings.h
    /// @brief This is a helper class for configuring graphics settings of an actor.
    /// @details This class contains functions for the configuring of graphics specific settings of an actor.
    /// This class can only configure the actors graphics.  For configuring actor physics, see ActorBasePhysicsSettings.
    ///////////////////////////////////////
    class ActorGraphicsSettings
    {
        protected:
            /// @internal
            /// @brief Stores all the data to go with the
            internal::InternalActorGraphicsSettings* IAGS;

            ///@internal
            /// @brief Material/textures for the actor
            Ogre::MaterialPtr GetMaterial(Whole Submesh = 0);
        public:

            ///////////////////////////////////////////////////////////////////////////////
            // Construction

            /// @brief Standard Constructor.
            /// @param Actor The actor this settings class configures.
            /// @param GraphicsObject The graphics object belonging to the actor this class configures.
            ActorGraphicsSettings(ActorBase* Actor, Ogre::Entity* GraphicsObject);
            /// @brief Class destructor.
            ~ActorGraphicsSettings();

            ///////////////////////////////////////////////////////////////////////////////
            // Material Management

            /// @brief Sets the material of the specified submesh.
            /// @details The name of the material isn't the same as the name of the .material file.
            /// As a single material file can contain several material scripts.  The name this function
            /// expects is the name of the material script inside a .material file.
            /// @param MatName The name of the material to be applied.
            /// @param Submesh The submesh to apply the material to.
            void SetMaterial(String MatName, Whole Submesh = 0);
            /// @brief Gets the material name of the specified submesh.
            /// @details Note the returned name isn't of the .material file, but the material script.
            /// @param Submesh The submesh you want to get the material name from.
            /// @return Returns a String containing the name of the material script in use.
            ConstString& GetMaterialName(Whole Submesh = 0);
            /// @brief Gets whether or not the specified submesh has a material script assigned to it.
            /// @param Submesh The submesh to check.
            /// @return Returns a bool indicating whether there is a material assigned to the specified submesh.
            bool HasMaterialSet(Whole Submesh = 0);
            /// @brief Gets the number of submeshes in the mesh of the graphics ohject.
            /// @return Returns a whole representing the number of submeshes in this objects mesh.
            Whole GetNumSubmeshes();

            ///////////////////////////////////////////////////////////////////////////////
            // Material Colors

            /// @brief Sets the Ambient colour value of the material belonging to the specified submesh.
            /// @details The set value is applied to every technique of every pass in the material.
            /// @param Ambient The colour you wish to set as the material Ambient.
            /// @param Submesh The submesh you want to alter the material of.
            void SetMaterialAmbient(const ColourValue& Ambient, Whole Submesh = 0);
            /// @brief Sets the Specular colour value of the material belonging to the specified submesh.
            /// @details The set value is applied to every technique of every pass in the material.
            /// @param Specular The colour you wish to set as the material Specular.
            /// @param Submesh The submesh you want to alter the material of.
            void SetMaterialSpecular(const ColourValue& Specular, Whole Submesh = 0);
            /// @brief Sets the Diffuse colour value of the material belonging to the specified submesh.
            /// @details The set value is applied to every technique of every pass in the material.
            /// @param Diffuse The colour you wish to set as the material Diffuse.
            /// @param Submesh The submesh you want to alter the material of.
            void SetMaterialDiffuse(const ColourValue& Diffuse, Whole Submesh = 0);

            /// @brief Gets the Ambient colour value of the material belonging to the specified submesh.
            /// @details If no Ambient colour was specified for a given submesh a default ColourValue is returned.
            /// @param Submesh The submesh you want to alter the material of.
            /// @return The colour that was as the material Ambient colour, OR a default colourvalue.
            ColourValue GetMaterialAmbient(Whole Submesh = 0);
            /// @brief Gets the Specular colour value of the material belonging to the specified submesh.
            /// @details If no Specular colour was specified for a given submesh a default ColourValue is returned.
            /// @param Submesh The submesh you want to alter the material of.
            /// @return The colour that was as the material Specular colour, OR a default colourvalue.
            ColourValue GetMaterialSpecular(Whole Submesh = 0);
            /// @brief Gets the Diffuse colour value of the material belonging to the specified submesh.
            /// @details If no Diffuse colour was specified for a given submesh a default ColourValue is returned.
            /// @param Submesh The submesh you want to alter the material of.
            /// @return The colour that was as the material Diffuse colour, OR a default colourvalue.
            ColourValue GetMaterialDiffuse(Whole Submesh = 0);



    };//actorgraphicssettings
}//phys

#endif
