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

#include "uimanager.h"
#include "UI/screen.h"
#include "UI/viewportupdatetool.h"
#include "UI/textureatlas.h"
#include "UI/widget.h"
#include "UI/button.h"
#include "UI/rectangle.h"
#include "UI/caption.h"
#include "UI/widget.h"
#include "UI/checkbox.h"
#include "UI/markuptext.h"
#include "UI/listbox.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"
#include "UI/linelist.h"
#include "UI/window.h"
#include "UI/menu.h"
#include "UI/spinner.h"
#include "UI/scrolledcellgrid.h"
#include "UI/pagedcellgrid.h"
#include "UI/dropdownlist.h"
#include "UI/tabset.h"
#include "UI/renderablecontainerwidget.h"
#include "Graphics/viewport.h"
#include "cameramanager.h"
#include "scenemanager.h"
#include "camera.h"
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
                if( ParentScreen->IsVisible() )
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
                { MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Out of bounds index requested in ScreenVertexData"); }
        }

        Screen::Screen(const String& name, const String& Atlas, Graphics::Viewport* WindowViewport)
            : Name(name),
              PrimaryAtlas(Atlas),
              GameViewport(WindowViewport),
              Scale(Vector3(1,1,1)),
              VertexBufferSize(0),
              Visible(true),
              IndexRedrawNeeded(false),
              IndexRedrawAll(false),
              OrientationChanged(false),
              ViewportSizeChanged(false),
              Orientation(Mezzanine::OM_Degree_0)
        {
            KnownViewportSize.X = (Real)GameViewport->GetActualWidth();
            KnownViewportSize.Y = (Real)GameViewport->GetActualHeight();
            InverseViewportSize.X = 1 / KnownViewportSize.X;
            InverseViewportSize.Y = 1 / KnownViewportSize.Y;

            SID = new ScreenInternalData();
            SID->RenderSys = Ogre::Root::getSingletonPtr()->getRenderSystem();
            SID->ParentScreen = this;

            SceneManager* SceneMan = GetSceneManager();
            if(SceneMan)
                SceneMan->GetGraphicsWorldPointer()->addRenderQueueListener(SID);

            VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(0,0,0,1));
            CreateVertexBuffer();
        }

        Screen::~Screen()
        {
        }

        SceneManager* Screen::GetSceneManager()
        {
            /// @todo This function exists (as opposed to storing a pointer that doesn't change) so that if changes in the
            /// viewport configuration occur this will pick up on that.  However the render queue listener that is added in
            /// this class' constructor never gets re-assigned.  This needs to be fixed.  Until then if a change does occur
            /// the UI will be rendered at a different time then it needs to be, potentially overwritten by the scene render.
            if(GameViewport)
            {
                Camera* Cam = GameViewport->GetViewportCamera();
                if(Cam)
                {
                    SceneManager* SceneMan = Cam->GetCameraManager()->GetScene();
                    if(SceneMan) return SceneMan;
                    else return NULL;
                }else return NULL;
            }else return NULL;
        }

        Screen* Screen::GetScreen()
        {
            return this;
        }

        Widget* Screen::CheckAndInsert(Widget* ToInsert)
        {
            String WidgetName = ToInsert->GetName();
            WidgetIterator WidIt = Widgets.find(WidgetName);
            if( WidIt != Widgets.end() )
            {
                MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Widget with name \"" + Name + "\" already exists.")
            }
            Widgets[WidgetName] = ToInsert;
            return ToInsert;
        }

        void Screen::PrepareRenderSystem()
        {
            this->SID->RenderSys->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setViewMatrix( Ogre::Matrix4::IDENTITY );
            SceneManager* SceneMan = GetSceneManager();
            if(SceneMan)
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

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Visibility Methods

        String& Screen::GetName()
        {
            return Name;
        }

        void Screen::SetVisible(bool visible)
        {
            if( this->Visible == visible )
                return;
            UIManager::GetSingletonPtr()->_NotifyScreenVisibility(this,visible);
            this->Visible = visible;
        }

        bool Screen::IsVisible()
        {
            return Visible;
        }

        void Screen::Show()
        {
            SetVisible(true);
        }

        void Screen::Hide()
        {
            SetVisible(false);
        }

        const Vector2& Screen::GetViewportDimensions()
        {
            return KnownViewportSize;
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

            for( IndexIterator it = WidgetsRoot.begin() ; it != WidgetsRoot.end() ; it++ )
            {
                (*it).second->IndexWidget->UpdateDimensions();
            }
            IndexRedrawAll = true;
            ViewportSizeChanged = true;
        }

        Graphics::Viewport* Screen::GetViewport()
        {
            return GameViewport;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Hover Checks

        Widget* Screen::CheckWidgetMouseIsOver()
        {
            for( ReverseIndexIterator it = WidgetsRoot.rbegin() ; it != WidgetsRoot.rend() ; ++it )
            {
                if( (*it).second->IndexWidget->IsVisible() )
                {
                    if( (*it).second->IndexWidget->CheckMouseHover() )
                    {
                        return (*it).second->IndexWidget;
                    }
                }
            }
            return 0;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating Widgets

        Button* Screen::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            return static_cast<Button*>( CheckAndInsert( RenderableFactory::CreateButton(Name,Rect) ) );
        }

        Button* Screen::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            return static_cast<Button*>( CheckAndInsert( RenderableFactory::CreateButton(Name,Rect,Glyph,Text) ) );
        }

        Button* Screen::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            return static_cast<Button*>( CheckAndInsert( RenderableFactory::CreateButton(Name,Rect,LineHeight,Text) ) );
        }

        Scrollbar* Screen::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            return static_cast<Scrollbar*>( CheckAndInsert( RenderableFactory::CreateScrollbar(Name,Rect,Style) ) );
        }

        CheckBox* Screen::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& LabelText)
        {
            return static_cast<CheckBox*>( CheckAndInsert( RenderableFactory::CreateCheckBox(Name,Rect,LineHeight,LabelText) ) );
        }

        ListBox* Screen::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle)
        {
            return static_cast<ListBox*>( CheckAndInsert( RenderableFactory::CreateListBox(Name,Rect,ScrollStyle) ) );
        }

        Spinner* Screen::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            return static_cast<Spinner*>( CheckAndInsert( RenderableFactory::CreateSpinner(Name,Rect,SStyle,GlyphHeight) ) );
        }

        ScrolledCellGrid* Screen::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            return static_cast<ScrolledCellGrid*>( CheckAndInsert( RenderableFactory::CreateScrolledCellGrid(Name,Rect,Thickness,Style) ) );
        }

        PagedCellGrid* Screen::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            return static_cast<PagedCellGrid*>( CheckAndInsert( RenderableFactory::CreatePagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight) ) );
        }

        DropDownList* Screen::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            return static_cast<DropDownList*>( CheckAndInsert( RenderableFactory::CreateDropDownList(Name,Rect,LineHeight,ScrollStyle) ) );
        }

        TabSet* Screen::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            return static_cast<TabSet*>( CheckAndInsert( RenderableFactory::CreateTabSet(Name,SetRect) ) );
        }

        EnclosedRenderableContainerWidget* Screen::CreateEnclosedRenderableContainerWidget(ConstString& Name, const RenderableRect& Rect)
        {
            return static_cast<EnclosedRenderableContainerWidget*>( CheckAndInsert( RenderableFactory::CreateEnclosedRenderableContainerWidget(Name,Rect) ) );
        }

        Window* Screen::CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect)
        {
            return static_cast<Window*>( CheckAndInsert( ExtendedRenderableFactory::CreateWidgetWindow(Name,Rect) ) );
        }

        Menu* Screen::CreateMenu(ConstString& Name, const RenderableRect& Rect)
        {
            return static_cast<Menu*>( CheckAndInsert( ExtendedRenderableFactory::CreateMenu(Name,Rect) ) );
        }

        OpenRenderableContainerWidget* Screen::CreateOpenRenderableContainerWidget(ConstString& Name)
        {
            return static_cast<OpenRenderableContainerWidget*>( CheckAndInsert( ExtendedRenderableFactory::CreateOpenRenderableContainerWidget(Name) ) );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with all Widgets

        void Screen::AddRootWidget(const Whole& ZOrder, Widget* ToAdd)
        {
            IndexData* NewIndex = new IndexData();
            NewIndex->IndexWidget = ToAdd;
            WidgetsRoot.insert(std::pair<Whole,IndexData*>(ZOrder,NewIndex));
            ToAdd->_SetZOrder(ZOrder);
            IndexRedrawNeeded = true;
        }

        Widget* Screen::GetWidget(ConstString& Name)
        {
            std::map<String,Widget*>::iterator WidIt = Widgets.find(Name);
            if( WidIt != Widgets.end() ) return (*WidIt).second;
            else return NULL;
        }

        Widget* Screen::GetWidget(const Whole& ZOrder)
        {
            IndexIterator WidIt = WidgetsRoot.find(ZOrder);
            if( WidIt != WidgetsRoot.end() ) return (*WidIt).second->IndexWidget;
            else return NULL;
        }

        Whole Screen::GetNumWidgets()
        {
            return Widgets.size();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Atlas Query

        void Screen::SetPrimaryAtlas(const String& Atlas)
        {
            PrimaryAtlas = Atlas;
        }

        String Screen::GetPrimaryAtlas()
        {
            return PrimaryAtlas;
        }

        Vector2 Screen::GetSolidUV(const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetWhitePixel();
        }

        Sprite* Screen::GetSprite(const String& SpriteName,const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetSprite(SpriteName);
        }

        GlyphData* Screen::GetGlyphData(const Whole& ID,const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetGlyphData(ID);
        }

        Vector2 Screen::GetTextureSize(const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetTextureSize();
        }

        TextureAtlas* Screen::GetAtlas(const String& Atlas) const
        {
            return Manager->GetAtlas(Atlas);
        }

        ColourValue Screen::GetMarkupColour(const Whole& Index,const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetMarkupColour(Index);
        }

        Real Screen::GetTexelOffsetX() const
        {
            return this->SID->RenderSys->getHorizontalTexelOffset();
        }

        Real Screen::GetTexelOffsetY() const
        {
            return this->SID->RenderSys->getVerticalTexelOffset();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions

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

        void Screen::_Transform(ScreenVertexData& Vertices, const Whole& Begin, const Whole& End)
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

        void Screen::_RedrawIndex(const UInt32& Index)
        {
            IndexIterator it = WidgetsRoot.find( Index );
            if (it == WidgetsRoot.end())
                return;

            IndexData* CurrIndexData = (*it).second;
            CurrIndexData->Vertices.Clear();

            if( CurrIndexData->IndexWidget->IsVisible() )
            {
                CurrIndexData->IndexWidget->_AppendVertices( CurrIndexData->Vertices );
                _Transform( CurrIndexData->Vertices, 0, CurrIndexData->Vertices.Size() );
            }
        }

        void Screen::_RedrawAllIndexes(bool Force)
        {
            IndexRedrawNeeded = false;
            IndexRedrawAll = false;
            for( IndexIterator it = WidgetsRoot.begin() ; it != WidgetsRoot.end() ; it++ )
            {
                IndexData* CurrIndexData = (*it).second;
                if( CurrIndexData->IndexWidget->Dirty || Force )
                {
                    CurrIndexData->Vertices.Clear();

                    if( CurrIndexData->IndexWidget->IsVisible() )
                    {
                        if( CurrIndexData->IndexWidget->Dirty )
                            CurrIndexData->IndexWidget->_Redraw();
                        CurrIndexData->IndexWidget->_AppendVertices( CurrIndexData->Vertices );
                        _Transform( CurrIndexData->Vertices, 0, CurrIndexData->Vertices.Size() );
                    }
                }
            }
        }

        void Screen::_RequestIndexRedraw(const UInt16& Index)
        {
            IndexIterator it = WidgetsRoot.find( Index );
            if (it == WidgetsRoot.end())
                return;

            IndexRedrawNeeded = true;
        }

        void Screen::_RenderVertices(bool Force)
        {
            if(IndexRedrawNeeded == false)
                if(!Force)
                    return;

            _RedrawAllIndexes(Force);

            Whole KnownVertexCount = 0;

            for(IndexIterator it = WidgetsRoot.begin(); it != WidgetsRoot.end();it++)
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
            for( IndexIterator it = WidgetsRoot.begin() ; it != WidgetsRoot.end() ; it++ )
            {
                CurrIndexData = (*it).second;
                for( Whole I = 0 ; I < CurrIndexData->Vertices.Size() ; I++ )
                {
                    if( CurrIndexData->Vertices[I].Atlas.empty() )
                    {
                        MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Null or Empty String Atlas found when rendering UI.");
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
