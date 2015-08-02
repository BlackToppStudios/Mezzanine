// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#include "XML/xml.h"

namespace Ogre
{
    class Pass;
    class Material;
    class Texture;
    template<typename T> class SharedPtr;
    typedef SharedPtr<Material> MaterialPtr;
    typedef SharedPtr<Texture> TexturePtr;
}

namespace Mezzanine
{
    namespace UI
    {
        struct TextureAtlasInternalData;
        class FontData;
        class Sprite;
        class Glyph;

        /// @brief
        /// @details
        class MEZZ_LIB TextureAtlas
        {
        public:
            typedef std::map<String,FontData*>           FontDataContainer;
            typedef FontDataContainer::iterator          FontDataIterator;
            typedef FontDataContainer::const_iterator    ConstFontDataIterator;
            typedef std::map<String,Sprite*>             SpriteContainer;
            typedef SpriteContainer::iterator            SpriteIterator;
            typedef SpriteContainer::const_iterator      ConstSpriteIterator;
        protected:
            TextureAtlasInternalData* TAID;
            FontDataContainer Fonts;
            SpriteContainer Sprites;
            Vector2 WhitePixel;
            Vector2 InverseTextureSize;
            String AtlasName;

            /// @brief Parses the texture section of the Texture Atlas.
            void ParseTexture(XML::Node& AtlasTextureNode);
            /// @brief Parses the data for a font.
            void ParseFonts(XML::Node& AtlasFontsNode);
            /// @brief Parses the glyphs section of the Texture Atlas.
            void ParseGlyphs(XML::Node& GlyphsNode, const Vector2& Offset, FontData* GlyphD);
            /// @brief Parses the kerning section of the Texture Atlas.
            void ParseKernings(XML::Node& KerningsNode, FontData* FontD);
            /// @brief Parses the vertical offsets section of the Texture Atlas.
            void ParseVerticalOffsets(XML::Node& VerticalOffsetsNode, FontData* FontD);
            /// @brief Parses the sprites section of the Texture Atlas.
            void ParseSprites(XML::Node& AtlasSpritesNode);
            /// @brief Creates the material to be used by 2D interfaces with this Atlas.
            void Create2DMaterial();
            /// @brief Creates the material to be used by 3D interfaces with this Atlas.
            void Create3DMaterial();
            /// @brief Gets or creates the 2D "template" or "master" material that is to be used to copy from for all Atlases.
            Ogre::MaterialPtr GetOrCreate2DMasterMaterial();
            /// @brief Gets or creates the 3D "template" or "master" material that is to be used to copy from for all Atlases.
            Ogre::MaterialPtr GetOrCreate3DMasterMaterial();
        public:
            /// @brief Proceadural constructor.
            /// @param Name The name to be given to this Texture Atlas.
            /// @param Width The width of the desired texture to create for this atlas.
            /// @param Height The height of the desired texture to create for this atlas.
            TextureAtlas(const String& Name, const UInt32& Width, const UInt32& Height);
            /// @brief XML constructor.
            /// @param AtlasNode The XML node containing all the information to create and configure this texture atlas.
            TextureAtlas(XML::Node& AtlasNode);
            /// @brief Class destructor.
            ~TextureAtlas();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this Texture Atlas.
            /// @return Returns a string containing the name of this Texture Atlas
            const String& GetName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Information Gathering

            /// @brief Gets the set of Glyphs of the specified name.
            /// @param Name The name of the font as specified in the MTA file.
            /// @return Returns a pointer to the GlyphData containing all the Glyphs.
            FontData* GetFont(const String& FontName) const;
            /// @brief Gets the full listing of all the FontData instances in this TextureAtlas.
            /// @return Returns a reference to the map storing all the FontData instances loaded for this TextureAtlas.
            FontDataContainer& GetFonts();
            /// @brief Gets a Sprite by name.
            /// @param Name The name of the Sprite to get.
            /// @return Returns a pointer to the named Sprite.
            Sprite* GetSprite(const String& Name) const;
            /// @brief Gets the full listing of all the Sprite's in this TextureAtlas.
            /// @return Returns a reference to the map storing all the Sprite's loaded for this TextureAtlas.
            SpriteContainer& GetSprites();
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
