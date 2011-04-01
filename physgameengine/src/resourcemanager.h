//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#include "resourceinputstream.h"

#include <istream>
#include <vector>

class btBulletWorldImporter;

namespace Ogre
{
    class ResourceGroupManager;
}

namespace phys
{
    class ActorBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ResourceManager
    /// @headerfile resourcemanager.h
    /// @brief This is the manager responsible for the loading and unloading of files.
    /// @details This class is responsible for the reading and writing of files of all kinds, be
    /// it graphical meshes, physics data, or XMl files.
    ///////////////////////////////////////
    class PHYS_LIB ResourceManager : public ManagerBase {
        protected:
            /// @brief Encapsulates the functionality of the ogre resource group manager.
            Ogre::ResourceGroupManager* OgreResource;

            /// @brief Applies a saved shape to an Actor.
            void ApplyShapeToActor(ActorBase* Actor, btCollisionShape* ColShape);

            /// A list of Pointers to streams created to delete periodically.
            std::vector<ResourceInputStream*> DeleteList;

            std::vector<String> ResourceGroups;
            void AddResourceGroupName(String Name);

        public:
            /// @brief Class constructor.
            /// @details Standard manager constructor.
            ResourceManager();
            /// @details Class Destructor.
            ~ResourceManager();

            /// @brief Exports an actors shape data to a file.
            /// @param FileName The Filename to save the data too. Remember to include a ".bullet" extension to the filename when serializing.
            /// @param Actor The Actor's Shape to save
            bool ExportShapeData(ActorBase* Actor, const String &FileName);
            /// @brief Imports serialized shape data from the disk to be used in an Actor.
            /// @param FileName The Filename to load the data from. Remember to include a ".bullet" extension to the filename when serializing.
            /// @param Actor The Actor's Shape to restore from file
            bool ImportShapeData(ActorBase* Actor, const String &FileName);

            /// @brief Adds a location for graphical resources.
            /// @details This function will add a location on the disk to find files needed to create and
            /// manipulate graphical objects. Once a resource is added it must be initalized using
            /// ResourceManager::InitResourceGroup(String Group).
            /// @param Location The location on the file system the resource can be found.
            /// @param Type The kind of file system the location can be found in. @n
            /// Options are: filesystem, zip.
            /// @param Group The name of the group the resources at this location belong to.
            /// @param recursive Whether or not to search sub-directories.
            void AddResourceLocation(const String &Location, const String &Type, const String &Group, const bool &recursive=false);
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

            /// @brief Parses all Ogre Material scripts.
            void ParseMaterialScripts();

            /// @brief Get a stream to read from the specified file
            /// @param FileName The name of the File you want to stream data from
            /// @return An derivative of std::istream a ResourceInputStream that will pull it's data from the desired resource
            /// @details The returned ResourceInputStream is the Caller's responsibility to deal with. If it is not deleted it is a memory leak.
            ResourceInputStream* GetResourceStream(const String& FileName);

            ////Functions inherited from ManagerBase
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
