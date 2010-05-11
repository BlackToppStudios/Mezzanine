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
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef WORLDCALLBACKMANAGER_H
#define WORLDCALLBACKMANAGER_H

class PhysWorld;


namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class WorldCallBackManager
    /// @headerfile worldcallbackmanager.h
    /// @brief This Stores callbacks for for use in the main loop
    /// @details This stores a series of pointers to functions that the main loop
    /// will call. This can be swapped out at any point in time
    class WorldCallBackManager
    {
        private:
            //pointers to internal structures
            PhysWorld* TheWorldIListenTo;

            //pointers to callback functions
            bool (*PreInputCallback)();
            bool (*PostInputCallback)();

            bool (*PrePhysicsCallback)();
            bool (*PostPhysicsCallback)();

            bool (*PreRenderCallback)();
            bool (*PostRenderCallback)();

        public:

            WorldCallBackManager(PhysWorld* _Parent);
            ~WorldCallBackManager();

            //Called when a frame is about to begin rendering. so is the
            //Function you passe in with the setFrameStartCallback
            bool PreInput();
            void ErasePreInput();
            void SetPreInput(bool (*Callback)());
            bool IsPreInputCallbackSet();

            bool PostInput();
            void ErasePostInput();
            void SetPostInput(bool (*Callback)());
            bool IsPostInputCallbackSet();

            bool PrePhysics();
            void ErasePrePhysics();
            void SetPrePhysics(bool (*Callback)());
            bool IsPrePhysicsCallbackSet();

            bool PostPhysics();
            void ErasePostPhysics();
            void SetPostPhysics(bool (*Callback)());
            bool IsPostPhysicsCallbackSet();

            bool PreRender();
            void ErasePreRender();
            void SetPreRender(bool (*Callback)());
            bool IsPreRenderCallbackSet();

            bool PostRender();
            void ErasePostRender();
            void SetPostRender(bool (*Callback)());
            bool IsPostRenderCallbackSet();

            friend class PhysWorld;
    };
}

#endif
