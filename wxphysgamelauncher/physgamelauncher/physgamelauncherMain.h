/***************************************************************
 * Name:      physgamelauncherMain.h
 * Purpose:   Defines Application Frame
 * Author:    kirk (hydrowolfy@gmail.com)
 * Created:   2009-12-28
 * Copyright: kirk (http://www.assuredts.com/physgame/wiki/doku.php)
 * License:
 **************************************************************/

#ifndef PHYSGAMELAUNCHERMAIN_H
#define PHYSGAMELAUNCHERMAIN_H

//(*Headers(physgamelauncherFrame)
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class physgamelauncherFrame: public wxFrame
{
    public:

        physgamelauncherFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~physgamelauncherFrame();

    private:

        //(*Handlers(physgamelauncherFrame)
        void OnQuit(wxCommandEvent& event);
        void RunGame(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        //*)

        //(*Identifiers(physgamelauncherFrame)
        static const long RUN_GAME;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(physgamelauncherFrame)
        wxMenuItem* MenuItem3;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PHYSGAMELAUNCHERMAIN_H
