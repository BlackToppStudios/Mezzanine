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
#ifndef _graphicsmaterialmanager_h
#define _graphicsmaterialmanager_h

#include "datastream.h"
#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"

namespace Ogre
{
    class Resource;
    typedef std::shared_ptr<Resource> ResourcePtr;
    class Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    class MaterialManager;
}

namespace Mezzanine
{
    namespace Physics
    {
        class CollisionShape;
    }
    namespace Graphics
    {
        class Material;
        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonMaterialManager) Singleton<MaterialManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This manager handles the storage, generation, and query of of Graphics Materials.
        ///////////////////////////////////////
        class MEZZ_LIB MaterialManager : public EntresolManager, public Singleton<MaterialManager>
        {
        public:
            /// @brief Basic container type for Material storage in this class.
            typedef std::map< String, Material* >             MaterialContainer;
            /// @brief Iterator type for Material instances stored in this class.
            typedef MaterialContainer::iterator               MaterialIterator;
            /// @brief Const Iterator type for Material instances stored in this class.
            typedef MaterialContainer::const_iterator         ConstMaterialIterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @brief Container storing all of the currently loaded Materials.
            MaterialContainer Materials;

            /// @brief Adds a Material to this manager.
            /// @exception If the name of the Material being added is not unique a II_DUPLICATE_IDENTITY_EXCEPTION will be thrown.
            /// @param ToAdd The Material to be added.
            void AddMaterial(Material* ToAdd);
        public:
            /// @brief Class constructor.
            MaterialManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            MaterialManager(const XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~MaterialManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Material Management

            /// @brief Creates a new blank Material that can be built upon.
            /// @param Name The new to be given to the new Material.
            /// @return Returns a pointer to the created Material.
            Material* CreateMaterial(const String& Name);

            /// @brief Gets a Material stored in this manager.
            /// @param Name The name of the Material to retrieve.
            /// @return Returns a pointer to the requested Material.
            Material* GetMaterial(const String& Name);
            /// @brief Gets the number of currently loaded Materials.
            /// @return Returns a Whole representing the number of Materials currently loaded.
            Whole GetNumMaterials();

            /// @brief Removes a Material from this manager.
            /// @remarks Due to the shared use of Materials in objects this method cannot outright destroy
            /// any Material object.  Instead it can remove the Material from this manager and it will be
            /// destroyed when no objects reference it.
            /// @param ToBeRemoved The Material to be removed from this manager.
            void RemoveMaterial(Material* ToBeRemoved);
            /// @brief Removes every Material from this manager.
            /// @remarks Due to the shared use of Materials in objects this method cannot outright destroy
            /// any Material object.  Instead it can remove the Material from this manager and it will be
            /// destroyed when no objects reference it.
            void RemoveAllMaterials();


            ///////////////////////////////////////////////////////////////////////////////
            // Material I/O

            /// @brief Loads a Material file from an asset group and prepares it for use.
            /// @param AssetIdentifier The identifier of the Material file to be loaded.
            /// @param GroupName The resource group from which the Material file should be loaded.
            /// @return Returns a pointer to the loaded Material.
            Material* LoadMaterial(const String& AssetIdentifier, const String& GroupName);
            /// @brief Loads a Material file from disk and prepares it for use.
            /// @param LocalPath The full path and filename of the Material to be read on the local disk.
            /// @return Returns a pointer to the loaded Material.
            Material* LoadMaterial(const String& LocalPath);
            /// @brief Loads an Material from an input stream.
            /// @param Stream A pointer to the stream to load the Material from.
            /// @return Returns a pointer to the loaded Material.
            Material* LoadMaterial(IStreamPtr Stream);

            /// @brief Writes a Material to the asset group.
            /// @param ToSave The Material to be saved.
            /// @param AssetIdentifier The identifier of the file to save the Material as.
            /// @param GroupName The name of the asset group to save the Material to.
            void SaveMaterial(Material* ToSave, const String& AssetIdentifier, const String& GroupName);
            /// @brief Writes a Material to the disk.
            /// @param ToSave The Material to be saved.
            /// @param LocalPath The full path and filename of the Material to be written.
            void SaveMaterial(Material* ToSave, const String& LocalPath);
            /// @brief Writes a Material in a final serializable form to an output stream.
            /// @param ToSave The Material to be saved.
            /// @param Stream A pointer to the stream to save the Material to.
            void SaveMaterial(Material* ToSave, OStreamPtr Stream);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
            /// @internal
            /// @brief Gets the internal MaterialManager.
            /// @return Returns a pointer to the internal MaterialManager.
            Ogre::MaterialManager* _GetInternalManager() const;
        };//MaterialManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultMaterialManagerFactory
        /// @brief A factory responsible for the creation and destruction of the default MaterialManager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultMaterialManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultMaterialManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultMaterialManagerFactory();

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
        };//DefaultMaterialManagerFactory
    }//Graphics
}//Mezzanine

#endif
