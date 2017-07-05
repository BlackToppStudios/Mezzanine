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

#ifndef _terrain_h
#define _terrain_h

#include "entity.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class from which all the actors inherit.
    /// @details The terrain classes are responsible for large collections of static objects in the
    /// 3D world.
    ///////////////////////////////////////
    class MEZZ_LIB Terrain : public Entity
    {
    protected:
    public:
        /// @brief Class constructor.
        Terrain();
        /// @brief Class destructor.
        virtual ~Terrain();
    };//Terrain

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of Terrain objects.
    ///////////////////////////////////////
    class MEZZ_LIB TerrainFactory
    {
    public:
        /// @brief Class constructor.
        TerrainFactory() {  }
        /// @brief Class destructor.
        virtual ~TerrainFactory() {  }

        /// @brief Gets the name of the Terrain that is created by this factory.
        /// @return Returns the typename of the Terrain created by this factory.
        virtual String GetTypeName() const = 0;

        /// @brief Creates a Terrain of the type represented by this factory.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        /// @param Params A NameValuePairList containing the params to be applied during construction.
        /// @return Returns a pointer to the Terrain created.
        virtual Terrain* CreateTerrain(const String& Name, World* TheWorld, const NameValuePairMap& Params) = 0;
        /// @brief Creates a Terrain from XML.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        /// @return Returns a pointer to the Terrain created.
        virtual Terrain* CreateTerrain(const XML::Node& XMLNode, World* TheWorld) = 0;
        /// @brief Destroys a Terrain created by this factory.
        /// @param ToBeDestroyed A pointer to the Terrain to be destroyed.
        virtual void DestroyTerrain(Terrain* ToBeDestroyed) = 0;
    };//TerrainFactory
}//Mezzanine

#endif // _terrainbase_h
