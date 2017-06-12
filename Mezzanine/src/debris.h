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
#ifndef _debris_h
#define _debris_h

/// @file
/// @brief This file contains the declaration for the debris class used to represent a small loose object in the world.

#include "worldentity.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A simple world object without a large structure ideal for representing loose small objects.
    ///////////////////////////////////////
    class MEZZ_LIB Debris : public WorldEntity
    {
    protected:
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        Debris(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        Debris(const String& Name, World* TheWorld);
        /// @brief Class destructor.
        virtual ~Debris();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Configuration

        ///////////////////////////////////////////////////////////////////////////////
        // Working with the World

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @copydoc Mezzanine::WorldEntity::ProtoSerializeProperties(XML::Node&) const
        virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
        /// @copydoc Mezzanine::WorldEntity::ProtoDeSerializeProperties(const XML::Node&)
        virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

        /// @copydoc Mezzanine::WorldEntity::GetDerivedSerializableName() const
        virtual String GetDerivedSerializableName() const;
        /// @copydoc Mezzanine::WorldEntity::GetSerializableName()
        static String GetSerializableName();

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods
    };//Debris

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of Debris objects.
    ///////////////////////////////////////
    class MEZZ_LIB DebrisFactory
    {
    public:
        /// @brief Class constructor.
        DebrisFactory() {  }
        /// @brief Class destructor.
        virtual ~DebrisFactory() {  }

        /// @brief Gets the name of the Debris that is created by this factory.
        /// @return Returns the typename of the Debris created by this factory.
        virtual String GetTypeName() const = 0;

        /// @brief Creates a Debris of the type represented by this factory.
        /// @param Name The name to be given to this object.
        /// @param TheWorld A pointer to the world this object belongs to.
        /// @param Params A NameValuePairList containing the params to be applied during construction.
        /// @return Returns a pointer to the Debris created.
        virtual Debris* CreateDebris(const String& Name, World* TheWorld, const NameValuePairMap& Params) = 0;
        /// @brief Creates a Debris from XML.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        /// @return Returns a pointer to the Debris created.
        virtual Debris* CreateDebris(const XML::Node& XMLNode, World* TheWorld) = 0;
        /// @brief Destroys a Debris created by this factory.
        /// @param ToBeDestroyed A pointer to the Debris to be destroyed.
        virtual void DestroyDebris(Debris* ToBeDestroyed) = 0;
    };//DebrisFactory
}//Mezzanine

#endif
