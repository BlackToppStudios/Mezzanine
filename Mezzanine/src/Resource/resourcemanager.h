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
#ifndef _resourcemanager_h
#define _resourcemanager_h

#include "datastream.h"
#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "eventpublisher.h"
#include "singleton.h"

#include "Resource/resourceenumerations.h"

/// @file
/// @brief This file contains the definition of the Resource Manager.

namespace Mezzanine
{
    namespace Resource
    {
        class AssetGroup;

        // Used by the scripting language binder to help create bindings for this class.
        #ifdef SWIG
        %template(SingletonResourceManager) Singleton<ResourceManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the interface class for all resource management listeners.
        /// @remarks Adding a callback for Stream creation was considered, but given the potential frequency of stream
        /// creation it may have too great of a negative performance impact to allow.
        ///////////////////////////////////////
        class MEZZ_LIB ResourceEventListener
        {
        public:
            /// @brief The type to use for uniquely identifying ResourceEventListener instances.
            using IDType = EventSubscriberID;

            /// @brief Gets the ID of this listener.
            /// @return Returns an EventSubscriberID uniquely identifying this listener.
            virtual IDType GetID() const = 0;

            /// @brief Notifies the listener that a new location is being added to an AssetGroup.
            /// @param Location The identifier (usually a path) of the location being added.
            /// @param Type The type of group the location is being added to.
            /// @param GroupName The name of the group the location is being added to.
            virtual void AssetLocationAdded(const String& Location, const ArchiveType Type, const String& GroupName) = 0;
            /// @brief Notifies the listener that an AssetGroup has been created.
            /// @param GroupName The name of the group being created.
            virtual void AssetGroupCreated(const String& GroupName) = 0;
            /// @brief Notifies the listener that an AssetGroup will be destroyed.
            /// @param GroupName The name of the group being destroyed.
            virtual void AssetGroupDestroyed(const String& GroupName) = 0;
        };//ResourceEventListener

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A config/traits class for configuration of the event publisher used by a ResourceManager.
        ///////////////////////////////////////
        struct MEZZ_LIB ResourceEventTableConfig : public EventSubscriptionTableConfig< ResourceEventListener* >
        {
            ///////////////////////////////////////////////////////////////////////////////
            // Factory Traits

            /// @brief Use bindings to help manage lifetime where appropriate.
            static const SubscriptionFactoryType FactoryType = SubscriptionFactoryType::SFT_Binding;

            ///////////////////////////////////////////////////////////////////////////////
            // Storage Traits

            /// @brief Use a dynamic sizing unsorted container because we want minimum restrictions for the subscribers.
            static const SubscriptionContainerType ContainerType = SubscriptionContainerType::SCT_Unsorted;

            ///////////////////////////////////////////////////////////////////////////////
            // Dispatch Traits

            /// @brief Don't need anything fancy for the dispatcher (yet).
            static const EventDispatcherType DispatcherType = EventDispatcherType::EDT_Empty;
            /// @brief EventID works fine for dispatch.
            using DispatchIDType = EventID;
        };//ResourceEventTableConfig

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the manager responsible for the loading and unloading of files.
        /// @details This class is responsible for the reading and writing of files of all kinds, be
        /// it graphical meshes, physics data, or XMl files.
        ///////////////////////////////////////
        class MEZZ_LIB ResourceManager : public EntresolManager, public Singleton<ResourceManager>
        {
        public:
            /// @brief Basic container type for AssetGroup storage in this class.
            using AssetGroupContainer = std::vector<AssetGroup*>;
            /// @brief Iterator type for AssetGroup instances stored in this class.
            using AssetGroupIterator = AssetGroupContainer::iterator;
            /// @brief Const Iterator type for AssetGroup instances stored in this class.
            using ConstAssetGroupIterator = AssetGroupContainer::const_iterator;

            /// @brief Convenience type for the event publisher config used by ResourceManagers.
            using ResourceEventPublisher = EventSubscriptionTable<ResourceEventTableConfig>;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @brief Event publisher used to dispatch high level resource events.
            ResourceEventPublisher ResourcePublisher;
            /// @brief Container storing all of the asset groups created and managed by this manager.
            AssetGroupContainer AssetGroups;
            /// @brief The location of engine data.
            String EngineDataDir;
            /// @brief Whether or not asset lookup should be restricted to explicitly specified groups.
            /// @remarks If false, this will permit searching for an asset in other groups if it wasn't
            /// found in the group specified.
            Boole LimitedSearches = true;
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
            // AssetGroup Management

            /// @brief Adds a location for graphical resources.
            /// @param Location The location on the file system the asset can be found.
            /// @param Type The kind of file system the location can be found in.
            /// @param GroupName The name of the group that will have it's locations updated.
            void AddAssetLocation(const String& Location, const ArchiveType Type, const String& GroupName);

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

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Management

            /// @brief Opens an asset from an archive location in this group.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier The full path within the group specified to the resource.
            /// @param GroupName The AssetGroup to open the Asset from.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr OpenAsset(const String& Identifier,
                                 const String& GroupName,
                                 const Whole Flags = SF_Read,
                                 const Boole Raw = false);
            /// @brief Opens an asset from a location in this group and pre-loads it all into a memory buffer.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier The full path within the group specified to the resource.
            /// @param GroupName The AssetGroup to open the Asset from.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr BufferAsset(const String& Identifier,
                                   const String& GroupName,
                                   const Whole Flags = SF_Read,
                                   const Boole Raw = false);
            /// @brief Does the same as OpenAsset, but will mark the asset as a child for another asset.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param ParentIdentifier The full path within the parent group specified to the parent resource.
            /// @param ParentGroupName The AssetGroup to open the parent Asset from.
            /// @param ChildIdentifier The full path within the child group specified to the child resource.
            /// @param ChildGroupName The AssetGroup to open the child Asset from.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr OpenChildAsset(const String& ParentIdentifier,
                                      const String& ParentGroupName,
                                      const String& ChildIdentifier,
                                      const String& ChildGroupName,
                                      const Whole Flags = SF_Read,
                                      const Boole Raw = false);
            /// @brief Does the same as BufferAsset, but will mark the asset as a child for another asset.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param ParentIdentifier The full path within the parent group specified to the parent resource.
            /// @param ParentGroupName The AssetGroup to open the parent Asset from.
            /// @param ChildIdentifier The full path within the child group specified to the child resource.
            /// @param ChildGroupName The AssetGroup to open the child Asset from.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr BufferChildAsset(const String& ParentIdentifier,
                                        const String& ParentGroupName,
                                        const String& ChildIdentifier,
                                        const String& ChildGroupName,
                                        const Whole Flags = SF_Read,
                                        const Boole Raw = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Encrypted Stream Management

            /// @brief Opens an encrypted asset from an archive location in this group.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier The full path within the group specified to the resource.
            /// @param GroupName The AssetGroup to open the Asset from.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr OpenEncryptedAsset(const String& Identifier,
                                          const String& GroupName,
                                          const String& Password,
                                          const Whole Flags = SF_Read,
                                          const Boole Raw = false);
            /// @brief Opens an encrypted asset from a location in this group and pre-loads it all into a memory buffer.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param Identifier The full path within the group specified to the resource.
            /// @param GroupName The AssetGroup to open the Asset from.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr BufferEncryptedAsset(const String& Identifier,
                                            const String& GroupName,
                                            const String& Password,
                                            const Whole Flags = SF_Read,
                                            const Boole Raw = false);
            /// @brief Does the same as OpenEncryptedAsset, but will mark the asset as a child for another asset.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param ParentIdentifier The full path within the parent group specified to the parent resource.
            /// @param ParentGroupName The AssetGroup to open the parent Asset from.
            /// @param ChildIdentifier The full path within the child group specified to the child resource.
            /// @param ChildGroupName The AssetGroup to open the child Asset from.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr OpenEncryptedChildAsset(const String& ParentIdentifier,
                                               const String& ParentGroupName,
                                               const String& ChildIdentifier,
                                               const String& ChildGroupName,
                                               const String& Password,
                                               const Whole Flags = SF_Read,
                                               const Boole Raw = false);
            /// @brief Does the same as BufferEncryptedAsset, but will mark the asset as a child for another asset.
            /// @warning This will completely load the asset into memory.  Be mindful of file sizes.
            /// @remarks Locations are not searched in any particular order, the first match will be returned.
            /// @param ParentIdentifier The full path within the parent group specified to the parent resource.
            /// @param ParentGroupName The AssetGroup to open the parent Asset from.
            /// @param ChildIdentifier The full path within the child group specified to the child resource.
            /// @param ChildGroupName The AssetGroup to open the child Asset from.
            /// @param Password The password necessary to decrypt the asset.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for more info.
            /// @param Raw If true, the stream will perform no processing on the raw data before returning.
            /// @return Returns a shared pointer to an IStream to the opened asset.
            IStreamPtr BufferEncryptedChildAsset(const String& ParentIdentifier,
                                                 const String& ParentGroupName,
                                                 const String& ChildIdentifier,
                                                 const String& ChildGroupName,
                                                 const String& Password,
                                                 const Whole Flags = SF_Read,
                                                 const Boole Raw = false);

            ///////////////////////////////////////////////////////////////////////////////
            // Asset Query

            /// @brief Gets the actual path to an asset.
            /// @note This function currently only returns the first match, and doesn't check for multiple matches.
            /// @param FileName The name of the file to search for.
            /// @param GroupName The asset group to search in for the file.
            /// @return Returns a string containing the path to the file, or an empty string if no such file was found.
            String GetAssetPath(const String& FileName, const String& GroupName);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Get the pathname where engine data is stored.
            /// @return A String that contains the path to where the engine data is stored.
            const String& GetEngineDataDirectory() const;

            /// @brief Sets whether or not asset searching will be limited to a group specified.
            /// @remarks The searches this method (and the accompanying get method) are referring to is any method on
            /// this manager where you specify an asset as well as it's group.  Including but not limited to all the
            /// open methods on this manager.
            /// @param Limited If false, assets will be searched for in all groups until found.
            void SetLimitedSearches(const Boole Limited);
            /// @brief Gets whether or not asset searching will be limited to a group specified.
            /// @return Returns true if only the asset group specified will be searched, false otherwise.
            Boole GetLimitedSearches() const;

            /// @brief Gets the dot-and-extension of this platforms plugins.
            /// @return Returns the platform appropriate extension for plugin files.
            static String GetPlatformPluginExtension();

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
