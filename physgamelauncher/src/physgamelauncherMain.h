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
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)

using namespace std;



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
        bool FileExists(string strFilename);
        void OnButton3Click(wxCommandEvent& event);
        void levelselectscreen();
        void Options();
        void fullscreen(bool fullscreencheck){


        //*)

        //(*Identifiers(physgamelauncherFrame)
        static const long ID_STATICBITMAP1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON1;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_PANEL1;
        //*)

        //(*Declarations(physgamelauncherFrame)
        wxButton* Button4;
        wxButton* Button1;
        wxStaticBitmap* StaticBitmap1;
        wxPanel* Panel1;
        wxButton* Button2;
        wxButton* Button6;
        wxButton* Button5;
        wxButton* Button3;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // PHYSGAMELAUNCHERMAIN_H
