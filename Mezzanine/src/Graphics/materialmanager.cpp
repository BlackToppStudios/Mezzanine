// © Copyright 2010 - 2019 BlackTopp Studios Inc.
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
#ifndef _graphicsskeletonmanager_cpp
#define _graphicsskeletonmanager_cpp

#include "Graphics/materialmanager.h"
#include "Graphics/material.h"

#include "Graphics/Loaders/iostreamwrapper.h.cpp"
#include "Graphics/Loaders/manualmaterialloader.h.cpp"

#include "Resource/archiveentry.h"
#include "Resource/archivereader.h"
#include "Resource/assetgroup.h"
#include "Resource/pathutilities.h"

#include "datastream.h"
#include "filestream.h"
#include "exception.h"
#include "entresol.h"

#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreScriptCompiler.h>
//#include <OgreMaterialSerializer.h>

namespace
{
    class MaterialLoaderListener : public Ogre::ScriptCompilerListener
    {
    public:
        MaterialLoaderListener() = default;
        ~MaterialLoaderListener() = default;

        bool handleEvent(Ogre::ScriptCompiler* Compiler, Ogre::ScriptCompilerEvent* Event, void* RetVal)
        {
            if( Event->mType == "createMaterial" ) {

            }
            return false;
        }
    };//MaterialLoaderListener
}

namespace Mezzanine
{
    template<> Graphics::MaterialManager* Singleton<Graphics::MaterialManager>::SingletonPtr = nullptr;

    namespace Graphics
    {
        const String MaterialManager::ImplementationName = "DefaultMaterialManager";
        const ManagerBase::ManagerType MaterialManager::InterfaceType = ManagerBase::MT_MaterialManager;

        ///////////////////////////////////////////////////////////////////////////////
        // Material Manager

        MaterialManager::MaterialManager()
            {  }

        MaterialManager::MaterialManager(const XML::Node& XMLNode)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        MaterialManager::~MaterialManager()
        {
            this->Deinitialize();
            this->RemoveAllMaterials();
        }

        Material* MaterialManager::LoadMaterialNoCheck(IStreamPtr Stream)
        {
            if( Stream->IsValid() ) {
                String MaterialName = Resource::GetBaseName(Stream->GetIdentifier());
                ManualMaterialLoader* NewLoader = new ManualMaterialLoader(Stream);
                Material* Ret = new Material(this->InternalManager->load(MaterialName,"Mezzanine",true,NewLoader),NewLoader);
                this->Materials.push_back(Ret);
                this->LoadChildMaterials(Ret);
                this->LoadChildTextures(Ret);
                this->LoadChildGPUPrograms();
                return Ret;
            }
            return nullptr;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Material Management

        Material* MaterialManager::CreateMaterial(const String& Name)
        {

        }

        Material* MaterialManager::GetMaterial(const String& Name)
        {
            auto Finder = [&](Material* ToCheck) -> Boole { return ToCheck->GetName() == Name; };
            MaterialIterator MaterialIt = std::find_if(this->Materials.begin(),this->Materials.end(),Finder);
            if( MaterialIt != this->Materials.end() ) {
                return (*MaterialIt);
            }
            return nullptr;
        }

        Whole MaterialManager::GetNumMaterials()
        {
            return this->Materials.size();
        }

        void MaterialManager::RemoveMaterial(Material* ToBeRemoved)
        {
            MaterialIterator MaterialIt = std::find(this->Materials.begin(),this->Materials.end(),ToBeRemoved);
            if( MaterialIt != this->Materials.end() ) {
                this->Materials.erase(MaterialIt);
                this->_GetInternalManager()->remove(ToBeRemoved->GetName());
                delete (*MaterialIt);
            }
        }

        void MaterialManager::RemoveAllMaterials()
        {
            for( MaterialIterator MaterialIt = this->Materials.begin() ; MaterialIt != this->Materials.end() ; ++MaterialIt )
                { delete (*MaterialIt); }
            this->Materials.clear();
            this->InternalManager->removeAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Material I/O

        MaterialVector MaterialManager::LoadMaterials(const String& AssetIdentifier, const String& GroupName)
        {
            String MaterialName = Resource::GetBaseName(AssetIdentifier);
            Material* Ret = this->GetMaterial(MaterialName);
            if( Ret == nullptr ) {
                Resource::ResourceManager* ResourceMan = this->TheEntresol->GetManager<Resource::ResourceManager>();
                IStreamPtr MaterialStream = ResourceMan->OpenAsset(AssetIdentifier,GroupName);
                Ret = this->LoadMaterialNoCheck(MaterialStream);
            }
            return Ret;
        }

        MaterialVector MaterialManager::LoadMaterials(const String& LocalPath)
        {
            size_t Slash = LocalPath.find_last_of("\\/");
            String MaterialName = Resource::GetBaseName(LocalPath.substr(Slash + 1));
            Material* Ret = this->GetMaterial(MaterialName);
            if( Ret == nullptr ) {
                FileIStreamPtr MaterialStream = std::make_shared<FileIStream>();
                MaterialStream->OpenFile(LocalPath,Slash);
                Ret = this->LoadMaterialNoCheck(MaterialStream);
            }
        }

        MaterialVector MaterialManager::LoadMaterials(IStreamPtr Stream)
        {
            String MaterialName = Resource::GetBaseName(Stream->GetIdentifier());
            Material* Ret = this->GetMaterial(MaterialName);
            if( Ret == nullptr ) {
                Ret = this->LoadMaterialNoCheck(Stream);
            }
            return Ret;
        }

        void MaterialManager::SaveMaterials(MaterialVector& ToSave, const String& AssetIdentifier, const String& GroupName)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Saving Materials via asset groups is not supported yet.");
        }

        void MaterialManager::SaveMaterials(MaterialVector& ToSave, const String& LocalPath)
        {
            //Ogre::MaterialSerializer OgreSerializer;
            //OgreSerializer.exportMaterial(ToSave->_GetInternalMaterial().get(),LocalPath);
        }

        void MaterialManager::SaveMaterials(MaterialVector& ToSave, OStreamPtr Stream)
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void MaterialManager::Initialize()
            { this->Initialized = true; }

        void MaterialManager::Deinitialize()
            { this->Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType MaterialManager::GetInterfaceType() const
            { return MaterialManager::InterfaceType; }

        String MaterialManager::GetImplementationTypeName() const
            { return MaterialManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // ResourceEventListener Methods

        Resource::ResourceEventListener::IDType MaterialManager::GetID() const
            { return IDType(this); }

        void MaterialManager::OnAssetLocationAdded(const String& Location, const ArchiveType Type, const String& GroupName)
        {
            Resource::ResourceManager* ResourceMan = this->TheEntresol->GetManager<Resource::ResourceManager>();
            if( ResourceMan ) {
                Resource::AssetGroup* Group = ResourceMan->GetAssetGroup(GroupName);
                if( Group ) {
                    Resource::ArchiveReader* Reader = Group->GetAssetReader(Location);
                    if( Reader ) {
                        Resource::ArchiveEntryVector Results = Reader->GetEntries("*.material",true);

                        for( Resource::ArchiveEntry& Entry : Results)
                        {
                            IStreamPtr MatStream = Reader->OpenIStream(Entry.Name);
                            this->LoadMaterials(MatStream);
                        }
                    }
                }
            }
        }

        void MaterialManager::OnAssetGroupCreated(const String& GroupName)
            { /* We don't care. */ }

        void MaterialManager::OnAssetGroupDestroyed(const String& GroupName)
            { /* We don't care. */ }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::MaterialManager* MaterialManager::_GetInternalManager() const
            { return Ogre::MaterialManager::getSingletonPtr(); }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultMaterialManagerFactory Methods

        DefaultMaterialManagerFactory::DefaultMaterialManagerFactory()
            {  }

        DefaultMaterialManagerFactory::~DefaultMaterialManagerFactory()
            {  }

        String DefaultMaterialManagerFactory::GetManagerImplName() const
            { return MaterialManager::ImplementationName; }

        ManagerBase::ManagerType DefaultMaterialManagerFactory::GetManagerType() const
            { return MaterialManager::InterfaceType; }

        EntresolManager* DefaultMaterialManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( MaterialManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MaterialManager::GetSingletonPtr();
            }else return new MaterialManager();
        }

        EntresolManager* DefaultMaterialManagerFactory::CreateManager(const XML::Node& XMLNode)
        {
            if( MaterialManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MaterialManager::GetSingletonPtr();
            }else return new MaterialManager(XMLNode);
        }

        void DefaultMaterialManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Graphics
}//Mezzanine

#endif
