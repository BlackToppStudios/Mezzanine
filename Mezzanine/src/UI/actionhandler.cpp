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
#ifndef _uiactionhandler_cpp
#define _uiactionhandler_cpp

#include "UI/actionhandler.h"
#include "UI/action.h"

namespace Mezzanine
{
    namespace UI
    {
        ActionHandler::ActionHandler()
        {

        }

        ActionHandler::~ActionHandler()
        {

        }

        ///////////////////////////////////////////////////////////////////////////////
        // Action Management

        Action* ActionHandler::CreateAction(const String& Name)
        {
            ActionIterator ActIt = Actions.find(Name);
            if( ActIt == Actions.end() )
            {
                Action* NewAction = new Action(Name,this);
                Actions.insert( std::pair<String,Action*>(Name,NewAction) );
                return NewAction;
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"An Action with the name \"" + Name + "\" already exists.");
            }
        }

        Action* ActionHandler::GetAction(const String& Name)
        {
            ActionIterator ActIt = Actions.find(Name);
            if( ActIt != Actions.end() ) return (*ActIt).second;
            else return NULL;
        }

        void ActionHandler::DestroyAction(Action* ToBeDestroyed)
        {
            Unbind(ToBeDestroyed);
            ActionIterator ActIt = Actions.find( ToBeDestroyed->GetName() );
            if( ActIt != Actions.end() )
            {
                delete ToBeDestroyed;
                Actions.erase(ActIt);
            }
        }

        void ActionHandler::DestroyAllActions()
        {
            UnbindAll();
            for( ActionIterator ActIt = Actions.begin() ; ActIt != Actions.end() ; ++ActIt )
            {
                delete (*ActIt).second;
            }
            Actions.clear();
        }

        ActionHandler::ActionIterator ActionHandler::BeginAction()
        {
            return Actions.begin();
        }

        ActionHandler::ActionIterator ActionHandler::EndAction()
        {
            return Actions.end();
        }

        ActionHandler::ConstActionIterator ActionHandler::BeginAction() const
        {
            return Actions.begin();
        }

        ActionHandler::ConstActionIterator ActionHandler::EndAction() const
        {
            return Actions.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Binding Methods

        ActionHandler::ConstBindingRange ActionHandler::GetActionsBoundToCode(const Input::MetaCode& Code)
        {
            return Bindings.equal_range( Input::MetaCodeKey(Code) );
        }

        void ActionHandler::Bind(const Input::MetaCode& Code, Action* ToBind, bool ForceUnique)
        {
            Input::MetaCodeKey Temp(Code);
            if( ForceUnique )
            {
                BindingRange Range = Bindings.equal_range(Temp);
                if( Range.first != Range.second )
                {
                    Bindings.erase(Range.first,Range.second);
                }
            }
            Bindings.insert( BindingPair(Temp,ToBind) );
        }

        void ActionHandler::Unbind(const Input::MetaCode& Code)
        {
            BindingRange Range = Bindings.equal_range( Input::MetaCodeKey(Code) );
            for( BindingIterator BindIt = Range.first ; BindIt != Range.second ; ++BindIt )
                (*BindIt).second = NULL;
        }

        void ActionHandler::Unbind(Action* ToUnbind)
        {
            for( BindingIterator BindIt = Bindings.begin() ; BindIt != Bindings.end() ; ++BindIt )
            {
                if( ToUnbind == (*BindIt).second )
                    (*BindIt).second = NULL;
            }
        }

        void ActionHandler::UnbindAll()
        {
            for( BindingIterator BindIt = Bindings.begin() ; BindIt != Bindings.end() ; ++BindIt )
            {
                (*BindIt).second = NULL;
            }
        }

        void ActionHandler::RemoveAllBindings()
        {
            Bindings.clear();
        }

        ActionHandler::BindingIterator ActionHandler::BeginBinding()
        {
            return Bindings.begin();
        }

        ActionHandler::BindingIterator ActionHandler::EndBinding()
        {
            return Bindings.end();
        }

        ActionHandler::ConstBindingIterator ActionHandler::BeginBinding() const
        {
            return Bindings.begin();
        }

        ActionHandler::ConstBindingIterator ActionHandler::EndBinding() const
        {
            return Bindings.end();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void ActionHandler::_NotifyActionActivated(Action* BeingActivated)
        {
            for( ActivatedIterator ActIt = ActivatedActions.begin() ; ActIt != ActivatedActions.end() ; ++ActIt )
            {
                if( BeingActivated == (*ActIt) )
                    return;
            }
            ActivatedActions.push_back(BeingActivated);
        }

        bool ActionHandler::_HandleInput(const Input::MetaCode& Code)
        {
            Input::MetaCodeKey Temp(Code);
            BindingRange CodeRange = Bindings.equal_range( Temp );
            if( CodeRange.first != CodeRange.second )
            {

            }
            return false;
        }

        void ActionHandler::_ProcessAllActions()
        {

        }
    }//UI
}//Mezzanine

#endif
