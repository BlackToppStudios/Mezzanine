//© Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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

#ifndef _actorbase_cpp
#define _actorbase_cpp

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include <sstream>
#include <memory>

#include "actorbase.h"
#include "actorrigid.h"
#include "actorsoft.h"
#include "actorphysicssettings.h"
#include "serialization.h"
#include "Audio/audiomanager.h"
#include "Audio/soundset.h"
#include "physicsmanager.h"
#include "scenemanager.h"
#include "entresol.h"
#include "XML/xml.h"
#include "Internal/motionstate.h.cpp"
#include "Internal/meshtools.h.cpp"

/// @file
/// @brief Code used by all actors is implemented here.

namespace Mezzanine
{
    ///////////////////////////////////
    // ActorBase class fuctions
    ActorBase::ActorBase()
        : MotionState(NULL),
          Animation(NULL)
    {
    }

    ActorBase::~ActorBase()
    {
        SceneManager* SceneMan = SceneManager::GetSingletonPtr();
        SceneMan->GetGraphicsWorldPointer()->destroyEntity(GraphicsObject);
        delete MotionState;
    }

    ///////////////////////////////////
    // Utility and Configuration

    void ActorBase::SetAnimation(ConstString& AnimationName, bool Loop)
    {
        if(this->IsAnimated())
        {
            Animation->setEnabled(false);
        }
        Animation = GraphicsObject->getAnimationState(AnimationName);
        Animation->setLoop(Loop);
    }

    void ActorBase::EnableAnimation(bool Enable)
    {
        if(Animation)
        {
            Animation->setEnabled(Enable);
        }
    }

    bool ActorBase::IsAnimated() const
    {
        if(Animation)
        {
            return Animation->getEnabled();
        }else{
            return false;
        }
    }

    void ActorBase::AdvanceAnimation(const Real& Time)
    {
        if(Animation)
        {
            Animation->addTime(Time);
        }
    }

    void ActorBase::RemoveSetAnimation()
    {
        if(Animation)
        {
            Animation->setEnabled(false);
            Animation = NULL;
        }
    }

///////////////////////////////////////////////////////////////////////////////
// Serialization
///////////////////////////////////////
    void ActorBase::ThrowSerialError(const String& Fail) const
        { SerializeError(Fail, SerializableName()); }

    void ActorBase::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        XML::Node ActorBaseNode = CurrentRoot.AppendChild("ActorBase");
        if (!ActorBaseNode) { ThrowSerialError("create ActorBaseNode");}

        XML::Attribute ActorVersion = ActorBaseNode.AppendAttribute("Version");
        ActorVersion.SetValue(1);

        NonStaticWorldObject::ProtoSerialize(ActorBaseNode);
    }

    void ActorBase::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==this->ActorBase::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                NonStaticWorldObject::ProtoDeSerialize(OneNode.GetChild(this->NonStaticWorldObject::SerializableName()));
            }
        }
    }

    String ActorBase::SerializableName()
        { return String("ActorBase"); }

}// /mezz



#endif
