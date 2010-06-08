//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _worldquerytool_cpp
#define _worldquerytool_cpp

#include "worldquerytool.h"

#include <Ogre.h>

namespace phys
{
    WorldQueryTool::WorldQueryTool(World* GameWorld_)
    {
        this->GameWorld = GameWorld_;
        this->MouseButtonCache.reset();
        this->KeyboardButtonCache.reset();
        this->MouseXCache=0;
        this->MouseYCache=0;

        // create the ray scene query object
        this->RayQuery = this->GameWorld->OgreSceneManager->createRayQuery(Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
        if (NULL == this->RayQuery)
            {this->GameWorld->LogAndThrow("Failed to create RaySceneQuery instance in WorldQueryTool"); }
        this->RayQuery->setSortByDistance(true);
    }

    WorldQueryTool::~WorldQueryTool()
    {
        delete this->RayQuery;
    }

    Whole WorldQueryTool::GetMouseX()
        {return this->MouseXCache;}

    Whole WorldQueryTool::GetMouseY()
        {return this->MouseYCache;}

    bool WorldQueryTool::IsMouseButtonPushed(short unsigned int MouseButton)
    {
        if(MouseButton >= this->MouseButtonLimit || MouseButton<0)
            {this->GameWorld->LogAndThrow("Unsupported mouse button access through WorldQueryTool");}
        return this->MouseButtonCache[MouseButton];
    }

    bool WorldQueryTool::IsKeyboardButtonPushed(MetaCode::InputCode KeyboardButton)
        {return this->KeyboardButtonCache[KeyboardButton];}

}



#endif
