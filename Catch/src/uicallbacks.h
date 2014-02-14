#ifndef _uicallbacks_h
#define _uicallbacks_h

#include <mezzanine.h>

using namespace Mezzanine;

/// @brief Subscriber method allowing publishers to break the main loop.
/// @param Args An EventArguments struct containing additional data about the event being fired.
void AllAppExit(const EventArguments& Args);

#endif
