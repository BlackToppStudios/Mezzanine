//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _timermanager_h
#define _timermanager_h

#include "managerbase.h"
#include "datatypes.h"
#include "timer.h"

namespace Ogre
{
    class Timer;
}

namespace phys
{
    class ExtendedTimer;
    class SimpleTimer;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class TimerManager
    /// @headerfile timermanager.h
    /// @brief A manager responsible for the updating of all timers in use by this engine.
    /// @details
    ///////////////////////////////////////
    class TimerManager : public ManagerBase
    {
        protected:
            std::vector<Timer*> Timers;
            Ogre::Timer* MasterTimer;
        public:
            /// @brief Standard Constructor.
            TimerManager();
            /// @brief Class Destructor.
            virtual ~TimerManager();
            /// @brief Creates a simple timer within this manager.
            /// @return Returns a pointer to the created timer.
            /// @param Style The style of timer you wish to create.
            virtual SimpleTimer* CreateSimpleTimer(const Timer::TimerStyle Style);
            /// @brief Creates an extended timer within this manager.
            /// @return Returns a pointer to the created timer.
            /// @param Style The style of timer you wish to create.
            virtual ExtendedTimer* CreateExtendedTimer(const Timer::TimerStyle Style);
            /// @brief Gets a Timer by index.
            /// @return Returns a pointer to the Timer at the specified index.
            virtual Timer* GetTimer(const Whole Index);
            /// @brief Gets the number of Timers created and stored in this manager.
            /// @return Returns the number of Timers this manager is storing.
            virtual Whole GetNumTimers();
            /// @brief Destroys a timer within this manager.
            /// @param ToBeDestroyed A pointer to the timer class to be destroyed.
            virtual void DestroyTimer(Timer* ToBeDestroyed);

            //Inherited From ManagerBase
            /// @brief Empty Initializor.
            /// @details This specific initializor is unneeded, but we implement it for compatibility. It also exists
            /// in case a derived class wants to override it for some reason.
            virtual void Initialize();

            /// @brief Items to be done during the mainloop.
            /// @details Updates all the timers.
            virtual void DoMainLoopItems();

            /// @brief This returns the type of this manager.
            /// @return This returns ManagerTypeName::TimerManager.
            virtual ManagerTypeName GetType() const;
    };
}

#endif
