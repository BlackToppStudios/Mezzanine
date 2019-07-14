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
#ifndef _assetid_h
#define _assetid_h

#include "datatypes.h"
#include "enumerations.h"
#include "serialization.h"
#include "exception.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Mezzanine
{
    /// @brief An enum used to describe (at a high level) where the asset is coming from.
    enum class AssetSourceType : UInt16
    {
        Unknown,           ///< Error Value.
        FileSystem,        ///< The asset is coming from a locally mounted drive.
        Network,           ///< The asset is coming from a remote host.
        Memory             ///< The asset is coming local system memory.
    };

    /// @brief An enum used to describe additional details about the stream to the asset.
    /// @remarks The details are (currently) exclusively additional operations the stream performs on the data
    /// before the data is returned.
    enum class AssetStreamDesc : UInt16
    {
        Unknown,           ///< Error value.
        Raw,               ///< The stream is not from an archive.
        Raw_Encrypted,     ///< The stream is not from an archive but is encrypted.
        Zip,               ///< The stream is from a zip archive.
        Zip_Encrypted,     ///< The stream is from a zip archive and is encrypted.
        SevenZ,            ///< The stream is from a 7z archive.
        SevenZ_Encrypted   ///< The stream is from a 7z archive and is encrypted.
    };

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is a simple class storing the necessary information to open a stream to a specific asset.
    ///////////////////////////////////////
    class MEZZ_LIB AssetID
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief A string containing the path and name of the Asset.
        String Identifier;
        /// @brief A string containing the name of the resource group the Asset was loaded from.
        /// @remarks This string is optional and only needed when using the resource system.
        String GroupName;
        /// @brief The type of storage the asset is being streamed from.
        AssetSourceType SourceType = AssetSourceType::Memory;
        /// @brief Additional details for the stream to the asset.
        AssetStreamDesc StreamDesc = AssetStreamDesc::Unknown;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        AssetID() = default;
        /// @brief Copy constructor.
        /// @param Other The other AssetID to copy from.
        AssetID(const AssetID& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other AssetID to be moved.
        AssetID(AssetID&& Other) = default;
        /// @brief Source-Stream constructor.
        /// @param Source The type of storage the asset is being streamed from.
        /// @param Stream Additional details for the stream to the asset.
        AssetID(const AssetSourceType Source, const AssetStreamDesc Stream) :
            SourceType(Source),
            StreamDesc(Stream)
            {  }
        /// @brief Source-ID constructor.
        /// @param Source The type of storage the asset is being streamed from.
        /// @param ID The unique identifier for the asset.
        AssetID(const AssetSourceType Source, const String& ID) :
            Identifier(ID),
            SourceType(Source)
            {  }
        /// @brief Non-Group constructor.
        /// @param Source The type of storage the asset is being streamed from.
        /// @param Stream Additional details for the stream to the asset.
        /// @param ID The unique identifier for the asset.
        AssetID(const AssetSourceType Source, const AssetStreamDesc Stream, const String& ID) :
            Identifier(ID),
            SourceType(Source),
            StreamDesc(Stream)
            {  }
        /// @brief Complete constructor.
        /// @param Source The type of storage the asset is being streamed from.
        /// @param Stream Additional details for the stream to the asset.
        /// @param ID The unique identifier for the asset.
        /// @param Group The resource group where the asset can be found.
        AssetID(const AssetSourceType Source, const AssetStreamDesc Stream, const String& ID, const String& Group) :
            Identifier(ID),
            GroupName(Group),
            SourceType(Source),
            StreamDesc(Stream)
            {  }
        /// @brief Serialization constructor.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        AssetID(const XML::Node& SelfRoot) :
            SourceType(AssetSourceType::Unknown),
            StreamDesc(AssetStreamDesc::Unknown)
            { this->ProtoDeSerialize(SelfRoot); }
        /// @brief Class destructor.
        ~AssetID()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Checks

        /// @brief Checks to see if this AssetID has been initialized.
        /// @return Returns true if this AssetID has no error/placeholder values, false otherwise.
        Boole IsValid() const
        {
            return ( !this->Identifier.empty() &&
                     this->SourceType != AssetSourceType::Unknown &&
                     this->StreamDesc != AssetStreamDesc::Unknown );
        }

        /// @brief Checks to see if the stream this AssetID refers to is encrypted.
        /// @return Returns true if this AssetID refers to an encrypted Asset, false otherwise.
        Boole IsEncrypted() const
        {
            return ( this->StreamDesc == AssetStreamDesc::Raw_Encrypted ||
                     this->StreamDesc == AssetStreamDesc::Zip_Encrypted ||
                     this->StreamDesc == AssetStreamDesc::SevenZ_Encrypted );
        }

        /// @brief Checks to see if the stream this AssetID refers to is uncompressed.
        /// @return Returns true if this AssetID refers to an uncompressed Asset, false otherwise.
        Boole IsStreamRaw() const
        {
            return ( this->StreamDesc == AssetStreamDesc::Raw ||
                     this->StreamDesc == AssetStreamDesc::Raw_Encrypted );
        }

        /// @brief Checks to see if the stream this AssetID refers to is compressed within a Zip archive.
        /// @return Returns true if this AssetID refers to an Asset compressed into a Zip archive, false otherwise.
        Boole IsStreamZip() const
        {
            return ( this->StreamDesc == AssetStreamDesc::Zip ||
                     this->StreamDesc == AssetStreamDesc::Zip_Encrypted );
        }

        /// @brief Checks to see if the stream this AssetID refers to is compressed within a 7Zip archive.
        /// @return Returns true if this AssetID refers to an Asset compressed into a 7Zip archive, false otherwise.
        Boole IsStreamSevenZ() const
        {
            return ( this->StreamDesc == AssetStreamDesc::SevenZ ||
                     this->StreamDesc == AssetStreamDesc::SevenZ_Encrypted );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy Assignment operator.
        /// @param Other The other AssetID to be copied.
        /// @return Returns a reference to this.
        AssetID& operator=(const AssetID& Other) = default;
        /// @brief Move Assignment operator.
        /// @param Other The other AssetID to be moved.
        /// @return Returns a reference to this.
        AssetID& operator=(AssetID&& Other) = default;

        /// @brief Less-than operator.
        /// @param Other The other AssetID to compare with.
        /// @return Returns true if this Asset should be order before the other Asset, false otherwise.
        Boole operator<(const AssetID& Other)
        {
            if( this->SourceType != Other.SourceType )
                return ( this->SourceType < Other.SourceType );

            if( this->StreamDesc != Other.StreamDesc )
                return ( this->StreamDesc < Other.StreamDesc );

            if( this->GroupName != Other.GroupName )
                return ( this->GroupName < Other.GroupName );

            return ( this->Identifier < Other.Identifier );
        }
        /// @brief Greater-than operator.
        /// @param Other The other AssetID to compare with.
        /// @return Returns true if this Asset should be order after the other Asset, false otherwise.
        Boole operator>(const AssetID& Other)
        {
            if( this->SourceType != Other.SourceType )
                return ( this->SourceType > Other.SourceType );

            if( this->StreamDesc != Other.StreamDesc )
                return ( this->StreamDesc > Other.StreamDesc );

            if( this->GroupName != Other.GroupName )
                return ( this->GroupName > Other.GroupName );

            return ( this->Identifier > Other.Identifier );
        }

        /// @brief Equality operator.
        /// @param Other The other AssetID to compare with.
        /// @return Returns true if this Asset is the same as another asset, false otherwise.
        Boole operator==(const AssetID& Other)
        {
            return ( this->SourceType == Other.SourceType ) &&
                   ( this->StreamDesc == Other.StreamDesc ) &&
                   ( this->GroupName == Other.GroupName ) &&
                   ( this->Identifier == Other.Identifier );
        }
        /// @brief Inequality operator.
        /// @param Other The other AssetID to compare with.
        /// @return Returns true if this Asset is not the same as another asset, false otherwise..
        Boole operator!=(const AssetID& Other)
        {
            return ( this->SourceType != Other.SourceType ) ||
                   ( this->StreamDesc != Other.StreamDesc ) ||
                   ( this->GroupName != Other.GroupName ) ||
                   ( this->Identifier != Other.Identifier );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that all this instance should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(AssetID::GetSerializableName());

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("SourceType").SetValue( EnumClassToInt( this->SourceType ) ) &&
                SelfRoot.AppendAttribute("StreamDesc").SetValue( EnumClassToInt( this->StreamDesc ) ) &&
                SelfRoot.AppendAttribute("GroupName").SetValue( this->GroupName ) &&
                SelfRoot.AppendAttribute("Identifier").SetValue( this->Identifier ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",AssetID::GetSerializableName(),true);
            }
        }
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
        void ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( SelfRoot.Name() == AssetID::GetSerializableName() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = SelfRoot.GetAttribute("SourceType");
                    if( !CurrAttrib.Empty() )
                        this->SourceType = static_cast<AssetSourceType>( CurrAttrib.AsUint() );

                    CurrAttrib = SelfRoot.GetAttribute("StreamDesc");
                    if( !CurrAttrib.Empty() )
                        this->StreamDesc = static_cast<AssetStreamDesc>( CurrAttrib.AsUint() );

                    CurrAttrib = SelfRoot.GetAttribute("GroupName");
                    if( !CurrAttrib.Empty() )
                        this->GroupName = CurrAttrib.AsString();

                    CurrAttrib = SelfRoot.GetAttribute("Identifier");
                    if( !CurrAttrib.Empty() )
                        this->Identifier = CurrAttrib.AsString();
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + AssetID::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,AssetID::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName()
            { return "AssetID"; }
    };//AssetID
}//Mezzanine

#endif
