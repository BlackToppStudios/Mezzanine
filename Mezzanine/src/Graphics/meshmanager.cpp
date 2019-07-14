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
#ifndef _graphicsmeshmanager_cpp
#define _graphicsmeshmanager_cpp

#include "Graphics/meshmanager.h"
#include "Graphics/mesh.h"

#include "Graphics/Loaders/iostreamwrapper.h.cpp"
#include "Graphics/Loaders/manualmeshloader.h.cpp"

#include "Resource/pathutilities.h"

#include "exception.h"

#include <OgreMeshManager.h>
#include <OgreMeshSerializer.h>

namespace Mezzanine
{
    template<> Graphics::MeshManager* Singleton<Graphics::MeshManager>::SingletonPtr = nullptr;

    namespace Graphics
    {
        using Resource::ResourceManager;
        const String MeshManager::ImplementationName = "DefaultMeshManager";
        const ManagerBase::ManagerType MeshManager::InterfaceType = ManagerBase::MT_MeshManager;

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Manager

        MeshManager::MeshManager() :
            InternalManager(Ogre::MeshManager::getSingletonPtr())
            {  }

        MeshManager::MeshManager(const XML::Node& XMLNode) :
            InternalManager(Ogre::MeshManager::getSingletonPtr())
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        MeshManager::~MeshManager()
        {
            this->Deinitialize();
        }

        Mesh* MeshManager::LoadMeshNoCheck(IStreamPtr Stream)
        {
            if( Stream.IsValid() ) {
                ManualMeshLoader* NewLoader = new ManualMeshLoader(Stream);
                Ret = new Mesh(this->InternalManager->load(MeshName,"Mezzanine",true,NewLoader),NewLoader);
                this->AddMesh(Ret);
            }
            return nullptr;
        }

        void MeshManager::AddMesh(Mesh* ToAdd)
        {
            MeshIterator MeshIt = std::find(this->Meshes.begin(),this->Meshes.end(),ToBeRemoved);
            if( MeshIt == this->Meshes.end() ) {
                this->Meshes.push_back(ToAdd);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"Meshes must have unique names!");
            }
        }

        void MeshManager::LoadChildMaterial(Mesh* ParentMesh)
        {

        }

        void MeshManager::LoadChildSkeleton(Mesh* ParentMesh)
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Management

        Mesh* MeshManager::CreateMesh(const String& Name)
        {
            Ogre::MeshPtr NewInternalMesh = this->InternalManager->create(Name,"Internal",true,nullptr,nullptr);
            return this->_WrapInternalMesh( NewInternalMesh );
        }

        Mesh* MeshManager::GetMesh(const String& Name)
        {
            MeshIterator MeshIt = std::find_if(this->Meshes.begin(),this->Meshes.end(),[&](Mesh* ToCheck) {
                return ToCheck->GetName() == Name;
            });
            if( MeshIt != this->Meshes.end() ) {
                return (*MeshIt);
            }
            return nullptr;
        }

        Whole MeshManager::GetNumMeshes()
        {
            return this->Meshes.size();
        }

        void MeshManager::RemoveMesh(Mesh* ToBeRemoved)
        {
            MeshIterator MeshIt = std::find(this->Meshes.begin(),this->Meshes.end(),ToBeRemoved);
            if( MeshIt != this->Meshes.end() ) {
                this->Meshes.erase(MeshIt);
                this->InternalManager->remove(ToBeRemoved->GetName());
                delete (*MeshIt);
            }
        }

        void MeshManager::RemoveAllMeshes()
        {
            for( Mesh* CurrMesh : this->Meshes )
                { delete CurrMesh; }
            this->Meshes.clear();
            this->InternalManager->removeAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh I/O

        Mesh* MeshManager::LoadMesh(const String& AssetIdentifier, const String& GroupName)
        {
            String MeshName = Resource::GetBaseName(AssetIdentifier);
            Mesh* Ret = this->GetMesh(MeshName);
            if( Ret == nullptr ) {
                ResourceManager* ResourceMan = this->TheEntresol->GetManager<ResourceManager::InterfaceType>();
                IStreamPtr MeshStream = ResourceMan->OpenAsset(AssetIdentifier,GroupName);
                Ret = this->LoadMeshNoCheck(MeshStream);
            }
            return Ret;
        }

        Mesh* MeshManager::LoadMesh(const String& LocalPath)
        {
            String MeshName = Resource::GetBaseName(AssetIdentifier);
            Mesh* Ret = this->GetMesh(MeshName);
            if( Ret == nullptr ) {
                size_t Slash = LocalPath.find_last_of("\\/");
                Resource::FileIStreamPtr MeshStream = std::make_shared<Resource::FileIStream>();
                MeshStream->OpenFile(LocalPath,Slash);
                Ret = this->LoadMeshNoCheck(MeshStream);
            }
        }

        Mesh* MeshManager::LoadMesh(IStreamPtr Stream)
        {
            Mesh* Ret = nullptr;
            if( Stream.IsValid() ) {
                String MeshName = Resource::GetBaseName(Stream->GetIdentifier());
                Ret = this->GetMesh(MeshName);
                if( Ret == nullptr ) {
                    ManualMeshLoader* NewLoader = new ManualMeshLoader(Stream);
                    Ret = this->LoadMeshNoCheck(Stream);
                }
            }
            return Ret;
        }

        void MeshManager::SaveMesh(Mesh* ToSave, const String& AssetIdentifier, const String& GroupName)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Saving Meshes via asset groups is not supported yet.");
        }

        void MeshManager::SaveMesh(Mesh* ToSave, const String& LocalPath)
        {
            String PathAndName = ( LocalPath.empty() ? "."s : LocalPath );
            if( !Resource::IsDirectorySeparator( PathAndName.back() ) ) {
                PathAndName.append( 1, Resource::GetDirectorySeparator_Host() );
            }
            Resource::FileIStreamPtr MeshStream = std::make_shared<Resource::FileIStream>();
            MeshStream.OpenFile(LocalPath + ToSave.GetName(),PathAndName.size() - 1);
            this->SaveMesh(ToSave,MeshStream);
        }

        void MeshManager::SaveMesh(Mesh* ToSave, OStreamPtr Stream)
        {
            Ogre::MeshSerializer OgreSerializer;
            Ogre::DataStreamPtr OgreStreamPtr(new OStreamWrapper(Stream));
            OgreSerializer.exportMesh(ToSave->_GetInternalMesh().get(),OgreStreamPtr);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void MeshManager::Initialize()
        {
            this->InternalManager = Ogre::MeshManager::getSingletonPtr();
            if( this->InternalManager == nullptr ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Internal MeshManager is not initialized.");
            }
            this->Initialized = true;
        }

        void MeshManager::Deinitialize()
        {
            this->RemoveAllMeshes();
            this->Initialized = false;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType MeshManager::GetInterfaceType() const
            { return MeshManager::InterfaceType; }

        String MeshManager::GetImplementationTypeName() const
            { return MeshManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::MeshManager* MeshManager::_GetInternalManager() const
            { return this->InternalManager; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultMeshManagerFactory Methods

        DefaultMeshManagerFactory::DefaultMeshManagerFactory()
            {  }

        DefaultMeshManagerFactory::~DefaultMeshManagerFactory()
            {  }

        String DefaultMeshManagerFactory::GetManagerImplName() const
            { return MeshManager::ImplementationName; }

        ManagerBase::ManagerType DefaultMeshManagerFactory::GetManagerType() const
            { return MeshManager::InterfaceType; }

        EntresolManager* DefaultMeshManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( MeshManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MeshManager::GetSingletonPtr();
            }else return new MeshManager();
        }

        EntresolManager* DefaultMeshManagerFactory::CreateManager(const XML::Node& XMLNode)
        {
            if( MeshManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MeshManager::GetSingletonPtr();
            }else return new MeshManager(XMLNode);
        }

        void DefaultMeshManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Graphics
}//Mezzanine

#endif
