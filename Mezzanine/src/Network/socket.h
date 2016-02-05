// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#ifndef _networksocket_h
#define _networksocket_h

#include "Network/networkenumerations.h"

#include "Network/packet.h"
#include "Network/systemaddress.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Socket Errors - Common

        /// @brief An unknown error occured.  This is for when our errors error.
        extern const Integer SE_Unknown;
        /// @brief The OS modules needed for networking are unavailable.
        extern const Integer SE_NetworkNotReady;
        /// @brief The OS modules for networking haven't been started.
        extern const Integer SE_Uninitialized;

        /// @brief The thread or process running a socket operation was terminated prematurely.
        extern const Integer SE_Interrupted;
        /// @brief Insufficient access permissions to perform specified socket operation.
        extern const Integer SE_AccessDenied;
        /// @brief The address data provided was faulty or invalid.
        extern const Integer SE_InvalidAddr;
        /// @brief An argument/parameter was provided to a socket operation.
        extern const Integer SE_InvalidParam;
        /// @brief Too many IO handles (Files and sockets share a pool) are open.  This may be a global, process, or thread limit depending on platform.
        extern const Integer SE_IOLimit;
        /// @brief A resource is temporarily unavailable.  This can be returned from operations on non-blocking sockets that cannot be completed immediately.
        extern const Integer SE_WouldBlock;
        /// @brief The socket is non-blocking and the operation cannot be completed immediately, but is now in progress.
        extern const Integer SE_NowInProgress;
        /// @brief The socket is non-blocking and an operation is already in progress.
        extern const Integer SE_AlreadyInProgress;
        /// @brief Specifying a destination address is required for the erroring operation.  This often applies to a socket that isn't connected or has no peer address set in the socket call.
        extern const Integer SE_DestAddrReq;
        /// @brief The message to be sent through a socket is too large.  The buffer provided to be sent on a datagram socket was larger than the internal buffer or another network limit.
        extern const Integer SE_MessageSize;
        /// @brief The system does not have the specified protocol installed or configured.
        extern const Integer SE_ProtoNotSupported;
        /// @brief Either the specified socket type does not exist or support for the specified socket type with the specified address family does not exist on the system.
        extern const Integer SE_SocketNotSupported;
        /// @brief The system does not have the specified protocol family installed or configured.  This is mostly interchangable with SE_AFNotSupported.
        extern const Integer SE_PFNotSupported;
        /// @brief The system does not have the specified address family installed or configured.  This is mostly interchangable with SE_PFNotSupported.
        extern const Integer SE_AFNotSupported;
        /// @brief The address specified for binding is already in use by another socket.
        extern const Integer SE_AddrInUse;
        /// @brief The address specified is not valid in the current context.
        extern const Integer SE_AddrNotAvail;
        /// @brief The operation encountered a dead network.  This could be because of a failure of the network system, the interface, of the local network itself.
        extern const Integer SE_NetworkDown;
        /// @brief The operation could not find a valid route to the remote host specified.
        extern const Integer SE_NetworkUnreachable;
        /// @brief The connection was dropped.  Usually occurs after keep-alive detected a failure while an operation was in progress.
        extern const Integer SE_ConnectionDropped;
        /// @brief The connection was aborted by software on the local host.
        extern const Integer SE_ConnectionAborted;
        /// @brief The connection was reset/closed by the remote host.  This can also occur if keep-alive detected a failure.
        extern const Integer SE_ConnectionReset;
        /// @brief Either the OS lacked sufficient buffer space or the queue was full.
        extern const Integer SE_NoBufferSpace;
        /// @brief The socket is already connected.  Additional connections are not supported.
        extern const Integer SE_AlreadyConnected;
        /// @brief The socket is not connected.  Operations requiring a connection cannot be performed.
        extern const Integer SE_NotConnected;
        /// @brief The socket has already been shut down and operations cannot be performed on it.
        extern const Integer SE_SocketShutdown;
        /// @brief The connection timed out.  The connect failed because the remote host didn't respond within the timeout peroid.
        extern const Integer SE_ConnectionTimeout;
        /// @brief The connection was refused.  The remote host doesn't like you.  Probably because you smell funny.  Or slightly more likely no application was running on the specified port.
        extern const Integer SE_ConnectionRefused;
        /// @brief The operation failed because the remote host is down or doesn't exist.
        extern const Integer SE_HostIsDown;
        /// @brief No route to the specified host could be found.
        extern const Integer SE_HostUnreachable;
        /*/// @brief The number of processes interacting with the network subsystem has met or exceeded it's limit.
        extern const Integer SE_ProcessLimit;// */

        ///////////////////////////////////////////////////////////////////////////////
        // Socket Errors - Could occur, but you shouldn't see because of our API wrapper

        /// @brief Attempting to perform a socket operation on a handle that does not belong to a valid socket.
        extern const Integer SE_NonSocket;
        /// @brief The specified protocol is of the wrong type for the socket type specified.  This error occurs on socket creation.
        extern const Integer SE_InvalidProtoType;
        /// @brief An invalid option level was specified for a call setting or getting a socket option.
        extern const Integer SE_InvalidProtoOpt;
        /// @brief The operation requested is not supported for the socket provided.  For example "accept" being called on a datagram socket.
        extern const Integer SE_InvalidOperation;

        class PlatformSocket;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Socket
        /// @brief This is a basic socket interface for the transmission and retrieval of packets.
        /// @details
        /// @note This class is intented to house methods for the configuration of all Socket and IPv4 (because IPv6 is a superset of IPv4 in the
        /// case of socket options) options that can be set using a sockets API.  For the most part this is accomplished, however some functionality
        /// has been skipped.  On windows in particular, there are a ton of additional options related to multi-casting which may be addressed in the
        /// future.  Some socket options that are available cross-platform were also not implemented included: SO_ERROR, SO_LINGER, SO_RCVLOWAT,
        /// SO_RCVTIMEO, SO_SNDLOWAT, and SO_SNDTIMEO.  @n @n
        /// Some QoS functionality also didn't make the cut due to some strange circumstances.  The SO_PRIORITY socket option seems to simply not exist
        /// in Winsock.  SO_PRIORITY overlaps somewhat in purpose with the IP_TOS socket option on the IP level.  While that option does exist in
        /// Winsock, the API documentation for Winsock says explicitly to not use it, favoring the Windows QoS API instead.  This makes a crossplatform
        /// layer over these options non-trivial.  For both SO_PRIORITY and IP_TOS, what it does is set the priority flags in the IP header for
        /// transmissions going out the socket, which could impact how and when packets are handled starting as early as the hosts local machine.  This
        /// is very low level and while it's possible some game devs could make use of it, it is outside of the scope for most devs.  Thus for revision
        /// 1 of the network subsystem this is skipped.
        ///////////////////////////////////////
        class MEZZ_LIB Socket
        {
        /*public:
            /// @brief This enum is used to describe the current working status of the socket.
            enum SocketStatus
            {

            };// */
        protected:
            /// @internal
            /// @brief A pointer to the internal system socket.
            PlatformSocket* InternalSocket;
        public:
            /// @brief Class constructor.
            Socket();
            /// @brief Class destructor.
            virtual ~Socket();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /*/// @brief Gets the current status of this socket.
            /// @return Returns a SocketStatus enum value representing the current working state of this socket.
            virtual SocketStatus GetStatus() const = 0;// */

            /// @brief Gets the layer 3 protocol this socket is operating on.
            /// @return Returns a NetworkLayerProtocol enum value describing the layer 3 protocol being used.
            virtual NetworkLayerProtocol GetNetworkLayerProtocol() const = 0;
            /// @brief Gets the layer 4 protocol this socket is operating on.
            /// @return Returns a TransportLayerProtocol enum value describing the layer 4 protocol being used.
            virtual TransportLayerProtocol GetTransportLayerProtocol() const = 0;

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

            /// @brief Connects this socket to a remote host connected to the network.
            /// @remarks This method will block transmissions from any address other than the one the socket is connected to.
            /// @param Address The remote address to connect to.
            /// @return Returns true if the Connect was successful, false on a failure.
            Boole Connect(const SystemAddress& Address);

            /// @brief Sends raw data out the socket to it's configured peer.
            /// @remarks This should only be used on connected sockets.  Calling this method on unconnected sockets will error.
            /// @param Buffer A raw buffer to be sent.
            /// @param BufSize The size of the raw buffer to send.
            /// @return Returns the number of bytes sent, or -1 if there was an error.
            Integer Send(const void* Buffer, const UInt32 BufSize) const;

            /// @brief Reads raw data recieved on this socket from it's configured peer.
            /// @remarks This should only be used on connected sockets.  Calling this method on unconnected sockets will error.
            /// @param Buffer A raw buffer to be written to.
            /// @param BufSize The size of the raw buffer to write to.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer Receive(void* Buffer, const UInt32 BufSize) const;
            /// @brief Reads raw data recieved on this socket from it's configured peer, but doesn't remove that data from the receive buffer.
            /// @remarks This should only be used on connected sockets.  Calling this method on unconnected sockets will error.
            /// An additional call to Receive will return the same data returned by this call, as the data wasn't removed from the queue with this method.
            /// @param Buffer A raw buffer to be written to.
            /// @param BufSize The size of the raw buffer to write to.
            /// @return Returns the number of bytes received/written to the buffer, or -1 if there was an error.
            Integer ReceivePeek(void* Buffer, const UInt32 BufSize) const;

            /// @brief Disables some or all I/O on the socket.
            /// @param How Which I/O should be disabled (send, receive, or both).
            /// @return Returns true if the Shutdown was successful, false if there was a failure.
            Boole Shutdown(const Network::SocketShutdown How);

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets the number of hops permitted when sending to a non-multicast address before the message is destroyed.
            /// @remarks A "Hop" is a single stop at a router in the path to the destination host.  Limiting the number of hops taken prevents
            /// situations where a host cannot be reached, but it's state also can't be assessed in advance, thus the message just keeps moving
            /// around the network without possibly arriving, creating congestion.
            /// @param Hops The number of hops a message sent from this socket is allowed to make.  Cannot be greater than 255.
            virtual void SetNumUnicastHops(const Integer Hops) = 0;
            /// @brief Gets the number of hops permitted when sending to a non-multicast address before the message is destroyed.
            /// @return Returns the number of times a unicast message from this socket sent to a non-multicast address is allowed to be routed before being destroyed.
            virtual Integer GetNumUnicastHops() const = 0;

            /// @brief Sets whether or not local addresses can be re-used for socket binding.
            /// @param Reuse Whether or not the port/IPaddress this socket is bound to can be shared.
            void SetReuseAddress(const Boole Reuse);
            /// @brief Gets whether or not local addresses can be re-used for socket binding.
            /// @return Returns true if this socket can share its bound local address with other sockets, false otherwise.
            Boole GetReuseAddress() const;
            /// @brief Sets whether or not packets should be sent directly to other nodes on the network if possible.
            /// @param Route Whether or not more advanced routing through internetworking devices should be skipped if possible.
            void SetDontRoute(const Boole Route);
            /// @brief Gets whether or not packets should be sent directly to other nodes on the network if possible.
            /// @return Returns true if packets should be sent to the host directly if possible, false otherwise.
            Boole GetDontRoute() const;
            /// @brief Sets whether or not additional debugging information should be collected during socket operations.
            /// @param Debug Whether or not additional debug facilities in internal sockets should be enabled.
            void SetDebugInformation(const Boole Debug);
            /// @brief Gets whether or not additional debugging information should be collected during socket operations.
            /// @return Returns true if debug information on errors should be logged, false otherwise.
            Boole GetDebugInformation() const;
            /// @brief Sets the size of the buffer for sending packets.
            /// @remarks This buffer is not for singular packets, but for all the packets intended to be sent out to the network.  Datagram sockets are fire
            /// and forget so unless you are trying to send out a ton of packets in a single frame you are unlikely to fill the buffer before the kernel
            /// unloads it to the network.  In stream sockets however it is possible for a network failure to occur and cause some parts to need to be resent.
            /// In those cases adding more packets can cause the send buffer to become more populated and even fill up in extreme cases.
            /// @param Size The size to set the send buffer as in bytes.
            void SetSendBufferSize(const Integer Size);
            /// @brief Gets the size of the buffer for sending packets.
            /// @return Returns an Integer representing the size of the send buffer in bytes.
            Integer GetSendBufferSize() const;
            /// @brief Sets the size of the buffer for receiving packets.
            /// @remarks This buffer is not for singular packets but, for the entire backlog of packets you receive.  In stream sockets the sending host will
            /// be notified to slow down to preserve data.  In datagram sockets any packets received after the buffer is filled will have their contents
            /// discarded.
            /// @param Size The size to set the receive buffer as in bytes.
            void SetReceiveBufferSize(const Integer Size);
            /// @brief Gets the size of the buffer for receiving packets.
            /// @return Returns an Integer representing the size of the receive buffer in bytes.
            Integer GetReceiveBufferSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Error Handling

            /// @brief Gets the last error that occured on a socket.
            /// @return Returns the error code of the most recent socket error.
            static Integer GetLastError();
            /// @brief Throws an exception containing the message for a specific error code.
            /// @param ErrorCode The error code to throw an exception for.
            static void ThrowError(const Integer ErrorCode);
            /// @brief Throws an exception containing the message from the last socket error.
            static void ThrowLastError();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal

            /// @brief Starts the socket subsystem.
            /// @remarks This exists primarily because Winsock is lame.  Always returns success on non-winsock platforms.
            /// @return Returns zero if startup was successful, or an error code if something went wrong.
            static Integer Initialize();
            /// @brief Stops the socket subsystem.
            /// @remarks This exists primarily because Winsock is lame.  Always returns success on non-winsock platforms.
            /// @return Returns zero if shutdown was successful, or an error code if something went wrong.
            static Integer Deinitialize();

            /// @brief Gets a pointer to the internal socket.
            /// @return Returns a pointer to the internal platform-specific socket.
            PlatformSocket* _GetPlatformSocket() const;
        };//Socket
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
