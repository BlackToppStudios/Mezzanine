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

#ifndef _networkplatformsocket_cpp
#define _networkplatformsocket_cpp

#include "Network/platformsocket.h.cpp"

#include "stringtool.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        short int ConvertLayer3ProtocolType(const NetworkLayerProtocol Protocol)
        {
            switch( Protocol )
            {
                case Network::NLP_IPv4:   return AF_INET;
                case Network::NLP_IPv6:   return AF_INET6;
                default:                  return AF_UNSPEC;
            }
        }

        NetworkLayerProtocol ConvertLayer3ProtocolType(const short int Protocol)
        {
            switch( Protocol )
            {
                case AF_INET:    return Network::NLP_IPv4;
                case AF_INET6:   return Network::NLP_IPv6;
                default:         return Network::NLP_Invalid;
            }
        }

        int ConvertLayer4ProtocolType(const TransportLayerProtocol Protocol)
        {
            switch( Protocol )
            {
                case Network::TLP_TCP:  return getprotobyname("tcp")->p_proto;
                case Network::TLP_UDP:  return getprotobyname("udp")->p_proto;
                default:                return 0;
            }
        }

        TransportLayerProtocol ConvertLayer4ProtocolType(const int Protocol)
        {
            switch( Protocol )
            {
                case IPPROTO_TCP:       return Network::TLP_TCP;
                case IPPROTO_UDP:       return Network::TLP_UDP;
                default:                return Network::TLP_Invalid;
            }
        }

        int ConvertToSocketType(const TransportLayerProtocol Protocol)
        {
            switch( Protocol )
            {
                case Network::TLP_TCP:  return SOCK_STREAM;
                case Network::TLP_UDP:  return SOCK_DGRAM;
                default:                return SOCK_STREAM;
            }
        }

        TransportLayerProtocol ConvertToSocketType(const int SockType)
        {
            switch( SockType )
            {
                case SOCK_STREAM:       return Network::TLP_TCP;
                case SOCK_DGRAM:        return Network::TLP_UDP;
                default:                return Network::TLP_Invalid;
            }
        }

        sockaddr_storage ConvertToSocketStorage(const SystemAddress& MezzAddr)
        {
            sockaddr_storage Ret;
            Network::NetworkLayerProtocol Protocol = MezzAddr.GetAddress().GetProtocol();
            if( Protocol == Network::NLP_IPv4 ) {
                sockaddr_in* Casted = (sockaddr_in*)&Ret;
                Casted->sin_family = AF_INET;
                Casted->sin_port = MezzAddr.GetPort(true);
                Casted->sin_addr.s_addr = MezzAddr.GetAddress().GetV4Address(true);
            }else if( Protocol == Network::NLP_IPv6 ) {
                sockaddr_in6* Casted = (sockaddr_in6*)&Ret;
                Casted->sin6_family = AF_INET6;
                Casted->sin6_port = MezzAddr.GetPort(true);
                Casted->sin6_flowinfo = MezzAddr.GetV6Flow();
                Casted->sin6_scope_id = MezzAddr.GetV6Scope();
                memcpy( &Casted->sin6_addr.s6_addr[0], &MezzAddr.GetAddress().GetBinaryAddress()[0], IPAddress::IPv6BinaryLength );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unknown or invalid address type attempting to be converted to Berkeley socket type.");
            }
            return Ret;
        }

        sockaddr_storage ConvertToSocketStorage(const SocketDescription& MezzAddr)
        {
            sockaddr_storage Ret;
            Network::NetworkLayerProtocol Protocol = MezzAddr.SocketAddress.GetProtocol();
            if( Protocol == Network::NLP_IPv4 ) {
                sockaddr_in* Casted = (sockaddr_in*)&Ret;
                Casted->sin_family = AF_INET;
                Casted->sin_port = MezzAddr.SocketPort;
                Casted->sin_addr.s_addr = MezzAddr.SocketAddress.GetV4Address(true);
            }else if( Protocol == Network::NLP_IPv6 ) {
                sockaddr_in6* Casted = (sockaddr_in6*)&Ret;
                Casted->sin6_family = AF_INET6;
                Casted->sin6_port = MezzAddr.SocketPort;
                Casted->sin6_flowinfo = MezzAddr.V6FlowInformation;
                Casted->sin6_scope_id = MezzAddr.V6ScopeID;
                memcpy( &Casted->sin6_addr.s6_addr[0], &MezzAddr.SocketAddress.GetBinaryAddress()[0], IPAddress::IPv6BinaryLength );
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unknown or invalid address type attempting to be converted to Berkeley socket type.");
            }
            return Ret;
        }

        SystemAddress ConvertToSystemAddress(const sockaddr_storage& BerkAddr)
        {
            SystemAddress Ret;
            if( BerkAddr.ss_family == AF_INET ) {
                const sockaddr_in& Casted = reinterpret_cast<const sockaddr_in&>(BerkAddr);

                Ret.GetAddress().SetV4Address(Casted.sin_addr.s_addr,true);
                Ret.SetPort(Casted.sin_port,true);
            }else if( BerkAddr.ss_family == AF_INET6 ) {
                const sockaddr_in6& Casted = reinterpret_cast<const sockaddr_in6&>(BerkAddr);

                IPAddress::AddressContainer BinAddr(IPAddress::IPv6BinaryLength);
                memcpy( &BinAddr[0] , &Casted.sin6_addr.s6_addr[0] , IPAddress::IPv6BinaryLength );
                Ret.GetAddress().SetBinaryAddress(BinAddr);
                Ret.SetPort(Casted.sin6_port,true);
                Ret.SetV6Flow(Casted.sin6_flowinfo);
                Ret.SetV6Scope(Casted.sin6_scope_id);
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unknown or invalid socket address type attempting to be converted to Mezzanine address.");
            }
            return Ret;
        }

        SocketDescription ConvertToSocketDescription(const sockaddr_storage& BerkAddr)
        {
            SocketDescription Ret;
            if( BerkAddr.ss_family == AF_INET ) {
                const sockaddr_in& Casted = reinterpret_cast<const sockaddr_in&>(BerkAddr);

                Ret.SocketAddress.SetV4Address(Casted.sin_addr.s_addr,true);
                Ret.SocketPort = Casted.sin_port;
            }else if( BerkAddr.ss_family == AF_INET6 ) {
                const sockaddr_in6& Casted = reinterpret_cast<const sockaddr_in6&>(BerkAddr);

                IPAddress::AddressContainer BinAddr(IPAddress::IPv6BinaryLength);
                memcpy( &BinAddr[0] , &Casted.sin6_addr.s6_addr[0] , IPAddress::IPv6BinaryLength );
                Ret.SocketAddress.SetBinaryAddress(BinAddr);
                Ret.SocketPort = Casted.sin6_port;
                Ret.V6FlowInformation = Casted.sin6_flowinfo;
                Ret.V6ScopeID = Casted.sin6_scope_id;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unknown or invalid socket address type attempting to be converted to Mezzanine socket description.");
            }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // PlatformSocket Methods

        PlatformSocket::PlatformSocket(int Domain, int Type, int Protocol) :
            SockDomain(Domain),
            SockType(Type),
            Blocking(true)
        {
            this->InternalSocket = socket(Domain,Type,Protocol);
        }

        PlatformSocket::PlatformSocket(int Domain, int Type, int Protocol, const UInt16 Port) :
            SockDomain(Domain),
            SockType(Type),
            Blocking(true)
        {
            this->InternalSocket = socket(Domain,Type,Protocol);
            this->Bind(Port);
        }

        PlatformSocket::PlatformSocket(SocketHandle Handle, int Domain, int Type) :
            InternalSocket(Handle),
            SockDomain(Domain),
            SockType(Type),
            Blocking(true)
            {  }

        PlatformSocket::~PlatformSocket()
        {
            #if defined(MEZZ_WINDOWS)
                ::closesocket(this->InternalSocket);
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                ::close(this->InternalSocket);
            #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        Boole PlatformSocket::Bind(const UInt16 Port)
        {
            addrinfo Hints;
            addrinfo* Results;

            Hints.ai_family = this->SockDomain;
            Hints.ai_socktype = this->SockType;
            Hints.ai_flags = AI_PASSIVE;

            int Result = getaddrinfo(NULL,( Port != 0 ? StringTools::ConvertToString(Port).c_str() : NULL ),&Hints,&Results);
            if( Result != 0 ) {
                MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,gai_strerror(Result));
            }

            return ( ::bind(this->InternalSocket,Results->ai_addr,Results->ai_addrlen) == 0 );
        }

        Boole PlatformSocket::Bind(const SystemAddress& Address)
        {
            sockaddr_storage AddressStorage = ConvertToSocketStorage(Address);
            Network::NetworkLayerProtocol ProtoType = Address.GetAddress().GetProtocol();
            if( ProtoType == Network::NLP_IPv4 ) {
                return this->Bind( (sockaddr*)&AddressStorage, sizeof(sockaddr_in) );
            }else if( ProtoType == Network::NLP_IPv6 ) {
                return this->Bind( (sockaddr*)&AddressStorage, sizeof(sockaddr_in6) );
            }
            return false;
        }

        Boole PlatformSocket::Bind(sockaddr* Address, const AddrLen Length)
        {
            #if defined(MEZZ_WINDOWS)
                return ( ::bind(this->InternalSocket,Address,Length) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ( ::bind(this->InternalSocket,Address,Length) == 0 );
            #endif
        }

        Boole PlatformSocket::Shutdown(const int How)
        {
            #if defined(MEZZ_WINDOWS)
                return ( ::shutdown(this->InternalSocket,How) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ( ::shutdown(this->InternalSocket,How) == 0 );
            #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations - Stream

        Boole PlatformSocket::Connect(const SystemAddress& Address)
        {
            sockaddr_storage AddressStorage = ConvertToSocketStorage(Address);
            Network::NetworkLayerProtocol ProtoType = Address.GetAddress().GetProtocol();
            if( ProtoType == Network::NLP_IPv4 ) {
                return this->Connect( (sockaddr*)&AddressStorage, sizeof(sockaddr_in) );
            }else if( ProtoType == Network::NLP_IPv6 ) {
                return this->Connect( (sockaddr*)&AddressStorage, sizeof(sockaddr_in6) );
            }
            return false;
        }

        Boole PlatformSocket::Connect(sockaddr* Address, const AddrLen Length)
        {
            #if defined(MEZZ_WINDOWS)
            return ( ::connect(this->InternalSocket,Address,Length) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            return ( ::connect(this->InternalSocket,Address,Length) == 0 );
            #endif
        }

        Boole PlatformSocket::Listen(const int Backlog)
        {
            #if defined(MEZZ_WINDOWS)
                return ( ::listen(this->InternalSocket,Backlog) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ( ::listen(this->InternalSocket,Backlog) == 0 );
            #endif
        }

        PlatformSocket* PlatformSocket::Accept(SystemAddress& Address)
        {
            sockaddr_storage AddressStorage;
            AddrLen AddressSize = sizeof(sockaddr_storage);
            SocketHandle NewSock = ::accept(this->InternalSocket,(sockaddr*)&AddressStorage,&AddressSize);
            if( NewSock != INVALID_SOCKET )
            {
                Address = ConvertToSystemAddress(AddressStorage);
                return new PlatformSocket(NewSock,this->SockDomain,this->SockType);
            }
            return NULL;
        }

        PlatformSocket* PlatformSocket::Accept(sockaddr* Address, AddrLen* Length)
        {
            SocketHandle NewSock = ::accept(this->InternalSocket,Address,Length);
            if( NewSock != INVALID_SOCKET )
            {
                return new PlatformSocket(NewSock,this->SockDomain,this->SockType);
            }
            return NULL;
        }

        Integer PlatformSocket::Send(const void* Buffer, const Integer BufSize, const Integer Flags)
        {
            #if defined(MEZZ_WINDOWS)
                int Ret = ::send(this->InternalSocket,static_cast<const char*>(Buffer),BufSize,Flags);
                return ( Ret != SOCKET_ERROR ? Ret : -1 );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ::send(this->InternalSocket,Buffer,BufSize,Flags);
            #endif
        }

        Integer PlatformSocket::Receive(void* Buffer, const Integer BufSize, const Integer Flags)
        {
            #if defined(MEZZ_WINDOWS)
                int Ret = ::recv(this->InternalSocket,static_cast<char*>(Buffer),BufSize,Flags);
                return ( Ret != SOCKET_ERROR ? Ret : -1 );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ::recv(this->InternalSocket,Buffer,BufSize,Flags);
            #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations - Datagram

        Integer PlatformSocket::SendTo(const void* Buffer, const Integer BufSize, const Integer Flags, const SystemAddress& Address)
        {
            sockaddr_storage AddressStorage = ConvertToSocketStorage(Address);
            Network::NetworkLayerProtocol ProtoType = Address.GetAddress().GetProtocol();
            if( ProtoType == Network::NLP_IPv4 ) {
                return this->SendTo( Buffer, BufSize, Flags, (sockaddr*)&AddressStorage, sizeof(sockaddr_in) );
            }else if( ProtoType == Network::NLP_IPv6 ) {
                return this->SendTo( Buffer, BufSize, Flags, (sockaddr*)&AddressStorage, sizeof(sockaddr_in6) );
            }
            return -1;
        }

        Integer PlatformSocket::SendTo(const void* Buffer, const Integer BufSize, const Integer Flags, const sockaddr* Address, const AddrLen Length)
        {
            #if defined(MEZZ_WINDOWS)
                int Ret = ::sendto(this->InternalSocket,static_cast<const char*>(Buffer),BufSize,Flags,Address,Length);
                return ( Ret != SOCKET_ERROR ? Ret : -1 );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ::sendto(this->InternalSocket,Buffer,BufSize,Flags,Address,Length);
            #endif
        }

        Integer PlatformSocket::ReceiveFrom(void* Buffer, const Integer BufSize, const Integer Flags, SystemAddress& Address)
        {
            sockaddr_storage AddressStorage;
            AddrLen AddressSize = sizeof(sockaddr_storage);
            int Ret = this->ReceiveFrom(Buffer,BufSize,Flags,(sockaddr*)&AddressStorage,&AddressSize);
            Address = ConvertToSystemAddress(AddressStorage);
            return Ret;
        }

        Integer PlatformSocket::ReceiveFrom(void* Buffer, const Integer BufSize, const Integer Flags, sockaddr* Address, AddrLen* Length)
        {
            #if defined(MEZZ_WINDOWS)
                int Ret = ::recvfrom(this->InternalSocket,static_cast<char*>(Buffer),BufSize,Flags,Address,Length);
                return ( Ret != SOCKET_ERROR ? Ret : -1 );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ::recvfrom(this->InternalSocket,Buffer,BufSize,Flags,Address,Length);
            #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole PlatformSocket::SetBlocking(const Boole Block)
        {
            #if defined(MEZZ_WINDOWS)
                unsigned long Mode = ( Block ? 0 : 1 );
                return ( ::ioctlsocket(this->InternalSocket, FIONBIO, &Mode) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                //int Flags = fcntl(fd, F_GETFL, 0);
                int Flags = fcntl(this->InternalSocket, F_GETFL, 0);
                if( Flags >= 0 ) {
                    Flags = Block ? ( Flags & ~O_NONBLOCK ) : ( Flags | O_NONBLOCK );
                    return ( ::fcntl(this->InternalSocket, F_SETFL, Flags) == 0 );
                }
                return false;
            #endif
        }

        Boole PlatformSocket::GetBlocking() const
            { return this->Blocking; }

        Integer PlatformSocket::GetNumBytesAvailable() const
        {
            unsigned long Ret = 0;
            #if defined(MEZZ_WINDOWS)
                if( ::ioctlsocket(this->InternalSocket,FIONREAD,&Ret) == SOCKET_ERROR )
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                if( ::ioctl(this->InternalSocket,FIONREAD,&Ret) < 0 )
            #endif
            {
                return -1;
            }
            return Ret;
        }

        Boole PlatformSocket::SetSocketOption(const Integer Level, const Integer OptID, const char* Value, const AddrLen ValueLen)
        {
            #if defined(MEZZ_WINDOWS)
                return ( ::setsockopt(this->InternalSocket,Level,OptID,Value,ValueLen) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ( ::setsockopt(this->InternalSocket,Level,OptID,Value,ValueLen) == 0 );
            #endif
        }

        Boole PlatformSocket::GetSocketOption(const Integer Level, const Integer OptID, char* Value, AddrLen* ValueLen) const
        {
            #if defined(MEZZ_WINDOWS)
                return ( ::getsockopt(this->InternalSocket,Level,OptID,Value,ValueLen) != SOCKET_ERROR );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return ( getsockopt(this->InternalSocket,Level,OptID,Value,ValueLen) == 0 );
            #endif
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Error Handling

        int PlatformSocket::GetLastError()
        {
            #if defined(MEZZ_WINDOWS)
                return WSAGetLastError();
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                return errno;
            #endif
        }

        void PlatformSocket::ThrowError(const int ErrorCode)
        {
            #if defined(MEZZ_WINDOWS)
                char* Str = NULL;
                FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, ErrorCode,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&Str, 0, NULL);
                String ErrorMsg( Str );
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
                String ErrorMsg( strerror(ErrorCode) );
            #endif
            MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,ErrorMsg);
        }

        void PlatformSocket::ThrowLastError()
        {
            PlatformSocket::ThrowError( PlatformSocket::GetLastError() );
        }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
