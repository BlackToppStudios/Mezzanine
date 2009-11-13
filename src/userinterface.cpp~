//This is where the user interface will reside.

#ifndef _userinterface_cpp
#define _userinterface_cpp

#include <wx/wx.h>
#include "gamebase.h"

using namespace std;

char GameName[]="Physgame";

class MyApp: public wxApp
{
	virtual bool OnInit();
};

class MyFrame: public wxFrame
{
	public:

	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnNewGame(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

enum
{
	ID_Quit = 1,
 	ID_About = 2,
	ID_NewGame = 3,
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Quit, MyFrame::OnQuit)
EVT_MENU(ID_About, MyFrame::OnAbout)
EVT_MENU(ID_NewGame, MyFrame::OnNewGame)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp) 

bool MyApp::OnInit()
{
	SetAppName( _T("Physgame"));
	SetVendorName( _T("Some Guys"));
	
	MyFrame *frame = new MyFrame( _T("Physgame"), wxPoint(50,50), wxSize(450,340) );
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
} 

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame((wxFrame *)NULL, -1, title, pos, size)
{	
	wxMenu *menuGame = new wxMenu;
	wxMenu *menuHelp = new wxMenu;
	
	menuGame->Append( ID_NewGame, _T("&New Game") );
	menuGame->AppendSeparator();
	menuGame->Append( ID_Quit, _T("E&xit") );

	menuHelp->Append( ID_About, _T("&About...") );

	wxMenuBar *menuBar = new wxMenuBar;
	
	menuBar->Append( menuGame, _T("&Game") );
	menuBar->Append( menuHelp, _T("&Help") );

	SetMenuBar( menuBar );
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_T("Physgame version 0.01"), _T("About Physgame"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnNewGame(wxCommandEvent& WXUNUSED(event))
{
	GameStart(this);
}

#endif
