//this is the place where ALL of the non-multiplatform code goes.
//
#ifndef _crossplatform_cpp
#define _crossplatform_cpp

#include "crossplatform.h"

string GetPluginsDotCFG()
{
#ifdef UBUNTU
	return "data/ubuntu/plugins.cfg";
#endif
#ifdef WINDOWS
	return "data/windows/plugins.cfg";
#endif

}

#endif
