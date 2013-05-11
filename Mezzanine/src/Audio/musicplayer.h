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
        class Sound;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a convenience class for the playing of music in a game.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MusicPlayer
        {
        protected:
            Audio::Playlist* MusicPlaylist;
            Audio::Sound* CurrSong;
            bool ManualStop;
            bool Playing;
            bool EOPRepeat;
            bool EOPShuffle;
            std::list<Audio::Sound*>::iterator GetIteratorToSong(Sound* Song);
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

            /// @brief Sets the specified song as the current song.
            /// @throw If the provided song isn't in the playlist, this will throw an @ref InstanceIdentityNotFoundException . Use the ContainsSong() function to verify before using this.
            /// @param Song The song to set.
            void SwitchToSong(Sound* Song);

            /// @brief Sets the specified song as the current song.
            /// @throw If the provided song isn't in the playlist, this will throw an @ref InstanceIdentityNotFoundException . Use the ContainsSong() function to verify before using this.
            /// @param SongName The name of the song to select.
            void SwitchToSong(const String& SongName);

            ///////////////////////////////////////////////////////////////////////////////
            // Checks

            /// @brief Gets whether or not the current selection is playing.
            /// @return Returns true if the current song is playing, false otherwise.
            bool IsPlaying() const;
            /// @brief Gets whether or not the current selection is stopped.
            /// @return Returns true if the current song is stopped, false otherwise.
            bool IsStopped() const;
            /// @brief Gets whether or not the current selection is paused.
            /// @return Returns true if the current song is paused, false otherwise.
            bool IsPaused() const;
            /// @brief Checks the set playlist to see if it contains a song.
            /// @param Song The song to check for.
            bool ContainsSong(Sound* Song) const;
            /// @brief Checks the set playlist to see if it contains a song.
            /// @param SongName The name of the song to check for.
            bool ContainsSong(const String& SongName) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Configuration

            /// @brief Sets whether the playlist should return to the start after it reaches the end of the list.
            /// @param Repeat Enables/Disables repeating the playlist when it reaches the end.
            void SetEOPRepeat(bool Repeat);
            /// @brief Gets wether playlist repeat is enabled.
            /// @return Returns true if the playlist is set to repeat when it finishes, false otherwise.
            bool GetEOPRepeat() const;
            /// @brief Sets whether the playlist should shuffle it's contents after it reaches the end of the list.
            /// @param Shuffle Enables/Disables shuffling the playlist when it reaches the end.
            void SetEOPShuffle(bool Shuffle);
            /// @brief Gets wether playlist shuffle is enabled.
            /// @return Returns true if the playlist is set to shuffle when it finishes, false otherwise.
            bool GetEOPShuffle() const;

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
