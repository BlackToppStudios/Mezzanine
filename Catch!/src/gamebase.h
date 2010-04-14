#ifndef _gamebase_h
#define _gamebase_h
///////////////////////////////////////////////////////////////////////////////
// This is where all the gamewide features will go. If it has no better home
//Somewhere else we will stuff it here until it is time to refactor again.
///////////////////////////////////////////////////////////////////////////////


//Callbacks AKA Functions the mainloops call
bool PreInput();
bool PostInput();

bool PrePhysics();

bool PostRender();


//Functions that protocatch uses
bool CheckForEsc();

void LoadContent();

#endif
