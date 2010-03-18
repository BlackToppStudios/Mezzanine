#ifndef _physdatatypes_h
#define _physdatatypes_h
///////////////////////////////////////////////////////////////////////////////
//Any Special data types that we need will get declared right here
///////////////////////////////////////
#include <string>

typedef float PhysReal;
typedef unsigned long PhysWhole;
typedef const std::string PhysString;

union SDL_Event;
typedef SDL_Event RawEvent;

//TODO make a typedef for RawEvent.type and a function that return that new datatype
//  this will make it easier to swap out event subsystems when porting between platforms
//  If done right we will only need to swap out the code specific to message internals


#endif

