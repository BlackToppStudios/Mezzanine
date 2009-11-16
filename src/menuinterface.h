//This is the Menu interface definition

#ifndef _menuinterface_h
#define _menuinterface_h

#include "gamebase.h"

using namespace std;

//The definition for the user interface backing
class PhysApp: public wxApp
{
	virtual bool OnInit();
};

class UiFrame: public wxFrame
{
	private:
		wxApp* ParentApp;

	public:
		UiFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
		void SetParentApp(wxApp* ParentApp_);
		void Yield();

		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnNewGame(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
