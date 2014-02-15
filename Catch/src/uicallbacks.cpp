#ifndef _uicallbacks_cpp
#define _uicallbacks_cpp

#include "uicallbacks.h"
#include "catchapp.h"

void AllAppExit(const EventArguments& Args)
{
    CatchApp::GetCatchAppPointer()->GetLevelLoader()->SetNextLevel("");
    Entresol::GetSingletonPtr()->BreakMainLoop();
}

#endif
