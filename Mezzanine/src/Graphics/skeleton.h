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
#ifndef _graphicsskeleton_h
#define _graphicsskeleton_h

#include "datatypes.h"

namespace Ogre
{
    class Bone;
    class Skeleton;
    class SkeletonPtr;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Bone;
        class InternalSkeletonData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class encapsulates the Skeletal animation functionality of a Mesh.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Skeleton
        {
            public:
                typedef std::vector<Bone*>                  BoneContainer;
                typedef BoneContainer::iterator             BoneIterator;
                typedef BoneContainer::const_iterator       ConstBoneIterator;
                typedef std::map<String,Bone*>              NamedBoneContainer;
                typedef NamedBoneContainer::iterator        NamedBoneIterator;
                typedef NamedBoneContainer::const_iterator  ConstNamedBoneIterator;
            protected:
                InternalSkeletonData* ISD;
                BoneContainer Bones;
            public:
                /// @internal
                /// @brief Internal constructor.
                /// @param InternalSkeleton The internal skeleton this skeleton is based upon.
                Skeleton(Ogre::SkeletonPtr InternalSkeleton);
                /// @brief Class destructor.
                ~Skeleton();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility Methods

                ///////////////////////////////////////////////////////////////////////////////
                // Bone Methods

                /// @brief Gets the number of bones contained in this skeleton.
                /// @return Returns a UInt16 representing the number of bones in this skeleton.
                UInt16 GetNumBones() const;

                /// @brief Gets an iterator to the first Bone.
                /// @return Returns an iterator to the first Bone being stored by this Skeleton.
                BoneIterator BoneBegin();
                /// @brief Gets an iterator to one passed the last Bone.
                /// @return Returns an iterator to one passed the last Bone being stored by this Skeleton.
                BoneIterator BoneEnd();
                /// @brief Gets a const iterator to the first Bone.
                /// @return Returns a const iterator to the first Bone being stored by this Skeleton.
                ConstBoneIterator BoneBegin() const;
                /// @brief Gets an iterator to one passed the last Bone.
                /// @return Returns an iterator to one passed the last Bone being stored by this Skeleton.
                ConstBoneIterator BoneEnd() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Animation Methods

                /// @todo Implement these

                ///////////////////////////////////////////////////////////////////////////////
                // Asset Methods

                /// @brief Gets the Name of this Skeleton.
                /// @note If this Skeleton originated from a file, usually the name of the Skeleton will be the file name.
                /// @return Returns a const string reference containing the name of this Skeleton.
                ConstString& GetName() const;
                /// @brief Gets the resource group this Skeleton belongs to.
                /// @return Returns a const string reference containing the group this Skeleton belongs to.
                ConstString& GetGroup() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Creates a bone wrapper for a pre-made internal bone.
                /// @param InternalBone The internal bone this Bone is based on.
                Bone* _CreateBoneWrapper(Ogre::Bone* InternalBone);
                /// @internal
                /// @brief Gets the internal Skeleton pointer.
                /// @return Returns a shared pointer pointing to the internal Skeleton.
                Ogre::SkeletonPtr _GetInternalSkeleton() const;
        };//Skeleton
    }//Graphics
}//Mezzanine

#endif
