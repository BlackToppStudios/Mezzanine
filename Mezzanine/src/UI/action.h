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
#ifndef _uiaction_h
#define _uiaction_h

#include "eventpublisher.h"
#include "Input/metacode.h"

namespace Mezzanine
{
    namespace UI
    {
        class ActionHandler;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for action specific events.
        ///////////////////////////////////////
        class MEZZ_LIB ActionEvent : public Event
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The identification of the source firing this event.
            const String ActionName;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Source The identification of the action firing this event.
            ActionEvent(const EventNameType& Name, const String& Source) :
                Event(Name), ActionName(Source) {  }
            /// @brief Class destructor.
            virtual ~ActionEvent() {  }
        };//ActionEvent

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class represents an action to be taken.  Can have multiple inputs bound to it.
        ///////////////////////////////////////
        class MEZZ_LIB Action : public EventPublisher
        {
        public:
            /// @brief Event name for when this Action is activated.
            static const EventNameType EventActionActivated;
            /// @brief Event name for when this Action is deactivated.
            static const EventNameType EventActionDeactivated;
        protected:
            friend class ActionHandler;
            /// @internal
            /// @brief The name of this action.
            const String ActionName;
            /// @internal
            /// @brief A pointer to the handler that owns this Action.
            ActionHandler* Parent;
        //public:
            /// @brief Class constructor.
            /// @param Name The name to be given to the action.
            /// @param Handler A pointer to this actions creator.
            Action(const String& Name, ActionHandler* Handler);
            /// @brief Class destructor.
            ~Action();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the name of this Action.
            /// @return Returns a const reference to a string containing the name of this Action.
            const String& GetName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Runs all logic associated with this Action being activated.
            void _OnActivateAction();
            /// @internal
            /// @brief Runs all logic associated with this Action being deactivated.
            void _OnDeactivateAction();
            /// @internal
            /// @brief Handles input passed to this Action.
            /// @param Code The MetaCode to be processed.
            /// @return Returns true if this input was consumed/handled, false otherwise.
            Boole _HandleInput(const Input::MetaCode& Code);
        };//Action
    }//UI
}//Mezzanine

#endif
