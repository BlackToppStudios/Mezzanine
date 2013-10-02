
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
#ifndef _worldnode_cpp
#define _worldnode_cpp

#include "worldnode.h"
#include "Graphics/scenemanager.h"
#include "exception.h"
#include "Graphics/camera.h"
#include "actorbase.h"
#include "stringtool.h"
#include "entresol.h"

#include <Ogre.h>

#include <memory>

/// @file worldnode.cpp
/// @brief The implementation of the WorldNode, A class to facilitate navigation in 3d environment


namespace Mezzanine
{
    WorldNode::WorldNode(const String& Name, Graphics::SceneManager* SManager)
        : AutoTrackTarget(NULL),
          Manager(SManager),
          FixedYaw(false),
          NodeName(Name)
    {
    }

    WorldNode::~WorldNode()
    {
        if(AutoTrackTarget)
        {
            Manager->_UnRegisterTrackingNode(this);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Basic Data

    ConstString& WorldNode::GetName() const
        { return NodeName; }

    ///////////////////////////////////////////////////////////////////////////////
    // Navigation

    void WorldNode::LookAt(const Vector3& LookAt, const Mezzanine::TransformSpace& TS, const Vector3& LocalDirection)
    {
        Vector3 Origin;
        switch(TS)
        {
        default:
        case Mezzanine::TS_World:
            Origin = GetLocation();
            break;
        case Mezzanine::TS_Parent:
            Origin = GetLocalLocation();
            break;
        case Mezzanine::TS_Local:
            Origin.Zero();
            break;
        }

        SetDirection(LookAt - Origin,TS,LocalDirection);
    }

    void WorldNode::SetDirection(const Vector3& Direction, const Mezzanine::TransformSpace& TS, const Vector3& LocalAxis)
    {
        static const Vector3 Zero(0,0,0);
        if(Direction == Zero)
            return;

        Vector3 NormalizedDir = Direction.GetNormal();
        switch(TS)
        {
            default:
            case Mezzanine::TS_World:
            {
                // Do nothing
                break;
            }
            case Mezzanine::TS_Local:
            {
                NormalizedDir = GetOrientation() * NormalizedDir;
                break;
            }
            case Mezzanine::TS_Parent:
            {
                if(Parent) NormalizedDir = Parent->GetOrientation() * NormalizedDir;
                else return;  /// @todo May want to change this to an exception, maybe.
                break;
            }
        }

        Quaternion FinalOrientation;
        if(FixedYaw)
        {
            Vector3 XVec = FixedYawAxis.CrossProduct(NormalizedDir);
            XVec.Normalize();
            Vector3 YVec = NormalizedDir.CrossProduct(XVec);
            YVec.Normalize();
            Quaternion ZToTarget(XVec,YVec,NormalizedDir);

            if(LocalAxis == Vector3::Neg_Unit_Z())
            {
                FinalOrientation.SetValues(-ZToTarget.Y,-ZToTarget.Z,ZToTarget.W,ZToTarget.X);
            }else{
                FinalOrientation = ZToTarget * (LocalAxis.GetRotationToAxis(Vector3::Unit_Z()));
            }
        }else{
            Quaternion CurrOri = GetOrientation();
            Vector3 CurrDir = CurrOri * LocalAxis;
            if( (CurrDir+NormalizedDir).SquaredLength() < 0.00005 )
            {
                FinalOrientation.SetValues(-CurrOri.Y,-CurrOri.Z,CurrOri.W,CurrOri.X);
            }else{
                FinalOrientation = (CurrDir.GetRotationToAxis(NormalizedDir)) * CurrOri;
            }
        }

        SetOrientation(FinalOrientation);
    }

    void WorldNode::SetAutoTracking(AttachableBase* Target, const Vector3& LocalDirection, const Vector3& Offset)
    {
        bool Toggle = (!AutoTrackTarget && Target) || (AutoTrackTarget && !Target);
        AutoTrackTarget = Target;
        if(AutoTrackTarget)
        {
            AutoTrackDirection = LocalDirection;
            AutoTrackOffset = Offset;
        }
        if(Toggle)
        {
            if(AutoTrackTarget) Manager->_RegisterTrackingNode(this);
            else Manager->_UnRegisterTrackingNode(this);
        }
    }

    AttachableBase* WorldNode::GetAutoTrackingTarget() const
    {
        return AutoTrackTarget;
    }

    const Vector3& WorldNode::GetAutoTrackingDirection() const
    {
        return AutoTrackDirection;
    }

    const Vector3& WorldNode::GetAutoTrackingOffset() const
    {
        return AutoTrackOffset;
    }

    void WorldNode::Rotate(const Quaternion& Rotation, const Mezzanine::TransformSpace& TS)
    {
        Quaternion NormRot = Rotation.GetNormalizedCopy();
        switch(TS)
        {
            case Mezzanine::TS_World:
                SetLocalOrientation(NormRot * GetLocalOrientation());
                break;
            case Mezzanine::TS_Parent:// This...I don't even...
                SetLocalOrientation(GetLocalOrientation() * GetOrientation().GetInverse() * NormRot * GetOrientation());
                break;
            default:
            case Mezzanine::TS_Local:
                SetLocalOrientation(GetLocalOrientation() * NormRot);
                break;
        }
    }

    void WorldNode::Rotate(const Real& Angle, const Vector3& Axis, const Mezzanine::TransformSpace& TS)
    {
        Quaternion Rot(Angle,Axis);
        Rotate(Rot,TS);
    }

    void WorldNode::Yaw(const Real& Angle, const Mezzanine::TransformSpace& TS)
    {
        if(FixedYaw) Rotate(Angle,FixedYawAxis,TS);
        else Rotate(Angle,Vector3::Unit_Y(),TS);
    }

    void WorldNode::Pitch(const Real& Angle, const Mezzanine::TransformSpace& TS)
    {
        Rotate(Angle,Vector3::Unit_X(),TS);
    }

    void WorldNode::Roll(const Real& Angle, const Mezzanine::TransformSpace& TS)
    {
        Rotate(Angle,Vector3::Unit_Z(),TS);
    }

    void WorldNode::SetFixedYawAxis(bool Enable, const Vector3& FixedAxis)
    {
        FixedYaw = Enable;
        FixedYawAxis = FixedAxis;
    }

    bool WorldNode::GetFixedYawAxisEnabled() const
    {
        return FixedYaw;
    }

    const Vector3& WorldNode::GetFixedYawAxis() const
    {
        return FixedYawAxis;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Inherited from Attachable classes

    void WorldNode::SetLocation(const Vector3& Location)
    {
        NodeLoc = Location;
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
        _RecalculateAllChildTransforms();
    }

    Vector3 WorldNode::GetLocation() const
    {
        return NodeLoc;
    }

    void WorldNode::SetOrientation(const Quaternion& Orientation)
    {
        NodeOri = Orientation;
        LocalTransformDirty = !(GetUpdating(Parent));

        _RecalculateLocalTransform();
        _RecalculateAllChildTransforms();
    }

    Quaternion WorldNode::GetOrientation() const
    {
        return NodeOri;
    }

    void WorldNode::SetScaling(const Vector3& Scale)
    {
        NodeScale = Scale;
        LocalTransformDirty = true;

        _RecalculateLocalTransform();
        _RecalculateAllChildTransforms();
    }

    Vector3 WorldNode::GetScaling() const
    {
        return NodeScale;
    }

    WorldAndSceneObjectType WorldNode::GetType() const
    {
        return Mezzanine::WSO_WorldNode;
    }

    void WorldNode::SetLocalLocation(const Vector3& Location)
    {
        LocalXform.Location = Location;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
        _RecalculateAllChildTransforms();
    }

    void WorldNode::SetLocalOrientation(const Quaternion& Orientation)
    {
        LocalXform.Rotation = Orientation;
        GlobalTransformDirty = true;

        _RecalculateGlobalTransform();
        _RecalculateAllChildTransforms();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void WorldNode::_UpdateTracking()
    {
        if(AutoTrackTarget)
        {
            LookAt(AutoTrackTarget->GetLocation()+AutoTrackOffset,Mezzanine::TS_World,AutoTrackDirection);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::WorldNode& Ev)
{
    stream      << "<WorldNode Version=\"1\" Name=\"" << Ev.GetName()
                << "\" AttachedTo=\"" << ( Ev.GetParent() ? Ev.GetParent()->GetName() : "" )
                << "\">";

                for(Mezzanine::AttachableParent::ConstAttachableIterator Iter = Ev.BeginChild(); Iter!=Ev.EndChild(); ++Iter)
                {
                    stream << "<Attached Name=\"" << (*Iter)->GetName() << "\"/>";
                }

    stream      << "<Orientation>" << Ev.GetOrientation() << "</Orientation>"
                << "<Location>" << Ev.GetLocation() << "</Location>"
                << "</WorldNode>";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::WorldNode& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "WorldNode", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::XML::Node& operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::WorldNode& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("WorldNode") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Mezzanine::WorldNode * AttachPtr = Mezzanine::Entresol::GetSingletonPtr()->GetSceneManager()->GetNode( OneNode.GetAttribute("AttachedTo").AsString() );
            if (AttachPtr)
                { AttachPtr->AttachObject(&Ev); }

            Mezzanine::Vector3 TempVec(0,0,0);
            Mezzanine::Quaternion TempQuat(0,0,0,0);
            for(Mezzanine::XML::Node Child = OneNode.GetFirstChild(); Child!=0; Child = Child.GetNextSibling())
            {
                Mezzanine::String Name(Child.Name());
                switch(Name[0])
                {
                    case 'A':
                        if(Name==Mezzanine::String("Attached"))
                        {
                            Mezzanine::String AttributeName(OneNode.GetAttribute("Name").AsString());
                            Mezzanine::WorldNode * AttachPtr = Mezzanine::Entresol::GetSingletonPtr()->GetSceneManager()->GetNode( Child.GetAttribute("Name").AsString() );
                            /// @todo This doesn't account for other objects aside from world nodes to be attached.  Additional checks that fetch other types by name in the case of a null pointer should be implemented.

                            if (AttachPtr)  // fail silently, because if we don't find it then that means it just hasn't been deserialized yeat
                            {
                                if (0==AttachPtr->GetParent())// unless it is attached to something else
                                {
                                    Ev.AttachObject(AttachPtr);
                                }else{
                                    MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_STATE_EXCEPTION,"Cannot reconcile WorldNode with the current state of the world: Attachable \"" + AttachPtr->GetName() + "\" needs to be attached, but is already attached to " + AttachPtr->GetParent()->GetName() + ".");
                                }
                            }

                        }else{
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldNode: Includes unknown Element D-\"" + Name + "\".");
                        }
                        break;
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
                            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldNode: Includes unknown Element L-\"" + Name + "\".");
                        }
                        break;
                    default:
                        { MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldNode: Includes unknown Element default-\"" + Name + "\"."); }
                        break;
                }
            }
        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for WorldNode: Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a WorldNode, found a " + Mezzanine::String(OneNode.Name()) + ".");
    }

}



#endif
