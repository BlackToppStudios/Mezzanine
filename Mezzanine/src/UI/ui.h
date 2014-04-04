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
#ifndef _ui_h
#define _ui_h

namespace Mezzanine
{
    /// @namespace Mezzanine::UI
    /// @brief This namespace is for all the classes belonging to the Graphical User Interface Subsystem.
    /// @details Our GUI subsystem is based on a heavily modified/forked version of Gorilla, and as such uses
    /// a similiar structure of classes and layout for it's config files(e.g. .mta files).
    namespace UI
    {

    }
}

#include "UI/uienumerations.h"
#include "UI/uimanager.h"

#include "UI/action.h"
#include "UI/actionhandler.h"
#include "UI/brutestrategy.h"
#include "UI/button.h"
#include "UI/character.h"
#include "UI/charactertraits.h"
#include "UI/checkbox.h"
#include "UI/defaultmarkupparser.h"
#include "UI/dropdownlist.h"
#include "UI/editbox.h"
#include "UI/font.h"
#include "UI/glyph.h"
#include "UI/gridcontainer.h"
#include "UI/horizontalcontainer.h"
#include "UI/horizontallayoutstrategy.h"
#include "UI/horizontalscrollbar.h"
#include "UI/hotkeyhandler.h"
#include "UI/imagelayer.h"
#include "UI/kerning.h"
#include "UI/layoutcontainer.h"
#include "UI/layoutstrategy.h"
#include "UI/linearcontainer.h"
#include "UI/linelist.h"
#include "UI/listbox.h"
#include "UI/markupparser.h"
#include "UI/menuentry.h"
#include "UI/mousehoverstrategy.h"
#include "UI/multiimagelayer.h"
#include "UI/multilinetextlayer.h"
#include "UI/nineboxstrategy.h"
#include "UI/pagedcontainer.h"
#include "UI/pageprovider.h"
#include "UI/positioninginfo.h"
#include "UI/quadrenderable.h"
#include "UI/quadrenderer.h"
#include "UI/radiobutton.h"
#include "UI/rect.h"
#include "UI/renderable.h"
#include "UI/renderlayer.h"
#include "UI/renderlayergroup.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"
#include "UI/simplerenderer.h"
#include "UI/singleimagelayer.h"
#include "UI/singlelinetextlayer.h"
#include "UI/sizinginfo.h"
#include "UI/spinner.h"
#include "UI/sprite.h"
#include "UI/stackbutton.h"
#include "UI/stackedcontainer.h"
#include "UI/tabset.h"
#include "UI/textcursor.h"
#include "UI/textlayer.h"
#include "UI/textline.h"
#include "UI/texttoken.h"
#include "UI/textureatlas.h"
#include "UI/textureatlashandler.h"
#include "UI/unifieddim.h"
#include "UI/vertex.h"
#include "UI/verticalcontainer.h"
#include "UI/verticallayoutstrategy.h"
#include "UI/verticalscrollbar.h"
#include "UI/widget.h"
#include "UI/widgetfactory.h"
#include "UI/window.h"

/// @page UIManual Mezzanine::UI Manual
/// This will describe the use and purpose of the UI components of the Mezzanine.
/// @section UITOC Table Of Contents
///     - @ref UIOverview
///         - @ref UIIntroduction
///         - @ref UIFeedBack
///         - @ref UIAcknowledgments
///     - @ref UICore
///         - @ref UIClassUIManager
///         - @ref UIClassScreen
///         - @ref UIClassQuadRenderable
///         - @ref UIClassWidget
///     - @ref UIInputLife
///     - @ref UIRender
///         - @ref UILayers
///             - @ref UIImageLayer
///             - @ref UITextLayer
///         - @ref UILayerGroups
///         - @ref UICaching
///     - @ref UIConfig
///         - @ref UISizePos
///         - @ref UILayout
///         - @ref UIHover
///         - @ref UIFactories
///         - @ref UIMarkup
///     - @ref UIContainer
///         - @ref UIPaged
///         - @ref UIStacked
///
/// \n \n
/// @section UIOverview Overview
///     - @ref UIIntroduction
///     - @ref UIFeedback
///
/// @subsection UIIntroduction Introduction
/// The Mezzanine UI subsystem is designed to be a powerful and feature rich Graphical User Interface system that can be used to power all the UI needs of a given game.
/// Mezzanine::UI attempts to assemble render operations in as few batches as possible for maximum performance.  Mezzanine::UI also provides a default set of Widgets to
/// use when assembling an in-game UI that should meet most basic needs, and can be extended if the needs of a game are not met.
/// \n \n
/// @subsection UIFeedback Feedback
/// If you believe you've found a bug in Mezzanine::UI (bugs include compilation problems (errors/warnings), crashes, performance degradation and incorrect
/// behavior), please contact Blacktopp Studios Inc ( http://www.blacktoppstudios.com/ ) . We check the the Forums ( http://www.blacktoppstudios.com/?page_id=753 )
/// and items sent by our contact form ( http://www.blacktoppstudios.com/?page_id=33 ) regularly. Be sure to include the relevant information so that
/// the bug can be reproduced: the version of Mezzanine::UI, compiler version and target architecture, the code that uses Mezzanine::UI and exhibits the bug, etc.
/// \n \n
/// Feature requests can be reported the same way as bugs, so if you're missing some functionality in Mezzanine::UI or if the API is rough in some places
/// and you can suggest an improvement, please let us know. However, please note that there are many factors when considering API changes (compatibility
/// with previous versions, API redundancy, etc.).
/// \n \n
/// If you have a contribution to Mezzanine::UI, such as build script for some build system/IDE, or a well-designed set of helper functions, or a binding
/// to some language other than C++, please let us know. You can include the relevant patches as issue attachments. We will have to communicate on the
/// Licensing terms of your contribution though.
/// \n \n
/// @subsection UIAcknowledgments Acknowledgments
/// By no means does the author of Mezzanine::UI consider themselves an expert on UI systems nor claims all of the concepts used in Mezzanine::UI are completely original.
/// In fact the first iteration of Mezzanine::UI was a paper thin wrapper around Gorilla; a simple UI rendering library written for Ogre3D.  This wrapper was expanded with
/// an early attempt to add typical behaviors such as detecting key presses and mouse hovers was made and something resembling a working system came out of it.  But that
/// early system was still severely limited and did not have much in the way of forward thinking applied to it.  It was an academic exercise as it was a hack.  Maintaining it
/// was impractical.
/// \n \n
/// So actual planning was put forth to create a sane system the core of which could stand the test of time without the need for additional extensive refactors.  To do this an
/// extensive amount of research was done to better understand how others resolved the issues I had encountered, as well as the issues they encountered from thier early choices,
/// and issues they completely dodged.  As a result, there are concepts from a large number of other sources/libraries.  Core input handling was inspired by Torque3D.  Event
/// handling was inspired by CEGUI.  RenderLayer was inspired by Unity3D 4.  LayoutStrategy was inspired by Qt.  TextLayer, TextLine, and TextCursor was inspired by QuickGui.
/// The RenderData dirty/caching system was inspired by Gorilla.
/// \n \n
/// Thank you to everyone responsible for the creation and ongoing existence of these libraries and sharing their thought process for the benefit of others.  Without them
/// Mezzanine::UI could not exist in it's current form, instead being left as a poorly thought out hack.
///
/// \n \n
/// @section UICore The Core Classes
///     - @ref UIClassUIManager
///     - @ref UIClassScreen
///     - @ref UIClassQuadRenderable
///     - @ref UIClassWidget
///
/// @subsection UIClassUIManager UIManager
/// The @ref Mezzanine::UI::UIManager is the core class at the top of the hierarchy for the UI subsystem.  The UIManagers primary responsibilities are the storage of UI Screens,
/// initial input handling, and Button activation code auto registration.  The UIManager does not require any special parameters to initialize in code or XML.  It does however
/// expect the GraphicsManager to have been initialized prior to a UI Screen being constructed.  Further details on the initialization and use of Screens can be found in the
/// subsection below: @ref UIClassSCreen .
/// \n \n
/// The UIManager only stores a single WorkUnit for the updating of every visible Screen and the Widgets it contains.  This UI WorkUnit depends upon the Input System/WorkUnit
/// and then when it runs automatically injects all of the new inputs detected since the last frame update into itself.  Inputs may also be injected manually (see
/// @ref Mezzanine::UI::UIManager::InjectInput(const Input::MetaCode&) ) at any time the WidgetUpdateWorkUnit isn't executing to simulate any input a MetaCode can represent.
/// More information on the lifetime of an Input in the UI system can be found here: @ref UIInputLife .
/// \n \n
/// The UI::Button class has a concept of Activation and Deactivation, however there is no default input for this.  On the UI::Button you can set whatever code for Activation
/// and Deactivation you need for your game or application.  Although, usually for the sake of user experience you will want to set the same activation code or codes for each
/// UI::Button in your configuration.  There are a set of methods on the UIManager to facilitate this.  The methods controlling the Auto Register behavior are as follows: \n
///     @ref Mezzanine::UI::UIManager::EnableButtonAutoRegister(Boole) \n
///     @ref Mezzanine::UI::UIManager::ButtonAutoRegisterEnabled() const \n
///     @ref Mezzanine::UI::UIManager::AddAutoRegisterCode(const Input::MetaCode&) \n
///     @ref Mezzanine::UI::UIManager::RemoveAutoRegisterCode(const Input::MetaCode&) \n
///     @ref Mezzanine::UI::UIManager::RemoveAllAutoRegisterCodes() \n
///     @ref Mezzanine::UI::UIManager::GetAutoRegisteredCodes() const \n
/// \n
/// It is important to note that these methods are not retroactive in any way.  Adding or removing an Auto-Register code will not update all of the created buttons.  Any change
/// to the configuration you need to do will need to be done manually.  Ideally you should set the Button Auto-Register codes shortly after the construction of the UIManager and
/// before construction of any UI Screens to ensure they all get the same codes.
/// \n \n
/// Throughout the entire UI system there can only be a single widget that is being focused and is given input priority.  That widget is evaluated, stored, and can be accessed
/// through the UIManager using the @ref Mezzanine::UI::UIManager::GetWidgetFocus() const method.  The focus widget and the hovered widget may not be the same widget, and you
/// may want to access them separately.  To retrieve the hovered widget use the @ref Mezzanine::UI::UIManager::GetHoveredWidget() const method.  Both the focus and hovered widget
/// (be them separate widgets or not) are in part evaluated from hover checks.  Hover checks are primarily performed by UI Screens, however a system-wide hover check can be called
/// on using the @ref Mezzanine::UI::UIManager::CheckWidgetUnderPoint(Graphics::Viewport*, const Vector2&) method.  Note that this method is already called on once per update with
/// the appropriate parameters IF the mouse is determined to have moved since the last update.  As such you shouldn't need to call on this method manually, but can if the need
/// arises.  Calling @ref Mezzanine::UI::UIManager::CheckWidgetUnderPoint(Graphics::Viewport*, const Vector2&) will not update the stored focus or hovered widgets.  Screens on the
/// specified viewport will be checked based on ZOrder, with the highest order Screens checked first.
///
/// \n \n
/// @subsection UIClassScreen Screen
/// The @ref Mezzanine::UI::Screen class is the top of the Widget hierarchy and the primary class responsible for the rendering of Widgets.  The UI Screen is created from the
/// UIManager, and requires a .mta (Mezzanine Texture Atlas) file to have already been loaded.  MTA files can be loaded from the UIManager using the
/// @ref Mezzanine::UI::UIManager::LoadMTA(const String&,const String&) method.  Additionally a viewport needs to be specified and will be where all of the Widgets belonging to that
/// screen will be rendered to.  Lastly, a unique name must be specified to create a UI Screen.  If a name already in use is specified, an exception will be thrown.  If the specified
/// texture atlas isn't available, an exception will be thrown.  If an invalid viewport is provided, a segmentation fault will (likely) occur.
/// \n \n
/// You can place multiple screens on the same viewport, but you cannot specify multiple viewports for the same screen.  Screens can be given a ZOrder to resolve render behavior of
/// overlapping Screens.  The highest ZOrder will appear at the top, unobstructed, with the lower screens appearing beneath it as transparency permits.  This feature could be used
/// to enable things like stat displays where you want them to always be visible across multiple HUDs but would otherwise require duplicate widgets on each Screen/HUD.
/// \n \n
/// The primary responsibilities of the UI Screen class are the creation and storage of Widgets and their factories, performing hover checks, and collecting of render data from child
/// Widgets to be passed to the graphics subsystem.  Additional in depth information about Widgets and their factories can be found in the following sections: @ref UIClassWidget
/// and/or @ref UIFactories .
/// \n \n
/// Hover checks performed by the UI Screen class is somewhat modular and multiple strategies can be deployed using the @ref Mezzanine::UI::MouseHoverStrategy class.  There is a single
/// pure virtual method ( @ref Mezzanine::UI::MouseHoverStrategy::FindHoveredWidget(const Vector2&) ) that must be defined in a derived class for it to operate.  MouseHoverStrategies
/// can be set and retrieved on a given Screen using the @ref Mezzanine::UI::Screen::SetMouseHoverStrategy(MouseHoverStrategy*) and @ref Mezzanine::UI::Screen::GetMouseHoverStrategy() const
/// , respectively.  Manual hover checks for a single Screen can be done using the @ref Mezzanine::UI::Screen::FindHoveredWidget(const Vector2&) method, which accepts an arbitrary point,
/// and calls the FindHoveredWidget method on the currently set MouseHoverStrategy.  This is the same method that is called on when a system-wide Hover check is performed from the UIManager
/// so it will be called automatically when the mouse has been moved.  Manual calling should only be used for usual setups where a check is desired even if the mouse hasn't moved or if
/// multiple checks in a single frame are required.  Only a single MouseHoverStrategy can be applied to a screen at a time.
/// \n \n
/// There are 2 MouseHoverStrategy classes that have already been made and are available for use in Mezzanine::UI: @ref Mezzanine::UI::BruteStrategy and @ref Mezzanine::UI::NineBoxStrategy .
/// The BruteStrategy class is extremely simple and simply performs a check on all child Widgets of the Screen from the highest ZOrder to the lowest ZOrder, stopping only when it finds a
/// valid result or exhausts all the children to be searched.  The NineBoxStrategy is a little more sophisticated and attempts to divide the screen up into 9 partitions in the form of a
/// 3x3 grid.  On either the first run or whenever the Screen is updated it will remove it's old cache and iterate over all Widgets in the screen, putting each Widget in the appropriate
/// partitions.  Each paritition is checked against the Widget, so if a Widget overlaps with multiple partitions it will be added to each.  Then when a hover check is performed it will
/// determine the partition the mouse is located it, and only check the Widgets within that partition.  Due to the costs involved in updating the cache for the NineBoxStrategy, it is only
/// recommended to use it when you have a large and mostly static UI with infrequent updates.  By default, all Screens create and use the BruteStrategy class for hover checks.
/// \n \n
/// "Render Data" collected by a UI Screen pertains to any data that is necessary for the Screen to be rendered.  As of the time of this writing, it is a std::vector of proceadurally
/// generated vertices.  However this is subject to change.  See issues #96 and #97 on the Mezzanine issue tracker ( https://github.com/BlackToppStudios/Mezzanine/issues ) for more
/// information on these potential changes.  To collect these vertices the @ref Mezzanine::UI::QuadRenderable::_AppendRenderDataCascading(ScreenRenderData&) method is used.  This method
/// iterates over every Widget in the hierarchy starting with the lowest ZOrder child of the screen.  For each Widget checked, it will first append any vertices that belong to that Widget,
/// and then pass along the render data container to that Widgets children, once again starting with the lowest ZOrder.  This causes every Widget to get checked and any valid RenderData
/// to be appended with the higher ZOrder Widgets overlapping the lower ZOrder Widgets.  More information on logic specific to QuadRenderables appending render data can be found in the
/// section below: @ref UIClassQuadRenderable .
/// \n \n
/// Once all of the RenderData has been collected from every Widget in the hierarchy the first thing that is done is the converting of Screen coordinates on each verticy to coordinates
/// that can be read by and used by the video card.  Video devices use a coordinate system the top left corner is (-1,-1), the center is (0,0), and the bottom right is (1,1).  After the
/// verticies are transformed they are assembled into a render operation that will get passed into the internal graphics subsystem.  Collecting render data is not done during any widget
/// updates.  Instead it is a part of a callback that gets called during the graphics rendering work unit (which is a monopoly work unit).
/// \n \n
/// The UI Screen class also comes with a small set of templated utility methods that can be used to perform tasks on all Widgets in the Widget hierarchy.  The methods are as follows: \n
///     @ref Mezzanine::UI::Screen::_ProcessAllChildren(Callback*) \n
///     @ref Mezzanine::UI::Screen::_ReverseProcessAllChildren(Callback*) \n
///     @ref Mezzanine::UI::Screen::_ProcessAllChildren(Widget*,Callback*) \n
///     @ref Mezzanine::UI::Screen::_ReverseProcessAllChildren(Widget*,Callback*) \n
/// \n
/// The "normal" (as in not labeled with "Reverse") processing methods will check child Widgets based on their ZOrder, from lowest to highest.  The "Reverse" processing methods will also
/// check child Widgets based on their ZOrder, but from highest to lowest.  The overloads where you specify a Widget will only check the specified Widget and it's children.  The two
/// overloads that don't allow you to specify a Widget will assume you want to process the entire hierarchy.  In all methods the Callback can be either a C-Function or a functor that
/// accepts a pointer to the QuadRenderable to be processed.
///
/// \n \n
/// @subsection UIClassQuadRenderable QuadRenderable
/// The @ref Mezzanine::UI::QuadRenderable class is the base class for Screens and Widgets, and as such both of those classes contain all of the functionality mentioned in this section.
/// The QuadRenderable has pure virtual methods, and thus cannot be initialized as is.  The primary responsibilities of the QuadRenderable class are the management of rendering related
/// classes and data, storage of transform classes, and management of child Widgets.  The QuadRenderable and Widget classes were separated in an effort to separate the rendering and input
/// response functionality in Widgets from each other.  The QuadRenderable focuses on rendering.
/// \n \n
/// The rendering related classes used by the QuadRenderable are the @ref Mezzanine::UI::RenderLayer and it's derived classes as well as the @ref Mezzanine::UI::RenderLayerGroup .
/// RenderLayer classes that are responsible for rendering a type of quad, which could be a sprite/image or text.  Multiple renderlayers can be stacked together to achieve different
/// effects, for example: an image layer can be placed at the lowest ZOrder, a border image with a transparent center can be placed at the center ZOrder, and a text layer can be placed
/// at the highest ZOrder to give the appearance of styled and bordered button that is labeled "Start".  In order to stack these RenderLayers the way described (and for them to get
/// rendered at all), they must be added to a RenderLayerGroup.  RenderLayers belonging to a QuadRenderable can be added to an abitrary number of RenderLayerGroups also belonging to
/// that QuadRenderable, and their ZOrder can be set per RenderLayerGroup.  Not all RenderLayer groups can be rendered at the same time.  In fact only one can render at a given time,
/// that one being the Active Group.  The Active Group can be set manually, or can be configured to be set automatically based on a Widget state (if you are working with a Widget, not
/// a Screen).  More on Widget States is covered in the @ref UIClassWidget section.  More details covering the initialization and configuration of RenderLayers is covered in their
/// respective subsections here: @ref UIRender .
/// \n \n
/// The position and size of a QuadRenderable/Widget is expressed as a UnifiedDim(Unified Dimension) for each relevant axis.  A UnifiedDim is simple a class that stores both Absolute
/// (pixel) and Relative (to the parent) components that could express it's size.  When a position or size is evaluated, the Relative component is calculated first (again, based on it's
/// parent) and then has the Absolute component added to it.  Relative values are generally between 0 and 1, but nothing locks them into that range.  The values can be positive or negative.
/// The Screens position and size are more or less locked, with the position always being (0,0) and it's size matching the pixel size of the Graphics::Viewport that it is attached to.  In
/// some cases individual axes on the position or size of a QuadRenderable/Widget can be generated based on other values, rather than specifying their Unified Dimensions.  The exact array
/// of possible configurations is large, and is covered in greater detail here: @ref UIConfig .
/// \n \n
/// All QuadRenderables have the ability to have Widgets added to them as children.  Child Widgets are stored and sorted based on their ZOrder from lowest to highest.  Widgets that aren't
/// in a hierarchy with a UI Screen at the top will not be rendered or have their dimensions properly evaluated if they define more than just absolute components of their dimensions.  Child
/// Widgets can be added or removed at any time to update the hierarchy.  When a QuadRenderable is destroyed, all of it's Widget children are automatically destroyed along with it.  So if
/// you wish to preserve a child Widget of a QuadRenderable being destroyed you can remove it from the QuadRenderable, then destroy it, and add it to another QuadRenderable.  The exception
/// to this is if you attempt to move a Widget from one Screen to another Screen, as Widgets cannot change Screen ownership.  There are no checks in place to prevent adding a Widget as a
/// child to a Screen it doesn't belong to, so it may appear to work, but when it updates it will send those updates to the wrong screen and could generate rendering artifacts.  Additionally
/// on destruction this could lead to segmentation faults.  Don't do this.
/// \n \n
/// There are a few additional utilities that the QuadRenderable provides.  A QuadRenderable can be made exempt from mouse hover checks using the
/// @ref Mezzanine::UI::QuadRenderable::SetMousePassthrough(Boole) method.  This does not prevent it's children from being checked however.  Additionally when a QuadRenderable has it's
/// dimensions updated by it's parent, you can make a QuadRenderable become exempt from the auto update using the @ref Mezzanine::UI::QuadRenderable::SetManualTransformUpdates(Boole)
/// method.  Typically you will only want to do this if you are creating a Widget where positions are determined by some other Widget specific metadata.  For example the Scrollbar classes
/// manually update the Scroller absolute position based on the current Scroll value after the other child Buttons have been updated.
///
/// \n \n
/// @subsection UIClassWidget Widget
/// The @ref Mezzanine::UI::Widget class is the base class for the entire UI Widget set and contains the core functionality for input response.  Widgets can be created using their respective
/// method on the UI Screen if it is among the default Widget set.  If it is not among the default Widget set then a factory for that Widget will need to be registered.  Once it is a Widget
/// of that type can be constructed under the Screen the factory is registered to using one of three polymorphic construction methods.  Those methods are as follows: \n
///     @ref Mezzanine::UI::Screen::CreateWidget(const XML::Node&) \n
///     @ref Mezzanine::UI::Screen::CreateWidget(const String&, const String&, const NameValuePairMap&) \n
///     @ref Mezzanine::UI::Screen::CreateWidget(const String&, const String&, const UnifiedRect&, const NameValuePairMap&) \n
/// \n
/// The first method listed is an XML constructor used primarily for serialization/deserialization.  It will read the name of the XML tag passed in to look up the appropriate factory,
/// and will then pass it into a deserializing constructor.  The other two methods force you to specify the typename of the Widget to be constructed.  The name and additional parameters
/// are also necessary.  Widget names must be unique among the Widgets on the screen, otherwise an exception will be thrown.  This is universal among all Widgets constructed from the
/// Screen.  The additional parameters container must be provided, but can be empty.  Some Widgets require additional parameters, and some do not.  All Widgets should provide sane defaults
/// in the absence of additional parameters being defined.  Optionally a UnifiedRect can also be specified and set to the Widget being constructed.  The base Widget class does not have any
/// undefined virtual methods, and can be initialized as is if you only need the basic Widget functionality to achieve a certain effect.  The methods to do this are as follows: \n
///     @ref Mezzanine::UI::Screen::CreateWidget(const String&) \n
///     @ref Mezzanine::UI::Screen::CreateWidget(const String&,const String&) \n
/// \n
/// Widgets do have a "WidgetState" that is used to help determine their behavior and interactions between them and the UI system.  The @ref Mezzanine::UI::Widget::WidgetState enum is a
/// bitfield that defines a set of core states (Untouched, Hovered, Focused, and Dragged), a range of reserved states to be used by derived classes, and a range of values safe for end users
/// to use for any additional configuration that may be needed.  The "Untouched" state means nothing is applied to it.  No real state of any kind.  The "Hovered" state means the mouse is
/// currently hovered over the Widget.  The "Focused" state means the Widget is the current focus for the UI system and is being given input priority.  The "Dragged" state means that the
/// mouse is being dragged over the Widget, NOT that the Widget itself is being dragged.  It is up to a given Widget to decide what a mouse drag means, be it highlighting text, or dragging
/// and dropping child Widgets.  The core states are set by the UI manager, and the reserved states are typically set by the derived class(but never the UI Manager).  More information on
/// how the UI manager sets the core states can be found here: @ref UIInputLife .
/// \n \n
/// Widget states can have RenderLayerGroups bound to them using the @ref Mezzanine::UI::Widget::BindGroupToState(const UInt32, RenderLayerGroup*) method.  An explicit combination of
/// WidgetState enum values must be used, such as ( WS_Hovered | WS_Focused ), to bind a Widget state to a RenderLayerGroup.  Once a state is set the Widget will check if a RenderLayerGroup
/// is bound to that state and make it the Active Group if one is found.  To facilitate this, two RenderLayerGroups are automatically created based on some of the core WidgetState values.
/// Here is the relevant initialization
/// code:
/// @code
/// RenderLayerGroup* NormalGroup = this->CreateRenderLayerGroup("Normal");
/// RenderLayerGroup* HoveredGroup = this->CreateRenderLayerGroup("Hovered");
///
/// this->BindGroupToState( WS_Untouched, NormalGroup);
/// this->BindGroupToState( WS_Hovered, HoveredGroup);
/// this->BindGroupToState( WS_Focused, NormalGroup);
/// this->BindGroupToState( WS_Dragged, NormalGroup);
/// this->BindGroupToState( WS_Hovered | WS_Focused, HoveredGroup);
/// this->BindGroupToState( WS_Hovered | WS_Dragged, HoveredGroup);
/// this->BindGroupToState( WS_Focused | WS_Dragged, NormalGroup);
/// this->BindGroupToState( WS_Hovered | WS_Focused | WS_Dragged, HoveredGroup);
/// @endcode
/// In general, if the state includes "WS_Hovered", it applies the "Hovered" RenderLayerGroup.  Otherwise it applies the "Normal" RenderLayerGroup.  These two RenderLayerGroups are
/// created for ALL Widgets, unless explicitly overridden in a derived class (none of the default widgets override this).  However no Widget is locked into needing these RenderLayerGroups
/// or these bindings.  This configuration can be altered after construction however is needed.  It is important to note that while no specific binding or RenderLayerGroup needs to exist,
/// there does need to be an Active Group for a Widget to pass along it's render data to the Screen when it is collected.  So if there is no RenderLayerGroup for a particular state(s),
/// when it enters that state it will set NULL to the Active Group and the Widget will become invisible.  In some cases this may be desirable, but probably not.
/// \n \n
/// When a new state is set on a Widget, parent Widgets in the hierarchy are notified using the @ref Mezzanine::UI::Widget::_NotifyChildStateChange(Widget*, const UInt32&, const UInt32&)
/// method, which internally calls the @ref Mezzanine::UI::Widget::HandleChildStateChangeImpl(Widget*, const UInt32&, const UInt32&) method.  The provided implementation for
/// "HandleChildStateChangeImpl" on the Widget class is empty and intended to be overridden if a derived class intends to make use of the information passed into it.  In addition to
/// calling "HandleChildStateChangeImpl", the "_NotifyChildStateChange" has logic for checking and invoking the same method if appropriate on the parent Widget.  So that logic doesn't
/// need to be included when overriding "HandleChildStateChangeImpl".  Currently this functionality is used by the PagedContainer and it's derived classes to fire events when their children
/// have gained focus which is useful when reacting to a Widget in a container has been "selected".
/// \n \n
/// The UI Widget class inherits from @ref Mezzanine::EventPublisher , which gives it the ability to fire events based on changes made to it.  Most of the events present on the Widget
/// that get fired are based on changes being made to the Widgets state.  The two exceptions to this are the events that fire based on it's visibility.  Checks are made carefully to ensure
/// the events are only fired when a change is actually made.  If you find an event is being fired when a change isn't made, it's a bug and please report it on our issue tracker.  To
/// subscribe to a given event you need a publisher, a subscriber, and an event name.  All default Widget classes will have Strings that are static members of the class containing the
/// name of the event.  The names of these members are prefixed with "Event".  Simply call @ref Mezzanine::EventPublisher::Subscribe passing in one of those event names and a subscriber
/// and it will become notified when that event occurs.  The Strings containing the names of the events fired by the Widget class are as follows: \n
///     @ref Mezzanine::UI::Widget::EventMouseEnter \n
///     @ref Mezzanine::UI::Widget::EventMouseExit \n
///     @ref Mezzanine::UI::Widget::EventMouseDragStart \n
///     @ref Mezzanine::UI::Widget::EventMouseDragEnd \n
///     @ref Mezzanine::UI::Widget::EventFocusGained \n
///     @ref Mezzanine::UI::Widget::EventFocusLost \n
///     @ref Mezzanine::UI::Widget::EventFocusLocked \n
///     @ref Mezzanine::UI::Widget::EventFocusUnlocked \n
///     @ref Mezzanine::UI::Widget::EventVisibilityShown \n
///     @ref Mezzanine::UI::Widget::EventVisibilityHidden \n
/// \n
/// In addition to being a @ref Mezzanine::EventPublisher , the Widget class is also a @ref Mezzanine::EventSubscriber .  The default implementation of the
/// "_HandleInput(const Input::MetaCode&)" method is empty, and must be overridden by a derived class if a Widget needs to subscribe to events belonging to another Widget.
/// \n \n
/// Aside from managing its own Widget state, the Widget class is designed to receive and respond to inputs.  The method it does this with is the
/// @ref Mezzanine::UI::Widget::_HandleInput(const Input::MetaCode&) method.  Inside this method it calls @ref Mezzanine::UI::Widget::HandleInputImpl(const Input::MetaCode&) , which is
/// intended to be overridden by derived classes and have class specific logic that will check if it is the type of input the Widget responds to, process the input if it is, and then
/// return true or false based on whether or not the input was "used".  The default implementation on the Widget base class always returns false.  If the input was not used (if the method
/// returns false) then the input will be passed upwards in the Widget hierarchy until it is consumed or reaches the Screen.  The UI manager may do additional processing if no Widget "uses"
/// the input.  More detailed information on how inputs are handled by the Widget class can be found here: @ref UIInputLife .
/// \n \n
/// In addition to the core functionality, the Widget also comes with a few utility methods relating to hovered child Widgets and setting the Stste.  There are a couple of convenient methods
/// that will check whether a child of a Widget is hovered.  The @ref Mezzanine::UI::Widget::GetHoveredSubWidget() const method will return either a pointer to an immediate child that the
/// mouse is hovered over, or NULL if the mouse is not hovered over any immediate children.  For a deeper check, you can use the @ref Mezzanine::UI::Widget::GetBottomMostHoveredWidget() method
/// which will search all children, and children of children, etc..  The return is the same, NULL if none are hovered, or a pointer to the hovered child.  \n
/// The Widget state can also be forced using the @ref Mezzanine::UI::Widget::ForceState(const UInt32) method.  This method will assign the the provided bitfield to the widget, set the
/// ActiveGroup assigned to that state, pass the notification of the state change upwards in the Widget hierarchy.  However, it will not fire any events associated with that state(s).
/// Wherever possible altering bits that belong to the core states should be avoided.  The intended use case for this method is to change/update the presence of a user defined state.
///
/// \n \n
/// @section UIRender The Render Classes
///     - @ref UILayers
///         - @ref UISingleImage SingleImageLayer
///         - @ref UIMultiImage MultiImageLayer
///         - @ref UISingleLineText SingleLineTextLayer
///         - @ref UIMultiLineText MultiLineTextLayer
///     - @ref UILayerGroups
///     - @ref UICaching
///
/// @subsection UILayers Render Layers
/// The @ref Mezzanine::UI::RenderLayer is the base class for classes that proceadurally generate vertices for quads to be rendered.  RenderLayers lack an explicit size and position of
/// their own, instead using the dimensions of the parent QuadRenderable to determine their bounds.  The bounds of a RenderLayer can be further manipulated by applying Scaling or Rotation.
/// The methods to do this are: @ref Mezzanine::UI::RenderLayer::SetRotationDegrees(const Real&) or @ref Mezzanine::UI::RenderLayer::SetRotationRadians(const Real&) and
/// @ref Mezzanine::UI::RenderLayer::SetScale(const Vector2&) .  Depending on the RenderLayer, multiple quads or a single quad can be generated.  The available set of RenderLayers are: \n
///     @ref Mezzanine::UI::MultiImageLayer \n
///     @ref Mezzanine::UI::MultiLineTextLayer \n
///     @ref Mezzanine::UI::SingleImageLayer \n
///     @ref Mezzanine::UI::SingleLineTextLayer \n
/// \n
/// @subsubsection UIImageLayer SingleImageLayer and MultiImageLayer
/// The @ref Mezzanine::UI::SingleImageLayer class is designed to present simple colours and/or a single sprite to be stretched across the bounds of the layer.  A sprite can be set via
/// pointer or name, from the primary atlas of the parent Screen or any other atlas.  However care should be taken when adding an image from an atlas that isn't the parent Screens primary
/// atlas.  Doing so will cause the batch count to increase when rendering the UI, which will adversely affect the performance of rendering the UI.  Using the @ref Mezzanine::UI::QuadCorner
/// enum you can also set colours for each corner of the layers bounds.  A convenience method for setting a gradient between two colours is also available, which will set the colour as
/// appropriate for each corner of the layer.
/// \n \n
/// The @ref Mezzanine::UI::MultiImageLayer class has a number of similar concepts to the @ref Mezzanine::UI::SingleImageLayer class.  It can take a sprite from an atlas to be rendered.
/// Colours can be set for corners and even a gradient can be set.  However it is a little more complicated to allow for multiple images to be rendered in the same layer space.  This class
/// exists for the cases where otherwise setting multiple images in the same area would require multiple QuadRenderables, and may change in a way that would make each possible look prohibitive
/// to put into the atlas.  A lot of the methods to set the sprite or colour have similar counterparts on the MultiImageLayer, with one additional parameter that is the index of the
/// @ref Mezzanine::UI::MultiImageData that is to be altered.  Instances of @ref Mezzanine::UI::MultiImageData are not allocated on the fly, and should be created in advance using the
/// @ref Mezzanine::UI::MultiImageLayer::ReserveMultiImageData(const Whole) method.  In addition to the normal things that can be set for a SingleImageLayer there are some additional parameters
/// that be set on a @ref Mezzanine::UI::MultiImageData , in the form of a size and position that can be set for that particular sprite/image.  The size and position are set using
/// @ref Mezzanine::UI::UnifiedVec2 , which uses the scaled bounds for the layer as the relative component to determine it's size and position.  Like the other parameters, the getters and
/// setters for the size and position of an image require a valid index of the image to work with.  For more information on how the @ref Mezzanine::UI::UnifiedVec2 class is used to set the
/// size and position of an image, see the @ref UISizePos section.
/// \n \n
/// @subsubsection UITextLayer SingleLineTextLayer and MultiLineTextLayer
/// The @ref Mezzanine::UI::TextLayer class is the base class for the text layer classes in the UI system and is designed to generate a single quad for each renderable character in a UTF-8
/// encoded string.  TextLayers are designed to partition the characters into the lines, and support limited markup that can be customized.  Basic renderable properties of the characters to
/// be rendered can be easily altered per-character.  Before text can be added to any text layer a default font must be set using the following constructor or methods: \n
///     - @ref Mezzanine::UI::TextLayer::TextLayer(const String& FontName, QuadRenderable*) \n
///     - @ref Mezzanine::UI::TextLayer::SetDefaultFont(FontData*) \n
///     - @ref Mezzanine::UI::TextLayer::SetDefaultFont(const String&) \n
///     - @ref Mezzanine::UI::TextLayer::SetDefaultFont(const String&,const String&) \n
/// \n
/// Note that the constructor listed here should not be called directly.  Instead the appropriate create method should be called on the QuadRenderable that takes a Font or FontName as a paramter.
/// In addition to a default font, text layers also use a default text colour to decide what colour text should be when one isn't specified in Markup.  This value defaults to white, but can be
/// set using the @ref Mezzanine::UI::TextLayer::SetTextColour(const ColourValue&) method.
/// \n \n
/// Text passed into a TextLayer is converted into a series of @ref Mezzanine::UI::Character instances.  The Character class exists to provide an abstraction for both Glyphs and Sprites to be
/// rendered within a TextLayer.  Characters can be individually highlighted, coloured, or have a custom size set using the following methods: \n
///     - @ref Mezzanine::UI::Character::SetHighlighted(Boole) \n
///     - @ref Mezzanine::UI::Character::SetCharacterColour(const ColourValue&) \n
///     - @ref Mezzanine::UI::Character::SetCustomSize(const Vector2&) \n
/// \n
/// Characters are also subject to scaling based on the configuration of their parent TextLayer.  The custom size you can set is subject to scaling.  Outside of very specific configurations, the
/// Character class shouldn't need to be manipulated directly via code.  Most of it's data is set and used by TextLines and TextLayers.  In most cases the data that is safe to alter or isn't likely
/// to get wiped the next time the TextLayer is updated is data that can changed via Markup.  More information on Markup and the @ref Mezzanine::UI::DefaultMarkupParser is available below.
/// \n \n
/// After all of the appropriate @ref Mezzanine::UI::Character instances are created, they are placed into TextLines that are stacked vertically within the bounds of the TextLayer.  Rather than
/// having a number of explicitly stated positions, positions for both Characters and TextLines are stored as offsets.  TextLines have a vertical offset from the top of the TextLayer.  TextLines are
/// assumed to fill the complete horizontal space of a TextLayer, so Characters store a horizontal offset from the left of the TextLine/TextLayer.  Offsets for TextLines are regenerated any time a
/// new TextLine is created and offsets for Characters within a given TextLine are regenerated any time a Character is inserted.  Both offsets are regenerated any time the TextLayer changes its
/// horizontal bounds.  When Characters are inserted into TextLines, only horizontal space is checked to determine if they can be inserted.  The vertical size of a TextLine is equal to the Character
/// in that TextLine with the largest vertical size.  Characters within a TextLine can have their horizontal alignment within that TextLine using the
/// @ref Mezzanine::UI::TextLine::SetAlignment(const UI::LinearAlignment) method.  The @ref Mezzanine::UI::TextLayer::SetTextLineHorizontalAlignment(const UI::LinearAlignment) method is also available
/// on the TextLayer class to set the horizontal alignment of all TextLines it is storing.
/// \n \n
/// When a UTF-8 encoded string is passed into a TextLayer and converted into a series of Character instances, the string will be tokenized with the tokens stored in a @ref Mezzanine::UI::TokenString
/// and then run through the @ref Mezzanine::UI::MarkupParser set for that TextLayer.  In order to avoid data duplication and have a minor optimization for inserting characters, TextLayers do not
/// cache the String passed into it as is.  Instead the String passed into it is cached as a @ref Mezzanine::UI::TokenString .  Calls to @ref Mezzanine::UI::TextLayer::SetText(const String&) will
/// destroy the old TokenString and tokenize/parse the new String.  Calls to @ref Mezzanine::UI::TextLayer::GetText() const will collect the raw characters from each token and concatenate each into
/// a single String that will be returned.  When a markup tag is detected by the set MarkupParser it will enable a feature that will change the look or behavior of all render characters that are
/// generated until the terminating tag is detected.  If a start tag or end tag is created and doesn't have a corresponding tag to define it's range, then all the text in that tag that would have been
/// hidden is instead rendered, and has no effect on rendered characters in that String.  As such, the MarkupParsers in Mezzanine::UI support a form of tag soup where tags can overlap with each other
/// partially.  Like most markup, the end tag for a range needs to use the "/" character to let the Parser know it is ending a range of it's type.  Not all Markup tags need to define a range, however.
/// An example of a non-range tag is available below.
/// \n \n
/// The @ref Mezzanine::UI::DefaultMarkupParser class defines 3 basic tags that can be used to manipulate text.  The tags available can change the Font or Colour of a Glyph, or insert a Sprite.  Here
/// are some examples of text that could be passed into a TextLayer using the DefaultMarkupParser:
/// @code
/// "This is a [Colour Value=ff0000]Markup[/Colour] Test."
/// "This is [Font Name=Ubuntu]another[/Font] test of the Markup."
/// "This is a bird: [Sprite Name=BirdSprite Size=10x5]"
/// "This is a [Colour Value=ff0000]test of [Font Name=Ubuntu]different[/Colour] overlapping[/Font] tags."
/// "This is a [Colour Value=ff0000]test of [Colour Value=00ff00]identical[/Colour] overlapping[/Colour] tags."
/// "This is a test of an [Colour Value=ff0000]invalid range tag."
/// @endcode
/// The first test string should produce a string where every character is the layers default character colour with the exception of the characters in the "Markup" word, which should be red. \n
/// The second text string should produce a string where every character is the layers default character colour and uses the default set font (for example "Times New Roman") for every work except
/// "another", which should use the Ubuntu font as defined in the layer atlas.  If the default font is set to "Ubuntu" in this example, there should be no distinguishable effect. \n
/// The third test should display an image/sprite of a bird (or a sprite named "BirdSprite" in the layers atlas at least) at the end of the String that is 10 pixels by 5 pixels.  The size parameter is
/// purely optional and if not defined then the characters size on the atlas will be used, unless text scaling on the parent TextLayer is enabled, in which case it will be scaled appropriately. \n
/// The fourth test string should display the "This is a" section as the layers default character colour and the layers default font.  The "test of" section should be red text in default font.  The
/// "different" word should be red text in Ubuntu font.  The "overlapping" word should be the layers default character colour in Ubuntu font.  The "tags" word should be the layers default character
/// colour in the layers default font. \n
/// The fifth test string should display the "This is a" section in the layers default character colour.  The "test of" section should be red text.  The "identical" word should be green text.  The
/// "overlapping" word should be red text.  The "tags" word should be the layers default character colour.  All text should be in the layers default font. \n
/// The sixth test string should display the entire text, including "[Colour Value=ff0000]", in the layers default font and text colour. \n
/// A few extra notes about the tags:  The names of both the tags and their parameters are case sensitive and the parameters must not have spaces when defining their values, only when separating
/// different paramters or the tag name.  For example: "[Colour Value=ff0000]" is good, while "[Colour Value = ff0000]" is bad. \n
/// More information on creating and setting a MarkupParser that isn't the default can be found here: @ref UIMarkup .
/// \n \n
/// Glyphs or Sprites can be inserted directly into a TextLayer or removed from a TextLayer using the following methods: \n
///     - @ref Mezzanine::UI::TextLayer::InsertCharacterAtIndex(const Integer, const UInt32) \n
///     - @ref Mezzanine::UI::TextLayer::InsertCharactersAtIndex(const Integer, const Char8*, const UInt32) \n
///     - @ref Mezzanine::UI::TextLayer::InsertCharactersAtIndex(const Integer, const UInt32*, const UInt32) \n
///     - @ref Mezzanine::UI::TextLayer::RemoveCharacterAtIndex(const Integer) \n
///     - @ref Mezzanine::UI::TextLayer::RemoveCharactersAtIndex(const Integer, const UInt32) \n
///     - @ref Mezzanine::UI::TextLayer::RemoveCharacterRange(const Integer, const Integer) \n
/// \n
/// Each of these methods accepts an Integer for where in the string the manipulation will take place.  Passing in a negative index will always cause the operation to occur at the last index in the
/// TextLayer.  The index used corresponds to the index among the characters that were generated from the original string, not the index of the raw string passed in.  So if you have working tags in
/// the raw string that was passed in, and they are hidden from rendering then they aren't considered for the index.  The @ref Mezzanine::UI::TextCursor class is also available to help with
/// insertion or removal operations by allowing you to insert or remove characters from either side of the cursor.  By default the cursor for a TextLayer is disabled.  You can enable the TextCursor
/// using the @ref Mezzanine::UI::TextLayer::SetCursorEnabled(Boole) method.
/// \n \n
/// Highlighting characters within a TextLayer can be done conveniently using one of the following methods: \n
///     - @ref Mezzanine::UI::TextLayer::Highlight() \n
///     - @ref Mezzanine::UI::TextLayer::Highlight(const Integer) \n
///     - @ref Mezzanine::UI::TextLayer::Highlight(const Integer, const Integer) \n
/// \n
/// Once again the integer passed in is referring to the index among the rendered characters, not the raw index of the string passed into the TextLayer.  Also, any highlighting operation will clear
/// any previously set highlights when using these methods.  The TextLayer will only attempt to keep track of a single range of highlighted characters at a time.
/// \n \n
/// There are two types of scaling that can be set for text displayed by a TextLayer: Manual and Auto.  Manual text scaling is set with the
/// @ref Mezzanine::UI::TextLayer::SetManualTextScale(const Vector2&) method and it simply applies the values of the provided vector to each of the characters within the TextLayer.  Auto text scaling
/// is set with the @ref Mezzanine::UI::TextLayer::SetAutoTextScale(const TextLayer::ScalingMode, const Real) method and will instead scale both axes evenly based on how much Y scaling is needed to
/// achieve the desired height for a given character.  The height that is used to calculate this isn't based on the pixel height of the character if it is a glyph, but rather the lineheight of the
/// font used by that character.  So you don't have to worry about strange and unevenly stretched glyphs from the same font.  If both manual and auto text scaling are enabled on a TextLayer, then the
/// auto scaling will be applied first, getting it to the desired height and then manual scaling will be applied.
/// \n \n
/// Compared to the @ref Mezzanine::UI::TextLayer base class the @ref Mezzanine::UI::SingleLineTextLayer and @ref Mezzanine::UI::MultiLineTextLayer classes are quite small; focusing what
/// what little additional functionality is needed for rendering to a single line, or multiple lines respectively.  Between the two classes there is only one bit of publicly exposed functionality.
/// The @ref Mezzanine::UI::SingleLineTextLayer::SetStartIndex(const Integer&) can be used to exclude the rendering of the first (n) characters of the string to be rendered.
/// \n \n
/// In any Widget that uses an auto-generated TextLayer to fulfill its purpose (such as the EditBox Widget) then the TextLayer will be given a ZOrder of 5 in both the Normal and Hovered groups, as
/// well as any other group that Widget automatically creates.
/// \n \n
/// @subsection UILayerGroups Render Layer Groups
/// The @ref Mezzanine::UI::RenderLayerGroup class is a grouping of RenderLayers sorted by ZOrder to achieve a specific visual effect.  RenderLayerGroups can have RenderLayers belonging to the same
/// QuadRenderable added to them.
/// \n \n
/// @subsection UICaching Local Vertex Caching
/// Local Vertex Caching is a feature available to each QuadRenderable and is designed to be a minor optimization that can be enabled for static portions of a UI.  Vertex Caching can be enabled
/// using the @ref Mezzanine::QuadRenderable::SetLocalVertexCaching(Boole) method.  This will create an instance of a RenderData that will be populated with all of the RenderData belonging to that
/// QuadRenderable and all child QuadRenderables saving the trouble of the iterations, checks, and appends that would otherwise be necessary.  When the child of a QuadRenderable with Local Vertex
/// Caching is altered, then it still has to go through all of the same processes to update the cache.  So QuadRenderables that get updated frequently (or have children that are) are not good
/// candidates for Local Vertex Caching.  By default, all QuadRenderables that are direct children of the Screen have Local Vertex Caching enabled, but this is in no way required and can be disabled
/// if it doesn't make sense for that QuadRenderable.
///
/// \n \n
/// @section UIInputLife The Lifetime of an Input
/// Input Lifetime text.
///
/// \n \n
/// @section UIConfig Configuring and Expanding
///     - @ref UISizePos
///     - @ref UILayout
///     - @ref UIFactories
///     - @ref UIMarkup
///
/// @subsection UISizePos Sizing and Positioning
/// Sizing and Positioning text.
/// \n \n
/// @subsection UILayout Layout Strategies
/// Layout Strategies text.
/// \n \n
/// @subsection UIFactories Widget Factories
/// Widget Factories text.
/// \n \n
/// @subsection UIMarkup Markup Parsers
/// Markup Parsers text.
///
/// \n \n
/// @section UIContainer Container Widgets
///     - @ref UIPaged
///     - @ref UIStacked
///
/// @subsection UIPaged Paged Containers and Providers
/// Paged Containers and Providers text.
/// \n \n
/// @subsection UIStacked Stacked Containers and Buttons
/// Stacked Containers and Buttons text.

/// Hotkeys?
/// Actions?
/// Widget Implementations?

#endif
