//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uiscreen_cpp
#define _uiscreen_cpp

#include "uiscreen.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uibutton.h"
#include "uiviewportupdatetool.h"
#include "uitextureatlas.h"
#include "viewport.h"
#include "world.h"
#include "scenemanager.h"
#include "mathtool.h"

#include <OgreRoot.h>
#include <OgreMatrix4.h>
#include <OgreRenderSystem.h>
#include <OgreRenderOperation.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreRenderQueueListener.h>

namespace Mezzanine
{
    namespace UI
    {
        /// @struct ScreenInternalData
        /// @brief Basic struct holding some of the internal bits of this class that could not be placed on the class directly.
        struct ScreenInternalData : public Ogre::RenderQueueListener
        {
            Screen* ParentScreen;
            Ogre::RenderOperation RenderOp;
            Ogre::RenderSystem* RenderSys;
            Ogre::HardwareVertexBufferSharedPtr VertexBuffer;

            void renderQueueStarted(Ogre::uint8, const Ogre::String&, bool&) {};
            void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
            {
                if( RenderSys->_getViewport() != ParentScreen->GetViewport()->GetOgreViewport() || queueGroupId != Ogre::RENDER_QUEUE_OVERLAY )
                    return;
                if( ParentScreen->IsVisible() && ParentScreen->GetNumLayers() )
                    ParentScreen->_RenderScreen();
            }
        };

        struct OgreVertex
        {
            Ogre::Vector3 Position;
            Ogre::ColourValue Colour;
            Ogre::Vector2 UV;
        };

        void ScreenVertexData::Clear()
        {
            LowVertices.clear();
            MediumVertices.clear();
            HighVertices.clear();
        }

        Whole ScreenVertexData::Size()
        {
            return LowVertices.size() + MediumVertices.size() + HighVertices.size();
        }

        VertexData& ScreenVertexData::operator[](const Whole& Index)
        {
            if( Index < LowVertices.size() )
                return LowVertices[Index];
            else if( Index < LowVertices.size() + MediumVertices.size() )
                return MediumVertices[Index - LowVertices.size()];
            else if( Index < LowVertices.size() + MediumVertices.size() + HighVertices.size() )
                return HighVertices[Index - (LowVertices.size() + MediumVertices.size())];
            else
                World::GetWorldPointer()->LogAndThrow(Exception("Out of bounds index requested in ScreenVertexData::operator[]."));
        }

        Screen::Screen(const String& name, const String& Atlas, Viewport* WindowViewport)
            : Name(name),
              PrimaryAtlas(Atlas),
              GameViewport(WindowViewport),
              Scale(Vector3(1,1,1)),
              VertexBufferSize(0),
              CanRender(true),
              Visible(true),
              IndexRedrawNeeded(false),
              IndexRedrawAll(false),
              OrientationChanged(false),
              ViewportSizeChanged(false),
              Orientation(Mezzanine::OM_Degree_0)
        {
            Manager = UIManager::GetSingletonPtr();
            SceneMan = SceneManager::GetSingletonPtr();

            KnownViewportSize.X = (Real)GameViewport->GetActualWidth();
            KnownViewportSize.Y = (Real)GameViewport->GetActualHeight();
            InverseViewportSize.X = 1 / KnownViewportSize.X;
            InverseViewportSize.Y = 1 / KnownViewportSize.Y;

            SID = new ScreenInternalData();
            SID->RenderSys = Ogre::Root::getSingletonPtr()->getRenderSystem();
            SID->ParentScreen = this;

            SceneMan->GetGraphicsWorldPointer()->addRenderQueueListener(SID);

            VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(0,0,0,1));
            CreateVertexBuffer();
        }

        Screen::~Screen()
        {
        }

        void Screen::PrepareRenderSystem()
        {
            this->SID->RenderSys->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setViewMatrix( Ogre::Matrix4::IDENTITY );
            SceneMan->GetGraphicsWorldPointer()->_setPass( UIManager::GetSingletonPtr()->GetAtlas(PrimaryAtlas)->_Get2DPass() );
        }

        void Screen::CreateVertexBuffer(const Whole& InitialSize)
        {
            VertexBufferSize = InitialSize * 6;
            this->SID->RenderOp.vertexData = OGRE_NEW Ogre::VertexData;
            this->SID->RenderOp.vertexData->vertexStart = 0;

            Ogre::VertexDeclaration* VertexDecl = this->SID->RenderOp.vertexData->vertexDeclaration;
            size_t Offset = 0;

            // Position.
            VertexDecl->addElement(0,0,Ogre::VET_FLOAT3,Ogre::VES_POSITION);
            Offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

            // Colour
            VertexDecl->addElement(0,Offset,Ogre::VET_FLOAT4,Ogre::VES_DIFFUSE);
            Offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT4);

            // Texture Coordinates
            VertexDecl->addElement(0,Offset,Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

            this->SID->VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(
                VertexDecl->getVertexSize(0),
                VertexBufferSize,
                Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                false);

            this->SID->RenderOp.vertexData->vertexBufferBinding->setBinding(0,this->SID->VertexBuffer);
            this->SID->RenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            this->SID->RenderOp.useIndexes = false;
        }

        void Screen::DestroyVertexBuffer()
        {
            OGRE_DELETE this->SID->RenderOp.vertexData;
            this->SID->RenderOp.vertexData = 0;
            this->SID->VertexBuffer.setNull();
            VertexBufferSize = 0;
        }

        void Screen::ResizeVertexBuffer(const Whole& RequestedSize)
        {
            if( VertexBufferSize == 0 )
                CreateVertexBuffer(RequestedSize);

            if(RequestedSize > VertexBufferSize)
            {
                Whole NewVertexBufferSize = 1;

                while(NewVertexBufferSize < RequestedSize)
                    NewVertexBufferSize <<= 1;

                this->SID->VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(
                    this->SID->RenderOp.vertexData->vertexDeclaration->getVertexSize(0),
                    NewVertexBufferSize,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                    false);
                VertexBufferSize = NewVertexBufferSize;
                this->SID->RenderOp.vertexData->vertexStart = 0;
                this->SID->RenderOp.vertexData->vertexBufferBinding->setBinding(0,this->SID->VertexBuffer);
            }
        }

        String& Screen::GetName()
        {
            return Name;
        }

        void Screen::SetVisible(bool Visible)
        {
            this->Visible = Visible;
        }

        bool Screen::IsVisible()
        {
            return Visible;
        }

        void Screen::Show()
        {
            this->Visible = true;
        }

        void Screen::Hide()
        {
            this->Visible = false;
        }

        Layer* Screen::CreateLayer(const String& Name, const Whole& Zorder)
        {
            Layer* NewLayer = new Layer(Name, Zorder, this);
            IndexData* NewIndexData = new IndexData();
            NewIndexData->IndexLayer = NewLayer;
            NewIndexData->RedrawNeeded = true;
            std::pair<std::map<Whole,IndexData*>::iterator,bool> TestPair = Indexes.insert(std::pair<Whole,IndexData*>(Zorder,NewIndexData));
            Layers.push_back(NewLayer);
            if(TestPair.second)
            {
                return NewLayer;
            }else{
                /// @todo add an exception here or maybe log entry, some notification it failed.
                return 0;
            }
            IndexRedrawNeeded = true;
        }

        Layer* Screen::GetLayer(const String& Name)
        {
            for ( std::vector<Layer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Layer* Ret = (*it);
                    return Ret;
                }
            }
            return 0;
        }

        Layer* Screen::GetLayer(const Whole& Index)
        {
            return Layers.at(Index);
            /*std::vector<Layer*>::iterator it = Layers.begin();
            while(0 < Index)
            {
                it++;
                Index--;
            }
            if(it != Layers.end())
                return (*it).second;
            return 0;//*/
        }

        Layer* Screen::GetLayerbyZorder(const Whole& Zorder)
        {
            std::map<Whole,IndexData*>::iterator it = Indexes.find(Zorder);
            if(it != Indexes.end())
                return (*it).second->IndexLayer;
            return 0;
        }

        Whole Screen::GetNumLayers()
        {
            return Layers.size();
        }

        void Screen::DestroyLayer(Layer* ToBeDestroyed)
        {
            if(Layers.empty())
                return;
            for( std::vector<Layer*>::iterator it = Layers.begin() ; it != Layers.end() ; ++it )
            {
                if( ToBeDestroyed == (*it) )
                {
                    Layers.erase(it);
                    break;
                }
            }
            for( std::map<Whole,IndexData*>::iterator it = Indexes.begin() ; it != Indexes.end() ; it++ )
            {
                if( ToBeDestroyed == (*it).second->IndexLayer )
                {
                    delete (*it).second->IndexLayer;
                    Indexes.erase(it);
                    break;
                }
            }
        }

        const Vector2& Screen::GetViewportDimensions()
        {
            return KnownViewportSize;
        }

        Button* Screen::CheckButtonMouseIsOver()
        {
            for( std::map<Whole,IndexData*>::reverse_iterator it = Indexes.rbegin() ; it != Indexes.rend() ; it++ )
            {
                if( (*it).second->IndexLayer->IsVisible() )
                {
                    Button* button = (*it).second->IndexLayer->CheckButtonMouseIsOver();
                    if(button)
                    {
                        return button;
                    }
                }
            }
            return 0;
        }

        Real Screen::GetTexelOffsetX() const
        {
            return this->SID->RenderSys->getHorizontalTexelOffset();
        }

        Real Screen::GetTexelOffsetY() const
        {
            return this->SID->RenderSys->getVerticalTexelOffset();
        }

        Widget* Screen::CheckWidgetMouseIsOver()
        {
            for( std::map<Whole,IndexData*>::reverse_iterator it = Indexes.rbegin() ; it != Indexes.rend() ; it++ )
            {
                if( (*it).second->IndexLayer->IsVisible() )
                {
                    Widget* widget = (*it).second->IndexLayer->CheckWidgetMouseIsOver();
                    if(widget)
                    {
                        return widget;
                    }
                }
            }
            return 0;
        }

        void Screen::SetPrimaryAtlas(const String& Atlas)
        {
            PrimaryAtlas = Atlas;
        }

        String Screen::GetPrimaryAtlas()
        {
            return PrimaryAtlas;
        }

        void Screen::CheckViewportSize()
        {
            Vector2 CurrentSize((Real)GameViewport->GetActualWidth(),(Real)GameViewport->GetActualHeight());
            if(KnownViewportSize == CurrentSize)
                return;

            ViewportUpdateTool::ConfigureSizes(KnownViewportSize,CurrentSize);
            KnownViewportSize = CurrentSize;

            InverseViewportSize.X = 1.0 / KnownViewportSize.X;
            InverseViewportSize.Y = 1.0 / KnownViewportSize.Y;

            for( std::vector<Layer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
            {
                (*it)->ViewportUpdate();
            }
            IndexRedrawAll = true;
            ViewportSizeChanged = true;
        }

        Viewport* Screen::GetViewport()
        {
            return GameViewport;
        }

        void Screen::_RenderScreen()
        {
            bool Force = false;
            // force == true if viewport size changed.
            if(ViewportSizeChanged
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
            || Orientation != GameViewport->getOrientationMode()
#else
            || OrientationChanged
#endif
            )
            {
                /*KnownViewportSize.X = GameViewport->GetActualWidth();
                KnownViewportSize.Y = GameViewport->GetActualHeight();

                InverseViewportSize.X = 1.0 / KnownViewportSize.X;
                InverseViewportSize.Y = 1.0 / KnownViewportSize.Y;// */

#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
                Orientation = GameViewport->getOrientationMode();
#else
                if(Orientation == Mezzanine::OM_Degree_90 || Orientation == Mezzanine::OM_Degree_270)
                {
                    std::swap(KnownViewportSize.X,KnownViewportSize.Y);
                    std::swap(InverseViewportSize.X,InverseViewportSize.Y);
                }
                OrientationChanged = false;
#endif
                if(Orientation == Mezzanine::OM_Degree_90)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTool::GetHalfPi(),Vector3::Unit_Z()));
                else if(Orientation == Mezzanine::OM_Degree_180)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTool::GetPi(),Vector3::Unit_Z()));
                else if(Orientation == Mezzanine::OM_Degree_270)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTool::GetPi() * 1.5,Vector3::Unit_Z()));
                else
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(0,0,0,1));
                Force = true;
            }
            _RenderVertices(Force);
            size_t KnownVertexCount = 0;
            KnownVertexCount = this->SID->RenderOp.vertexData->vertexCount;
            if(this->SID->RenderOp.vertexData->vertexCount)
            {
                if(TextureByVertex.size() == 0)
                {
                    AtlasAndPosition MyObject;
                    MyObject.RenderStart = 0;
                    MyObject.RenderEnd = KnownVertexCount;
                    MyObject.Atlas = PrimaryAtlas;
                    TextureByVertex.push_back(MyObject);
                }
                PrepareRenderSystem();
                String CurrAtlas = PrimaryAtlas;
                for( int i = 0 ; i < TextureByVertex.size() ; i++ )
                {
                    if(TextureByVertex[i].Atlas != CurrAtlas)
                    {
                        CurrAtlas = TextureByVertex[i].Atlas;
                        Ogre::TexturePtr TextureUse = UIManager::GetSingletonPtr()->GetAtlas(CurrAtlas)->_GetTexture();
                        this->SID->RenderSys->_setTexture(0,true,TextureUse);
                    }
                    this->SID->RenderOp.vertexData->vertexCount = TextureByVertex[i].RenderEnd - TextureByVertex[i].RenderStart;
                    this->SID->RenderOp.vertexData->vertexStart = TextureByVertex[i].RenderStart;
                    this->SID->RenderSys->_render(this->SID->RenderOp);
                }
            }
        }

        void Screen::_SetOrientation(const Mezzanine::OrientationMode& Mode)
        {
#if OGRE_NO_VIEWPORT_ORIENTATIONMODE == 1
            Orientation = Mode;
            OrientationChanged = true;

            if( Orientation == Mezzanine::OM_Degree_90 || Orientation == Mezzanine::OM_Degree_270 )
            {
                std::swap(KnownViewportSize.X,KnownViewportSize.Y);
                std::swap(InverseViewportSize.X,InverseViewportSize.Y);
            }
#endif
        }

        void Screen::_Transform(std::vector<VertexData>& Vertices, const Whole& Begin, const Whole& End)
        {
            static const Matrix4x4 Iden;
            Whole X;
            if( Begin != End )
            {
                for( X = Begin ; X < End ; X++ )
                {
                    Vertices[X].Vert.Position.X = ((Vertices[X].Vert.Position.X) * InverseViewportSize.X) * 2 - 1;
                    Vertices[X].Vert.Position.Y = ((Vertices[X].Vert.Position.Y) * InverseViewportSize.Y) * -2 + 1;
                }
            }
            if( VertexTransform != Iden )
            {
                for( X = Begin ; X < End ; X++ )
                    Vertices[X].Vert.Position = VertexTransform * Vertices[X].Vert.Position;
            }
        }

        void Screen::_RecalculateIndexes()
        {
            std::map<UInt32,IndexData*>::iterator IndIt;
            for( IndIt = Indexes.begin() ; IndIt != Indexes.end() ; IndIt++ )
            {
                (*IndIt).second->Vertices.Clear();
                (*IndIt).second->IndexLayer = NULL;
                (*IndIt).second->RedrawNeeded = false;
            }
            for( std::vector<Layer*>::iterator LayerIt = Layers.begin() ; LayerIt != Layers.end() ; LayerIt++ )
            {
                IndIt = Indexes.find( (*LayerIt)->GetIndex() );
                if (IndIt == Indexes.end())
                {
                    Indexes[(*LayerIt)->GetIndex()] = new IndexData();
                    IndIt = Indexes.find( (*LayerIt)->GetIndex() );
                }

                (*IndIt).second->IndexLayer = (*LayerIt);
            }
            bool Deleted = false;

            for(;;)
            {
                Deleted = false;
                for( IndIt = Indexes.begin() ; IndIt != Indexes.end() ; IndIt++ )
                {
                    if( (*IndIt).second->IndexLayer == NULL )
                    {
                        delete (*IndIt).second;
                        Indexes.erase(IndIt);
                        Deleted = true;
                        break;
                    }
                }
                if( IndIt == Indexes.end() )
                    break;
                if(!Deleted)
                    break;
            }
            IndexRedrawAll = true;
        }

        void Screen::_RedrawIndex(const UInt32& Index, bool Force)
        {
            std::map<UInt32,IndexData*>::iterator it = Indexes.find( Index );
            if (it == Indexes.end())
                return;

            IndexData* CurrIndexData = (*it).second;

            CurrIndexData->Vertices.Clear();
            CurrIndexData->RedrawNeeded = false;

            if( CurrIndexData->IndexLayer->IsVisible() )
                CurrIndexData->IndexLayer->_Render( CurrIndexData->Vertices, Force );
        }

        void Screen::_RedrawAllIndexes(bool Force)
        {
            IndexRedrawNeeded = false;
            IndexRedrawAll = false;
            for( std::map<UInt32,IndexData*>::iterator it = Indexes.begin() ; it != Indexes.end() ; it++ )
            {
                IndexData* CurrIndexData = (*it).second;
                if( CurrIndexData->RedrawNeeded || Force )
                {
                    CurrIndexData->Vertices.Clear();
                    CurrIndexData->RedrawNeeded = false;

                    if( CurrIndexData->IndexLayer->IsVisible() )
                        CurrIndexData->IndexLayer->_Render( CurrIndexData->Vertices, Force );
                }
            }
        }

        void Screen::_RequestIndexRedraw(const UInt32& Index)
        {
            std::map<UInt32,IndexData*>::iterator it = Indexes.find( Index );
            if (it == Indexes.end())
                return;

            (*it).second->RedrawNeeded = true;
            IndexRedrawNeeded = true;
        }

        void Screen::_RenderVertices(bool Force)
        {
            if(IndexRedrawNeeded == false)
                if(!Force)
                    return;

            _RedrawAllIndexes(Force);

            Whole KnownVertexCount = 0;

            for(std::map<UInt32,IndexData*>::iterator it = Indexes.begin(); it != Indexes.end();it++)
                KnownVertexCount += (*it).second->Vertices.Size();

            ResizeVertexBuffer(KnownVertexCount);
            OgreVertex* WriteIterator = (OgreVertex*) this->SID->VertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            IndexData* CurrIndexData = NULL;
            String CurrentName = PrimaryAtlas;
            TextureByVertex.clear();
            Whole PreviousTally = 0;
            AtlasAndPosition MyObject;
            MyObject.RenderStart = 0;
            MyObject.Atlas = PrimaryAtlas;
            for( std::map<UInt32,IndexData*>::iterator it = Indexes.begin() ; it != Indexes.end() ; it++ )
            {
                CurrIndexData = (*it).second;
                for( Whole I = 0 ; I < CurrIndexData->Vertices.Size() ; I++ )
                {
                    if( CurrIndexData->Vertices[I].Atlas.empty() )
                    {
                        World::GetWorldPointer()->LogAndThrow(Exception("Null or Empty String Atlas found when rendering UI."));
                    }
                    if( CurrIndexData->Vertices[I].Atlas != CurrentName )
                    {
                        if( I + PreviousTally != 0 )
                        {
                            MyObject.RenderEnd = I + PreviousTally;
                            TextureByVertex.push_back(MyObject);
                        }
                        MyObject.Atlas = CurrIndexData->Vertices[I].Atlas;
                        MyObject.RenderStart = I + PreviousTally;
                        CurrentName = CurrIndexData->Vertices[I].Atlas;
                    }
                    OgreVertex NewVertex;
                    NewVertex.Position = CurrIndexData->Vertices[I].Vert.Position.GetOgreVector3();
                    NewVertex.Colour = CurrIndexData->Vertices[I].Vert.Colour.GetOgreColourValue();
                    NewVertex.UV = CurrIndexData->Vertices[I].Vert.UV.GetOgreVector2();// */

                    *WriteIterator++ = NewVertex;//CurrIndexData->Vertices[I].Vert;
                }
                PreviousTally += CurrIndexData->Vertices.Size();
            }
            MyObject.RenderEnd = KnownVertexCount;
            MyObject.Atlas = CurrentName;
            TextureByVertex.push_back(MyObject);

            this->SID->VertexBuffer->unlock();
            this->SID->RenderOp.vertexData->vertexCount = KnownVertexCount;
        }
    }//ui
}//Mezzanine

#endif
