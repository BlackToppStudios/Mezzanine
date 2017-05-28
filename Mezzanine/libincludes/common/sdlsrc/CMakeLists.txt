cmake_minimum_required (VERSION 2.8)
project (SDL)
message ( "${PROJECT_NAME} - Begin" )

# When updating the version of SDL, apply all the patches located in the "Pending Patches" folder located in the same directory as this file.  Patches may need to be applied manually.
# Also, when updating check SDL for any mention of "-mwindows" in it's CMake files(Note: Not configure files where I also saw it used, probably because of the CMake files).  
#  Any entries that add it to the linker flags should be commented out and replaced with lines that do all the same flags minus "-mwindows".  If adding "-mwindows" is the only 
#  thing it does then the line can be commented out entirely.
# Lastly, around line 1134 of the cmakelists.txt -fPIC had to be added in an non-windows conditional check to allow compilation on linux.

set(SDL_ATOMIC         off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_AUDIO          off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_CPUINFO        off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_EVENTS         on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_FILE           on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_HAPTIC         on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_JOYSTICK       on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_LOADSO         on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_POWER          on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_RENDER         off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_THREADS        off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_VIDEO          on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)

set(SDL_SHARED         off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_STATIC         on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_VIDEO_OPENGLES off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)

add_subdirectory ( SDL )

mark_as_advanced ( FORCE 
    # SDL Sub-Systems
    SDL_ATOMIC 
	SDL_AUDIO 
	SDL_CPUINFO 
	SDL_EVENTS 
	SDL_FILE 
	SDL_HAPTIC 
	SDL_JOYSTICK 
	SDL_LOADSO
    SDL_POWER 
	SDL_RENDER 
	SDL_SHARED 
	SDL_STATIC 
	SDL_THREADS 
	SDL_TIMERS 
	SDL_VIDEO

    # SDL Low-Level Options
	SDL_ALTIVEC
    SDL_ASSEMBLY 
	SDL_MMX
	SDL_SSEMATH
	SDL_SSE
	SDL_SSE2
	SDL_SSE3
	SDL_3DNOW 
	
	# SDL Video Options
	SDL_DIRECTX 
	SDL_RENDER_D3D
	SDL_VIDEO_COCOA
	SDL_VIDEO_DIRECTFB
	SDL_VIDEO_DUMMY
	SDL_VIDEO_MIR
	SDL_VIDEO_OPENGL
	SDL_VIDEO_OPENGLES
	SDL_VIDEO_RPI
	SDL_VIDEO_VIVANTE 
	SDL_VIDEO_WAYLAND
	SDL_VIDEO_X11
	SDL_VIDEO_X11_XCURSOR
	SDL_VIDEO_X11_XINERAMA
	SDL_VIDEO_X11_XINPUT
	SDL_VIDEO_X11_XSCRNSAVER
	SDL_VIDEO_X11_XSHAPE
	SDL_VIDEO_X11_XRANDR
	SDL_VIDEO_X11_XVM
	
	# SDL Audio Options
	SDL_ALSA
	SDL_ALSA_SHARED
	SDL_ARTS
	SDL_ARTS_SHARED
	SDL_DISKAUDIO
	SDL_DUMMYAUDIO
	SDL_ESD
	SDL_ESD_SHARED
	SDL_FUSIONSOUND
	SDL_FUSIONSOUND_SHARED
	SDL_NAS
	SDL_NAS_SHARED 
	SDL_OSS
	SDL_PULSEAUDIO
	SDL_PULSEAUDIO_SHARED
	
	# SDL Threading Options
	SDL_GCC_ATOMICS
	SDL_PTHREADS
	SDL_PTHREADS_SEM
	
	# SDL Options
    SDL_ASSERTIONS 
	SDL_CLOCK_GETTIME
	SDL_LIBC 
    SDL_DIRECTFB_SHARED 
    SDL_DLOPEN
    SDL_RPATH
    SDL_X11_SHARED 
	# Updated Options
	SDL_FILESYSTEM 
	SDL_INPUT_TSLIB 
	SDL_LIBSAMPLERATE 
	SDL_SNDIO 
	SDL_STATIC_PIC

    # SDL Indirect Options
    ARTS_CONFIG 
	D_NAS_LIB 
	XCURSOR_LIB 
	XEXT_LIB 
	XI_LIB 
	XINERAMA_LIB 
	XRANDR_LIB 
	XRENDER_LIB 
	XSS_LIB 
	XXF86VM_LIB 
	X11_LIB
)

message ("${PROJECT_NAME} - End")
