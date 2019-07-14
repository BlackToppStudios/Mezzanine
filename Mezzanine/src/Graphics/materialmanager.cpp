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

#include "exception.h"

#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
//#include <OgreMaterialSerializer.h>

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

        void MaterialManager::AddMaterial(Material* ToAdd)
        {
            String MaterialName = ToAdd->GetName();
            MaterialIterator MaterialIt = this->Materials.find( MaterialName );
            if( MaterialIt == this->Materials.end() ) {
                this->Materials.insert( std::pair<String,Material*>(MaterialName,ToAdd) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"Materials must have unique names when loaded!");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Material Management

        Material* MaterialManager::CreateMaterial(const String& Name)
        {

        }

        Material* MaterialManager::GetMaterial(const String& Name)
        {
            MaterialIterator MaterialIt = this->Materials.find(Name);
            if( MaterialIt != this->Materials.end() ) {
                return (*MaterialIt).second;
            }
            return nullptr;
        }

        Whole MaterialManager::GetNumMaterials()
        {
            return this->Materials.size();
        }

        void MaterialManager::RemoveMaterial(Material* ToBeRemoved)
        {
            MaterialIterator MaterialIt = this->Materials.find(ToBeRemoved->GetName());
            if( MaterialIt != this->Materials.end() ) {
                this->_GetInternalManager()->remove(ToBeRemoved->GetName());
                delete (*MaterialIt).second;
                this->Materials.erase(MaterialIt);
            }
        }

        void MaterialManager::RemoveAllMaterials()
        {
            for( MaterialIterator MaterialIt = this->Materials.begin() ; MaterialIt != this->Materials.end() ; ++MaterialIt )
                { delete (*MaterialIt).second; }
            this->Materials.clear();
            this->_GetInternalManager()->removeAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Material I/O

        Material* MaterialManager::LoadMaterial(const String& AssetIdentifier, const String& GroupName)
        {
            MaterialIterator MaterialIt = this->Materials.find(AssetIdentifier);
            if( MaterialIt != this->Materials.end() ) {
                return (*MaterialIt).second;
            }
            Ogre::MaterialPtr NewMat = Material::_Upcast( this->_GetInternalManager()->load(AssetIdentifier,GroupName) );
            return this->_WrapInternalMaterial( NewMat );
        }

        Material* MaterialManager::LoadMaterial(const String& LocalPath)
        {
            size_t Slash = LocalPath.find_last_of("\\/");
            if( Slash != String::npos ) {
                /*std::ifstream Stream;
                Stream.open(LocalPath.c_str());
                Ogre::MaterialSerializer OgreSerializer;
                Ogre::DataStreamPtr OgreStreamPtr(new STDIStreamWrapper(&Stream,false));
                Ogre::MaterialPtr NewMaterial = std::static_pointer_cast<Ogre::Material>( Ogre::MaterialManager::getSingleton().create(LocalPath.substr(Slash+1),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) );
                OgreSerializer.importMaterial(OgreStreamPtr,NewMaterial.get());
                return this->_WrapInternalMaterial(NewMaterial);//*/
            }
            return nullptr;
        }

        Material* MaterialManager::LoadMaterial(IStreamPtr Stream)
        {

        }

        void MaterialManager::SaveMaterial(Material* ToSave, const String& AssetIdentifier, const String& GroupName)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Saving Materials via asset groups is not supported yet.");
        }

        void MaterialManager::SaveMaterial(Material* ToSave, const String& LocalPath)
        {
            //Ogre::MaterialSerializer OgreSerializer;
            //OgreSerializer.exportMaterial(ToSave->_GetInternalMaterial().get(),LocalPath);
        }

        void MaterialManager::SaveMaterial(Material* ToSave, OStreamPtr Stream)
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
