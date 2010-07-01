//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distributionhttp://www.google.com/firefox

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#include "managerbase.h"


namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class CallBackManager
    /// @headerfile callbackmanager.h
    /// @brief This Stores callbacks for for use in the main loop
    /// @details This stores a series of pointers to functions that the main loop
    /// will call. This can be swapped out at any point in time with another
    /// CallBackManager to completely (or subtley) alter game behavior.
    class CallBackManager//: public ManagerBase
    {
        private:
            //pointers to engine internal structures
            //World* Game;

            //pointers to callback functions
            bool (*PreInputCallback)();
            bool (*PostInputCallback)();

            bool (*PrePhysicsCallback)();
            bool (*PostPhysicsCallback)();

            bool (*PreRenderCallback)();
            bool (*PostRenderCallback)();

        public:

        ///////////////////////////////////////////////////////////////////////////////
        // Important Methods
        ///////////////////////////////////////
            /// @brief Constructor
            /// @details This creates a usable but empty CallBackManager
            /// @param _Parent This is a pointer to the world that this callback manager works with.
            CallBackManager(World* _Parent);

            /// @brief Deconstructor
            /// @details Deconstructor Currently doesn't do very much
            virtual ~CallBackManager();

            /// @brief Empty Initilizor
            /// @details This class implements this for the sake of entension and compatibility this function does nothing
            virtual void Initialize();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::CallBackManager
//            virtual ManagerBase::ManagerTypeName GetType() const;


        ///////////////////////////////////////////////////////////////////////////////
        // PreInput Management
        ///////////////////////////////////////
            /// @brief This calls the PreInput Callback.
            /// @details This calls the PreInput Callback and returns true if the callback
            /// intends to continue with the mainloop, and false if the callback intend to
            /// end the mainloop.
            /// @return A bool that represents whether the mainloop should end.
            virtual bool PreInput();

            /// @brief Drops the PreInput pointer.
            /// @details Drops the PreInput pointer, this does not 'delete' the pointer, merely
            /// assigns it to zero.
            virtual void ErasePreInput();

            /// @brief This assigns a function to be the callback function for PreInput
            /// @details This assigns a function to be the callback function for PreInput.
            /// @param Callback This is a pointer to a function that returns a bool and accepts no arguments
            virtual void SetPreInput(bool (*Callback)());

            /// @brief Is the PreInput callback set or not.
            /// @details Is the PreInput callback set or not. The returns true if set or false otherwise.
            /// @return This returns true if the PreInput CallBack is set, false otherwise
            virtual bool IsPreInputCallbackSet() const;


        ///////////////////////////////////////////////////////////////////////////////
        // PostInput Management
        ///////////////////////////////////////
            /// @brief This calls the PostInput Callback.
            /// @details This calls the PostInput Callback and returns true if the callback
            /// intends to continue with the mainloop, and false if the callback intend to
            /// end the mainloop.
            /// @return A bool that represents whether the mainloop should end.
            virtual bool PostInput();

            /// @brief Drops the PostInput pointer.
            /// @details Drops the PostInput pointer, this does not 'delete' the pointer, merely
            /// assigns it to zero.
            virtual void ErasePostInput();

            /// @brief This assigns a function to be the callback function for PostInput
            /// @details This assigns a function to be the callback function for PostInput.
            /// @param Callback This is a pointer to a function that returns a bool and accepts no arguments
            virtual void SetPostInput(bool (*Callback)());

            /// @brief Is the PostInput callback set or not.
            /// @details Is the PostInput callback set or not. The returns true if set or false otherwise.
            /// @return This returns true if the PostInput CallBack is set, false otherwise
            virtual bool IsPostInputCallbackSet() const;


        ///////////////////////////////////////////////////////////////////////////////
        // PrePhysics Management
        ///////////////////////////////////////
            /// @brief This calls the PrePhysics Callback.
            /// @details This calls the PrePhysics Callback and returns true if the callback
            /// intends to continue with the mainloop, and false if the callback intend to
            /// end the mainloop.
            /// @return A bool that represents whether the mainloop should end.
            virtual bool PrePhysics();

            /// @brief Drops the PrePhysics pointer.
            /// @details Drops the PrePhysics pointer, this does not 'delete' the pointer, merely
            /// assigns it to zero.
            virtual void ErasePrePhysics();

            /// @brief This assigns a function to be the callback function for PrePhysics
            /// @details This assigns a function to be the callback function for PrePhysics.
            /// @param Callback This is a pointer to a function that returns a bool and accepts no arguments
            virtual void SetPrePhysics(bool (*Callback)());

            /// @brief Is the PrePhysics callback set or not.
            /// @details Is the PrePhysics callback set or not. The returns true if set or false otherwise.
            /// @return This returns true if the PrePhysics CallBack is set, false otherwise
            virtual bool IsPrePhysicsCallbackSet() const;


        ///////////////////////////////////////////////////////////////////////////////
        // PostPhysics Management
        ///////////////////////////////////////
            /// @brief This calls the PostPhysics Callback.
            /// @details This calls the PostPhysics Callback and returns true if the callback
            /// intends to continue with the mainloop, and false if the callback intend to
            /// end the mainloop.
            /// @return A bool that represents whether the mainloop should end.
            virtual bool PostPhysics();

            /// @brief Drops the PostPhysics pointer.
            /// @details Drops the PostPhysics pointer, this does not 'delete' the pointer, merely
            /// assigns it to zero.
            virtual void ErasePostPhysics();

            /// @brief This assigns a function to be the callback function for PostPhysics
            /// @details This assigns a function to be the callback function for PostPhysics.
            /// @param Callback This is a pointer to a function that returns a bool and accepts no arguments
            virtual void SetPostPhysics(bool (*Callback)());

            /// @brief Is the PostPhysics callback set or not.
            /// @details Is the PostPhysics callback set or not. The returns true if set or false otherwise.
            /// @return This returns true if the PostPhysics CallBack is set, false otherwise
            virtual bool IsPostPhysicsCallbackSet() const;


        ///////////////////////////////////////////////////////////////////////////////
        // PreRender Management
        ///////////////////////////////////////
            /// @brief This calls the PreRender Callback.
            /// @details This calls the PreRender Callback and returns true if the callback
            /// intends to continue with the mainloop, and false if the callback intend to
            /// end the mainloop.
            /// @return A bool that represents whether the mainloop should end.
            virtual bool PreRender();

            /// @brief Drops the PreRender pointer.
            /// @details Drops the PreRender pointer, this does not 'delete' the pointer, merely
            /// assigns it to zero.
            virtual void ErasePreRender();

            /// @brief This assigns a function to be the callback function for PreRender
            /// @details This assigns a function to be the callback function for PreRender.
            /// @param Callback This is a pointer to a function that returns a bool and accepts no arguments
            virtual void SetPreRender(bool (*Callback)());

            /// @brief Is the PreRender callback set or not.
            /// @details Is the PreRender callback set or not. The returns true if set or false otherwise.
            /// @return This returns true if the PreRender CallBack is set, false otherwise
            virtual bool IsPreRenderCallbackSet() const;


        ///////////////////////////////////////////////////////////////////////////////
        // PostRender Management
        ///////////////////////////////////////
            /// @brief This calls the PostRender Callback.
            /// @details This calls the PostRender Callback and returns true if the callback
            /// intends to continue with the mainloop, and false if the callback intend to
            /// end the mainloop.
            /// @return A bool that represents whether the mainloop should end.
            virtual bool PostRender();

            /// @brief Drops the PostRender pointer.
            /// @details Drops the PostRender pointer, this does not 'delete' the pointer, merely
            /// assigns it to zero.
            virtual void ErasePostRender();

            /// @brief This assigns a function to be the callback function for PostRender
            /// @details This assigns a function to be the callback function for PostRender.
            /// @param Callback This is a pointer to a function that returns a bool and accepts no arguments
            virtual void SetPostRender(bool (*Callback)());

            /// @brief Is the PostRender callback set or not.
            /// @details Is the PostRender callback set or not. The returns true if set or false otherwise.
            /// @return This returns true if the PostRender CallBack is set, false otherwise
            virtual bool IsPostRenderCallbackSet() const;

            //Once upon a time this was required, it will be required the exact instant someone remove too. So we settle for and uneasy peace, and leave it, but ineffectual,
            //friend class World;
    };
}

#endif
