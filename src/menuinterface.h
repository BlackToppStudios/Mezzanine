#ifndef _menuinterface_h
#define _menuinterface_h

///////////////////////////////////////////////////////////////////////////////
// This is where the Menu interface definition exists


///////////////////////////////////////////////////////////////////////////////
// Includes

#include "gamebase.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// The definition for the user interface backing, this is the object that
//wxwidgets will use to handle the user interface main loop
class PhysApp: public wxApp
{
	virtual bool OnInit();
};

///////////////////////////////////////////////////////////////////////////////
// This is the declaration for the main frame for the menu. Not the user
//interface for the game, but rather the place were players will setup games
class UiFrame: public wxFrame
{
	private:
		wxApp* ParentApp;

	public:
		UiFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

		//Sets which application will has ownership of this frame
		void SetParentApp(wxApp* ParentApp_);
		//pauses execution to allow frame redraws
		void Yield();


///////////////////////////////////////////////////////////////////////////////
//		Messaging functions
//these functions are called when the user interface triggers certain events
//such as clicking a button or selecting a menu
//5/5 declare your message accepting function
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnNewGame(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
