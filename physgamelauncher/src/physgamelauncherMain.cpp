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
const long physgamelauncherFrame::RUN_GAME2 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON2 = wxNewId();
const long physgamelauncherFrame::ID_BUTTON3 = wxNewId();
const long physgamelauncherFrame::idMenuQuit2 = wxNewId();
const long physgamelauncherFrame::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(physgamelauncherFrame,wxFrame)
    //(*EventTable(physgamelauncherFrame)
    //*)
END_EVENT_TABLE()

physgamelauncherFrame::physgamelauncherFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(physgamelauncherFrame)
    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(350,400));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(200,136), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Button1 = new wxButton(Panel1, RUN_GAME2, _("Play"), wxPoint(224,64), wxSize(104,28), 0, wxDefaultValidator, _T("RUN_GAME2"));
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Options"), wxPoint(224,120), wxSize(104,28), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("Load new files"), wxPoint(224,176), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Button4 = new wxButton(Panel1, idMenuQuit2, _("Quit"), wxPoint(224,232), wxSize(104,28), 0, wxDefaultValidator, _T("idMenuQuit2"));

    Connect(RUN_GAME2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::RunGame);
    Connect(idMenuQuit2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&physgamelauncherFrame::OnQuit);
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

