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
#ifndef _lua51tests_h
#define _lua51tests_h

#include "main.h"

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief The group of tests for
class lua51tests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "lua51"
        virtual String Name()
            { return String("lua51"); }

        /// @copydoc Mezzanine::Testing::UnitTestGroup::RunTests
        /// @warning Totally incomplete, when run this will prove nothing.
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                {
                    Scripting::Lua::Lua51ScriptingEngine LuaRuntimeSafe(Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
                    //Scripting::Lua::Lua51ScriptingEngine LuaRuntimeAll(Scripting::Lua::Lua51ScriptingEngine::AllLibs);

                    String WorldWorldSource("print (\"Hello World!\")");

                    // Lua script Default constructor skipped
                    // Skipped FlaggedBuffer completely.

                    //////////////////////////////////////////////////////////////////////////////////////////
                    // Static Data Tests
                    if(String("Lua51ScriptingEngine")==LuaRuntimeSafe.GetImplementationTypeName())
                    {
                        AddTestResult("Lua51::Engine::ImplementationName", Success);
                    }else{
                        AddTestResult("Lua51::Engine::ImplementationName", Failed);
                    }

                    if(541==Scripting::Lua::Lua51ScriptingEngine::DefaultLibs)
                    {
                        AddTestResult("Lua51::Engine::LuaLibEnumDefault", Success);
                    }else{
                        AddTestResult("Lua51::Engine::LuaLibEnumDefault", Failed);
                    }

                    if(511==Scripting::Lua::Lua51ScriptingEngine::AllLibs)
                    {
                        AddTestResult("Lua51::Engine::LuaLibEnumUnsafe", Success);
                    }else{
                        AddTestResult("Lua51::Engine::LuaLibEnumUnsafe", Failed);
                    }

                    //////////////////////////////////////////////////////////////////////////////////////////
                    // Compile Tests
                    try
                    {
                        Scripting::Lua::Lua51Script Hello(WorldWorldSource,&LuaRuntimeSafe); //From pointer
                        AddTestResult("Lua51::Script::ConstructCompilePointer", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Script::ConstructCompilePointer", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe); // from reference
                        AddTestResult("Lua51::Script::ConstructCompileReference", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Script::ConstructCompileReference", Failed);
                    }


                    //////////////////////////////////////////////////////////////////////////////////////////
                    // Execute Tests
                    try
                    {
                        Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(Hello);
                        AddTestResult("Lua51::Engine::ExecuteFromReference", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::ConstructCompileReference", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(&Hello);
                        AddTestResult("Lua51::Engine::ExecuteFromPointer", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::ExecuteFromPointer", Failed);
                    }

                    try
                    {
                        CountedPtr<Scripting::Lua::Lua51Script> Hello(new Scripting::Lua::Lua51Script(WorldWorldSource,LuaRuntimeSafe));
                        LuaRuntimeSafe.Execute(Hello);
                        AddTestResult("Lua51::Engine::ExecuteFromCountedPtr", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::ExecuteFromCountedPtr", Failed);
                    }

                    try
                    {
                        CountedPtr<Scripting::iScript> Hello(new Scripting::Lua::Lua51Script(WorldWorldSource,LuaRuntimeSafe));
                        LuaRuntimeSafe.Execute(Hello);
                        AddTestResult("Lua51::Engine::ExecuteFromCountedPtrCovariant", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::ExecuteFromCountedPtrCovariant", Failed);
                    }

                    try
                    {
                        LuaRuntimeSafe.Execute(WorldWorldSource);
                        AddTestResult("Lua51::Engine::ExecuteFromSource", Success);
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::ExecuteFromSource", Failed);
                    }

                    //////////////////////////////////////////////////////////////////////////////////////////
                    // Argument Tests
                    try
                    {
                        Scripting::Lua::Lua51Script IntArgScript("function TakeArgInt(x)\n"
                                                                 "   return x * 2\n"
                                                                 "end"
                                                                 ,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(IntArgScript);

                        Scripting::Lua::Lua51Script IntArgCall("TakeArgInt",LuaRuntimeSafe,true);
                        //IntArgCall.AddArgument(Scripting::Lua::Lua51IntegerArgument(9));
                        IntArgCall.AddArgument(9);
                        CountedPtr<Scripting::Lua::Lua51IntegerArgument> IntReturn(new Scripting::Lua::Lua51IntegerArgument);
                        IntArgCall.AddReturn(IntReturn);
                        LuaRuntimeSafe.Execute(IntArgCall);

                        if(18==IntReturn->GetInteger())
                        {
                            AddTestResult("Lua51::Engine::PassInt", Success);
                        }else{
                            AddTestResult("Lua51::Engine::PassInt", Failed);
                        }

                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::PassInt", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script WholeArgScript("function TakeArgWhole(x)\n"
                                                                   "   return x * 2\n"
                                                                   "end"
                                                                   ,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(WholeArgScript);

                        Scripting::Lua::Lua51Script WholeArgCall("TakeArgWhole",LuaRuntimeSafe,true);
                        WholeArgCall.AddArgument(9);
                        CountedPtr<Scripting::Lua::Lua51WholeArgument> WholeReturn(new Scripting::Lua::Lua51WholeArgument);
                        WholeArgCall.AddReturn(WholeReturn);
                        LuaRuntimeSafe.Execute(WholeArgCall);

                        if(18==WholeReturn->GetWhole())
                        {
                            AddTestResult("Lua51::Engine::PassWhole", Success);
                        }else{
                            AddTestResult("Lua51::Engine::PassWhole", Failed);
                        }

                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::PassWhole", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script RealArgScript("function TakeArgReal(x)\n"
                                                                  "   return x * 2\n"
                                                                  "end"
                                                                  ,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(RealArgScript);

                        Scripting::Lua::Lua51Script RealArgCall("TakeArgReal",LuaRuntimeSafe,true);
                        RealArgCall.AddArgument(Real(9.5));
                        CountedPtr<Scripting::Lua::Lua51RealArgument> RealReturn(new Scripting::Lua::Lua51RealArgument);
                        RealArgCall.AddReturn(RealReturn);
                        LuaRuntimeSafe.Execute(RealArgCall);

                        if(19.0==RealReturn->GetWhole())
                        {
                            AddTestResult("Lua51::Engine::PassReal", Success);
                        }else{
                            AddTestResult("Lua51::Engine::PassReal", Failed);
                        }
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::PassReal", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script StringArgScript("function TakeArgString(x)\n"
                                                                    "   results = x .. \"World!\"\n"
                                                                    "   return results\n"
                                                                    "end"
                                                                    ,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(StringArgScript);

                        Scripting::Lua::Lua51Script StringArgCall("TakeArgString",LuaRuntimeSafe,true);
                        StringArgCall.AddArgument((char*)"Hello ");
                        CountedPtr<Scripting::Lua::Lua51StringArgument> StringReturn(new Scripting::Lua::Lua51StringArgument);
                        StringArgCall.AddReturn(StringReturn);
                        LuaRuntimeSafe.Execute(StringArgCall);

                        if(String("Hello World!")==StringReturn->GetString())
                        {
                            AddTestResult("Lua51::Engine::PassString", Success);
                        }else{
                            AddTestResult("Lua51::Engine::PassString", Failed);
                        }
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::PassString", Failed);
                    }

                    try
                    {
                        Scripting::Lua::Lua51Script BoolArgScript("function TakeArgBool(x)\n"
                                                                  "   results = not x\n"
                                                                  "   return results\n"
                                                                  "end"
                                                                  ,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(BoolArgScript);

                        Scripting::Lua::Lua51Script BoolArgCall("TakeArgBool",LuaRuntimeSafe,true);
                        BoolArgCall.AddArgument(false);
                        CountedPtr<Scripting::Lua::Lua51BoolArgument> BoolReturn(new Scripting::Lua::Lua51BoolArgument);
                        BoolArgCall.AddReturn(BoolReturn);
                        LuaRuntimeSafe.Execute(BoolArgCall);

                        if(true==BoolReturn->GetBool())
                        {
                            AddTestResult("Lua51::Engine::PassBool", Success);
                        }else{
                            AddTestResult("Lua51::Engine::PassBool", Failed);
                        }
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::PassBool", Failed);
                    }


                    try
                    {
                        Scripting::Lua::Lua51Script NilArgScript("function TakeArgNil(x)\n"
                                                                 "   if \"nil\"==type(x) then\n"
                                                                 "      return nil\n"
                                                                 "   else\n"
                                                                 "      return 10\n"
                                                                 "   end\n"
                                                                 "end"
                                                                 ,LuaRuntimeSafe);
                        LuaRuntimeSafe.Execute(NilArgScript);

                        Scripting::Lua::Lua51Script NilArgCall("TakeArgNil",LuaRuntimeSafe,true);
                        NilArgCall.AddArgument();
                        CountedPtr<Scripting::Lua::Lua51NilArgument> NilReturn(new Scripting::Lua::Lua51NilArgument);
                        NilArgCall.AddReturn(NilReturn);
                        LuaRuntimeSafe.Execute(NilArgCall);

                        if(false==NilReturn->GetBool())
                        {
                            AddTestResult("Lua51::Engine::PassNil", Success);
                        }else{
                            AddTestResult("Lua51::Engine::PassNil", Failed);
                        }
                    } catch (ScriptLuaException& e) {
                        AddTestResult("Lua51::Engine::PassNil", Failed);
                    }
                }

                {
                    //////////////////////////////////////////////////////////////////////////////////////////
                    // Library Tests
                    {
                        String FeatureSource("assert(1)\n"
                                             "print(type(tostring(1)))\n"
                                             "coroutine.running()\n"
                                            );

                        Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                        Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                        cout << "Attempting execution of Base library function without that library being loaded." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::BaselibExclude", Failed); // Why does this work?
                        } catch (ScriptLuaException& e) {
                            cout << endl << "It failed as it should." << endl;
                            AddTestResult("Lua51::Script::BaselibExclude", Success);
                        }

                        LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::BaseLib);
                        //LuaRuntimePartial.OpenBaseLibrary();

                        cout << "Attempting normal execution of properly loaded Base library function." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::BaselibInclude", Success);
                        } catch (ScriptLuaException& e) {
                            AddTestResult("Lua51::Script::BaselibInclude", Failed);
                        }
                        cout << endl;
                    }

                    /*{
                        String FeatureSource("module(EmptyModule)\n");

                        Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::BaseLib);
                        Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                        cout << "Attempting execution of Package library function without that library being loaded." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::PackagelibExclude", Failed); // Why does this work?
                        } catch (ScriptLuaException& e) {
                            cout << end << "It failed as it should." << endl;
                            AddTestResult("Lua51::Script::PackagelibExclude", Success);
                        }

                        LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::PackageLib);
                        //LuaRuntimePartial.OpenPackageLibrary(); //identical to above line

                        cout << "Attempting normal execution of properly loaded Package library function." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::PackagelibInclude", Success);
                        } catch (ScriptLuaException& e) {
                            AddTestResult("Lua51::Script::PackagelibInclude", Failed);
                        }
                        cout << endl;
                    }*/

                    {
                        String FeatureSource("x = string.lower(\"A\")");

                        Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                        Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                        cout << "Attempting execution of String library function without that library being loaded." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::StringlibExclude", Failed); // Why does this work?
                        } catch (ScriptLuaException& e) {
                            cout << endl << "It failed as it should." << endl;
                            AddTestResult("Lua51::Script::StringlibExclude", Success);
                        }

                        LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::StringLib);
                        //LuaRuntimePartial.OpenStringLibrary();

                        cout << "Attempting normal execution of properly loaded String library function." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::StringlibInclude", Success);
                        } catch (ScriptLuaException& e) {
                            AddTestResult("Lua51::Script::StringlibInclude", Failed);
                        }
                        cout << endl;
                    }


                    {
                        String FeatureSource("num = {1,2,3}\n"
                                             "table.maxn(num)");

                        Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                        Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                        cout << "Attempting execution of Table library function without that library being loaded." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::TablelibExclude", Failed); // Why does this work?
                        } catch (ScriptLuaException& e) {
                            cout << endl << "It failed as it should." << endl;
                            AddTestResult("Lua51::Script::TablelibExclude", Success);
                        }

                        LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::TableLib);
                        //LuaRuntimePartial.OpenTableLibrary();

                        cout << "Attempting normal execution of properly loaded Table library function." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::TablelibInclude", Success);
                        } catch (ScriptLuaException& e) {
                            AddTestResult("Lua51::Script::TablelibInclude", Failed);
                        }
                        cout << endl;
                    }

                    {
                        String FeatureSource("x = math.abs(-10)");

                        Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                        Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                        cout << "Attempting execution of Math library function without that library being loaded." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::MathlibExclude", Failed); // Why does this work?
                        } catch (ScriptLuaException& e) {
                            cout << endl << "It failed as it should." << endl;
                            AddTestResult("Lua51::Script::MathlibExclude", Success);
                        }

                        LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::MathLib);
                        //LuaRuntimePartial.OpenMathLibrary();

                        cout << "Attempting normal execution of properly loaded Math library function." << endl;
                        try
                        {
                            FeatureScript.Compile(LuaRuntimePartial);
                            LuaRuntimePartial.Execute(FeatureScript);
                            AddTestResult("Lua51::Script::MathlibInclude", Success);
                        } catch (ScriptLuaException& e) {
                            AddTestResult("Lua51::Script::MathlibInclude", Failed);
                        }
                        cout << endl;
                    }

                    /*
                    virtual void OpenIOLibrary();

                    /// @detail See http://www.lua.org/manual/5.1/manual.html#5.7 in the Lua manual for details

                    virtual void OpenOSLibrary();

                    /// Lua manual at http://www.lua.org/manual/5.1/manual.html#5.9 .
                    virtual void OpenDebugLibrary();


                    virtual void OpenMezzanineLibrary();

                    virtual void OpenMezzanineSafeLibrary();
                    */

                    cout << endl;
                }


            }else{
                AddTestResult("Lua51::Engine::ImplementationName", Skipped);
                AddTestResult("Lua51::Engine::LuaLibEnumDefault", Skipped);
                AddTestResult("Lua51::Engine::LuaLibEnumUnsafe", Skipped);

                AddTestResult("Lua51::Script::ConstructCompilePointer", Skipped);
                AddTestResult("Lua51::Script::ConstructCompileReference", Skipped);
                AddTestResult("Lua51::Script::ConstructCompileCountedPtr", Skipped);

                AddTestResult("Lua51::Engine::ExecuteFromReference", Skipped);
                AddTestResult("Lua51::Engine::ExecuteFromPointer", Skipped);
                AddTestResult("Lua51::Engine::ExecuteFromCountedPtr", Skipped);
                AddTestResult("Lua51::Engine::ExecuteFromSource", Skipped);
                AddTestResult("Lua51::Engine::ExecuteFromCountedPtrCovariant", Skipped);

                AddTestResult("Lua51::Engine::PassInt", Skipped);
                AddTestResult("Lua51::Engine::PassWhole", Skipped);
                AddTestResult("Lua51::Engine::PassReal", Skipped);
                AddTestResult("Lua51::Engine::PassString", Skipped);
                AddTestResult("Lua51::Engine::PassBool", Skipped);
                AddTestResult("Lua51::Engine::PassNil", Skipped);

                AddTestResult("Lua51::Script::BaselibExclude", Skipped);
                AddTestResult("Lua51::Script::BaselibInclude", Skipped);
                //AddTestResult("Lua51::Script::PackagelibExclude", Skipped);
                //AddTestResult("Lua51::Script::PackagelibInclude", Skipped);
                AddTestResult("Lua51::Script::StringlibExclude", Skipped);
                AddTestResult("Lua51::Script::StringlibInclude", Skipped);
                AddTestResult("Lua51::Script::TablelibExclude", Skipped);
                AddTestResult("Lua51::Script::TablelibInclude", Skipped);
                AddTestResult("Lua51::Script::MathlibExclude", Skipped);
                AddTestResult("Lua51::Script::MathlibInclude", Skipped);
            }
        }
};

#endif

