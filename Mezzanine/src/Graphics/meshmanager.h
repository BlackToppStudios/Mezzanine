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
#ifndef _graphicsmeshmanager_h
#define _graphicsmeshmanager_h

#include "vector3.h"
#include "colourvalue.h"
#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"

namespace Ogre
{
    class MeshPtr;
    class MeshManager;
}

namespace Mezzanine
{
    namespace Physics
    {
        class CollisionShape;
    }
    namespace Graphics
    {
        class Mesh;
        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonMeshManager) Singleton<MeshManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @class MeshManager
        /// @brief This manager handles the storage, generation, and query of of Graphics Meshes.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MeshManager : public EntresolManager, public Singleton<MeshManager>
        {
        public:
            /// @brief Basic container type for Mesh storage in this class.
            typedef std::map< String, Mesh* >              MeshContainer;
            /// @brief Iterator type for Mesh instances stored in this class.
            typedef MeshContainer::iterator                MeshIterator;
            /// @brief Const Iterator type for Mesh instances stored in this class.
            typedef MeshContainer::const_iterator          ConstMeshIterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @internal
            /// @brief Container storing all of the currently loaded Meshes.
            MeshContainer Meshes;

            /// @internal
            /// @brief Adds a Mesh to this manager.
            /// @exception If the name of the Mesh being added is not unique a II_DUPLICATE_IDENTITY_EXCEPTION will be thrown.
            /// @param ToAdd The Mesh to be added.
            virtual void AddMesh(Mesh* ToAdd);
        public:
            /// @brief Class constructor.
            MeshManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            MeshManager(XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~MeshManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Mesh Management

            /// @brief Loads a Mesh file from disk and prepares it for use.
            /// @param MeshName The name of the Mesh file to be loaded.
            /// @param Group The resource group from which the Mesh file should be loaded.
            /// @return Returns a pointer to the loaded Mesh.
            virtual Mesh* LoadMesh(const String& MeshName, const String& Group);
            /// @brief Unloads a Mesh file.
            /// @param MeshName The name of the Mesh to be unloaded.
            virtual void UnloadMesh(const String& MeshName);
            /// @brief Gets a Mesh stored in this manager.
            /// @param MeshName The name of the Mesh to retrieve.
            /// @return Returns a pointer to the requested Mesh.
            Mesh* GetMesh(const String& MeshName);
            /// @brief Gets the number of currently loaded meshes.
            /// @return Returns a Whole representing the number of meshes currently loaded.
            virtual Whole GetNumMeshes();
            /// @brief Unloads every Mesh that is currently loaded.
            virtual void UnloadAllMeshes();

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
            /// @brief Wraps and stores an Ogre Mesh instance.
            /// @param ToWrap The Ogre Mesh to get wrapped.
            /// @return Returns a pointer to the wrapped Mesh.
            virtual Mesh* _WrapInternalMesh(Ogre::MeshPtr ToWrap);
            /// @internal
            /// @brief Gets the internal MeshManager.
            /// @return Returns a pointer to the internal MeshManager.
            Ogre::MeshManager* _GetInternalManager() const;
        };//MeshManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultMeshManagerFactory
        /// @brief A factory responsible for the creation and destruction of the default MeshManager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultMeshManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultMeshManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultMeshManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(NameValuePairList&)
            EntresolManager* CreateManager(NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(XML::Node&)
            EntresolManager* CreateManager(XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        };//DefaultMeshManagerFactory
    }//Graphics
}//Mezzanine

#endif
