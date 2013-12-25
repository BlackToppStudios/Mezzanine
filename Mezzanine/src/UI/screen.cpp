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
#ifndef _uiscreen_cpp
#define _uiscreen_cpp

#include "UI/uimanager.h"
#include "UI/screen.h"
#include "UI/textureatlas.h"
#include "UI/mousehoverstrategy.h"
#include "UI/brutestrategy.h"

#include "UI/button.h"
#include "UI/checkbox.h"
#include "UI/dropdownlist.h"
#include "UI/editbox.h"
#include "UI/gridcontainer.h"
#include "UI/horizontalcontainer.h"
#include "UI/horizontalscrollbar.h"
#include "UI/linelist.h"
#include "UI/listbox.h"
#include "UI/menubutton.h"
#include "UI/menuentry.h"
#include "UI/radiobutton.h"
#include "UI/scrollbar.h"
#include "UI/spinner.h"
#include "UI/tabset.h"
#include "UI/verticalcontainer.h"
#include "UI/verticalscrollbar.h"
#include "UI/widget.h"
#include "UI/window.h"

#include "Graphics/gamewindow.h"
#include "Graphics/viewport.h"
#include "Graphics/cameramanager.h"
#include "Graphics/cameraproxy.h"
#include "Graphics/graphicsmanager.h"
#include "Graphics/scenemanager.h"

#include "mathtool.h"
#include "exception.h"

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
        ///////////////////////////////////////////////////////////////////////////////
        // Child Processing Functors

        ///////////////////////////////////////////////////////////////////////////////
        /// @class VertexCollectFunctor
        /// @headerfile screen.cpp
        /// @brief Simple functor for appending all vertices in the renderable tree to a vector.
        ///////////////////////////////////////
        class VertexCollectFunctor
        {
            public:
                ScreenRenderData* Data;

                VertexCollectFunctor(ScreenRenderData* pData) : Data(pData) {}
                ~VertexCollectFunctor() {}

                bool operator()(QuadRenderable* Quad)
                {
                    Quad->_AppendRenderData(*Data);
                    return false;
                }
        };//VertexCollectFunctor

        ///////////////////////////////////////////////////////////////////////////////
        // OgreVertex

        ///////////////////////////////////////////////////////////////////////////////
        /// @struct OgreVertex
        /// @headerfile screen.cpp
        /// @brief Simple class that facilitates conversions when inserting vertex's into the video buffer.
        ///////////////////////////////////////
        struct OgreVertex
        {
            Ogre::Vector3 Position;
            Ogre::ColourValue Colour;
            Ogre::Vector2 UV;
        };

        ///////////////////////////////////////////////////////////////////////////////
        // ScreenRenderData Methods

        void ScreenRenderData::Clear()
        {
            LowVertices.clear();
            MediumVertices.clear();
            HighVertices.clear();
        }

        Whole ScreenRenderData::Size()
        {
            return LowVertices.size() + MediumVertices.size() + HighVertices.size();
        }

        void ScreenRenderData::Append(ScreenRenderData& OtherData)
        {
            this->LowVertices.insert(this->LowVertices.end(),OtherData.LowVertices.begin(),OtherData.LowVertices.end());
            this->MediumVertices.insert(this->MediumVertices.end(),OtherData.MediumVertices.begin(),OtherData.MediumVertices.end());
            this->HighVertices.insert(this->HighVertices.end(),OtherData.HighVertices.begin(),OtherData.HighVertices.end());
        }

        VertexData& ScreenRenderData::operator[](const Whole& Index)
        {
            if( Index < LowVertices.size() )
                return LowVertices[Index];
            else if( Index < LowVertices.size() + MediumVertices.size() )
                return MediumVertices[Index - LowVertices.size()];
            else if( Index < LowVertices.size() + MediumVertices.size() + HighVertices.size() )
                return HighVertices[Index - (LowVertices.size() + MediumVertices.size())];
            else
                { MEZZ_EXCEPTION(Exception::MM_OUT_OF_BOUNDS_EXCEPTION,"Out of bounds index requested in ScreenRenderData"); }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ScreenInternalData Methods

        ///////////////////////////////////////////////////////////////////////////////
        /// @struct ScreenInternalData
        /// @headerfile screen.cpp
        /// @brief Basic struct holding some of the internal bits of this class that could not be placed on the class directly.
        ///////////////////////////////////////
        struct ScreenInternalData : public Ogre::RenderQueueListener
        {
            Screen* ParentScreen;
            Ogre::RenderOperation RenderOp;
            Ogre::RenderSystem* RenderSys;
            Ogre::HardwareVertexBufferSharedPtr VertexBuffer;

            void renderQueueStarted(Ogre::uint8, const Ogre::String&, bool&) {  }
            void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
            {
                if( this->RenderSys->_getViewport() != this->ParentScreen->GetViewport()->GetOgreViewport() || queueGroupId != Ogre::RENDER_QUEUE_OVERLAY )
                    return;
                if( this->ParentScreen->IsVisible() )
                    this->ParentScreen->_RenderScreen();
            }
        };//ScreenInternalData

        ///////////////////////////////////////////////////////////////////////////////
        // Screen Methods

        Screen::Screen(const String& RendName, const String& Atlas, Graphics::Viewport* WindowViewport, UIManager* Manager) :
            QuadRenderable(RendName,this),

            PrimaryAtlas(Atlas),
            Scale(1,1,1),
            MouseHitPosition(-1,-1),

            UIMan(Manager),
            GameViewport(WindowViewport),
            MouseStrat(NULL),

            Orientation(Mezzanine::OM_Degree_0)
        {
            this->AddAllDefaultWidgetFactories();
            this->ActDims.Size.X = (Real)this->GameViewport->GetActualWidth();
            this->ActDims.Size.Y = (Real)this->GameViewport->GetActualHeight();
            this->InverseSize.X = 1 / this->ActDims.Size.X;
            this->InverseSize.Y = 1 / this->ActDims.Size.Y;

            this->SID = new ScreenInternalData();
            this->SID->RenderSys = Ogre::Root::getSingletonPtr()->getRenderSystem();
            this->SID->ParentScreen = this;

            Graphics::SceneManager* SceneMan = this->GetSceneManager();
            if(SceneMan)
                SceneMan->_GetGraphicsWorldPointer()->addRenderQueueListener(SID);

            this->VertexTransform.SetTransform(Vector3(0,0,0),this->Scale,Quaternion(0,0,0,1));
            this->CreateVertexBuffer(32 * 6);

            this->SetMousePassthrough(true);
            this->SetMouseHoverStrategy(new BruteStrategy());
        }

        Screen::Screen(const XML::Node& XMLNode, UIManager* Manager) :
            QuadRenderable(this),
            Scale(1,1,1),
            MouseHitPosition(-1,-1),
            UIMan(Manager),
            GameViewport(NULL),
            MouseStrat(NULL),
            Orientation(Mezzanine::OM_Degree_0)
        {
            this->AddAllDefaultWidgetFactories();

            this->SID = new ScreenInternalData();
            this->SID->RenderSys = Ogre::Root::getSingletonPtr()->getRenderSystem();
            this->SID->ParentScreen = this;

            Graphics::SceneManager* SceneMan = this->GetSceneManager();
            if(SceneMan)
                SceneMan->_GetGraphicsWorldPointer()->addRenderQueueListener(SID);

            this->CreateVertexBuffer(32 * 6);
            this->SetMouseHoverStrategy(new BruteStrategy());

            this->ProtoDeSerialize(XMLNode);
        }

        Screen::~Screen()
        {
            this->DestroyAllWidgets();
        }

        Graphics::SceneManager* Screen::GetSceneManager() const
        {
            /// @todo This function exists (as opposed to storing a pointer that doesn't change) so that if changes in the
            /// viewport configuration occur this will pick up on that.  However the render queue listener that is added in
            /// this class' constructor never gets re-assigned.  This needs to be fixed.  Until then if a change does occur
            /// the UI will be rendered at a different time then it needs to be, potentially overwritten by the scene render.
            if( GameViewport ) {
                Graphics::CameraProxy* Cam = this->GameViewport->GetViewportCamera();
                if( Cam ) {
                    Graphics::SceneManager* SceneMan = static_cast<Graphics::CameraManager*>( Cam->GetCreator() )->GetScene();
                    if( SceneMan ) return SceneMan;
                    else return NULL;
                }else return NULL;
            }else return NULL;
        }

        WidgetFactory* Screen::GetWidgetFactoryExcept(const String& WidgetTypeName)
        {
            WidgetFactoryIterator WidFactIt = this->WidgetFactories.find( WidgetTypeName );
            if( WidFactIt  != this->WidgetFactories.end() ) {
                return (*WidFactIt).second;
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Attempting to create a " + WidgetTypeName + " Widget without it's factory registered.");
            }
        }

        Widget* Screen::CheckAndInsertExcept(Widget* ToInsert)
        {
            String WidgetName = ToInsert->GetName();
            std::pair<WidgetIterator,Bool> InsertReturn = this->Widgets.insert( std::pair<String,Widget*>(WidgetName,ToInsert) );
            if( !InsertReturn.second )
                { MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Widget with name \"" + Name + "\" already exists."); }
            return ToInsert;
        }

        void Screen::PrepareRenderSystem()
        {
            this->SID->RenderSys->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
            this->SID->RenderSys->_setViewMatrix( Ogre::Matrix4::IDENTITY );
            Graphics::SceneManager* SceneMan = GetSceneManager();
            if(SceneMan)
                SceneMan->_GetGraphicsWorldPointer()->_setPass( UIManager::GetSingletonPtr()->GetAtlas(PrimaryAtlas)->_Get2DPass() );
        }

        void Screen::CreateVertexBuffer(const Whole& InitialSize)
        {
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
                InitialSize,
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
        }

        void Screen::ResizeVertexBuffer(const Whole& RequestedSize)
        {
            if( this->SID->VertexBuffer.isNull() )
                this->CreateVertexBuffer(RequestedSize);

            if(RequestedSize > this->SID->VertexBuffer->getNumVertices() )
            {
                // Update only by powers of 2
                Whole NewVertexBufferSize = 1;
                while(NewVertexBufferSize < RequestedSize)
                    NewVertexBufferSize <<= 1;

                this->SID->VertexBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(
                    this->SID->RenderOp.vertexData->vertexDeclaration->getVertexSize(0),
                    NewVertexBufferSize,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                    false);
                this->SID->RenderOp.vertexData->vertexStart = 0;
                this->SID->RenderOp.vertexData->vertexBufferBinding->setBinding(0,this->SID->VertexBuffer);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility and Visibility Methods

        void Screen::SetVisible(Bool CanSee)
            { this->Visible = CanSee; }

        Bool Screen::GetVisible() const
            { return this->Visible; }

        Bool Screen::IsVisible() const
            { return this->Visible; }

        void Screen::Show()
            { this->SetVisible(true); }

        void Screen::Hide()
            { this->SetVisible(false); }

        Renderable::RenderableType Screen::GetRenderableType() const
            { return Renderable::RT_Screen; }

        const Vector2& Screen::GetViewportDimensions() const
            { return this->ActDims.Size; }

        void Screen::CheckViewportSize()
        {
            Vector2 CurrentSize((Real)this->GameViewport->GetActualWidth(),(Real)this->GameViewport->GetActualHeight());
            if(this->ActDims.Size == CurrentSize)
                return;

            Rect OldRect(this->ActDims);
            Rect NewRect(Vector2(0,0),CurrentSize);
            this->UpdateDimensions(OldRect,NewRect);

            this->InverseSize.X = 1.0 / this->ActDims.Size.X;
            this->InverseSize.Y = 1.0 / this->ActDims.Size.Y;
        }

        Graphics::Viewport* Screen::GetViewport() const
            { return this->GameViewport; }

        UIManager* Screen::GetManager() const
            { return this->UIMan; }

        ///////////////////////////////////////////////////////////////////////////////
        // Mouse Hover Methods

        void Screen::SetMouseHoverStrategy(MouseHoverStrategy* Strategy)
        {
            if(this->MouseStrat) {
                delete this->MouseStrat;
                this->MouseStrat = NULL;
            }
            this->MouseStrat = Strategy;
            this->MouseStrat->_SetScreen(this);
        }

        MouseHoverStrategy* Screen::GetMouseHoverStrategy() const
        {
            return this->MouseStrat;
        }

        Widget* Screen::FindHoveredWidget(const Vector2& MousePos)
        {
            Widget* Ret = this->MouseStrat->FindHoveredWidget(MousePos);
            if( Ret != NULL ) this->MouseHitPosition = MousePos;
            else this->MouseHitPosition.SetValues(-1,-1);

            return Ret;
        }

        const Vector2& Screen::GetMouseHitPosition() const
        {
            return this->MouseHitPosition;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // WidgetFactory Management

        void Screen::AddWidgetFactory(WidgetFactory* ToBeAdded)
        {
            this->WidgetFactories.insert( std::pair<String,WidgetFactory*>(ToBeAdded->GetWidgetTypeName(),ToBeAdded) );
        }

        void Screen::RemoveWidgetFactory(WidgetFactory* ToBeRemoved)
        {
            this->RemoveWidgetFactory( ToBeRemoved->GetWidgetTypeName() );
        }

        void Screen::RemoveWidgetFactory(const String& ImplName)
        {
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( ImplName );
            if( FactIt != this->WidgetFactories.end() ) {
                this->WidgetFactories.erase(FactIt);
            }
        }

        void Screen::DestroyWidgetFactory(WidgetFactory* ToBeDestroyed)
        {
            this->DestroyWidgetFactory( ToBeDestroyed->GetWidgetTypeName() );
        }

        void Screen::DestroyWidgetFactory(const String& ImplName)
        {
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( ImplName );
            if( FactIt != this->WidgetFactories.end() ) {
                delete (*FactIt).second;
                this->WidgetFactories.erase(FactIt);
            }
        }

        void Screen::DestroyAllWidgetFactories()
        {
            for( WidgetFactoryIterator FactIt = this->WidgetFactories.begin() ; FactIt != this->WidgetFactories.end() ; ++FactIt )
            {
                delete (*FactIt).second;
            }
            this->WidgetFactories.clear();
        }

        void Screen::AddAllDefaultWidgetFactories()
        {
            WidgetFactoryIterator FactIt;
            // Generic Widget
            FactIt = this->WidgetFactories.find( Widget::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new GenericWidgetFactory() );
            // Button
            FactIt = this->WidgetFactories.find( Button::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new ButtonFactory() );
            // MenuButton
            FactIt = this->WidgetFactories.find( MenuButton::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new MenuButtonFactory() );
            // CheckBox
            FactIt = this->WidgetFactories.find( CheckBox::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new CheckBoxFactory() );
            // HorizontalScrollbar
            FactIt = this->WidgetFactories.find( HorizontalScrollbar::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new HorizontalScrollbarFactory() );
            // VerticalScrollbar
            FactIt = this->WidgetFactories.find( VerticalScrollbar::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new VerticalScrollbarFactory() );

            // HorizontalContainer
            FactIt = this->WidgetFactories.find( HorizontalContainer::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new HorizontalContainerFactory() );
            // VerticalContainer
            FactIt = this->WidgetFactories.find( VerticalContainer::TypeName );
            if( FactIt == this->WidgetFactories.end() ) this->AddWidgetFactory( new VerticalContainerFactory() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Widget Management

        Widget* Screen::CreateWidget(const XML::Node& WidgetNode)
        {
            String WidgetTypeName = WidgetNode.Name();
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( WidgetTypeName );
            if( FactIt != this->WidgetFactories.end() ) {
                return this->CheckAndInsertExcept( (*FactIt).second->CreateWidget(WidgetNode,this) );
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to create widget of type \"" + WidgetTypeName + "\", which has no factory registered.");
            }
        }

        Widget* Screen::GetWidget(const String& Name)
        {
            WidgetIterator WidIt = this->Widgets.find(Name);
            if( WidIt != this->Widgets.end() ) return (*WidIt).second;
            else return NULL;
        }

        Whole Screen::GetNumWidgets()
        {
            return this->Widgets.size();
        }

        void Screen::DestroyWidget(Widget* ToBeDestroyed)
        {
            // Remove the widget from the Widget container.
            WidgetIterator WidIt = this->Widgets.find( ToBeDestroyed->GetName() );
            if( WidIt != this->Widgets.end() )
                this->Widgets.erase(WidIt);

            // Delete the Widget
            WidgetFactoryIterator FactIt = this->WidgetFactories.find( ToBeDestroyed->GetTypeName() );
            if( FactIt != this->WidgetFactories.end() ) {
                (*FactIt).second->DestroyWidget( ToBeDestroyed );
            }
        }

        void Screen::DestroyAllWidgets()
        {
            for( ChildIterator ChildIt = this->ChildWidgets.begin() ; ChildIt != this->ChildWidgets.end() ; ++ChildIt )
            {
                WidgetFactoryIterator FactIt = this->WidgetFactories.find( (*ChildIt)->GetTypeName() );
                if( FactIt != this->WidgetFactories.end() ) {
                    (*FactIt).second->DestroyWidget( (*ChildIt) );
                }
            }
            this->RemoveAllChildren();
            this->Widgets.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Widget Creation Methods

        Widget* Screen::CreateWidget(const String& Name)
        {
            Widget* NewWidget = static_cast<GenericWidgetFactory*>( this->GetWidgetFactoryExcept( Widget::TypeName ) )->CreateWidget( Name, this );
            this->CheckAndInsertExcept( NewWidget );
            return NewWidget;
        }

        Widget* Screen::CreateWidget(const String& Name, const UnifiedRect& RendRect)
        {
            Widget* NewWidget = static_cast<GenericWidgetFactory*>( this->GetWidgetFactoryExcept( Widget::TypeName ) )->CreateWidget( Name, RendRect, this );
            this->CheckAndInsertExcept( NewWidget );
            return NewWidget;
        }

        Button* Screen::CreateButton(const String& Name)
        {
            Button* NewButton = static_cast<ButtonFactory*>( this->GetWidgetFactoryExcept( Button::TypeName ) )->CreateButton( Name, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        Button* Screen::CreateButton(const String& Name, const UnifiedRect& RendRect)
        {
            Button* NewButton = static_cast<ButtonFactory*>( this->GetWidgetFactoryExcept( Button::TypeName ) )->CreateButton( Name, RendRect, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        MenuButton* Screen::CreateMenuButton(const String& Name)
        {
            MenuButton* NewButton = static_cast<MenuButtonFactory*>( this->GetWidgetFactoryExcept( MenuButton::TypeName ) )->CreateMenuButton( Name, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        MenuButton* Screen::CreateMenuButton(const String& Name, const UnifiedRect& RendRect)
        {
            MenuButton* NewButton = static_cast<MenuButtonFactory*>( this->GetWidgetFactoryExcept( MenuButton::TypeName ) )->CreateMenuButton( Name, RendRect, this );
            this->CheckAndInsertExcept( NewButton );
            return NewButton;
        }

        CheckBox* Screen::CreateCheckBox(const String& Name)
        {
            CheckBox* NewCheckBox = static_cast<CheckBoxFactory*>( this->GetWidgetFactoryExcept( CheckBox::TypeName ) )->CreateCheckBox( Name, this );
            this->CheckAndInsertExcept( NewCheckBox );
            return NewCheckBox;
        }

        CheckBox* Screen::CreateCheckBox(const String& Name, const UnifiedRect& RendRect)
        {
            CheckBox* NewCheckBox = static_cast<CheckBoxFactory*>( this->GetWidgetFactoryExcept( CheckBox::TypeName ) )->CreateCheckBox( Name, RendRect, this );
            this->CheckAndInsertExcept( NewCheckBox );
            return NewCheckBox;
        }

        HorizontalScrollbar* Screen::CreateHorizontalScrollbar(const String& Name, const UI::ScrollbarStyle Style)
        {
            HorizontalScrollbar* NewHScroll = static_cast<HorizontalScrollbarFactory*>( this->GetWidgetFactoryExcept( HorizontalScrollbar::TypeName ) )->CreateHorizontalScrollbar( Name, Style, this );
            this->CheckAndInsertExcept( NewHScroll );
            return NewHScroll;
        }

        HorizontalScrollbar* Screen::CreateHorizontalScrollbar(const String& Name, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style)
        {
            HorizontalScrollbar* NewHScroll = static_cast<HorizontalScrollbarFactory*>( this->GetWidgetFactoryExcept( HorizontalScrollbar::TypeName ) )->CreateHorizontalScrollbar( Name, RendRect, Style, this );
            this->CheckAndInsertExcept( NewHScroll );
            return NewHScroll;
        }

        VerticalScrollbar* Screen::CreateVerticalScrollbar(const String& Name, const UI::ScrollbarStyle Style)
        {
            VerticalScrollbar* NewVScroll = static_cast<VerticalScrollbarFactory*>( this->GetWidgetFactoryExcept( VerticalScrollbar::TypeName ) )->CreateVerticalScrollbar( Name, Style, this );
            this->CheckAndInsertExcept( NewVScroll );
            return NewVScroll;
        }

        VerticalScrollbar* Screen::CreateVerticalScrollbar(const String& Name, const UnifiedRect& RendRect, const UI::ScrollbarStyle Style)
        {
            VerticalScrollbar* NewVScroll = static_cast<VerticalScrollbarFactory*>( this->GetWidgetFactoryExcept( VerticalScrollbar::TypeName ) )->CreateVerticalScrollbar( Name, RendRect, Style, this );
            this->CheckAndInsertExcept( NewVScroll );
            return NewVScroll;
        }

        HorizontalContainer* Screen::CreateHorizontalContainer(const String& RendName)
        {
            HorizontalContainer* NewHContain = static_cast<HorizontalContainerFactory*>( this->GetWidgetFactoryExcept( HorizontalContainer::TypeName ) )->CreateHorizontalContainer( Name, this );
            this->CheckAndInsertExcept( NewHContain );
            return NewHContain;
        }

        HorizontalContainer* Screen::CreateHorizontalContainer(const String& RendName, const UnifiedRect& RendRect)
        {
            HorizontalContainer* NewHContain = static_cast<HorizontalContainerFactory*>( this->GetWidgetFactoryExcept( HorizontalContainer::TypeName ) )->CreateHorizontalContainer( Name, RendRect, this );
            this->CheckAndInsertExcept( NewHContain );
            return NewHContain;
        }

        VerticalContainer* Screen::CreateVerticalContainer(const String& RendName)
        {
            VerticalContainer* NewVContain = static_cast<VerticalContainerFactory*>( this->GetWidgetFactoryExcept( VerticalContainer::TypeName ) )->CreateVerticalContainer( Name, this );
            this->CheckAndInsertExcept( NewVContain );
            return NewVContain;
        }

        VerticalContainer* Screen::CreateVerticalContainer(const String& RendName, const UnifiedRect& RendRect)
        {
            VerticalContainer* NewVContain = static_cast<VerticalContainerFactory*>( this->GetWidgetFactoryExcept( VerticalContainer::TypeName ) )->CreateVerticalContainer( Name, RendRect, this );
            this->CheckAndInsertExcept( NewVContain );
            return NewVContain;
        }


        /*ListBox* Screen::CreateListBox(ConstString& Name, const Rect& RendRect, const UI::ScrollbarStyle& ScrollStyle)
        {
            return static_cast<ListBox*>( this->CheckAndInsert( RenderableFactory::CreateListBox(Name,RendRect,ScrollStyle) ) );
        }

        Spinner* Screen::CreateSpinner(ConstString& Name, const Rect& RendRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            return static_cast<Spinner*>( this->CheckAndInsert( RenderableFactory::CreateSpinner(Name,RendRect,SStyle,GlyphHeight) ) );
        }

        ScrolledCellGrid* Screen::CreateScrolledCellGrid(ConstString& Name, const Rect& RendRect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            return static_cast<ScrolledCellGrid*>( this->CheckAndInsert( RenderableFactory::CreateScrolledCellGrid(Name,RendRect,Thickness,Style) ) );
        }

        PagedCellGrid* Screen::CreatePagedCellGrid(ConstString& Name, const Rect& RendRect, const Rect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            return static_cast<PagedCellGrid*>( this->CheckAndInsert( RenderableFactory::CreatePagedCellGrid(Name,RendRect,SpnRect,SStyle,GlyphHeight) ) );
        }

        DropDownList* Screen::CreateDropDownList(ConstString& Name, const Rect& RendRect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            return static_cast<DropDownList*>( this->CheckAndInsert( RenderableFactory::CreateDropDownList(Name,RendRect,LineHeight,ScrollStyle) ) );
        }

        TabSet* Screen::CreateTabSet(ConstString& Name, const Rect& SetRect)
        {
            return static_cast<TabSet*>( this->CheckAndInsert( RenderableFactory::CreateTabSet(Name,SetRect) ) );
        }

        Window* Screen::CreateWidgetWindow(ConstString& Name, const Rect& RendRect)
        {
            return static_cast<Window*>( this->CheckAndInsert( ExtendedRenderableFactory::CreateWidgetWindow(Name,RendRect) ) );
        }

        Menu* Screen::CreateMenu(ConstString& Name, const Rect& RendRect)
        {
            return static_cast<Menu*>( this->CheckAndInsert( ExtendedRenderableFactory::CreateMenu(Name,RendRect) ) );
        }//*/

        ///////////////////////////////////////////////////////////////////////////////
        // Atlas Query

        void Screen::SetPrimaryAtlas(const String& Atlas)
            { this->PrimaryAtlas = Atlas; }

        String Screen::GetPrimaryAtlas()
            { return this->PrimaryAtlas; }

        Vector2 Screen::GetWhitePixel(const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetWhitePixel(); }

        Sprite* Screen::GetSprite(const String& SpriteName,const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetSprite(SpriteName); }

        FontData* Screen::GetFont(const String& FontName,const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetFont(FontName); }

        Vector2 Screen::GetTextureSize(const String& Atlas) const
            { return this->GetAtlas(Atlas)->GetTextureSize(); }

        TextureAtlas* Screen::GetAtlas(const String& Atlas) const
            { return this->UIMan->GetAtlas(Atlas); }

        Real Screen::GetTexelOffsetX() const
            { return this->SID->RenderSys->getHorizontalTexelOffset(); }

        Real Screen::GetTexelOffsetY() const
            { return this->SID->RenderSys->getVerticalTexelOffset(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Other Query

        bool Screen::IsMarkupParserRegistered(const String& ParserName) const
            { return this->UIMan->IsMarkupParserRegistered(ParserName); }

        MarkupParser* Screen::GetMarkupParser(const String& ParserName) const
            { return this->UIMan->GetMarkupParser(ParserName); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void Screen::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->QuadRenderable::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( Screen::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("WindowTitle").SetValue( this->GameViewport->GetParentWindow()->GetWindowCaption() ) &&
                PropertiesNode.AppendAttribute("ViewportZOrder").SetValue( this->GameViewport->GetZOrder() ) &&
                PropertiesNode.AppendAttribute("PriAtlas").SetValue( this->PrimaryAtlas ) )
            {
                XML::Node VertexTransformNode = PropertiesNode.AppendChild("VertexTransform");
                this->VertexTransform.ProtoSerialize( VertexTransformNode );
                XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
                this->Scale.ProtoSerialize( ScaleNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",Screen::GetSerializableName() + "Properties",true);
            }
        }

        void Screen::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->QuadRenderable::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( Screen::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    String WindowTitle;
                    Whole ViewZOrder = 0;

                    // Get the single data type properties
                    CurrAttrib = PropertiesNode.GetAttribute("PriAtlas");
                    if( !CurrAttrib.Empty() )
                        this->PrimaryAtlas = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("WindowTitle");
                    if( !CurrAttrib.Empty() )
                        WindowTitle = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("ViewportZOrder");
                    if( !CurrAttrib.Empty() )
                        ViewZOrder = CurrAttrib.AsWhole();

                    // Get the properties that need their own nodes
                    XML::Node VertexTransformNode = PropertiesNode.GetChild("VertexTransform").GetFirstChild();
                    if( !VertexTransformNode.Empty() )
                        this->VertexTransform.ProtoDeSerialize(VertexTransformNode);

                    XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                    if( !ScaleNode.Empty() )
                        this->Scale.ProtoDeSerialize(ScaleNode);

                    if( !WindowTitle.empty() ) {
                        Graphics::GraphicsManager* GraphicsMan = Graphics::GraphicsManager::GetSingletonPtr();
                        Graphics::GameWindow* NamedWindow = GraphicsMan->GetGameWindow(WindowTitle);
                        if( NamedWindow != NULL ) {
                            this->GameViewport = NamedWindow->GetViewportByZOrder(ViewZOrder);
                            if( this->GameViewport != NULL ) {
                                this->ActDims.Size.X = (Real)this->GameViewport->GetActualWidth();
                                this->ActDims.Size.Y = (Real)this->GameViewport->GetActualHeight();
                                this->InverseSize.X = 1 / this->ActDims.Size.X;
                                this->InverseSize.Y = 1 / this->ActDims.Size.Y;
                            }else{
                                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The Viewport specified via ZOrder was not found in the named GameWindow.");
                            }
                        }else{
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"The named GameWindow to be used by UI Screen was not found.");
                        }
                    }else{
                        MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"A GameWindow Title/Caption was not specified for UI Screen.");
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (Screen::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,Screen::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String Screen::GetDerivedSerializableName() const
            { return Screen::GetSerializableName(); }

        String Screen::GetSerializableName()
            { return "Screen"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions

        void Screen::_MarkDirty()
        {
            this->Dirty = true;
            this->MouseStrat->_NotifyScreenDirty();
        }

        void Screen::_RenderScreen()
        {
            bool Force = false;
            if(Orientation != this->GameViewport->GetOrientationMode() )
            {
                this->Orientation = GameViewport->GetOrientationMode();
                if(this->Orientation == Mezzanine::OM_Degree_90)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTools::GetHalfPi(),Vector3::Unit_Z()));
                else if(this->Orientation == Mezzanine::OM_Degree_180)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTools::GetPi(),Vector3::Unit_Z()));
                else if(this->Orientation == Mezzanine::OM_Degree_270)
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(MathTools::GetPi() * 1.5,Vector3::Unit_Z()));
                else
                    VertexTransform.SetTransform(Vector3(0,0,0),Scale,Quaternion(0,0,0,1));
                Force = true;
            }
            this->_RenderVertices(Force);
            size_t KnownVertexCount = this->SID->RenderOp.vertexData->vertexCount;
            if(this->SID->RenderOp.vertexData->vertexCount)
            {
                if(this->TextureByVertex.size() == 0)
                {
                    AtlasAndPosition MyObject;
                    MyObject.RenderStart = 0;
                    MyObject.RenderEnd = KnownVertexCount;
                    MyObject.Atlas = this->PrimaryAtlas;
                    this->TextureByVertex.push_back(MyObject);
                }
                PrepareRenderSystem();
                String CurrAtlas = this->PrimaryAtlas;
                for( int i = 0 ; i < this->TextureByVertex.size() ; i++ )
                {
                    String& CurrVertAtlas = this->TextureByVertex[i].Atlas;
                    if(CurrVertAtlas.empty())
                    {
                        CurrVertAtlas = this->PrimaryAtlas;
                    }
                    if(CurrVertAtlas != CurrAtlas)
                    {
                        CurrAtlas = CurrVertAtlas;
                        Ogre::TexturePtr TextureUse = this->UIMan->GetAtlas(CurrAtlas)->_GetTexture();
                        this->SID->RenderSys->_setTexture(0,true,TextureUse);
                    }
                    this->SID->RenderOp.vertexData->vertexCount = this->TextureByVertex[i].RenderEnd - TextureByVertex[i].RenderStart;
                    this->SID->RenderOp.vertexData->vertexStart = this->TextureByVertex[i].RenderStart;
                    this->SID->RenderSys->_render(this->SID->RenderOp);
                }
            }
        }

        void Screen::_SetOrientation(const Mezzanine::OrientationMode& Mode)
        {
            this->Orientation = Mode;
            if( this->Orientation == Mezzanine::OM_Degree_90 || this->Orientation == Mezzanine::OM_Degree_270 )
            {
                std::swap(this->ActDims.Size.X,this->ActDims.Size.Y);
                std::swap(this->InverseSize.X,this->InverseSize.Y);
            }
        }

        void Screen::_Transform(ScreenRenderData& RenderData, const Whole& Begin, const Whole& End)
        {
            static const Matrix4x4 Iden;
            Whole X;
            if( Begin != End )
            {
                for( X = Begin ; X < End ; X++ )
                {
                    RenderData[X].Vert.Position.X = ((RenderData[X].Vert.Position.X) * this->InverseSize.X) * 2 - 1;
                    RenderData[X].Vert.Position.Y = ((RenderData[X].Vert.Position.Y) * this->InverseSize.Y) * -2 + 1;
                }
            }
            if( this->VertexTransform != Iden )
            {
                for( X = Begin ; X < End ; X++ )
                    RenderData[X].Vert.Position = this->VertexTransform * RenderData[X].Vert.Position;
            }
        }

        void Screen::_RenderVertices(bool Force)
        {
            if( !this->Dirty )
                return;

            Whole KnownVertexCount = 0;
            //Whole PreviousTally = 0;
            String CurrentName = this->PrimaryAtlas;
            AtlasAndPosition MyObject(this->PrimaryAtlas);
            this->TextureByVertex.clear();

            ScreenRenderData TempVertexCache;
            this->_AppendRenderDataCascading(TempVertexCache);
            KnownVertexCount = TempVertexCache.Size();

            this->ResizeVertexBuffer(KnownVertexCount);
            //OgreVertex* WriteIterator = (OgreVertex*) this->SID->VertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            Vertex* WriteIterator = (Vertex*) this->SID->VertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            for( Whole Index = 0 ; Index < TempVertexCache.Size() ; ++Index )
            {
                if( TempVertexCache[Index].Atlas.empty() )
                {
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Null or Empty String Atlas found when rendering UI.");
                }
                if( TempVertexCache[Index].Atlas != CurrentName )
                {
                    if( Index != 0 )
                    {
                        MyObject.RenderEnd = Index;
                        this->TextureByVertex.push_back(MyObject);
                    }
                    MyObject.Atlas = TempVertexCache[Index].Atlas;
                    MyObject.RenderStart = Index;
                    CurrentName = TempVertexCache[Index].Atlas;
                }

                /*OgreVertex NewVertex;
                NewVertex.Position = TempVertexCache[Index].Vert.Position.GetOgreVector3();
                NewVertex.Colour = TempVertexCache[Index].Vert.Colour.GetOgreColourValue();
                NewVertex.UV = TempVertexCache[Index].Vert.UV.GetOgreVector2();
                *WriteIterator++ = NewVertex;// */
                *WriteIterator++ = TempVertexCache[Index].Vert;
            }
            MyObject.RenderEnd = KnownVertexCount;
            MyObject.Atlas = CurrentName;
            this->TextureByVertex.push_back(MyObject);

            this->SID->VertexBuffer->unlock();
            this->SID->RenderOp.vertexData->vertexCount = KnownVertexCount;

            this->Dirty = false;
        }
    }//UI
}//Mezzanine

#endif
