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

#ifdef MEZZNETWORK

#ifndef _networkplatformsocket_h_cpp
#define _networkplatformsocket_h_cpp

#include "Network/platformincludes.h.cpp"

#include "Network/socketdescription.h"
#include "Network/systemaddress.h"

namespace Mezzanine
{
    namespace Network
    {
    #if defined(MEZZ_WINDOWS)
        /// @internal
        /// @brief Convenience type for the length of the address in Winsock 2.
        typedef int AddrLen;
        /// @internal
        /// @brief Convenience type for the system handle to the socket in Winsock 2.
        typedef SOCKET SocketHandleImplementation;
    #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
        /// @internal
        /// @brief Convenience type for the length of the address in Berkeley Sockets.
        typedef socklen_t AddrLen;
        /// @internal
        /// @brief Convenience type for the system handle to the socket in Berkeley Sockets.
        typedef int SocketHandleImplementation;
        #ifndef INVALID_SOCKET
            #define INVALID_SOCKET -1
        #endif
        #ifndef SOCKET_ERROR
            #define SOCKET_ERROR -1
        #endif
    #endif

        /// @internal
        /// @brief Used to allow type inference on @ref PlatformSocket constructors.
        /// @details on Windows an @ref SocketHandleImplementation would be a SOCKET on Linux it
        /// would be an int. This class allows easy differentiation between @ref PlatformSocket
        /// where method calls would be identical otherwise.
        class SocketHandle
        {
        protected:
            /// @internal
            /// @brief The actual handle store as a platform specific type thanks to typedefs.
            SocketHandleImplementation handle;
        public:
            /// @brief Platform specific contructor
            /// @details Because of the @ref SocketHandleImplementation typedef the
            /// signature of this constructor changes to match lower level OS constructs.
            /// Because this constructor accepts only on parameter the compiler will
            /// automatically use this when passing a @ref SocketHandleImplementation to a
            /// function or method that accepts a SocketHandle.
            /// @param OSHandle The handle the OS would use to reference this.
            SocketHandle(SocketHandleImplementation OSHandle = INVALID_SOCKET) :
                handle(OSHandle)
                {  }

        #if defined(MEZZ_WINDOWS)
            /// @brief Casting operator for Winsock.
            /// @details When a function accepts a platform specific socket handle (SOCKET),
            /// but an instance of this class was passed the compiler will find this method
            /// and use it to correctly convert to a SOCKET.
            /// @return A valid platform specific socket.
            operator SOCKET() const
                { return handle; }
        #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            /// @brief Casting operator for Berkeley Sockets.
            /// @details When a function accepts a platform specific socket handle (int),
            /// but an instance of this class was passed the compiler will find this method
            /// and use it to correctly convert to an int.
            /// @return A valid platform specific socket.
            operator int() const
                { return handle; }
        #endif
        };

        /// @brief An enum for the different configurable levels of socket options.
        enum SocketOptionLevel
        {
            Sol_IPv4      = IPPROTO_IP,
            Sol_IPv6      = IPPROTO_IPV6,
            Sol_TCP       = IPPROTO_TCP,
            Sol_UDP       = IPPROTO_UDP,
            Sol_Socket    = SOL_SOCKET
        };

        /// @brief An enum for the configurable options that exist on the socket level.
        enum SocketLevelOptions
        {
            // Datagram Options
            SockOpts_Broadcast      = SO_BROADCAST,
            // Stream Options
            SockOpts_KeepAlive      = SO_KEEPALIVE,
            // Regular Options
            SockOpts_DebugInfo      = SO_DEBUG,
            SockOpts_DontRoute      = SO_DONTROUTE,
            SockOpts_ReuseAddress   = SO_REUSEADDR,
            SockOpts_RecieveBufSize = SO_RCVBUF,
            SockOpts_SendBufSize    = SO_SNDBUF
        };

        /// @brief An enum for the configurable options that exist on the TCP protocol level.
        enum TCPLevelOptions
        {
            TCPOpts_NoDelay = TCP_NODELAY
        };

        /// @brief An enum for the configurable options that exist on the UDP protocol level.
        enum UDPLevelOptions
        {
            // Heh
        };

        /// @brief An enum for the configurable options that exist on the IPv4 protocol level.
        enum IPv4LevelOptions
        {
            // Datagram Options
            // Not fragmenting between the two platforms is weird.
        #if defined(MEZZ_WINDOWS)
            IPv4Opts_Fragmentation    = IP_DONTFRAGMENT,
        #elif defined(MEZZ_MACOSX)
            // No Support
        #elif defined(MEZZ_LINUX)
            IPv4Opts_Fragmentation    = IP_MTU_DISCOVER,
        #endif
            // Multicasting Options
            IPv4Opts_MulticastLoop    = IP_MULTICAST_LOOP,
            IPv4Opts_MulticastTTL     = IP_MULTICAST_TTL,
            // Regular Options
            //IPv4Opts_ReceiveBroadcast = IP_RECEIVE_BROADCAST,  // Windows Specific
            IPv4Opts_OptionsHeader    = IP_OPTIONS,              // Not used
            IPv4Opts_TimeToLive       = IP_TTL
        };

        /// @brief An enum for the easy mapping of the actual values needed to toggle IPv4 fragmentation.
        enum MsgFragValues
        {
        #if defined(MEZZ_WINDOWS)
            MFV_Frag_Enable  = 0,
            MFV_Frag_Disable = 1
        #elif defined(MEZZ_MACOSX)
            // No Support
        #elif defined(MEZZ_LINUX)
            MFV_Frag_Enable  = IP_PMTUDISC_DONT,
            MFV_Frag_Disable = IP_PMTUDISC_DO
        #endif
        };

        /// @brief An enum for the configurable options that exist on the IPv6 protocol level.
        enum IPv6LevelOptions
        {
            // Datagram Options
            //IPv6Opts_HopLimit      = IPV6_HOPLIMIT,       // Not used
            // Multicasting Options
            IPv6Opts_MulticastHops = IPV6_MULTICAST_HOPS,
            IPv6Opts_MulticastLoop = IPV6_MULTICAST_LOOP,
            // Regular Options
            IPv6Opts_UnicastHops   = IPV6_UNICAST_HOPS,
            IPv6Opts_V6Only        = IPV6_V6ONLY
        };

        /// @brief Converts from a Mezzanine address protocol type to it's internal counterpart.
        /// @param Protocol The Mezzanine protocol type to be converted.
        /// @return Returns the converted value from the Mezzanine type to the internal type.
        short int MEZZ_LIB ConvertLayer3ProtocolType(const NetworkLayerProtocol Protocol);
        /// @brief Converts from an internal address protocol type to it's Mezzanine counterpart.
        /// @param Protocol The internal protocol type to be converted.
        /// @return Returns the converted value from the internal type to the Mezzanine type.
        NetworkLayerProtocol MEZZ_LIB ConvertLayer3ProtocolType(const int Protocol);

        /// @brief Converts from a Mezzanine transport protocol type to it's internal counterpart.
        /// @param Protocol The Mezzanine protocol type to be converted.
        /// @return Returns the converted value from the Mezzanine type to the internal type.
        int MEZZ_LIB ConvertLayer4ProtocolType(const TransportLayerProtocol Protocol);
        /// @brief Converts from an internal transport protocol type to it's Mezzanine counterpart.
        /// @param Protocol The internal protocol type to be converted.
        /// @return Returns the converted value from the internal type to the Mezzanine type.
        TransportLayerProtocol MEZZ_LIB ConvertLayer4ProtocolType(const int Protocol);

        /// @brief Converts from a Mezzanine transport protocol type to it's internal counterpart.
        /// @param Protocol The Mezzanine protocol type to be converted.
        /// @return Returns the converted value from the Mezzanine type to the internal type.
        int MEZZ_LIB ConvertToSocketType(const TransportLayerProtocol Protocol);
        /// @brief Converts from an internal transport protocol type to it's Mezzanine counterpart.
        /// @param SockType The internal socket type to be converted.
        /// @return Returns the converted value from the internal type to the Mezzanine type.
        TransportLayerProtocol MEZZ_LIB ConvertToSocketType(const int SockType);

        /// @brief Converts a Mezzanine address to a Berkeley sockets address.
        /// @param MezzAddr The Mezzanine system address to be converted.
        /// @return Returns an std::pair containing the converted socket and it's size to indicate it's type.
        sockaddr_storage MEZZ_LIB ConvertToSocketStorage(const SystemAddress& MezzAddr);
        /// @brief Converts a Mezzanine address to a Berkeley sockets address.
        /// @param MezzAddr The Mezzanine socket descriptor to be converted.
        /// @return Returns an std::pair containing the converted socket and it's size to indicate it's type.
        sockaddr_storage MEZZ_LIB ConvertToSocketStorage(const SocketDescription& MezzAddr);
        /// @brief Converts a Berkeley sockets address to a Mezzanine address.
        /// @param BerkAddr The Berkeley Sockets sockaddr structure to be converted.
        /// @return Returns a Mezzanine System address containing the converted socket descriptor.
        SystemAddress MEZZ_LIB ConvertToSystemAddress(const sockaddr_storage& BerkAddr);
        /// @brief Converts a Berkeley sockets address to a Mezzanine socket descriptor.
        /// @param BerkAddr The Berkeley Sockets sockaddr structure to be converted.
        /// @return Returns a Mezzanine socket description containing the converted socket descriptor.
        SocketDescription MEZZ_LIB ConvertToSocketDescription(const sockaddr_storage& BerkAddr);

        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @class PlatformSocket
        /// @brief This is helper class for managing platform specific data of socket implementations.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB PlatformSocket
        {
        protected:
            /// @brief The handle to the internal socket implementation.
            SocketHandle InternalSocket;
            /// @brief The address type used for transmissions on this socket.
            const int SockDomain;
            /// @brief Whether this is a stream or datagram socket.
            const int SockType;
            /// @brief Whether or not this socket is currently in blocking mode.
            Boole Blocking;
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Non-binding constructor.
            /// @param Domain The Address family of the socket to create.
            /// @param Type Whether this is a stream or datagram socket.
            /// @param Protocol The specific protocol to use with this socket.
            PlatformSocket(int Domain, int Type, int Protocol);
            /// @brief Binding constructor.
            /// @param Domain The Address family of the socket to create.
            /// @param Type Whether this is a stream or datagram socket.
            /// @param Protocol The specific protocol to use with this socket.
            /// @param Port The port to bind this socket to.
            PlatformSocket(int Domain, int Type, int Protocol, const UInt16 Port);
            /// @brief Accept constructor.
            /// @param Handle The socket handle to wrap.
            /// @param Domain The Address family of the socket to create.
            /// @param Type Whether this is a stream or datagram socket.
            PlatformSocket(SocketHandle Handle, int Domain, int Type);
            /// @brief Class destructor.
            ~PlatformSocket();

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations

            /// @brief Binds this socket only to a port, accepting transmissions from any interface.
            /// @param The port to bind this socket to.
            /// @return Returns true if the Bind was successful, false if there was a failure.
            Boole Bind(const UInt16 Port);
            /// @brief Binds this socket to a local address so that it may receive transmissions addressed only to that interface.
            /// @param Address The local address to bind this sockets transmissions to.
            /// @return Returns true if the Bind was successful, false if there was a failure.
            Boole Bind(const SystemAddress& Address);
            /// @brief Binds this socket to a local address so that it may receive transmissions addressed only to that interface.
            /// @param Address The local address to bind this sockets transmissions to.
            /// @param Length The length of the address structure used to indicate it's type.
            /// @return Returns true if the Bind was successful, false if there was a failure.
            Boole Bind(sockaddr* Address, const AddrLen Length);

            /// @brief Disables some or all I/O on the socket.
            /// @param How Which I/O should be disabled (send, receive, or both).
            /// @return Returns true if the Shutdown was successful, false if there was a failure.
            Boole Shutdown(const int How);

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations - Stream

            /// @brief Connects this socket to a remote host connected to the network.
            /// @param Address The remote address to connect to.
            /// @return Returns true if the Connect was successful, false on a failure.
            Boole Connect(const SystemAddress& Address);
            /// @brief Connects this socket to a remote host connected to the network.
            /// @param Address The remote address to connect to.
            /// @param Length The length of the address structure used to indicate it's type.
            /// @return Returns true if the Connect was successful, false on a failure.
            Boole Connect(sockaddr* Address, const AddrLen Length);

            /// @brief Prepares this socket for incoming connections.
            /// @param Backlog The amount of pending connections to keep track of in the queue before rejecting other connections.
            /// @return Returns true if this put the socket into a listening state, false if there was a failure.
            Boole Listen(const int Backlog = 5);

            /// @brief Accepts an incoming connection from a remote host.
            /// @param Address The address structure to be populated with connection data to the Accepted remote host.
            /// @return Returns a pointer to a new socket specifically configured to talk to the Accepted remote host, or NULL if there was a failure.
            PlatformSocket* Accept(SystemAddress& Address);
            /// @brief Accepts an incoming connection from a remote host.
            /// @param Address A pointer to the address structure to be populated with connection data to the Accepted remote host.
            /// @param Length The size of the address structure passed in.  Will be set to a smaller value if a smaller struct is used to express the address.
            /// @return Returns a pointer to a new socket specifically configured to talk to the Accepted remote host, or NULL if there was a failure.
            PlatformSocket* Accept(sockaddr* Address, AddrLen* Length);

            /// @brief Sends data to the connected remote host.
            /// @param Buffer The data to be sent to the remote host.
            /// @param BufSize The size of the available buffer.
            /// @param Flags A bitfield containing additional options for transmission.
            /// @return Returns the number of bytes sent, or -1 if there was an error.
            Integer Send(const void* Buffer, const Integer BufSize, const Integer Flags);

            /// @brief Receives data from the connected remote host.
            /// @param Buffer The buffer to place the data received from the remote host.
            /// @param BufSize The size of the available buffer.
            /// @param Flags A bitfield containing additional options for transmission.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer Receive(void* Buffer, const Integer BufSize, const Integer Flags);

            ///////////////////////////////////////////////////////////////////////////////
            // Core Operations - Datagram

            /// @brief Sends data to a remote host.
            /// @param Buffer The data to be sent to the remote host.
            /// @param BufSize The size of the available buffer.
            /// @param Flags A bitfield containing additional options for transmission.
            /// @param Address The address of the remote host to send to.
            /// @return Returns the number of bytes sent, or -1 if there was an error.
            Integer SendTo(const void* Buffer, const Integer BufSize, const Integer Flags, const SystemAddress& Address);
            /// @brief Sends data to a remote host.
            /// @param Buffer The data to be sent to the remote host.
            /// @param BufSize The size of the available buffer.
            /// @param Flags A bitfield containing additional options for transmission.
            /// @param Address The address of the remote host to send to.
            /// @param Length The size of the address structure passed in.
            /// @return Returns the number of bytes sent, or -1 if there was an error.
            Integer SendTo(const void* Buffer, const Integer BufSize, const Integer Flags, const sockaddr* Address, const AddrLen Length);

            /// @brief Receives data from a remote host.
            /// @param Buffer The buffer to place the data received from the remote host.
            /// @param BufSize The size of the available buffer.
            /// @param Flags A bitfield containing additional options for transmission.
            /// @param Address The address of the remote host the data is being received from.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer ReceiveFrom(void* Buffer, const Integer BufSize, const Integer Flags, SystemAddress& Address);
            /// @brief Receives data from a remote host.
            /// @param Buffer The buffer to place the data received from the remote host.
            /// @param BufSize The size of the available buffer.
            /// @param Flags A bitfield containing additional options for transmission.
            /// @param Address The address of the remote host the data is being received from.
            /// @param Length The size of the address structure that was populated.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer ReceiveFrom(void* Buffer, const Integer BufSize, const Integer Flags, sockaddr* Address, AddrLen* Length);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets whether or not this socket will sleep on I/O calls.
            /// @param Block Whether or not you want the calling thread to block until data is available on I/O calls.
            /// @return Returns true if the operation was successful, false on a failure.
            Boole SetBlocking(const Boole Block);
            /// @brief Gets whether or not this socket will sleep on I/O calls.
            /// @return Returns true if this socket will block the calling thread on I/O calls, false otherwise.
            Boole GetBlocking() const;

            /// @brief Gets the number of bytes currently available to be read from the receive buffer.
            /// @remarks It is entirely possible for additional data to arrive immediately after this method is called,
            /// and before another method gets called.  Thus, the number returned cannot be a guarentee in all cases.
            /// @return Returns the amount of data that can be read from the receive buffer in bytes, or -1 if there was an error.
            Integer GetNumBytesAvailable() const;
            /// @brief Gets whether or not this socket is invalid, indicating an error during creation.
            /// @return Returns true if this socket is invalid, false if this socket has a valid file descriptor.
            Boole IsInvalid() const;

            /// @brief Set a low level socket parameter.
            /// @param Level The type of option to be set.
            /// @param OptID The ID of the option to be set.
            /// @param Value A buffer containing the value to set for the specified option.
            /// @param ValueLen The length of the Value buffer.
            /// @return Returns true if the socket option was set successfully, false on a failure.
            Boole SetSocketOption(const Integer Level, const Integer OptID, const char* Value, const AddrLen ValueLen);
            /// @brief Get a low level socket parameter.
            /// @param Level The type of option to retrieve.
            /// @param OptID The ID of the option to retrieve.
            /// @param Value A buffer to be populated with the requested option value.
            /// @param ValueLen The length of the Value buffer.
            /// @return Returns true if the socket option was set successfully, false on a failure.
            Boole GetSocketOption(const Integer Level, const Integer OptID, char* Value, AddrLen* ValueLen) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Error Handling

            /// @brief Gets the last error that occured on a socket.
            /// @return Returns the error code of the most recent socket error.
            static int GetLastError();
            /// @brief Throws an exception containing the message for a specific error code.
            /// @param ErrorCode The error code to throw an exception for.
            static void ThrowError(const int ErrorCode);
            /// @brief Throws an exception containing the message from the last socket error.
            static void ThrowLastError();
        };//PlatformSocket
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
