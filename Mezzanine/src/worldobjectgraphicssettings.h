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
#ifndef _worldobjectgraphicssettings_h
#define _worldobjectgraphicssettings_h

#include "datatypes.h"
#include "colourvalue.h"

#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Ogre
{
    class Entity;
    class MaterialPtr;
}

namespace Mezzanine
{
    namespace Internal
    {
        class InternalWorldObjectGraphicsSettings;
    }
    namespace Graphics
    {
        class Mesh;
    }
    class ActorBase;
    class WorldObject;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldObjectGraphicsSettings
    /// @headerfile worldobjectgraphicssettings.h
    /// @brief This is a helper class for configuring graphics settings of a World Object.
    /// @details This class contains functions for the configuring of graphics specific settings of an actor.
    /// This class can only configure the actors graphics.  For configuring World Object physics, see ActorBasePhysicsSettings.
    ///////////////////////////////////////
    class MEZZ_LIB WorldObjectGraphicsSettings
    {
        protected:
            /// @internal
            /// @brief Stores all the data to go with the
            Internal::InternalWorldObjectGraphicsSettings* IWOGS;

            /// @internal
            /// @brief The World Object this belongs to.
            WorldObject* Parent;

            /// @internal
            /// @brief The mesh being used by the World Object.
            Graphics::Mesh* WorldObjectMesh;

            ///@internal
            /// @brief Material/textures for the World Object.
            virtual Ogre::MaterialPtr GetMaterial(const Whole& Submesh = 0);
        public:

            ///////////////////////////////////////////////////////////////////////////////
            // Construction

            /// @brief Standard Constructor.
            /// @param WO The World Object this settings class configures.
            /// @param GraphicsObject The graphics object belonging to the World Object this class configures.
            WorldObjectGraphicsSettings(WorldObject* WO, Ogre::Entity* GraphicsObject);
            /// @brief Class destructor.
            virtual ~WorldObjectGraphicsSettings();

            ///////////////////////////////////////////////////////////////////////////////
            // Mesh Management

            /// @brief Sets the mesh to be used along with this World Object.
            /// @param Mesh The mesh to apply to this World Object.
            virtual void SetMesh(Graphics::Mesh* ObjectMesh);

            /// @brief Sets the mesh to be used along with this World Object.
            /// @param MeshName The name of the mesh to apply to this World Object.
            /// @param Group The resource group to which the mesh belongs.
            virtual void SetMesh(const String& MeshName, const String& Group);

            /// @brief Gets the mesh being used by this World Object.
            /// @return Returns a pointer to the mesh being used by this World Object.
            virtual Graphics::Mesh* GetMesh() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Material Management

            /// @brief Sets the material of the specified submesh.
            /// @details The name of the material isn't the same as the name of the .material file.
            /// As a single material file can contain several material scripts.  The name this function
            /// expects is the name of the material script inside a .material file.
            /// @param MatName The name of the material to be applied.
            /// @param Submesh The submesh to apply the material to.
            virtual void SetMaterial(const String& MatName, const Whole& Submesh = 0);
            /// @brief Gets the material name of the specified submesh.
            /// @details Note the returned name isn't of the .material file, but the material script.
            /// @param Submesh The submesh you want to get the material name from.
            /// @return Returns a String containing the name of the material script in use.
            virtual ConstString GetMaterialName(const Whole& Submesh = 0) const;
            /// @brief Gets whether or not the specified submesh has a material script assigned to it.
            /// @param Submesh The submesh to check.
            /// @return Returns a bool indicating whether there is a material assigned to the specified submesh.
            virtual bool HasMaterialSet(const Whole& Submesh = 0);
            /// @brief Gets the number of submeshes in the mesh of the graphics ohject.
            /// @return Returns a whole representing the number of submeshes in this objects mesh.
            virtual Whole GetNumSubmeshes() const;
            /// @brief Creates a copy of the material script of the graphics object and places it in the same resource group.
            /// @param NewName of the cloned material script.
            virtual void CloneMaterial(const String& NewName);

            ///////////////////////////////////////////////////////////////////////////////
            // Material Colors

            /// @brief Sets the Ambient colour value of the material belonging to the specified submesh.
            /// @details The set value is applied to every technique of every pass in the material.
            /// @param Ambient The colour you wish to set as the material Ambient.
            /// @param Submesh The submesh you want to alter the material of.
            virtual void SetMaterialAmbient(const ColourValue& Ambient, const Whole& Submesh = 0);
            /// @brief Sets the Specular colour value of the material belonging to the specified submesh.
            /// @details The set value is applied to every technique of every pass in the material.
            /// @param Specular The colour you wish to set as the material Specular.
            /// @param Submesh The submesh you want to alter the material of.
            virtual void SetMaterialSpecular(const ColourValue& Specular, const Whole& Submesh = 0);
            /// @brief Sets the Diffuse colour value of the material belonging to the specified submesh.
            /// @details The set value is applied to every technique of every pass in the material.
            /// @param Diffuse The colour you wish to set as the material Diffuse.
            /// @param Submesh The submesh you want to alter the material of.
            virtual void SetMaterialDiffuse(const ColourValue& Diffuse, const Whole& Submesh = 0);

            /// @brief Gets the Ambient colour value of the material belonging to the specified submesh.
            /// @details If no Ambient colour was specified for a given submesh a default ColourValue is returned.
            /// @param Submesh The submesh you want to alter the material of.
            /// @return The colour that was as the material Ambient colour, OR a default colourvalue.
            virtual ColourValue GetMaterialAmbient(const Whole& Submesh = 0) const;
            /// @brief Gets the Specular colour value of the material belonging to the specified submesh.
            /// @details If no Specular colour was specified for a given submesh a default ColourValue is returned.
            /// @param Submesh The submesh you want to alter the material of.
            /// @return The colour that was as the material Specular colour, OR a default colourvalue.
            virtual ColourValue GetMaterialSpecular(const Whole& Submesh = 0) const;
            /// @brief Gets the Diffuse colour value of the material belonging to the specified submesh.
            /// @details If no Diffuse colour was specified for a given submesh a default ColourValue is returned.
            /// @param Submesh The submesh you want to alter the material of.
            /// @return The colour that was as the material Diffuse colour, OR a default colourvalue.
            virtual ColourValue GetMaterialDiffuse(const Whole& Submesh = 0) const;


            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ActorGraphicsSettings"
            static String SerializableName();

    };//worldobjectgraphicssettings
}//Mezzanine

/// @brief Serializes the passed Mezzanine::WorldObjectGraphicsSettings to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::WorldObjectGraphicsSettings to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::WorldObjectGraphicsSettings& Ev);

/// @brief Deserialize a Mezzanine::WorldObjectGraphicsSettings
/// @param stream The istream to get the xml from to (re)make the Mezzanine::ActorGraphicsSettings.
/// @param Ev the Mezzanine::WorldObjectGraphicsSettings to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::ActorGraphicsSettings that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::WorldObjectGraphicsSettings& Ev);

/// @brief Set all values of a Mezzanine::WorldObjectGraphicsSettings from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::ActorGraphicsSettings.
/// @param Ev the Mezzanine::WorldObjectGraphicsSettings to be reset.
/// @return This returns theXML::Node that was passed in.
Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::WorldObjectGraphicsSettings& Ev);

#endif
