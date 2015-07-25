// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalsefxinterface_cpp
#define _audiooalsefxinterface_cpp

#include "Audio/OALS/oalsefxinterface.h.cpp"

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            VoidFunct* FetchFunctionPointer(const String& FunctName)
            {
                /// @todo This suppresses the warnings for casting between object and function pointers on GCC, but other compilers may need their own tricks.
                #ifdef __GNUC__
                // Do to the expected semantics of using this preprocessor, we can't return the cast directly.
                __extension__
                #endif
                VoidFunct* Ret = reinterpret_cast<VoidFunct*>(alGetProcAddress(FunctName.c_str()));
                return Ret;
            }//*/

            ///////////////////////////////////////////////////////////////////////////////
            // EFXInterface Methods

            EFXInterface::EFXInterface(ALCdevice* Device)
                : Supported(false)
            {
                if(alcIsExtensionPresent(Device,"ALC_EXT_EFX"))
                {
                    // Get function pointers
                    alGenEffects = (LPALGENEFFECTS)FetchFunctionPointer("alGenEffects");
                    alDeleteEffects = (LPALDELETEEFFECTS)FetchFunctionPointer("alDeleteEffects");
                    alIsEffect = (LPALISEFFECT)FetchFunctionPointer("alIsEffect");
                    alEffecti = (LPALEFFECTI)FetchFunctionPointer("alEffecti");
                    alEffectiv = (LPALEFFECTIV)FetchFunctionPointer("alEffectiv");
                    alEffectf = (LPALEFFECTF)FetchFunctionPointer("alEffectf");
                    alEffectfv = (LPALEFFECTFV)FetchFunctionPointer("alEffectfv");
                    alGetEffecti = (LPALGETEFFECTI)FetchFunctionPointer("alGetEffecti");
                    alGetEffectiv = (LPALGETEFFECTIV)FetchFunctionPointer("alGetEffectiv");
                    alGetEffectf = (LPALGETEFFECTF)FetchFunctionPointer("alGetEffectf");
                    alGetEffectfv = (LPALGETEFFECTFV)FetchFunctionPointer("alGetEffectfv");
                    alGenFilters = (LPALGENFILTERS)FetchFunctionPointer("alGenFilters");
                    alDeleteFilters = (LPALDELETEFILTERS)FetchFunctionPointer("alDeleteFilters");
                    alIsFilter = (LPALISFILTER)FetchFunctionPointer("alIsFilter");
                    alFilteri = (LPALFILTERI)FetchFunctionPointer("alFilteri");
                    alFilteriv = (LPALFILTERIV)FetchFunctionPointer("alFilteriv");
                    alFilterf = (LPALFILTERF)FetchFunctionPointer("alFilterf");
                    alFilterfv = (LPALFILTERFV)FetchFunctionPointer("alFilterfv");
                    alGetFilteri = (LPALGETFILTERI)FetchFunctionPointer("alGetFilteri");
                    alGetFilteriv= (LPALGETFILTERIV)FetchFunctionPointer("alGetFilteriv");
                    alGetFilterf = (LPALGETFILTERF)FetchFunctionPointer("alGetFilterf");
                    alGetFilterfv= (LPALGETFILTERFV)FetchFunctionPointer("alGetFilterfv");
                    alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)FetchFunctionPointer("alGenAuxiliaryEffectSlots");
                    alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)FetchFunctionPointer("alDeleteAuxiliaryEffectSlots");
                    alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)FetchFunctionPointer("alIsAuxiliaryEffectSlot");
                    alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)FetchFunctionPointer("alAuxiliaryEffectSloti");
                    alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)FetchFunctionPointer("alAuxiliaryEffectSlotiv");
                    alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)FetchFunctionPointer("alAuxiliaryEffectSlotf");
                    alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)FetchFunctionPointer("alAuxiliaryEffectSlotfv");
                    alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)FetchFunctionPointer("alGetAuxiliaryEffectSloti");
                    alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)FetchFunctionPointer("alGetAuxiliaryEffectSlotiv");
                    alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)FetchFunctionPointer("alGetAuxiliaryEffectSlotf");
                    alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)FetchFunctionPointer("alGetAuxiliaryEffectSlotfv");

                    if( alGenEffects &&	alDeleteEffects && alIsEffect && alEffecti && alEffectiv &&	alEffectf &&
                        alEffectfv && alGetEffecti && alGetEffectiv && alGetEffectf && alGetEffectfv &&	alGenFilters &&
                        alDeleteFilters && alIsFilter && alFilteri && alFilteriv &&	alFilterf && alFilterfv &&
                        alGetFilteri &&	alGetFilteriv && alGetFilterf && alGetFilterfv && alGenAuxiliaryEffectSlots &&
                        alDeleteAuxiliaryEffectSlots &&	alIsAuxiliaryEffectSlot && alAuxiliaryEffectSloti &&
                        alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
                        alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf &&
                        alGetAuxiliaryEffectSlotfv )
                        Supported = true;
                }
            }

            EFXInterface::~EFXInterface()
                {  }
        }//OALS
    }//Audio
}//Mezzanine

#endif
