#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include "catchapp.h"

int main(int argc, char **argv)
{
	// Temporary Hack
    #ifdef MEZZ_MACOSX
	String ExeDir = Mezzanine::Resource::GetExecutableDirFromArg(argc,argv);
	Mezzanine::Resource::ChangeWorkingDirectory(ExeDir);
    #endif

    CatchApp* Catch = new CatchApp();
    Catch->GetCatchin();
    delete Catch;
	return 0;
}

#endif
