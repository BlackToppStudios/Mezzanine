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
//#include <windows.h>
#include "zlib.h"
#include <sys/stat.h>


#include <iostream>
#include <fstream>
#include <string>
#include "configopener.h"
#include <stdio.h>
#include <stdlib.h>
//#include "unzip.h"
//(*InternalHeaders(physgamelauncherFrame)
#include <wx/bitmap.h>
#include <wx/settings.h>
#include <wx/intl.h>
#include <wx/image.h>
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
const long physgamelauncherFrame::ID_STATICBITMAP1 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON2 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON3 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON1 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON4 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON5 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON6 = wxNewId();
const long physgamelauncherFrame::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(physgamelauncherFrame,wxFrame)
    //(*EventTable(physgamelauncherFrame)
    //*)

END_EVENT_TABLE()

physgamelauncherFrame::physgamelauncherFrame(wxWindow* parent,wxWindowID id)
{

    //(*Initialize(physgamelauncherFrame)
    wxGridSizer* GridSizer4;
    wxGridSizer* GridSizer15;
    wxGridSizer* GridSizer10;
    wxGridSizer* GridSizer13;
    wxGridSizer* GridSizer11;
    wxGridSizer* GridSizer5;
    wxGridSizer* GridSizer7;
    wxGridSizer* GridSizer8;
    wxGridSizer* GridSizer16;
    wxGridSizer* GridSizer9;
    wxGridSizer* GridSizer6;
    wxGridSizer* GridSizer1;
    wxGridSizer* GridSizer3;
    wxGridSizer* GridSizer14;
    wxGridSizer* GridSizer12;
    wxFlexGridSizer* FlexGridSizer1;
    wxGridSizer* GridSizer2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(921,628));
    SetForegroundColour(wxColour(255,255,255));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(200,136), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    GridSizer1 = new wxGridSizer(0, 3, 0, 0);
    GridSizer3 = new wxGridSizer(0, 3, 0, 0);
    GridSizer1->Add(GridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBitmap1 = new wxStaticBitmap(Panel1, ID_STATICBITMAP1, wxBitmap(wxImage(_T("C:\\Users\\hydrowolfy\\Desktop\\New Bitmap Image.bmp")).Rescale(wxSize(433,238).GetWidth(),wxSize(433,238).GetHeight())), wxDefaultPosition, wxSize(433,238), 0, _T("ID_STATICBITMAP1"));
    GridSizer1->Add(StaticBitmap1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer12 = new wxGridSizer(0, 3, 0, 0);
    GridSizer1->Add(GridSizer12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer11 = new wxGridSizer(0, 3, 0, 0);
    GridSizer1->Add(GridSizer11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer5 = new wxGridSizer(0, 3, 0, 0);
    GridSizer9 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Continue"), wxDefaultPosition, wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    GridSizer5->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    GridSizer5->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer7 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("New Campiagn"), wxDefaultPosition, wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    GridSizer5->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer8 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer6 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Standalone levels"), wxDefaultPosition, wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    GridSizer5->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button4 = new wxButton(Panel1, ID_BUTTON4, _("Load"), wxDefaultPosition, wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    GridSizer5->Add(Button4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer13 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer10 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button5 = new wxButton(Panel1, ID_BUTTON5, _("Options"), wxDefaultPosition, wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    GridSizer5->Add(Button5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer16 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer15 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button6 = new wxButton(Panel1, ID_BUTTON6, _("Quit"), wxDefaultPosition, wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    GridSizer5->Add(Button6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer14 = new wxGridSizer(0, 3, 0, 0);
    GridSizer5->Add(GridSizer14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(GridSizer5, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    Panel1->SetSizer(GridSizer1);
    GridSizer1->Fit(Panel1);
    GridSizer1->SetSizeHints(Panel1);

    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
    //*)
           wxFrame::ShowFullScreen(true, wxFULLSCREEN_ALL);

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

    wxString wxexe = wxString::FromAscii(Exe.c_str());
    if (FileExists(Exe))
    {
        wxExecute(wxexe);
    }
    else
    {
        //insert popup here saying that the launcher could not find the proper exe here
    }

    z_stream strm;

//   zzip_dir_open steddddd;

//     ZZIP_File;


}

void physgamelauncherFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void physgamelauncherFrame::OnButton1Click(wxCommandEvent& event)
{
}

bool physgamelauncherFrame::FileExists(string strFilename)
{
    struct stat stFileInfo;
    bool blnReturn;
    int intStat;

    // Attempt to get the file attributes
    intStat = stat(strFilename.c_str(),&stFileInfo);
    if (intStat == 0)
    {
        // We were able to get the file attributes
        // so the file obviously exists.
        blnReturn = true;
    }
    else
    {
        // We were not able to get the file attributes.
        blnReturn = false;
    }

    return(blnReturn);
}


void physgamelauncherFrame::OnButton3Click(wxCommandEvent& event)
{
}
