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
#ifndef _internalbulletfilemanager_cpp
#define _internalbulletfilemanager_cpp

#include "internalbulletfilemanager.h.cpp"

namespace Mezzanine
{
    namespace internal
    {
       /* template<> BulletFileManager* Ogre::Singleton<BulletFileManager>::ms_Singleton = 0;

        BulletFileManager::BulletFileManager()
        {
            mResourceType = "BulletFile";

            mLoadOrder = 250.0f;

            Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType,this);
        }

        BulletFileManager::~BulletFileManager()
        {
            Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
        }

        Ogre::Resource* BulletFileManager::createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual,
                                                      Ogre::ManualResourceLoader* loader, const Ogre::NameValuePairList* createParams)
        {
            BulletFile* BFile = new BulletFile(this,name,handle,group,isManual,loader);
            return BFile;
        }

        BulletFilePtr BulletFileManager::load(const Ogre::String& name, const Ogre::String& group)
        {
            BulletFilePtr BFile = getByName(name);

            if(BFile.isNull())
                BFile = create(name,group);

            BFile->load();
            return BFile;
        }

        BulletFileManager& BulletFileManager::getSingleton()
        {
            assert(ms_Singleton);
            return *ms_Singleton;
        }

        BulletFileManager* BulletFileManager::getSingletonPtr()
        {
            return ms_Singleton;
        }*/

    }//internal
}//Mezzanine

#endif
