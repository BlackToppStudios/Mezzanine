#ifndef _gamebase_h
#define _gamebase_h

//Callbacks AKA Functions the mainloops call
bool PreInput();
bool PostInput();
bool PrePhysics();
bool PostRender();

//Functions that protocatch uses
bool CheckForEsc();
void LoadContent();

#endif
