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
#ifndef _resourcemanager_h
#define _resourcemanager_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "managerbase.h"
#include "singleton.h"
#include "resourceinputstream.h"

class btBulletWorldImporter;

namespace Ogre
{
    class ResourceGroupManager;
}

namespace Mezzanine
{
    class ActorBase;

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonResourceManager) Singleton<ResourceManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ResourceManager
    /// @headerfile resourcemanager.h
    /// @brief This is the manager responsible for the loading and unloading of files.
    /// @details This class is responsible for the reading and writing of files of all kinds, be
    /// it graphical meshes, physics data, or XMl files.
    ///////////////////////////////////////
    class MEZZ_LIB ResourceManager : public ManagerBase, public Singleton<ResourceManager>
    {
        protected:
            /// @brief Encapsulates the functionality of the ogre resource group manager.
            Ogre::ResourceGroupManager* OgreResource;

            /// A list of Pointers to streams created to delete periodically.
            std::vector<ResourceInputStream*> DeleteList;

            std::vector<String> ResourceGroups;
            void AddResourceGroupName(String Name);

            /// @brief The location of engine data
            String EngineDataDir;

        public:
            /// @brief Class constructor.
            /// @details Standard manager constructor.
            /// @param EngineDataPath The directory for engine specific data.
            ResourceManager(const String& EngineDataPath);
#ifdef MEZZXML
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            ResourceManager(xml::Node& XMLNode);
#endif
            /// @details Class Destructor.
            ~ResourceManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Directory Management

            /// @brief Creates a new directory.
            /// @param DirectoryPath The path for the newly created directory.
            /// @return Returns true if the directory was created, false if it failed.
            bool CreateDirectory(const String& DirectoryPath);
            /// @brief Get a Listing of the files and subdirectories in a directory.
            /// @details This follows normal command line conventions, "." is the current directory,
            /// ".." is the parent directory. To access the file system root you will need to use a
            /// leading "c:/", "c:\\", or "/" as appropriate for the operating system the software will run on.
            /// @return This will return a pointer to a set of Strings the caller is responsible for deleting or a null pointer on an error.
            /// @param Dir The directory to check.
            std::set<String>* GetDirContents(const String& Dir = ".");
            /// @brief Get the working directory as a Mezzanine::String
            /// @return The Directory the game was called from (not nescessarilly the location of the executable), as a Mezzanine::String
            String GetWorkingDirectory() const;
            /// @brief Get the pathname where engine data is stored
            /// @return A String that contains the pathname
            String GetEngineDataDirectory() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Management

            ///////////////////////////////////////////////////////////////////////////////
            // Resource Management

            /// @brief Adds a location for graphical resources.
            /// @details This function will add a location on the disk to find files needed to create and
            /// manipulate graphical objects. Once a resource is added it must be initalized using
            /// ResourceManager::InitResourceGroup(String Group).
            /// @param Location The location on the file system the resource can be found.
            /// @param Type The kind of file system the location can be found in. @n
            /// Options are: filesystem, zip.
            /// @param Group The name of the group the resources at this location belong to.  If the group does not exist it will be created.
            /// @param recursive Whether or not to search sub-directories.
            void AddResourceLocation(const String& Location, const String& Type, const String& Group, const bool recursive=false);
            /// @brief Creates a resource group.
            /// @param GroupName The name to be given to the created resource group.
            void CreateResourceGroup(const String& GroupName);
            /// @brief Destroys a resource group, unloading all of it's resources.
            /// @param GroupName The name of the resource group to destroy.
            void DestroyResourceGroup(const String& GroupName);
            /// @brief Prepares the resource for use.
            /// @details This function can be thought of as a preloader.  This will prepare the defined
            /// resource located on the disk for use.
            /// @param Name Name of the file/resource to be 'prepared'.
            /// @param Type The type of resource that the file is. @n
            /// Options are: Font, GpuProgram, HighLevelGpuProgram, Material, Mesh, Skeleton, Texture.
            /// @param Group Name of the group the resource belongs to.
            void DeclareResource(const String& Name, const String& Type, const String& Group);
            /// @brief Makes a resource group ready to use.
            /// @details After adding all of your resources and declaring them as nessessary, this function
            /// is the final step.  After calling this function any and all resources within the defined group
            /// will be ready to use.  Do not initialize any more groups then you need to however, as that will
            /// take up memory and drop performance.
            /// @param Name Name of the resource group.
            void InitResourceGroup(const String& Name);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the dot-and-extention of this platforms plugins.
            /// @return Returns the platform appropriate extention for plugin files.
            String GetPluginExtension() const;

            /// @brief Get a stream to read from the specified file
            /// @param FileName The name of the File you want to stream data from
            /// @return An derivative of std::istream a ResourceInputStream that will pull it's data from the desired resource
            /// @details The returned ResourceInputStream is the Caller's responsibility to deal with. If it is not deleted it is a memory leak.
            ResourceInputStream* GetResourceStream(const String& FileName);

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from ManagerBase

            /// @brief Empty Initializor
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason.
            void Initialize();
            /// @brief Empty MainLoopItems
            /// @details This class implements this for the sake of entension and compatibility this function does nothing. This is just empty during this round of refactoring,
            /// and this will get all the functionality that currently should be here, but is in the world.
            void DoMainLoopItems();
            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::ResourceManager
            ManagerBase::ManagerTypeName GetType() const;
    };
}

#endif
