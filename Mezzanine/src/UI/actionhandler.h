// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _uiactionhandler_h
#define _uiactionhandler_h

#include "Input/metacodekey.h"

namespace Mezzanine
{
    namespace UI
    {
        class Action;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is the core class responsible for the management of actions.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ActionHandler
        {
        public:
            /// @brief Container type for Action instances that are stored by this class.
            typedef std::map< String,Action* >                   ActionContainer;
            /// @brief Iterator type for Action instances that are stored in this class.
            typedef ActionContainer::iterator                    ActionIterator;
            /// @brief Const Iterator type for Action instances that are stored in this class.
            typedef ActionContainer::const_iterator              ConstActionIterator;
            /// @brief Container type for in-progress Action instances that are stored by this class.
            typedef std::vector< Action* >                       ActivatedContainer;
            /// @brief Iterator type for in-progress Action instances that are stored in this class.
            typedef ActivatedContainer::iterator                 ActivatedIterator;
            /// @brief Const Iterator type for in-progress Action instances that are stored in this class.
            typedef ActivatedContainer::const_iterator           ConstActivatedIterator;
            /// @brief Container for bindings that connect to input codes to actions.
            typedef std::multimap< Input::MetaCodeKey,Action* >  BindingContainer;
            /// @brief Iterator type for input-action bindings stored in this class.
            typedef BindingContainer::iterator                   BindingIterator;
            /// @brief Const Iterator type for input-action bindings stored in this class.
            typedef BindingContainer::const_iterator             ConstBindingIterator;
            /// @brief A convenience type for the actual input-action binding.
            typedef std::pair< Input::MetaCodeKey,Action* >      BindingPair;
            /// @brief A convenience type for an iterator range of bindings.
            typedef std::pair<BindingIterator,BindingIterator>   BindingRange;
            /// @brief A convenience type for a const iterator range of bindings.
            typedef std::pair<ConstBindingIterator,ConstBindingIterator>  ConstBindingRange;
        protected:
            /// @internal
            /// @brief Container storing all registered actions.
            ActionContainer Actions;
            /// @internal
            /// @brief Container storing all the bindings that connect inputs to actions.
            BindingContainer Bindings;
            /// @internal
            /// @brief Container storing all the actions that are in-progress.
            ActivatedContainer ActivatedActions;
        public:
            /// @brief Class constructor.
            ActionHandler();
            /// @brief Class destructor.
            ~ActionHandler();

            ///////////////////////////////////////////////////////////////////////////////
            // Action Management

            /// @brief Creates a new Action that can be bound to a MetaCode.
            /// @param Name The name to be given to the created Action.
            /// @return Returns a pointer to the created Action.
            Action* CreateAction(const String& Name);
            /// @brief Gets an Action by name.
            /// @param Name The name of the Action to retrieve.
            /// @return Returns a pointer to the named Action.
            Action* GetAction(const String& Name);
            /// @brief Destroy's an action.
            /// @param ToBeDestroyed The action to be destroyed.
            void DestroyAction(Action* ToBeDestroyed);
            /// @brief Destroys all Actions being stored by this Handler.
            void DestroyAllActions();

            /// @brief Gets an iterator to the first Action.
            /// @return Returns an iterator to the first Action stored by this handler.
            ActionIterator BeginAction();
            /// @brief Gets an iterator to one-passed-the-last Action.
            /// @return Returns an iterator to one-passed-the-last Action stored by this handler.
            ActionIterator EndAction();
            /// @brief Gets a const iterator to the first Action.
            /// @return Returns a const iterator to the first Action stored by this handler.
            ConstActionIterator BeginAction() const;
            /// @brief Gets a const iterator to one-passed-the-last Action.
            /// @return Returns a const iterator to one-passed-the-last Action stored by this handler.
            ConstActionIterator EndAction() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Binding Methods

            /// @brief Gets all Actions bound to a MetaCode.
            /// @param Code The MetaCode to use to search for bound Actions.
            /// @return Returns the first and one-passed-the-last iterators representing all of the Actions bound to the provided code.
            ConstBindingRange GetActionsBoundToCode(const Input::MetaCode& Code);

            /// @brief Binds a MetaCode to an action, making the action fire when this handler recieves the code.
            /// @note Only one Action can be bound to a given code.  Binding a second Action to a code will unbind the first.
            /// @param Code The code to trigger the Action.
            /// @param ToBind The Action to be triggered.
            /// @param ForceUnique If true this will clear any previous entries that are equal to the MetaCode provided in the binding multimap.
            void Bind(const Input::MetaCode& Code, Action* ToBind, bool ForceUnique = true);
            /// @brief Unbinds Actions via MetaCode.
            /// @note This can unbind multiple Actions.
            /// @param Code The MetaCode to find and remove all bindings to actions.
            void Unbind(const Input::MetaCode& Code);
            /// @brief Unbinds an Action via Action pointer.
            /// @note This method will iterate over all the elements in the binding multimap in search of all bindings to the provided Action.
            /// @param ToUnbind The Action to unbind from any and all MetaCodes.
            void Unbind(Action* ToUnbind);
            /// @brief Unbinds all actions in this handler.
            /// @note This does not delete entries for MetaCodeKey's, this simply sets the Action value to NULL.
            void UnbindAll();
            /// @brief Completely removes all bindings from this Handler.
            void RemoveAllBindings();

            /// @brief Gets an iterator to the first Binding.
            /// @return Returns an iterator to the first Binding stored by this handler.
            BindingIterator BeginBinding();
            /// @brief Gets an iterator to one-passed-the-last Binding.
            /// @return Returns an iterator to one-passed-the-last Binding stored by this handler.
            BindingIterator EndBinding();
            /// @brief Gets a const iterator to the first Binding.
            /// @return Returns a const iterator to the first Binding stored by this handler.
            ConstBindingIterator BeginBinding() const;
            /// @brief Gets a const iterator to one-passed-the-last Binding.
            /// @return Returns a const iterator to one-passed-the-last Binding stored by this handler.
            ConstBindingIterator EndBinding() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Used by Actions to notify this handler it was activated.
            /// @param BeingActivated The Action calling this method and being activated.
            void _NotifyActionActivated(Action* BeingActivated);
            /// @internal
            /// @brief Handles input passed to this handler.
            /// @param Code The MetaCode to be processed.
            /// @return Returns true if this input was consumed/handled, false otherwise.
            bool _HandleInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Processes all active actions, and deactivates them if necessary.
            void _ProcessAllActions();
        };//ActionHandler
    }//UI
}//Mezzanine

#endif
