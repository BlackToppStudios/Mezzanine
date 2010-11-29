cmake_minimum_required (VERSION 2.8)
Project( cAudio )
message ("${PROJECT_NAME} - Begin")

#Add some source files
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cAudioCapture.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cAudioEffects.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cAudioManager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cAudioSource.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cConsoleLogReceiver.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cEFXFunctions.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cEffect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cFileLogReceiver.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cFileSource.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cFileSourceFactory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cFilter.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cListener.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cLogger.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cMemoryOverride.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cMemorySource.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cMemoryTracker.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cMutex.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cOggAudioDecoderFactory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cOggDecoder.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cPluginManager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cRawAudioDecoderFactory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cRawDecoder.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cSTLAllocator.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cStandardMemoryProvider.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cThread.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cUtils.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cWavAudioDecoderFactory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/cWavDecoder.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cAudioCapture.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cAudioEffects.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cAudioManager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cAudioMemory.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cAudioSleep.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cAudioSource.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cConsoleLogReceiver.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cEffect.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cFileLogReceiver.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cFileSource.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cFilter.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cListener.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cLogger.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cMemorySource.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cMemoryTracker.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cMutex.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cOggAudioDecoderFactory.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cOggDecoder.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cPluginManager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cRawDecoder.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cStandardMemoryProvider.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cThread.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cWavAudioDecoderFactory.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Source/cWavDecoder.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/EAudioFormats.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioCapture.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioDecoder.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioDecoderFactory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioEffects.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioManager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioPlugin.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IAudioSource.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/ICaptureEventHandler.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IDataSource.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IDataSourceFactory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IEffect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IEffectParameters.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IFilter.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IListener.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/ILogReceiver.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/ILogger.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IManagerEventHandler.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IMemoryProvider.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IPluginManager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/IRefCounted.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/ISourceEventHandler.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/cAudio.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/cAudioDefines.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/cAudioMemory.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/cAudioPlatform.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/cAudioSleep.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/cVector3.h"
)

#Choose the destination directory
#set (${PROJECT_NAME}_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/caudio/")
#set (PROJECT_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/caudio/")
set (LIBRARY_OUTPUT_PATH "${physgame_ROOT_DIR}data/${BuildPlatform}/caudio/")
message ("Putting cAudio binaries in: ${LIBRARY_OUTPUT_PATH}")

add_definitions( ${CommonBuildFlags} )
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources})


#We have removed the cAudio depencies because they were built with an assumed compile and OS version
# to make this work, we have modified the source to compile and removed the "AL/" from
include_directories(
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/include/"
	"${${PROJECT_NAME}_SOURCE_DIR}/cAudio2.1.0/Headers/"
)

if (${WIN32})
	add_dependencies( ${PROJECT_NAME} oggvorbis)
	include_directories(
		"${physgame_LIBINCLUDE_DIR}${BuildPlatform}/openal/"
		"${physgame_LIBINCLUDE_DIR}${BuildPlatform}/openal/AL/"
		"${physgame_LIBINCLUDE_DIR}${BuildPlatform}/oggvorbis/libogg-1.2.1/include"
		"${physgame_LIBINCLUDE_DIR}${BuildPlatform}/oggvorbis/libvorbis-1.3.2/lib/"
		"${physgame_LIBINCLUDE_DIR}${BuildPlatform}/oggvorbis/libvorbis-1.3.2/include"
	)
	link_directories(
		"${physgame_ROOT_DIR}data/${BuildPlatform}/openal/"
		"${physgame_ROOT_DIR}data/${BuildPlatform}/oggvorbis/"
	)
	
	target_link_libraries( ${PROJECT_NAME} OpenAL32 oggvorbis)
else (${WIN32})
	include_directories( "/usr/include/" )		#This should somehow search for pthread.h

	include(FindOpenAL)
	FIND_LIBRARY(VORBIS_LIBRARY
		NAMES vorbis
		PATHS	/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
			/usr/local/lib /usr/lib /sw/lib
			/opt/local/lib /opt/csw/lib /opt/lib
	) 
	FIND_LIBRARY(OGG_LIBRARY
		NAMES ogg
		PATHS	/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
			/usr/local/lib /usr/lib /sw/lib
			/opt/local/lib /opt/csw/lib /opt/lib
	) 
	FIND_LIBRARY(VORBISFILE_LIBRARY
		NAMES vorbisfile
		PATHS	/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.1/lib
			/usr/local/lib /usr/lib /sw/lib
			/opt/local/lib /opt/csw/lib /opt/lib
	) 
	target_link_libraries( ${PROJECT_NAME} ogg openal vorbis vorbisfile)  
endif (${WIN32})

message ("${PROJECT_NAME} - End")
