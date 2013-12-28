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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalsefxinterface_h
#define _audiooalsefxinterface_h

#include "datatypes.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/efx-presets.h>

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @internal
            /// @brief This is a class for the binding of internal methods belonging to the EFX extension.
            /// @details
            ///////////////////////////////////////
            struct MEZZ_LIB EFXInterface
            {
                /// @internal
                /// @brief This indicates whether or not all the methods saved are valid and ready for use.
                bool Supported;

                ///////////////////////////////////////////////////////////////////////////////
                // Effect Methods

                /// @internal
                /// @brief Function pointer for the OpenAL method "alGenEffects".
                LPALGENEFFECTS alGenEffects;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alDeleteEffects".
                LPALDELETEEFFECTS alDeleteEffects;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alIsEffect".
                LPALISEFFECT alIsEffect;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alEffecti".
                LPALEFFECTI alEffecti;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alEffectiv".
                LPALEFFECTIV alEffectiv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alEffectf".
                LPALEFFECTF alEffectf;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alEffectfv".
                LPALEFFECTFV alEffectfv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetEffecti".
                LPALGETEFFECTI alGetEffecti;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetEffectiv".
                LPALGETEFFECTIV alGetEffectiv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetEffectf".
                LPALGETEFFECTF alGetEffectf;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetEffectfv".
                LPALGETEFFECTFV alGetEffectfv;

                ///////////////////////////////////////////////////////////////////////////////
                // Filter Methods

                /// @internal
                /// @brief Function pointer for the OpenAL method "alGenFilters".
                LPALGENFILTERS alGenFilters;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alDeleteFilters".
                LPALDELETEFILTERS alDeleteFilters;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alIsFilter".
                LPALISFILTER alIsFilter;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alFilteri".
                LPALFILTERI alFilteri;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alFilteriv".
                LPALFILTERIV alFilteriv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alFilterf".
                LPALFILTERF alFilterf;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alFilterfv".
                LPALFILTERFV alFilterfv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetFilteri".
                LPALGETFILTERI alGetFilteri;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetFilteriv".
                LPALGETFILTERIV alGetFilteriv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetFilterf".
                LPALGETFILTERF alGetFilterf;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetFilterfv".
                LPALGETFILTERFV alGetFilterfv;

                ///////////////////////////////////////////////////////////////////////////////
                // Auxiliary Effect Slot Methods

                /// @internal
                /// @brief Function pointer for the OpenAL method "alGenAuxiliaryEffectSlots".
                LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alDeleteAuxiliaryEffectSlots".
                LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alIsAuxiliaryEffectSlot".
                LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alAuxiliaryEffectSloti".
                LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alAuxiliaryEffectSlotiv".
                LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alAuxiliaryEffectSlotf".
                LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alAuxiliaryEffectSlotfv".
                LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetAuxiliaryEffectSloti".
                LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetAuxiliaryEffectSlotiv".
                LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetAuxiliaryEffectSlotf".
                LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
                /// @internal
                /// @brief Function pointer for the OpenAL method "alGetAuxiliaryEffectSlotfv".
                LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

                /// @internal
                /// @brief Class constructor.
                /// @param Device The OpenAL device this interface will check for EFX support.
                EFXInterface(ALCdevice* Device)
                    : Supported(false)
                {
                    if(alcIsExtensionPresent(Device,"ALC_EXT_EFX"))
                    {
                        // Get function pointers
                        alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
                        alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
                        alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
                        alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
                        alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
                        alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
                        alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
                        alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
                        alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
                        alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
                        alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
                        alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
                        alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
                        alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
                        alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
                        alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
                        alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
                        alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
                        alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
                        alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
                        alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
                        alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
                        alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
                        alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
                        alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
                        alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
                        alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
                        alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
                        alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
                        alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
                        alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
                        alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
                        alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");

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
                /// @internal
                /// @brief Class destructor.
                ~EFXInterface() {  }
            };//EFXInterface
        }//OALS
    }//Audio
}//Mezzanine

#endif
