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
#ifndef _resourcemanager_cpp
#define _resourcemanager_cpp

#include "Resource/resourcemanager.h"
#include "Resource/resourceenumerations.h"
#include "Resource/filestream.h"
#include "Resource/memorystream.h"
#include "Resource/assetgroup.h"

#include "Graphics/meshmanager.h"

#include "stringtool.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

#include <Ogre.h>
//#include <btBulletWorldImporter.h>
//#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    template<> Resource::ResourceManager* Singleton<Resource::ResourceManager>::SingletonPtr = NULL;

    namespace Resource
    {
        const String ResourceManager::ImplementationName = "DefaultResourceManager";
        const ManagerBase::ManagerType ResourceManager::InterfaceType = ManagerBase::MT_ResourceManager;

        ResourceManager::ResourceManager(const String& EngineDataPath, const Mezzanine::ArchiveType ArchType)
        {
            this->OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
            this->EngineDataDir = EngineDataPath;
            this->AddAssetLocation(EngineDataPath, ArchType, "EngineData", false);
            this->CreateAssetGroup("");
        }

        ResourceManager::ResourceManager(const XML::Node& XMLNode)
        {
            this->OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
            this->CreateAssetGroup("");
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        ResourceManager::~ResourceManager()
        {
            this->Deinitialize();
            this->DestroyAllAssetGroups();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Management

        Resource::DataStreamPtr ResourceManager::OpenAssetStream(const String& AssetName, const String& GroupName)
            { return this->GetAssetGroupExcept(GroupName)->OpenAssetStream(AssetName); }

        Resource::DataStreamPtr ResourceManager::CreateDataStream(void* Buffer, const UInt32 BufferSize)
            { return this->GetAssetGroupExcept("")->CreateDataStream(Buffer,BufferSize); }

        Resource::DataStreamPtr ResourceManager::CreateDataStream(const String& AssetName, void* Buffer, const UInt32 BufferSize)
            { return this->GetAssetGroupExcept("")->CreateDataStream(AssetName,Buffer,BufferSize); }

        Resource::DataStreamPtr ResourceManager::CreateDataStream(const String& AssetName, const String& GroupName, void* Buffer, const UInt32 BufferSize)
            { return this->GetAssetGroupExcept(GroupName)->CreateDataStream(AssetName,Buffer,BufferSize); }

        ///////////////////////////////////////////////////////////////////////////////
        // AssetGroup Management

        void ResourceManager::AddAssetLocation(const String& Location, const ArchiveType Type, const String& Group, Boole Recursive)
        {
            this->GetOrCreateAssetGroup(Group);
            this->OgreResource->addResourceLocation(Location, this->GetStringFromArchiveType(Type), Group, Recursive);
        }

        AssetGroup* ResourceManager::CreateAssetGroup(const String& GroupName)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.find(GroupName);
            if( GroupIt != this->AssetGroups.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"An AssetGroup with the name \"" + GroupName + "\" already exists.");
            }

            AssetGroup* NewGroup = new AssetGroup(GroupName);
            this->AssetGroups[GroupName] = NewGroup;
            return NewGroup;
        }

        AssetGroup* ResourceManager::GetOrCreateAssetGroup(const String& GroupName)
        {
            AssetGroup* Ret = this->GetAssetGroup(GroupName);
            if( Ret == NULL ) {
                Ret = this->CreateAssetGroup(GroupName);
            }
            return Ret;
        }

        AssetGroup* ResourceManager::GetAssetGroup(const String& GroupName)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.find(GroupName);
            if( GroupIt != this->AssetGroups.end() ) {
                return (*GroupIt).second;
            }
            return NULL;
        }

        AssetGroup* ResourceManager::GetAssetGroupExcept(const String& GroupName)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.find(GroupName);
            if( GroupIt == this->AssetGroups.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"AssetGroup named \"" + GroupName + "\" not found.");
            }
            return (*GroupIt).second;
        }

        void ResourceManager::DestroyAssetGroup(const String& GroupName)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.find(GroupName);
            if( GroupIt != this->AssetGroups.end() ) {
                delete (*GroupIt).second;
                this->AssetGroups.erase(GroupIt);
            }
        }

        void ResourceManager::DestroyAssetGroup(AssetGroup* ToBeDestroyed)
        {
            this->DestroyAssetGroup( ToBeDestroyed->GetName() );
        }

        void ResourceManager::DestroyAllAssetGroups()
        {
            for( AssetGroupIterator GroupIt = this->AssetGroups.begin() ; GroupIt != this->AssetGroups.end() ; ++GroupIt )
                { delete (*GroupIt).second; }
            this->AssetGroups.clear();
        }

        void ResourceManager::InitAssetGroup(const String& GroupName)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.find(GroupName);
            if( GroupIt != this->AssetGroups.end() ) {
                (*GroupIt).second->InitializeAssets();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Resource/Asset Query

        String ResourceManager::GetAssetPath(const String& FileName, const String& Group)
        {
            Ogre::FileInfoListPtr FileList = this->OgreResource->listResourceFileInfo(Group);
            for( Whole X = 0 ; X < FileList->size() ; ++X )
            {
                if( FileName == FileList->at(X).filename ) {
                    //return FileList->at(X).path;
                    return FileList->at(X).archive->getName() + "/" + FileList->at(X).path;
                }
            }
            return "";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        String ResourceManager::GetEngineDataDirectory() const
        {
            return EngineDataDir;
        }

        String ResourceManager::GetPluginExtension() const
        {
            #ifdef MEZZ_WINDOWS
            return ".dll";
            #elif MEZZ_LINUX
            return ".so";
            #elif MEZZ_MACOSX
            return ".dylib";
            #endif
        }

        void ResourceManager::Initialize()
        {
            Initialized = true;
        }

        void ResourceManager::Deinitialize()
        {
            Initialized = false;
        }

        String ResourceManager::GetStringFromArchiveType(const Mezzanine::ArchiveType ArchType)
        {
            switch(ArchType)
            {
                case Mezzanine::AT_FileSystem:
                    return String("FileSystem");
                case Mezzanine::AT_Zip:
                    return String("Zip");
                default:
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION, "Invalid archive type passed to ResourceManager::GetStringFromArchiveType.");
                    return String("");
            }
        }

        Mezzanine::ArchiveType ResourceManager::GetArchiveTypeFromString(const String& FromString)
        {
            if(String("FileSystem")==FromString)
                { return Mezzanine::AT_FileSystem; }
            if(String("Zip")==FromString)
                { return Mezzanine::AT_Zip; }
            MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION, "Invalid archive type passed to ResourceManager::GetArchiveTypeFromString.");
            return AT_Invalid;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType ResourceManager::GetInterfaceType() const
            { return ResourceManager::InterfaceType; }

        String ResourceManager::GetImplementationTypeName() const
            { return ResourceManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultResourceManagerFactory Methods

        DefaultResourceManagerFactory::DefaultResourceManagerFactory()
            {  }

        DefaultResourceManagerFactory::~DefaultResourceManagerFactory()
            {  }

        String DefaultResourceManagerFactory::GetManagerImplName() const
            { return ResourceManager::ImplementationName; }

        ManagerBase::ManagerType DefaultResourceManagerFactory::GetManagerType() const
            { return ResourceManager::InterfaceType; }

        EntresolManager* DefaultResourceManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( ResourceManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return ResourceManager::GetSingletonPtr();
            }
            if( Params.empty() ) {
                return new ResourceManager();
            }

            String EngineDataPath = ".";
            ArchiveType ArchType = Mezzanine::AT_FileSystem;
            for( NameValuePairList::const_iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
            {
                String Lower = (*ParIt).first;
                StringTools::ToLowerCase(Lower);
                if( "enginedatapath" == Lower ) {
                    EngineDataPath = (*ParIt).second;
                }else if( "archivetype" == Lower ) {
                    ArchType = ResourceManager::GetArchiveTypeFromString((*ParIt).second);
                }
            }
            return new ResourceManager(EngineDataPath,ArchType);
        }

        EntresolManager* DefaultResourceManagerFactory::CreateManager(const XML::Node& XMLNode)
        {
            if( ResourceManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return ResourceManager::GetSingletonPtr();
            }
            return new ResourceManager(XMLNode);
        }

        void DefaultResourceManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Resource
}//Mezzanine

#endif
