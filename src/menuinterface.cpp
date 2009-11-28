#ifndef _menuinterface_cpp
#define _menuinterface_cpp
///////////////////////////////////////////////////////////////////////////////
//This is where the user interface will reside.

///////////////////////////////////////////////////////////////////////////////
// Additional includes
#include <wx/wx.h>
#include "gamebase.h"
#include "menuinterface.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Menu messages
// 1/5 enumarating game ui message, to help keep them sorted
enum
{
	ID_Quit = 1,
 	ID_About = 2,
	ID_NewGame = 3,
};

//This where we state which functions deal with each message
// 2/5  ties messages to a functions
BEGIN_EVENT_TABLE(UiFrame, wxFrame)
EVT_MENU(ID_Quit, UiFrame::OnQuit)
EVT_MENU(ID_About, UiFrame::OnAbout)
EVT_MENU(ID_NewGame, UiFrame::OnNewGame)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
//This is the main loop, this accepts a pointer to the wxwidget application
IMPLEMENT_APP(PhysApp)

///////////////////////////////////////////////////////////////////////////////
// Start the wxwidget applicaiton
bool PhysApp::OnInit()
{
	SetAppName( _T("Physgame"));
	SetVendorName( _T("Some Guys"));

	UiFrame *frame = new UiFrame( _T("Physgame"), wxPoint(50,50), wxSize(450,340));
	frame->SetParentApp(this);

	frame->Show(TRUE);
	SetTopWindow(frame);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// builds a wxuiframe a the designated coordinates
// 3/5 for UI messages, they are initiated here
UiFrame::UiFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame((wxFrame *)NULL, -1, title, pos, size)
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

void UiFrame::SetParentApp(wxApp* ParentApp_)
{
	this->ParentApp=ParentApp_;
}

void UiFrame::Yield()
{
	this->ParentApp->Yield();
}

///////////////////////////////////////////////////////////////////////////////
// These functions are called when the UI throws a command message
//4/5 create the UI function for the message don't forget to declare any new
//Functions in the header
void UiFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void UiFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_T("Physgame version 0.01"), _T("About Physgame"), wxOK | wxICON_INFORMATION, this);
}

void UiFrame::OnNewGame(wxCommandEvent& WXUNUSED(event))
{
	StartGame(this);
}

#endif
