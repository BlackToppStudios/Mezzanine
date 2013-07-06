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
#ifndef _uiuimanager_h
#define _uiuimanager_h

#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "UI/renderablerect.h"
#include "Input/metacode.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    class InputQueryTool;
    namespace Graphics
    {
        class Viewport;
    }
    namespace UI
    {
        class Screen;
        class Layer;
        class Button;
        class Widget;
        class Scrollbar;
        class TextureAtlas;
        class UIManager;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a @ref iWorkUnit for the updating of widgets in the UI system.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB WidgetUpdateWorkUnit : public Threading::DefaultWorkUnit
        {
        protected:
            /// @internal
            /// @brief A pointer to the manager this work unit is processing.
            UIManager* TargetManager;
            /// @internal
            /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            WidgetUpdateWorkUnit(const WidgetUpdateWorkUnit& Other);
            /// @internal
            /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            WidgetUpdateWorkUnit& operator=(const WidgetUpdateWorkUnit& Other);
        public:
            /// @brief Class constructor.
            /// @param Target The UIManager this work unit will process during the frame.
            WidgetUpdateWorkUnit(UIManager* Target);
            /// @brief Class destructor.
            virtual ~WidgetUpdateWorkUnit();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required update of the Graphical Scene graph and REnders one frame
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//WidgetUpdateWorkUnit

        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonUIManager) Singleton<UIManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @class UIManager
        /// @headerfile uimanager.h
        /// @brief This class is responsible for any and all user interactions with the User interface/HUD.
        /// @details Currently, you have to create the UI/HUD in code.  Font and sprite data is loaded through a premade mta file(*.mta).
        ///////////////////////////////////////
        class MEZZ_LIB UIManager : public ManagerBase, public Singleton<UIManager>
        {
        public:
            typedef std::vector< Screen* >                     ScreenContainer;
            typedef ScreenContainer::iterator                  ScreenIterator;
            typedef ScreenContainer::const_iterator            ConstScreenIterator;
            typedef std::map< Graphics::Viewport*, Screen* >   VisibleScreenContainer;
            typedef VisibleScreenContainer::iterator           VisibleScreenIterator;
            typedef VisibleScreenContainer::const_iterator     ConstVisibleScreenIterator;
            typedef std::vector< Button* >                     ButtonContainer;
            typedef ButtonContainer::iterator                  ButtonIterator;
            typedef ButtonContainer::const_iterator            ConstButtonIterator;
            typedef std::vector< Input::InputCode >            CodeContainer;
            typedef CodeContainer::iterator                    CodeIterator;
            typedef CodeContainer::const_iterator              ConstCodeIterator;
            typedef std::map< String, TextureAtlas* >          AtlasContainer;
            typedef AtlasContainer::iterator                   AtlasIterator;
            typedef AtlasContainer::const_iterator             ConstAtlasIterator;
            typedef std::multimap< Input::InputCode, Button* > HotKeyContainer;
            typedef HotKeyContainer::iterator                  HotKeyIterator;
            typedef HotKeyContainer::const_iterator            ConstHotKeyIterator;
            typedef std::pair< Whole, Real >                   GlyphIndexResult;
        protected:
            friend class WidgetUpdateWorkUnit;

            ScreenContainer Screens;
            VisibleScreenContainer VisibleScreens;
            ButtonContainer ActivatedButtons;
            CodeContainer AutoRegisterCodes;
            AtlasContainer Atlases;
            HotKeyContainer HotKeys;
            Widget* HoveredWidget;
            Widget* WidgetFocus;
            Widget* InputCapture;
            Widget* LastWidgetSelected;
            bool ButtonAutoRegister;

            /// @internal
            /// @brief The work unit that updates all of the widgets in the UI system.
            WidgetUpdateWorkUnit* WidgetUpdateWork;
            /// @internal
            /// @brief Can be used for thread safe logging and other thread specific resources.
            Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

            void HoverChecks();
            void HotKeyAndInputCaptureChecks();
            void WidgetUpdates();
            void ClearButtonActivations();
            void ViewportUpdateChecks();
            void MouseActivationCheck(Button* ToCheck);
            void HotKeyActivationCheck(const Input::InputCode& Code);
        public:
            /// @brief Class Constructor.
            UIManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            UIManager(XML::Node& XMLNode);
            /// @brief Class Destructor.
            ~UIManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Texture Atlas Management

            /// @brief Loads a Mezzanine Texture Atlas file for use with Screens.
            /// @param Name The name of the file to be loaded, not including the extension.
            /// @param Group The resource group where the MTA file can be found.
            void LoadMTA(const String& Name, const String& Group = "UI");
            /// @brief Gets a loaded Atlas being stored in this manager.
            /// @param AtlasName The name of the Atlas, which is usually the name of the file without the extension.
            /// @return Returns a pointer to the requested Atlas, or NULL if it doesn't exist.
            TextureAtlas* GetAtlas(const String& AtlasName);

            ///////////////////////////////////////////////////////////////////////////////
            // Screen Management

            /// @brief Creates an internal HUD screen.
            /// @details Screens are the base set of renderable UI you can use, allowing you to switch entire sets of UI's
            /// on the fly if needed.  For performance reasons you should always keep the number of screens you create to a minimum.
            /// @param ScreenName The name to be given to the screen.
            /// @param Atlas The name of a previously loaded mta file to be used with this screen.
            /// @param WindowViewport The viewport to create this screen in.
            Screen* CreateScreen(const String& ScreenName, const String& Atlas, Graphics::Viewport* WindowViewport);
            /// @brief Gets an already created screen by name.
            /// @return Returns a pointer to the screen of the specified name.
            Screen* GetScreen(const String& Name);
            /// @brief Gets an already created screen by index.
            /// @return Returns a pointer to the screen at the specified index.
            Screen* GetScreen(const Whole& Index);
            /// @brief Gets the number of screens created and stored in this manager.
            /// @return Returns the number of screens this manager is storing.
            Whole GetNumScreens();
            /// @brief Deletes a screen and removes all trace of it from the manager.
            /// @details Destroying a screen will also destroy all of it's layers, and everything contained in those layers.
            /// @param Screen The screen to be destroyed.
            void DestroyScreen(Screen* Screen);
            /// @brief Deletes all screens stored in this manager.
            void DestroyAllScreens();

            /// @brief Gets the currently visible screen on the provided viewport.
            /// @param WindowViewport The viewport to check for a visible screen.
            /// @return Returns a pointer to the screen currently being shown on the requested viewport.
            Screen* GetVisibleScreenOnViewport(Graphics::Viewport* WindowViewport);

            ///////////////////////////////////////////////////////////////////////////////
            // HotKey and Activation Management

            /// @brief Binds a key to a button.
            /// @details This function allows buttons to behave like they are pressed without mouse input.
            /// @param HotKey The key or button (on the input device) to activate the button.
            /// @param BoundButton The button being bound to the hotkey.
            void BindHotKey(const Input::InputCode& HotKey, Button* BoundButton);
            /// @brief Removes a previously set hotkey binding.
            /// @param HotKey The key or button (on the input device) to activate the button.
            /// @param BoundButton The button currently bound to the hotkey.
            void UnbindHotKey(const Input::InputCode& HotKey, Button* BoundButton);
            /// @brief Clears all registered hotkeys.
            void RemoveAllHotKeys();

            /// @brief Enables whether or not to automatically set the activation key or button for UI buttons.
            /// @details If true, this will cause every UI button to have keyboard keys or mouse buttons added with
            /// AddButtonAutoRegister() to be added as activation keys or buttons immediately after they are created. @n
            /// Default: False.
            /// @param Enable Whether or not to enable this feature.
            void EnableButtonAutoRegister(bool Enable);
            /// @brief Gets whether or not the ButtonAutoRegister feature is enabled.
            /// @return Returns a bool indicating whether or not the ButtonAutoRegister feature is enabled.
            bool ButtonAutoRegisterEnabled();
            /// @brief Adds a key or button that will be auto-registered with every created UI button.
            /// @param Code The input code for the keyboard key or mouse button to be added to the list of codes to be auto-registered.
            void AddAutoRegisterCode(Input::InputCode Code);
            /// @brief Removes a previously set auto-registering input code.
            /// @param Code The input code to be removed from the list of auto-registering codes.
            void RemoveAutoRegisterCode(Input::InputCode Code);
            /// @brief Removes all auto-registering input codes.
            void RemoveAllAutoRegisterCodes();
            /// @brief Gets the list of codes that will be auto-registered with each UI button.
            /// @return Returns a pointer to the vector containing all the codes to be auto-registered with every UI button.
            CodeContainer* GetAutoRegisteredCodes();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Forces everything loaded into the UI system to be redrawn.
            /// @param Force If Force is set to true, it will redraw everything regardless of if it has changed.
            void RedrawAll(bool Force = false);
            /// @brief Searches all visable screens and layers to see if a widget was clicked.
            /// @details This is called automatically once every frame.  Should only be called on manually if
            /// you need more then one check per frame.
            /// @return Returns the widget clicked if there is one, NULL if not.
            Widget* CheckWidgetMouseIsOver();
            /// @brief Checks to see if the mouse is over a UI element.
            /// @details This should only be called on after this manager does it's main loop items for best results.
            /// @return Returns true if the mouse is over a visable UI element, false if not.
            bool MouseIsInUISystem();
            /// @brief Suggests a glyph index based on the desired actual height.
            /// @param Height The desired Height of the glyph wanted in pixels.
            /// @param Atlas The altas to search.
            /// @return Returns a std::pair, First is a whole for the Glyph index and second is a Real for the scaling that should be provided to it.
            GlyphIndexResult SuggestGlyphIndex(const Whole& Height, const String& Atlas);

            /// @brief Gets the work unit responsible for updating the widgets in this manager.
            /// @return Returns a pointer to the WidgetUpdateWorkUnit used by this manager.
            WidgetUpdateWorkUnit* GetWidgetUpdateWork();

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets the Widget the mouse is hovering over.
            /// @details If the widget found during widget checks belongs to a widget, this will get that widget.
            /// @return Returns a pointer to the widget, or NULL if it's not over any visable buttons.
            Widget* GetHoveredWidget();
            /// @brief Gets the current widget being controlled.
            /// @details The widget control is used mostly for manipulating widgets while the mouse is not
            /// currently hovering over them, such as the click and drag action of scrollbars and resizing windows.
            /// @return Returns a pointer to the currently controlled widget, or NULL if none are being controlled this frame.
            Widget* GetWidgetFocus();
            /// @brief Gets the widget that is currently capturing input.
            /// @details A widget that is currently capturing input will prevent hotkeys from working while active.
            /// @return Returns a pointer to the widget that currently capturing input, or NULL if no widget is capturing input.
            Widget* GetWidgetCapturingInput();

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from ManagerBase

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::DoMainLoopItems()
            virtual void DoMainLoopItems();
            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Sets the screen that will be visible on the viewport it is bound to.
            /// @remarks Only one screen may be visible at a time on a given viewport.  This function updates the
            /// other screens so they are hidden when this screen becomes visible.
            /// @param BeingShown The screen being set to visible.
            /// @param Visible The state of the screen's visibility.
            void _NotifyScreenVisibility(Screen* BeingShown, bool Visible);
        };//UIManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultUIManagerFactory
        /// @headerfile uimanager.h
        /// @brief A factory responsible for the creation and destruction of the default uimanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultUIManagerFactory : public ManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultUIManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultUIManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;
            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
        };//DefaultUIManagerFactory
    }//UI
}//Mezzanine

#endif
