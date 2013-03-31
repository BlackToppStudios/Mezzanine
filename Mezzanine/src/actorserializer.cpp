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
#ifndef _actorserializer_cpp
#define _actorserializer_cpp

#include "actorserializer.h"

namespace Mezzanine
{
    ActorRigid* ActorRigidDeSerializer::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==ActorRigid::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                String Name(OneNode.GetAttribute("Name").AsString());
                if (Name.empty())
                    { DeSerializeError("find a Name Attribute",ActorRigid::SerializableName()); }

                String File(OneNode.GetAttribute("File").AsString());
                if (File.empty())
                    { DeSerializeError("find a File Attribute",ActorRigid::SerializableName()); }

                String Group(OneNode.GetAttribute("Group").AsString());
                if (Group.empty())
                    { DeSerializeError("find a Group Attribute",ActorRigid::SerializableName()); }

                ActorRigid *Results = new ActorRigid(10,Name,File,Group);
                Results->ProtoDeSerialize(OneNode);

                if (0 != Target)
                    { Target->AddActor(Results); }

                if( Results->IsInWorld() != OneNode.GetChild(ActorBase::SerializableName()).GetAttribute("IsInWorld").AsBool() )
                {
                    if(Results->IsInWorld())
                        { Results->RemoveFromWorld(); }
                    else
                        { Results->AddToWorld(); }
                }

                /// @todo TODO: Why is gravity done here, can it be done elsewhere? Apparently the addind to the world removed what is deserialized here and this should how be used if it is different from the default

                Vector3 tempvec;
                //[rename, /RigidActors[1]/ActorRigid[1]/ActorBase[1]/NonStaticWorldObject[1]/WorldObject[1]/ActorRigidPhysicsSettings[1]/IndividualGravity[1]/Vector3[1]/@X, LogilabXmldiffTmpAttrZ]

                XML::Node GravityNode = OneNode.GetChild(ActorBase::SerializableName()).GetChild("NonStaticWorldObject").GetChild("WorldObject").GetChild("ActorRigidPhysicsSettings").GetChild("IndividualGravity").GetFirstChild();
                if(!GravityNode)
                    { DeSerializeError("find gravity node", "ActorRigidDeSerializer"); }
                tempvec.ProtoDeSerialize(GravityNode);
                Results->GetPhysicsSettings()->SetIndividualGravity(tempvec);

                return Results;

            }else{
                DeSerializeError("find usable serialization version","ActorRigidDeSerializer");
            }
        }else{
            DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),"ActorRigidDeSerializer");
        }
    }

    String ActorRigidDeSerializer::ContainerName() const
        { return String("RigidActors"); }

}

#endif
