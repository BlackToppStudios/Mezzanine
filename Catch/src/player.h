#ifndef _player_h
#define _player_h

#include <mezzanine.h>

#include "playercontrol.h"
#include "profilemanager.h"

using namespace Mezzanine;

class Player
{
protected:
    /// @brief The controlling elements of how the player moves through the game world.
    PlayerControl Control;
    /// @brief A pointer to the profile storing player specific game data.
    GameProfile* Profile;
    /// @brief A pointer to the manager housing utilities related to profile management.
    ProfileManager* ProMan;
public:
    /// @brief Class constructor.
    /// @param ProMan A pointer to the Profile Manager for the game.
    Player(ProfileManager* Manager);
    /// @brief Class destructor.
    ~Player();

    /// @brief Sets the profile for this player, establishing an identity for the playe.r
    /// @param PlayerProfile A pointer to the profile to be set.
    void SetIdentity(GameProfile* PlayerProfile);
    /// @brief Creates all the necessary objects to represent the player in the game world.
    /// @param GameWorld A pointer to the world in which to create the player objects.
    void InitWorldObjects(World* GameWorld);

    /// @brief Gets the controller for the player objects.
    /// @return Returns a reference to the PlayerControl that facilitates player movement in the game world.
    PlayerControl& GetControl();
    /// @brief Gets the profile for this player.
    /// @return Returns a pointer to the game profile for this player.
    GameProfile* GetProfile() const;
    /// @brief Gets the name of this player.
    /// @return Returns a const string reference to the name of this player.
    const String& GetName() const;

    /// @brief Advances the state for the player.
    /// @param InputMan A pointer to the input manager for querying inputs.
    /// @param DeltaTime The amount of time that has elapsed since the last update.
    void Update(Input::InputManager* InputMan, const Whole DeltaTime);
};//Player

#endif
