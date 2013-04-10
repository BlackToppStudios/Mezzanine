//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _graphicsskeleton_cpp
#define _graphicsskeleton_cpp

#include "Graphics/skeleton.h"
#include "Graphics/bone.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is used to store the internal structures needed by the Skeleton class.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB InternalSkeletonData
        {
            public:
                Ogre::SkeletonPtr GraphicsSkeleton;
        };//InternalSkeletonData

        ///////////////////////////////////////////////////////////////////////////////
        // Skeleton Methods

        Skeleton::Skeleton(Ogre::SkeletonPtr InternalSkeleton)
        {
            this->ISD = new InternalSkeletonData();
            this->ISD->GraphicsSkeleton = InternalSkeleton;

            Ogre::Skeleton::BoneIterator BoneIt = this->ISD->GraphicsSkeleton->getBoneIterator();
            while( BoneIt.hasMoreElements() )
            {
                Ogre::Bone* InternalBone = BoneIt.getNext();
                Bones.push_back( this->_CreateBoneWrapper( InternalBone ) );
            }
        }

        Skeleton::~Skeleton()
        {
            delete ISD;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        ///////////////////////////////////////////////////////////////////////////////
        // Bone Methods

        UInt16 Skeleton::GetNumBones() const
        {
            return Bones.size();
        }

        BoneIterator Skeleton::BoneBegin()
        {
            return Bones.begin();
        }

        BoneIterator Skeleton::BoneEnd()
        {
            return Bones.end();
        }

        ConstBoneIterator Skeleton::BoneBegin() const
        {
            return Bones.begin();
        }

        ConstBoneIterator Skeleton::BoneEnd() const
        {
            return Bones.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Animation Methods

        /// @todo Implement these

        ///////////////////////////////////////////////////////////////////////////////
        // Asset Methods

        ConstString& Skeleton::GetName() const
        {
            _GetInternalSkeleton()->getName();
        }

        ConstString& Skeleton::GetGroup() const
        {
            _GetInternalSkeleton()->getGroup();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Bone* Skeleton::_CreateBoneWrapper(Ogre::Bone* InternalBone)
        {
            Bone* NewBone = new Bone(this,InternalBone);
            Bones.push_back(NewBone);
            return NewBone;
        }

        Ogre::SkeletonPtr Skeleton::_GetInternalSkeleton() const
        {
            return this->ISD->GraphicsSkeleton;
        }
    }//Graphics
}//Mezzanine

#endif
