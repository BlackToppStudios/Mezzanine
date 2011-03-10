#ifndef _callbacks_cpp
#define _callbacks_cpp

#include "callbacks.h"
#include "catchapp.h"

bool CPostRender()
{
    return CatchApp::GetCatchAppPointer()->PostRender();
}

bool CPrePhysics()
{
    return CatchApp::GetCatchAppPointer()->PrePhysics();
}

bool CPostPhysics()
{
    return CatchApp::GetCatchAppPointer()->PostPhysics();
}

bool CPreUI()
{
    return CatchApp::GetCatchAppPointer()->PreUI();
}

bool CPostUI()
{
    return CatchApp::GetCatchAppPointer()->PostUI();
}

bool CPreInput()
{
    return CatchApp::GetCatchAppPointer()->PreInput();
}

bool CPostInput()
{
    return CatchApp::GetCatchAppPointer()->PostInput();
}

#endif
