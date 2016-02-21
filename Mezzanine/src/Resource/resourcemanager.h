// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#include "datatypes.h"
#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"
#include "Resource/resourceenumerations.h"
#include "Resource/datastream.h"

/// @file
/// @brief The defintion of the Resource Manager.

namespace Ogre
{
    class ResourceGroupManager;
}

namespace Mezzanine
{
    namespace Resource
    {
        class AssetGroup;

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
        class MEZZ_LIB ResourceManager : public EntresolManager, public Singleton<ResourceManager>
        {
        public:
            /// @brief Basic container type for AssetGroup storage in this class.
            typedef std::map<String,AssetGroup*>               AssetGroupContainer;
            /// @brief Iterator type for AssetGroup instances stored in this class.
            typedef AssetGroupContainer::iterator              AssetGroupIterator;
            /// @brief Const Iterator type for AssetGroup instances stored in this class.
            typedef AssetGroupContainer::const_iterator        ConstAssetGroupIterator;

            /// @brief Basic container type for @ref DataStream storage by this class.
            typedef std::vector<Resource::DataStreamPtr>       DataStreamContainer;
            /// @brief Iterator type for @ref DataStream instances stored by this class.
            typedef DataStreamContainer::iterator              DataStreamIterator;
            /// @brief Const Iterator type for @ref DataStream instances stored by this class.
            typedef DataStreamContainer::const_iterator        ConstDataStreamIterator;
            /// @brief Basic container type for named @ref DataStream storage by this class.
            typedef std::map<String,Resource::DataStreamPtr>   NamedDataStreamContainer;
            /// @brief Iterator type for named @ref DataStream instances stored by this class.
            typedef NamedDataStreamContainer::iterator         NamedDataStreamIterator;
            /// @brief Const Iterator type for named @ref DataStream instances stored by this class.
            typedef NamedDataStreamContainer::const_iterator   ConstNamedDataStreamIterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @internal
            /// @brief Container storing all of the asset groups created and managed by this manager.
            AssetGroupContainer AssetGroups;
            /// @internal
            /// @brief The location of engine data.
            String EngineDataDir;
            /// @internal
            /// @brief Encapsulates the functionality of the ogre resource group manager.
            Ogre::ResourceGroupManager* OgreResource;
        public:
            /// @brief Class constructor.
            /// @details Standard manager constructor.
            /// @param EngineDataPath The directory for engine specific data.
            /// @param ArchiveType The name of the type of archive at this path.
            ResourceManager(const String& EngineDataPath = ".", const ArchiveType ArchType = AT_FileSystem);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            ResourceManager(const XML::Node& XMLNode);
            /// @details Class Destructor.
            virtual ~ResourceManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Management

            /// @brief Opens a stream to an asset in an AssetGroup.
            /// @param AssetName The identity of the asset to be opened (commonly a file name).
            /// @param GroupName The name of the AssetGroup where the Asset can be found.
            Resource::DataStreamPtr OpenAssetStream(const String& AssetName, const String& GroupName);

            /// @brief Creates a stream from a memory buffer.
            /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
            /// @param Buffer A pointer to the memory to stream from.
            /// @param BufferSize The size of the provided buffer in bytes.
            /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
            Resource::DataStreamPtr CreateDataStream(void* Buffer, const UInt32 BufferSize);
            /// @brief Creates a named stream from a memory buffer.
            /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
            /// @param AssetName The name to be given to the created stream.
            /// @param Buffer A pointer to the memory to stream from.
            /// @param BufferSize The size of the provided buffer in bytes.
            /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
            Resource::DataStreamPtr CreateDataStream(const String& AssetName, void* Buffer, const UInt32 BufferSize);
            /// @brief Creates a named stream from a memory buffer and adds it to the named AssetGroup.
            /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
            /// @param AssetName The name to be given to the created stream.
            /// @param GroupName The name of the AssetGroup this stream will be added to.
            /// @param Buffer A pointer to the memory to stream from.
            /// @param BufferSize The size of the provided buffer in bytes.
            /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
            Resource::DataStreamPtr CreateDataStream(const String& AssetName, const String& GroupName, void* Buffer, const UInt32 BufferSize);

            ///////////////////////////////////////////////////////////////////////////////
            // AssetGroup Management

            /// @brief Adds a location for graphical resources.
            /// @details This function will add a location on the disk to find files needed to create and
            /// manipulate graphical objects. Once an asset is added it must be initalized using
            /// ResourceManager::InitResourceGroup(String Group).
            /// @param Location The location on the file system the asset can be found.
            /// @param Type The kind of file system the location can be found in. @n
            /// Options are: filesystem, zip.
            /// @param Group The name of the group the resources at this location belong to.  If the group does not exist it will be created.
            /// @param Recursive Whether or not to search sub-directories.
            void AddAssetLocation(const String& Location, const ArchiveType Type, const String& Group, const Boole Recursive = false);

            /// @brief Creates a new asset group.
            /// @param GroupName The name to be given to the created asset group.
            /// @return Returns a pointer to the created AssetGroup.
            AssetGroup* CreateAssetGroup(const String& GroupName);
            /// @brief Gets an AssetGroup by name.  If it does not exist it will be created.
            /// @param GroupName The name of the group to create or retrieve.
            /// @return Returns a pointer to the requested AssetGroup.
            AssetGroup* GetOrCreateAssetGroup(const String& GroupName);
            /// @brief Gets an AssetGroup by name.
            /// @param GroupName The name of the AssetGroup to retrieve.
            /// @return Returns a pointer to the specified group, or NULL if it does not exist.
            AssetGroup* GetAssetGroup(const String& GroupName);
            /// @brief Gets an AssetGroup by name.
            /// @exception If the requested AssetGroup is not found this will throw an exception.
            /// @param GroupName The name of the AssetGroup to retrieve.
            /// @return Returns a pointer to the specified group.
            AssetGroup* GetAssetGroupExcept(const String& GroupName);
            /// @brief Destroys an asset group, unloading all of it's resources.
            /// @param GroupName The name of the asset group to destroy.
            void DestroyAssetGroup(const String& GroupName);
            /// @brief Destroys an asset group, unloading all of it's resources.
            /// @param ToBeDestroyed A pointer to the asset group to be destroyed.
            void DestroyAssetGroup(AssetGroup* ToBeDestroyed);
            /// @brief Destroys all asset groups being stored by this manager.
            void DestroyAllAssetGroups();

            /// @brief Makes a asset group ready to use.
            /// @details After adding all of your assets and declaring them as nessessary, this function
            /// is the final step.  After calling this function any and all assets within the defined group
            /// will be ready to use.  Do not initialize any more groups then you need to however, as that will
            /// take up memory and drop performance.
            /// @param GroupName Name of the asset group.
            void InitAssetGroup(const String& GroupName);

            ///////////////////////////////////////////////////////////////////////////////
            // Asset Query

            /// @brief Gets the actual path to an asset.
            /// @note This function currently only returns the first match, and doesn't check for multiple matches.
            /// @param FileName The name of the file to search for.
            /// @param Group The asset group to search in for the file.
            /// @return Returns a string containing the path to the file.
            String GetAssetPath(const String& FileName, const String& Group);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Get the pathname where engine data is stored.
            /// @return A String that contains the path to where the engine data is stored.
            String GetEngineDataDirectory() const;

            /// @brief Gets the dot-and-extention of this platforms plugins.
            /// @return Returns the platform appropriate extention for plugin files.
            String GetPluginExtension() const;

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            /// @brief Gets a string that describes an @ref ArchiveType.
            /// @param ArchType A @ref ArchiveType That you want to log or pass to Ogre, or just need a @ref String that represents it.
            /// @return A String that represents the @ref ArchiveType passed.
            static String GetStringFromArchiveType(const Resource::ArchiveType ArchType);
            /// @brief Gets an @ref ArchiveType from a string.
            /// @param FromString The string to be converted to an archive type.
            /// @return Returns a @ref ArchiveType corresponding to the string provided, or AT_Invalid if it is invalid.
            static Resource::ArchiveType GetArchiveTypeFromString(const String& FromString);

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
        };//ResourceManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultResourceManagerFactory
        /// @headerfile resourcemanager.h
        /// @brief A factory responsible for the creation and destruction of the default resourcemanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultResourceManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultResourceManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultResourceManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(const NameValuePairList&)
            EntresolManager* CreateManager(const NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(const XML::Node&)
            EntresolManager* CreateManager(const XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        };//DefaultResourceManagerFactory
    }//Resource
}//Mezzanine

#endif
