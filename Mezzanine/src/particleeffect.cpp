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
#ifndef _particleeffect_cpp
#define _particleeffect_cpp

#include "particleeffect.h"
#include "scenemanager.h"
#include "stringtool.h"
#include "worldnode.h"
#include "entresol.h"

#include "particleaffector.h"
#include "particleemitter.h"

#include <memory>

#include <Ogre.h>

/// @file particleeffect.cpp
/// @brief Contains the implemenation of the ParticleEffect class

namespace Mezzanine
{
    namespace Internal
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
            /// @brief Stores the template, primarily for serializatino
            String Template;

            /// @internal
            /// @brief Stores the current speed factor of this particle system for when it gets paused.
            Real SpeedCache;

            /// @internal
            /// @brief Functionally, this constructs the whole ParticleEffect
            ParticleEffectInternalData(SceneManager* manager, Ogre::ParticleSystem* System)
            {
                Template = "";
                SpeedCache = 0.0;
                OgreParticle = System;
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
        };//ParticleEffectInternalData
    }//internal

    ///////////////////////////////////////////////////////////////////////////////
    // Construction

    ParticleEffect::ParticleEffect(const String& Name, const String& Template, SceneManager* manager)
    {
        this->Pie = new Internal::ParticleEffectInternalData(manager, manager->GetGraphicsWorldPointer()->createParticleSystem(Name, Template));
        this->Pie->Template = Template;

        UInt16 NumEmitters = Pie->OgreParticle->getNumEmitters();
        for( UInt16 X = 0 ; X < NumEmitters ; ++X )
        {
            ParticleEmitter* NewEmitter = new ParticleEmitter(Pie->OgreParticle->getEmitter(X));
            Emitters.push_back(NewEmitter);
        }

        UInt16 NumAffectors = Pie->OgreParticle->getNumAffectors();
        for( UInt16 X = 0 ; X < NumAffectors ; ++X )
        {
            ParticleAffector* NewAffector = new ParticleAffector(Pie->OgreParticle->getAffector(X));
            Affectors.push_back(NewAffector);
        }
    }

    ParticleEffect::ParticleEffect(Ogre::ParticleSystem* System, const String& Template, SceneManager* manager)
    {
        this->Pie = new Internal::ParticleEffectInternalData(manager, System);
        this->Pie->Template = Template;

        UInt16 NumEmitters = Pie->OgreParticle->getNumEmitters();
        for( UInt16 X = 0 ; X < NumEmitters ; ++X )
        {
            ParticleEmitter* NewEmitter = new ParticleEmitter(Pie->OgreParticle->getEmitter(X));
            Emitters.push_back(NewEmitter);
        }

        UInt16 NumAffectors = Pie->OgreParticle->getNumAffectors();
        for( UInt16 X = 0 ; X < NumAffectors ; ++X )
        {
            ParticleAffector* NewAffector = new ParticleAffector(Pie->OgreParticle->getAffector(X));
            Affectors.push_back(NewAffector);
        }
    }

    ParticleEffect::~ParticleEffect()
        { delete this->Pie; }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited From AttachableChild

    ConstString& ParticleEffect::GetName() const
        { return this->Pie->OgreParticle->getName(); }

    WorldAndSceneObjectType ParticleEffect::GetType() const
        { return Mezzanine::WSO_ParticleEffect; }

    void ParticleEffect::SetLocation(const Vector3& Location)
    {
        this->Pie->OgreNode->setPosition(Location.GetOgreVector3());
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
    }

    Vector3 ParticleEffect::GetLocation() const
        { return Vector3(this->Pie->OgreNode->getPosition()); }

    void ParticleEffect::SetOrientation(const Quaternion& Orientation)
    {
        this->Pie->OgreNode->setOrientation(Orientation.GetOgreQuaternion());
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
    }

    Quaternion ParticleEffect::GetOrientation() const
        { return Quaternion(this->Pie->OgreNode->getOrientation()); }

    void ParticleEffect::SetScaling(const Vector3& Scale)
        { this->Pie->OgreNode->setScale(Scale.GetOgreVector3()); }

    Vector3 ParticleEffect::GetScaling() const
        { return Vector3(this->Pie->OgreNode->getScale()); }

    void ParticleEffect::SetLocalLocation(const Vector3& Location)
    {
        LocalXform.Location = Location;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
    }

    void ParticleEffect::SetLocalOrientation(const Quaternion& Orientation)
    {
        LocalXform.Rotation = Orientation;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Emitters

    ParticleEmitter* ParticleEffect::GetEmitter(const UInt16& Index) const
    {
        return Emitters.at(Index);
    }

    UInt16 ParticleEffect::GetNumEmitters() const
    {
        return Pie->OgreParticle->getNumEmitters();
    }

    void ParticleEffect::DestroyEmitter(const UInt16& Index)
    {
        std::vector<ParticleEmitter*>::iterator it = Emitters.begin() + Index;
        delete (*it);
        Emitters.erase(it);
        Pie->OgreParticle->removeEmitter(Index);
    }

    void ParticleEffect::DestroyAllEmitters()
    {
        for( std::vector<ParticleEmitter*>::iterator it = Emitters.begin() ; it != Emitters.end() ; ++it )
            delete (*it);
        Emitters.clear();
        Pie->OgreParticle->removeAllEmitters();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Affectors

    ParticleAffector* ParticleEffect::GetAffector(const UInt16& Index) const
    {
        return Affectors.at(Index);
    }

    UInt16 ParticleEffect::GetNumAffectors() const
    {
        return Pie->OgreParticle->getNumAffectors();
    }

    void ParticleEffect::DestroyAffector(const UInt16& Index)
    {
        std::vector<ParticleAffector*>::iterator it = Affectors.begin() + Index;
        delete (*it);
        Affectors.erase(it);
        Pie->OgreParticle->removeAffector(Index);
    }

    void ParticleEffect::DestroyAllAffectors()
    {
        for( std::vector<ParticleAffector*>::iterator it = Affectors.begin() ; it != Affectors.end() ; ++it )
            delete (*it);
        Affectors.clear();
        Pie->OgreParticle->removeAllAffectors();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Other Particle Functionality

    void ParticleEffect::AddToWorld()
    {
        if(!IsInWorld())
            { this->Pie->OgreNode->attachObject(this->Pie->OgreParticle); }
    }

    void ParticleEffect::RemoveFromWorld()
    {
        if(IsInWorld())
            { this->Pie->OgreNode->detachObject(this->Pie->OgreParticle); }
    }

    bool ParticleEffect::IsInWorld() const
        { return this->Pie->OgreParticle->getParentSceneNode() == this->Pie->OgreNode; }

    void ParticleEffect::SetSpeedFactor(const Real& Factor)
    {
        if(this->Pie->SpeedCache) this->Pie->SpeedCache = Factor;
        else this->Pie->OgreParticle->setSpeedFactor(Factor);
    }

    Real ParticleEffect::GetSpeedFactor()
    {
        if(this->Pie->SpeedCache) return this->Pie->SpeedCache;
        else return this->Pie->OgreParticle->getSpeedFactor();
    }

    void ParticleEffect::PauseParticleEffect(bool Pause)
    {
        if(Pause && !this->Pie->SpeedCache)
        {
            this->Pie->SpeedCache = this->Pie->OgreParticle->getSpeedFactor();
            this->Pie->OgreParticle->setSpeedFactor(0.0);
        }
        else if(!Pause && this->Pie->SpeedCache)
        {
            this->Pie->OgreParticle->setSpeedFactor(this->Pie->SpeedCache);
            this->Pie->SpeedCache = 0.0;
        }
    }

    ConstString& ParticleEffect::GetTemplate() const
        { return this->Pie->Template; }

    void ParticleEffect::SetCustomParam(const String& Name, const String& Value)
        { this->Pie->OgreParticle->setParameter(Name,Value); }

    String ParticleEffect::GetCustomParam(const String& Name) const
        { return this->Pie->OgreParticle->getParameter(Name); }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

std::ostream& operator << (std::ostream& stream, const Mezzanine::ParticleEffect& Ev)
{
    stream      << "<ParticleEffect Version=\"1\" Name=\"" << Ev.GetName()
                    << "\" AttachedTo=\"" << ( Ev.GetParent() ? Ev.GetParent()->GetName() : "" )
                    << "\" InWorld=\"" << Ev.IsInWorld()
                    << "\" Template=\"" << Ev.GetTemplate()
                << "\">"
                << "<Orientation>" << Ev.GetOrientation() << "</Orientation>"
                << "<Location>" << Ev.GetLocation() << "</Location>"
                << "</ParticleEffect>";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ParticleEffect& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "ParticleEffect", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::ParticleEffect& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("ParticleEffect") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            if(OneNode.GetAttribute("InWorld").AsBool())
            {
                Ev.AddToWorld();
            }else{
                Ev.RemoveFromWorld();
            }

            Mezzanine::WorldNode * AttachPtr = Mezzanine::Entresol::GetSingletonPtr()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            Mezzanine::Quaternion TempQuat(0,0,0,0);
            Mezzanine::Vector3 TempVec(0,0,0);

            for(Mezzanine::XML::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                Mezzanine::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'O':   //Orientation
                        if(Name==Mezzanine::String("Orientation"))
                        {
                            Child.GetFirstChild() >> TempQuat;
                            Ev.SetOrientation(TempQuat);
                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldNode: Includes unknown Element D-\"" + Name + "\".");
                        }
                        break;
                    case 'L':   //Location
                        if(Name==Mezzanine::String("Location"))
                        {
                            Child.GetFirstChild() >> TempVec;
                            Ev.SetLocation(TempVec);
                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for ParticleEffect: Includes unknown Element L-\"" + Name + "\".");
                        }
                        break;
                    default:
                        { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for ParticleEffect: Includes unknown Element default-\"" + Name + "\"."); }
                        break;
                }
            }

        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for ParticleEffect: Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a ParticleEffect, found a " + Mezzanine::String(OneNode.Name()));
    }
}

#endif
