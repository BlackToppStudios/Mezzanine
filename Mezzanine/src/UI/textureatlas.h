//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uitextureatlas_h
#define _uitextureatlas_h

#include "vector2.h"
#include "colourvalue.h"
#include "uienumerations.h"
#include "resourcetextsettingsfile.h"

namespace Ogre
{
    class Pass;
    class MaterialPtr;
    class TexturePtr;
}

namespace Mezzanine
{
    namespace UI
    {
        struct TextureAtlasInternalData;
        class GlyphData;
        class Sprite;
        class Glyph;
        /// @class TextureAtlas
        /// @headerfile uitextureatlas.h
        /// @brief
        /// @details
        class MEZZ_LIB TextureAtlas
        {
            protected:
                TextureAtlasInternalData* TAID;
                std::map<UInt32, GlyphData*> Glyphs;
                std::map<String, Sprite*> Sprites;
                std::vector<ColourValue> MarkupColours;
                Vector2 WhitePixel;
                Vector2 InverseTextureSize;

                /// @brief Loads a Mezzanine Texture Atlas file.
                void Load(const String& MTAFile, const String& Group);
                /// @brief Loads the texture section of the Texture Atlas.
                void LoadTexture(Resource::TextSettingsFile::SettingsMap* Config);
                /// @brief Loads the glyphs section of the Texture Atlas.
                void LoadGlyphs(Resource::TextSettingsFile::SettingsMap* Config, GlyphData* GlyphD);
                /// @brief Loads the kerning section of the Texture Atlas.
                void LoadKerning(Resource::TextSettingsFile::SettingsMap* Config, GlyphData* GlyphD);
                /// @brief Loads the vertical offsets section of the Texture Atlas.
                void LoadVerticalOffsets(Resource::TextSettingsFile::SettingsMap* Config, GlyphData* GlyphD);
                /// @brief Loads the sprites section of the Texture Atlas.
                void LoadSprites(Resource::TextSettingsFile::SettingsMap* Config,const String& Atlas);
                /// @brief Creates the material to be used by 2D interfaces with this Atlas.
                void Create2DMaterial();
                /// @brief Creates the material to be used by 3D interfaces with this Atlas.
                void Create3DMaterial();
                /// @brief Gets or creates the 2D "template" or "master" material that is to be used to copy from for all Atlases.
                Ogre::MaterialPtr GetOrCreate2DMasterMaterial();
                /// @brief Gets or creates the 3D "template" or "master" material that is to be used to copy from for all Atlases.
                Ogre::MaterialPtr GetOrCreate3DMasterMaterial();
                /// @brief Converts all the coordinates on glyphs and sprites from human-readable format to rendersystem-readable format.
                void CalculateCoordinates();
            public:
                /// @brief Class constructor.
                /// @param MTAFile The name of the Mezzanine Texture Atlas File to be loaded.
                /// @param Group The resource group where the MTA file can be found.
                TextureAtlas(const String& MTAFile, const String& Group);
                /// @brief Class destructor.
                ~TextureAtlas();

                ///////////////////////////////////////////////////////////////////////////////
                // Information Gathering

                /// @brief Gets the set of Glyphs of the specified ID.
                /// @param ID Unique integer identifier of the Glyph set as specified in the MTA file.
                /// @return Returns a pointer to the GlyphData containing all the Glyphs.
                GlyphData* GetGlyphData(const UInt32& ID) const;
                /// @brief Gets the full listing of all the GlyphData instances in this TextureAtlas.
                /// @return Returns a reference to the map storing all the GlyphData instances loaded for this TextureAtlas.
                std::map<UInt32, GlyphData*>& GetGlyphs();
                /// @brief Gets a Sprite by name.
                /// @param Name The name of the Sprite to get.
                /// @return Returns a pointer to the named Sprite.
                Sprite* GetSprite(const String& Name) const;
                /// @brief Gets the full listing of all the Sprite's in this TextureAtlas.
                /// @return Returns a reference to the map storing all the Sprite's loaded for this TextureAtlas.
                std::map<String, Sprite*>& GetSprites();
                /// @brief Gets the location of the WhitePixel on the TextureAtlas.
                /// @return Returns a Vector2 with the location of the WhitePixel.
                Vector2 GetWhitePixel() const;
                /// @brief Gets the X coordinate for the WhitePixel.
                /// @return Returns a Real containing the X coordinate of the WhitePixel.
                Real GetWhitePixelX() const;
                /// @brief Gets the Y coordinate for the WhitePixel.
                /// @return Returns a Real containing the Y coordinate of the WhitePixel.
                Real GetWhitePixelY() const;
                /// @brief Gets the size of the TextureAtlas.
                /// @return Returns a Vector2 with the size of this TextureAtlas.
                Vector2 GetTextureSize() const;
                /// @brief Gets the inverse size of the TextureAtlas on the X size.
                /// @return Returns a Real containing the inverse width of the TextureAtlas.
                Real GetInvTextureCoordsX() const;
                /// @brief Gets the inverse size of the TextureAtlas on the Y size.
                /// @return Returns a Real containing the inverse height of the TextureAtlas.
                Real GetInvTextureCoordsY() const;
                /// @brief Sets the Markup colour at the specified index.
                /// @param Index The index to set the colour for.
                /// @param Colour The colour to be set.
                /// @remarks This is relevant only if you are using MarkupText's in Markup Mode in your UI.  See that class documentation for more details.
                void SetMarkupColour(const UInt32& Index, const ColourValue& Colour);
                /// @brief Gets the Markup colour at the specified index.
                /// @param Index The index at which to get the Markup colour.
                /// @return Returns a ColourValue containing the Colour at the specified index.
                ColourValue GetMarkupColour(const UInt32& Index) const;
                /// @brief Clears all currently set Markup colours.
                void ResetMarkupColours();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions

                /// @internal
                /// @brief Gets the 2D Material for this TextureAtlas, creating it also if neccessary.
                /// @return Returns a shared pointer to the internal material for this Atlas.
                Ogre::MaterialPtr _GetOrCreate2DMaterial();
                /// @internal
                /// @brief Gets the 3D Material for this TextureAtlas, creating it also if neccessary.
                /// @return Returns a shared pointer to the internal material for this Atlas.
                Ogre::MaterialPtr _GetOrCreate3DMaterial();
                /// @internal
                /// @brief Gets the texture being used by this Atlas.
                /// @return Returns a shared pointer to the internal texture for this Atlas.
                Ogre::TexturePtr _GetTexture();
                /// @internal
                /// @brief Gets the 2D Material Pass for this TextureAtlas.
                /// @return Returns a pointer to the 2D Pass used for this TextureAtlas.
                Ogre::Pass* _Get2DPass() const;
                /// @internal
                /// @brief Gets the 3D Material Pass for this TextureAtlas.
                /// @return Returns a pointer to the 3D Pass used for this TextureAtlas.
                Ogre::Pass* _Get3DPass() const;
        };//TextureAtlas
    }//UI
}//Mezzanine

#endif
