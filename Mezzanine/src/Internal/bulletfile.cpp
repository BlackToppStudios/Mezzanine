//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _internalbulletfile_cpp
#define _internalbulletfile_cpp

#include "Internal/bulletfile.h.cpp"
#include <btBulletWorldImporter.h>

namespace Mezzanine
{
    namespace Internal
    {
        BulletFile::BulletFile(Ogre::ResourceManager* creator, const String& name, Ogre::ResourceHandle handle, const String& group, bool isManual, Ogre::ManualResourceLoader* loader)
            : Ogre::Resource(creator, name, handle, group, isManual, loader),
              KnownFileSize(0)
        {
            createParamDictionary("BulletFile");
        }

        BulletFile::~BulletFile()
        {
            unload();
        }

        void BulletFile::loadImpl()
        {
            btBulletWorldImporter* Importer = new btBulletWorldImporter();
            Ogre::DataStreamPtr Stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
            KnownFileSize = Stream->size();
            assert(sizeof(char)==1);
            char* buffer = new char[KnownFileSize];
            Stream->read((void*)buffer, KnownFileSize);
            if(Importer->loadFileFromMemory(buffer, KnownFileSize))
            {
                for( Whole X = 0 ; X < Importer->getNumCollisionShapes() ; X++ )
                {
                    btCollisionShape* Shape = Importer->getCollisionShapeByIndex(X);
                    BulletShapes.push_back(std::pair<String,btCollisionShape*>(Importer->getNameForPointer((void*)Shape),Shape));
                }
                for( Whole X = 0 ; X < Importer->getNumRigidBodies() ; X++ )
                {
                    btCollisionObject* Body = Importer->getRigidBodyByIndex(X);
                    BulletBodies.push_back(std::pair<String,btCollisionObject*>(Importer->getNameForPointer((void*)Body),Body));
                }
                for( Whole X = 0 ; X < Importer->getNumConstraints() ; X++ )
                {
                    btTypedConstraint* Constraint = Importer->getConstraintByIndex(X);
                    BulletConstraints.push_back(std::pair<String,btTypedConstraint*>(Importer->getNameForPointer((void*)Constraint),Constraint));
                }
                for( Whole X = 0 ; X < Importer->getNumBvhs() ; X++ )
                {
                    btOptimizedBvh* BVH = Importer->getBvhByIndex(X);
                    BulletBVHs.push_back(std::pair<String,btOptimizedBvh*>(Importer->getNameForPointer((void*)BVH),BVH));
                }
                for( Whole X = 0 ; X < Importer->getNumTriangleInfoMaps() ; X++ )
                {
                    btTriangleInfoMap* TriInfoMap = Importer->getTriangleInfoMapByIndex(X);
                    BulletTriangleInfoMaps.push_back(std::pair<String,btTriangleInfoMap*>(Importer->getNameForPointer((void*)TriInfoMap),TriInfoMap));
                }
            }else{
            }
        }

        void BulletFile::unloadImpl()
        {
            BulletShapes.clear();
            BulletBodies.clear();
            BulletConstraints.clear();
            BulletBVHs.clear();
            BulletTriangleInfoMaps.clear();
        }

        size_t BulletFile::calculateSize() const
        {
            return KnownFileSize;
        }

        btCollisionShape* BulletFile::GetShape(Whole Index)
        {
            return BulletShapes[Index].second;
        }

        btCollisionShape* BulletFile::GetShape(String Name)
        {
            for( std::vector<std::pair<String,btCollisionShape*> >::iterator CSI = BulletShapes.begin() ; CSI != BulletShapes.end() ; CSI++ )
            {
                if( Name == (*CSI).first )
                    return (*CSI).second;
            }
        }

        btCollisionObject* BulletFile::GetRigidBody(Whole Index)
        {
            return BulletBodies[Index].second;
        }

        btCollisionObject* BulletFile::GetRigidBody(String Name)
        {
            for( std::vector<std::pair<String,btCollisionObject*> >::iterator RBI = BulletBodies.begin() ; RBI != BulletBodies.end() ; RBI++ )
            {
                if( Name == (*RBI).first )
                    return (*RBI).second;
            }
        }

        btTypedConstraint* BulletFile::GetConstraint(Whole Index)
        {
            return BulletConstraints[Index].second;
        }

        btTypedConstraint* BulletFile::GetConstraint(String Name)
        {
            for( std::vector<std::pair<String,btTypedConstraint*> >::iterator TCI = BulletConstraints.begin() ; TCI != BulletConstraints.end() ; TCI++ )
            {
                if( Name == (*TCI).first )
                    return (*TCI).second;
            }
        }

        btOptimizedBvh* BulletFile::GetBVH(Whole Index)
        {
            return BulletBVHs[Index].second;
        }

        btOptimizedBvh* BulletFile::GetBVH(String Name)
        {
            for( std::vector<std::pair<String,btOptimizedBvh*> >::iterator BVHI = BulletBVHs.begin() ; BVHI != BulletBVHs.end() ; BVHI++ )
            {
                if( Name == (*BVHI).first )
                    return (*BVHI).second;
            }
        }

        btTriangleInfoMap* BulletFile::GetTriangleInfoMap(Whole Index)
        {
            return BulletTriangleInfoMaps[Index].second;
        }

        btTriangleInfoMap* BulletFile::GetTriangleInfoMap(String Name)
        {
            for( std::vector<std::pair<String,btTriangleInfoMap*> >::iterator TIMI = BulletTriangleInfoMaps.begin() ; TIMI != BulletTriangleInfoMaps.end() ; TIMI++ )
            {
                if( Name == (*TIMI).first )
                    return (*TIMI).second;
            }
        }
    }//internal
}//Mezzanine

#endif
