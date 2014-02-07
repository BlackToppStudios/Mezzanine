// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _audiomusicplayer_h
#define _audiomusicplayer_h

#include "datatypes.h"

/// @file
/// @brief The interface for the Musicplayer class

namespace Mezzanine
{
    namespace Audio
    {
        class Playlist;
        class iSound;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a convenience class for the playing of music in a game.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MusicPlayer
        {
        public:
            /// @brief Convenience type for track iterators from playlists.
            typedef std::list<Audio::iSound*>::iterator TrackIterator;
        protected:
            /// @internal
            /// @brief A pointer to the active playlist used by this player.
            Audio::Playlist* MusicPlaylist;
            /// @internal
            /// @brief A pointer to the currently selected track.
            Audio::iSound* CurrTrack;
            /// @internal
            /// @brief Stores whether or not the current track has been manually stopped.
            Boolean ManualStop;
            /// @internal
            /// @brief Stores whether or not this player is currently playing.
            Boolean Playing;
            /// @internal
            /// @brief Stores whether or not the player will loop back to the start when it finishes playing all tracks in the playlist.
            Boolean EOPRepeat;
            /// @internal
            /// @brief Stores whether or not the player will shuffle the playlist when it finishes playing all the tracks in the playlist.
            Boolean EOPShuffle;

            /// @internal
            /// @brief Gets an iterator to the iSound instance in the current playlist.
            /// @param Track The iSound instance to get an iterator to.
            /// @return Returns a TrackIterator to the specified iSound in the currenlt playlist.
            TrackIterator GetIteratorToTrack(iSound* Track);
        public:
            /// @brief Class constructor.
            MusicPlayer();
            /// @brief Class destructor.
            ~MusicPlayer();

            ///////////////////////////////////////////////////////////////////////////////
            // Playback and Selection

            /// @brief Plays the current selection.
            /// @throw If the current playlist is empty this throws a @ref InvalidStateException
            void Play();
            /// @brief Stops the current selection.
            void Stop();
            /// @brief Pauses the current selection.
            void Pause();
            /// @brief Advances to the next selection on the playlist.
            void Next();
            /// @brief Moves back to the previous selection on the playlist.
            void Previous();

            /// @brief Sets the specified track as the current track.
            /// @throw If the provided track isn't in the playlist, this will throw an @ref InstanceIdentityNotFoundException . Use the ContainsSong() function to verify before using this.
            /// @param Track The track to set.
            void SwitchToTrack(iSound* Track);

            ///////////////////////////////////////////////////////////////////////////////
            // Checks

            /// @brief Gets whether or not the current selection is playing.
            /// @return Returns true if the current track is playing, false otherwise.
            Boolean IsPlaying() const;
            /// @brief Gets whether or not the current selection is stopped.
            /// @return Returns true if the current track is stopped, false otherwise.
            Boolean IsStopped() const;
            /// @brief Gets whether or not the current selection is paused.
            /// @return Returns true if the current track is paused, false otherwise.
            Boolean IsPaused() const;
            /// @brief Checks the set playlist to see if it contains a track.
            /// @param Track The track to check for.
            /// @return Returns true if the current playlist contains the specified song, false otherwise.
            Boolean ContainsSong(iSound* Track) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets whether the playlist should return to the start after it reaches the end of the list.
            /// @param Repeat Enables/Disables repeating the playlist when it reaches the end.
            void SetPlaylistRepeat(Boolean Repeat);
            /// @brief Gets wether playlist repeat is enabled.
            /// @return Returns true if the playlist is set to repeat when it finishes, false otherwise.
            Boolean GetPlaylistRepeat() const;
            /// @brief Sets whether the playlist should shuffle it's contents after it reaches the end of the list.
            /// @param Shuffle Enables/Disables shuffling the playlist when it reaches the end.
            void SetPlaylistShuffle(Boolean Shuffle);
            /// @brief Gets wether playlist shuffle is enabled.
            /// @return Returns true if the playlist is set to shuffle when it finishes, false otherwise.
            Boolean GetPlaylistShuffle() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the playlist in use by this music player.
            /// @return Returns a pointer to the current playlist in use.
            Playlist* GetPlaylist() const;
            /// @brief Called on by the AudioManager to perform all music player responsibilities.
            void Update();
        };//MusicPlayer
    }//Audio
}//Mezzanine

#endif
