/***************************************************************
 * Name:      physgamelauncherApp.h
 * Purpose:   Defines Application Class
 * Author:    kirk (hydrowolfy@gmail.com)
 * Created:   2009-12-28
 * Copyright: kirk (http://www.assuredts.com/physgame/wiki/doku.php)
 * License: gpl 3?
 **************************************************************/

#ifndef PHYSGAMELAUNCHERAPP_H
#define PHYSGAMELAUNCHERAPP_H

#include <wx/app.h>
#include "zzip.h"
class physgamelauncherApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // PHYSGAMELAUNCHERAPP_H
