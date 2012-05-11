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
#ifndef _internalbulletfile_h_cpp
#define _internalbulletfile_h_cpp

#include "datatypes.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Internal
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @class BulletFile
        /// @headerfile internalbulletfile.h.cpp
        /// @brief This is an internal resource class that allows the integration of the .bullet file format into the Ogre resource system.
        /// @details Using this class (and it's accompanying manager), it should be possible to enable .bullet files to be loaded in the
        /// background through Ogre's resource system.
        ///////////////////////////////////////
        class BulletFile : public Ogre::Resource
        {
            protected:
                size_t KnownFileSize;
                void loadImpl();
                void unloadImpl();
                size_t calculateSize() const;

                std::vector<std::pair<String,btCollisionShape*> > BulletShapes;
                std::vector<std::pair<String,btCollisionObject*> > BulletBodies;
                std::vector<std::pair<String,btTypedConstraint*> > BulletConstraints;
                std::vector<std::pair<String,btOptimizedBvh*> > BulletBVHs;
                std::vector<std::pair<String,btTriangleInfoMap*> > BulletTriangleInfoMaps;
            public:
                /// @brief Resource Constructor.
                /// @param creator The Resource Manager that created this Resource.
                /// @param name The Name of this Resource.
                /// @param handle The unique resource identifier belonging to this resource.
                /// @param group The name of the group this resource can be found.
                /// @param isManual Whether or not this resource should be loaded with a manual loader.
                /// @param loader If "isManual" is true, this is a pointer to the manual loader that will be used to load this resource.
                BulletFile(Ogre::ResourceManager* creator, const String& name, Ogre::ResourceHandle handle, const String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);
                /// @brief Class Destructor.
                virtual ~BulletFile();
                /// @brief Gets a physics shape by index.
                /// @param Index The index of the physics shape.
                /// @return Returns a pointer to the specified physics shape.
                btCollisionShape* GetShape(Whole Index);
                /// @brief Gets a physics shape by name.
                /// @param Name The name of the physics shape.
                /// @return Returns a pointer to the specified physics shape.
                btCollisionShape* GetShape(String Name);
                /// @brief Gets a physics body by index.
                /// @param Index The index of the physics body.
                /// @return Returns a pointer to the specified physics body.
                btCollisionObject* GetRigidBody(Whole Index);
                /// @brief Gets a physics body by name.
                /// @param Name The name of the physics body.
                /// @return Returns a pointer to the specified physics body.
                btCollisionObject* GetRigidBody(String Name);
                /// @brief Gets a physics constraint by index.
                /// @param Index The index of the physics constraint.
                /// @return Returns a pointer to the specified physics constraint.
                btTypedConstraint* GetConstraint(Whole Index);
                /// @brief Gets a physics constraint by name.
                /// @param Name The name of the physics constraint.
                /// @return Returns a pointer to the specified physics constraint.
                btTypedConstraint* GetConstraint(String Name);
                /// @brief Gets a physics acceleration structure by index.
                /// @param Index The index of the physics acceleration structure.
                /// @return Returns a pointer to the specified physics acceleration structure.
                btOptimizedBvh* GetBVH(Whole Index);
                /// @brief Gets a physics acceleration structure by name.
                /// @param Name The name of the physics acceleration structure.
                /// @return Returns a pointer to the specified physics acceleration structure.
                btOptimizedBvh* GetBVH(String Name);
                /// @brief Gets a physics triangle info map by index.
                /// @param Index The index of the physics triangle info map.
                /// @return Returns a pointer to the specified physics triangle info map.
                btTriangleInfoMap* GetTriangleInfoMap(Whole Index);
                /// @brief Gets a physics triangle info map by name.
                /// @param Name The name of the physics triangle info map.
                /// @return Returns a pointer to the specified physics triangle info map.
                btTriangleInfoMap* GetTriangleInfoMap(String Name);
        };//bulletfile

        /// @internal
        class BulletFilePtr : public Ogre::SharedPtr<BulletFile>
        {
            protected:
            public:
                BulletFilePtr() : Ogre::SharedPtr<BulletFile>() {};
                explicit BulletFilePtr(BulletFile *rep) : Ogre::SharedPtr<BulletFile>(rep) {};
                BulletFilePtr(const BulletFilePtr &r) : Ogre::SharedPtr<BulletFile>(r) {};
                BulletFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<BulletFile>()
                {
                    if( r.isNull() )
                        return;
                    // lock & copy other mutex pointer
                    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                    pRep = static_cast<BulletFile*>(r.getPointer());
                    pUseCount = r.useCountPointer();
                    useFreeMethod = r.freeMethod();
                    if (pUseCount)
                    {
                        ++(*pUseCount);
                    }
                }
                BulletFilePtr& operator=(const Ogre::ResourcePtr& r)
                {
                    if(pRep == static_cast<BulletFile*>(r.getPointer()))
                        return *this;
                    release();
                    if( r.isNull() )
                        return *this; // resource ptr is null, so the call to release above has done all we need to do.
                    // lock & copy other mutex pointer
                    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
                    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                    pRep = static_cast<BulletFile*>(r.getPointer());
                    pUseCount = r.useCountPointer();
                    useFreeMethod = r.freeMethod();
                    if (pUseCount)
                    {
                        ++(*pUseCount);
                    }
                    return *this;
                }
        };//bulletfileptr
    }//internal
}//Mezzanine

#endif
