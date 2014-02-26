// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _graphicsbillboard_cpp
#define _graphicsbillboard_cpp

/// @file
/// @brief This file contains the implementation for the Billboard class used by BillboardSetProxy.

#include "Graphics/billboard.h"

#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        Billboard::Billboard(Ogre::Billboard* InternalBillboard) :
            GraphicsBillboard(InternalBillboard)
            {  }

        Billboard::~Billboard()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole Billboard::HasOwnDimensions() const
            { return this->GraphicsBillboard->hasOwnDimensions(); }

        void Billboard::ResetDimensions()
            { this->GraphicsBillboard->resetDimensions(); }

        Boole Billboard::IsUsingTextureCoordRect() const
            { return this->GraphicsBillboard->isUseTexcoordRect(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Billboard Properties

        void Billboard::SetLocation(const Vector3& Loc)
            { this->GraphicsBillboard->setPosition( Loc.GetOgreVector3() ); }

        Vector3 Billboard::GetLocation() const
            { return Vector3( this->GraphicsBillboard->getPosition() ); }

        void Billboard::SetDirection(const Vector3& Dir)
            { this->GraphicsBillboard->mDirection = Dir.GetOgreVector3(); }

        Vector3 Billboard::GetDirection() const
            { return Vector3( this->GraphicsBillboard->mDirection ); }

        void Billboard::SetRotation(const Real Rot)
            { this->GraphicsBillboard->setRotation( Ogre::Radian(Rot) ); }

        Real Billboard::GetRotation() const
            { return this->GraphicsBillboard->getRotation().valueRadians(); }

        void Billboard::SetColour(const ColourValue& Colour)
            { this->GraphicsBillboard->setColour( Colour.GetOgreColourValue() ); }

        ColourValue Billboard::GetColour() const
            { return ColourValue( this->GraphicsBillboard->getColour() ); }

        void Billboard::SetDimensions(const Real Width, const Real Height)
            { this->GraphicsBillboard->setDimensions(Width,Height); }

        Real Billboard::GetOwnWidth() const
            { return this->GraphicsBillboard->getOwnWidth(); }

        Real Billboard::GetOwnHeight() const
            { return this->GraphicsBillboard->getOwnHeight(); }

        void Billboard::SetTextureCoordRect(const Real Left, const Real Top, const Real Right, const Real Bottom)
            { this->GraphicsBillboard->setTexcoordRect(Left,Top,Right,Bottom); }

        Real Billboard::GetLeftTextureCoord() const
            { return this->GraphicsBillboard->getTexcoordRect().left; }

        Real Billboard::GetTopTextureCoord() const
            { return this->GraphicsBillboard->getTexcoordRect().top; }

        Real Billboard::GetRightTextureCoord() const
            { return this->GraphicsBillboard->getTexcoordRect().right; }

        Real Billboard::GetBottomTextureCoord() const
            { return this->GraphicsBillboard->getTexcoordRect().bottom; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Billboard::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = SelfRoot.AppendChild( Billboard::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("Rotation").SetValue( this->GetRotation() ) )
            {
                if( this->HasOwnDimensions() ) {
                    if( !( SelfRoot.AppendAttribute("OwnWidth").SetValue( this->GetOwnWidth() ) &&
                           SelfRoot.AppendAttribute("OwnHeight").SetValue( this->GetOwnHeight() ) ) )
                    {
                        SerializeError("Create XML Attribute Values",Billboard::GetSerializableName(),true);
                    }
                }

                if( this->IsUsingTextureCoordRect() ) {
                    if( !( SelfRoot.AppendAttribute("LeftTexCoord").SetValue( this->GetLeftTextureCoord() ) &&
                           SelfRoot.AppendAttribute("TopTexCoord").SetValue( this->GetTopTextureCoord() ) &&
                           SelfRoot.AppendAttribute("RightTexCoord").SetValue( this->GetRightTextureCoord() ) &&
                           SelfRoot.AppendAttribute("BottomTexCoord").SetValue( this->GetBottomTextureCoord() ) ) )
                    {
                        SerializeError("Create XML Attribute Values",Billboard::GetSerializableName(),true);
                    }
                }

                XML::Node LocationNode = SelfRoot.AppendChild("Location");
                this->GetLocation().ProtoSerialize( LocationNode );
                XML::Node DirectionNode = SelfRoot.AppendChild("Direction");
                this->GetDirection().ProtoSerialize( DirectionNode );
                XML::Node ColourNode = SelfRoot.AppendChild("Colour");
                this->GetColour().ProtoSerialize( ColourNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",Billboard::GetSerializableName(),true);
            }
        }

        void Billboard::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;

            if( String( SelfRoot.Name() ) == Billboard::GetSerializableName() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    Real OwnWidth = -1.0, OwnHeight = -1.0;
                    Real LeftTexCoord = -1.0, TopTexCoord = -1.0, RightTexCoord = -1.0, BottomTexCoord = -1.0;

                    CurrAttrib = SelfRoot.GetAttribute("Rotation");
                    if( !CurrAttrib.Empty() )
                        this->SetRotation( CurrAttrib.AsReal() );

                    CurrAttrib = SelfRoot.GetAttribute("OwnWidth");
                    if( !CurrAttrib.Empty() )
                        OwnWidth = CurrAttrib.AsReal();

                    CurrAttrib = SelfRoot.GetAttribute("OwnHeight");
                    if( !CurrAttrib.Empty() )
                        OwnHeight = CurrAttrib.AsReal();

                    if( OwnWidth >= 0 && OwnHeight >= 0 ) {
                        this->SetDimensions(OwnWidth,OwnHeight);
                    }

                    CurrAttrib = SelfRoot.GetAttribute("LeftTexCoord");
                    if( !CurrAttrib.Empty() )
                        LeftTexCoord = CurrAttrib.AsReal();

                    CurrAttrib = SelfRoot.GetAttribute("TopTexCoord");
                    if( !CurrAttrib.Empty() )
                        TopTexCoord = CurrAttrib.AsReal();

                    CurrAttrib = SelfRoot.GetAttribute("RightTexCoord");
                    if( !CurrAttrib.Empty() )
                        RightTexCoord = CurrAttrib.AsReal();

                    CurrAttrib = SelfRoot.GetAttribute("BottomTexCoord");
                    if( !CurrAttrib.Empty() )
                        BottomTexCoord = CurrAttrib.AsReal();

                    if( LeftTexCoord >= 0 && TopTexCoord >= 0 && RightTexCoord >= 0 && BottomTexCoord >= 0 ) {
                        this->SetTextureCoordRect(LeftTexCoord,TopTexCoord,RightTexCoord,BottomTexCoord);
                    }

                    XML::Node LocationNode = SelfRoot.GetChild("Location").GetFirstChild();
                    if( !LocationNode.Empty() ) {
                        Vector3 Loc(LocationNode);
                        this->SetLocation(Loc);
                    }

                    XML::Node DirectionNode = SelfRoot.GetChild("Direction").GetFirstChild();
                    if( !DirectionNode.Empty() ) {
                        Vector3 Dir(DirectionNode);
                        this->SetDirection(Dir);
                    }

                    XML::Node ColourNode = SelfRoot.GetChild("Colour").GetFirstChild();
                    if( !ColourNode.Empty() ) {
                        ColourValue Colour(ColourNode);
                        this->SetColour(Colour);
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Billboard::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Billboard::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String Billboard::GetSerializableName()
            { return "Billboard"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Billboard* Billboard::_GetGraphicsObject() const
            { return this->GraphicsBillboard; }
    }//Graphics
}//Mezzanine

#endif
