// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _resourceassetgroup_cpp
#define _resourceassetgroup_cpp

#include "Resource/assetgroup.h"
#include "memorystream.h"
#include "filestream.h"

#include <OgreResourceGroupManager.h>
#include <OgreDataStream.h>

namespace Mezzanine
{
    namespace Resource
    {
        AssetGroup::AssetGroup(const String& GroupName) :
            Name(GroupName)
        {
            Ogre::ResourceGroupManager::getSingletonPtr()->createResourceGroup(this->Name);
        }

        AssetGroup::~AssetGroup()
        {
            Ogre::ResourceGroupManager::getSingletonPtr()->destroyResourceGroup(this->Name);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& AssetGroup::GetName() const
        {
            return this->Name;
        }

        void AssetGroup::InitializeAssets()
        {
            Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup(this->Name);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Management

        DataStreamPtr AssetGroup::OpenAssetStream(const String& AssetName)
        {
            /// @todo This entire method is a bit of a hack.  When the resource system gets refactored it should go through our archives or whatever equivalent.
            /// Since we currently have to put up with Ogre's system, we'll use it for now as a hack.

            AssetIterator StreamIt = this->GroupAssets.find(AssetName);
            if( StreamIt != this->GroupAssets.end() )
                return (*StreamIt).second;

            Ogre::DataStreamPtr OgreStream = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(AssetName,this->Name);
            Char8* AssetBuffer = new Char8[ OgreStream->size() ];
            OgreStream->read( (void*)AssetBuffer, OgreStream->size() );

            DataStreamPtr Ret = this->CreateDataStream(AssetName,AssetBuffer,OgreStream->size());
            return Ret;
        }

        DataStreamPtr AssetGroup::CreateDataStream(void* Buffer, const UInt32 BufferSize)
        {
            DataStreamPtr NewStream( new MemoryStream(String(),Buffer,BufferSize,true) );
            this->UnnamedGroupAssets.push_back(NewStream);
            return NewStream;
        }

        DataStreamPtr AssetGroup::CreateDataStream(const String& AssetName, void* Buffer, const UInt32 BufferSize)
        {
            DataStreamPtr NewStream( new MemoryStream(AssetName,Buffer,BufferSize,true) );
            this->GroupAssets.insert(std::pair<String,DataStreamPtr>(AssetName,NewStream));
            return NewStream;
        }
    }//Resource
}//Mezzanine

#endif
