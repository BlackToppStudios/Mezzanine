// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#ifndef _graphicsmaterial_h
#define _graphicsmaterial_h

#include "datatypes.h"

namespace Ogre
{
    class Resource;
    typedef std::shared_ptr<Resource> ResourcePtr;
    class Material;
    typedef std::shared_ptr<Material> MaterialPtr;
}//Ogre

namespace Mezzanine
{
    namespace Graphics
    {
        class ManualMaterialLoader;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents all the rendering passes a rendered object has.
        /// @details Materials are composed of techniques which define all the passes an object can have.
        /// Materials can be attached to any renderable geometry and are quite flexible.
        ///////////////////////////////////////
        class MEZZ_LIB Material
        {
        protected:
            /// @brief A pointer to the internal data this Material is based on.
            Ogre::MaterialPtr InternalMaterial;
            /// @brief A pointer to the loader that will make the Material data available to the graphics system.
            ManualMaterialLoader* InternalLoader;
        public:
            /// @internal
            /// @brief Internal Constructor.
            /// @param ToWrap The internal Material the instance will wrap.
            /// @param Loader The loader that will make the Material data available to the graphics system.
            Material(Ogre::MaterialPtr ToWrap, ManualMaterialLoader* Loader);
            /// @brief Class Destructor.
            ~Material();



            ///////////////////////////////////////////////////////////////////////////////
            // Asset Query

            /// @brief Gets the Name of this Material.
            /// @note If this Material originated from a file, usually the name of the Material will be the file name.
            /// @return Returns a const string reference containing the name of this Material.
            const String& GetName() const;
            /// @brief Gets the resource group this Material belongs to.
            /// @return Returns a const string reference containing the group this Material belongs to.
            const String& GetGroup() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the internal Material pointer.
            /// @return Returns a shared pointer pointing to the internal Material.
            Ogre::MaterialPtr _GetInternalMaterial() const;
            /// @internal
            /// @brief Casts an internal resource pointer to a MaterialPtr.
            /// @param ToCast The pointer to be casted.
            /// @return Returns the casted MaterialPtr.
            static Ogre::MaterialPtr _Upcast(Ogre::ResourcePtr ToCast);
        };//Material
    }//Graphics
}//Mezzanine

#endif
