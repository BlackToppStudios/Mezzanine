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
#ifndef _physicsenumerations_h
#define _physicsenumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the Physics namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace Physics
    {
        /// @enum CollisionFilter
        /// @brief These are the various filters that can be applied to all physics accelerated world objects to limit what they collide with.
        /// @details You can create your own groups on top of the ones that exist here, but they have to be powers of 2, and not overlap with
        /// any of the existing groups defined here.  Simply pass in the Whole where appropriate.
        enum CollisionFilter
        {
            // Standard base filters
            CF_GenericFilter = 1,
            CF_StaticFilter = 2,
            CF_KinematicFilter = 4,
            CF_DebrisFilter = 8,
            CF_SensorFilter = 16,
            CF_CharacterFilter = 32,
            CF_AllFilter = -1,

            // Non-Standard Filters
            CF_UserFilter1 = 64,
            CF_UserFilter2 = 128
        };

        /// @enum WorldObjectActivationState
        /// @brief This is used by all the world objects to monitor and set their physics activation.
        enum WorldObjectActivationState
        {
            WOAS_Active = 1,
            WOAS_Island_Sleeping = 2,
            WOAS_Wants_Deactivation = 3,
            WOAS_DisableDeactivation = 4,
            WOAS_DisableSimulation = 5
        };
    }//physics
}//Mezzanine

#endif
