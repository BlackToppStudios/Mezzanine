//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _resourcemanager_cpp
#define _resourcemanager_cpp

#include <iostream>
#include <fstream>

#include <Ogre.h>
#include <btBulletWorldImporter.h>
#include <btBulletDynamicsCommon.h>

#include "resourcemanager.h"
#include "actorbase.h"
#include "ogredatastreambuf.h.cpp"

#define PHYSDEBUG
#ifdef PHYSDEBUG
#include "world.h"
#endif

namespace phys {

    ResourceManager::ResourceManager(World* _World) : ManagerBase(_World)
    {
        this->Priority = 20;
        OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();

    }

    ResourceManager::~ResourceManager()
    {
        for(std::vector<ResourceInputStream*>::iterator Iter = DeleteList.begin(); Iter != DeleteList.end(); Iter++)
            { delete *Iter; }
    }

    void ResourceManager::ApplyShapeToActor(ActorBase* Actor, btCollisionShape* ColShape)
    {
        if(ActorBase::Actorrigid == Actor->GetType())
        {
            btRigidBody* Rigid = static_cast < btRigidBody* > (Actor->CollisionObject);
            btScalar mass = Rigid->getInvMass();
            mass=1/mass;
            btVector3 inertia(0,0,0);
            ColShape->calculateLocalInertia(mass, inertia);
            Rigid->setMassProps(mass,inertia);
        }
        if(ActorBase::Actorragdoll == Actor->GetType())
        {
        }
        if(!Actor->ShapeIsSaved)
        {
            delete Actor->Shape;
        }
        Actor->CollisionObject->setCollisionShape(ColShape);
        Actor->Shape = ColShape;
    }

    bool ResourceManager::ExportShapeData(ActorBase* Actor, const String &FileName)
    {
        //Copy pasta'd from Erwin Coumans with permission
        btDefaultSerializer* Serializer = new btDefaultSerializer(1024*1024*5);
        Serializer->startSerialization();
        int len = Actor->Shape->calculateSerializeBufferSize();
        btChunk* chunk = Serializer->allocate(len,1);
        const char* structType = Actor->Shape->serialize(chunk->m_oldPtr, Serializer);
        Serializer->finalizeChunk(chunk,structType,BT_SHAPE_CODE,Actor->Shape);
        Serializer->finishSerialization();
        FILE* f2 = fopen(FileName.c_str(),"wb");
        size_t verify = fwrite(Serializer->getBufferPointer(),Serializer->getCurrentBufferSize(),1,f2);
        fclose(f2);
        if(1 != verify)
        {
            return false;
        }
        return true;
    }

    bool ResourceManager::ImportShapeData(ActorBase* Actor, const String &FileName)
    {
        btBulletWorldImporter* Importer = new btBulletWorldImporter();

        //Check root directory first
        if(Importer->loadFile(FileName.c_str()))
        {
            btCollisionShape* shape = Importer->getCollisionShapeByIndex(0);
            if(shape)
            {
                ApplyShapeToActor(Actor, shape);
                delete Importer;
                return true;
            }
            delete Importer;
            return false;
        }

        Ogre::DataStreamPtr Stream = OgreResource->openResource(FileName);
        assert(sizeof(char)==1);
        char* buffer = new char[Stream->size()];
        Stream->read((void*)buffer, Stream->size());
        if(Importer->loadFileFromMemory(buffer, Stream->size()))
        {
            btCollisionShape* shape = Importer->getCollisionShapeByIndex(0);
            if(shape)
            {
                ApplyShapeToActor(Actor, shape);
                delete buffer;
                delete Importer;
                return true;
            }
            delete buffer;
            delete Importer;
            return false;
        }

        delete buffer;
        delete Importer;
        return false;
    }

    void ResourceManager::AddResourceLocation(const String &Location, const String &Type, const String &Group, const bool &recursive)
    {
        this->OgreResource->addResourceLocation(Location, Type, Group, recursive);
    }

    void ResourceManager::DeclareResource(const String& Name, const String& Type, const String& Group)
    {
        this->OgreResource->declareResource(Name, Type, Group);
    }

    void ResourceManager::InitResourceGroup(const String& Group)
    {
        this->OgreResource->initialiseResourceGroup(Group);
    }

    ResourceInputStream* ResourceManager::GetResourceStream(const String& FileName)
    {
        #ifdef PHYSDEBUG
        World::GetWorldPointer()->Log("Entering ResourceManager::GetResourceStream(const String& FileName)");
        #endif
        internal::OgreDataStreamBuf *TempBuffer = new internal::OgreDataStreamBuf(OgreResource->openResource(FileName));
        ResourceInputStream *Results =  new ResourceInputStream(TempBuffer, this);
        this->DeleteList.push_back(Results);
        #ifdef PHYSDEBUG
        World::GetWorldPointer()->Log("Exiting ResourceManager::GetResourceStream(const String& FileName)");
        #endif
        return Results;
    }

    void ResourceManager::Initialize()
    {
    }

    void ResourceManager::DoMainLoopItems()
    {
    }

    ManagerBase::ManagerTypeName ResourceManager::GetType() const
        { return ManagerBase::ResourceManager; }
}

#endif
