#include "wx/wx.h" 
#include "gamebase.cpp"

#include <iostream>

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

	DECLARE_EVENT_TABLE()
};

enum
{
	ID_Quit = 1,
	ID_About,
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Quit, MyFrame::OnQuit)
EVT_MENU(ID_About, MyFrame::OnAbout)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp) 

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame( _T("Physgame"), wxPoint(50,50), wxSize(450,340) );
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
} 

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;

	menuFile->Append( ID_About, _T("&About...") );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Quit, _T("E&xit") );

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, _T("&Game") );

	SetMenuBar( menuBar );

	CreateStatusBar();
	SetStatusText( _T("Physgame") );
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(_T("Physgame version 0.01"),
		_T("About Physgame"), wxOK | wxICON_INFORMATION, this);
}

