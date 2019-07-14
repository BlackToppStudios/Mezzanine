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
#ifndef _resourcemanager_cpp
#define _resourcemanager_cpp

#include "Resource/resourcemanager.h"
#include "Resource/resourceenumerations.h"
#include "Resource/assetgroup.h"

#include "filestream.h"
#include "memorystream.h"

#include "Graphics/meshmanager.h"

#include "stringtool.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace Mezzanine
{
    template<> Resource::ResourceManager* Singleton<Resource::ResourceManager>::SingletonPtr = nullptr;

    namespace Resource
    {
        const String ResourceManager::ImplementationName = "DefaultResourceManager";
        const ManagerBase::ManagerType ResourceManager::InterfaceType = ManagerBase::MT_ResourceManager;

        ResourceManager::ResourceManager(const String& EngineDataPath, const Resource::ArchiveType ArchType) :
            ResourcePublisher(0)
        {
            this->EngineDataDir = EngineDataPath;
            this->AddAssetLocation(EngineDataPath, ArchType, "EngineData");
        }

        ResourceManager::ResourceManager(const XML::Node& XMLNode) :
            ResourcePublisher(0)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        ResourceManager::~ResourceManager()
        {
            this->Deinitialize();
            this->DestroyAllAssetGroups();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // AssetGroup Management

        void ResourceManager::AddAssetLocation(const String& Location, const ArchiveType Type, const String& GroupName)
        {
            AssetGroup* FoundGroup = this->GetOrCreateAssetGroup(GroupName);
            FoundGroup->AddReadLocation(Location,Type);
            this->ResourcePublisher.DispatchEvent(&ResourceEventListener::AssetLocationAdded,Location,Type,GroupName);
        }

        AssetGroup* ResourceManager::CreateAssetGroup(const String& GroupName)
        {
            if( this->GetAssetGroup(GroupName) != nullptr ) {
                StringStream ExceptionStream;
                ExceptionStream << "An AssetGroup with the name \"" << GroupName << "\" already exists.";
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,ExceptionStream.str());
            }

            AssetGroup* NewGroup = new AssetGroup(GroupName);
            this->AssetGroups.push_back(NewGroup);
            this->ResourcePublisher.DispatchEvent(&ResourceEventListener::AssetGroupCreated,GroupName);
            return NewGroup;
        }

        AssetGroup* ResourceManager::GetOrCreateAssetGroup(const String& GroupName)
        {
            AssetGroup* Ret = this->GetAssetGroup(GroupName);
            if( Ret == nullptr ) {
                Ret = this->CreateAssetGroup(GroupName);
            }
            return Ret;
        }

        AssetGroup* ResourceManager::GetAssetGroup(const String& GroupName)
        {
            for( AssetGroup* CurrGroup : this->AssetGroups )
            {
                if( CurrGroup->GetName() == GroupName ) {
                    return CurrGroup;
                }
            }
            return nullptr;
        }

        AssetGroup* ResourceManager::GetAssetGroupExcept(const String& GroupName)
        {
            AssetGroup* FoundGroup = this->GetAssetGroup(GroupName);
            if( FoundGroup == nullptr ) {
                StringStream ExceptionStream;
                ExceptionStream << "AssetGroup named \"" << GroupName << "\" not found.";
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return FoundGroup;
        }

        void ResourceManager::DestroyAssetGroup(const String& GroupName)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.begin();
            while( GroupIt != this->AssetGroups.end() )
            {
                if( (*GroupIt)->GetName() == GroupName ) {
                    this->ResourcePublisher.DispatchEvent(&ResourceEventListener::AssetGroupDestroyed,GroupName);
                    delete (*GroupIt);
                    this->AssetGroups.erase(GroupIt);
                    return;
                }
                ++GroupIt;
            }
        }

        void ResourceManager::DestroyAssetGroup(AssetGroup* ToBeDestroyed)
        {
            AssetGroupIterator GroupIt = this->AssetGroups.begin();
            while( GroupIt != this->AssetGroups.end() )
            {
                if( (*GroupIt) == ToBeDestroyed ) {
                    const String& GroupName = ToBeDestroyed->GetName();
                    this->ResourcePublisher.DispatchEvent(&ResourceEventListener::AssetGroupDestroyed,GroupName);
                    delete ToBeDestroyed;
                    this->AssetGroups.erase(GroupIt);
                    return;
                }
                ++GroupIt;
            }
        }

        void ResourceManager::DestroyAllAssetGroups()
        {
            for( AssetGroup* CurrGroup : this->AssetGroups )
                { delete CurrGroup; }
            this->AssetGroups.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Management

        IStreamPtr ResourceManager::OpenAsset(const String& Identifier,
                                              const String& GroupName,
                                              const Whole Flags,
                                              const Boole Raw)
        {
            AssetGroup* NamedGroup = this->GetAssetGroup(GroupName);
            IStreamPtr Ret = NamedGroup->OpenAsset(Identifier,Flags,Raw);
            if( !Ret && !this->LimitedSearches ) {
                for( AssetGroup* CurrGroup : this->AssetGroups )
                {
                    if( CurrGroup != NamedGroup ) {
                        Ret = CurrGroup->OpenAsset(Identifier,Flags,Raw);
                        if( Ret ) {
                            break;
                        }
                    }
                }
            }
            if( !Ret ) {
                StringStream ExceptionStream;
                ExceptionStream << "File/Asset \"" << Identifier << "\" in Group \""
                                << GroupName << "\" could not be found.";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return Ret;
        }

        IStreamPtr ResourceManager::BufferAsset(const String& Identifier,
                                                const String& GroupName,
                                                const Whole Flags,
                                                const Boole Raw)
        {
            AssetGroup* NamedGroup = this->GetAssetGroup(GroupName);
            IStreamPtr Ret = NamedGroup->BufferAsset(Identifier,Flags,Raw);
            if( !Ret && !this->LimitedSearches ) {
                for( AssetGroup* CurrGroup : this->AssetGroups )
                {
                    if( CurrGroup != NamedGroup ) {
                        Ret = CurrGroup->BufferAsset(Identifier,Flags,Raw);
                        if( Ret ) {
                            break;
                        }
                    }
                }
            }
            if( !Ret ) {
                StringStream ExceptionStream;
                ExceptionStream << "File/Asset \"" << Identifier << "\" in Group \""
                                << GroupName << "\" could not be found.";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return Ret;
        }

        IStreamPtr ResourceManager::OpenChildAsset(const String& ParentIdentifier,
                                                   const String& ParentGroupName,
                                                   const String& ChildIdentifier,
                                                   const String& ChildGroupName,
                                                   const Whole Flags,
                                                   const Boole Raw)
        {
            /// @todo Use this method to generate a manifest of dependent resources.
            return this->OpenAsset(ChildIdentifier,ChildGroupName,Flags,Raw);
        }

        IStreamPtr ResourceManager::BufferChildAsset(const String& ParentIdentifier,
                                                     const String& ParentGroupName,
                                                     const String& ChildIdentifier,
                                                     const String& ChildGroupName,
                                                     const Whole Flags,
                                                     const Boole Raw)
        {
            /// @todo Use this method to generate a manifest of dependent resources.
            return this->BufferAsset(ChildIdentifier,ChildGroupName,Flags,Raw);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Encrypted Stream Management

        IStreamPtr ResourceManager::OpenEncryptedAsset(const String& Identifier,
                                                       const String& GroupName,
                                                       const String& Password,
                                                       const Whole Flags,
                                                       const Boole Raw)
        {
            AssetGroup* NamedGroup = this->GetAssetGroup(GroupName);
            IStreamPtr Ret = NamedGroup->OpenEncryptedAsset(Identifier,Password,Flags,Raw);
            if( !Ret && !this->LimitedSearches ) {
                for( AssetGroup* CurrGroup : this->AssetGroups )
                {
                    if( CurrGroup != NamedGroup ) {
                        Ret = CurrGroup->OpenEncryptedAsset(Identifier,Password,Flags,Raw);
                        if( Ret ) {
                            break;
                        }
                    }
                }
            }
            if( !Ret ) {
                StringStream ExceptionStream;
                ExceptionStream << "File/Asset \"" << Identifier << "\" in Group \""
                                << GroupName << "\" could not be found.";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return Ret;
        }

        IStreamPtr ResourceManager::BufferEncryptedAsset(const String& Identifier,
                                                         const String& GroupName,
                                                         const String& Password,
                                                         const Whole Flags,
                                                         const Boole Raw)
        {
            AssetGroup* NamedGroup = this->GetAssetGroup(GroupName);
            IStreamPtr Ret = NamedGroup->BufferEncryptedAsset(Identifier,Password,Flags,Raw);
            if( !Ret && !this->LimitedSearches ) {
                for( AssetGroup* CurrGroup : this->AssetGroups )
                {
                    if( CurrGroup != NamedGroup ) {
                        Ret = CurrGroup->BufferEncryptedAsset(Identifier,Password,Flags,Raw);
                        if( Ret ) {
                            break;
                        }
                    }
                }
            }
            if( !Ret ) {
                StringStream ExceptionStream;
                ExceptionStream << "File/Asset \"" << Identifier << "\" in Group \""
                                << GroupName << "\" could not be found.";
                MEZZ_EXCEPTION(ExceptionBase::IO_FILE_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            }
            return Ret;
        }

        IStreamPtr ResourceManager::OpenEncryptedChildAsset(const String& ParentIdentifier,
                                                            const String& ParentGroupName,
                                                            const String& ChildIdentifier,
                                                            const String& ChildGroupName,
                                                            const String& Password,
                                                            const Whole Flags,
                                                            const Boole Raw)
        {
            /// @todo Use this method to generate a manifest of dependent resources.
            return this->OpenEncryptedAsset(ChildIdentifier,ChildGroupName,Password,Flags,Raw);
        }

        IStreamPtr ResourceManager::BufferEncryptedChildAsset(const String& ParentIdentifier,
                                                              const String& ParentGroupName,
                                                              const String& ChildIdentifier,
                                                              const String& ChildGroupName,
                                                              const String& Password,
                                                              const Whole Flags,
                                                              const Boole Raw)
        {
            /// @todo Use this method to generate a manifest of dependent resources.
            return this->BufferEncryptedAsset(ChildIdentifier,ChildGroupName,Password,Flags,Raw);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Resource/Asset Query

        String ResourceManager::GetAssetPath(const String& FileName, const String& GroupName)
            { return this->GetAssetGroupExcept(GroupName)->GetAssetPath(FileName); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        const String& ResourceManager::GetEngineDataDirectory() const
        {
            return this->EngineDataDir;
        }

        void ResourceManager::SetLimitedSearches(const Boole Limited)
        {
            this->LimitedSearches = Limited;
        }

        Boole ResourceManager::GetLimitedSearches() const
        {
            return this->LimitedSearches;
        }

        String ResourceManager::GetPlatformPluginExtension()
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

        String ResourceManager::GetStringFromArchiveType(const Resource::ArchiveType ArchType)
        {
            switch(ArchType)
            {
                case Resource::AT_FileSystem:
                    return String("FileSystem");
                case Resource::AT_Zip:
                    return String("Zip");
                default:
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION, "Invalid archive type passed to ResourceManager::GetStringFromArchiveType.");
                    return String("");
            }
        }

        Resource::ArchiveType ResourceManager::GetArchiveTypeFromString(const String& FromString)
        {
            if(String("FileSystem")==FromString)
                { return Resource::AT_FileSystem; }
            if(String("Zip")==FromString)
                { return Resource::AT_Zip; }
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
            ArchiveType ArchType = Resource::AT_FileSystem;
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
