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
#ifndef _resourcemanager_cpp
#define _resourcemanager_cpp

#include <iostream>
#include <fstream>

#include <Ogre.h>
#include <btBulletWorldImporter.h>
#include <btBulletDynamicsCommon.h>

#include "resourcemanager.h"
#include "actorbase.h"
#include "internalogredatastreambuf.h.cpp"
#include "internalbulletfilemanager.h.cpp"

#ifdef PHYSDEBUG
#include "world.h"
#endif

namespace phys {

    ResourceManager::ResourceManager()
    {
        this->Priority = 20;
        OgreResource = Ogre::ResourceGroupManager::getSingletonPtr();
        internal::BulletFileManager* BulletFileMan = internal::BulletFileManager::getSingletonPtr();
        //internal::BulletFileManager* BulletFileMan = new internal::BulletFileManager();
    }

    ResourceManager::~ResourceManager()
    {
        for(std::vector<ResourceInputStream*>::iterator Iter = DeleteList.begin(); Iter != DeleteList.end(); Iter++)
            { delete *Iter; }
    }

    void ResourceManager::AddResourceGroupName(String Name)
    {
        for( Whole X = 0 ; X < ResourceGroups.size() ; X++ )
        {
            if(Name == ResourceGroups[X])
                return;
        }
        ResourceGroups.push_back(Name);
    }

    void ResourceManager::ApplyShapeToActor(ActorBase* Actor, btCollisionShape* ColShape)
    {
        if(ActorBase::Actorrigid == Actor->GetType())
        {
            btRigidBody* Rigid = static_cast < btRigidBody* > (Actor->CollisionObject);
            btScalar mass = Rigid->getInvMass();
            if(0 != mass)
                mass=1/mass;
            btVector3 inertia(0,0,0);
            ColShape->calculateLocalInertia(mass, inertia);
            Rigid->setMassProps(mass,inertia);
            Rigid->updateInertiaTensor();
        }
        if(ActorBase::Actorcharacter == Actor->GetType())
        {
        }
        if(!Actor->ShapeIsSaved)
        {
            delete Actor->Shape;
        }
        Actor->CollisionObject->setCollisionShape(ColShape);
        Actor->Shape = ColShape;
    }

    void ResourceManager::AddResourceLocation(const String &Location, const String &Type, const String &Group, const bool &recursive)
    {
        this->OgreResource->addResourceLocation(Location, Type, Group, recursive);
        AddResourceGroupName(Group);
    }

    void ResourceManager::DestroyResourceGroup(const String& GroupName)
    {
        this->OgreResource->destroyResourceGroup(GroupName);
        for( std::vector<String>::iterator it = ResourceGroups.begin() ; it != ResourceGroups.end() ; it++ )
        {
            if(GroupName == (*it))
            {
                ResourceGroups.erase(it);
                return;
            }
        }
    }

    void ResourceManager::DeclareResource(const String& Name, const String& Type, const String& Group)
    {
        this->OgreResource->declareResource(Name, Type, Group);
    }

    void ResourceManager::InitResourceGroup(const String& Group)
    {
        this->OgreResource->initialiseResourceGroup(Group);
    }

    void ResourceManager::ParseMaterialScripts()
    {
        Ogre::MaterialManager* OgreMatMan = Ogre::MaterialManager::getSingletonPtr();
        for( Whole X = 0 ; X < ResourceGroups.size() ; X++ )
        {
            Ogre::DataStreamListPtr MatFiles = OgreResource->openResources("*.material",ResourceGroups[X]);
            for( std::list<Ogre::DataStreamPtr>::iterator it = MatFiles->begin() ; it != MatFiles->end() ; it++ )
            {
                OgreMatMan->parseScript((*it),ResourceGroups[X]);
            }
        }
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
