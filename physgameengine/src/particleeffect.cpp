//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _particleeffect_cpp
#define _particleeffect_cpp

#include "particleeffect.h"
#include "scenemanager.h"
#include "world.h"
#include "worldnode.h"


#include <Ogre.h>

/// @file particleeffect.cpp
/// @brief Contains the implemenation of the ParticleEffect class

namespace phys
{
    namespace internal
    {
        /// @internal
        /// @brief used to store internal data and functions for the ParticleEffect;
        struct ParticleEffectInternalData
        {
            /// @internal
            /// @brief Pointer to the ogre ParticleSystem from which this class gets it's functionality.
            Ogre::ParticleSystem* OgreParticle;

            /// @internal
            /// @brief Pointer to the ogre Scenenode to which this object is attached.
            Ogre::SceneNode* OgreNode;

            /// @internal
            /// @brief Pointer to the manager that created this class.
            SceneManager* Manager;

            /// @internal
            /// @brief Functionally, this constructs the whole ParticleEffect
            ParticleEffectInternalData(SceneManager* manager, Ogre::ParticleSystem* System)
            {
                OgreParticle=System;
                this->Manager = manager;
                Ogre::SceneManager* OgreManager = Manager->GetGraphicsWorldPointer();
                OgreNode = OgreManager->createSceneNode();
                OgreManager->getRootSceneNode()->addChild(OgreNode);
            }

            /// @internal
            /// @brief Cleans up after the Particle Effect.
            ~ParticleEffectInternalData()
            {
                Manager->GetGraphicsWorldPointer()->destroyParticleSystem(OgreParticle);
                Manager->GetGraphicsWorldPointer()->destroySceneNode(OgreNode);
            }
        };
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Construction

    ParticleEffect::ParticleEffect(const String& Name, const String& Template, SceneManager* manager)
        { this->Pie = new internal::ParticleEffectInternalData(manager, manager->GetGraphicsWorldPointer()->createParticleSystem(Name, Template)); }

    ParticleEffect::ParticleEffect(Ogre::ParticleSystem* System, SceneManager* manager)
        { this->Pie = new internal::ParticleEffectInternalData(manager, System); }

    ParticleEffect::~ParticleEffect()
        { delete this->Pie; }

    ///////////////////////////////////////////////////////////////////////////////
    /// Inherited From Attachable

    ConstString& ParticleEffect::GetName() const
        { return this->Pie->OgreParticle->getName(); }

    Attachable::AttachableElement ParticleEffect::GetAttachableType() const
        { return Attachable::ParticleEffect; }

    void ParticleEffect::AttachToFinal(Ogre::SceneNode* RawTarget, phys::WorldNode* Target)
    {
        Attachable::AttachToFinal(RawTarget, Target);
        if(this->Pie->OgreNode->getParent())
            { this->Pie->OgreNode->getParentSceneNode()->removeChild(this->Pie->OgreNode); }
        RawTarget->addChild(this->Pie->OgreNode);
    }

    void ParticleEffect::DetachFromFinal(Ogre::SceneNode* RawTarget)
    {
        Attachable::DetachFromFinal(RawTarget);
        RawTarget->removeChild(this->Pie->OgreNode);
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// Particle Functionality
    void ParticleEffect::EnableParticleEffect()
    {
        if(!IsEnabled())
            { this->Pie->OgreNode->attachObject(this->Pie->OgreParticle); }
    }

    void ParticleEffect::DisableParticleEffect()
    {
        if(IsEnabled())
            { this->Pie->OgreNode->detachObject(this->Pie->OgreParticle); }
    }

    bool ParticleEffect::IsEnabled() const
        { return this->Pie->OgreNode->numAttachedObjects(); }

    void ParticleEffect::SetLocation(const Vector3& Vec)
        { this->Pie->OgreNode->setPosition(Vec.GetOgreVector3()); }

    Vector3 ParticleEffect::GetLocation() const
        { return Vector3(this->Pie->OgreNode->getPosition()); }

    void ParticleEffect::SetOrientation(Quaternion Orientation)
        { this->Pie->OgreNode->setOrientation(Orientation.GetOgreQuaternion()); }

    Quaternion ParticleEffect::GetOrientation() const
        { return Quaternion(this->Pie->OgreNode->getOrientation()); }

}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ParticleEffect& Ev)
{
    stream      << "<ParticleEffect Version=\"1\" Name=\"" << Ev.GetName()
                    << "\" AttachedTo=\"" << ( Ev.GetAttachedTo() ? Ev.GetAttachedTo()->GetName() : "" )
                    << "\" Enabled=\"" << Ev.IsEnabled()
                << "\">"
                << "<Orientation>" << Ev.GetOrientation() << "</Orientation>"
                << "<Location>" << Ev.GetLocation() << "</Location>"
                << "</ParticleEffect>";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ParticleEffect& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ParticleEffect", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ParticleEffect& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("ParticleEffect") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            if(OneNode.GetAttribute("Enabled").AsBool())
            {
                Ev.EnableParticleEffect();
            }else{
                Ev.DisableParticleEffect();
            }

            phys::WorldNode * AttachPtr = phys::World::GetWorldPointer()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            phys::Quaternion TempQuat(0,0,0,0);
            phys::Vector3 TempVec(0,0,0);

            for(phys::xml::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                phys::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'O':   //Orientation
                        if(Name==phys::String("Orientation"))
                        {
                            Child.GetFirstChild() >> TempQuat;
                            Ev.SetOrientation(TempQuat);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for WorldNode: Includes unknown Element D-\"",Name,"\"")) );
                        }
                        break;
                    case 'L':   //Location
                        if(Name==phys::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            throw( phys::Exception(phys::StringCat("Incompatible XML Version for ParticleEffect: Includes unknown Element L-\"",Name,"\"")) );
                        }
                        break;
                    default:
                        throw( phys::Exception(phys::StringCat("Incompatible XML Version for ParticleEffect: Includes unknown Element default-\"",Name,"\"")) );
                        break;
                }
            }

        }else{
            throw( phys::Exception("Incompatible XML Version for ParticleEffect: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ParticleEffect, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML

#endif
