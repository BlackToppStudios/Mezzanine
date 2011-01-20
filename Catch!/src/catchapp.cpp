#ifndef _catchapp_cpp
#define _catchapp_cpp

#include "catchapp.h"
#include <cassert>

using namespace phys;

CatchApp* CatchApp::TheRealCatchApp = 0;

CatchApp::CatchApp(const Vector3 &WorldLowerBounds, const Vector3 &WorldUpperBounds, SceneManager::SceneManagerType SceneType, const unsigned short int &MaxPhysicsProxies)
    : CurrScore(0),
      LastActorThrown(NULL)
{
    assert(0==CatchApp::TheRealCatchApp);
    CatchApp::TheRealCatchApp = this;

    try
    {
        GameWorld = new World( WorldLowerBounds, WorldUpperBounds, SceneType, MaxPhysicsProxies);
    }catch( exception x){
        //could not create world
    }
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
