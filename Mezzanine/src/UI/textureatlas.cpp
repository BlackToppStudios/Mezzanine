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
#ifndef _uitextureatlas_cpp
#define _uitextureatlas_cpp

#include "UI/textureatlas.h"
#include "UI/glyph.h"
#include "UI/sprite.h"
#include "stringtool.h"

#include "entresol.h"

#include <OgreTexture.h>
#include <OgreMaterial.h>
#include <OgrePass.h>
#include <OgreGpuProgram.h>
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////
        // TextureAtlasInternalData class and functions
        struct MEZZ_LIB TextureAtlasInternalData
        {
            Ogre::TexturePtr TATexture;
            Ogre::MaterialPtr Mat2D;
            Ogre::MaterialPtr Mat3D;
            Ogre::Pass* Pass2D;
            Ogre::Pass* Pass3D;

            TextureAtlasInternalData()
            {
                TATexture.setNull();
                Mat2D.setNull();
                Mat3D.setNull();
                Pass2D = NULL;
                Pass3D = NULL;
            }
        };//TextureAtlasInternalData

        ///////////////////////////////////////////////////////////
        // TextureAtlas functions

        TextureAtlas::TextureAtlas(const String& Name, const UInt32& Width, const UInt32& Height) :
            AtlasName(Name)
        {
            this->TAID = new TextureAtlasInternalData();
            /// @todo This is just a starter code sample for proceadurally generated texture atlases.  Not ready and needs to be completed.
            String GroupName = "UI";
            this->TAID->TATexture = Ogre::TextureManager::getSingletonPtr()->createManual(this->AtlasName+"Texture",GroupName,Ogre::TEX_TYPE_2D,Width,Height,0,Ogre::PF_R8G8B8A8,Ogre::TU_DEFAULT);
            this->Create2DMaterial();
            this->Create3DMaterial();
        }

        TextureAtlas::TextureAtlas(XML::Node& AtlasNode)
        {
            this->TAID = new TextureAtlasInternalData();
            // Parse and set the name
            XML::Attribute NameAttrib = AtlasNode.GetAttribute("Name");
            if( !NameAttrib.Empty() ) {
                this->AtlasName = NameAttrib.AsString();
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Empty string being used to set name of Texture Atlas being parsed from XML.");
            }

            XML::Node TextureNode = AtlasNode.GetChild("Texture");
            this->ParseTexture( TextureNode );
            XML::Node FontsNode = AtlasNode.GetChild("Fonts");
            this->ParseFonts( FontsNode );
            XML::Node SpritesNode = AtlasNode.GetChild("Sprites");
            this->ParseSprites( SpritesNode );

            this->Create2DMaterial();
            this->Create3DMaterial();
        }

        TextureAtlas::~TextureAtlas()
        {
            for( FontDataIterator FontIt = this->Fonts.begin() ; FontIt != this->Fonts.end() ; ++FontIt )
            {
                delete (*FontIt).second;
            }
            this->Fonts.clear();

            for( SpriteIterator SpriteIt = this->Sprites.begin() ; SpriteIt != this->Sprites.end() ; ++SpriteIt )
            {
                delete (*SpriteIt).second;
            }
            this->Sprites.clear();

            delete this->TAID;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Private & Setup Functions

        void TextureAtlas::ParseTexture(XML::Node& AtlasTextureNode)
        {
            XML::Attribute CurrAttrib;
            String FileName;
            String GroupName = "UI";

            // Get the filename
            CurrAttrib = AtlasTextureNode.GetAttribute("TexFile");
            if( !CurrAttrib.Empty() )
                FileName = CurrAttrib.AsString();
            else{ MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Empty string parsed for texture file name when parsing Texture Atlas from XML."); }

            // Get the resource group
            CurrAttrib = AtlasTextureNode.GetAttribute("TexFileGroup");
            if( !CurrAttrib.Empty() )
                GroupName = CurrAttrib.AsString();

            // Setup the texture
            this->TAID->TATexture = Ogre::TextureManager::getSingletonPtr()->getByName(FileName,GroupName);
            if(this->TAID->TATexture.isNull())
            {
                this->TAID->TATexture = Ogre::TextureManager::getSingletonPtr()->load(FileName,GroupName,Ogre::TEX_TYPE_2D,0);
            }

            this->InverseTextureSize.X = 1.0 / this->TAID->TATexture->getWidth();
            this->InverseTextureSize.Y = 1.0 / this->TAID->TATexture->getHeight();

            // Get the white pixel location
            CurrAttrib = AtlasTextureNode.GetAttribute("WhitePixel");
            if( !CurrAttrib.Empty() )
                this->WhitePixel = StringTools::ConvertToVector2( CurrAttrib.AsString() );

            // Convert the white pixel location to relative
            this->WhitePixel.X *= this->InverseTextureSize.X;
            this->WhitePixel.Y *= this->InverseTextureSize.Y;
        }

        void TextureAtlas::ParseFonts(XML::Node& AtlasFontsNode)
        {
            for( XML::NodeIterator FontNode = AtlasFontsNode.begin() ; FontNode != AtlasFontsNode.end() ; ++FontNode )
            {
                XML::Attribute CurrAttrib;
                Vector2 Offset(0,0);
                FontData* Data = new FontData(this);

                // Get the name
                CurrAttrib = (*FontNode).GetAttribute("Name");
                if( !CurrAttrib.Empty() )
                    Data->_SetName( CurrAttrib.AsString() );

                // Now that we have the name, quickly check if this is unique before we continue to parse
                FontDataIterator FontIt = Fonts.find(Data->GetName());
                if( FontIt == Fonts.end() ) { Fonts[Data->GetName()] = Data; }
                else { MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Duplicate name of font:\"" + Data->GetName() + "\", found in atlas:\"" + AtlasName + "\"." ); }

                // Get the horizontal offset if there is any
                CurrAttrib = (*FontNode).GetAttribute("OffsetX");
                if( !CurrAttrib.Empty() )
                    Offset.X = CurrAttrib.AsReal();

                // Get the vertical offset if there is any
                CurrAttrib = (*FontNode).GetAttribute("OffsetY");
                if( !CurrAttrib.Empty() )
                    Offset.Y = CurrAttrib.AsReal();

                // Get the lineheight
                CurrAttrib = (*FontNode).GetAttribute("LineHeight");
                if( !CurrAttrib.Empty() )
                    Data->_SetLineHeight( CurrAttrib.AsReal() );

                // Get the spacelength
                CurrAttrib = (*FontNode).GetAttribute("SpaceLength");
                if( !CurrAttrib.Empty() )
                    Data->_SetSpaceLength( CurrAttrib.AsReal() );

                // Get the baseline
                CurrAttrib = (*FontNode).GetAttribute("BaseLine");
                if( !CurrAttrib.Empty() )
                    Data->_SetBaseLine( CurrAttrib.AsReal() );

                // Get the letterspacing
                CurrAttrib = (*FontNode).GetAttribute("LetterSpacing");
                if( !CurrAttrib.Empty() )
                    Data->_SetLetterSpacing( CurrAttrib.AsReal() );

                // Get the monowidth
                /*CurrAttrib = (*FontNode).GetAttribute("MonoWidth");
                if( !CurrAttrib.Empty() )
                    Data->_SetMonoWidth( CurrAttrib.AsReal() );//*/

                // Get the start of the range
                /*CurrAttrib = (*FontNode).GetAttribute("RangeBegin");
                if( !CurrAttrib.Empty() )
                    Data->_SetRangeBegin( CurrAttrib.AsReal() );//*/

                // Get the end of the range
                /*CurrAttrib = (*FontNode).GetAttribute("RangeEnd");
                if( !CurrAttrib.Empty() )
                    Data->_SetRangeEnd( CurrAttrib.AsReal() );//*/

                // Get the glyphs and parse them
                XML::Node GlyphsNode = (*FontNode).GetChild("Glyphs");
                this->ParseGlyphs( GlyphsNode, Offset, Data );
                // Next parse the kernings for those glyphs
                XML::Node KerningsNode = (*FontNode).GetChild("Kernings");
                this->ParseKernings( KerningsNode, Data );
                // Lastly parse the vertical offsets
                XML::Node VerticalOffsetsNode = (*FontNode).GetChild("VerticalOffsets");
                this->ParseVerticalOffsets( VerticalOffsetsNode, Data );
            }
        }

        void TextureAtlas::ParseGlyphs(XML::Node& GlyphsNode, const Vector2& Offset, FontData* FontD)
        {
            XML::Attribute CurrAttrib;
            Ogre::RenderSystem* rs = Ogre::Root::getSingletonPtr()->getRenderSystem();

            Real TexelX = rs->getHorizontalTexelOffset();
            Real TexelY = rs->getVerticalTexelOffset();

            UInt32 ParsedGlyphID = 0;
            Real Left = 0, Top = 0, Right = 0, Bottom = 0;

            for( XML::NodeIterator GlyphIt = GlyphsNode.begin() ; GlyphIt != GlyphsNode.end() ; ++GlyphIt )
            {
                // Get the ID
                CurrAttrib = (*GlyphIt).GetAttribute("ID");
                if( !CurrAttrib.Empty() ) ParsedGlyphID = CurrAttrib.AsUint();
                else continue;

                // Get the left position
                CurrAttrib = (*GlyphIt).GetAttribute("PositionX");
                if( !CurrAttrib.Empty() ) Left = (Offset.X + CurrAttrib.AsReal()) - TexelX;
                else continue;

                // Get the top position
                CurrAttrib = (*GlyphIt).GetAttribute("PositionY");
                if( !CurrAttrib.Empty() ) Top = (Offset.Y + CurrAttrib.AsReal()) - TexelY;
                else continue;

                // Get the width
                CurrAttrib = (*GlyphIt).GetAttribute("SizeX");
                if( !CurrAttrib.Empty() ) Right = (Left + CurrAttrib.AsReal()) - TexelX;
                else continue;

                // Get the height
                CurrAttrib = (*GlyphIt).GetAttribute("SizeY");
                if( !CurrAttrib.Empty() ) Bottom = (Top + CurrAttrib.AsReal()) - TexelY;
                else continue;

                // If we got this far, we have enough data for a valid glyph, create and insert it
                Glyph* NewGlyph = new Glyph(FontD);
                NewGlyph->GlyphID = ParsedGlyphID;
                FontD->_AddGlyph(NewGlyph);

                // Convert the coordinates to relative
                Left   *= InverseTextureSize.X;
                Top    *= InverseTextureSize.Y;
                Right  *= InverseTextureSize.X;
                Bottom *= InverseTextureSize.Y;

                // Assign the values to the glyph
                NewGlyph->AtlasCoords[QC_TopLeft].SetValues(Left,Top);
                NewGlyph->AtlasCoords[QC_TopRight].SetValues(Right,Top);
                NewGlyph->AtlasCoords[QC_BottomRight].SetValues(Right,Bottom);
                NewGlyph->AtlasCoords[QC_BottomLeft].SetValues(Left,Bottom);

                // Get or generate the glyph advance
                CurrAttrib = (*GlyphIt).GetAttribute("Advance");
                if( !CurrAttrib.Empty() ) NewGlyph->GlyphAdvance = CurrAttrib.AsReal();
                else NewGlyph->GlyphAdvance = NewGlyph->GetWidth();
            }
        }

        void TextureAtlas::ParseKernings(XML::Node& KerningsNode, FontData* FontD)
        {
            XML::Attribute CurrAttrib;
            UInt32 LeftID = 0;
            UInt32 RightID = 0;
            Real Kerning = 0;

            for( XML::NodeIterator KerningNode = KerningsNode.begin() ; KerningNode != KerningsNode.end() ; ++KerningNode )
            {
                // Get the left ID
                CurrAttrib = (*KerningNode).GetAttribute("Left");
                if( !CurrAttrib.Empty() ) LeftID = CurrAttrib.AsReal();
                else continue;

                // Get the right ID
                CurrAttrib = (*KerningNode).GetAttribute("Right");
                if( !CurrAttrib.Empty() ) RightID = CurrAttrib.AsReal();
                else continue;

                // Get the kerning adjustment
                CurrAttrib = (*KerningNode).GetAttribute("Adjust");
                if( !CurrAttrib.Empty() ) Kerning = CurrAttrib.AsReal();
                else continue;

                FontD->GetGlyph(RightID)->Kernings.push_back(KerningInfo(LeftID,Kerning));
            }
        }

        void TextureAtlas::ParseVerticalOffsets(XML::Node& VerticalOffsetsNode, FontData* FontD)
        {
            XML::Attribute CurrAttrib;
            UInt32 GlyphID = 0;
            Real Offset = 0;

            for( XML::NodeIterator VerticalOffsetNode = VerticalOffsetsNode.begin() ; VerticalOffsetNode != VerticalOffsetsNode.end() ; ++VerticalOffsetNode )
            {
                // Get the glyph ID
                CurrAttrib = (*VerticalOffsetNode).GetAttribute("GlyphID");
                if( !CurrAttrib.Empty() ) GlyphID = CurrAttrib.AsUint();
                else continue;

                // Get the offset
                CurrAttrib = (*VerticalOffsetNode).GetAttribute("Offset");
                if( !CurrAttrib.Empty() ) Offset = CurrAttrib.AsReal();
                else continue;

                FontD->GetGlyph(GlyphID)->VerticalOffset = Offset;
            }
        }

        void TextureAtlas::ParseSprites(XML::Node& AtlasSpritesNode)
        {
            XML::Attribute CurrAttrib;
            Ogre::RenderSystem* rs = Ogre::Root::getSingletonPtr()->getRenderSystem();

            Real TexelX = rs->getHorizontalTexelOffset();
            Real TexelY = rs->getVerticalTexelOffset();

            String SpriteName;
            Real Left = 0, Top = 0, Right = 0, Bottom = 0;

            for( XML::NodeIterator SpriteIt = AtlasSpritesNode.begin() ; SpriteIt != AtlasSpritesNode.end() ; ++SpriteIt )
            {
                // Get the name
                CurrAttrib = (*SpriteIt).GetAttribute("Name");
                if( !CurrAttrib.Empty() ) SpriteName = CurrAttrib.AsString();
                else continue;

                // Get the left position
                CurrAttrib = (*SpriteIt).GetAttribute("PositionX");
                if( !CurrAttrib.Empty() ) Left = CurrAttrib.AsReal() - TexelX;
                else continue;

                // Get the top position
                CurrAttrib = (*SpriteIt).GetAttribute("PositionY");
                if( !CurrAttrib.Empty() ) Top = CurrAttrib.AsReal() - TexelY;
                else continue;

                // Get the width
                CurrAttrib = (*SpriteIt).GetAttribute("SizeX");
                if( !CurrAttrib.Empty() ) Right = (Left + CurrAttrib.AsReal()) - TexelX;
                else continue;

                // Get the height
                CurrAttrib = (*SpriteIt).GetAttribute("SizeY");
                if( !CurrAttrib.Empty() ) Bottom = (Top + CurrAttrib.AsReal()) - TexelY;
                else continue;

                // Convert the coordinates to relative
                Left   *= InverseTextureSize.X;
                Top    *= InverseTextureSize.Y;
                Right  *= InverseTextureSize.X;
                Bottom *= InverseTextureSize.Y;

                // If we got this far, we have enough data for a valid sprite, create and insert it
                Sprite* NewSprite = new Sprite(SpriteName,Top,Left,Bottom,Right);
                SpriteIterator SpIt = this->Sprites.find(SpriteName);
                if( SpIt == this->Sprites.end() ) {
                    this->Sprites.insert(std::pair<String,Sprite*>(SpriteName,NewSprite));
                }else{
                    StringStream ExceptionStream;
                    ExceptionStream << "Sprite named \"" << SpriteName << "\" already exists in Atlas: \"" << this->AtlasName << "\".";
                    MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,ExceptionStream.str());
                }
            }
        }

        void TextureAtlas::Create2DMaterial()
        {
            String MatName = "Mezz2D." + this->TAID->TATexture->getName();
            this->TAID->Mat2D = Ogre::MaterialManager::getSingletonPtr()->getByName(MatName);
            if(!this->TAID->Mat2D.isNull())
            {
                return;
            }

            this->TAID->Mat2D = GetOrCreate2DMasterMaterial()->clone(MatName);
            this->TAID->Pass2D = this->TAID->Mat2D->getTechnique(0)->getPass(0);
            this->TAID->Pass2D->getTextureUnitState(0)->setTextureName(this->TAID->TATexture->getName());
        }

        void TextureAtlas::Create3DMaterial()
        {
            String MatName = "Mezz3D." + this->TAID->TATexture->getName();
            this->TAID->Mat3D = Ogre::MaterialManager::getSingletonPtr()->getByName(MatName);
            if(!this->TAID->Mat3D.isNull())
            {
                return;
            }

            this->TAID->Mat3D = GetOrCreate3DMasterMaterial()->clone(MatName);
            this->TAID->Pass3D = this->TAID->Mat3D->getTechnique(0)->getPass(0);
            this->TAID->Pass3D->getTextureUnitState(0)->setTextureName(this->TAID->TATexture->getName());
        }

        Ogre::MaterialPtr TextureAtlas::GetOrCreate2DMasterMaterial()
        {
            Ogre::MaterialPtr Material2D = Ogre::MaterialManager::getSingletonPtr()->getByName("Mezz2D");
            if(Material2D.isNull() == false)
            {
                Ogre::Pass* MatPass = Material2D->getTechnique(0)->getPass(0);

                if(MatPass->hasVertexProgram())
                {
                    Ogre::GpuProgramPtr gpuPtr = MatPass->getVertexProgram();
                    gpuPtr->load();
                }

                if(MatPass->hasFragmentProgram())
                {
                    Ogre::GpuProgramPtr gpuPtr = MatPass->getFragmentProgram();
                    gpuPtr->load();
                }

                return Material2D;
            }

            Material2D = Ogre::MaterialManager::getSingletonPtr()->create("Mezz2D","UI");
            Ogre::Pass* MatPass = Material2D->getTechnique(0)->getPass(0);
            MatPass->setCullingMode(Ogre::CULL_NONE);
            MatPass->setDepthCheckEnabled(false);
            MatPass->setDepthWriteEnabled(false);
            MatPass->setLightingEnabled(false);
            MatPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

            Ogre::TextureUnitState* TexUnit = MatPass->createTextureUnitState();
            TexUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
            TexUnit->setTextureFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
            return Material2D;
        }

        Ogre::MaterialPtr TextureAtlas::GetOrCreate3DMasterMaterial()
        {
            Ogre::MaterialPtr Material3D = Ogre::MaterialManager::getSingletonPtr()->getByName("Mezz3D");
            if(Material3D.isNull() == false)
            {
                Ogre::Pass* MatPass = Material3D->getTechnique(0)->getPass(0);

                if(MatPass->hasVertexProgram())
                {
                    Ogre::GpuProgramPtr gpuPtr = MatPass->getVertexProgram();
                    gpuPtr->load();
                }

                if(MatPass->hasFragmentProgram())
                {
                    Ogre::GpuProgramPtr gpuPtr = MatPass->getFragmentProgram();
                    gpuPtr->load();
                }

                return Material3D;
            }

            Material3D = Ogre::MaterialManager::getSingletonPtr()->create("Mezz3D","UI");
            Ogre::Pass* MatPass = Material3D->getTechnique(0)->getPass(0);
            MatPass->setCullingMode(Ogre::CULL_NONE);
            MatPass->setDepthCheckEnabled(false);
            MatPass->setDepthWriteEnabled(false);
            MatPass->setLightingEnabled(false);
            MatPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

            Ogre::TextureUnitState* TexUnit = MatPass->createTextureUnitState();
            TexUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
            TexUnit->setTextureFiltering(Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC, Ogre::FO_ANISOTROPIC);
            return Material3D;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& TextureAtlas::GetName() const
        {
            return AtlasName;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Information Gathering

        FontData* TextureAtlas::GetFont(const String& FontName) const
        {
            ConstFontDataIterator it = Fonts.find(FontName);
            if (it == Fonts.end())
                return 0;
            return (*it).second;
        }

        TextureAtlas::FontDataContainer& TextureAtlas::GetFonts()
        {
            return Fonts;
        }

        Sprite* TextureAtlas::GetSprite(const String& Name) const
        {
            ConstSpriteIterator it = Sprites.find(Name);
            if (it == Sprites.end())
                return NULL;
            return (*it).second;
        }

        TextureAtlas::SpriteContainer& TextureAtlas::GetSprites()
            { return Sprites; }

        Vector2 TextureAtlas::GetWhitePixel() const
            { return WhitePixel; }

        Real TextureAtlas::GetWhitePixelX() const
            { return WhitePixel.X; }

        Real TextureAtlas::GetWhitePixelY() const
            { return WhitePixel.Y; }

        Vector2 TextureAtlas::GetTextureSize() const
            { return Vector2( Real( this->TAID->TATexture->getWidth() ), Real( this->TAID->TATexture->getHeight() ) ); }

        Real TextureAtlas::GetInvTextureCoordsX() const
            { return 1.0 / Real(this->TAID->TATexture->getWidth()); }

        Real TextureAtlas::GetInvTextureCoordsY() const
            { return 1.0 / Real(this->TAID->TATexture->getHeight()); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions

        Ogre::MaterialPtr TextureAtlas::_GetOrCreate2DMaterial()
        {
            if(this->TAID->Mat2D.isNull()) {
                Create2DMaterial();
            }
            return this->TAID->Mat2D;
        }

        Ogre::MaterialPtr TextureAtlas::_GetOrCreate3DMaterial()
        {
            if(this->TAID->Mat3D.isNull()) {
                Create3DMaterial();
            }
            return this->TAID->Mat3D;
        }

        Ogre::TexturePtr TextureAtlas::_GetTexture()
            { return this->TAID->TATexture; }

        Ogre::Pass* TextureAtlas::_Get2DPass() const
            { return this->TAID->Pass2D; }

        Ogre::Pass* TextureAtlas::_Get3DPass() const
            { return this->TAID->Pass3D; }
    }//UI
}//Mezzanine

#endif
