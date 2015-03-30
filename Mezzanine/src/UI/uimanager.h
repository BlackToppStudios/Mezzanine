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
#ifndef _uiuimanager_h
#define _uiuimanager_h

#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"
#include "UI/rect.h"
#include "Input/metacode.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    namespace Graphics
    {
        class Viewport;
    }
    namespace UI
    {
        class Screen;
        class Button;
        class Widget;
        class TextureAtlas;
        class TextureAtlasHandler;
        class HotKeyHandler;
        class ActionHandler;
        class QuadRenderable;
        class MarkupParser;
        class UIManager;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of widgets in the UI system.
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
        class MEZZ_LIB UIManager : public EntresolManager, public Singleton<UIManager>
        {
        public:
            /// @brief Basic container type for @ref MarkupParser storage by this class.
            typedef std::map<String,MarkupParser*>                 MarkupParserContainer;
            /// @brief Iterator type for @ref MarkupParser instances stored by this class.
            typedef MarkupParserContainer::iterator                MarkupParserIterator;
            /// @brief Const Iterator type for @ref MarkupParser instances stored by this class.
            typedef MarkupParserContainer::const_iterator          ConstMarkupParserIterator;
            /// @brief Basic container type for @ref Screen storage by this class.
            typedef std::vector< UI::Screen* >                     ScreenContainer;
            /// @brief Iterator type for @ref Screen instances stored by this class.
            typedef ScreenContainer::iterator                      ScreenIterator;
            /// @brief Const Iterator type for @ref Screen instances stored by this class.
            typedef ScreenContainer::const_iterator                ConstScreenIterator;
            /// @brief Reverse Iterator type for @ref Screen instances stored by this class.
            typedef ScreenContainer::reverse_iterator              ReverseScreenIterator;
            /// @brief Const Reverse Iterator type for @ref Screen instances stored by this class.
            typedef ScreenContainer::const_reverse_iterator        ConstReverseScreenIterator;
            /// @brief Basic container type for @ref Input::MetaCode storage by this class.
            typedef std::vector< Input::MetaCode >                 InputContainer;
            /// @brief Iterator type for @ref Input::MetaCode instances stored by this class.
            typedef InputContainer::iterator                       InputIterator;
            /// @brief Const Iterator type for @ref Input::MetaCode instances stored by this class.
            typedef InputContainer::const_iterator                 ConstInputIterator;
            /// @brief An std::pair type for returning the result of a font suggestion.
            typedef std::pair< String, Real >                      FontResult;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @internal
            /// @brief A container storing all the registered/known markup parsers.
            MarkupParserContainer MarkupParsers;
            /// @internal
            /// @brief A container storing all the Screens created in the UI system.
            ScreenContainer Screens;
            /// @internal
            /// @brief A container storing all of the Inputs buttons will be set to activate in response to.
            InputContainer AutoRegisterCodes;
            /// @internal
            /// @brief A container storing all of the Inputs generated/tracked during the last UI update.
            InputContainer InjectedInputs;

            /// @internal
            /// @brief Stores the Input that locked the current focus.
            Input::MetaCode FocusLockCode;
            /// @internal
            /// @brief A pointer to the current widget in the UI system the mouse is hovered over.
            Widget* HoveredWidget;
            /// @internal
            /// @brief A pointer to the widget that is currently holding the focus.
            Widget* WidgetFocus;
            /// @internal
            /// @brief A pointer to the class responsible for Atlas tasks in the UI system.
            TextureAtlasHandler* AtlasHandler;
            /// @internal
            /// @brief A pointer to the class responsible for HotKey tasks in the UI system.
            HotKeyHandler* HKHandler;
            /// @internal
            /// @brief The work unit that updates all of the widgets in the UI system.
            WidgetUpdateWorkUnit* WidgetUpdateWork;
            /// @internal
            /// @brief Can be used for thread safe logging and other thread specific resources.
            Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
            /// @internal
            /// @brief Stores whether or not newly created buttons will have activation codes binded to them automatically.
            Boole ButtonAutoRegister;
            /// @internal
            /// @brief Stores whether or not the mouse moved since the last time the UI was updated.
            Boole MouseMoved;

            /// @internal
            /// @brief Handles UI processing prior to the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePreFocusInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles UI processing after the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePostFocusInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles keyboard processes prior to the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePreFocusKeyboardInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles mouse processes prior to the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePreFocusMouseInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles controller processes prior to the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePreFocusControllerInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles keyboard processes after the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePostFocusKeyboardInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles mouse processes after the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePostFocusMouseInput(const Input::MetaCode& Code);
            /// @internal
            /// @brief Handles controller processes after the update of the main focus.
            /// @param Code The MetaCode containing the input to be processed.
            void HandlePostFocusControllerInput(const Input::MetaCode& Code);

            /// @internal
            /// @brief Updates the current focus to a new target.
            /// @param NewFocus The new widget obtaining the UI focus.
            /// @return Returns true if the target widget successfully gained focus, false otherwise.
            Boole SwitchFocus(Widget* NewFocus);
            /// @internal
            /// @brief Gets whether or not the systems current focus is locked.
            /// @return Returns true if the focus is currently locked, false otherwise.
            Boole FocusIsLocked() const;
        public:
            /// @brief Class Constructor.
            UIManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            UIManager(XML::Node& XMLNode);
            /// @brief Class Destructor.
            virtual ~UIManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Texture Atlas Management

            /// @brief Loads a Mezzanine Texture Atlas file for use with Screens.
            /// @param Name The name of the file to be loaded.
            /// @param Group The resource group where the MTA file can be found.
            void LoadMTA(const String& Name, const String& Group = "UI");

            /// @brief Gets a loaded Atlas being stored in this manager.
            /// @param AtlasName The name of the Atlas, which is usually the name of the file without the extension.
            /// @return Returns a pointer to the requested Atlas, or NULL if it doesn't exist.
            TextureAtlas* GetAtlas(const String& AtlasName);
            /// @brief Gets a loaded Atlas being stored in this manager.
            /// @param AtlasName The name of the Atlas, which is usually the name of the file without the extension.
            /// @return Returns a pointer to the requested Atlas, or throws an exception if it doesn't exist.
            TextureAtlas* GetAtlasExcept(const String& AtlasName);
            /// @brief Destroys a loaded texture atlas.
            /// @param ToBeDestroyed A pointer to the texture atlas that will be destroyed.
            void DestroyAtlas(TextureAtlas* ToBeDestroyed);
            /// @brief Destroys all loaded texture atlases.
            void DestroyAllAtlases();

            ///////////////////////////////////////////////////////////////////////////////
            // Screen Management

            /// @brief Creates an internal HUD screen.
            /// @details Screens are the base set of renderable UI you can use, allowing you to switch entire sets of UI's
            /// on the fly if needed.  For performance reasons you should always keep the number of screens you create to a minimum.
            /// @param ScreenName The name to be given to the screen.
            /// @param Atlas The name of a previously loaded mta file to be used with this screen.
            /// @param WindowViewport The viewport to create this screen in.
            /// @param ZOrder The ZOrder determining the position relative to the other screens.
            Screen* CreateScreen(const String& ScreenName, const String& Atlas, Graphics::Viewport* WindowViewport, const UInt16 ZOrder);
            /// @brief Gets an already created screen by name.
            /// @return Returns a pointer to the screen of the specified name.
            Screen* GetScreen(const String& Name) const;
            /// @brief Gets an already created screen by index.
            /// @return Returns a pointer to the screen at the specified index.
            Screen* GetScreen(const Whole& Index) const;
            /// @brief Gets the number of screens created and stored in this manager.
            /// @return Returns the number of screens this manager is storing.
            Whole GetNumScreens() const;
            /// @brief Deletes a screen and removes all trace of it from the manager.
            /// @details Destroying a screen will also destroy all of it's layers, and everything contained in those layers.
            /// @param Screen The screen to be destroyed.
            void DestroyScreen(Screen* Screen);
            /// @brief Deletes all screens stored in this manager.
            void DestroyAllScreens();

            /// @brief Shows all screens bound to a specific viewport.
            /// @param WindowViewport The viewport bound to all screens to be shown.
            /// @param Exclude Optional parameter that will make a single screen be excluded from this operation.
            void ShowScreensOnViewport(Graphics::Viewport* WindowViewport, Screen* Exclude = NULL);
            /// @brief Shows all screens owned by this manager.
            /// @param Exclude Optional parameter that will make a single screen be excluded from this operation.
            void ShowAllScreens(Screen* Exclude = NULL);
            /// @brief Hides all screens bound to a specific viewport.
            /// @param WindowViewport The viewport bound to all screens to be hidden.
            /// @param Exclude Optional parameter that will make a single screen be excluded from this operation.
            void HideScreensOnViewport(Graphics::Viewport* WindowViewport, Screen* Exclude = NULL);
            /// @brief Hides all screens owned by this manager.
            /// @param Exclude Optional parameter that will make a single screen be excluded from this operation.
            void HideAllScreens(Screen* Exclude = NULL);
            /// @brief Gets the lowest ZOrder screen that is visible on a viewport.
            /// @param WindowViewport The viewport to check for a visible screen.
            /// @return Returns a pointer to the screen at the lowest ZOrder currently being shown on the specified viewport.
            Screen* GetLowestVisibleScreenOnViewport(Graphics::Viewport* WindowViewport) const;
            /// @brief Gets the highest ZOrder screen that is visible on a viewport.
            /// @param WindowViewport The viewport to check for a visible screen.
            /// @return Returns a pointer to the screen at the highest ZOrder currently being shown on the specified viewport.
            Screen* GetHighestVisibleScreenOnViewport(Graphics::Viewport* WindowViewport) const;

            ///////////////////////////////////////////////////////////////////////////////
            // HotKey Management

            /// @brief Binds a key to a button.
            /// @details This function allows buttons to behave like they are pressed without mouse input.
            /// @param HotKey The key or button (on the input device) to activate the button.
            /// @param BoundButton The button being bound to the hotkey.
            void BindHotKey(const Input::MetaCode& HotKey, Button* BoundButton);
            /// @brief Removes a previously set hotkey binding.
            /// @param HotKey The key or button (on the input device) to activate the button.
            /// @param BoundButton The button currently bound to the hotkey.
            void UnbindHotKey(const Input::MetaCode& HotKey, Button* BoundButton);

            ///////////////////////////////////////////////////////////////////////////////
            // Activation Management

            /// @brief Enables whether or not to automatically set the activation key or button for UI buttons.
            /// @details If true, this will cause every UI button to have keyboard keys or mouse buttons added with
            /// AddButtonAutoRegister() to be added as activation keys or buttons immediately after they are created. @n
            /// Manager Default Setting: False.
            /// @param Enable Whether or not to enable this feature.
            void EnableButtonAutoRegister(Boole Enable);
            /// @brief Gets whether or not the ButtonAutoRegister feature is enabled.
            /// @return Returns a Boole indicating whether or not the ButtonAutoRegister feature is enabled.
            Boole ButtonAutoRegisterEnabled() const;
            /// @brief Adds a key or button that will be auto-registered with every created UI button.
            /// @note Buttons expect the input to be a button of some sort, but do not descriminate based on device.  If you provide an
            /// input that is not a Mouse, Keyboard, or Controller button then checks inside the button will fail and the button won't
            /// activate or deactivate.
            /// @param Code The input code for the keyboard key or mouse button to be added to the list of codes to be auto-registered.
            void AddAutoRegisterCode(const Input::MetaCode& Code);
            /// @brief Removes a previously set auto-registering input code.
            /// @param Code The input code to be removed from the list of auto-registering codes.
            void RemoveAutoRegisterCode(const Input::MetaCode& Code);
            /// @brief Removes all auto-registering input codes.
            void RemoveAllAutoRegisterCodes();
            /// @brief Gets the list of codes that will be auto-registered with each UI button.
            /// @return Returns a pointer to the vector containing all the codes to be auto-registered with every UI button.
            const InputContainer& GetAutoRegisteredCodes() const;

            ///////////////////////////////////////////////////////////////////////////////
            // MarkupParser Management

            /// @brief Registers a new MarkupParser with this Manager.
            /// @exception This method will thrown a "II_DUPLICATE_IDENTITY_EXCEPTION" if the provided name is already registered.
            /// @param ParserName The name to be given to the registered MarkupParser.
            /// @param ToAdd A pointer to the MarkupParser to be registered with the given name.
            void RegisterMarkupParser(const String& ParserName, MarkupParser* ToAdd);
            /// @brief Checks to see if a MarkupParser has already been registsered under a specific name.
            /// @param ParserName The name of the MarkupParser to check for.
            /// @return Returns true if a MarkupParser is registered under the specified name.
            Boole IsMarkupParserRegistered(const String& ParserName) const;
            /// @brief Gets a MarkupParser by it's registered name.
            /// @param ParserName The name of the MarkupParser to retrieve.
            /// @return Returns a pointer to the requested MarkupParser, or NULL if none are registered under the specified name.
            MarkupParser* GetMarkupParser(const String& ParserName) const;
            /// @brief Removes a MarkupParser from this manager.
            /// @param ParserName The name of the MarkupParser to be removed.
            void UnregisterMarkupParser(const String& ParserName);
            /// @brief Removes all MarkupParsers from this manager.
            void UnregisterAllMarkupParsers();
            /// @brief Removes and destroys a MarkupParser from this manager.
            /// @param ParserName The name of the MarkupParser to be destroyed.
            void DestroyMarkupParser(const String& ParserName);
            /// @brief Removes and destroys all MarkupParsers from this manager.
            void DestroyAllMarkupParsers();

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets the Widget the mouse is hovering over.
            /// @return Returns a pointer to the widget, or NULL if it's not over any visable buttons.
            Widget* GetHoveredWidget() const;
            /// @brief Gets the current widget being controlled.
            /// @details The widget control is used mostly for manipulating widgets while the mouse is not
            /// currently hovering over them, such as the click and drag action of scrollbars and resizing windows.
            /// @return Returns a pointer to the currently controlled widget, or NULL if none are being controlled this frame.
            Widget* GetWidgetFocus() const;
            /// @brief Gets this managers Atlas Handler.
            /// @return Returns a pointer to this managers Atlas Handler.
            TextureAtlasHandler* GetAtlasHandler() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Injects a metacode into this manager, allowing the UI system to be aware of it.
            /// @param Code The MetaCode to be injected and later processed.
            void InjectInput(const Input::MetaCode& Code);
            /// @brief Searches all visable screens and layers to see if a Widget is under the given point.
            /// @details This is called automatically once every frame.  Should only be called on manually if
            /// you need more then one check per frame.
            /// @param VP A pointer to the viewport to check.
            /// @param Point The point on screen(in viewport space) to use when checking Widgets.
            /// @return Returns the hovered Widget if there is one, NULL if not.
            Widget* CheckWidgetUnderPoint(Graphics::Viewport* VP, const Vector2& Point);
            /// @brief Checks to see if the mouse is over a UI element.
            /// @details This should only be called on after this manager does it's main loop items for best results.
            /// @return Returns true if the mouse is over a visable UI element, false if not.
            Boole MouseIsInUISystem() const;
            /// @brief Suggests a glyph index based on the desired actual height.
            /// @param Height The desired Height of the glyph wanted in pixels.
            /// @param Atlas The altas to search.
            /// @return Returns a std::pair, First is the name of the font and second is a Real for the scaling that should be provided to it.
            FontResult SuggestGlyphIndex(const Whole& Height, const String& Atlas);

            /// @brief Updates the UI system based on the most recent inputs.
            virtual void UpdateScreens();

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            /// @brief Gets the work unit responsible for updating the widgets in this manager.
            /// @return Returns a pointer to the WidgetUpdateWorkUnit used by this manager.
            WidgetUpdateWorkUnit* GetWidgetUpdateWork();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
        };//UIManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultUIManagerFactory
        /// @headerfile uimanager.h
        /// @brief A factory responsible for the creation and destruction of the default uimanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultUIManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultUIManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultUIManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(NameValuePairList&)
            EntresolManager* CreateManager(NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(XML::Node&)
            EntresolManager* CreateManager(XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        };//DefaultUIManagerFactory
    }//UI
}//Mezzanine

#endif
