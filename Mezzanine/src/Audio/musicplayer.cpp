//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#include "world.h"

namespace Mezzanine
{
    namespace Audio
    {
        MusicPlayer::MusicPlayer()
            : ManualStop(false),
              Playing(false),
              EOPRepeat(false),
              EOPShuffle(false),
              MusicPlaylist(NULL),
              CurrSong(NULL)
        {
            MusicPlaylist = new Playlist();
        }

        MusicPlayer::~MusicPlayer()
        {
            delete MusicPlaylist;
        }

        std::list<Audio::Sound*>::iterator MusicPlayer::GetIteratorToSong(Sound* Song)
        {
            for( std::list< Audio::Sound* >::iterator it = MusicPlaylist->begin() ; it != MusicPlaylist->end() ; ++it )
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
                if(MusicPlaylist->empty()) World::GetWorldPointer()->LogAndThrow(Exception("Attempting to play a song in MusicPlayer with an empty playlist."));
                else CurrSong = *(MusicPlaylist->begin());
            }
            CurrSong->Play2d();
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
            std::list<Sound*>::iterator SongIt = GetIteratorToSong(CurrSong);
            SongIt++;
            CurrSong = (*SongIt);
            if(Playing && !ManualStop)
                this->Play();
        }

        void MusicPlayer::Previous()
        {
            std::list<Sound*>::iterator SongIt = GetIteratorToSong(CurrSong);
            SongIt--;
            CurrSong = (*SongIt);
            if(Playing && !ManualStop)
                this->Play();
        }

        void MusicPlayer::SwitchToSong(Sound* Song)
        {
            for( std::list< Audio::Sound* >::iterator it = MusicPlaylist->begin() ; it != MusicPlaylist->end() ; ++it )
            {
                if(Song == (*it))
                {
                    CurrSong = (*it);
                    if(Playing && !ManualStop)
                        this->Play();
                    return;
                }
            }
            World::GetWorldPointer()->LogAndThrow(Exception("Attempting to switch to song not contained within the current Playlist."));
        }

        void MusicPlayer::SwitchToSong(const String& SongName)
        {
            for( std::list< Audio::Sound* >::iterator it = MusicPlaylist->begin() ; it != MusicPlaylist->end() ; ++it )
            {
                if(SongName == (*it)->GetName())
                {
                    CurrSong = (*it);
                    if(Playing && !ManualStop)
                        this->Play();
                    return;
                }
            }
            World::GetWorldPointer()->LogAndThrow(Exception("Attempting to switch to song not contained within the current Playlist."));
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

        bool MusicPlayer::ContainsSong(Sound* Song) const
        {
            return MusicPlaylist->ContainsSound(Song);
        }

        bool MusicPlayer::ContainsSong(const String& SongName) const
        {
            return MusicPlaylist->ContainsSound(SongName);
        }

        void MusicPlayer::SetEOPRepeat(bool Repeat)
        {
            EOPRepeat = Repeat;
        }

        bool MusicPlayer::GetEOPRepeat() const
        {
            return EOPRepeat;
        }

        void MusicPlayer::SetEOPShuffle(bool Shuffle)
        {
            EOPShuffle = Shuffle;
        }

        bool MusicPlayer::GetEOPShuffle() const
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
                std::list<Audio::Sound*>::iterator SongIt = GetIteratorToSong(CurrSong);
                std::list<Audio::Sound*>::iterator NextSong = SongIt;
                NextSong++;
                if(CurrSong->IsStopped() && Playing)
                {
                    if(NextSong == MusicPlaylist->end())
                    {
                        if(EOPRepeat)
                        {
                            if(EOPShuffle) MusicPlaylist->ShuffleList();
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
