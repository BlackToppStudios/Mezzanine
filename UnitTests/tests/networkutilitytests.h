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
#ifndef _networkutilitytests_h
#define _networkutilitytests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file performs all the basic testing and verification of the Mezzanine Network utilities that have no specific grouping.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A series of tests to verify basic functionality of small routines of the Network subsystem.
class networkutilitytests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "NetworkUtility"
    virtual String Name()
        { return String("NetworkUtility"); }

    /// @brief Verifies the contents of the two address containers are identical.
    /// @param First The first AddressContainer to be tested.
    /// @param Second The second AddressContainer to be tested.
    /// @return Returns true if both containers are identical, false otherwise.
    Boole VerifyAddressContainer(const Network::IPAddress::AddressContainer& First, const Network::IPAddress::AddressContainer& Second)
    {
        if( First.size() != Second.size() ) {
            return false;
        }

        for( Whole X = 0 ; X < First.size() ; ++X )
        {
            if( First[X] != Second[X] ) {
                return false;
            }
        }

        return true;
    }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        {//IPAddress
            {//IPv4 Resolve
                String v4StrAddress("192.168.1.100");
                UInt8 v4BinAddress[] = { 192, 168, 1, 100 };
                Network::IPAddress::AddressContainer v4VecAddress(&v4BinAddress[0],&v4BinAddress[4]);

                TEST( VerifyAddressContainer( Network::IPAddress::Resolve(v4StrAddress), v4VecAddress) , "IPAddress::Resolve(const_String&)_v4" );
                TEST( Network::IPAddress::Resolve(v4VecAddress) == v4StrAddress , "IPAddress::Resolve(const_AddressContainer&)_v4" );
            }//IPv4 Resolve

            {//IPv6 Resolve
                String v6StrAddressShort("2002:4559:1fe2::4559:1fe2");
                String v6StrAddressMedium("2002:4559:1fe2:0:0:0:4559:1fe2");
                String v6StrAddressLong("2002:4559:1fe2:0000:0000:0000:4559:1fe2");
                UInt8 v6BinAddress[] = { 0x20, 0x02, 0x45, 0x59, 0x1f, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x59, 0x1f, 0xe2 };
                Network::IPAddress::AddressContainer v6VecAddress(&v6BinAddress[0],&v6BinAddress[16]);

                TEST( VerifyAddressContainer( Network::IPAddress::Resolve(v6StrAddressShort), v6VecAddress) , "IPAddress::Resolve(const_String&)_v6_Short" );
                TEST( VerifyAddressContainer( Network::IPAddress::Resolve(v6StrAddressMedium), v6VecAddress) , "IPAddress::Resolve(const_String&)_v6_Medium" );
                TEST( VerifyAddressContainer( Network::IPAddress::Resolve(v6StrAddressLong), v6VecAddress) , "IPAddress::Resolve(const_String&)_v6_Long" );
                TEST( Network::IPAddress::Resolve(v6VecAddress) == v6StrAddressShort , "IPAddress::Resolve(const_AddressContainer&)_v6" );
            }//IPv6 Resolve

            {//Operators
                Network::IPAddress TestAddress1("192.168.1.1");
                Network::IPAddress TestAddress2("10.10.10.8");
                Network::IPAddress CopyTarget;
                CopyTarget = TestAddress2;

                TEST( TestAddress1 == Network::IPAddress("192.168.1.1") , "IPAddress::operator==(const_IPAddress&)_const" );
                TEST( TestAddress2 != Network::IPAddress("2.2.2.2") , "IPAddress::operator!=(const_IPAddress&)_const" );
                TEST( CopyTarget == TestAddress2 , "IPAddress::operator=(const_IPAddress&)" )
            }//Operators

            {//Serialization
                XML::Document TestDoc;
                Network::IPAddress TestAddress1("192.168.1.1");
                Network::IPAddress TestAddress2;
                String StrResult("<IPAddress Version=\"1\" Address=\"192.168.1.1\" />");
                String StrInit("<IPAddress Version=\"1\" Address=\"192.168.3.50\" />");

                StringStream TestStream;
                TestAddress1.ProtoSerialize(TestDoc);
                TestDoc.Print(TestStream);
                TEST( TestStream.str() == StrResult , "IPAddress::ProtoSerialize(XML::Node&)_const" );

                TestDoc.Reset();
                TestStream.str(StrInit);
                TestDoc.Load(TestStream);
                TestAddress2.ProtoDeSerialize(TestDoc.GetFirstChild());
                TEST( TestAddress2 == Network::IPAddress("192.168.3.50") , "IPAddress::ProtoDeSerialize(const_XML::Node&)" );
            }//Serialization
        }//IPAddress

        {//SystemAddress
            {//Operators
                Network::SystemAddress TestAddress1(Network::IPAddress("192.168.1.1"),80);
                Network::SystemAddress TestAddress2(Network::IPAddress("10.10.10.8"),40);
                Network::SystemAddress CopyTarget;
                CopyTarget = TestAddress2;

                TEST( TestAddress1 == Network::SystemAddress(Network::IPAddress("192.168.1.1"),80) , "SystemAddress::operator==(const_SystemAddress&)_const" );
                TEST( TestAddress2 != Network::SystemAddress(Network::IPAddress("2.2.2.2"),1200) , "SystemAddress::operator!=(const_SystemAddress&)_const" );
                TEST( CopyTarget == TestAddress2 , "SystemAddress::operator=(const_SystemAddress&)" )
            }//Operators

            {//Serialization
                XML::Document TestDoc;
                Network::SystemAddress TestAddress1(Network::IPAddress("192.168.1.1"),80);
                Network::SystemAddress TestAddress2;
                String StrResult("<SystemAddress Version=\"1\" Port=\"80\" V6Flow=\"0\" V6Scope=\"0\"><IPAddress Version=\"1\" Address=\"192.168.1.1\" /></SystemAddress>");
                String StrInit("<SystemAddress Version=\"1\" Port=\"40\" V6Flow=\"0\" V6Scope=\"0\"><IPAddress Version=\"1\" Address=\"192.168.3.50\" /></SystemAddress>");

                StringStream TestStream;
                TestAddress1.ProtoSerialize(TestDoc);
                TestDoc.Print(TestStream);
                String DebugString(TestStream.str());
                TEST( DebugString == StrResult , "SystemAddress::ProtoSerialize(XML::Node&)_const" );

                TestDoc.Reset();
                TestStream.str(StrInit);
                TestDoc.Load(TestStream);
                TestAddress2.ProtoDeSerialize(TestDoc.GetFirstChild());
                TEST( TestAddress2 == Network::SystemAddress(Network::IPAddress("192.168.3.50"),40) , "SystemAddress::ProtoDeSerialize(const_XML::Node&)" );
            }//Serialization
        }//SystemAddress
    }
    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//networkutilitytests

#endif
