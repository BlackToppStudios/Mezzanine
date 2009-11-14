//This is where all the gamewide features will go
//this file is also where all dirty hacks shall reside.
#ifndef _gamebase_cpp
#define _gamebase_cpp

#include <wx/wx.h>
#include "gamebase.h"
#include "physworld.h"
#include "Ogre.h"

bool GameStart(wxFrame* GameWindow)
{
	int Width=0, Height=0;
	wxString Conv;
	wxRect Dim = GameWindow->GetRect();

	//GameWindow->Hide();

	GameWindow->GetClientSize(&Width, &Height);
	Conv << _T("Client Height:") << Height << _T(" Client Width:") << Width << _T("\nGetTop:") << Dim.GetTop() << _T(" GetLeft:") << Dim.GetLeft() << _T(" GetWidth:") << Dim.GetWidth() << _T(" GetHeight:") << Dim.GetHeight();
	wxMessageBox(Conv , _T("About Physgame"), wxOK | wxICON_INFORMATION, GameWindow);

	physworld TestOne;
	TestOne.GameInit();

	//GameWindow->Show();

	return true;
}

#endif
