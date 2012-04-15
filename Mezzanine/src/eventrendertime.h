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
#ifndef EVENTRENDERTIME_H
#define EVENTRENDERTIME_H
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time since rendering events that occured recently
///////////////////////////////////////

#include "crossplatformexport.h"
#include "eventbase.h"
#include "datatypes.h"
#include "xml.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class EventRenderTime
    /// @headerfile eventrendertime.h
    /// @brief This communicates the amount of time since the world was rendered
    /// @details This stores in milliseconds the amount of time since the last
    /// rendering of the world.
    class MEZZ_LIB EventRenderTime : public EventBase
    {
        private:
            Whole Rendertime;

        public:
            /// @brief The Constructor
            /// @details This is the only way to set the time
            /// @param Milliseconds As it says, the amount of milliseconds since the last rendering
            EventRenderTime (Whole Milliseconds=0);

            /// @brief Returns that this event is a EventType::RenderTime
            /// @details This is primarily for the benefit of sorting thorugh event pointers. If
            /// this functions returns EventType::RenderTime, then and event pointer can safely be
            /// cast to Mezzanine::EventRenderTime . This method is inherited from Mezzanine::Event .
            virtual EventType GetType() const;

            /// @brief Returns the a floating point value with the amount of time.
            /// @details Returns the a floating point value with the amount of time.
            /// @return A floating point value with the amount of time.
            Whole getMilliSecondsSinceLastFrame() const;

            /// @brief Assignment operator
            /// @param rhs the Right Hand object in the assignment operator
            void operator= (const EventRenderTime& rhs);

    };
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::EventRenderTime to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::EventRenderTime to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::EventRenderTime& Ev);

/// @brief Deserialize a Mezzanine::EventRenderTime
/// @param stream The istream to get the xml from to (re)make the Mezzanine::EventRenderTime.
/// @param Ev the Mezzanine::EventRenderTime to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::EventRenderTime that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventRenderTime& Ev);

/// @brief Set all values of a Mezzanine::EventRenderTime from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::EventRenderTime.
/// @param Ev the Mezzanine::EventRenderTime to be reset.
/// @return This returns thexml::Node that was passed in.
Mezzanine::xml::Node& MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::EventRenderTime& Ev);

#endif // \MEZZXML



#endif
