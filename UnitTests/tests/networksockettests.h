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
#ifndef _networksockettests_h
#define _networksockettests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file performs all the basic testing and verification of the Mezzanine Network socket classes.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A series of tests to verify functionality of sockets in the Network subsystem.
class networksockettests : public UnitTestGroup
{
protected:
    /// @brief An integer used to store the result of the client TCP tests from another thread.
    static Integer TCPClientThreadResult;
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "NetworkSocket"
    virtual String Name()
        { return String("NetworkSocket"); }

    ///////////////////////////////////////////////////////////////////////////////
    // Static Members for convenience

    /// @brief The port number for UDP tests.
    static const UInt16 UDPSystemPort = 4444;
    /// @brief The port number for TCP client-side testing.
    static const UInt16 TCPClientPort = 3420;
    /// @brief The port number for TCP server-side testing.
    static const UInt16 TCPServerPort = 3421;
    /// @brief The size of the buffer for reading message data.
    static const Whole ReceiveBufferSize = 500;

    /// @brief Thread function for the TCPv4 client-side testing.
    /// @param NullParam A parameter that was created just to satisfy the necessary signature.
    static void TCPClientv4HelperFunction(void* NullParam)
    {
        const String Message1("Hello from myself!  So alone...");
        const Whole Message1Size = Message1.size();//31;
        const String Message2("I am all I need!");
        //const Whole Message2Size = Message2.size();//16;

        char ReceiveBuffer[ReceiveBufferSize];
        Integer ReceiveSize = 0;

        Network::IPAddress LoopAddr(Network::IPAddress::IPv4LoopbackAddress);
        Network::SystemAddress SendAddr(LoopAddr,TCPServerPort);

        Network::TCPv4Socket V4Client;
        if( !V4Client.IsInvalid() ) {
            if( V4Client.Bind(TCPClientPort) ) {
                if( V4Client.Connect(SendAddr) ) {
                    if( V4Client.Send(Message1.c_str(),Message1Size) == Message1Size ) {
                        ReceiveSize = V4Client.Receive(ReceiveBuffer,ReceiveBufferSize);
                        if( ReceiveSize > 0 ) {
                            String MsgResult(ReceiveBuffer,ReceiveSize);
                            TCPClientThreadResult = ( Message2 == MsgResult ? 1 : 0 );
                        }
                    }
                }
            }
        }
        if( TCPClientThreadResult < 0 ) {
            TCPClientThreadResult = 0;
        }
        return;
    }

    /// @brief Thread function for the TCPv6 client-side testing.
    /// @param NullParam A parameter that was created just to satisfy the necessary signature.
    static void TCPClientv6HelperFunction(void* NullParam)
    {
        const String Message1("Hello from myself!  So alone...");
        const Whole Message1Size = Message1.size();//31;
        const String Message2("I am all I need!");
        //const Whole Message2Size = Message2.size();//16;

        char ReceiveBuffer[ReceiveBufferSize];
        Integer ReceiveSize = 0;

        Network::TCPv6Socket V6Client;

        Network::SocketResultContainer Results = Network::ResolveDomainName("localhost",TCPServerPort,Network::NLP_IPv6,Network::TLP_UDP);
        const Network::SocketDescription* Result = Results.GetNextResult();
        if( Result != NULL ) {
            Network::SystemAddress SendAddr = Result->ConvertToSystemAddress();

            if( !V6Client.IsInvalid() ) {
                if( V6Client.Bind(TCPClientPort) ) {
                    if( V6Client.Connect(SendAddr) ) {
                        if( V6Client.Send(Message1.c_str(),Message1Size) == Message1Size ) {
                            ReceiveSize = V6Client.Receive(ReceiveBuffer,ReceiveBufferSize);
                            if( ReceiveSize > 0 ) {
                                String MsgResult(ReceiveBuffer,ReceiveSize);
                                TCPClientThreadResult = ( Message2 == MsgResult ? 1 : 0 );
                            }
                        }
                    }
                }
            }
        }
        if( TCPClientThreadResult < 0 ) {
            TCPClientThreadResult = 0;
        }
        return;
    }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        if( Network::Socket::Initialize() != 0 ) {
            TEST(false,"Failed_to_initialize_sockets.");
        }

        const String Message("Hello from myself!  So alone...");
        const Whole MessageSize = Message.size();//31;
        const String TCPAck("I am all I need!");
        const Whole TCPAckSize = TCPAck.size();//16;
        Network::SystemAddress DummyAddress;
        char ReceiveBuffer[ReceiveBufferSize];
        Integer ReceiveSize = 0;

        {//IPv4
            Network::IPAddress LoopAddr(Network::IPAddress::IPv4LoopbackAddress);

            {//TCP
                TCPClientThreadResult = -1;
                memset(ReceiveBuffer,0,ReceiveBufferSize);
                ReceiveSize = 0;
                Boole TCPv4Result = false;

                Network::TCPv4Socket ListenerSocket;
                if( !ListenerSocket.IsInvalid() ) {
                    if( ListenerSocket.Bind(TCPServerPort) ) {
                        if( ListenerSocket.Listen() ) {
                            // Here's where we have to start making things complicated.
                            Threading::Thread* ClientThread = new Threading::Thread( &networksockettests::TCPClientv4HelperFunction );

                            Network::SystemAddress AcceptedAddress;
                            Network::TCPSocket* ClientConnection = ListenerSocket.Accept(AcceptedAddress);
                            if( ClientConnection != NULL ) {
                                // Now we have our connection to the other thread.
                                ReceiveSize = ClientConnection->Receive(ReceiveBuffer,ReceiveBufferSize);
                                if( ReceiveSize > 0 ) {
                                    String MsgResult(ReceiveBuffer,ReceiveSize);
                                    TCPv4Result = ( Message == MsgResult ) && ( ClientConnection->Send(TCPAck.c_str(),TCPAckSize) == TCPAckSize );
                                }
                            }

                            while( TCPClientThreadResult < 0 )
                                Threading::this_thread::sleep_for(1 * 1000);

                            delete ClientThread;
                        }
                    }
                }
                TEST( TCPv4Result, "TCPv4_Send/Receive_Server_functionality." );
                TEST( TCPClientThreadResult == 1, "TCPv4_Send/Receive_Client_functionality." );
            }//TCP

            {//UDP
                Network::SystemAddress SendAddr(LoopAddr,UDPSystemPort);
                memset(ReceiveBuffer,0,ReceiveBufferSize);
                ReceiveSize = 0;
                Boole UDPv4Result = false;

                Network::UDPv4Socket TestSocket;
                if( !TestSocket.IsInvalid() ) {
                    if( TestSocket.Bind(UDPSystemPort) ) {
                        if( TestSocket.Send(Message.c_str(),MessageSize,SendAddr) == MessageSize ) {
                            ReceiveSize = TestSocket.Receive(ReceiveBuffer,ReceiveBufferSize,DummyAddress);
                            if( ReceiveSize > 0 ) {
                                String MsgResult(ReceiveBuffer,ReceiveSize);
                                UDPv4Result = ( Message == MsgResult );
                            }
                        }
                    }
                }
                TEST( UDPv4Result, "UDPv4_Send/Receive_Loopback_functionality." );
            }//UDP
        }//IPv4

        {//IPv6
            {//TCP
                TCPClientThreadResult = -1;
                memset(ReceiveBuffer,0,ReceiveBufferSize);
                ReceiveSize = 0;
                Boole TCPv6Result = false;

                Network::TCPv6Socket ListenerSocket;
                if( !ListenerSocket.IsInvalid() ) {
                    if( ListenerSocket.Bind(TCPServerPort) ) {
                        if( ListenerSocket.Listen() ) {
                            // Here's where we have to start making things complicated.
                            Threading::Thread* ClientThread = new Threading::Thread( &networksockettests::TCPClientv6HelperFunction );

                            Network::SystemAddress AcceptedAddress;
                            Network::TCPSocket* ClientConnection = ListenerSocket.Accept(AcceptedAddress);
                            if( ClientConnection != NULL ) {
                                // Now we have our connection to the other thread.
                                ReceiveSize = ClientConnection->Receive(ReceiveBuffer,ReceiveBufferSize);
                                if( ReceiveSize > 0 ) {
                                    String MsgResult(ReceiveBuffer,ReceiveSize);
                                    TCPv6Result = ( Message == MsgResult ) && ( ClientConnection->Send(TCPAck.c_str(),TCPAckSize) == TCPAckSize );
                                }
                            }

                            while( TCPClientThreadResult < 0 )
                                Threading::this_thread::sleep_for(1 * 1000);

                            delete ClientThread;
                        }
                    }
                }
                TEST( TCPv6Result, "TCPv6_Send/Receive_Server_functionality." );
                TEST( TCPClientThreadResult == 1, "TCPv6_Send/Receive_Client_functionality." );
            }//TCP

            {//UDP
                memset(ReceiveBuffer,0,ReceiveBufferSize);
                ReceiveSize = 0;
                Boole UDPv6Result = false;
                Network::UDPv6Socket TestSocket;

                //IPv6 is hard.  Have to ask the system to help us make the addresses (Really this is primarily for the Scope-ID).
                Network::SocketResultContainer Results = Network::ResolveDomainName("localhost",UDPSystemPort,Network::NLP_IPv6,Network::TLP_UDP);
                const Network::SocketDescription* Result = Results.GetNextResult();
                if( Result != NULL ) {
                    Network::SystemAddress SendAddr = Result->ConvertToSystemAddress();

                    if( !TestSocket.IsInvalid() ) {
                        TestSocket.SetV6Only(true);
                        if( TestSocket.Bind(UDPSystemPort) ) {
                            if( TestSocket.Send(Message.c_str(),MessageSize,SendAddr) == MessageSize ) {
                                ReceiveSize = TestSocket.Receive(ReceiveBuffer,ReceiveBufferSize,DummyAddress);
                                if( ReceiveSize > 0 ) {
                                    String MsgResult(ReceiveBuffer,ReceiveSize);
                                    UDPv6Result = ( Message == MsgResult );
                                }
                            }
                        }
                    }
                }
                TEST( UDPv6Result, "UDPv6_Send/Receive_Loopback_functionality." );
            }//UDP
        }//IPv6

        Network::Socket::Deinitialize();
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasAutomaticTests() const
        { return true; }

    /*/// @brief This is called when Interactive tests are run.
    void RunInteractiveTests()
    {
        if( Network::Socket::Initialize() != 0 ) {
            TEST(false,"Failed_to_initialize_sockets.");
        }

        {//IPv4
            {//TCP

            }//TCP

            {//UDP

            }//UDP
        }//IPv4

        {//IPv6
            {//TCP

            }//TCP

            {//UDP

            }//UDP
        }//IPv6

        Network::Socket::Deinitialize();
    }

    /// @brief Since RunInteractiveTests is implemented so is this.
    /// @return Returns true.
    virtual bool HasInteractiveTests() const
        { return true; }// */
};//networksockettests

Integer networksockettests::TCPClientThreadResult = -1;

#endif
