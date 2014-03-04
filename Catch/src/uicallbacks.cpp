#ifndef _uicallbacks_cpp
#define _uicallbacks_cpp

#include "uicallbacks.h"
#include "catchapp.h"

void AllAppExit(EventArgumentsPtr Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelManager()->SetNextLevel(NULL);
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

#endif
