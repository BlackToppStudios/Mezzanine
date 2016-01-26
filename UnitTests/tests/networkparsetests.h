// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _networkparsetests_h
#define _networkparsetests_h

#include "mezztest.h"

#include "Network/network.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file performs all the basic testing and verification of the Mezzanine Network parsing structures.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A series of tests to verify parsing functionality of the Network subsystem.
class networkparsetests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "NetworkParse"
    virtual String Name()
        { return String("NetworkParse"); }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        {//URI
            //Absolute
            //Relative
            //Opaque
            //Hierarchical

            {//Absolute-Opaque
                String SourceString("mailto:example@blacktoppstudios.com");
                Network::URI TestURI(SourceString);

                Boole SchemeTest = TestURI.GetScheme() == "mailto";
                Boole UserInfoTest = TestURI.GetUserInfo().empty();
                Boole HostTest = TestURI.GetHost().empty();
                Boole PortTest = TestURI.GetPort() == 0;
                Boole PathTest = TestURI.GetPath() == "example@blacktoppstudios.com";
                Boole QueryTest = TestURI.GetQuery().empty();
                Boole FragmentTest = TestURI.GetFragment().empty();

                TEST(TestURI.IsAbsolute() && TestURI.IsOpaque(),"URI::Absolute-Opaque_Type_Detection");
                TEST(SchemeTest && UserInfoTest && HostTest && PortTest && PathTest && QueryTest && FragmentTest,"URI::Absolute-Opaque_Component_Parse");
                TEST(TestURI.ConvertToString() == SourceString,"URI::Absolute-Opaque_Reconstruct");
            }//Absolute-Opaque

            {//Absolute-Hierarchical
                String SourceString("http://www.blacktoppstudios.com/index.html?Name=Value#Frag1");
                Network::URI TestURI(SourceString);

                Boole SchemeTest = TestURI.GetScheme() == "http";
                Boole UserInfoTest = TestURI.GetUserInfo().empty();
                Boole HostTest = TestURI.GetHost() == "www.blacktoppstudios.com";
                Boole PortTest = TestURI.GetPort() == 0;
                Boole PathTest = TestURI.GetPath() == "/index.html";
                Boole QueryTest = TestURI.GetQuery() == "Name=Value";
                Boole FragmentTest = TestURI.GetFragment() == "Frag1";

                TEST(TestURI.IsAbsolute() && TestURI.IsHierarchical(),"URI::Absolute-Hierarchical_Type_Detection");
                TEST(SchemeTest && UserInfoTest && HostTest && PortTest && PathTest && QueryTest && FragmentTest,"URI::Absolute-Hierarchical_Component_Parse");
                TEST(TestURI.ConvertToString() == SourceString,"URI::Absolute-Hierarchical_Reconstruct");
            }//Absolute-Hierarchical

            {//Relative-Opaque
                String SourceString("./index.html?Name=Value#Frag1");
                Network::URI TestURI(SourceString);

                Boole SchemeTest = TestURI.GetScheme().empty();
                Boole UserInfoTest = TestURI.GetUserInfo().empty();
                Boole HostTest = TestURI.GetHost().empty();
                Boole PortTest = TestURI.GetPort() == 0;
                Boole PathTest = TestURI.GetPath() == "./index.html";
                Boole QueryTest = TestURI.GetQuery() == "Name=Value";
                Boole FragmentTest = TestURI.GetFragment() == "Frag1";

                TEST(TestURI.IsRelative() && TestURI.IsOpaque(),"URI::Relative-Opaque_Type_Detection");
                TEST(SchemeTest && UserInfoTest && HostTest && PortTest && PathTest && QueryTest && FragmentTest,"URI::Relative-Opaque_Component_Parse");
                TEST(TestURI.ConvertToString() == SourceString,"URI::Relative-Opaque_Reconstruct");
            }//Relative-Opaque

            {//Relative-Hierarchical
                String SourceString("//www.blacktoppstudios.com?Name=Value");
                Network::URI TestURI(SourceString);

                Boole SchemeTest = TestURI.GetScheme().empty();
                Boole UserInfoTest = TestURI.GetUserInfo().empty();
                Boole HostTest = TestURI.GetHost() == "www.blacktoppstudios.com";
                Boole PortTest = TestURI.GetPort() == 0;
                Boole PathTest = TestURI.GetPath().empty();
                Boole QueryTest = TestURI.GetQuery() == "Name=Value";
                Boole FragmentTest = TestURI.GetFragment().empty();

                TEST(TestURI.IsRelative() && TestURI.IsHierarchical(),"URI::Relative-Hierarchical_Type_Detection");
                TEST(SchemeTest && UserInfoTest && HostTest && PortTest && PathTest && QueryTest && FragmentTest,"URI::Relative-Hierarchical_Component_Parse");
                TEST(TestURI.ConvertToString() == SourceString,"URI::Relative-Hierarchical_Reconstruct");
            }//Relative-Hierarchical

            {//Percent-Encode
                String EncodedFullPlus("%48%65%6C%6C%6F+%57%6F%72%6C%64%21");
                String EncodedFullNoPlus("%48%65%6C%6C%6F%20%57%6F%72%6C%64%21");
                String EncodedWithStdDelims("%5BThis%5D is%3A An%2Fexhaustive%2Fdecode%3F %40%232.");
                String Decoded("Hello World!");
                String DecodedWithStdDelims("[This] is: An/exhaustive/decode? @#2.");

                TEST(Network::URI::PercentDecode(EncodedFullNoPlus) == Decoded,"URI::Full_Percent_Decode");
                TEST(Network::URI::PercentDecode(EncodedFullPlus,true) == Decoded,"URI::Full_Percent_Decode_Plus");
                TEST(Network::URI::PercentEncode(Decoded,Decoded) == EncodedFullNoPlus,"URI::Full_Percent_Encode");
                TEST(Network::URI::PercentDecode(EncodedWithStdDelims) == DecodedWithStdDelims,"URI::Mixed_Percent_Decode");
                TEST(Network::URI::PercentEncode(DecodedWithStdDelims,"") == EncodedWithStdDelims,"URI::Mixed_Percent_Encode");
            }//Percent-Encode

            {//Normalize
                String SourceString("HTTP://www.BlackToppStudios.com:80/./folderABC/../robot.mesh");
                Network::URI TestURI(SourceString);
                TestURI.Normalize();

                TEST(TestURI.ConvertToString() == "http://www.blacktoppstudios.com/robot.mesh","URI::Normalize");
            }//Normalize

            {//Resolve
                Network::URI AbsoluteFirst("http://www.blacktoppstudios.com");
                Network::URI AbsoluteSecond("ftp://files.blacktoppstudios.com/Game/Assets");
                Network::URI RelativeFirst("/Game/Assets/Meshes/robot.mesh?Compress=Yes");
                Network::URI RelativeSecond("/Engine/Core/doc.pdf#page=2");
                Network::URI RelativeThird("?Binary=No#Line=2");

                Boole ResolveFirst = ( AbsoluteFirst.Resolve(RelativeFirst).ConvertToString() == "http://www.blacktoppstudios.com/Game/Assets/Meshes/robot.mesh?Compress=Yes" );
                Boole ResolveSecond = ( AbsoluteSecond.Resolve(RelativeSecond).ConvertToString() == "ftp://files.blacktoppstudios.com/Engine/Core/doc.pdf#page=2" );
                Boole ResolveThird = ( AbsoluteFirst.Resolve(RelativeSecond).ConvertToString() == "http://www.blacktoppstudios.com/Engine/Core/doc.pdf#page=2" );
                Boole ResolveFourth = ( AbsoluteSecond.Resolve(RelativeFirst).ConvertToString() == "ftp://files.blacktoppstudios.com/Game/Assets/Meshes/robot.mesh?Compress=Yes" );
                Boole ResolveFifth = ( AbsoluteFirst.Resolve(RelativeFirst).Resolve(RelativeThird).ConvertToString() == "http://www.blacktoppstudios.com/Game/Assets/Meshes/robot.mesh?Binary=No#Line=2" );
                TEST(ResolveFirst && ResolveSecond && ResolveThird && ResolveFourth && ResolveFifth,"URI::Resolve");
            }//Resolve

            {//Utilities
                String SourceStringHTTP("http://www.blacktoppstudios.com");
                String SourceStringFTP("ftp://files.blacktoppstudios.com");
                String QueryString1("?Compress=Yes");
                String QueryString2("Compress=Yes&Binary=No");

                Network::URI EqualTest1(SourceStringHTTP);
                Network::URI EqualTest2(SourceStringHTTP);
                Network::URI InequalTest(SourceStringFTP);
                TEST(EqualTest1 == EqualTest2,"URI::operator==");
                TEST(EqualTest1 != InequalTest,"URI::operator!=");

                Network::URI AssignTest1(SourceStringFTP);
                Network::URI AssignTest2(SourceStringHTTP);
                AssignTest2 = AssignTest1;
                TEST(AssignTest2 == Network::URI(SourceStringFTP),"URI::operator=");

                Network::URI EmptyTest1(SourceStringHTTP);
                Network::URI EmptyTest2;
                TEST(EmptyTest2.IsEmpty(),"URI::IsEmpty()")
                TEST(EmptyTest1.Clear() == EmptyTest2,"URI::Clear()");

                Network::URI SwapTest1(SourceStringHTTP);
                Network::URI SwapTest2(SourceStringFTP);
                SwapTest1.Swap(SwapTest2);
                TEST(SwapTest1 == Network::URI(SourceStringFTP) && SwapTest2 == Network::URI(SourceStringHTTP),"URI::Swap");

                Network::URI QueryTest(QueryString1);
                Boole QueryParseResult = QueryTest.GetQuery() == "Compress=Yes";
                QueryTest.AddQueryParameter("Binary","No");
                TEST(QueryParseResult && QueryTest.GetQuery() == QueryString2,"URI::AddQueryParameter(const_String&,const_String&)");
                TEST(QueryTest.HasQueryParameter("Compress") && QueryTest.HasQueryParameter("Binary"),"URI::HasQueryParameter(const_String&)");
                TEST(QueryTest.GetParameterValue("Compress") == "Yes" && QueryTest.GetParameterValue("Binary") == "No","URI::GetParameterValue(const_String&)");
            }//Utilities
        }//URI

        {//HTTP
            {//Request
                String SourceStringFirst( "GET http://www.blacktoppstudios.com/index.html HTTP/1.1\r\n"
                                          "Host: www.blacktoppstudios.com\r\n"
                                          "User-Agent: Mezzanine Network\r\n"
                                          "\r\n" );
                Network::HTTPRequest RequestTestFirst(SourceStringFirst);

                Boole MethodTestFirst = RequestTestFirst.GetMethod() == Network::HRM_GET;
                Boole URITestFirst = RequestTestFirst.GetURI() == Network::URI("http://www.blacktoppstudios.com/index.html");
                Boole VersionTestFirst = RequestTestFirst.GetHTTPVersion() == SimpleVersion(1,1);
                Boole HostTestFirst = RequestTestFirst.GetHostHeader() == "www.blacktoppstudios.com";
                Boole UserAgentTestFirst = RequestTestFirst.GetUserAgentHeader() == "Mezzanine Network";
                Boole BodyTestFirst = RequestTestFirst.GetBody().empty();

                TEST(MethodTestFirst && URITestFirst && VersionTestFirst && HostTestFirst && UserAgentTestFirst && BodyTestFirst,"HTTPRequest_Decompose_First");
                TEST(RequestTestFirst.Compose() == SourceStringFirst,"HTTPRequest_Compose_First");

                String SourceStringSecond( "POST http://www.blacktoppstudios.com/forums HTTP/1.0\r\n"
                                           "Host: www.blacktoppstudios.com\r\n"
                                           "Content-Length: 13\r\n"
                                           "\r\n"
                                           "Hello Server!" );
                Network::HTTPRequest RequestTestSecond(SourceStringSecond);

                Boole MethodTestSecond = RequestTestSecond.GetMethod() == Network::HRM_POST;
                Boole URITestSecond = RequestTestSecond.GetURI() == Network::URI("http://www.blacktoppstudios.com/forums");
                Boole VersionTestSecond = RequestTestSecond.GetHTTPVersion() == SimpleVersion(1,0);
                Boole HostTestSecond = RequestTestSecond.GetHostHeader() == "www.blacktoppstudios.com";
                Boole BodyTestSecond = RequestTestSecond.GetBody() == "Hello Server!";

                TEST(MethodTestSecond && URITestSecond && VersionTestSecond && HostTestSecond && BodyTestSecond,"HTTPRequest_Decompose_Second");
                TEST(RequestTestSecond.Compose() == SourceStringSecond,"HTTPRequest_Compose_Second");
            }//Request

            {//Response
                String SourceStringFirst( "HTTP/1.1 200 OK\r\n"
                                          "Date: Sun, 24 Jan 2016 4:15 GMT\r\n"
                                          "Server: Server\r\n"
                                          "Content-Length: 21\r\n"
                                          "\r\n"
                                          "YES.  THIS IS SERVER." );
                Network::HTTPResponse ResponseTestFirst(SourceStringFirst);

                Boole VersionTestFirst = ResponseTestFirst.GetHTTPVersion() == SimpleVersion(1,1);
                Boole StatusCodeTestFirst = ResponseTestFirst.GetStatusCode() == 200;
                Boole StatusDescriptionTestFirst = ResponseTestFirst.GetResponseDescription() == "OK";
                Boole DateTestFirst = ResponseTestFirst.GetDateHeader() == "Sun, 24 Jan 2016 4:15 GMT";
                Boole ServerTestFirst = ResponseTestFirst.GetServerHeader() == "Server";
                Boole BodyTestFirst = ResponseTestFirst.GetBody() == "YES.  THIS IS SERVER.";

                TEST(VersionTestFirst && StatusCodeTestFirst && StatusDescriptionTestFirst && DateTestFirst && ServerTestFirst && BodyTestFirst,"HTTPResponse_Decompose_First");
                TEST(ResponseTestFirst.Compose() == SourceStringFirst,"HTTPResponse_Compose_First");

                String SourceStringSecond( "HTTP/1.0 404 Not Found\r\n"
                                           "Date: Fri, 1 Jan 2010 22:00 GMT\r\n"
                                           "Allow: POST\r\n"
                                           "Content-Length: 33\r\n"
                                           "\r\n"
                                           "Specified Resource doesn't exist." );
                Network::HTTPResponse ResponseTestSecond(SourceStringSecond);

                Boole VersionTestSecond = ResponseTestSecond.GetHTTPVersion() == SimpleVersion(1,0);
                Boole StatusCodeTestSecond = ResponseTestSecond.GetStatusCode() == 404;
                Boole StatusDescriptionTestSecond = ResponseTestSecond.GetResponseDescription() == "Not Found";
                Boole DateTestSecond = ResponseTestSecond.GetDateHeader() == "Fri, 1 Jan 2010 22:00 GMT";
                Boole AllowTestSecond = ResponseTestSecond.GetAllowHeader() == "POST";
                Boole BodyTestSecond = ResponseTestSecond.GetBody() == "Specified Resource doesn't exist.";

                TEST(VersionTestSecond && StatusCodeTestSecond && StatusDescriptionTestSecond && DateTestSecond && AllowTestSecond && BodyTestSecond,"HTTPResponse_Decompose_Second");
                TEST(ResponseTestSecond.Compose() == SourceStringSecond,"HTTPResponse_Compose_Second");
            }//Response
        }//HTTP
    }
    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//networkparsetests

#endif
