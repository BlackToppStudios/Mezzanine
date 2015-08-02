// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef _networksocket_cpp
#define _networksocket_cpp

#include "Network/platformsocket.h.cpp"

#include "Network/socket.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Socket Errors

        // SE_Unknown is the only error intended to have a value of -1.  However some errors exist on one platform but not others.
        // For those cases where there is not cross-platform equivalent a value of -1 is used.
        const Integer SE_Unknown = -1;
        #if defined(MEZZ_WINDOWS)
        const Integer SE_Uninitialized = WSANOTINITIALISED;
        const Integer SE_NetworkNotReady = WSASYSNOTREADY;
        const Integer SE_Interrupted = WSAEINTR;
        const Integer SE_AccessDenied = WSAEACCES;
        const Integer SE_InvalidAddr = WSAEFAULT;
        const Integer SE_InvalidParam = WSAEINVAL;
        const Integer SE_IOLimit = WSAEMFILE;
        const Integer SE_WouldBlock = WSAEWOULDBLOCK;
        const Integer SE_NowInProgress = WSAEINPROGRESS;
        const Integer SE_AlreadyInProgress = WSAEALREADY;
        const Integer SE_DestAddrReq = WSAEDESTADDRREQ;
        const Integer SE_MessageSize = WSAEMSGSIZE;
        const Integer SE_ProtoNotSupported = WSAEPROTONOSUPPORT;
        const Integer SE_SocketNotSupported = WSAESOCKTNOSUPPORT;
        const Integer SE_PFNotSupported = WSAEPFNOSUPPORT;
        const Integer SE_AFNotSupported = WSAEAFNOSUPPORT;
        const Integer SE_AddrInUse = WSAEADDRINUSE;
        const Integer SE_AddrNotAvail = WSAEADDRNOTAVAIL;
        const Integer SE_NetworkDown = WSAENETDOWN;
        const Integer SE_NetworkUnreachable = WSAENETUNREACH;
        const Integer SE_ConnectionDropped = WSAENETRESET;
        const Integer SE_ConnectionAborted = WSAECONNABORTED;
        const Integer SE_ConnectionReset = WSAECONNRESET;
        const Integer SE_NoBufferSpace = WSAENOBUFS;
        const Integer SE_AlreadyConnected = WSAEISCONN;
        const Integer SE_NotConnected = WSAENOTCONN;
        const Integer SE_SocketShutdown = WSAESHUTDOWN;
        const Integer SE_ConnectionTimeout = WSAETIMEDOUT;
        const Integer SE_ConnectionRefused = WSAECONNREFUSED;
        const Integer SE_HostIsDown = WSAEHOSTDOWN;
        const Integer SE_HostUnreachable = WSAEHOSTUNREACH;
        const Integer SE_ProcessLimit = WSAEPROCLIM;
        #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
        const Integer SE_Uninitialized = -1;
        const Integer SE_NetworkNotReady = -1;
        const Integer SE_Interrupted = EINTR;
        const Integer SE_AccessDenied = EACCES;
        const Integer SE_InvalidAddr = EFAULT;
        const Integer SE_InvalidParam = EINVAL;
        const Integer SE_IOLimit = EMFILE;
        const Integer SE_WouldBlock = EWOULDBLOCK;
        const Integer SE_NowInProgress = EINPROGRESS;
        const Integer SE_AlreadyInProgress = EALREADY;
        const Integer SE_DestAddrReq = EDESTADDRREQ;
        const Integer SE_MessageSize = EMSGSIZE;
        const Integer SE_ProtoNotSupported = EPROTONOSUPPORT;
        const Integer SE_SocketNotSupported = ESOCKTNOSUPPORT;
        const Integer SE_PFNotSupported = EPFNOSUPPORT;
        const Integer SE_AFNotSupported = EAFNOSUPPORT;
        const Integer SE_AddrInUse = EADDRINUSE;
        const Integer SE_AddrNotAvail = EADDRNOTAVAIL;
        const Integer SE_NetworkDown = ENETDOWN;
        const Integer SE_NetworkUnreachable = ENETUNREACH;
        const Integer SE_ConnectionDropped = ENETRESET;
        const Integer SE_ConnectionAborted = ECONNABORTED;
        const Integer SE_ConnectionReset = ECONNRESET;
        const Integer SE_NoBufferSpace = ENOBUFS;
        const Integer SE_AlreadyConnected = EISCONN;
        const Integer SE_NotConnected = ENOTCONN;
        const Integer SE_SocketShutdown = ESHUTDOWN;
        const Integer SE_ConnectionTimeout = ETIMEDOUT;
        const Integer SE_ConnectionRefused = ECONNREFUSED;
        const Integer SE_HostIsDown = EHOSTDOWN;
        const Integer SE_HostUnreachable = EHOSTUNREACH;
        const Integer SE_ProcessLimit = EPROCLIM;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // Socket Errors - Could occur, but you shouldn't see because of our API wrapper

        #if defined(MEZZ_WINDOWS)
        const Integer SE_NonSocket = WSAENOTSOCK;
        const Integer SE_InvalidProtoType = WSAEPROTOTYPE;
        const Integer SE_InvalidProtoOpt = WSAENOPROTOOPT;
        const Integer SE_InvalidOperation = WSAEOPNOTSUPP;
        #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
        const Integer SE_NonSocket = ENOTSOCK;
        const Integer SE_InvalidProtoType = EPROTOTYPE;
        const Integer SE_InvalidProtoOpt = ENOPROTOOPT;
        const Integer SE_InvalidOperation = EOPNOTSUPP;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        // Socket Methods

        Socket::Socket() :
            InternalSocket(NULL)
            {  }

        Socket::~Socket()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole Socket::SetBlocking(const Boole Block)
            { return this->InternalSocket->SetBlocking(Block); }

        Boole Socket::GetBlocking() const
            { return this->InternalSocket->GetBlocking(); }

        Integer Socket::GetNumBytesAvailable() const
            { return this->InternalSocket->GetNumBytesAvailable(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Core Operations

        Boole Socket::Bind(const UInt16 Port)
            { return this->InternalSocket->Bind(Port); }

        Boole Socket::Bind(const SystemAddress& Address)
            { return this->InternalSocket->Bind(Address); }

        Boole Socket::Connect(const SystemAddress& Address)
            { return this->InternalSocket->Connect(Address); }

        Integer Socket::Send(const void* Buffer, const UInt32 BufSize) const
            { return this->InternalSocket->Send(Buffer,BufSize,0); }

        Integer Socket::Receive(void* Buffer, const UInt32 BufSize) const
            { return this->InternalSocket->Receive(Buffer,BufSize,0); }

        Integer Socket::ReceivePeek(void* Buffer, const UInt32 BufSize) const
            { return this->InternalSocket->Receive(Buffer,BufSize,MSG_PEEK); }

        Boole Socket::Shutdown(const Network::SocketShutdown How)
            { return this->InternalSocket->Shutdown(How); }

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration

        void Socket::SetReuseAddress(const Boole Reuse)
        {
            int Value = ( Reuse ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_ReuseAddress,(char*)&Value,sizeof(Value));
        }

        Boole Socket::GetReuseAddress() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::SOL_Socket,Network::SockOpts_ReuseAddress,(char*)&Value,&Size);
            return ( Value != 0 );
        }

        void Socket::SetDontRoute(const Boole Route)
        {
            int Value = ( Route ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_DontRoute,(char*)&Value,sizeof(Value));
        }

        Boole Socket::GetDontRoute() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::SOL_Socket,Network::SockOpts_DontRoute,(char*)&Value,&Size);
            return ( Value != 0 );
        }

        void Socket::SetDebugInformation(const Boole Debug)
        {
            int Value = ( Debug ? 1 : 0 );
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_DebugInfo,(char*)&Value,sizeof(Value));
        }

        Boole Socket::GetDebugInformation() const
        {
            int Value = 0;
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::SOL_Socket,Network::SockOpts_DebugInfo,(char*)&Value,&Size);
            return ( Value != 0 );
        }

        void Socket::SetSendBufferSize(const Integer Size)
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = ( Size < 0 ? 0 : Size );
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_SendBufSize,(char*)&Value,sizeof(Value));
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_SendBufSize,(char*)&Size,sizeof(Size));
            #endif
        }

        Integer Socket::GetSendBufferSize() const
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = 0;
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            int Value = 0;
            #endif
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::SOL_Socket,Network::SockOpts_SendBufSize,(char*)&Value,&Size);
            return static_cast<Integer>(Value);
        }

        void Socket::SetReceiveBufferSize(const Integer Size)
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = ( Size < 0 ? 0 : Size );
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_RecieveBufSize,(char*)&Value,sizeof(Value));
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            this->InternalSocket->SetSocketOption(Network::SOL_Socket,Network::SockOpts_RecieveBufSize,(char*)&Size,sizeof(Size));
            #endif
        }

        Integer Socket::GetReceiveBufferSize() const
        {
            #if defined(MEZZ_WINDOWS)
            DWORD Value = 0;
            #elif defined(MEZZ_MACOSX) || defined(MEZZ_LINUX)
            int Value = 0;
            #endif
            AddrLen Size;
            this->InternalSocket->GetSocketOption(Network::SOL_Socket,Network::SockOpts_RecieveBufSize,(char*)&Value,&Size);
            return static_cast<Integer>(Value);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Error Handling

        Integer Socket::GetLastError()
            { return PlatformSocket::GetLastError(); }

        void Socket::ThrowError(const Integer ErrorCode)
            { PlatformSocket::ThrowError(ErrorCode); }

        void Socket::ThrowLastError()
            { PlatformSocket::ThrowLastError(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal

        PlatformSocket* Socket::_GetPlatformSocket() const
            { return this->InternalSocket; }
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
