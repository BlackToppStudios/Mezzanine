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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalssoundproxy_h
#define _audiooalssoundproxy_h

#include "Audio/soundproxy.h"
#include "vector3.h"
#include "quaternion.h"

// OALS forward declares
#ifndef OALS_STRUCTS_DECLARED
struct ALCcontext;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Filter;
            class Effect;
            class SoundScapeManager;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OALS implementation for a sound in 3D space.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB SoundProxy : public Audio::SoundProxy
            {
            public:
                /// @brief Basic container type for OALS context storage by this class.
                typedef std::vector<ALCcontext*>                  ContextContainer;
                /// @brief Iterator type for OALS context instances stored by this class.
                typedef ContextContainer::iterator                ContextIterator;
                /// @brief Const Iterator type for OALS context instances stored by this class.
                typedef ContextContainer::const_iterator          ConstContextIterator;
                /// @brief Datatype for maping an internal source to a context.
                typedef std::pair<ALCcontext*,UInt32>             ContextSourcePair;
                /// @brief Basic container type for @ref ContextSourcePair storage by this class.
                typedef std::vector<ContextSourcePair>            ContextSourceContainer;
                /// @brief Iterator type for @ref ContextSourcePair instances stored by this class.
                typedef ContextSourceContainer::iterator          ContextSourceIterator;
                /// @brief Const Iterator type for @ref ContextSourcePair instances stored by this class.
                typedef ContextSourceContainer::const_iterator    ConstContextSourceIterator;
                /// @brief Basic container type for the storage of @ref iEffect instances by this class.
                typedef std::vector<OALS::Effect*>                EffectContainer;
                /// @brief Iterator type for @ref iEffect instances stored by this class.
                typedef EffectContainer::iterator                 EffectIterator;
                /// @brief Const Iterator type for @ref iEffect instances stored by this class.
                typedef EffectContainer::const_iterator           ConstEffectIterator;
                /// @brief Basic container type for the storage of internal buffer handles.
                typedef std::vector<UInt32>                       BufferContainer;
                /// @brief Iterator type for buffer handles stored by this class.
                typedef BufferContainer::iterator                 BufferIterator;
                /// @brief Const Iterator type for buffer handles stored by this class.
                typedef BufferContainer::const_iterator           ConstBufferIterator;
            protected:
                /// @internal
                /// @brief This stores all the contexts this proxy needs to be aware of and the sources that belong to those contexts.
                ContextSourceContainer ContextsAndSources;
                /// @internal
                /// @brief This stores the handles to all the OALS buffers owned by this sound proxy.
                BufferContainer Buffers;
                /// @internal
                /// @brief This stores the effects currently being applied to audio emitted by this sound proxy.
                EffectContainer Effects;
                /// @internal
                /// @brief The 3D rotation of this sound proxy.
                Quaternion Orientation;
                /// @internal
                /// @brief The 3D position of this sound proxy.
                Vector3 Location;
                /// @internal
                /// @brief The direction and speed of this sound proxy.
                Vector3 Velocity;
                /// @internal
                /// @brief The direction and speed of this object for doppler effects.
                Vector3 DopplerVelocity;
                /// @internal
                /// @brief This is a pointer to the managert that created this listener.
                SoundScapeManager* Manager;
                /// @internal
                /// @brief The filter processing audio emitted by this sound proxy.
                OALS::Filter* SoundFilter;
                /// @internal
                /// @brief The decoder responsible for tracking this sound proxy's position in the audio stream and decompressing into something we can populate buffers with.
                iDecoder* SoundDecoder;
                /// @internal
                /// @brief The modifier that can enhance or diminish the doppler effect coming from this sound proxy.
                Real DopplerStrength;
                /// @internal
                /// @brief The outer angle of the cone at which this sound proxy can be heard.
                Real OuterConeAngle;
                /// @internal
                /// @brief The gain of the sound proxy on the outer edge of it's audible cone.
                Real OuterConeVolume;
                /// @internal
                /// @brief The inner angle of the cone at which this sound proxy can be heard.
                Real InnerConeAngle;
                /// @internal
                /// @brief The minimum distance at which this sound proxy can be heard.
                Real MinDist;
                /// @internal
                /// @brief The maximum distance at which this sound proxy can be heard.
                Real MaxDist;
                /// @internal
                /// @brief The value determining how the sound proxy will attenuate over a distance.
                Real RolloffFactor;
                /// @internal
                /// @brief The base volume applied to this sound proxy alone, before external modifiers.
                Real BaseVolume;
                /// @internal
                /// @brief The minimum volume this sound proxy is allowed to reach, regardless of other settings.
                Real MinVolume;
                /// @internal
                /// @brief The maximum volume this sound proxy is allowed to reach, regardless of other settings.
                Real MaxVolume;
                /// @internal
                /// @brief The speed of playback modifier for the sound proxy.  Higher values make playback sound proxy more high pitched.
                Real SoundPitch;
                /// @internal
                /// @brief The type of sound proxy this is.
                UInt16 SType;
                /// @internal
                /// @brief This is a bitfield storing the the current state of this sound proxy.
                UInt16 State;
                /// @internal
                /// @brief Stores whether or not sound being emitted in a specific direction.
                bool DirectSound;

                /// @internal
                /// @brief Makes the context this listener belongs to the current context.
                void MakeCurrent(ALCcontext* Context);
                /// @internal
                /// @brief Streams the content from the decoder into an OALS buffer.
                bool StreamToBuffer(const UInt32 Buffer);
            public:
                /// @brief Class constructor.
                /// @param Type The type of sound to initialize.  See @ref Audio::SoundType enum for the basic values.
                /// @param Decode A pointer to the decoder assigned to this sound.
                /// @param Contexts A container holding all the current 3D contexts.
                /// @param Creator A pointer to the manager that created this proxy.
                SoundProxy(const UInt16 Type, iDecoder* Decode, const ContextContainer& Contexts, SoundScapeManager* Creator);
                /// @brief Class destructor.
                virtual ~SoundProxy();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iSound::IsValid() const
                virtual bool IsValid() const;
                /// @copydoc iSound::GetType() const
                virtual UInt16 GetType() const;
                /// @copydoc iSound::GetDecoder() const
                virtual iDecoder* GetDecoder() const;

                /// @copydoc iSound::SetPitch(const Real)
                virtual void SetPitch(const Real Pitch);
                /// @copydoc iSound::GetPitch() const
                virtual Real GetPitch() const;
                /// @copydoc iSound::SetStream(Resource::DataStreamPtr, const Audio::Encoding)
                virtual void SetStream(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
                /// @copydoc iSound::SetStream(const UInt16, Resource::DataStreamPtr, const Audio::Encoding)
                virtual void SetStream(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
                /// @copydoc iSound::SetStream(iDecoder*)
                virtual void SetStream(iDecoder* Decode);
                /// @copydoc iSound::SetStream(const UInt16, iDecoder*)
                virtual void SetStream(const UInt16 Type, iDecoder* Decode);

                /// @copydoc Audio::SoundProxy::SetDirectSound(bool)
                virtual void SetDirectSound(bool Enable);
                /// @copydoc Audio::SoundProxy::IsDirectingSound() const
                virtual bool IsDirectingSound() const;

                /// @copydoc Audio::SoundProxy::AddToWorld()
                virtual void AddToWorld();
                /// @copydoc Audio::SoundProxy::RemoveFromWorld()
                virtual void RemoveFromWorld();
                /// @copydoc WorldProxy::IsInWorld()
                virtual bool IsInWorld() const;

                /// @copydoc WorldProxy::GetCreator() const
                virtual WorldManager* GetCreator() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Playback

                /// @copydoc iSound::Play()
                virtual bool Play();
                /// @copydoc iSound::IsPlaying() const
                virtual bool IsPlaying() const;
                /// @copydoc iSound::Pause()
                virtual void Pause();
                /// @copydoc iSound::IsPaused() const
                virtual bool IsPaused() const;
                /// @copydoc iSound::Stop()
                virtual void Stop();
                /// @copydoc iSound::IsStopped() const
                virtual bool IsStopped() const;
                /// @copydoc iSound::Loop(bool ToLoop)
                virtual void Loop(bool ToLoop);
                /// @copydoc iSound::IsLooping() const
                virtual bool IsLooping() const;

                /// @copydoc iSound::Seek(const Real, bool)
                virtual bool Seek(const Real Seconds, bool Relative = false);

                ///////////////////////////////////////////////////////////////////////////////
                // Volume Control

                /// @copydoc iSound::GetVolume() const
                virtual Real GetVolume() const;
                /// @copydoc iSound::SetBaseVolume(const Real)
                virtual void SetBaseVolume(const Real Base);
                /// @copydoc iSound::GetBaseVolume() const
                virtual Real GetBaseVolume() const;
                /// @copydoc iSound::SetMinVolume(const Real)
                virtual void SetMinVolume(const Real MinVol);
                /// @copydoc iSound::GetMinVolume() const
                virtual Real GetMinVolume() const;
                /// @copydoc iSound::SetMaxVolume(const Real)
                virtual void SetMaxVolume(const Real MaxVol);
                /// @copydoc iSound::GetMaxVolume() const
                virtual Real GetMaxVolume() const;

                ///////////////////////////////////////////////////////////////////////////////
                // 3 Dimensional Configuration

                /// @copydoc Audio::SoundProxy::SetRolloffFactor(const Real&)
                virtual void SetRolloffFactor(const Real& Rolloff);
                /// @copydoc Audio::SoundProxy::GetRolloffFactor() const
                virtual Real GetRolloffFactor() const;
                /// @copydoc Audio::SoundProxy::SetMinDistance(const Real&)
                virtual void SetMinDistance(const Real& MinDistance);
                /// @copydoc Audio::SoundProxy::GetMinDistance() const
                virtual Real GetMinDistance() const;
                /// @copydoc Audio::SoundProxy::SetMaxDistance(const Real&)
                virtual void SetMaxDistance(const Real& MaxDistance);
                /// @copydoc Audio::SoundProxy::GetMaxDistance() const
                virtual Real GetMaxDistance() const;
                /// @copydoc Audio::SoundProxy::SetInnerConeAngle(const Real&)
                virtual void SetInnerConeAngle(const Real& InnerAngle);
                /// @copydoc Audio::SoundProxy::GetInnerConeAngle() const
                virtual Real GetInnerConeAngle() const;
                /// @copydoc Audio::SoundProxy::SetOuterConeAngle(const Real&)
                virtual void SetOuterConeAngle(const Real& OuterAngle);
                /// @copydoc Audio::SoundProxy::GetOuterConeAngle() const
                virtual Real GetOuterConeAngle() const;
                /// @copydoc Audio::SoundProxy::SetOuterConeVolume(const Real&)
                virtual void SetOuterConeVolume(const Real& OuterVolume);
                /// @copydoc Audio::SoundProxy::GetOuterConeVolume() const
                virtual Real GetOuterConeVolume() const;
                /// @copydoc Audio::SoundProxy::SetDopplerStrength(const Real&)
                virtual void SetDopplerStrength(const Real& DopStr);
                /// @copydoc Audio::SoundProxy::GetDopplerStrength() const
                virtual Real GetDopplerStrength() const;
                /// @copydoc Audio::SoundProxy::SetDopplerVelocity(const Vector3&)
                virtual void SetDopplerVelocity(const Vector3& DopVel);
                /// @copydoc Audio::SoundProxy::GetDopplerVelocity() const
                virtual Vector3 GetDopplerVelocity() const;
                /// @copydoc Audio::SoundProxy::SetVelocity(const Vector3&)
                virtual void SetVelocity(const Vector3& Vel);
                /// @copydoc Audio::SoundProxy::GetVelocity() const
                virtual Vector3 GetVelocity() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Methods

                /// @copydoc WorldProxy::SetLocation(const Vector3&)
                virtual void SetLocation(const Vector3& Loc);
                /// @copydoc WorldProxy::SetLocation(const Real, const Real, const Real)
                virtual void SetLocation(const Real X, const Real Y, const Real Z);
                /// @copydoc WorldProxy::GetLocation() const
                virtual Vector3 GetLocation() const;
                /// @copydoc WorldProxy::SetOrientation(const Quaternion&)
                virtual void SetOrientation(const Quaternion& Ori);
                /// @copydoc WorldProxy::SetOrientation(const Real, const Real, const Real, const Real)
                virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W);
                /// @copydoc WorldProxy::GetOrientation() const
                virtual Quaternion GetOrientation() const;
                /// @copydoc WorldProxy::SetScale(const Vector3&)
                virtual void SetScale(const Vector3& Sc);
                /// @copydoc WorldProxy::SetScale(const Real, const Real, const Real)
                virtual void SetScale(const Real X, const Real Y, const Real Z);
                /// @copydoc WorldProxy::GetScale() const
                virtual Vector3 GetScale() const;

                /// @copydoc WorldProxy::Translate(const Vector3&)
                virtual void Translate(const Vector3& Trans);
                /// @copydoc WorldProxy::Translate(const Real, const Real, const Real)
                virtual void Translate(const Real X, const Real Y, const Real Z);
                /// @copydoc WorldProxy::Yaw(const Real)
                virtual void Yaw(const Real Angle);
                /// @copydoc WorldProxy::Pitch(const Real)
                virtual void Pitch(const Real Angle);
                /// @copydoc WorldProxy::Roll(const Real)
                virtual void Roll(const Real Angle);
                /// @copydoc WorldProxy::Rotate(const Vector3&, const Real)
                virtual void Rotate(const Vector3& Axis, const Real Angle);
                /// @copydoc WorldProxy::Rotate(const Quaternion&)
                virtual void Rotate(const Quaternion& Rotation);
                /// @copydoc WorldProxy::Scale(const Vector3&)
                virtual void Scale(const Vector3& Scale);
                /// @copydoc WorldProxy::Scale(const Real, const Real, const Real)
                virtual void Scale(const Real X, const Real Y, const Real Z);

                ///////////////////////////////////////////////////////////////////////////////
                // Effects Methods

                /// @copydoc iSound::AttachEffect(const UInt32, iEffect*)
                virtual bool AttachEffect(const UInt32 Slot, iEffect* Eff);
                /// @copydoc iSound::GetEffect(const UInt32 Slot) const
                virtual iEffect* GetEffect(const UInt32 Slot) const;
                /// @copydoc iSound::GetMaxEffectSlots() const
                virtual UInt32 GetMaxEffectSlots() const;
                /// @copydoc iSound::GetNumEffectSlotsAvailable() const
                virtual UInt32 GetNumEffectSlotsAvailable() const;
                /// @copydoc iSound::RemoveEffect(const UInt32)
                virtual void RemoveEffect(const UInt32 Slot);

                ///////////////////////////////////////////////////////////////////////////////
                // Filter Methods

                /// @copydoc iSound::AttachFilter(iFilter*)
                virtual bool AttachFilter(iFilter* Fil);
                /// @copydoc iSound::GetFilter() const
                virtual iFilter* GetFilter() const;
                /// @copydoc iSound::RemoveFilter()
                virtual void RemoveFilter();

                ///////////////////////////////////////////////////////////////////////////////
                // Serialization

                /// @copydoc WorldProxy::ProtoSerialize(XML::Node& CurrentRoot) const
                virtual void ProtoSerialize(XML::Node& CurrentRoot) const;
                /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node& OneNode)
                virtual void ProtoDeSerialize(const XML::Node& OneNode);
                /// @copydoc WorldProxy::GetDerivedSerializableName() const
                virtual String GetDerivedSerializableName() const;
                /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
                /// @return A string containing the name of this class.
                static String SerializableName();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Updates all the necessary components of this sound.
                virtual void _Update();
                /// @internal
                /// @brief Updates the buffers for this sound as necessary for seemless playback.
                /// @return Returns true of there were no errors updating, false if there were.
                virtual bool _UpdateBuffers();
                /// @internal
                /// @brief Notifies this sound proxy a new context was created by it's parent @ref SoundScapeManager and needs to create an additional source.
                /// @param Index The index at which the context was created.
                /// @param Context The new context that was created.
                virtual void _OnContextCreated(const UInt32 Index, ALCcontext* Context);
                /// @internal
                /// @brief Notifies this sound proxy a context is being destroyed by it's parent @ref SoundScapeManager and needs to free it's resources related to that context.
                /// @param Index The index at which the context is being destroyed.
                /// @param Context The context being destroyed.
                virtual void _OnContextDestroyed(const UInt32 Index, ALCcontext* Context);
            };//SoundProxy
        }//OALS
    }//Audio
}//Mezzanine

#endif
