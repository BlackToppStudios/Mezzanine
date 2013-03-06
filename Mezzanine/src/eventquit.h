//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef EVENTQUIT_H
#define EVENTQUIT_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "crossplatformexport.h"
#include "eventbase.h"
#include "XML/xml.h"

namespace Mezzanine
{

    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventQuit
    /// @headerfile eventquit.h
    /// @brief This is intended to convey the message that quitting needs to happen
    /// @details This stores not data other than the fact that this is a Quit
    /// event. This means that either an underlying system like the OS or a service
    /// has requested a quit, or the application has manually put a quit message in
    /// the queue to signal that a graceful shutdown needs to occur.
    class MEZZ_LIB EventQuit : public EventBase
    {
        private:

        public:
            /// @brief This returns EventType::QuitMessage
            /// @details  This returns the kind of message this is, specifcally EventType::QuitMessage . If
            /// this functions returns EventType::QuitMessage, then and event pointer can safely be
            /// cast to Mezzanine::EventQuit . This method is inherited from Mezzanine::Event .
            virtual EventType GetType() const;
    };

}
///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

/// @brief Serializes the passed Mezzanine::EventQuit to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::EventQuit to be serialized
/// @return this retruns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventQuit& Ev);

/// @brief Deserialize a Mezzanine::EventQuit
/// @param stream The istream to get the xml from to (re)make the Mezzanine::EventQuit.
/// @param Ev the Mezzanine::EventQuit to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::EventQuit that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventQuit& Ev);

/// @brief Set all values of a Mezzanine::EventQuit from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::EventQuit.
/// @param Ev the Mezzanine::EventQuit to be reset.
/// @return This returns theXML::Node that was passed in.
Mezzanine::XML::Node& MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::EventQuit& Ev);


#endif

