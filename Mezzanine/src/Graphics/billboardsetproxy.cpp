// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
    /// @brief Converts an Ogre BillboardType enum value to it's corresponding Mezzanine type.
    /// @param Type The Ogre type to be converted.
    /// @return Returns the Mezzanine BillboardType corresponding to the provided Ogre type.
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
    /// @brief Converts a Mezzanine BillboardType enum value to it's corresponding Ogre type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the Ogre BillboardType corresponding to the provided Mezzanine type.
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

        void BillboardSetProxy::SetAutoExtend(const Bool AutoExtend)
            { this->GraphicsBillboardSet->setAutoextend(AutoExtend); }

        Bool BillboardSetProxy::GetAutoExtend() const
            { return this->GraphicsBillboardSet->getAutoextend(); }

        void BillboardSetProxy::SetAccurateFacing(const Bool AccFace)
            { this->GraphicsBillboardSet->setUseAccurateFacing(AccFace); }

        Bool BillboardSetProxy::GetAccurateFacing() const
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

        }

        void BillboardSetProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {

        }

        void BillboardSetProxy::ProtoSerializeBillboards(XML::Node& SelfRoot) const
        {

        }

        void BillboardSetProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {

        }

        void BillboardSetProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {

        }

        void BillboardSetProxy::ProtoDeSerializeBillboards(const XML::Node& SelfRoot)
        {

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
