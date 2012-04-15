//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _audioplaylist_cpp
#define _audioplsylist_cpp

#include "audioplaylist.h"
#include "audiosound.h"
#include <vector>
#include <algorithm>

namespace Mezzanine
{
    namespace Audio
    {
        Playlist::Playlist()
        {
        }

        Playlist::~Playlist()
        {
        }

        void Playlist::AddSound(Sound* ToAdd)
        {
            this->push_back(ToAdd);
        }

        void Playlist::ShuffleList()
        {
            std::vector< Audio::Sound* > temp( this->begin(), this->end() );
            std::random_shuffle( temp.begin(), temp.end() );
            this->assign( temp.begin(), temp.end() );
        }

        bool Playlist::ContainsSound(Sound* TheSound)
        {
            for( std::list< Audio::Sound* >::iterator it = this->begin() ; it != this->end() ; ++it )
            {
                if(TheSound == (*it))
                    return true;
            }
            return false;
        }

        bool Playlist::ContainsSound(const String& SoundName)
        {
            for( std::list< Audio::Sound* >::iterator it = this->begin() ; it != this->end() ; ++it )
            {
                if(SoundName == (*it)->GetName())
                    return true;
            }
            return false;
        }
    }//Audio
}//Mezzanine

#endif
