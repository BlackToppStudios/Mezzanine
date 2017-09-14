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
        MusicPlayer::MusicPlayer() :
            MusicPlaylist(NULL),
            CurrTrack(NULL),
            ManualStop(false),
            Playing(false),
            EOPRepeat(false),
            EOPShuffle(false)
            { this->MusicPlaylist = new Playlist(); }

        MusicPlayer::~MusicPlayer()
            { delete this->MusicPlaylist; }

        MusicPlayer::TrackIterator MusicPlayer::GetIteratorToTrack(iSound* Track)
        {
            for( TrackIterator TrackIt = this->MusicPlaylist->begin() ; TrackIt != this->MusicPlaylist->end() ; ++TrackIt )
            {
                if(Track == (*TrackIt)) {
                    this->CurrTrack = (*TrackIt);
                    return TrackIt;
                }
            }
            return this->MusicPlaylist->end();
        }

        void MusicPlayer::Play()
        {
            this->ManualStop = false;
            this->Playing = true;
            if( !this->CurrTrack ) {
                if( this->MusicPlaylist->empty() ) {
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to play a song in MusicPlayer with an empty playlist");
                }else{
                    this->CurrTrack = *(this->MusicPlaylist->begin());
                }
            }
            this->CurrTrack->Play();
        }

        void MusicPlayer::Stop()
        {
            this->ManualStop = true;
            this->Playing = false;
            this->CurrTrack->Stop();
        }

        void MusicPlayer::Pause()
        {
            this->CurrTrack->Pause();
        }

        void MusicPlayer::Next()
        {
            TrackIterator TrackIt = this->GetIteratorToTrack(this->CurrTrack);
            TrackIt++;
            this->CurrTrack = (*TrackIt);
            if( this->Playing && !this->ManualStop )
                this->Play();
        }

        void MusicPlayer::Previous()
        {
            TrackIterator TrackIt = this->GetIteratorToTrack(this->CurrTrack);
            TrackIt--;
            this->CurrTrack = (*TrackIt);
            if( this->Playing && !this->ManualStop )
                this->Play();
        }

        void MusicPlayer::SwitchToTrack(iSound* Track)
        {
            for( TrackIterator it = this->MusicPlaylist->begin() ; it != this->MusicPlaylist->end() ; ++it )
            {
                if(Track == (*it)) {
                    this->CurrTrack = (*it);
                    if( this->Playing && !this->ManualStop )
                        this->Play();
                    return;
                }
            }
            MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to switch to song not contained within the current Playlist.");
        }

        Boole MusicPlayer::IsPlaying() const
            { return this->CurrTrack->IsPlaying(); }

        Boole MusicPlayer::IsStopped() const
            { return this->CurrTrack->IsStopped(); }

        Boole MusicPlayer::IsPaused() const
            { return this->CurrTrack->IsPaused(); }

        Boole MusicPlayer::ContainsSong(iSound* Track) const
            { return this->MusicPlaylist->ContainsSound(Track); }

        void MusicPlayer::SetPlaylistRepeat(Boole Repeat)
            { this->EOPRepeat = Repeat; }

        Boole MusicPlayer::GetPlaylistRepeat() const
            { return this->EOPRepeat; }

        void MusicPlayer::SetPlaylistShuffle(Boole Shuffle)
            { this->EOPShuffle = Shuffle; }

        Boole MusicPlayer::GetPlaylistShuffle() const
            { return this->EOPShuffle; }

        Playlist* MusicPlayer::GetPlaylist() const
            { return this->MusicPlaylist; }

        void MusicPlayer::Update()
        {
            if( 0 != this->CurrTrack ) {
                TrackIterator TrackIt = this->GetIteratorToTrack(this->CurrTrack);
                TrackIterator NextTrack = TrackIt;
                NextTrack++;
                if( this->CurrTrack->IsStopped() && this->Playing) {
                    if(NextTrack == this->MusicPlaylist->end()) {
                        if( this->EOPRepeat ) {
                            if( this->EOPShuffle ) {
                                this->MusicPlaylist->Shuffle();
                            }
                            this->CurrTrack = *(this->MusicPlaylist->begin());
                            this->Play();
                        }else{
                            this->Stop();
                        }
                    }else{
                        this->Next();
                    }
                }
            }
        }
    }//Audio
}//Mezzanine

#endif
