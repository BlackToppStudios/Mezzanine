#ifndef _gamebase_h
#define _gamebase_h

//Callbacks AKA Functions the mainloops call
bool PreInput();
bool PostInput();
bool PrePhysics();
bool PostPhysics();
bool PostRender();
bool PreUI();
bool PostUI();

//Functions that protocatch uses
bool CheckForStuff();
void LoadContent();
void MakeGUI();
void TestsToSave();

#endif
