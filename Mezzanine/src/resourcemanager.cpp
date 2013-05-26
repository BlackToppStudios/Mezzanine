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
#include "stringtool.h"
#include "Internal/ogredatastreambuf.h.cpp"
#include "Internal/bulletfilemanager.h.cpp"

#include "Resource/resourceenumerations.h"

#include <Ogre.h>
#include <btBulletWorldImporter.h>
#include <btBulletDynamicsCommon.h>

#include <dirent.h>
#ifdef WINDOWS
    #include <Winuser.h>
    #include <WinBase.h>
    #include <Shlobj.h> // for getting system directories
    #include <direct.h> // for _getcwd
#elif MACOSX
    #include <CoreServices/CoreServices.h>
    #include <unistd.h>//for sleep and getcwd
	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
    #include "pwd.h"
#else
	#include <unistd.h>//for sleep and getcwd
	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
    #include <pwd.h>
#endif


#ifdef MEZZDEBUG
#include "entresol.h"
#endif

#ifdef CreateDirectory
#undef CreateDirectory
#endif

namespace Mezzanine
{
    template<> ResourceManager* Singleton<ResourceManager>::SingletonPtr = 0;

    ResourceManager::ResourceManager(const String& EngineDataPath, const Mezzanine::ArchiveType ArchType)
    {
        this->Priority = 60;
        OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
        EngineDataDir = EngineDataPath;
        this->AddAssetLocation(EngineDataPath, ArchType, "EngineData", false);
    }

    ResourceManager::ResourceManager(XML::Node& XMLNode)
    {
        this->Priority = 60;
        OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
    }

    ResourceManager::~ResourceManager()
    {
        for(std::vector<ResourceInputStream*>::iterator Iter = DeleteList.begin(); Iter != DeleteList.end(); Iter++)
            { delete *Iter; }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Directory Management

    bool ResourceManager::CreateDirectory(const String& DirectoryPath)
    {
        #ifdef WINDOWS
        if(::CreateDirectoryA(DirectoryPath.c_str(),NULL) < 0)
        {
            if(ERROR_ALREADY_EXISTS == ::GetLastError())
            {
                return false;
            }
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create directory.  Error follows:" << std::endl;
            if(ERROR_PATH_NOT_FOUND == ::GetLastError())
            {
                ExceptionStream << "Path to requested directory does not exist.";
            }
            else
            {
                ExceptionStream << "Error Unknown. :(";
            }
            MEZZ_EXCEPTION(Exception::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }
        return true;
        #else
        if(::mkdir(DirectoryPath.c_str(),0777) < 0)
        {
            if( EEXIST == errno )
            {
                return false;
            }
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create directory.  Error follows:" << std::endl;
            ExceptionStream << strerror(errno);
            MEZZ_EXCEPTION(Exception::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
        }
        return true;
        #endif
    }

    bool ResourceManager::CreateDirectoryPath(const String& DirectoryPath)
    {
        bool Result = true;
        StringVector FolderNames;
        CountedPtr<StringVector> FolderVec = StringTools::Split(DirectoryPath,"/\\");
        size_t StartIndex = 0;
        String PathAttempt;
        #ifdef WINDOWS
        char SysSlash = '\\';
        #else
        char SysSlash = '/';
        #endif
        #ifdef WINDOWS
        // For windows and windows like machines, see if the first entry is a drive, because attempting to make a drive is silly.
        if(FolderVec->at(0).find(':') != String::npos)
        {
            PathAttempt.append( FolderVec->at(0) );
            PathAttempt.append( 1, SysSlash );
            StartIndex++;
        }
        #else
        PathAttempt.append( 1, SysSlash );
        #endif
        for( size_t VecIndex = StartIndex ; VecIndex < FolderVec->size() ; ++VecIndex )
        {
            PathAttempt.append( FolderVec->at(VecIndex) );
            PathAttempt.append( 1, SysSlash );
            Result = CreateDirectory( PathAttempt );
        }
        return Result;
    }

    StringSet* ResourceManager::GetDirContents(const String& Dir)
    {
        StringSet* Results = new StringSet;
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
    {
        return EngineDataDir;
    }

    String ResourceManager::ResolveDataPathFromString(const String& PathVar)
    {
        String LowerVar = PathVar;
        StringTools::ToLowerCase(LowerVar);
        if(LowerVar == "localappdata") return GetLocalAppDataDir();
        else if(LowerVar == "shareableappdata") return GetShareableAppDataDir();
        else if(LowerVar == "currentuserdata") return GetCurrentUserDataDir();
        else if(LowerVar == "commonuserdata") return GetCommonUserDataDir();
        else
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempting to retrieve unknown path variable: \"" + PathVar + "\".");
        }
    }

    String ResourceManager::GetLocalAppDataDir() const
    {
        #ifdef WINDOWS
        TCHAR path_local_appdata[MAX_PATH];
        if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, path_local_appdata)))
        {
            return path_local_appdata;
        }
        #else
        struct passwd* pw = getpwuid(getuid());
        if(pw)
        {
            return String(pw->pw_dir);
        }
        else
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve app data directory.");
        }

        // might be some useful MAC OS X code
        /*#elif MACOSX
        FSRef ref;
        OSType folderType = kApplicationSupportFolderType;
        char path[PATH_MAX];
        FSFindFolder( kUserDomain, folderType, kCreateFolder, &ref );
        FSRefMakePath( &ref, (UInt8*)&path, PATH_MAX );
        return path;*/
        #endif
    }

    String ResourceManager::GetShareableAppDataDir() const
    {
        #ifdef WINDOWS
        TCHAR path_appdata[MAX_PATH];
        if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, path_appdata)))
        {
            return path_appdata;
        }
        #else
        struct passwd* pw = getpwuid(getuid());
        if(pw)
        {
            return String(pw->pw_dir);
        }
        else
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve home directory.");
        }
        #endif
    }

    String ResourceManager::GetCurrentUserDataDir() const
    {
        #ifdef WINDOWS
        TCHAR path_personal[MAX_PATH];
        if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, path_personal)))
        {
            return path_personal;
        }
        #else
        struct passwd* pw = getpwuid(getuid());
        if(pw)
        {
            return String(pw->pw_dir);
        }
        else
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve user data directory.");
        }
        #endif
    }

    String ResourceManager::GetCommonUserDataDir() const
    {
        #ifdef WINDOWS
        TCHAR path_common_personal[MAX_PATH];
        if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS|CSIDL_FLAG_CREATE, NULL, 0, path_common_personal)))
        {
            return path_common_personal;
        }
        #else
        struct passwd* pw = getpwuid(getuid());
        if(pw)
        {
            return String(pw->pw_dir);
        }
        else
        {
            MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Could not get user information to retrieve common data directory.");
        }
        #endif
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Stream Management

    ///////////////////////////////////////////////////////////////////////////////
    // AssetGroup Management

    void ResourceManager::AddAssetGroupName(String Name)
    {
        for( Whole X = 0 ; X < ResourceGroups.size() ; X++ )
        {
            if(Name == ResourceGroups[X])
                return;
        }
        ResourceGroups.push_back(Name);
    }

    void ResourceManager::AddAssetLocation(const String& Location, ArchiveType Type, const String& Group, bool Recursive)
    {
        this->OgreResource->addResourceLocation(Location, GetStringFromArchiveType(Type), Group, Recursive);
        AddAssetGroupName(Group);
    }

    void ResourceManager::CreateAssetGroup(const String& GroupName)
    {
        this->OgreResource->createResourceGroup(GroupName);
        AddAssetGroupName(GroupName);
    }

    void ResourceManager::DestroyAssetGroup(const String& GroupName)
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

    void ResourceManager::DeclareAsset(const String& Name, const String& Type, const String& Group)
    {
        this->OgreResource->declareResource(Name, Type, Group);
    }

    void ResourceManager::InitAssetGroup(const String& Group)
    {
        this->OgreResource->initialiseResourceGroup(Group);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Resource/Asset Query

    String ResourceManager::GetAssetPath(const String& FileName, const String& Group)
    {
        Ogre::FileInfoListPtr FileList = this->OgreResource->listResourceFileInfo(Group);
        for( Whole X = 0 ; X < FileList->size() ; ++X )
        {
            if( FileName == FileList->at(X).filename )
            {
                return FileList->at(X).path;
            }
        }
        return "";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    String ResourceManager::GetPluginExtension() const
    {
        #ifdef WINDOWS
                return ".dll";
        #elif LINUX
                return ".so";
        #elif MACOSX
                return ".dylib";
        #endif
    }

    ResourceInputStream* ResourceManager::GetResourceStream(const String& FileName)
    {
        #ifdef MEZZDEBUG
        Entresol::GetSingletonPtr()->Log("Entering ResourceManager::GetResourceStream(const String& FileName)");
        #endif
        Internal::OgreDataStreamBuf *TempBuffer = new Internal::OgreDataStreamBuf(OgreResource->openResource(FileName));
        ResourceInputStream *Results =  new ResourceInputStream(TempBuffer, this);
        this->DeleteList.push_back(Results);
        #ifdef MEZZDEBUG
        Entresol::GetSingletonPtr()->Log("Exiting ResourceManager::GetResourceStream(const String& FileName)");
        #endif
        return Results;
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
                MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION, "Invalid archive type passed to ResourceManager::GetStringFromArchiveType.");
                return String("");
        }
    }

    Mezzanine::ArchiveType ResourceManager::GetArchiveTypeFromString(const String& FromString)
    {
        if(String("FileSystem")==FromString)
            { return Mezzanine::AT_FileSystem; }
        if(String("Zip")==FromString)
            { return Mezzanine::AT_Zip; }
        MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION, "Invalid archive type passed to ResourceManager::GetArchiveTypeFromString.");
        return AT_Invalid;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited from ManagerBase

    void ResourceManager::Initialize()
    {
        Initialized = true;
    }

    void ResourceManager::DoMainLoopItems()
    {
    }

    ManagerBase::ManagerType ResourceManager::GetInterfaceType() const
        { return ManagerBase::ResourceManager; }

    String ResourceManager::GetImplementationTypeName() const
        { return "DefaultResourceManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultResourceManagerFactory Methods

    DefaultResourceManagerFactory::DefaultResourceManagerFactory()
    {
    }

    DefaultResourceManagerFactory::~DefaultResourceManagerFactory()
    {
    }

    String DefaultResourceManagerFactory::GetManagerTypeName() const
    {
        return "DefaultResourceManager";
    }

    ManagerBase* DefaultResourceManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(ResourceManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return ResourceManager::GetSingletonPtr();
        }else{
            if(Params.empty()) return new ResourceManager();
            else
            {
                String EngineDataPath;
                ArchiveType ArchiveType_;
                for( NameValuePairList::iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
                {
                    String Lower = (*ParIt).first;
                    StringTools::ToLowerCase(Lower);
                    if( "enginedatapath" == Lower )
                    {
                        EngineDataPath = (*ParIt).second;
                    }
                    else if( "archivetype" == Lower )
                    {
                        ArchiveType_ = ResourceManager::GetArchiveTypeFromString((*ParIt).second);
                    }
                }
                return new ResourceManager(EngineDataPath,ArchiveType_);
            }
        }
    }

    ManagerBase* DefaultResourceManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(ResourceManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return ResourceManager::GetSingletonPtr();
        }else return new ResourceManager(XMLNode);
    }

    void DefaultResourceManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
