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

#ifndef _networkpacket_h
#define _networkpacket_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Network
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A base class for packing information to be transferred over the network.
        /// @remarks Notes about Packets and Fragmentation: @n
        /// When creating packets for any application there should be size considerations.  Routers are permitted to fragment
        /// any packet that it cannot send along to the destination host due to hardware limitations that exist along the path
        /// the packet is taking.  There are some exceptions to this.  In the IPv4 options a flag can be set to disable
        /// fragmentation performed by routers.  In the event that a packet would be fragmented with the flag present the packet
        /// will be dropped and an error sent back saying it couldn't be delivered.  In IPv6 fragmentation simply cannot happen;
        /// it is disabled as per the IPv6 specification and packets are dropped and an error sent back just like with the IPv4
        /// flag.  The error sent back in both cases is often manifested as "Destination Unreachable". @n
        /// So it is important to know how big of a transmission is supported between the local host and the remote host to
        /// facilitate error-free and IP agnostic transmissions, or to just avoid fragmentation altogether that could generate
        /// odd errors if one fragment goes missing.  This is primarily a concern for UDP, which is a "fire-and-forget" protocol
        /// without any redundancy.  However it can still be useful to know for TCP for making some optimizations.  The big terms
        /// in relation to all of this are MTU(Maximum Transmission Unit) and MSS(Maximum Segment Size).  MTU is the maximum total
        /// size of a packet without fragmentation that exists between two hosts.  MSS is very closely related to the MTU and
        /// relates specifically to TCP; it is the maximum size of a single segment of a TCP stream. @n
        /// I mentioned hardware limitations earlier.  Each piece of internetworking equipment can and likely will implement their
        /// own MTU.  So the MTU is highly variable, even when connecting to the same host if it happens to choose a different
        /// path(which can happen even for something simple as the return trip on the same connection).  The more internetworking
        /// devices you encounter the most likely you are to encounter a device with a low MTU.  This is double true for devices
        /// that exist on networks you can't control (I.E. the internet).  Fortunately there are some standards in place that help
        /// us out. @n
        /// There are two things to keep in mind regarding standards relating to MTU, on is the standards relating to the actual
        /// MTU to be set, and the "minimum maximum reassembly buffer size" that must be set that determines the ideal MTU for a
        /// given device.  IETF RFC 1122 states the minimum MTU for IPv4 must be 68.  Likewise, IETF RFC 1122 states the minimum
        /// MTU for IPv6 is 1280.  Both of these standards go on to specify their reassembly buffer sizes to be at least 576 and
        /// 1500, respectively.  These numbers are the recommended minimums for MTU on any connected device.  While these numbers
        /// may be low, they are also the highest that can be expected among devices encountered on the internet.  Real world MTU
        /// can be higher and likely will be in some cases, but there is no hard and fast rules for that.  So the numbers for the
        /// IPv4 and IPv6 reassembly buffers will be our sane defaults prior to any actual detection along a path between
        /// connected hosts. @n
        /// So we have the total size of a transmission, but we're not quite set just yet.  There are headers created by the OS
        /// upon transmission of any given message that take up space in the packet.  So we must take the size of the headers and
        /// subtract that from the values mentioned above.  Both layer 3 and layer 4 protocols have packet headers that count
        /// against us here.  Most of them also have options so their headers are somewhat of a variable length.  The UDP is the
        /// most straightforward, with an 8 byte header and no available options.  TCP is a bit more complicated with a 40 byte
        /// header and depending on options can be up to an additional 40 bytes.  In other words the bulkiest TCP header is 10
        /// times the size of any UDP header!  But still 80 bytes isn't too bad, especially not when you are working with a
        /// reliable protocol such as TCP.  IPv4 and IPv6 is where things get a little weirder.  IPv4 has a simple 20 byte header
        /// and it's options can by up to 40 bytes on top of that, tripling it's size.  The IPv6 header is 40 bytes large, and it's
        /// options are...complicated.  IPv6 changed the packet format so in the core header it defines the next header, which
        /// could be a higher protocol header (such as TCP or UDP) or one of it's options.  Each option header has an identical
        /// field with information on it's offsets.  So options don't have a fixed layout, or really a fixed size, and can be
        /// chained for quite some length.  Unlike the other options values, using the max is just too large and too unrealistic
        /// to factor in.  Instead 40 bytes of space for possible options is a sane value to set aside. @n
        /// The static members that exist on the Packet class exist to help facilitate this math and provide constants to compare
        /// to when creating a message to be packed into a packet.  These values are meant to represent sane defaults to keep your
        /// packets under (in size) in order to avoid delivery complications for transmissions.  It is entirely possible, even
        /// common, for you to be able to sent packets larger than these sizes without experiencing any issues.  This could be
        /// as simple as the options fields for the packets sent aren't used, or the MTU between you and the remote host is much
        /// larger than what is expected here.  In situations where you need to send a transmission larger than the static values
        /// given in those constants, then the MTU should be tested before arbitrarily packing more data into your packets.  Also
        /// keep in mind that these values are intended for internet transmissions, and transmissions over LAN can and will have a
        /// much large MTU in nearly all cases.  Regardless, testing is valuable.
        /// @todo Update this with information on how to manually detect the MTU between local and remote hosts.
        ///////////////////////////////////////
        class MEZZ_LIB Packet
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Static Data Members

            /// @brief The maximum size an entire single transmission can be without fragmenting/rejection over IPv4.
            static const Whole DefaultIPv4MTU;
            /// @brief The maximum size an entire single transmission can be without fragmenting/rejection over IPv6.
            static const Whole DefaultIPv6MTU;
            /// @brief The maximum size the message portion of a UDP packet can be without fragmenting/rejection with IPv4.
            static const Whole DefaultUDPv4MsgSize;
            /// @brief The maximum size the message portion of a UDP packet can be without being rejected with IPv6.
            static const Whole DefaultUDPv6MsgSize;
            /// @brief The maximum size the message portion of a TCP packet can be without segmenting with IPv4.
            static const Whole DefaultTCPv4MsgSize;
            /// @brief The maximum size the message portion of a TCP packet can be without segmenting with IPv6.
            static const Whole DefaultTCPv6MsgSize;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            Packet();
            /// @brief Class destructor.
            virtual ~Packet();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the type of packet.
            /// @return Returns a UInt15 that is the type ID of this packet.
            virtual UInt16 GetPacketType() const = 0;
            /// @brief Gets the size this packet should be according to it's type.
            /// @return Returns the size this packet should be based on it's type ID in bytes.
            virtual Whole GetExpectedSize() const = 0;

            /// @brief Gets the actual size of this packet.
            /// @return Returns the current size of the buffer in this packet.
            virtual Whole GetSize() const = 0;
            /// @brief Gets whether or not this packet is the size it should be.
            /// @return Returns true if this packet is the size that is expected for it's type, false otherwise.
            Boole IsExpectedSize() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Buffer Management

            /// @brief Sets the raw data in this packet.
            /// @param Buffer The data to be set.
            /// @param BufSize The size of the buffer being set.
            virtual void SetData(const void* Buffer, const Whole BufSize) = 0;
            /// @brief Appends additional content onto this packet.
            /// @param Buffer The data to be appended.
            /// @param BufSize The size of the buffer being appended.
            virtual void AppendData(const void* Buffer, const Whole BufSize) = 0;
            /// @brief Gets the raw data of this packet.
            /// @return Returns a void pointer to the internal buffer for this packet.
            virtual void* GetData() = 0;
            /// @brief Gets the raw data of this packet.
            /// @return Returns a void pointer to the internal buffer for this packet.
            virtual const void* GetData() const = 0;
            /// @brief Clears or resets the contents of this packet.
            virtual void ClearData() = 0;
        };//Packet
    }//Network
}//Mezzanine

#endif
