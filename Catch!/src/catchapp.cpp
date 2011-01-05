#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include <cassert>

using namespace phys;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp()
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    GameWorld = World::GetWorldPointer();
}

CatchApp::~CatchApp()
{
    CatchApp::TheRealCatchApp = NULL;
}

CatchApp* CatchApp::GetCatchAppPointer()
{
    assert(0!=CatchApp::TheRealCatchApp);
    return TheRealCatchApp;
}

#endif
