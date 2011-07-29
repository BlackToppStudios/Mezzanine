//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _soundmanager_h
#define _soundmanager_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "managerbase.h"
#include "sound.h"
#include "soundlistener.h"

#include <vector>
#include <map>
#include <sstream>

namespace cAudio
{
    class IAudioManager;
    class IAudioSource;
    class IListener;
}

namespace phys
{
    /// @typedef SoundSet
    /// @brief This is a vector that stores sounds.
    /// @details This is a vector and can be use to store sounds that can be grouped together
    /// for similiar purposes or similiar content for easy tracking.
    class SoundSet : public std::vector< Sound* >
    {
        private:
            /// @brief The name of the sound
            String Name;
        public:
            /// @brief a Simple counter to insure unique names of soundsets
            static Whole UnnamedInstanceCount;

            /// @brief Default constructor
            SoundSet()
                { Name = StringCat("SoundSet",ToString(UnnamedInstanceCount++)); } // Name the First "SoundSet0" then the next "SoundSet1" and then "SoundSet2"...

            /// @brief Default constructor
            explicit SoundSet(const String& _Name) : Name(_Name)
                { }

            /// @brief Get the name of the SoundSet
            const String& GetName() const
                { return Name; }

            //Serializable
            xml::Node ProtoSerialize() const;

            void ProtoDeSerialize(const xml::Node&);

            String SerializableName() const;
    };
    //typedef std::vector< Sound* > SoundSet;
    /// Todo de/serialize sound set



    ///////////////////////////////////////////////////////////////////////////////
    /// @class SoundManager
    /// @headerfile soundmanager.h
    /// @brief This is simply a place for storing all the Sound utilities and functions.
    /// @details This is a place for loading, storing, and running sound files as
    /// necessary in a given game.
    ///////////////////////////////////////////////////////////////////////////////
    class PHYS_LIB SoundManager : public ManagerBase
    {
        protected:
            cAudio::IAudioManager* AudioManager;
            std::map< String, SoundSet* > SoundSets;
            cAudio::IListener* AudioListener;

        public:
            /// @brief Class Constructor
            /// @details This is the class constructor.  It gives you the option to start up the manager
            /// with default values if you choose.  If not then you'll need to setup and initialize the
            /// manager yourself after it is constructed.
            /// @param DefaultSettings Whether or not to load default settings and initialize the
            /// manager immediately.
            SoundManager(bool DefaultSettings=true);
            /// @brief Class Destructor
            /// @details The class destructor.
            ~SoundManager();
            /// @brief Inherited Initializer.
            /// @details Written to allow the class to compile, this currently does nothing, but could be used to run code Use InitializeManager() instead.
            void Initialize();
            /// @brief Empty MainLoopItems
            /// @details This class implements this for the sake of entension and compatibility this function does nothing
            virtual void DoMainLoopItems();

            /// @brief Initializes the Manager.
            /// @details This function will initialize the manager using the device and parameters provided.
            /// You need to call this function if you passed false into the sound manager constructor before
            /// you can use the manager.
            /// @param DeviceName The name of the device you wish to have this manager interface with/use.
            /// @param OutputFrequency Frequency of the output audio, -1 for the devices default.
            /// @param EAXEffectSlots The number of effects per sound allowed to be applied.
            void InitializeManager(ConstString &DeviceName, int OutputFrequency=-1, int EAXEffectSlots=4);
            /// @brief Creates a sound instance from a file that can be used to play sounds.
            /// @details This function will create a Sound from a file and returns a pointer to that Sound.
            /// You can also specify if you want the sound to be streamed from the disk so that it doesn't
            /// have to load the entire song from disk first.
            /// @param SoundName The name of the Sound instance.
            /// @param FilePath The path to the file AND the name of the file.
            /// @param Stream Whether or not to stream the audio from the disk.  Otherwise it has be completely
            /// loaded into memory before it can be used.
            /// @return Returns a pointer to the Sound Instance that was created.
            Sound* CreateSound(ConstString &SoundName, ConstString &FilePath, bool Stream);
            /// @brief Creates a sound instance from a memory buffer.
            /// @details This function allows you to create a sound instance from a sound that is already loaded
            /// into memory.
            /// @param SoundName The name of the sound instance.
            /// @param Data Pointer to the char array (buffer).
            /// @param Size Size of the char array (buffer).
            /// @param Extension The extension of the file used to load that sound (wav, ogg).
            /// @return Returns a pointer to the Sound Instance that was created.
            Sound* CreateSoundFromMemory(String &SoundName, const char* Data, int Size, String &Extension);
            /*/// @brief Creates a sound instance from raw data in a buffer(unknown format).
            /// @details This function allows you to create a sound instance from raw data of an unknown format existing
            /// inside memory.  This will assemble a decoder for the data based on the information you provide.
            /// @param SoundName The name of the sound instance.
            /// @param Data Pointer to the char array (buffer).
            /// @param Size Size of the char array (buffer).
            /// @param Frequency The Sample rate of the audio data.
            /// @param Quality The format of the sound.  Options are: 1 for 8bit Mono, 2 for 8bit Stereo, 3 for 16bit Mono, or 4 for 16bit Stereo.
            Sound* CreateSoundFromRaw(String &SoundName, const char* Data, int Size, Whole Frequency, Whole Quality);*/
            /// @brief Deletes a Sound.
            /// @details Deletes a single Sound via a pointer.
            /// @param SoundName A pointer to the Sound you want deleted.
            void DestroySound(Sound* SoundName);
            /// @brief Gets a sound by it's name.
            /// @details This function will find and return the sound with the speicified name.
            /// @return Returns a pointer to the specified sound.
            Sound* GetSoundByName(String SoundName);
            /// @brief Deletes all stored sounds.
            /// @details This function will delete every Sound that is stored in the manager.
            void DestroyAllSounds();
            /// @brief Creates a sound set.
            /// @details This function will create a sound set vector you can use to store similiar sound instances.
            /// @param SoundSetName The name you wish the sound set to have.
            /// @return Returns a pointer to the created Vector.
            SoundSet* CreateSoundSet(String SoundSetName);
            /// @brief Gets an existing sound set.
            /// @details This function will find the specified sound set and return a pointer to it.
            /// @param SoundSetName The name of the sound set to retrieve.
            /// @return Returns a pointer to the named Vector.
            SoundSet* GetSoundSet(String SoundSetName);
            /// @brief Add's a sound to the defined set.
            /// @details This function will add a sound instance to a created sound set.
            /// @param SoundName The sound instance to be added.
            void AddSoundToSoundSet(String SoundSetName, Sound* SoundInst);
            /// @brief Gets the name of an available device.
            /// @details This function will retrieve the name of a device by it's index on the sound managers device list.
            /// @param Index The position on the device list you wish to access the name of.
            /// @return Returns the name of the device.
            String GetAvailableDeviceNameByIndex(Whole Index);
            /// @brief Gets the name of the default device.
            /// @details This function will return the name of the system default device.
            /// @return Returns the name of the default device.
            String GetDefaultDeviceName();
            /// @brief Gets the number of available devices.
            /// @details This function will return the total number of available devices, including the default.
            /// @return Returns the number of available devices.
            Whole GetAvailableDeviceCount();
            /// @brief Retrieve's the listener for this sound manager.
            /// @details This function will return the listener for this manager which can be used to help create 3D sound.
            /// @return Returns a pointer to the managers Sound Listener.
            SoundListener* GetListener();
            /// @brief Gets the manager type.
            /// @details Gets the type of manager that this is.
            /// @return Returns the manager type.
            ManagerTypeName GetType() const;

            /// @brief This gets the logs that the audio subystem creates
            /// @details Internally the Physgame engine currently uses cAudio to process 3d sound. It has it's own
            /// logging system that we have customized to work with our logger.
            /// @return This gets the log of what actions the audio system has performed
            std::stringstream* GetLogs();

            /// @brief This empties logs that the audio subystem creates
            /// @details Internally the Physgame engine currently uses cAudio to process 3d sound. It has it's own
            /// logging system that we have customized to work with our logger. This clears that data allow us to
            /// work with it as we need
            void ClearLogs();
    };
}

#endif
