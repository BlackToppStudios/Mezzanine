//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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

#include "crossplatformexport.h"

namespace Mezzanine
{
    namespace Network
    {
        struct InternalSocketData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Socket
        /// @headerfile networksocket.h
        /// @brief This is a basic socket interface for the transmission and retrieval of packets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Socket
        {
            public:
                /// @enum SocketType
                /// @brief Simple enum to describe what type of socket this is.
                enum SocketType
                {
                    ST_TCP,
                    ST_UDP
                };
                /// @enum SocketError
                /// @brief This enum is used to convey the current error status of the socket.
                enum SocketError
                {

                };
            protected:
                InternalSocketData* ISD;
                SocketType SType;
            public:
                /// @brief Class constructor.
                /// @param Type The type of socket this is.
                Socket(const SocketType& Type);
                /// @brief Class destructor.
                virtual ~Socket();
                /// @brief Gets the type of socket that this is.
                /// @return Returns a socket type enum value, indicating what type of socket this is.
                SocketType GetType() const;
                /// @brief Gets the current error status of the socket.
                /// @return Returns a socket error enum value, indicating what error, if any, has occured on the socket.
                SocketError GetError() const;
        };//socket
    }//Network
}//Mezzanine

#endif

#endif //MEZZNETWORK
