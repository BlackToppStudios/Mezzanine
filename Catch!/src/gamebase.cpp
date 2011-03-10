#ifndef _gamebase_cpp
#define _gamebase_cpp
///////////////////////////////////////////////////////////////////////////////
// Gamewide Logic misc Features go here
///////////////////////////////////////////////////////////////////////////////

#include "gamebase.h"       //Game Include
#include "catchapp.h"

int main(int argc, char **argv)
{
    CatchApp* Catch = new CatchApp(Vector3(-1000.0,-1000.0,-1000.0), Vector3(1000.0,1000.0,1000.0), SceneManager::Generic, 50);
    Catch->GetCatchin();
    delete Catch;
	return 0;
}

#endif
