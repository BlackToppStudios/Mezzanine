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
#ifndef _manualmaterialloader_h_cpp
#define _manualmaterialloader_h_cpp

// Keeps this file from being documented by doxygen
/// @cond DontDocumentInternal

#include "datatypes.h"

#include <OgreResource.h>

namespace Mezzanine
{
    namespace Resource
    {
        class ResourceManager;
    }
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A manual loader for the loading of Material files into Ogre.
        /// @details ManualResourceLoaders in Ogre are meant to facilitate the loading of resources in
        /// any custom way that may be needed.  This specific implementation is meant to direct the
        /// loading of materials through the Mezzanine resource system, bypassing Ogre's system entirely.
        /// @n @n
        /// When creating an instance of a manual loader, it must remain valid and initialized for
        /// the entire duration of any and all resources that were loaded using in, just in case the
        /// resource needs to be reloaded.
        ///////////////////////////////////////
        class MEZZ_LIB ManualMaterialLoader : public Ogre::ManualResourceLoader
        {
        protected:
            /// @brief A pointer to the ResourceManager we're loading from.
            Resource::ResourceManager* ResourceMan = nullptr;
        public:
            /// @brief Class constructor.
            /// @param Manager A pointer to the manager providing streams to load from.
            ManualMaterialLoader(Resource::ResourceManager* Manager);
            /// @brief Class destructor.
            virtual ~ManualMaterialLoader() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Overrides

            /// @brief Loads a resource into a ready state.
            /// @param resource The resource to be populated.
            virtual void loadResource(Ogre::Resource* resource) override;
        };//ManualMaterialManager
    }//Graphics
}//Mezzanine

/// @endcond

#endif
