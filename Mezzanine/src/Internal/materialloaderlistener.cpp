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
#ifndef _internalmaterialloaderlistener_cpp
#define _internalmaterialloaderlistener_cpp

// Keeps this file form being documented by doxygen
/// @cond DontDocumentInternal

#include "Internal/materialloaderlistener.h.cpp"
#include "Internal/iostreamwrapper.h.cpp"

#include <OgreTextureManager.h>
#include <OgreCodec.h>
#include <OgreImageCodec.h>

#include "Resource/resourcemanager.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Internal
    {
        MaterialLoaderListener::MaterialLoaderListener()
            {  }

        MaterialLoaderListener::~MaterialLoaderListener()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Callbacks

        bool MaterialLoaderListener::handleEvent(Ogre::ScriptCompiler* compiler, Ogre::ScriptCompilerEvent* evt, void* retval)
        {
            // I tried writing this without the typedef, it got old fast.
            typedef Ogre::ProcessResourceNameScriptCompilerEvent ListeningEventType;
            if( evt->mType == ListeningEventType::eventType ) {
                ListeningEventType* Casted = static_cast<ListeningEventType*>(evt);
                if( Casted->mResourceType == ListeningEventType::TEXTURE ) {
                    String GroupName = compiler->getResourceGroup();

                    // Verify the texture doesn't already exist.
                    Ogre::TexturePtr NewTexture = Ogre::TextureManager::getSingletonPtr()->getByName(Casted->mName,GroupName);
                    if( NewTexture.isNull() ) {
                        // Get our stream and make it usable to Ogre.
                        Resource::DataStreamPtr TextureStream = Resource::ResourceManager::GetSingletonPtr()->OpenAssetStream(Casted->mName,GroupName);
                        Ogre::DataStreamPtr TextureWrapper(new IOStreamWrapper(TextureStream.get(),false));

                        // Textures are a bit more complicated given they don't have a convenient serialzied class like Skeletons and Meshes.
                        // Maybe because there isn't an official Ogre texture format, which is probably for the best.
                        // So we gotta jump through some hoops to get at the information we need, then fallback to the extension should that fail.
                        Ogre::Codec* TextureCodec = NULL;

                        size_t MagicNumLength = std::min(TextureStream->GetSize(),Resource::StreamSize(32));
                        char MagicBuffer[32];
                        TextureStream->Read(MagicBuffer,MagicNumLength);
                        // Reset the read position so we don't mess up the decode.
                        TextureStream->SetStreamPosition(0);
                        TextureCodec = Ogre::Codec::getCodec(MagicBuffer,MagicNumLength);

                        // Here's our fallback.  Infer from the extension, while technically easier, it makes the system less idiot proof to do first.
                        if( TextureCodec == NULL ) {
                            String TextureExtension = Casted->mName.substr(Casted->mName.find_last_of("."));
                            TextureCodec = Ogre::Codec::getCodec(TextureExtension);
                        }

                        // If we're still not here, it's unsupported.
                        if( TextureCodec == NULL ) {
                            MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,"Texture being loaded does not use a supported Codec.");
                        }

                        // Ok, finally lets decode.
                        Ogre::Codec::DecodeResult Result = TextureCodec->decode(TextureWrapper);
                        // Woo!  Cast the image data and pass it along to the texture manager to make our texture.
                        Ogre::ImageCodec::ImageData* TexData = static_cast<Ogre::ImageCodec::ImageData*>(Result.second.getPointer());
                        Ogre::DataStreamPtr CastedTexStream = Result.first;
                        Ogre::TextureManager::getSingletonPtr()->loadRawData(Casted->mName,GroupName,CastedTexStream,TexData->width,TexData->height,TexData->format,( TexData->depth == 1 ? Ogre::TEX_TYPE_2D : Ogre::TEX_TYPE_3D ),TexData->num_mipmaps);
                    }
                }
            }
            return false;
        }
    }//Internal
}//Mezzanine

/// @endcond

#endif
