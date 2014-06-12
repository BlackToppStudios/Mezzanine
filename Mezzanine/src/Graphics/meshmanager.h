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
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"

namespace Ogre
{
    class MeshPtr;
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
        /// @headerfile meshmanager.h
        /// @brief This manager handles the storage, generation, and query of of Graphics Meshes.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MeshManager : public ManagerBase, public Singleton<MeshManager>
        {
        public:
            /// @brief Basic container type for Mesh storage in this class.
            typedef std::map< String, Mesh* >              MeshContainer;
            /// @brief Iterator type for Mesh instances stored in this class.
            typedef MeshContainer::iterator                MeshIterator;
            /// @brief Const Iterator type for Mesh instances stored in this class.
            typedef MeshContainer::const_iterator          ConstMeshIterator;
        protected:
            /// @internal
            /// @brief Container storing all of the currently loaded meshes.
            MeshContainer Meshes;

            /// @internal
            /// @brief Adds a mesh to this manager.
            /// @exception If the name of the mesh being added is not unique a II_DUPLICATE_IDENTITY_EXCEPTION will be thrown.
            /// @param ToAdd The mesh to be added.
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
            // Non-Generated Mesh Management

            /// @brief Loads a mesh file from disk and prepares it for use.
            /// @return Returns a pointer to the loaded mesh.
            /// @param MeshName The name of the mesh file to be loaded.
            /// @param Group The resource group from which the mesh file should be loaded.
            virtual Mesh* LoadMesh(const String& MeshName, const String& Group);
            /// @brief Unloads a mesh file.
            /// @param MeshName The name of the mesh to be unloaded.
            virtual void UnloadMesh(const String& MeshName);
            /// @brief Gets a mesh stored in this manager.
            /// @return Returns a pointer to the requested mesh.
            /// @param MeshName The name of the mesh to retrieve.
            Mesh* GetMesh(const String& MeshName);
            /// @brief Gets the number of currently loaded meshes.
            /// @return Returns a whole representing the number of meshes currently loaded.
            virtual Whole GetNumMeshes();
            /// @brief Unloads every mesh that is currently loaded.
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
            /// @brief Wraps and stores an Ogre mesh instance.
            /// @param ToWrap The Ogre mesh to get wrapped.
            /// @return Returns a pointer to the wrapped Mesh.
            virtual Mesh* _WrapInternalMesh(Ogre::MeshPtr ToWrap);
        };//MeshManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultMeshManagerFactory
        /// @headerfile meshmanager.h
        /// @brief A factory responsible for the creation and destruction of the default meshmanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultMeshManagerFactory : public ManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultMeshManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultMeshManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;

            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
        };//DefaultMeshManagerFactory
    }//Graphics
}//Mezzanine

#endif
