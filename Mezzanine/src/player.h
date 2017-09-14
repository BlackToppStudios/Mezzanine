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

#ifndef _player_h
#define _player_h

#include "playercontrol.h"
#include "playerview.h"

namespace Mezzanine
{
    class PlayerManager;
    class PlayerProfile;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A class used to represent the user in the session.
    ///////////////////////////////////////
    class Player
    {
    protected:
        /// @brief The controlling elements of how the player moves through the world.
        PlayerControl Control;
        /// @brief The controlling elements of how the camera moves in relation to the player in the world.
        PlayerView* View;
        /// @brief A pointer to the profile storing player specific game data.
        PlayerProfile* Profile;
        /// @brief A pointer to the manager housing utilities related to profile management.
        PlayerManager* PlayMan;
    public:
        /// @brief Class constructor.
        /// @param Creator A pointer to the PlayerManager.
        Player(PlayerManager* Creator);
        /// @brief Class destructor.
        virtual ~Player();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the profile for this player, establishing an identity for the player.
        /// @param ToSet A pointer to the profile to be set.
        void SetIdentity(PlayerProfile* ToSet);
        /// @brief Creates all the necessary objects to represent the player in the game world.
        /// @param GameWorld A pointer to the world in which to create the player objects.
        void InitializeInWorld(World* GameWorld);

        /// @brief Gets the controller for the player objects.
        /// @return Returns a reference to the PlayerControl that facilitates player object movement in the world.
        PlayerControl& GetControl();
        /// @brief Gets the controller for the player camera.
        /// @return Returns a reference to the PlayerView that facilitates player camera movement in the world.
        PlayerView* GetView() const;
        /// @brief Gets the profile for this player.
        /// @return Returns a pointer to the game profile for this player.
        PlayerProfile* GetProfile() const;
        /// @brief Gets the name of this player.
        /// @return Returns a const string reference to the name of this player.
        const String& GetName() const;

        /// @brief Gets whether or not this player is giving inputs directly to the local host.
        /// @return Returns true if the player is local, false if the player is remote.
        virtual Boole IsLocal() const;
        /// @brief Gets whether or not this player is connected to this host over a network.
        /// @return Returns true if the player is remote, false if the player is local.
        virtual Boole IsRemote() const;

        /// @brief Advances the state for the player.
        /// @param InputMan A pointer to the input manager for querying inputs.
        /// @param DeltaTime The amount of time that has elapsed since the last update.
        void Update(Input::InputManager* InputMan, const Whole DeltaTime);
    };//Player
}//Mezzanine

#endif
