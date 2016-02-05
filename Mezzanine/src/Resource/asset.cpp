// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _resourceasset_cpp
#define _resourceasset_cpp

#include "Resource/asset.h"
#include "Resource/resourceutilities.h"
#include "Resource/resourcemanager.h"

#include "exception.h"

namespace Mezzanine
{
    namespace Resource
    {
        Asset::Asset()
            {  }

        Asset::~Asset()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Loading Methods

        void Asset::LoadAsset(const String& FileName, const String& GroupName)
        {
            ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
            if( ResourceMan == NULL ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to perform IO with AssetGroups when the resource system has not been created/initialized.");
            }
            // We should fetch the stream via the singleton pointer here, however may need to rethink how this is approached for large files or slow IO where loading may need to be split up between multiple frames.
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Loading via asset groups is not supported yet.");
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Saving Methods

        void Asset::SaveAsset(const String& FileName, const String& GroupName)
        {
            ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
            if( ResourceMan == NULL ) {
                MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to perform IO with AssetGroups when the resource system has not been created/initialized.");
            }
            // This is awaiting the addition of write directories to asset groups.
            MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Saving via asset groups is not supported yet.");
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Loading Methods

        void Asset::_LoadAsset(const String& FilePath, const String& FileName)
            { this->_LoadAsset(Resource::CombinePathAndFileName(FilePath,FileName)); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Saving Methods

        void Asset::_SaveAsset(const String& FilePath, const String& FileName)
            { this->_SaveAsset(Resource::CombinePathAndFileName(FilePath,FileName)); }
    }//Resource
}//Mezzanine

#endif
