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
#ifndef _graphicsbillboardsetproxy_cpp
#define _graphicsbillboardsetproxy_cpp

/// @file
/// @brief This file contains the implementation for the World proxy wrapping billboard functionality.

#include "Graphics/billboardsetproxy.h"
#include "Graphics/scenemanager.h"
#include "Graphics/billboard.h"

#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace
{
    /// @internal
    /// @brief Converts an Ogre BillboardOrigin enum value to it's corresponding Mezzanine value.
    /// @param Origin The Ogre value to be converted.
    /// @return Returns the Mezzanine BillboardOrigin corresponding to the provided Ogre value.
    Mezzanine::Graphics::BillboardOrigin ConvertBillboardOrigin(const Ogre::BillboardOrigin Origin)
    {
        switch(Origin)
        {
            case Ogre::BBO_TOP_LEFT:       return Mezzanine::Graphics::BBO_Top_Left;       break;
            case Ogre::BBO_TOP_CENTER:     return Mezzanine::Graphics::BBO_Top_Center;     break;
            case Ogre::BBO_TOP_RIGHT:      return Mezzanine::Graphics::BBO_Top_Right;      break;
            case Ogre::BBO_CENTER_LEFT:    return Mezzanine::Graphics::BBO_Center_Left;    break;
            case Ogre::BBO_CENTER:         return Mezzanine::Graphics::BBO_Center;         break;
            case Ogre::BBO_CENTER_RIGHT:   return Mezzanine::Graphics::BBO_Center_Right;   break;
            case Ogre::BBO_BOTTOM_LEFT:    return Mezzanine::Graphics::BBO_Bottom_Left;    break;
            case Ogre::BBO_BOTTOM_CENTER:  return Mezzanine::Graphics::BBO_Bottom_Center;  break;
            case Ogre::BBO_BOTTOM_RIGHT:   return Mezzanine::Graphics::BBO_Bottom_Right;   break;
        }
        return Mezzanine::Graphics::BBO_Center;
    }
    /// @internal
    /// @brief Converts a Mezzanine BillboardOrigin enum value to it's corresponding Ogre value.
    /// @param Origin The Mezzanine value to be converted.
    /// @return Returns the Ogre BillboardOrigin corresponding to the provided Mezzanine value.
    Ogre::BillboardOrigin ConvertBillboardOrigin(const Mezzanine::Graphics::BillboardOrigin Origin)
    {
        switch(Origin)
        {
            case Mezzanine::Graphics::BBO_Top_Left:       return Ogre::BBO_TOP_LEFT;       break;
            case Mezzanine::Graphics::BBO_Top_Center:     return Ogre::BBO_TOP_CENTER;     break;
            case Mezzanine::Graphics::BBO_Top_Right:      return Ogre::BBO_TOP_RIGHT;      break;
            case Mezzanine::Graphics::BBO_Center_Left:    return Ogre::BBO_CENTER_LEFT;    break;
            case Mezzanine::Graphics::BBO_Center:         return Ogre::BBO_CENTER;         break;
            case Mezzanine::Graphics::BBO_Center_Right:   return Ogre::BBO_CENTER_RIGHT;   break;
            case Mezzanine::Graphics::BBO_Bottom_Left:    return Ogre::BBO_BOTTOM_LEFT;    break;
            case Mezzanine::Graphics::BBO_Bottom_Center:  return Ogre::BBO_BOTTOM_CENTER;  break;
            case Mezzanine::Graphics::BBO_Bottom_Right:   return Ogre::BBO_BOTTOM_RIGHT;   break;
        }
        return Ogre::BBO_CENTER;
    }

    /// @internal
    /// @brief Converts an Ogre BillboardRotation enum value to it's corresponding Mezzanine type.
    /// @param Rotation The Ogre type to be converted.
    /// @return Returns the Mezzanine BillboardRotation corresponding to the provided Ogre type.
    Mezzanine::Graphics::BillboardRotation ConvertBillboardRotation(const Ogre::BillboardRotationType Rotation)
    {
        switch(Rotation)
        {
            case Ogre::BBR_VERTEX:    return Mezzanine::Graphics::BBR_Vertex;       break;
            case Ogre::BBR_TEXCOORD:  return Mezzanine::Graphics::BBR_TexureCoord;  break;
        }
        return Mezzanine::Graphics::BBR_TexureCoord;
    }
    /// @internal
    /// @brief Converts a Mezzanine BillboardRotation enum value to it's corresponding Ogre type.
    /// @param Rotation The Mezzanine type to be converted.
    /// @return Returns the Ogre BillboardRotation corresponding to the provided Mezzanine type.
    Ogre::BillboardRotationType ConvertBillboardRotation(const Mezzanine::Graphics::BillboardRotation Rotation)
    {
        switch(Rotation)
        {
            case Mezzanine::Graphics::BBR_Vertex:       return Ogre::BBR_VERTEX;    break;
            case Mezzanine::Graphics::BBR_TexureCoord:  return Ogre::BBR_TEXCOORD;  break;
        }
        return Ogre::BBR_TEXCOORD;
    }

    /// @internal
    /// @brief Converts an Ogre BillboardType enum value to it's corresponding Mezzanine type.
    /// @param Type The Ogre type to be converted.
    /// @return Returns the Mezzanine BillboardType corresponding to the provided Ogre type.
    Mezzanine::Graphics::BillboardType ConvertBillboardType(const Ogre::BillboardType Type)
    {
        switch(Type)
        {
            case Ogre::BBT_POINT:                 return Mezzanine::Graphics::BBT_Point;                 break;
            case Ogre::BBT_ORIENTED_COMMON:       return Mezzanine::Graphics::BBT_Oriented_Common;       break;
            case Ogre::BBT_ORIENTED_SELF:         return Mezzanine::Graphics::BBT_Oriented_Self;         break;
            case Ogre::BBT_PERPENDICULAR_COMMON:  return Mezzanine::Graphics::BBT_Perpendicular_Common;  break;
            case Ogre::BBT_PERPENDICULAR_SELF:    return Mezzanine::Graphics::BBT_Perpendicular_Self;    break;
        }
        return Mezzanine::Graphics::BBT_Point;
    }
    /// @internal
    /// @brief Converts a Mezzanine BillboardType enum value to it's corresponding Ogre type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the Ogre BillboardType corresponding to the provided Mezzanine type.
    Ogre::BillboardType ConvertBillboardType(const Mezzanine::Graphics::BillboardType Type)
    {
        switch(Type)
        {
            case Mezzanine::Graphics::BBT_Point:                 return Ogre::BBT_POINT;                 break;
            case Mezzanine::Graphics::BBT_Oriented_Common:       return Ogre::BBT_ORIENTED_COMMON;       break;
            case Mezzanine::Graphics::BBT_Oriented_Self:         return Ogre::BBT_ORIENTED_SELF;         break;
            case Mezzanine::Graphics::BBT_Perpendicular_Common:  return Ogre::BBT_PERPENDICULAR_COMMON;  break;
            case Mezzanine::Graphics::BBT_Perpendicular_Self:    return Ogre::BBT_PERPENDICULAR_SELF;    break;
        }
        return Ogre::BBT_POINT;
    }
}

namespace Mezzanine
{
    namespace Graphics
    {
        BillboardSetProxy::BillboardSetProxy(const UInt32 InitialPoolSize, SceneManager* Creator) :
            RenderableProxy(Creator)
            { this->CreateBillboardSet(InitialPoolSize); }

        BillboardSetProxy::BillboardSetProxy(const XML::Node& SelfRoot, SceneManager* Creator) :
            RenderableProxy(Creator)
        {
            this->CreateBillboardSet(20);
            this->ProtoDeSerialize(SelfRoot);
        }

        BillboardSetProxy::~BillboardSetProxy()
            { this->DestroyBillboardSet(); }

        void BillboardSetProxy::CreateBillboardSet(const UInt32 InitialPoolSize)
        {
            this->GraphicsBillboardSet = this->Manager->_GetGraphicsWorldPointer()->createBillboardSet(InitialPoolSize);
            this->GraphicsNode->attachObject( this->GraphicsBillboardSet );
            this->GraphicsBillboardSet->MovableObject::setUserAny( Ogre::Any( static_cast<RenderableProxy*>( this ) ) );
            this->GraphicsBillboardSet->setVisibilityFlags(0);
            this->GraphicsBillboardSet->setQueryFlags(0);
        }

        void BillboardSetProxy::DestroyBillboardSet()
        {
            if( this->GraphicsBillboardSet ) {
                this->GraphicsNode->detachObject( this->GraphicsBillboardSet );
                this->Manager->_GetGraphicsWorldPointer()->destroyBillboardSet( this->GraphicsBillboardSet );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType BillboardSetProxy::GetProxyType() const
        {
            return Mezzanine::PT_Graphics_BillboardSetProxy;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Billboard Management

        Billboard* BillboardSetProxy::CreateBillboard(const Vector3& Location, const ColourValue& Colour)
        {
            Billboard* NewBB = new Billboard( this->GraphicsBillboardSet->createBillboard(Location.GetOgreVector3(),Colour.GetOgreColourValue()) );
            this->Billboards.push_back(NewBB);
            return NewBB;
        }

        Billboard* BillboardSetProxy::GetBillboard(UInt32 Index)
        {
            BillboardIterator Ret = this->Billboards.begin();
            if( Index == 0 )
                return *Ret;

            while( Index-- )
                ++Ret;

            return *Ret;
        }

        UInt32 BillboardSetProxy::GetNumBillboards() const
        {
            return this->Billboards.size();
        }

        void BillboardSetProxy::DestroyBillboard(Billboard* ToBeDestroyed)
        {
            this->GraphicsBillboardSet->removeBillboard( ToBeDestroyed->_GetGraphicsObject() );
            for( BillboardIterator BillIt = this->Billboards.begin() ; BillIt != this->Billboards.end() ; ++BillIt )
            {
                if( (*BillIt) == ToBeDestroyed )
                {
                    delete (*BillIt);
                    this->Billboards.erase(BillIt);
                    return;
                }
            }
        }

        void BillboardSetProxy::DestroyAllBillboards()
        {
            for( BillboardIterator BillIt = this->Billboards.begin() ; BillIt != this->Billboards.end() ; ++BillIt )
            {
                delete (*BillIt);
            }
            this->Billboards.clear();
            this->GraphicsBillboardSet->clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // BillboardSet Properties

        void BillboardSetProxy::SetAutoExtend(const Boole AutoExtend)
            { this->GraphicsBillboardSet->setAutoextend(AutoExtend); }

        Boole BillboardSetProxy::GetAutoExtend() const
            { return this->GraphicsBillboardSet->getAutoextend(); }

        void BillboardSetProxy::SetAccurateFacing(const Boole AccFace)
            { this->GraphicsBillboardSet->setUseAccurateFacing(AccFace); }

        Boole BillboardSetProxy::GetAccurateFacing() const
            { return this->GraphicsBillboardSet->getUseAccurateFacing(); }

        void BillboardSetProxy::SetPoolSize(const UInt32 Size)
            { this->GraphicsBillboardSet->setPoolSize(Size); }

        UInt32 BillboardSetProxy::GetPoolSize() const
            { return this->GraphicsBillboardSet->getPoolSize(); }

        void BillboardSetProxy::SetBillboardOrigin(const Graphics::BillboardOrigin Origin)
            { this->GraphicsBillboardSet->setBillboardOrigin( ConvertBillboardOrigin(Origin) ); }

        Graphics::BillboardOrigin BillboardSetProxy::GetBillboardOrigin() const
            { return ConvertBillboardOrigin( this->GraphicsBillboardSet->getBillboardOrigin() ); }

        void BillboardSetProxy::SetBillboardRotation(const Graphics::BillboardRotation Rotation)
            { this->GraphicsBillboardSet->setBillboardRotationType( ConvertBillboardRotation(Rotation) ); }

        Graphics::BillboardRotation BillboardSetProxy::GetBillboardRotation() const
            { return ConvertBillboardRotation( this->GraphicsBillboardSet->getBillboardRotationType() ); }

        void BillboardSetProxy::SetBillboardType(const Graphics::BillboardType Type)
            { this->GraphicsBillboardSet->setBillboardType( ConvertBillboardType(Type) ); }

        Graphics::BillboardType BillboardSetProxy::GetBillboardType() const
            { return ConvertBillboardType( this->GraphicsBillboardSet->getBillboardType() ); }

        void BillboardSetProxy::SetDefaultDimensions(const Real Width, const Real Height)
            { this->GraphicsBillboardSet->setDefaultDimensions(Width,Height); }

        void BillboardSetProxy::SetDefaultWidth(const Real Width)
            { this->GraphicsBillboardSet->setDefaultWidth(Width); }

        Real BillboardSetProxy::GetDefaultWidth() const
            { return this->GraphicsBillboardSet->getDefaultWidth(); }

        void BillboardSetProxy::SetDefaultHeight(const Real Height)
            { this->GraphicsBillboardSet->setDefaultHeight(Height); }

        Real BillboardSetProxy::GetDefaultHeight() const
            { return this->GraphicsBillboardSet->getDefaultHeight(); }

        void BillboardSetProxy::SetCommonDirection(const Vector3& Dir)
            { this->GraphicsBillboardSet->setCommonDirection( Dir.GetOgreVector3() ); }

        Vector3 BillboardSetProxy::GetCommonDirection() const
            { return Vector3( this->GraphicsBillboardSet->getCommonDirection() ); }

        void BillboardSetProxy::SetCommonUpVector(const Vector3& UpDir)
            { this->GraphicsBillboardSet->setCommonUpVector( UpDir.GetOgreVector3() ); }

        Vector3 BillboardSetProxy::GetCommonUpVector() const
            { return Vector3( this->GraphicsBillboardSet->getCommonUpVector() ); }

        void BillboardSetProxy::SetMaterialName(const String& MatName)
            { this->GraphicsBillboardSet->setMaterialName(MatName); }

        String BillboardSetProxy::GetMaterialName() const
            { return this->GraphicsBillboardSet->getMaterialName(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void BillboardSetProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());
            if( !SelfRoot.AppendAttribute("InWorld").SetValue( this->IsInWorld() ? "true" : "false" ) ) {
                SerializeError("Create XML Attribute Values",BillboardSetProxy::GetSerializableName(),true);
            }

            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeBillboards(SelfRoot);
        }

        void BillboardSetProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( BillboardSetProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("AutoExtend").SetValue( this->GetAutoExtend() ? "true" : "false" ) &&
                PropertiesNode.AppendAttribute("AccurateFacing").SetValue( this->GetAccurateFacing() ? "true" : "false" ) &&
                PropertiesNode.AppendAttribute("PoolSize").SetValue( this->GetPoolSize() ) &&
                PropertiesNode.AppendAttribute("BillboardOrigin").SetValue( static_cast<Whole>( this->GetBillboardOrigin() ) ) &&
                PropertiesNode.AppendAttribute("BillboardRotation").SetValue( static_cast<Whole>( this->GetBillboardRotation() ) ) &&
                PropertiesNode.AppendAttribute("BillboardType").SetValue( static_cast<Whole>( this->GetBillboardType() ) ) &&
                PropertiesNode.AppendAttribute("DefaultWidth").SetValue( this->GetDefaultWidth() ) &&
                PropertiesNode.AppendAttribute("DefaultHeight").SetValue( this->GetDefaultHeight() ) &&
                PropertiesNode.AppendAttribute("MaterialName").SetValue( this->GetMaterialName() ) )
            {
                XML::Node CommonDirectionNode = PropertiesNode.AppendChild("CommonDirection");
                this->GetCommonDirection().ProtoSerialize( CommonDirectionNode );
                XML::Node CommonUpVectorNode = PropertiesNode.AppendChild("CommonUpVector");
                this->GetCommonUpVector().ProtoSerialize( CommonUpVectorNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",BillboardSetProxy::GetSerializableName() + "Properties",true);
            }
        }

        void BillboardSetProxy::ProtoSerializeBillboards(XML::Node& SelfRoot) const
        {
            XML::Node BillboardsNode = SelfRoot.AppendChild( BillboardSetProxy::GetSerializableName() + "Billboards" );

            if( BillboardsNode.AppendAttribute("Version").SetValue("1") )
            {
                for( ConstBillboardIterator BillIt = this->Billboards.begin() ; BillIt != this->Billboards.end() ; ++BillIt )
                {
                    (*BillIt)->ProtoSerialize( BillboardsNode );
                }
            }else{
                SerializeError("Create XML Attribute Values",BillboardSetProxy::GetSerializableName() + "Billboards",true);
            }
        }

        void BillboardSetProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            Boole WasInWorld = false;
            XML::Attribute InWorldAttrib = SelfRoot.GetAttribute("InWorld");
            if( !InWorldAttrib.Empty() ) {
                WasInWorld = StringTools::ConvertToBool( InWorldAttrib.AsString() );
            }

            this->DestroyAllBillboards();
            this->ProtoDeSerializeProperties(SelfRoot);
            this->ProtoDeSerializeBillboards(SelfRoot);

            if( WasInWorld ) {
                this->AddToWorld();
            }
        }

        void BillboardSetProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( BillboardSetProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("AutoExtend");
                    if( !CurrAttrib.Empty() )
                        this->SetAutoExtend( StringTools::ConvertToBool( CurrAttrib.AsString() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("AccurateFacing");
                    if( !CurrAttrib.Empty() )
                        this->SetAccurateFacing( StringTools::ConvertToBool( CurrAttrib.AsString() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("PoolSize");
                    if( !CurrAttrib.Empty() )
                        this->SetPoolSize( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("BillboardOrigin");
                    if( !CurrAttrib.Empty() )
                        this->SetBillboardOrigin( static_cast<Graphics::BillboardOrigin>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("BillboardRotation");
                    if( !CurrAttrib.Empty() )
                        this->SetBillboardRotation( static_cast<Graphics::BillboardRotation>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("BillboardType");
                    if( !CurrAttrib.Empty() )
                        this->SetBillboardType( static_cast<Graphics::BillboardType>( CurrAttrib.AsWhole() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("DefaultWidth");
                    if( !CurrAttrib.Empty() )
                        this->SetDefaultWidth( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("DefaultHeight");
                    if( !CurrAttrib.Empty() )
                        this->SetDefaultHeight( CurrAttrib.AsReal() );

                    CurrAttrib = PropertiesNode.GetAttribute("MaterialName");
                    if( !CurrAttrib.Empty() )
                        this->SetMaterialName( CurrAttrib.AsString() );

                    XML::Node CommonDirectionNode = PropertiesNode.GetChild("CommonDirection").GetFirstChild();
                    if( !CommonDirectionNode.Empty() ) {
                        Vector3 CommonDir(CommonDirectionNode);
                        this->SetCommonDirection(CommonDir);
                    }

                    XML::Node CommonUpVectorNode = PropertiesNode.GetChild("CommonUpVector").GetFirstChild();
                    if( !CommonUpVectorNode.Empty() ) {
                        Vector3 CommonUpVec(CommonUpVectorNode);
                        this->SetCommonUpVector(CommonUpVec);
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (BillboardSetProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,BillboardSetProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        void BillboardSetProxy::ProtoDeSerializeBillboards(const XML::Node& SelfRoot)
        {
            XML::Node BillboardsNode = SelfRoot.GetChild( BillboardSetProxy::GetSerializableName() + "Billboards" );

            if( !BillboardsNode.Empty() ) {
                if(BillboardsNode.GetAttribute("Version").AsInt() == 1) {
                    for( XML::NodeIterator BillIt = BillboardsNode.begin() ; BillIt != BillboardsNode.end() ; ++BillIt )
                    {
                        Billboard* NewBB = this->CreateBillboard( Vector3() );
                        NewBB->ProtoDeSerialize( (*BillIt) );
                    }
                }
            }
        }

        String BillboardSetProxy::GetDerivedSerializableName() const
            { return BillboardSetProxy::GetSerializableName(); }

        String BillboardSetProxy::GetSerializableName()
            { return "BillboardSetProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::BillboardSet* BillboardSetProxy::_GetGraphicsObject() const
            { return this->GraphicsBillboardSet; }

        Ogre::MovableObject* BillboardSetProxy::_GetBaseGraphicsObject() const
            { return this->GraphicsBillboardSet; }
    }//Graphics
}//Mezzanine

#endif
