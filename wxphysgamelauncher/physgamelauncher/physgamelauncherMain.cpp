/***************************************************************
 * Name:      physgamelauncherMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    kirk (hydrowolfy@gmail.com)
 * Created:   2009-12-28
 * Copyright: kirk (http://www.assuredts.com/physgame/wiki/doku.php)
 * License:
 **************************************************************/

#include "physgamelauncherMain.h"
#include <wx/msgdlg.h>


#include <iostream>
#include <fstream>
#include <string>
#include "configopener.h"
#include <stdio.h>
#include <stdlib.h>

//(*InternalHeaders(physgamelauncherFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions



enum wxbuildinfoformat
{
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(physgamelauncherFrame)
const long physgamelauncherFrame::RUN_GAME = wxNewId();
const long physgamelauncherFrame::idMenuQuit = wxNewId();
const long physgamelauncherFrame::idMenuAbout = wxNewId();
const long physgamelauncherFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(physgamelauncherFrame,wxFrame)
    //(*EventTable(physgamelauncherFrame)
    //*)
END_EVENT_TABLE()

physgamelauncherFrame::physgamelauncherFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(physgamelauncherFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu1, RUN_GAME, _("Run\tAlt-R"), _("Runs the program"), wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(RUN_GAME,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&physgamelauncherFrame::RunGame);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&physgamelauncherFrame::OnAbout);
    //*)
}

physgamelauncherFrame::~physgamelauncherFrame()
{
    //(*Destroy(physgamelauncherFrame)
    //*)
}

void physgamelauncherFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void physgamelauncherFrame::RunGame(wxCommandEvent& event)
{
    ConfigOpener RunExe;
    string Exe;
    Exe = RunExe.GetExeWindows();
    //system ("cmd");
    ofstream test;
    test.open("blah.txt");
    test << Exe;
    system(Exe.c_str());


}

void physgamelauncherFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void physgamelauncherFrame::OnButton1Click(wxCommandEvent& event)
{
}
