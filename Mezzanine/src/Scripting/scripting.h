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
#ifndef _scripting_h
#define _scripting_h

#ifdef MEZZLUA51
    #define LINKLUAMANUAL ///   - @ref LuaManual
#else
    #define LINKLUAMANUAL ///   - Lua Not Compiled in this build - @ref LuaManual
#endif

namespace Mezzanine
{
    /// @namespace Mezzanine::Scripting
    /// @brief This namespace is for all the classes belonging to the Scripting Subsystem.
    /// @details The system for integrating scripting languages into the engine.
    /// @n @n
    /// In general each languages Interpretter/Engine/Compiler/VM is locating in its own
    /// subnamespace in the associated manager. Here is a list of Languages compiled in
    /// this build.
    /// @n @n
    ///    - @ref ScriptingManual
    ///
    LINKLUAMANUAL



    namespace Scripting
        {}
}

/// @page ScriptingManual
/// The scripting system is included the Mezzanine::Scripting namespace. It is primarily
/// composed of four interface classes, four abstract classes. An implementation of a
/// scripting system should implement these four classes in a sub-namespacef
///     - Mezzanine::Scripting::iScript
///     - Mezzanine::Scripting::iScriptArgument
///     - Mezzanine::Scripting::iScriptingManager
///     - Mezzanine::Scripting::iScriptWorkUnit
///
/// Many scripting languages suitable for being embedded in games have a number of
/// similar traits. All have some kind of runtime that mustbe initialized, this
/// runtime accepts strings of script source and performs the steps the source
/// desribe. Some keep internal track of the source other expect the the caller to
/// do so. Most allow passing arguments into scripts and accepting return values.
/// Some allow the script to be compiled to a byte and others do not. A scripting manager is
/// @n @n
/// Whether or not this is updated each build
/// can be controlled by the Mezz_SWIG CMake Flag. When this flag
/// is enabled a SWIG_Mezzanine build target is added to the project. To satisfy
/// build systems this target/project compiles src/blank.cpp as the only files.
/// During the build steps in that target it attempt to Swig from the command line.
/// @n @n
/// This can be run manually by invoking that target specifically. For example:
/// @code
/// $make SWIG_Mezzanine
/// ninja SWIG_Mezzanine
/// @endcode
/// @section CppApiToLua How the C++ API converts to Lua
/// The Documentation for the C++ API is also the canonical documentation for the
/// Lua scripting system. The are some differences those that are not and these
/// detailed on the functions that are different are generalized here.
/// @subsection CApiLuaNamepacesBecomeModules Lua Modules are Related to Namespaces
/// Major namespaces become modules in Lua. You can check on the Lua51ScriptingEngine
/// if there is a function for loading a specific namespace. For example if you want
/// a script to use classes and functions in the Mezzanine::Threading namespace
/// then you will need to load either the safe or the unlimited threading libraries
/// in Lua. This can be done either by passing the right flags to the constructor of
/// the Lua51ScriptingEngine or by calling Lua51ScriptingEngine::OpenMezzanineThreadingLibrary()
/// or Lua51ScriptingEngine::OpenMezzanineThreadingSafeLibrary() . Each Module
/// in Lua stores all of its members in a table of the module's name, and a
/// reference to the table is stored in the Mezzanine table. This is preferred way
/// to call Lua functionality from modules, and calling the table directly may be
/// removed in the future. Here is an example of using a class in the Threading
/// and another in the root namespace:
/// @code
/// bar = MezzanineSafe.Threading.Barrier(4)
/// vec = MezzanineSafe.Vector3(1,1,1)
/// @endcode
/// @n @n
/// @subsection CApiLuaNamespacesFlattened Other Namespaces Are Flattened
/// Namespaces are flattened into the Lua module they are included n:
/// C++ code:
/// @code
/// Mezzanine::Vector3 Vec = Mezzanine::StringTools::ConvertToVector3("3 4 6")
/// @endcode
/// @n @n
/// Lua Code:
/// @code
/// Vec=MezzanineSafe.ConvertToVector3("3 4 6")
/// @endcode
/// @n @n
/// @subsection CApiLuaEnumsFlattened Enums Are Flattened
/// In C++ enumerations might hold values useful in the scope of class. For example
/// the Mezzanine::Exception holds ExceptionCodes that are useful categorizing types
/// of errors. Because each C++ class is represented in Lua as a function that
/// returns a class instance, the syntax "MezzanineSafe.Exception.IO_EXCEPTION" could
/// not work. Enums that are part of a class are instead defined in the same table
/// as the class function and have the class name concatenated to theirs.
/// @n
/// This is different than enums
/// A few example can explain better:
/// C++ code:
/// @code
/// int x = Mezzanine::Exception::IO_EXCEPTION // An enum value on a class
/// int y = Mezzanine::Threading::NotStarted // An enum value in the Threading namespace
/// @endcode
/// @n @n
/// Lua Code:
/// @code
/// x = MezzanineSafe.Exception_IO_EXCEPTION -- flattened to the MezzanineSafe Table/Module
/// y = MezzanineSafe.Threading.NotStarted -- In the Threading table/Module
/// @endcode
/// @n @n
///
///

#include "script.h"
#include "scriptargument.h"
#include "scriptingmanager.h"
#include "scriptworkunit.h"


// Each one of these sections should have a complete implementation of a scripting and
// appropriate bindings for the mezzanine engine.
#ifdef MEZZLUA51
    #include "Lua51/lua51script.h"
    #include "Lua51/lua51scriptargument.h"
    #include "Lua51/lua51scriptingengine.h"
    #include "Lua51/lua51workunit.h"
#endif


#endif // \_scripting_h
