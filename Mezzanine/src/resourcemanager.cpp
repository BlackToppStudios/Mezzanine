//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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

#include <iostream>
#include <fstream>

#include "resourcemanager.h"
#include "meshmanager.h"
#include "actorbase.h"
#include "internalogredatastreambuf.h.cpp"
#include "internalbulletfilemanager.h.cpp"

#include <Ogre.h>
#include <btBulletWorldImporter.h>
#include <btBulletDynamicsCommon.h>

#ifdef WINDOWS
    #include <Winuser.h>
    #include <direct.h> // for _getcwd
#else
	#include <unistd.h>//for sleep and getcwd
#endif

#ifdef MEZZDEBUG
#include "world.h"
#endif

namespace Mezzanine
{
    template<> ResourceManager* Singleton<ResourceManager>::SingletonPtr = 0;

    ResourceManager::ResourceManager(const String& EngineDataPath)
    {
        this->Priority = 55;
        OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
        internal::BulletFileManager* BulletFileMan = internal::BulletFileManager::getSingletonPtr();
        EngineDataDir = EngineDataPath;
        this->AddResourceLocation(EngineDataPath, "FileSystem", "EngineData", false);
        //internal::BulletFileManager* BulletFileMan = new internal::BulletFileManager();
    }

    ResourceManager::~ResourceManager()
    {
        for(std::vector<ResourceInputStream*>::iterator Iter = DeleteList.begin(); Iter != DeleteList.end(); Iter++)
            { delete *Iter; }
    }

    std::set<String>* ResourceManager::GetDirContents(const String& Dir)
    {
        std::set<String>* Results = new std::set<String>;
        DIR *Directory;
        struct dirent *DirEntry;
        if(Directory = opendir(Dir.c_str()))
        {
            while(DirEntry = readdir(Directory))
            {
                Results->insert(DirEntry->d_name);
                //DirEntry->d_type Later this can be modified to include the type of file entry it is, like a file, block device, directory, socket etc...
            }

            closedir(Directory);
            return Results;
        }else{
            delete Results;
            return NULL;
        }
    }

    String ResourceManager::GetWorkingDirectory() const
    {
        char cCurrentPath[FILENAME_MAX];
        //char cCurrentPath[MAXPATHLEN];
        #ifdef WINDOWS
        String Results (_getcwd(cCurrentPath,sizeof(cCurrentPath)));
        #else
        String Results (getcwd(cCurrentPath,sizeof(cCurrentPath)));
        #endif
        return Results;
    }

    String ResourceManager::GetEngineDataDirectory() const
        { return EngineDataPath; }

    void ResourceManager::AddResourceGroupName(String Name)
    {
        for( Whole X = 0 ; X < ResourceGroups.size() ; X++ )
        {
            if(Name == ResourceGroups[X])
                return;
        }
        ResourceGroups.push_back(Name);
    }

    void ResourceManager::AddResourceLocation(const String& Location, const String& Type, const String& Group, bool recursive)
    {
        this->OgreResource->addResourceLocation(Location, Type, Group, recursive);
        AddResourceGroupName(Group);
    }

    void ResourceManager::CreateResourceGroup(const String& GroupName)
    {
        this->OgreResource->createResourceGroup(GroupName);
        AddResourceGroupName(GroupName);
    }

    void ResourceManager::DestroyResourceGroup(const String& GroupName)
    {
        for( std::vector<String>::iterator it = ResourceGroups.begin() ; it != ResourceGroups.end() ; it++ )
        {
            if(GroupName == (*it))
            {
                ResourceGroups.erase(it);
                break;
            }
        }
        /// @todo This is a bit of a hack, but needs to be here until we can upgrade our resource system.
        Ogre::StringVectorPtr ResourceNames = this->OgreResource->listResourceNames(GroupName,false);
        for( Whole X = 0 ; X < ResourceNames->size() ; ++X )
        {
            if(ResourceNames->at(X).find(".mesh"))
            {
                MeshManager::GetSingletonPtr()->UnloadMesh(ResourceNames->at(X));
            }
        }
        this->OgreResource->destroyResourceGroup(GroupName);
    }

    void ResourceManager::DeclareResource(const String& Name, const String& Type, const String& Group)
    {
        this->OgreResource->declareResource(Name, Type, Group);
    }

    void ResourceManager::InitResourceGroup(const String& Group)
    {
        this->OgreResource->initialiseResourceGroup(Group);
    }

    ResourceInputStream* ResourceManager::GetResourceStream(const String& FileName)
    {
        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log("Entering ResourceManager::GetResourceStream(const String& FileName)");
        #endif
        internal::OgreDataStreamBuf *TempBuffer = new internal::OgreDataStreamBuf(OgreResource->openResource(FileName));
        ResourceInputStream *Results =  new ResourceInputStream(TempBuffer, this);
        this->DeleteList.push_back(Results);
        #ifdef MEZZDEBUG
        World::GetWorldPointer()->Log("Exiting ResourceManager::GetResourceStream(const String& FileName)");
        #endif
        return Results;
    }

    void ResourceManager::Initialize()
    {
    }

    void ResourceManager::DoMainLoopItems()
    {
    }

    ManagerBase::ManagerTypeName ResourceManager::GetType() const
        { return ManagerBase::ResourceManager; }
}

#endif
