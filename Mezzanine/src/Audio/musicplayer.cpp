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
#ifndef _audiomusicplayer_cpp
#define _audiomusicplayer_cpp

#include "Audio/musicplayer.h"
#include "Audio/playlist.h"
#include "Audio/sound.h"

#include "exception.h"

namespace Mezzanine
{
    namespace Audio
    {
        MusicPlayer::MusicPlayer()
            : MusicPlaylist(NULL),
              CurrSong(NULL),
              ManualStop(false),
              Playing(false),
              EOPRepeat(false),
              EOPShuffle(false)
        {
            MusicPlaylist = new Playlist();
        }

        MusicPlayer::~MusicPlayer()
        {
            delete MusicPlaylist;
        }

        std::list<Audio::iSound*>::iterator MusicPlayer::GetIteratorToSong(iSound* Song)
        {
            for( std::list< Audio::iSound* >::iterator it = MusicPlaylist->begin() ; it != MusicPlaylist->end() ; ++it )
            {
                if(Song == (*it))
                {
                    CurrSong = (*it);
                    return it;
                }
            }
            return MusicPlaylist->end();
        }

        void MusicPlayer::Play()
        {
            ManualStop = false;
            Playing = true;
            if(!CurrSong)
            {
                if(MusicPlaylist->empty()) { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to play a song in MusicPlayer with an empty playlist"); }
                else CurrSong = *(MusicPlaylist->begin());
            }
            CurrSong->Play();
        }

        void MusicPlayer::Stop()
        {
            ManualStop = true;
            Playing = false;
            CurrSong->Stop();
        }

        void MusicPlayer::Pause()
        {
            CurrSong->Pause();
        }

        void MusicPlayer::Next()
        {
            std::list<iSound*>::iterator SongIt = this->GetIteratorToSong(CurrSong);
            SongIt++;
            CurrSong = (*SongIt);
            if(Playing && !ManualStop)
                this->Play();
        }

        void MusicPlayer::Previous()
        {
            std::list<iSound*>::iterator SongIt = GetIteratorToSong(CurrSong);
            SongIt--;
            CurrSong = (*SongIt);
            if(Playing && !ManualStop)
                this->Play();
        }

        void MusicPlayer::SwitchToSong(iSound* Song)
        {
            for( std::list< Audio::iSound* >::iterator it = MusicPlaylist->begin() ; it != MusicPlaylist->end() ; ++it )
            {
                if(Song == (*it))
                {
                    CurrSong = (*it);
                    if(Playing && !ManualStop)
                        this->Play();
                    return;
                }
            }
            MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to switch to song not contained within the current Playlist.");
        }

        bool MusicPlayer::IsPlaying() const
        {
            return CurrSong->IsPlaying();
        }

        bool MusicPlayer::IsStopped() const
        {
            return CurrSong->IsStopped();
        }

        bool MusicPlayer::IsPaused() const
        {
            return CurrSong->IsPaused();
        }

        bool MusicPlayer::ContainsSong(iSound* Song) const
        {
            return MusicPlaylist->ContainsSound(Song);
        }

        void MusicPlayer::SetPlaylistRepeat(bool Repeat)
        {
            EOPRepeat = Repeat;
        }

        bool MusicPlayer::GetPlaylistRepeat() const
        {
            return EOPRepeat;
        }

        void MusicPlayer::SetPlaylistShuffle(bool Shuffle)
        {
            EOPShuffle = Shuffle;
        }

        bool MusicPlayer::GetPlaylistShuffle() const
        {
            return EOPShuffle;
        }

        Playlist* MusicPlayer::GetPlaylist() const
        {
            return MusicPlaylist;
        }

        void MusicPlayer::Update()
        {
            if(0!=CurrSong)
            {
                std::list<Audio::iSound*>::iterator SongIt = this->GetIteratorToSong(CurrSong);
                std::list<Audio::iSound*>::iterator NextSong = SongIt;
                NextSong++;
                if(CurrSong->IsStopped() && Playing)
                {
                    if(NextSong == MusicPlaylist->end())
                    {
                        if(EOPRepeat)
                        {
                            if(EOPShuffle) MusicPlaylist->Shuffle();
                            CurrSong = *(MusicPlaylist->begin());
                            Play();
                        }else{
                            Stop();
                        }
                    }else{
                        Next();
                    }
                }
            }
        }
    }//Audio
}//Mezzanine

#endif
