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
#ifndef _entityid_h
#define _entityid_h

#include "datatypes.h"

#ifndef SWIG
    #include "XML/xml.h"
#endif
#include "serialization.h"
#include "exception.h"

namespace Mezzanine
{
    /// @addtogroup ECS
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a convenience type for uniquely identifying an Entity.
    ///////////////////////////////////////
    class MEZZ_LIB EntityID
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Data Types

        /// @brief The underlying type for the ID to be used.
        using IDType = UInt64;
        /// @brief Convenience constant for representing an invalid internal value.
        static const IDType InvalidID = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Data Members

        /// @brief The stored ID for the Entity.
        IDType ID;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        EntityID() :
            ID(EntityID::InvalidID)
            {  }
        /// @brief IDType constructor.
        /// @param Identifier The unique value for the ID to be used.
        EntityID(const IDType Identifier) :
            ID(Identifier)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other EntityID to be copied.
        EntityID(const EntityID& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other EntityID to be moved.
        EntityID(EntityID&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Check to see if this EntityComponentID is holding an invalid value.
        /// @return Returns true if this EntityComponentID is invalid, false otherwise.
        Boole IsInvalid() const
            { return ( this->ID == InvalidID ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other EntityID to be copied.
        /// @return Returns a reference to this.
        EntityID& operator=(const EntityID& Other) = default;
        /// @brief Move Assignment operator.
        /// @param Other The other EntityID to be moved.
        /// @return Returns a reference to this.
        EntityID& operator=(EntityID&& Other) = default;

        /// @brief Equality comparison operator.
        /// @param Other The other EntityID to compare with.
        /// @return Returns true if this EntityID is the same as the other, false otherwise.
        Boole operator==(const EntityID& Other) const
            { return ( this->ID == Other.ID ); }
        /// @brief Inequality comparison operator.
        /// @param Other The other EntityID to compare with.
        /// @return Returns true if this EntityID is not the same as the other, false otherwise.
        Boole operator!=(const EntityID& Other) const
            { return ( this->ID != Other.ID ); }

        /// @brief Less-than operator.
        /// @param Other The other EntityID to compare with.
        /// @return Returns true if this EntityID is considered less than the other.
        Boole operator<(const EntityID& Other) const
            { return this->ID < Other.ID; }
        /// @brief Greater-than operator.
        /// @param Other The other EntityID to compare with.
        /// @return Returns true if this EntityID is considered greater than the other.
        Boole operator>(const EntityID& Other) const
            { return this->ID > Other.ID; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this EntityID should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(EntityID::GetSerializableName());
            if( SelfRoot.AppendAttribute("Version").SetValue("1") && SelfRoot.AppendAttribute("ID").SetValue( this->ID ) ) {
                return;
            }else{
                SerializeError("Create XML Attribute Values",EntityID::GetSerializableName(),true);
            }
        }
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        void ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            if( SelfRoot.Name() == EntityID::GetSerializableName() ) {
                if( SelfRoot.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = SelfRoot.GetAttribute("ID");
                    if( !CurrAttrib.Empty() )
                        this->ID = static_cast<IDType>( CurrAttrib.AsUint() );
                }else{
                    MEZZ_EXCEPTION( ExceptionBase::INVALID_VERSION_EXCEPTION,
                                    "Incompatible XML Version for " + EntityID::GetSerializableName() + ": Not Version 1." );
                }
            }else{
                MEZZ_EXCEPTION( ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,
                                EntityID::GetSerializableName() + " was not found in the provided XML node, which was expected." );
            }
        }

        /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName()
            { return "EntityID"; }
    };//EntityID

    /// @}
}//Mezzanine

#endif
