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
                {//First
                    String SourceString( "GET http://www.blacktoppstudios.com/index.html HTTP/1.1\r\n"
                                         "Host: www.blacktoppstudios.com\r\n"
                                         "User-Agent: Mezzanine Network\r\n"
                                         "\r\n" );
                    Network::HTTPRequest RequestTest(SourceString);

                    Boole MethodTest = RequestTest.GetMethod() == Network::HRM_GET;
                    Boole URITest = RequestTest.GetURI() == Network::URI("http://www.blacktoppstudios.com/index.html");
                    Boole VersionTest = RequestTest.GetHTTPVersion() == SimpleVersion(1,1);
                    Boole HostTest = RequestTest.GetHostHeader() == "www.blacktoppstudios.com";
                    Boole UserAgentTest = RequestTest.GetUserAgentHeader() == "Mezzanine Network";
                    Boole BodyTest = RequestTest.GetBody().empty();

                    TEST(MethodTest && URITest && VersionTest && HostTest && UserAgentTest && BodyTest,"HTTPRequest_Decompose_First");
                    TEST(RequestTest.Compose() == SourceString,"HTTPRequest_Compose_First");
                }//First

                {//Second
                    String SourceString( "POST http://www.blacktoppstudios.com/forums HTTP/1.0\r\n"
                                         "Host: www.blacktoppstudios.com\r\n"
                                         "Content-Length: 13\r\n"
                                         "\r\n"
                                         "Hello Server!" );
                    Network::HTTPRequest RequestTest(SourceString);

                    Boole MethodTest = RequestTest.GetMethod() == Network::HRM_POST;
                    Boole URITest = RequestTest.GetURI() == Network::URI("http://www.blacktoppstudios.com/forums");
                    Boole VersionTest = RequestTest.GetHTTPVersion() == SimpleVersion(1,0);
                    Boole HostTest = RequestTest.GetHostHeader() == "www.blacktoppstudios.com";
                    Boole BodyTest = RequestTest.GetBody() == "Hello Server!";

                    TEST(MethodTest && URITest && VersionTest && HostTest && BodyTest,"HTTPRequest_Decompose_Second");
                    TEST(RequestTest.Compose() == SourceString,"HTTPRequest_Compose_Second");
                }//Second
            }//Request

            {//Response
                {//First
                    String SourceString( "HTTP/1.1 200 OK\r\n"
                                         "Date: Sun, 24 Jan 2016 4:15 GMT\r\n"
                                         "Server: Server\r\n"
                                         "Content-Length: 21\r\n"
                                         "\r\n"
                                         "YES.  THIS IS SERVER." );
                    Network::HTTPResponse ResponseTest(SourceString);

                    Boole VersionTest = ResponseTest.GetHTTPVersion() == SimpleVersion(1,1);
                    Boole StatusCodeTest = ResponseTest.GetStatusCode() == 200;
                    Boole StatusDescriptionTest = ResponseTest.GetResponseDescription() == "OK";
                    Boole DateTest = ResponseTest.GetDateHeader() == "Sun, 24 Jan 2016 4:15 GMT";
                    Boole ServerTest = ResponseTest.GetServerHeader() == "Server";
                    Boole BodyTest = ResponseTest.GetBody() == "YES.  THIS IS SERVER.";

                    TEST(VersionTest && StatusCodeTest && StatusDescriptionTest && DateTest && ServerTest && BodyTest,"HTTPResponse_Decompose_First");
                    TEST(ResponseTest.Compose() == SourceString,"HTTPResponse_Compose_First");
                }//First

                {//Second
                    String SourceString( "HTTP/1.0 404 Not Found\r\n"
                                               "Date: Fri, 1 Jan 2010 22:00 GMT\r\n"
                                               "Allow: POST\r\n"
                                               "Content-Length: 33\r\n"
                                               "\r\n"
                                               "Specified Resource doesn't exist." );
                    Network::HTTPResponse ResponseTest(SourceString);

                    Boole VersionTest = ResponseTest.GetHTTPVersion() == SimpleVersion(1,0);
                    Boole StatusCodeTest = ResponseTest.GetStatusCode() == 404;
                    Boole StatusDescriptionTest = ResponseTest.GetResponseDescription() == "Not Found";
                    Boole DateTest = ResponseTest.GetDateHeader() == "Fri, 1 Jan 2010 22:00 GMT";
                    Boole AllowTest = ResponseTest.GetAllowHeader() == "POST";
                    Boole BodyTest = ResponseTest.GetBody() == "Specified Resource doesn't exist.";

                    TEST(VersionTest && StatusCodeTest && StatusDescriptionTest && DateTest && AllowTest && BodyTest,"HTTPResponse_Decompose_Second");
                    TEST(ResponseTest.Compose() == SourceString,"HTTPResponse_Compose_Second");
                }//Second
            }//Response
        }//HTTP

        {//FTP
            {//Command
                {//First
                    String SourceString( "NOOP\r\n" );
                    Network::FTPCommand CommandTest(SourceString);

                    Boole CommandTypeTest = CommandTest.GetCommand() == Network::FCL_NOOP;
                    Boole ArgumentsTest = CommandTest.GetArguments().empty();

                    TEST(CommandTypeTest && ArgumentsTest,"FTPCommand_Decompose_First");
                    TEST(CommandTest.Compose() == SourceString,"FTPCommand_Compose_First");
                }//First

                {//Second
                    String SourceString( "RETR /Uploads/test.txt\r\n" );
                    Network::FTPCommand CommandTest(SourceString);

                    Boole CommandTypeTest = CommandTest.GetCommand() == Network::FCL_RETR;
                    Boole ArgumentsTest = CommandTest.GetArguments() == "/Uploads/test.txt";

                    TEST(CommandTypeTest && ArgumentsTest,"FTPCommand_Decompose_Second");
                    TEST(CommandTest.Compose() == SourceString,"FTPCommand_Compose_Second");
                }//Second

                {//Third
                    String SourceString( "TYPE I\r\n" );
                    Network::FTPCommand CommandTest(SourceString);

                    Boole CommandTypeTest = CommandTest.GetCommand() == Network::FCL_TYPE;
                    Boole ArgumentsTest = CommandTest.GetArguments() == "I";

                    TEST(CommandTypeTest && ArgumentsTest,"FTPCommand_Decompose_Third");
                    TEST(CommandTest.Compose() == SourceString,"FTPCommand_Compose_Third");
                }//Third
            }//Command

            {//Response
                {//First
                    String SourceString( "200 OK\r\n" );
                    Network::FTPResponse ResponseTest(SourceString);

                    Boole ResponseCodeTest = ResponseTest.GetCode() == Network::FRC_Ok;
                    Boole ResponseLineTest = ResponseTest.GetLine(0) == "200 OK";

                    TEST(ResponseCodeTest && ResponseLineTest,"FTPResponse_Decompose_First");
                    TEST(ResponseTest.Compose() == SourceString,"FTPResponse_Compose_First");
                }//First

                {//Second
                    String SourceString( "211-Extensions supported\r\n"
                                         " MLSD\r\n"
                                         " MLST\r\n"
                                         " SIZE\r\n"
                                         " STAT\r\n"
                                         "211 End\r\n" );
                    Network::FTPResponse ResponseTest(SourceString);

                    Boole ResponseCodeTest = ResponseTest.GetCode() == Network::FRC_SystemStatus;
                    Boole ResponseLineTest1 = ResponseTest.GetLine(0) == "211-Extensions supported";
                    Boole ResponseLineTest2 = ResponseTest.GetLine(1) == " MLSD";
                    Boole ResponseLineTest3 = ResponseTest.GetLine(2) == " MLST";
                    Boole ResponseLineTest4 = ResponseTest.GetLine(3) == " SIZE";
                    Boole ResponseLineTest5 = ResponseTest.GetLine(4) == " STAT";
                    Boole ResponseLineTest6 = ResponseTest.GetLine(5) == "211 End";

                    TEST(ResponseCodeTest && ResponseLineTest1 && ResponseLineTest2 && ResponseLineTest3 &&
                                             ResponseLineTest4 && ResponseLineTest5 && ResponseLineTest6,"FTPResponse_Decompose_Second");
                    TEST(ResponseTest.Compose() == SourceString,"FTPResponse_Compose_Second");
                }//Second

                {//Third
                    String SourceString( "211-Extensions supported\r\n"
                                         "211-MLSD\r\n"
                                         "211-MLST\r\n"
                                         "211-SIZE\r\n"
                                         "211-STAT\r\n"
                                         "211 End\r\n" );
                    Network::FTPResponse ResponseTest(SourceString);

                    Boole ResponseCodeTest = ResponseTest.GetCode() == Network::FRC_SystemStatus;
                    Boole ResponseLineTest1 = ResponseTest.GetLine(0) == "211-Extensions supported";
                    Boole ResponseLineTest2 = ResponseTest.GetLine(1) == "211-MLSD";
                    Boole ResponseLineTest3 = ResponseTest.GetLine(2) == "211-MLST";
                    Boole ResponseLineTest4 = ResponseTest.GetLine(3) == "211-SIZE";
                    Boole ResponseLineTest5 = ResponseTest.GetLine(4) == "211-STAT";
                    Boole ResponseLineTest6 = ResponseTest.GetLine(5) == "211 End";

                    TEST(ResponseCodeTest && ResponseLineTest1 && ResponseLineTest2 && ResponseLineTest3 &&
                                             ResponseLineTest4 && ResponseLineTest5 && ResponseLineTest6,"FTPResponse_Decompose_Third");
                    TEST(ResponseTest.Compose() == SourceString,"FTPResponse_Compose_Third");
                }//Third
            }//Response
        }//FTP
    }
    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//networkparsetests

#endif
