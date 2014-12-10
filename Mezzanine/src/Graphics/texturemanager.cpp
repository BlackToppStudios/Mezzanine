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
#ifndef _graphicstexturemanager_cpp
#define _graphicstexturemanager_cpp

#include "Graphics/texturemanager.h"
#include "Graphics/texture.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> Graphics::TextureManager* Singleton<Graphics::TextureManager>::SingletonPtr = NULL;

    namespace Graphics
    {
        TextureManager::TextureManager()
            {  }

        TextureManager::TextureManager(XML::Node& XMLNode)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        TextureManager::~TextureManager()
        {
            this->Deinitialize();
            this->UnloadAllTextures();
        }

        void TextureManager::AddTexture(Texture* ToAdd)
        {
            String TextureName = ToAdd->GetName();
            TextureIterator TextureIt = this->Textures.find( TextureName );
            if( TextureIt == this->Textures.end() ) {
                this->Textures.insert( std::pair<String,Texture*>(TextureName,ToAdd) );
            }else{
                MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Textures must have unique names when loaded!");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Manual Creation

        Texture* TextureManager::CreateManualTexture(const String& ResourceName, const String& ResourceGroup, const Graphics::TextureType Type, const Whole Width, const Whole Height,
                                                     const Integer NumMipMaps, const Graphics::PixelFormat Format, const Whole Usage, const Boole HWGammaCorrect, const Whole FSAA)
        {
            return this->_WrapInternalTexture( this->_GetInternalManager()->createManual(ResourceName,ResourceGroup,static_cast<Ogre::TextureType>(Type),Width,Height,NumMipMaps,static_cast<Ogre::PixelFormat>(Format),Usage,NULL,HWGammaCorrect,FSAA) );
        }

        Texture* TextureManager::CreateManualTexture(const String& ResourceName, const String& ResourceGroup, const Graphics::TextureType Type, const Whole Width, const Whole Height, const Whole Depth,
                                                     const Integer NumMipMaps, const Graphics::PixelFormat Format, const Whole Usage, const Boole HWGammaCorrect, const Whole FSAA)
        {
            return this->_WrapInternalTexture( this->_GetInternalManager()->createManual(ResourceName,ResourceGroup,static_cast<Ogre::TextureType>(Type),Width,Height,Depth,NumMipMaps,static_cast<Ogre::PixelFormat>(Format),Usage,NULL,HWGammaCorrect,FSAA) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Texture Management

        Texture* TextureManager::LoadTexture(const String& TextureName, const String& Group)
        {
            TextureIterator TexIt = this->Textures.find(TextureName);
            if( TexIt != this->Textures.end() ) {
                return (*TexIt).second;
            }
            return this->_WrapInternalTexture( this->_GetInternalManager()->load(TextureName,Group) );
        }

        void TextureManager::UnloadTexture(const String& TextureName)
        {
            TextureIterator TexIt = this->Textures.find(TextureName);
            if( TexIt == this->Textures.end() ) {
                return;
            }
            this->_GetInternalManager()->unload(TextureName);
            delete (*TexIt).second;
            this->Textures.erase(TexIt);
        }

        Texture* TextureManager::GetTexture(const String& TextureName)
        {
            TextureIterator TexIt = this->Textures.find(TextureName);
            if( TexIt != this->Textures.end() ) {
                return (*TexIt).second;
            }
            return NULL;
        }

        Whole TextureManager::GetNumTextures()
        {
            return this->Textures.size();
        }

        void TextureManager::UnloadAllTextures()
        {
            for( TextureIterator TexIt = this->Textures.begin() ; TexIt != this->Textures.end() ; ++TexIt )
                { delete (*TexIt).second; }
            this->Textures.clear();
            this->_GetInternalManager()->unloadAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void TextureManager::Initialize()
            { this->Initialized = true; }

        void TextureManager::Deinitialize()
            { this->Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType TextureManager::GetInterfaceType() const
            { return ManagerBase::MT_TextureManager; }

        String TextureManager::GetImplementationTypeName() const
            { return "DefaultTextureManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Texture* TextureManager::_WrapInternalTexture(Ogre::TexturePtr ToWrap)
        {
            Texture* Wrapped = new Texture(ToWrap);
            this->AddTexture( Wrapped );
            return Wrapped;
        }

        Ogre::TextureManager* TextureManager::_GetInternalManager() const
            { return Ogre::TextureManager::getSingletonPtr(); }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultTextureManagerFactory Methods

        DefaultTextureManagerFactory::DefaultTextureManagerFactory()
            {  }

        DefaultTextureManagerFactory::~DefaultTextureManagerFactory()
            {  }

        String DefaultTextureManagerFactory::GetManagerTypeName() const
            { return "DefaultTextureManager"; }

        ManagerBase* DefaultTextureManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if( TextureManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return TextureManager::GetSingletonPtr();
            }else return new TextureManager();
        }

        ManagerBase* DefaultTextureManagerFactory::CreateManager(XML::Node& XMLNode)
        {
            if( TextureManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return TextureManager::GetSingletonPtr();
            }else return new TextureManager(XMLNode);
        }

        void DefaultTextureManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Graphics
}//Mezzanine


#endif
