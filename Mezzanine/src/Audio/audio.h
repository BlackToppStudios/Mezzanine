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
#ifndef _audio_h
#define _audio_h

namespace Mezzanine
{
    /// @namespace Mezzanine::Audio
    /// @brief This namespace is for all the classes belonging to the Audio Subsystem.
    /// @details Our Audio subsystem is based on a heavily modified/forked version of cAudio, and as such uses
    /// a similiar structure of classes and interfaces.
    namespace Audio
    {

    }
}

#include "Audio/audioenumerations.h"
#include "Audio/audiomanager.h"

#include "Audio/decoder.h"
#include "Audio/decoderfactory.h"
#include "Audio/effect.h"
#include "Audio/effectparameters.h"
#include "Audio/effectshandler.h"
#include "Audio/filter.h"
#include "Audio/listener.h"
#include "Audio/musicplayer.h"
#include "Audio/playlist.h"
#include "Audio/rawdecoder.h"
#include "Audio/rawdecoderfactory.h"
#include "Audio/recorder.h"
#include "Audio/sound.h"
#include "Audio/soundproxy.h"
#include "Audio/soundscapemanager.h"
#include "Audio/soundset.h"
#include "Audio/vorbisdecoder.h"
#include "Audio/vorbisdecoderfactory.h"
#include "Audio/wavdecoder.h"
#include "Audio/wavdecoderfactory.h"

#endif
