// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _softdebris_h
#define _softdebris_h

/// @file
/// @brief This file contains the declaration for the debris class that will compress and deform.

#include "debris.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A simple world object without a large structure.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB SoftDebris : public Debris
    {
    protected:
    public:
        /// @brief Blank constructor.
        /// @param TheWorld A pointer to the world this object belongs to.
        SoftDebris(World* TheWorld);
        /// @brief Class constructor.
        /// @param Name The name to be given to this object.
        /// @param Mass The mass of the debris object.
        /// @param TheWorld A pointer to the world this object belongs to.
        SoftDebris(const String& Name, const Real Mass, World* TheWorld);
        /// @brief Class destructor.
        ~SoftDebris();
    };//SoftDebris

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A base factory type for the creation of deformable Debris objects.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB SoftDebrisFactory : public DebrisFactory
    {
    public:
        /// @brief Class constructor.
        SoftDebrisFactory();
        /// @brief Class destructor.
        virtual ~SoftDebrisFactory();

        /// @copydoc DebrisFactory::GetTypeName() const
        virtual String GetTypeName() const;

        /// @brief Creates a SoftDebris object.
        /// @param Name The name to be given to this object.
        /// @param Mass The mass of the debris object.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual SoftDebris* CreateSoftDebris(const String& Name, const Real Mass, World* TheWorld);
        /// @brief Creates a SoftDebris object.
        /// @param XMLNode The node of the xml document to construct from.
        /// @param TheWorld A pointer to the world this object belongs to.
        virtual SoftDebris* CreateSoftDebris(const XML::Node& XMLNode, World* TheWorld);

        /// @copydoc DebrisFactory::CreateDebris(const String&, World*, NameValuePairList&)
        virtual Debris* CreateDebris(const String& Name, World* TheWorld, NameValuePairList& Params);
        /// @copydoc DebrisFactory::CreateDebris(const XML::Node&, World*)
        virtual Debris* CreateDebris(const XML::Node& XMLNode, World* TheWorld);
        /// @copydoc DebrisFactory::DestroyDebris(Debris* ToBeDestroyed)
        virtual void DestroyDebris(Debris* ToBeDestroyed);
    };//SoftDebrisFactory
}//Mezzanine

#endif
