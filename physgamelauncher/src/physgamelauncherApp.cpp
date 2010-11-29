/***************************************************************
 * Name:      physgamelauncherApp.cpp
 * Purpose:   Code for Application Class
 * Author:    kirk (hydrowolfy@gmail.com)
 * Created:   2009-12-28
 * Copyright: kirk (http://www.assuredts.com/physgame/wiki/doku.php)
 * License:
 **************************************************************/

#include "physgamelauncherApp.h"
#include <iostream>
#include <fstream>
//#include "zlib.h"
#include "crossplatform.h"

//(*AppHeaders
#include "physgamelauncherMain.h"
//#include "levelselect.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(physgamelauncherApp);




bool physgamelauncherApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {

    	physgamelauncherFrame* Frame = new physgamelauncherFrame(0);
    	//levelselect* Frame2 = new levelselect(0);
    	Frame->Show();
    	//Frame2->Show();
    	SetTopWindow(Frame);
    	//SetTopWindow(Frame2);
    }
    //*)
    return wxsOK;

}
