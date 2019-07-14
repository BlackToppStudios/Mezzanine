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

#include "Graphics/skeletonmanager.h"
#include "Graphics/skeleton.h"

#include "Graphics/Loaders/iostreamwrapper.h.cpp"

#include "exception.h"

#include <OgreSkeleton.h>
#include <OgreSkeletonManager.h>
#include <OgreSkeletonSerializer.h>

namespace Mezzanine
{
    template<> Graphics::SkeletonManager* Singleton<Graphics::SkeletonManager>::SingletonPtr = nullptr;

    namespace Graphics
    {
        const String SkeletonManager::ImplementationName = "DefaultSkeletonManager";
        const ManagerBase::ManagerType SkeletonManager::InterfaceType = ManagerBase::MT_SkeletonManager;

        ///////////////////////////////////////////////////////////////////////////////
        // Skeleton Manager

        SkeletonManager::SkeletonManager()
            {  }

        SkeletonManager::SkeletonManager(const XML::Node& XMLNode)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
        }

        SkeletonManager::~SkeletonManager()
        {
            this->Deinitialize();
            this->RemoveAllSkeletons();
        }

        void SkeletonManager::AddSkeleton(Skeleton* ToAdd)
        {
            String SkeletonName = ToAdd->GetName();
            SkeletonIterator SkeletonIt = this->Skeletons.find( SkeletonName );
            if( SkeletonIt == this->Skeletons.end() ) {
                this->Skeletons.insert( std::pair<String,Skeleton*>(SkeletonName,ToAdd) );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"Skeletons must have unique names when loaded!");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Skeleton Management

        Skeleton* SkeletonManager::CreateSkeleton(const String& Name)
        {

        }

        Skeleton* SkeletonManager::GetSkeleton(const String& Name)
        {
            SkeletonIterator SkeletonIt = this->Skeletons.find(SkeletonName);
            if( SkeletonIt != this->Skeletons.end() ) {
                return (*SkeletonIt).second;
            }
            return nullptr;
        }

        Whole SkeletonManager::GetNumSkeletons()
        {
            return this->Skeletons.size();
        }

        void SkeletonManager::RemoveSkeleton(Skeleton* ToBeRemoved)
        {
            SkeletonIterator SkeletonIt = this->Skeletons.find(ToBeRemoved->GetName());
            if( SkeletonIt != this->Skeletons.end() ) {
                this->_GetInternalManager()->remove(ToBeRemoved->GetName());
                delete (*SkeletonIt).second;
                this->Skeletons.erase(SkeletonIt);
            }
        }

        void SkeletonManager::RemoveAllSkeletons()
        {
            for( SkeletonIterator SkeletonIt = this->Skeletons.begin() ; SkeletonIt != this->Skeletons.end() ; ++SkeletonIt )
                { delete (*SkeletonIt).second; }
            this->Skeletons.clear();
            this->_GetInternalManager()->removeAll();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Skeleton I/O

        Skeleton* SkeletonManager::LoadSkeleton(const String& AssetIdentifier, const String& GroupName)
        {
            SkeletonIterator SkeletonIt = this->Skeletons.find(AssetIdentifier);
            if( SkeletonIt != this->Skeletons.end() ) {
                return (*SkeletonIt).second;
            }
            return this->_WrapInternalSkeleton( std::static_pointer_cast<Ogre::Skeleton>( this->_GetInternalManager()->load(AssetIdentifier,GroupName) ) );
        }

        Skeleton* SkeletonManager::LoadSkeleton(const String& LocalPath)
        {
            size_t Slash = LocalPath.find_last_of("\\/");
            if( Slash != String::npos ) {
                std::ifstream Stream;
                Stream.open(LocalPath.c_str());
                Ogre::SkeletonSerializer OgreSerializer;
                Ogre::DataStreamPtr OgreStreamPtr(new STDIStreamWrapper(&Stream,false));
                Ogre::SkeletonPtr NewSkeleton = std::static_pointer_cast<Ogre::Skeleton>( Ogre::SkeletonManager::getSingleton().create(LocalPath.substr(Slash+1),Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) );
                OgreSerializer.importSkeleton(OgreStreamPtr,NewSkeleton.get());
                return this->_WrapInternalSkeleton(NewSkeleton);
            }
            return nullptr;
        }

        Skeleton* SkeletonManager::LoadSkeleton(std::istream& Stream)
        {
            /*Ogre::SkeletonPtr NewSkeleton = Ogre::SkeletonManager::getSingleton().createManual();
            Ogre::SkeletonSerializer OgreSerializer;// */
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Loading Skeletons via exclusively a standard stream is not supported yet.");
        }

        Skeleton* SkeletonManager::LoadSkeleton(IStreamPtr Stream)
        {

        }

        void SkeletonManager::SaveSkeleton(Skeleton* ToSave, const String& AssetIdentifier, const String& GroupName)
        {
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Saving Skeletons via asset groups is not supported yet.");
        }

        void SkeletonManager::SaveSkeleton(Skeleton* ToSave, const String& LocalPath)
        {
            Ogre::SkeletonSerializer OgreSerializer;
            OgreSerializer.exportSkeleton(ToSave->_GetInternalSkeleton().get(),LocalPath);
        }

        void SkeletonManager::SaveSkeleton(Skeleton* ToSave, std::ostream& Stream)
        {
            Ogre::SkeletonSerializer OgreSerializer;
            Ogre::DataStreamPtr OgreStreamPtr(new STDOStreamWrapper(&Stream,false));
            OgreSerializer.exportSkeleton(ToSave->_GetInternalSkeleton().get(),OgreStreamPtr);
        }

        void SkeletonManager::SaveSkeleton(Skeleton* ToSave, OStreamPtr Stream)
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void SkeletonManager::Initialize()
            { this->Initialized = true; }

        void SkeletonManager::Deinitialize()
            { this->Initialized = false; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType SkeletonManager::GetInterfaceType() const
            { return SkeletonManager::InterfaceType; }

        String SkeletonManager::GetImplementationTypeName() const
            { return SkeletonManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Skeleton* SkeletonManager::_WrapInternalSkeleton(Ogre::SkeletonPtr ToWrap)
        {
            Skeleton* Wrapped = new Skeleton(ToWrap);
            this->AddSkeleton( Wrapped );
            return Wrapped;
        }

        Ogre::SkeletonManager* SkeletonManager::_GetInternalManager() const
            { return Ogre::SkeletonManager::getSingletonPtr(); }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultSkeletonManagerFactory Methods

        DefaultSkeletonManagerFactory::DefaultSkeletonManagerFactory()
            {  }

        DefaultSkeletonManagerFactory::~DefaultSkeletonManagerFactory()
            {  }

        String DefaultSkeletonManagerFactory::GetManagerImplName() const
            { return SkeletonManager::ImplementationName; }

        ManagerBase::ManagerType DefaultSkeletonManagerFactory::GetManagerType() const
            { return SkeletonManager::InterfaceType; }

        EntresolManager* DefaultSkeletonManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( SkeletonManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return SkeletonManager::GetSingletonPtr();
            }else return new SkeletonManager();
        }

        EntresolManager* DefaultSkeletonManagerFactory::CreateManager(const XML::Node& XMLNode)
        {
            if( SkeletonManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return SkeletonManager::GetSingletonPtr();
            }else return new SkeletonManager(XMLNode);
        }

        void DefaultSkeletonManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Graphics
}//Mezzanine

#endif
