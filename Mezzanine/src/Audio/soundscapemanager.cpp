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
#ifndef _audiosoundscapemanager_cpp
#define _audiosoundscapemanager_cpp

#include "Audio/soundscapemanager.h"

namespace Mezzanine
{
    namespace Audio
    {
        SoundScapeManager::SoundScapeManager(World* Creator) :
            WorldProxyManager(Creator)
            {  }

        SoundScapeManager::SoundScapeManager(World* Creator, XML::Node& XMLNode) :
            WorldProxyManager(Creator)
            {  }

        SoundScapeManager::~SoundScapeManager()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        ///////////////////////////////////////////////////////////////////////////////
        // Listener Management

        ///////////////////////////////////////////////////////////////////////////////
        // Proxy Management

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Creation Methods

        SoundProxy* SoundScapeManager::CreateAmbientSoundProxy()
            { return this->CreateSoundProxy(Audio::ST_Ambient); }
        SoundProxy* SoundScapeManager::CreateDialogSoundProxy()
            { return this->CreateSoundProxy(Audio::ST_Dialog); }
        SoundProxy* SoundScapeManager::CreateEffectSoundProxy()
            { return this->CreateSoundProxy(Audio::ST_Effect); }
        SoundProxy* SoundScapeManager::CreateMusicSoundProxy()
            { return this->CreateSoundProxy(Audio::ST_Music); }

        SoundProxy* SoundScapeManager::CreateAmbientSoundProxy(DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Ambient,Stream,Encode); }
        SoundProxy* SoundScapeManager::CreateDialogSoundProxy(DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Dialog,Stream,Encode); }
        SoundProxy* SoundScapeManager::CreateEffectSoundProxy(DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Effect,Stream,Encode); }
        SoundProxy* SoundScapeManager::CreateMusicSoundProxy(DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Music,Stream,Encode); }

        SoundProxy* SoundScapeManager::CreateAmbientSoundProxy(DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Ambient,Stream,Frequency,Config); }
        SoundProxy* SoundScapeManager::CreateDialogSoundProxy(DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Dialog,Stream,Frequency,Config); }
        SoundProxy* SoundScapeManager::CreateEffectSoundProxy(DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Effect,Stream,Frequency,Config); }
        SoundProxy* SoundScapeManager::CreateMusicSoundProxy(DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Music,Stream,Frequency,Config); }

        SoundProxy* SoundScapeManager::CreateAmbientSoundProxy(const String& FileName, const String& Group)
            { return this->CreateSoundProxy(Audio::ST_Ambient,FileName,Group); }
        SoundProxy* SoundScapeManager::CreateDialogSoundProxy(const String& FileName, const String& Group)
            { return this->CreateSoundProxy(Audio::ST_Dialog,FileName,Group); }
        SoundProxy* SoundScapeManager::CreateEffectSoundProxy(const String& FileName, const String& Group)
            { return this->CreateSoundProxy(Audio::ST_Effect,FileName,Group); }
        SoundProxy* SoundScapeManager::CreateMusicSoundProxy(const String& FileName, const String& Group)
            { return this->CreateSoundProxy(Audio::ST_Music,FileName,Group); }

        SoundProxy* SoundScapeManager::CreateAmbientSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Ambient,StreamName,Buffer,Length,Encode); }
        SoundProxy* SoundScapeManager::CreateDialogSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Dialog,StreamName,Buffer,Length,Encode); }
        SoundProxy* SoundScapeManager::CreateEffectSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Effect,StreamName,Buffer,Length,Encode); }
        SoundProxy* SoundScapeManager::CreateMusicSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSoundProxy(Audio::ST_Music,StreamName,Buffer,Length,Encode); }

        SoundProxy* SoundScapeManager::CreateAmbientSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Ambient,StreamName,Buffer,Length,Frequency,Config); }
        SoundProxy* SoundScapeManager::CreateDialogSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Dialog,StreamName,Buffer,Length,Frequency,Config); }
        SoundProxy* SoundScapeManager::CreateEffectSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Effect,StreamName,Buffer,Length,Frequency,Config); }
        SoundProxy* SoundScapeManager::CreateMusicSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSoundProxy(Audio::ST_Music,StreamName,Buffer,Length,Frequency,Config); }

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited from Managerbase

        ManagerBase::ManagerType SoundScapeManager::GetInterfaceType() const
            { return ManagerBase::MT_SoundScapeManager; }
    }//Audio
}//Mezzanine

#endif
