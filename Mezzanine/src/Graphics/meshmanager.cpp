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
#ifndef _graphicsmeshmanager_cpp
#define _graphicsmeshmanager_cpp

#include "Graphics/meshmanager.h"
#include "Graphics/mesh.h"

#include "vector2.h"
#include "exception.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> Graphics::MeshManager* Singleton<Graphics::MeshManager>::SingletonPtr = NULL;

    namespace Graphics
    {
        const String MeshManager::ImplementationName = "DefaultMeshManager";
        const ManagerBase::ManagerType MeshManager::InterfaceType = ManagerBase::MT_MeshManager;

        MeshManager::MeshManager()
            {  }

        MeshManager::MeshManager(XML::Node& XMLNode)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        MeshManager::~MeshManager()
        {
            this->Deinitialize();
            this->UnloadAllMeshes();
        }

        void MeshManager::AddMesh(Mesh* ToAdd)
        {
            String MeshName = ToAdd->GetName();
            MeshIterator MeshIt = this->Meshes.find( MeshName );
            if( MeshIt == this->Meshes.end() ) {
                this->Meshes.insert( std::pair<String,Mesh*>(MeshName,ToAdd) );
            }else{
                MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Meshes must have unique names when loaded!");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Management

        Mesh* MeshManager::LoadMesh(const String& MeshName, const String& Group)
        {
            MeshIterator MeshIt = this->Meshes.find(MeshName);
            if( MeshIt != this->Meshes.end() ) {
                return (*MeshIt).second;
            }
            return this->_WrapInternalMesh( this->_GetInternalManager()->load(MeshName,Group) );
        }

        void MeshManager::UnloadMesh(const String& MeshName)
        {
            MeshIterator MeshIt = this->Meshes.find(MeshName);
            if( MeshIt == this->Meshes.end() ) {
                return;
            }
            this->_GetInternalManager()->unload(MeshName);
            delete (*MeshIt).second;
            this->Meshes.erase(MeshIt);
        }

        Mesh* MeshManager::GetMesh(const String& MeshName)
        {
            MeshIterator MeshIt = this->Meshes.find(MeshName);
            if( MeshIt != this->Meshes.end() ) {
                return (*MeshIt).second;
            }
            return NULL;
        }

        Whole MeshManager::GetNumMeshes()
        {
            return this->Meshes.size();
        }

        void MeshManager::UnloadAllMeshes()
        {
            for( MeshIterator MeshIt = this->Meshes.begin() ; MeshIt != this->Meshes.end() ; ++MeshIt )
                { delete (*MeshIt).second; }
            this->Meshes.clear();
            this->_GetInternalManager()->unloadAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void MeshManager::Initialize()
            { this->Initialized = true; }

        void MeshManager::Deinitialize()
            { this->Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType MeshManager::GetInterfaceType() const
            { return MeshManager::InterfaceType; }

        String MeshManager::GetImplementationTypeName() const
            { return MeshManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Mesh* MeshManager::_WrapInternalMesh(Ogre::MeshPtr ToWrap)
        {
            Mesh* Wrapped = new Mesh(ToWrap);
            this->AddMesh( Wrapped );
            return Wrapped;
        }

        Ogre::MeshManager* MeshManager::_GetInternalManager() const
            { return Ogre::MeshManager::getSingletonPtr(); }

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

        EntresolManager* DefaultMeshManagerFactory::CreateManager(NameValuePairList& Params)
        {
            if( MeshManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return MeshManager::GetSingletonPtr();
            }else return new MeshManager();
        }

        EntresolManager* DefaultMeshManagerFactory::CreateManager(XML::Node& XMLNode)
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
