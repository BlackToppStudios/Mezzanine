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
#ifndef _graphicsskeletonmanager_h
#define _graphicsskeletonmanager_h

#include "datastream.h"
#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"

namespace Ogre
{
    class Resource;
    typedef std::shared_ptr<Resource> ResourcePtr;
    class Skeleton;
    typedef std::shared_ptr<Skeleton> SkeletonPtr;
    class SkeletonManager;
}

namespace Mezzanine
{
    namespace Physics
    {
        class CollisionShape;
    }
    namespace Graphics
    {
        class Skeleton;
        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonSkeletonManager) Singleton<SkeletonManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This manager handles the storage, generation, and query of of Graphics Skeletons.
        ///////////////////////////////////////
        class MEZZ_LIB SkeletonManager : public EntresolManager, public Singleton<SkeletonManager>
        {
        public:
            /// @brief Basic container type for Skeleton storage in this class.
            typedef std::map< String, Skeleton* >            SkeletonContainer;
            /// @brief Iterator type for Skeleton instances stored in this class.
            typedef SkeletonContainer::iterator              SkeletonIterator;
            /// @brief Const Iterator type for Skeleton instances stored in this class.
            typedef SkeletonContainer::const_iterator        ConstSkeletonIterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @brief Container storing all of the currently loaded Skeletons.
            SkeletonContainer Skeletons;

            /// @brief Adds a Skeleton to this manager.
            /// @exception If the name of the Skeleton being added is not unique a II_DUPLICATE_IDENTITY_EXCEPTION will be thrown.
            /// @param ToAdd The Skeleton to be added.
            virtual void AddSkeleton(Skeleton* ToAdd);
        public:
            /// @brief Class constructor.
            SkeletonManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            SkeletonManager(const XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~SkeletonManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Skeleton Management

            /// @brief Creates a new blank Skeleton that can be built upon.
            /// @param Name The new to be given to the new Skeleton.
            /// @return Returns a pointer to the created Skeleton.
            Skeleton* CreateSkeleton(const String& Name);

            /// @brief Gets a Skeleton stored in this manager.
            /// @param Name The name of the Skeleton to retrieve.
            /// @return Returns a pointer to the requested Skeleton.
            Skeleton* GetSkeleton(const String& Name);
            /// @brief Gets the number of currently loaded Skeletons.
            /// @return Returns a Whole representing the number of Skeletons currently loaded.
            Whole GetNumSkeletons();

            /// @brief Removes a Skeleton from this manager.
            /// @remarks Due to the shared use of Skeletons in objects this method cannot outright destroy
            /// any Skeleton object.  Instead it can remove the Skeleton from this manager and it will be
            /// destroyed when no objects reference it.
            /// @param ToBeRemoved The Skeleton to be removed from this manager.
            void RemoveSkeleton(Skeleton* ToBeRemoved);
            /// @brief Removes every Skeleton from this manager.
            /// @remarks Due to the shared use of Skeletons in objects this method cannot outright destroy
            /// any Skeleton object.  Instead it can remove the Skeleton from this manager and it will be
            /// destroyed when no objects reference it.
            void RemoveAllSkeletons();


            ///////////////////////////////////////////////////////////////////////////////
            // Skeleton I/O

            /// @brief Loads a Skeleton file from an asset group and prepares it for use.
            /// @param AssetIdentifier The identifier of the Skeleton file to be loaded.
            /// @param GroupName The resource group from which the Skeleton file should be loaded.
            /// @return Returns a pointer to the loaded Skeleton.
            Skeleton* LoadSkeleton(const String& AssetIdentifier, const String& GroupName);
            /// @brief Loads a Skeleton file from disk and prepares it for use.
            /// @param LocalPath The full path and filename of the Skeleton to be read on the local disk.
            /// @return Returns a pointer to the loaded Skeleton.
            Skeleton* LoadSkeleton(const String& LocalPath);

            /// @brief Loads an Skeleton from an input stream.
            /// @param Stream The stream to load the Skeleton from.
            /// @return Returns a pointer to the loaded Skeleton.
            Skeleton* LoadSkeleton(std::istream& Stream);
            /// @brief Loads an Skeleton from an input stream.
            /// @param Stream A pointer to the stream to load the Skeleton from.
            /// @return Returns a pointer to the loaded Skeleton.
            Skeleton* LoadSkeleton(IStreamPtr Stream);

            /// @brief Writes a Skeleton to the asset group.
            /// @param ToSave The Skeleton to be saved.
            /// @param AssetIdentifier The identifier of the file to save the Skeleton as.
            /// @param GroupName The name of the asset group to save the Skeleton to.
            void SaveSkeleton(Skeleton* ToSave, const String& AssetIdentifier, const String& GroupName);
            /// @brief Writes a Skeleton to the disk.
            /// @param ToSave The Skeleton to be saved.
            /// @param LocalPath The full path and filename of the Skeleton to be written.
            void SaveSkeleton(Skeleton* ToSave, const String& LocalPath);

            /// @brief Writes a Skeleton in a final serializable form to an output stream.
            /// @param ToSave The Skeleton to be saved.
            /// @param Stream The stream to save the Skeleton to.
            void SaveSkeleton(Skeleton* ToSave, std::ostream& Stream);
            /// @brief Writes a Skeleton in a final serializable form to an output stream.
            /// @param ToSave The Skeleton to be saved.
            /// @param Stream A pointer to the stream to save the Skeleton to.
            void SaveSkeleton(Skeleton* ToSave, OStreamPtr Stream);

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
            /// @brief Wraps and stores an Ogre Skeleton instance.
            /// @param ToWrap The Ogre Skeleton to get wrapped.
            /// @return Returns a pointer to the wrapped Skeleton.
            Skeleton* _WrapInternalSkeleton(Ogre::SkeletonPtr ToWrap);
            /// @internal
            /// @brief Gets the internal SkeletonManager.
            /// @return Returns a pointer to the internal SkeletonManager.
            Ogre::SkeletonManager* _GetInternalManager() const;
        };//SkeletonManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultSkeletonManagerFactory
        /// @brief A factory responsible for the creation and destruction of the default SkeletonManager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultSkeletonManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultSkeletonManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultSkeletonManagerFactory();

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
        };//DefaultSkeletonManagerFactory
    }//Graphics
}//Mezzanine

#endif
