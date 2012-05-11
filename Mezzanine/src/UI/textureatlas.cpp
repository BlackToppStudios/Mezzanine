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
#ifndef _uitextureatlas_cpp
#define _uitextureatlas_cpp

#include "UI/textureatlas.h"
#include "UI/glyph.h"
#include "UI/sprite.h"
#include "stringtool.h"

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
        TextureAtlas::TextureAtlas(const String& MTAFile, const String& Group)
        {
            this->TAID = new TextureAtlasInternalData();
            Load(MTAFile,Group);
            CalculateCoordinates();
            Create2DMaterial();
            Create3DMaterial();
            MarkupColours.push_back(ColourValue::White());
        }

        TextureAtlas::~TextureAtlas()
        {
            delete TAID;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Private & Setup Functions

        void TextureAtlas::Load(const String& MTAFile, const String& Group)
        {
            Mezzanine::Resource::TextSettingsFile MTA;
            MTA.Load(MTAFile,Group);
            Mezzanine::Resource::TextSettingsFile::SectionIterator SecIt = MTA.GetSectionBegin();
            Mezzanine::Resource::TextSettingsFile::SectionIterator SecEnd = MTA.GetSectionEnd();
            String AtlasName = MTAFile.substr(0,MTAFile.find_first_of("."));
            String SectionName;

            SecIt++;
            while(SecIt != SecEnd)
            {
                SectionName = (*SecIt).first;
                StringTool::ToLowerCase(SectionName);
                Mezzanine::Resource::TextSettingsFile::SettingsMap* CurrSection = (*SecIt).second;

                if(SectionName == "texture")
                    LoadTexture(CurrSection);
                else if(StringTool::StartsWith(SectionName,"font.",false))
                {
                    UInt32 Index = StringTool::ConvertToUInt32(SectionName.substr(5));
                    GlyphData* GlyphSet = new GlyphData();
                    Glyphs[Index] = GlyphSet;

                    LoadGlyphs(CurrSection, GlyphSet);
                    LoadKerning(CurrSection, GlyphSet);
                    LoadVerticalOffsets(CurrSection, GlyphSet);
                    GlyphSet->Atlas = AtlasName;
                }
                else if(SectionName == "sprites")
                    LoadSprites(CurrSection,AtlasName);
                SecIt++;
            }
        }

        void TextureAtlas::LoadTexture(Resource::TextSettingsFile::SettingsMap* Config)
        {
            String Name, Data;
            Resource::TextSettingsFile::SettingsIterator It;
            for(It = Config->begin() ; It != Config->end() ; ++It)
            {
                Name = It->first;
                Data = It->second;

                size_t Comment = Data.find_first_of('#');
                if(Comment != String::npos)
                    Data = Data.substr(0,Comment);

                StringTool::ToLowerCase(Name);

                // file texturename ~groupname
                // >  file myatlas.png
                // >  file myatlas.png ~loadinggroup
                if(Name == "file")
                {
                    String TextureName = Data;
                    String GroupName = "UI";
                    size_t GroupSplit = Data.find_first_of('~');
                    if(GroupSplit != String::npos)
                    {
                        TextureName = Data.substr(0,GroupSplit);
                        GroupName = Data.substr(GroupSplit+1);
                        StringTool::Trim(TextureName);
                        StringTool::Trim(GroupName);
                    }
                    this->TAID->TATexture = Ogre::TextureManager::getSingletonPtr()->getByName(Data,GroupName);
                    if(this->TAID->TATexture.isNull())
                    {
                        this->TAID->TATexture = Ogre::TextureManager::getSingletonPtr()->load(TextureName,GroupName,Ogre::TEX_TYPE_2D,0);
                    }

                    InverseTextureSize.X = 1.0 / this->TAID->TATexture->getWidth();
                    InverseTextureSize.Y = 1.0 / this->TAID->TATexture->getHeight();

                    continue;
                }
                else if(Name == "whitepixel")
                {
                    WhitePixel = StringTool::ConvertToVector2(Data);
                    WhitePixel.X *= InverseTextureSize.X;
                    WhitePixel.Y *= InverseTextureSize.Y;
                }
            }
        }

        void TextureAtlas::LoadGlyphs(Resource::TextSettingsFile::SettingsMap* Config, GlyphData* GlyphD)
        {
            String Name, Data;
            Resource::TextSettingsFile::SettingsIterator It;

            std::vector<String> StrValues;
            Vector2 Offset;
            for(It = Config->begin() ; It != Config->end() ; ++It)
            {
                Name = It->first;
                Data = It->second;

                size_t Comment = Data.find_first_of('#');
                if(Comment != String::npos)
                    Data = Data.substr(0, Comment);
                StringTool::ToLowerCase(Name);

                // lineheight x y
                // >  offset 0 80
                if (Name == "offset")
                {
                    Offset = StringTool::ConvertToVector2(Data);
                    continue;
                }
                // lineheight lineheight
                // >  lineheight 15
                else if (Name == "lineheight")
                {
                    GlyphD->LineHeight = StringTool::ConvertToReal(Data);
                    continue;
                }
                // spacelength spacelength
                // >  spacelength 3
                else if (Name == "spacelength")
                {
                    GlyphD->SpaceLength = StringTool::ConvertToReal(Data);
                    continue;
                }
                // spacelength spacelength
                // >  spacelength 3
                else if (Name == "baseline")
                {
                    GlyphD->Baseline = StringTool::ConvertToReal(Data);
                    continue;
                }
                // monowidth width
                // >  monowidth 3
                else if (Name == "monowidth")
                {
                    GlyphD->MonoWidth = StringTool::ConvertToReal(Data);
                    continue;
                }
                // range lower upper
                // >  range 33 126
                else if (Name == "range")
                {
                    Vector2 Range = StringTool::ConvertToVector2(Data);
                    GlyphD->RangeBegin = (UInt32)Range.X;
                    GlyphD->RangeEnd = (UInt32)Range.Y;
                }
                // kerning kerning
                // >  kerning -1
                else if(Name == "letterspacing")
                {
                    GlyphD->LetterSpacing = StringTool::ConvertToReal(Data);
                }
            }

            for(UInt32 Index = GlyphD->RangeBegin ; Index <= GlyphD->RangeEnd ; Index++)
            {
                Glyph* NewGlyph = new Glyph();
                GlyphD->Glyphs.push_back(NewGlyph);

                std::stringstream GlyphStr;
                GlyphStr << "glyph_" << Index;

                It = Config->find(GlyphStr.str());
                if(It == Config->end())
                    continue;

                StrValues = StringTool::Split((*It).second," ",5);

                if(StrValues.size() < 4)
                {
                    continue;
                }

                NewGlyph->UVLeft    = Offset.X + StringTool::ConvertToReal( StrValues[0] );
                NewGlyph->UVTop     = Offset.Y + StringTool::ConvertToReal( StrValues[1] );
                NewGlyph->UVWidth   = StringTool::ConvertToReal( StrValues[2] );
                NewGlyph->UVHeight  = StringTool::ConvertToReal( StrValues[3] );
                NewGlyph->UVRight   = NewGlyph->UVLeft + NewGlyph->UVWidth;
                NewGlyph->UVBottom  = NewGlyph->UVTop + NewGlyph->UVHeight;

                if(StrValues.size() == 5) NewGlyph->GlyphAdvance = StringTool::ConvertToReal( StrValues[4] );
                else NewGlyph->GlyphAdvance = NewGlyph->UVWidth;

                NewGlyph->GlyphChar = (GlyphD->Glyphs.size() - 1) + GlyphD->RangeBegin;
            }
        }

        void TextureAtlas::LoadKerning(Resource::TextSettingsFile::SettingsMap* Config, GlyphData* GlyphD)
        {
            String LeftName, Data;
            Resource::TextSettingsFile::SettingsIterator It;
            UInt32 LeftGlyphID;
            UInt32 RightGlyphID;
            Real Kern;
            std::vector<String> StrValues;

            for(It = Config->begin(); It != Config->end(); ++It)
            {
                LeftName = It->first;
                Data = It->second;
                StringTool::ToLowerCase(LeftName);

                if(LeftName.substr(0,6) != "kerning_")
                    continue;

                size_t Comment = Data.find_first_of('#');
                if(Comment != String::npos)
                    Data = Data.substr(0,Comment);

                LeftName = LeftName.substr(8); // chop of kerning_
                LeftGlyphID = StringTool::ConvertToUInt32(LeftName);
                StrValues = StringTool::Split(Data," ",2);

                if(StrValues.size() != 2)
                {
                    continue;
                }

                RightGlyphID = StringTool::ConvertToUInt32(StrValues[0]);
                Kern = StringTool::ConvertToReal(StrValues[1]);
                GlyphD->Glyphs[RightGlyphID - GlyphD->RangeBegin]->Kernings.push_back(KerningInfo(LeftGlyphID,Kern));
            }
        }

        void TextureAtlas::LoadVerticalOffsets(Resource::TextSettingsFile::SettingsMap* Config, GlyphData* GlyphD)
        {
            String LeftName, Data;
            Resource::TextSettingsFile::SettingsIterator It;
            UInt32 GlyphID;
            Real VerticalOffset;

            for (It = Config->begin() ; It != Config->end() ; ++It)
            {
                LeftName = It->first;
                Data = It->second;
                StringTool::ToLowerCase(LeftName);

                if(LeftName.substr(0,15) != "verticaloffset_")
                    continue;

                size_t Comment = Data.find_first_of('#');
                if(Comment != String::npos)
                    Data = Data.substr(0, Comment);

                LeftName = LeftName.substr(15); // chop of VerticalOffset_
                GlyphID = StringTool::ConvertToUInt32(LeftName);
                VerticalOffset = StringTool::ConvertToReal(Data);
                GlyphD->GetGlyph(GlyphID)->VerticalOffset = VerticalOffset;
            }
        }

        void TextureAtlas::LoadSprites(Resource::TextSettingsFile::SettingsMap* Config,const String& Atlas)
        {
            String SpriteName, Data;
            Resource::TextSettingsFile::SettingsIterator It;

            std::vector<String> StrValues;
            for(It = Config->begin() ; It != Config->end() ; ++It)
            {
                SpriteName = It->first;
                Data = It->second;

                size_t Comment = Data.find_first_of('#');
                if(Comment != String::npos)
                    Data = Data.substr(0, Comment);

                StrValues = StringTool::Split(Data," ",4);

                if(StrValues.size() != 4)
                    continue;

                Sprite* NewSprite = new Sprite();

                NewSprite->UVLeft = StringTool::ConvertToUInt32(StrValues[0]);
                NewSprite->UVTop = StringTool::ConvertToUInt32(StrValues[1]);
                NewSprite->SpriteSize.X = StringTool::ConvertToUInt32(StrValues[2]);
                NewSprite->SpriteSize.Y = StringTool::ConvertToUInt32(StrValues[3]);
                NewSprite->Atlas = Atlas;
                Sprites[SpriteName] = NewSprite;
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
        }

        void TextureAtlas::CalculateCoordinates()
        {
            Ogre::RenderSystem* rs = Ogre::Root::getSingletonPtr()->getRenderSystem();

            Real TexelX = rs->getHorizontalTexelOffset();
            Real TexelY = rs->getVerticalTexelOffset();

            for(std::map<UInt32, GlyphData*>::iterator gd_it = Glyphs.begin(); gd_it != Glyphs.end(); gd_it++)
            {
                for(std::vector<Glyph*>::iterator it = (*gd_it).second->Glyphs.begin(); it != (*gd_it).second->Glyphs.end(); it++)
                {
                    (*it)->UVLeft        -= TexelX;
                    (*it)->UVTop         -= TexelY;
                    (*it)->UVRight       += TexelX;
                    (*it)->UVBottom      += TexelY;

                    (*it)->UVLeft        *= InverseTextureSize.X;
                    (*it)->UVTop         *= InverseTextureSize.Y;
                    (*it)->UVRight       *= InverseTextureSize.X;
                    (*it)->UVBottom      *= InverseTextureSize.Y;

                    (*it)->AtlasCoords[QC_TopLeft].X = (*it)->UVLeft;
                    (*it)->AtlasCoords[QC_TopLeft].Y = (*it)->UVTop;
                    (*it)->AtlasCoords[QC_TopRight].X = (*it)->UVRight;
                    (*it)->AtlasCoords[QC_TopRight].Y = (*it)->UVTop;
                    (*it)->AtlasCoords[QC_BottomRight].X = (*it)->UVRight;
                    (*it)->AtlasCoords[QC_BottomRight].Y = (*it)->UVBottom;
                    (*it)->AtlasCoords[QC_BottomLeft].X = (*it)->UVLeft;
                    (*it)->AtlasCoords[QC_BottomLeft].Y = (*it)->UVBottom;

                    (*it)->GlyphSize.X    = (*it)->UVWidth;
                    (*it)->GlyphSize.Y    = (*it)->UVHeight;
                }
            }

            for(std::map<String, Sprite*>::iterator it = Sprites.begin(); it != Sprites.end(); it++)
            {
               (*it).second->UVRight    = (*it).second->UVLeft + (*it).second->SpriteSize.X;
               (*it).second->UVBottom   = (*it).second->UVTop  + (*it).second->SpriteSize.Y;

               (*it).second->UVLeft    *= InverseTextureSize.X;
               (*it).second->UVTop     *= InverseTextureSize.Y;
               (*it).second->UVRight   *= InverseTextureSize.X;
               (*it).second->UVBottom  *= InverseTextureSize.Y;

               (*it).second->AtlasCoords[QC_TopLeft].X = (*it).second->UVLeft;
               (*it).second->AtlasCoords[QC_TopLeft].Y = (*it).second->UVTop;
               (*it).second->AtlasCoords[QC_TopRight].X = (*it).second->UVRight;
               (*it).second->AtlasCoords[QC_TopRight].Y = (*it).second->UVTop;
               (*it).second->AtlasCoords[QC_BottomRight].X = (*it).second->UVRight;
               (*it).second->AtlasCoords[QC_BottomRight].Y = (*it).second->UVBottom;
               (*it).second->AtlasCoords[QC_BottomLeft].X = (*it).second->UVLeft;
               (*it).second->AtlasCoords[QC_BottomLeft].Y = (*it).second->UVBottom;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Information Gathering

        GlyphData* TextureAtlas::GetGlyphData(const UInt32& ID) const
        {
            std::map<UInt32, GlyphData*>::const_iterator it = Glyphs.find(ID);
            if (it == Glyphs.end())
                return 0;
            return (*it).second;
        }

        std::map<UInt32, GlyphData*>& TextureAtlas::GetGlyphs()
        {
            return Glyphs;
        }

        Sprite* TextureAtlas::GetSprite(const String& Name) const
        {
            std::map<String, Sprite*>::const_iterator it = Sprites.find(Name);
            if (it == Sprites.end())
                return 0;
            return (*it).second;
        }

        std::map<String, Sprite*>& TextureAtlas::GetSprites()
        {
            return Sprites;
        }

        Vector2 TextureAtlas::GetWhitePixel() const
        {
            return WhitePixel;
        }

        Real TextureAtlas::GetWhitePixelX() const
        {
            return WhitePixel.X;
        }

        Real TextureAtlas::GetWhitePixelY() const
        {
            return WhitePixel.Y;
        }

        Vector2 TextureAtlas::GetTextureSize() const
        {
            Vector2 Vec(Real(this->TAID->TATexture->getWidth()),Real(this->TAID->TATexture->getHeight()));
            return Vec;
        }

        Real TextureAtlas::GetInvTextureCoordsX() const
        {
            return 1.0 / Real(this->TAID->TATexture->getWidth());
        }

        Real TextureAtlas::GetInvTextureCoordsY() const
        {
            return 1.0 / Real(this->TAID->TATexture->getHeight());
        }

        void TextureAtlas::SetMarkupColour(const UInt32& Index, const ColourValue& Colour)
        {
            if(Index < MarkupColours.size())
            {
                MarkupColours[Index] = Colour;
            }else{
                MarkupColours.push_back(Colour);
            }
        }

        ColourValue TextureAtlas::GetMarkupColour(const UInt32& Index) const
        {
            if(Index > MarkupColours.size())
                return ColourValue::White();
            return MarkupColours[Index];
        }

        void TextureAtlas::ResetMarkupColours()
        {
            MarkupColours.clear();
            MarkupColours.push_back(ColourValue::White());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions

        Ogre::MaterialPtr TextureAtlas::_GetOrCreate2DMaterial()
        {
            if(this->TAID->Mat2D.isNull())
            {
                Create2DMaterial();
            }
            return this->TAID->Mat2D;
        }

        Ogre::MaterialPtr TextureAtlas::_GetOrCreate3DMaterial()
        {
            if(this->TAID->Mat3D.isNull())
            {
                Create3DMaterial();
            }
            return this->TAID->Mat3D;
        }

        Ogre::TexturePtr TextureAtlas::_GetTexture()
        {
            return this->TAID->TATexture;
        }

        Ogre::Pass* TextureAtlas::_Get2DPass() const
        {
            return this->TAID->Pass2D;
        }

        Ogre::Pass* TextureAtlas::_Get3DPass() const
        {
            return this->TAID->Pass3D;
        }
    }//UI
}//Mezzanine

#endif
