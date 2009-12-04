#ifndef _crossplatform_cpp
#define _crossplatform_cpp
///////////////////////////////////////////////////////////////////////////////
//this is the place where ALL of the non-multiplatform code goes.

///////////////////////////////////////////////////////////////////////////////
//Additional Includes

#include "crossplatform.h"

///////////////////////////////////////////////////////////////////////////////
//returns: The appropriate string, which contains a path, to the correct
//"plugins.cfg" file
string GetPluginsDotCFG()
{
#ifdef UBUNTU
	return "data/ubuntu/plugins.cfg";
#endif
#ifdef WINDOWS
	return "data/windows/plugins.cfg";
#endif

}


///////////////////////////////////////////////////////////////////////////////
//returns: The appropriate string, which contains a path, to the correct
//"settings.cfg" file
string GetSettingsDotCFG()
{
#ifdef UBUNTU
	return "data/ubuntu/settings.cfg";
#endif
#ifdef WINDOWS
	return "data/windows/settings.cfg";
#endif

}








#endif
