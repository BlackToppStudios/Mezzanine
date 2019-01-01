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
                /// @brief This stores all the contexts this proxy needs to be aware of and the sources that belong to those contexts.
                ContextSourceContainer ContextsAndSources;
                /// @brief This stores the handles to all the OALS buffers owned by this sound proxy.
                BufferContainer Buffers;
                /// @brief This stores the effects currently being applied to audio emitted by this sound proxy.
                EffectContainer Effects;
                /// @brief The 3D rotation of this sound proxy.
                Quaternion Orientation;
                /// @brief The 3D position of this sound proxy.
                Vector3 Location;
                /// @brief The direction and speed of this sound proxy.
                Vector3 Velocity;
                /// @brief The direction and speed of this object for doppler effects.
                Vector3 DopplerVelocity;
                /// @brief This is a pointer to the managert that created this listener.
                OALS::SoundScapeManager* Manager;
                /// @brief The filter processing audio emitted by this sound proxy.
                OALS::Filter* SoundFilter;
                /// @brief The decoder responsible for tracking this sound proxy's position in the audio stream and decompressing into something we can populate buffers with.
                iDecoder* SoundDecoder;
                /// @brief The modifier that can enhance or diminish the doppler effect coming from this sound proxy.
                Real DopplerStrength;
                /// @brief The outer angle of the cone at which this sound proxy can be heard.
                Real OuterConeAngle;
                /// @brief The gain of the sound proxy on the outer edge of it's audible cone.
                Real OuterConeVolume;
                /// @brief The inner angle of the cone at which this sound proxy can be heard.
                Real InnerConeAngle;
                /// @brief The minimum distance at which this sound proxy can be heard.
                Real MinDist;
                /// @brief The maximum distance at which this sound proxy can be heard.
                Real MaxDist;
                /// @brief The value determining how the sound proxy will attenuate over a distance.
                Real RolloffFactor;
                /// @brief The base volume applied to this sound proxy alone, before external modifiers.
                Real BaseVolume;
                /// @brief The minimum volume this sound proxy is allowed to reach, regardless of other settings.
                Real MinVolume;
                /// @brief The maximum volume this sound proxy is allowed to reach, regardless of other settings.
                Real MaxVolume;
                /// @brief The speed of playback modifier for the sound proxy.  Higher values make playback sound proxy more high pitched.
                Real SoundPitch;
                /// @brief The type of sound proxy this is.
                UInt16 SType;
                /// @brief This is a bitfield storing the the current state of this sound proxy.
                UInt16 State;
                /// @brief Stores whether or not sound being emitted in a specific direction.
                Boole DirectSound;

                /// @brief Makes the context this listener belongs to the current context.
                /// @param Context The audio context to be made current.
                void MakeCurrent(ALCcontext* Context);
                /// @brief Streams the content from the decoder into an OALS buffer.
                /// @param BufferID The ID of the buffer to be streamed to.
                /// @return Returns true if the streaming was successful, false if there was a failure.
                Boole StreamToBuffer(const UInt32 BufferID);
                /// @brief Updates the internal data of the attached filter to reflect any changes made to the filter externally.
                /// @param SourceID The ID of the audio source being updated with the newest filter settings.
                void RefreshFilter(const UInt32 SourceID);
                /// @brief Updates the internal data of the attached effects to reflect any changes made to the effects externally.
                /// @param SourceID The ID of the audio source being updated with the newest effect settings.
                void RefreshEffects(const UInt32 SourceID);
            public:
                /// @brief Class constructor.
                /// @param Type The type of sound to initialize.  See @ref Audio::SoundType enum for the basic values.
                /// @param ID The unique ID of this SoundProxy.
                /// @param Decode A pointer to the decoder assigned to this sound.
                /// @param Contexts A container holding all the current 3D contexts.
                /// @param Creator A pointer to the manager that created this proxy.
                SoundProxy(const UInt16 Type, const UInt32 ID, iDecoder* Decode, const ContextContainer& Contexts, OALS::SoundScapeManager* Creator);
                /// @brief Class destructor.
                virtual ~SoundProxy();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc EntityProxy::GetComponentType() const
                virtual Mezzanine::ComponentType GetComponentType() const override;

                /// @copydoc iSound::IsValid() const
                virtual Boole IsValid() const override;
                /// @copydoc iSound::GetType() const
                virtual UInt16 GetType() const override;
                /// @copydoc iSound::GetDecoder() const
                virtual iDecoder* GetDecoder() const override;

                /// @copydoc iSound::SetPitch(const Real)
                virtual void SetPitch(const Real Pitch) override;
                /// @copydoc iSound::GetPitch() const
                virtual Real GetPitch() const override;
                /// @copydoc iSound::SetStream(IStreamPtr, const Audio::Encoding)
                virtual void SetStream(IStreamPtr Stream, const Audio::Encoding Encode) override;
                /// @copydoc iSound::SetStream(const UInt16, IStreamPtr, const Audio::Encoding)
                virtual void SetStream(const UInt16 Type, IStreamPtr Stream, const Audio::Encoding Encode) override;
                /// @copydoc iSound::SetStream(iDecoder*)
                virtual void SetStream(iDecoder* Decode) override;
                /// @copydoc iSound::SetStream(const UInt16, iDecoder*)
                virtual void SetStream(const UInt16 Type, iDecoder* Decode) override;

                /// @copydoc Audio::SoundProxy::SetDirectSound(Boole)
                virtual void SetDirectSound(Boole Enable) override;
                /// @copydoc Audio::SoundProxy::IsDirectingSound() const
                virtual Boole IsDirectingSound() const override;

                /// @copydoc Audio::SoundProxy::Activate()
                virtual void Activate() override;
                /// @copydoc Audio::SoundProxy::Deactivate()
                virtual void Deactivate() override;

                /// @copydoc EntityProxy::IsActivated()
                virtual Boole IsActivated() const override;
                /// @copydoc EntityProxy::IsStatic() const
                virtual Boole IsStatic() const override;

                /// @copydoc EntityComponent::GetCreator() const
                virtual EntityComponentManager* GetCreator() const override;

                ///////////////////////////////////////////////////////////////////////////////
                // Playback

                /// @copydoc iSound::Play()
                virtual Boole Play() override;
                /// @copydoc iSound::IsPlaying() const
                virtual Boole IsPlaying() const override;
                /// @copydoc iSound::Pause()
                virtual void Pause() override;
                /// @copydoc iSound::IsPaused() const
                virtual Boole IsPaused() const override;
                /// @copydoc iSound::Stop()
                virtual void Stop() override;
                /// @copydoc iSound::IsStopped() const
                virtual Boole IsStopped() const override;
                /// @copydoc iSound::Loop(Boole ToLoop)
                virtual void Loop(Boole ToLoop) override;
                /// @copydoc iSound::IsLooping() const
                virtual Boole IsLooping() const override;

                /// @copydoc iSound::Seek(const Real, Boole)
                virtual Boole Seek(const Real Seconds, Boole Relative = false) override;

                ///////////////////////////////////////////////////////////////////////////////
                // Volume Control

                /// @copydoc iSound::GetVolume() const
                virtual Real GetVolume() const override;
                /// @copydoc iSound::SetBaseVolume(const Real)
                virtual void SetBaseVolume(const Real Base) override;
                /// @copydoc iSound::GetBaseVolume() const
                virtual Real GetBaseVolume() const override;
                /// @copydoc iSound::SetMinVolume(const Real)
                virtual void SetMinVolume(const Real MinVol) override;
                /// @copydoc iSound::GetMinVolume() const
                virtual Real GetMinVolume() const override;
                /// @copydoc iSound::SetMaxVolume(const Real)
                virtual void SetMaxVolume(const Real MaxVol) override;
                /// @copydoc iSound::GetMaxVolume() const
                virtual Real GetMaxVolume() const override;

                ///////////////////////////////////////////////////////////////////////////////
                // 3 Dimensional Configuration

                /// @copydoc Audio::SoundProxy::SetRolloffFactor(const Real&)
                virtual void SetRolloffFactor(const Real& Rolloff) override;
                /// @copydoc Audio::SoundProxy::GetRolloffFactor() const
                virtual Real GetRolloffFactor() const override;
                /// @copydoc Audio::SoundProxy::SetMinDistance(const Real&)
                virtual void SetMinDistance(const Real& MinDistance) override;
                /// @copydoc Audio::SoundProxy::GetMinDistance() const
                virtual Real GetMinDistance() const override;
                /// @copydoc Audio::SoundProxy::SetMaxDistance(const Real&)
                virtual void SetMaxDistance(const Real& MaxDistance) override;
                /// @copydoc Audio::SoundProxy::GetMaxDistance() const
                virtual Real GetMaxDistance() const override;
                /// @copydoc Audio::SoundProxy::SetInnerConeAngle(const Real&)
                virtual void SetInnerConeAngle(const Real& InnerAngle) override;
                /// @copydoc Audio::SoundProxy::GetInnerConeAngle() const
                virtual Real GetInnerConeAngle() const override;
                /// @copydoc Audio::SoundProxy::SetOuterConeAngle(const Real&)
                virtual void SetOuterConeAngle(const Real& OuterAngle) override;
                /// @copydoc Audio::SoundProxy::GetOuterConeAngle() const
                virtual Real GetOuterConeAngle() const override;
                /// @copydoc Audio::SoundProxy::SetOuterConeVolume(const Real&)
                virtual void SetOuterConeVolume(const Real& OuterVolume) override;
                /// @copydoc Audio::SoundProxy::GetOuterConeVolume() const
                virtual Real GetOuterConeVolume() const override;
                /// @copydoc Audio::SoundProxy::SetDopplerStrength(const Real&)
                virtual void SetDopplerStrength(const Real& DopStr) override;
                /// @copydoc Audio::SoundProxy::GetDopplerStrength() const
                virtual Real GetDopplerStrength() const override;
                /// @copydoc Audio::SoundProxy::SetDopplerVelocity(const Vector3&)
                virtual void SetDopplerVelocity(const Vector3& DopVel) override;
                /// @copydoc Audio::SoundProxy::GetDopplerVelocity() const
                virtual Vector3 GetDopplerVelocity() const override;
                /// @copydoc Audio::SoundProxy::SetVelocity(const Vector3&)
                virtual void SetVelocity(const Vector3& Vel) override;
                /// @copydoc Audio::SoundProxy::GetVelocity() const
                virtual Vector3 GetVelocity() const override;

                ///////////////////////////////////////////////////////////////////////////////
                // Transform Methods

                /// @copydoc EntityProxy::SetTransform(const Transform&)
                virtual void SetTransform(const Transform& Trans) override;
                /// @copydoc EntityProxy::SetTransform(const Vector3&,const Quaternion&)
                virtual void SetTransform(const Vector3& Loc, const Quaternion& Ori) override;
                /// @copydoc EntityProxy::GetTransform() const
                virtual Transform GetTransform() const override;

                /// @copydoc EntityProxy::SetLocation(const Vector3&)
                virtual void SetLocation(const Vector3& Loc) override;
                /// @copydoc EntityProxy::SetLocation(const Real, const Real, const Real)
                virtual void SetLocation(const Real X, const Real Y, const Real Z) override;
                /// @copydoc EntityProxy::GetLocation() const
                virtual Vector3 GetLocation() const override;
                /// @copydoc EntityProxy::SetOrientation(const Quaternion&)
                virtual void SetOrientation(const Quaternion& Ori) override;
                /// @copydoc EntityProxy::SetOrientation(const Real, const Real, const Real, const Real)
                virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W) override;
                /// @copydoc EntityProxy::GetOrientation() const
                virtual Quaternion GetOrientation() const override;
                /// @copydoc EntityProxy::SetScale(const Vector3&)
                virtual void SetScale(const Vector3& Sc) override;
                /// @copydoc EntityProxy::SetScale(const Real, const Real, const Real)
                virtual void SetScale(const Real X, const Real Y, const Real Z) override;
                /// @copydoc EntityProxy::GetScale() const
                virtual Vector3 GetScale() const override;

                /// @copydoc EntityProxy::Translate(const Vector3&)
                virtual void Translate(const Vector3& Trans) override;
                /// @copydoc EntityProxy::Translate(const Real, const Real, const Real)
                virtual void Translate(const Real X, const Real Y, const Real Z) override;
                /// @copydoc EntityProxy::Yaw(const Real)
                virtual void Yaw(const Real Angle) override;
                /// @copydoc EntityProxy::Pitch(const Real)
                virtual void Pitch(const Real Angle) override;
                /// @copydoc EntityProxy::Roll(const Real)
                virtual void Roll(const Real Angle) override;
                /// @copydoc EntityProxy::Rotate(const Vector3&, const Real)
                virtual void Rotate(const Vector3& Axis, const Real Angle) override;
                /// @copydoc EntityProxy::Rotate(const Quaternion&)
                virtual void Rotate(const Quaternion& Rotation) override;
                /// @copydoc EntityProxy::Scale(const Vector3&)
                virtual void Scale(const Vector3& Scale) override;
                /// @copydoc EntityProxy::Scale(const Real, const Real, const Real)
                virtual void Scale(const Real X, const Real Y, const Real Z) override;

                ///////////////////////////////////////////////////////////////////////////////
                // Effects Methods

                /// @copydoc iSound::AttachEffect(const UInt32, iEffect*)
                virtual Boole AttachEffect(const UInt32 Slot, iEffect* Eff) override;
                /// @copydoc iSound::GetEffect(const UInt32 Slot) const
                virtual iEffect* GetEffect(const UInt32 Slot) const override;
                /// @copydoc iSound::GetMaxEffectSlots() const
                virtual UInt32 GetMaxEffectSlots() const override;
                /// @copydoc iSound::GetNumEffectSlotsAvailable() const
                virtual UInt32 GetNumEffectSlotsAvailable() const override;
                /// @copydoc iSound::RemoveEffect(const UInt32)
                virtual void RemoveEffect(const UInt32 Slot) override;

                ///////////////////////////////////////////////////////////////////////////////
                // Filter Methods

                /// @copydoc iSound::AttachFilter(iFilter*)
                virtual Boole AttachFilter(iFilter* Fil) override;
                /// @copydoc iSound::GetFilter() const
                virtual iFilter* GetFilter() const override;
                /// @copydoc iSound::RemoveFilter()
                virtual void RemoveFilter() override;

                ///////////////////////////////////////////////////////////////////////////////
                // Serialization

                /// @copydoc EntityProxy::ProtoSerialize(XML::Node&) const
                virtual void ProtoSerialize(XML::Node& ParentNode) const override;
                /// @copydoc EntityProxy::ProtoSerializeProperties(XML::Node&) const
                virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const override;
                /// @brief Convert the Decoder used by this class to an XML::Node ready for serialization.
                /// @param SelfRoot The root node containing all the serialized data for this instance.
                virtual void ProtoSerializeDecoder(XML::Node& SelfRoot) const;
                /// @brief Convert the Filter attached to this class to an XML::Node ready for serialization.
                /// @param SelfRoot The root node containing all the serialized data for this instance.
                virtual void ProtoSerializeFilter(XML::Node& SelfRoot) const;
                /// @brief Convert the Effects attached to this class to an XML::Node ready for serialization.
                /// @param SelfRoot The root node containing all the serialized data for this instance.
                virtual void ProtoSerializeEffects(XML::Node& SelfRoot) const;

                /// @copydoc EntityProxy::ProtoDeSerialize(const XML::Node)
                virtual void ProtoDeSerialize(const XML::Node& SelfRoot) override;
                /// @copydoc EntityProxy::ProtoDeSerializeProperties(const XML::Node&)
                virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot) override;
                /// @brief Take the data stored in an XML Node and overwrite the Decoder used by this object with it.
                /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
                virtual void ProtoDeSerializeDecoder(const XML::Node& SelfRoot);
                /// @brief Take the data stored in an XML Node and overwrite the Filter attached to this object with it.
                /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
                virtual void ProtoDeSerializeFilter(const XML::Node& SelfRoot);
                /// @brief Take the data stored in an XML Node and overwrite the Effects attached to this object with it.
                /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
                virtual void ProtoDeSerializeEffects(const XML::Node& SelfRoot);

                /// @copydoc EntityProxy::GetDerivedSerializableName() const
                virtual String GetDerivedSerializableName() const override;
                /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
                /// @return A string containing the name of this class.
                static String GetSerializableName();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Updates all the necessary components of this sound.
                virtual void _Update();
                /// @internal
                /// @brief Updates the buffers for this sound as necessary for seemless playback.
                /// @return Returns true of there were no errors updating, false if there were.
                virtual Boole _UpdateBuffers();
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
